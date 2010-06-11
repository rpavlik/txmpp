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

#ifndef _TXMPP_XMPPASYNCSOCKETIMPL_H_
#define _TXMPP_XMPPASYNCSOCKETIMPL_H_

#ifndef NO_CONFIG_H
#include "config.h"
#endif

#include "asyncsocket.h"
#include "bytebuffer.h"
#include "sigslot.h"
#include "xmppasyncsocket.h"

// The below define selects the SSLStreamAdapter implementation for
// SSL, as opposed to the SSLAdapter socket adapter.
// #define USE_SSLSTREAM

namespace txmpp {

class StreamInterface;

extern AsyncSocket* cricket_socket_;

class XmppAsyncSocketImpl : public XmppAsyncSocket, public has_slots<> {
  public:
    XmppAsyncSocketImpl(bool tls);
    ~XmppAsyncSocketImpl();

    virtual XmppAsyncSocket::State state();
    virtual XmppAsyncSocket::Error error();
    virtual int GetError();

    virtual bool Connect(const SocketAddress& addr);
    virtual bool Read(char * data, size_t len, size_t* len_read);
    virtual bool Write(const char * data, size_t len);
    virtual bool Close();
    virtual bool StartTls(const std::string & domainname);

    signal1<int> SignalCloseEvent;

  private:
#ifndef USE_SSLSTREAM
    void OnReadEvent(AsyncSocket * socket);
    void OnWriteEvent(AsyncSocket * socket);
    void OnConnectEvent(AsyncSocket * socket);
    void OnCloseEvent(AsyncSocket * socket, int error);
#else  // USE_SSLSTREAM
    void OnEvent(StreamInterface* stream, int events, int err);
#endif  // USE_SSLSTREAM

    AsyncSocket * cricket_socket_;
#ifdef USE_SSLSTREAM
    StreamInterface *stream_;
#endif  // USE_SSLSTREAM
    XmppAsyncSocket::State state_;
    ByteBuffer buffer_;
    bool tls_;
};

}  // namespace txmpp

#endif  // _TXMPP_XMPPASYNCSOCKETIMPL_H_
