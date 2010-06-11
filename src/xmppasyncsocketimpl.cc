/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "xmppasyncsocketimpl.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <errno.h>
#include "basicdefs.h"
#include "logging.h"
#ifdef FEATURE_ENABLE_SSL
#include "ssladapter.h"
#endif
#ifdef USE_SSLSTREAM
#include "socketstream.h"
#ifdef FEATURE_ENABLE_SSL
#include "sslstreamadapter.h"
#endif  // FEATURE_ENABLE_SSL
#endif  // USE_SSLSTREAM
#include "thread.h"

namespace txmpp {

XmppAsyncSocketImpl::XmppAsyncSocketImpl(bool tls) : tls_(tls) {
  Thread* pth = Thread::Current();
  AsyncSocket* socket = pth->socketserver()->CreateAsyncSocket(SOCK_STREAM);
#ifndef USE_SSLSTREAM
#ifdef FEATURE_ENABLE_SSL
  if (tls_) {
    socket = SSLAdapter::Create(socket);
    InitializeSSL();
  }
#endif  // FEATURE_ENABLE_SSL
  cricket_socket_ = socket;
  cricket_socket_->SignalReadEvent.connect(
      this, &XmppAsyncSocketImpl::OnReadEvent);
  cricket_socket_->SignalWriteEvent.connect(
      this, &XmppAsyncSocketImpl::OnWriteEvent);
  cricket_socket_->SignalConnectEvent.connect(
      this, &XmppAsyncSocketImpl::OnConnectEvent);
  cricket_socket_->SignalCloseEvent.connect(
      this, &XmppAsyncSocketImpl::OnCloseEvent);
#else  // USE_SSLSTREAM
  cricket_socket_ = socket;
  stream_ = new SocketStream(cricket_socket_);
#ifdef FEATURE_ENABLE_SSL
  if (tls_) {
    stream_ = SSLStreamAdapter::Create(stream_);
    InitializeSSL();
  }
#endif  // FEATURE_ENABLE_SSL
  stream_->SignalEvent.connect(this, &XmppAsyncSocketImpl::OnEvent);
#endif  // USE_SSLSTREAM

  state_ = XmppAsyncSocket::STATE_CLOSED;
}

XmppAsyncSocketImpl::~XmppAsyncSocketImpl() {
  CleanupSSL();
  Close();
#ifndef USE_SSLSTREAM
  delete cricket_socket_;
#else  // USE_SSLSTREAM
  delete stream_;
#endif  // USE_SSLSTREAM
}

#ifndef USE_SSLSTREAM
void XmppAsyncSocketImpl::OnReadEvent(AsyncSocket * socket) {
  SignalRead();
}

void XmppAsyncSocketImpl::OnWriteEvent(AsyncSocket * socket) {
  // Write bytes if there are any
  while (buffer_.Length() != 0) {
    int written = cricket_socket_->Send(buffer_.Data(), buffer_.Length());
    if (written > 0) {
      buffer_.Shift(written);
      continue;
    }
    if (!cricket_socket_->IsBlocking())
      LOG(LS_ERROR) << "Send error: " << cricket_socket_->GetError();
    return;
  }
}

void XmppAsyncSocketImpl::OnConnectEvent(AsyncSocket * socket) {
#if defined(FEATURE_ENABLE_SSL)
  if (state_ == XmppAsyncSocket::STATE_TLS_CONNECTING) {
    state_ = XmppAsyncSocket::STATE_TLS_OPEN;
    SignalSSLConnected();
    OnWriteEvent(cricket_socket_);
    return;
  }
#endif  // !defined(FEATURE_ENABLE_SSL)
  state_ = XmppAsyncSocket::STATE_OPEN;
  SignalConnected();
}

void XmppAsyncSocketImpl::OnCloseEvent(AsyncSocket * socket, int error) {
  SignalCloseEvent(error);
}

#else  // USE_SSLSTREAM

void XmppAsyncSocketImpl::OnEvent(StreamInterface* stream,
                         int events, int err) {
  if ((events & SE_OPEN)) {
#if defined(FEATURE_ENABLE_SSL)
    if (state_ == XmppAsyncSocket::STATE_TLS_CONNECTING) {
      state_ = XmppAsyncSocket::STATE_TLS_OPEN;
      SignalSSLConnected();
      events |= SE_WRITE;
    } else
#endif
    {
      state_ = XmppAsyncSocket::STATE_OPEN;
      SignalConnected();
    }
  }
  if ((events & SE_READ))
    SignalRead();
  if ((events & SE_WRITE)) {
    // Write bytes if there are any
    while (buffer_.Length() != 0) {
      StreamResult result;
      size_t written;
      int error;
      result = stream_->Write(buffer_.Data(), buffer_.Length(),
                              &written, &error);
      if (result == SR_ERROR) {
        LOG(LS_ERROR) << "Send error: " << error;
        return;
      }
      if (result == SR_BLOCK)
        return;
      ASSERT(result == SR_SUCCESS);
      ASSERT(written > 0);
      buffer_.Shift(written);
    }
  }
  if ((events & SE_CLOSE))
    SignalCloseEvent(err);
}
#endif  // USE_SSLSTREAM

XmppAsyncSocket::State XmppAsyncSocketImpl::state() {
  return state_;
}

XmppAsyncSocket::Error XmppAsyncSocketImpl::error() {
  return XmppAsyncSocket::ERROR_NONE;
}

int XmppAsyncSocketImpl::GetError() {
  return 0;
}

bool XmppAsyncSocketImpl::Connect(const SocketAddress& addr) {
  if (cricket_socket_->Connect(addr) < 0) {
    return cricket_socket_->IsBlocking();
  }
  return true;
}

bool XmppAsyncSocketImpl::Read(char * data, size_t len, size_t* len_read) {
#ifndef USE_SSLSTREAM
  int read = cricket_socket_->Recv(data, len);
  if (read > 0) {
    *len_read = (size_t)read;
    return true;
  }
#else  // USE_SSLSTREAM
  StreamResult result = stream_->Read(data, len, len_read, NULL);
  if (result == SR_SUCCESS)
    return true;
#endif  // USE_SSLSTREAM
  return false;
}

bool XmppAsyncSocketImpl::Write(const char * data, size_t len) {
  buffer_.WriteBytes(data, len);
#ifndef USE_SSLSTREAM
  OnWriteEvent(cricket_socket_);
#else  // USE_SSLSTREAM
  OnEvent(stream_, SE_WRITE, 0);
#endif  // USE_SSLSTREAM
  return true;
}

bool XmppAsyncSocketImpl::Close() {
  if (state_ != XmppAsyncSocket::STATE_OPEN)
    return false;
#ifndef USE_SSLSTREAM
  if (cricket_socket_->Close() == 0) {
    state_ = XmppAsyncSocket::STATE_CLOSED;
    SignalClosed();
    return true;
  }
  return false;
#else  // USE_SSLSTREAM
  state_ = XmppAsyncSocket::STATE_CLOSED;
  stream_->Close();
  SignalClosed();
  return true;
#endif  // USE_SSLSTREAM
}

bool XmppAsyncSocketImpl::StartTls(const std::string & domainname) {
#if defined(FEATURE_ENABLE_SSL)
  if (!tls_)
    return false;
#ifndef USE_SSLSTREAM
  SSLAdapter* ssl_adapter =
    static_cast<SSLAdapter *>(cricket_socket_);
  ssl_adapter->set_ignore_bad_cert(true);
  if (ssl_adapter->StartSSL(domainname.c_str(), false) != 0)
    return false;
#else  // USE_SSLSTREAM
  SSLStreamAdapter* ssl_stream =
    static_cast<SSLStreamAdapter *>(stream_);
  ssl_stream->set_ignore_bad_cert(true);
  if (ssl_stream->StartSSLWithServer(domainname.c_str()) != 0)
    return false;
#endif  // USE_SSLSTREAM
  state_ = XmppAsyncSocket::STATE_TLS_CONNECTING;
  return true;
#else  // !defined(FEATURE_ENABLE_SSL)
  return false;
#endif  // !defined(FEATURE_ENABLE_SSL)
}

}  // namespace txmpp
