#ifndef _HELLO_XMPPSOCKET_H_
#define _HELLO_XMPPSOCKET_H_

#include "../../asyncsocket.h"
#include "../../bytebuffer.h"
#include "../../sigslot.h"
#include "../../xmppasyncsocket.h"

// The below define selects the SSLStreamAdapter implementation for
// SSL, as opposed to the SSLAdapter socket adapter.
// #define USE_SSLSTREAM 

namespace txmpp {
  class StreamInterface;
};
extern txmpp::AsyncSocket* cricket_socket_;

namespace hello {

class XmppSocket : public txmpp::XmppAsyncSocket, public txmpp::has_slots<> {
  public:
    XmppSocket(bool tls);
    ~XmppSocket();

    virtual txmpp::XmppAsyncSocket::State state();
    virtual txmpp::XmppAsyncSocket::Error error();
    virtual int GetError();

    virtual bool Connect(const txmpp::SocketAddress& addr);
    virtual bool Read(char * data, size_t len, size_t* len_read);
    virtual bool Write(const char * data, size_t len);
    virtual bool Close();
    virtual bool StartTls(const std::string & domainname);

    txmpp::signal1<int> SignalCloseEvent;

  private:
#ifndef USE_SSLSTREAM
    void OnReadEvent(txmpp::AsyncSocket * socket);
    void OnWriteEvent(txmpp::AsyncSocket * socket);
    void OnConnectEvent(txmpp::AsyncSocket * socket);
    void OnCloseEvent(txmpp::AsyncSocket * socket, int error);
#else  // USE_SSLSTREAM
    void OnEvent(txmpp::StreamInterface* stream, int events, int err);
#endif  // USE_SSLSTREAM

    txmpp::AsyncSocket * cricket_socket_;
#ifdef USE_SSLSTREAM
    txmpp::StreamInterface *stream_;
#endif  // USE_SSLSTREAM
    txmpp::XmppAsyncSocket::State state_;
    txmpp::ByteBuffer buffer_;
    bool tls_;
};

}

#endif  // _HELLO_XMPPSOCKET_H_
