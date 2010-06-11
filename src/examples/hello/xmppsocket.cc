#include "xmppsocket.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <errno.h>
#include "../../basicdefs.h"
#include "../../logging.h"
#ifdef FEATURE_ENABLE_SSL
#include "../../ssladapter.h"
#endif
#ifdef USE_SSLSTREAM
#include "../../socketstream.h"
#ifdef FEATURE_ENABLE_SSL
#include "../../sslstreamadapter.h"
#endif  // FEATURE_ENABLE_SSL
#endif  // USE_SSLSTREAM
#include "../../thread.h"

namespace hello {

XmppSocket::XmppSocket(bool tls) : tls_(tls) {
  txmpp::Thread* pth = txmpp::Thread::Current();
  txmpp::AsyncSocket* socket =
    pth->socketserver()->CreateAsyncSocket(SOCK_STREAM);
#ifndef USE_SSLSTREAM
#ifdef FEATURE_ENABLE_SSL
  if (tls_) {
    socket = txmpp::SSLAdapter::Create(socket);
    txmpp::InitializeSSL();
  }
#endif  // FEATURE_ENABLE_SSL
  cricket_socket_ = socket;
  cricket_socket_->SignalReadEvent.connect(this, &XmppSocket::OnReadEvent);
  cricket_socket_->SignalWriteEvent.connect(this, &XmppSocket::OnWriteEvent);
  cricket_socket_->SignalConnectEvent.connect(this,
                                              &XmppSocket::OnConnectEvent);
  cricket_socket_->SignalCloseEvent.connect(this, &XmppSocket::OnCloseEvent);
#else  // USE_SSLSTREAM
  cricket_socket_ = socket;
  stream_ = new txmpp::SocketStream(cricket_socket_);
#ifdef FEATURE_ENABLE_SSL
  if (tls_) {
    stream_ = txmpp::SSLStreamAdapter::Create(stream_);
    txmpp::InitializeSSL();
  }
#endif  // FEATURE_ENABLE_SSL
  stream_->SignalEvent.connect(this, &XmppSocket::OnEvent);
#endif  // USE_SSLSTREAM

  state_ = txmpp::XmppAsyncSocket::STATE_CLOSED;
}

XmppSocket::~XmppSocket() {
  txmpp::CleanupSSL();
  Close();
#ifndef USE_SSLSTREAM
  delete cricket_socket_;
#else  // USE_SSLSTREAM
  delete stream_;
#endif  // USE_SSLSTREAM
}

#ifndef USE_SSLSTREAM
void XmppSocket::OnReadEvent(txmpp::AsyncSocket * socket) {
  SignalRead();
}

void XmppSocket::OnWriteEvent(txmpp::AsyncSocket * socket) {
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

void XmppSocket::OnConnectEvent(txmpp::AsyncSocket * socket) {
#if defined(FEATURE_ENABLE_SSL)
  if (state_ == txmpp::XmppAsyncSocket::STATE_TLS_CONNECTING) {
    state_ = txmpp::XmppAsyncSocket::STATE_TLS_OPEN;
    SignalSSLConnected();
    OnWriteEvent(cricket_socket_);
    return;
  }
#endif  // !defined(FEATURE_ENABLE_SSL)
  state_ = txmpp::XmppAsyncSocket::STATE_OPEN;
  SignalConnected();
}

void XmppSocket::OnCloseEvent(txmpp::AsyncSocket * socket, int error) {
  SignalCloseEvent(error);
}

#else  // USE_SSLSTREAM

void XmppSocket::OnEvent(txmpp::StreamInterface* stream,
                         int events, int err) {
  if ((events & txmpp::SE_OPEN)) {
#if defined(FEATURE_ENABLE_SSL)
    if (state_ == txmpp::XmppAsyncSocket::STATE_TLS_CONNECTING) {
      state_ = txmpp::XmppAsyncSocket::STATE_TLS_OPEN;
      SignalSSLConnected();
      events |= txmpp::SE_WRITE;
    } else
#endif
    {
      state_ = txmpp::XmppAsyncSocket::STATE_OPEN;
      SignalConnected();
    }
  }
  if ((events & txmpp::SE_READ))
    SignalRead();
  if ((events & txmpp::SE_WRITE)) {
    // Write bytes if there are any
    while (buffer_.Length() != 0) {
      txmpp::StreamResult result;
      size_t written;
      int error;
      result = stream_->Write(buffer_.Data(), buffer_.Length(),
                              &written, &error);
      if (result == txmpp::SR_ERROR) {
        LOG(LS_ERROR) << "Send error: " << error;
        return;
      }
      if (result == txmpp::SR_BLOCK)
        return;
      ASSERT(result == txmpp::SR_SUCCESS);
      ASSERT(written > 0);
      buffer_.Shift(written);
    }
  }
  if ((events & txmpp::SE_CLOSE))
    SignalCloseEvent(err);
}
#endif  // USE_SSLSTREAM

txmpp::XmppAsyncSocket::State XmppSocket::state() {
  return state_;
}

txmpp::XmppAsyncSocket::Error XmppSocket::error() {
  return txmpp::XmppAsyncSocket::ERROR_NONE;
}

int XmppSocket::GetError() {
  return 0;
}

bool XmppSocket::Connect(const txmpp::SocketAddress& addr) {
  if (cricket_socket_->Connect(addr) < 0) {
    return cricket_socket_->IsBlocking();
  }
  return true;
}

bool XmppSocket::Read(char * data, size_t len, size_t* len_read) {
#ifndef USE_SSLSTREAM
  int read = cricket_socket_->Recv(data, len);
  if (read > 0) {
    *len_read = (size_t)read;
    return true;
  }
#else  // USE_SSLSTREAM
  txmpp::StreamResult result = stream_->Read(data, len, len_read, NULL);
  if (result == txmpp::SR_SUCCESS)
    return true;
#endif  // USE_SSLSTREAM
  return false;
}

bool XmppSocket::Write(const char * data, size_t len) {
  buffer_.WriteBytes(data, len);
#ifndef USE_SSLSTREAM
  OnWriteEvent(cricket_socket_);
#else  // USE_SSLSTREAM
  OnEvent(stream_, txmpp::SE_WRITE, 0);
#endif  // USE_SSLSTREAM
  return true;
}

bool XmppSocket::Close() {
  if (state_ != txmpp::XmppAsyncSocket::STATE_OPEN)
    return false;
#ifndef USE_SSLSTREAM
  if (cricket_socket_->Close() == 0) {
    state_ = txmpp::XmppAsyncSocket::STATE_CLOSED;
    SignalClosed();
    return true;
  }
  return false;
#else  // USE_SSLSTREAM
  state_ = txmpp::XmppAsyncSocket::STATE_CLOSED;
  stream_->Close();
  SignalClosed();
  return true;
#endif  // USE_SSLSTREAM
}

bool XmppSocket::StartTls(const std::string & domainname) {
#if defined(FEATURE_ENABLE_SSL)
  if (!tls_)
    return false;
#ifndef USE_SSLSTREAM
  txmpp::SSLAdapter* ssl_adapter =
    static_cast<txmpp::SSLAdapter *>(cricket_socket_);
  ssl_adapter->set_ignore_bad_cert(true);
  if (ssl_adapter->StartSSL(domainname.c_str(), false) != 0)
    return false;
#else  // USE_SSLSTREAM
  txmpp::SSLStreamAdapter* ssl_stream =
    static_cast<txmpp::SSLStreamAdapter *>(stream_);
  ssl_stream->set_ignore_bad_cert(true);
  if (ssl_stream->StartSSLWithServer(domainname.c_str()) != 0)
    return false;
#endif  // USE_SSLSTREAM
  state_ = txmpp::XmppAsyncSocket::STATE_TLS_CONNECTING;
  return true;
#else  // !defined(FEATURE_ENABLE_SSL)
  return false;
#endif  // !defined(FEATURE_ENABLE_SSL)
}

}  // namespace hello
