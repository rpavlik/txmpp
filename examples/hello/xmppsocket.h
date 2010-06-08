#ifndef _XMPPSOCKET_H_
#define _XMPPSOCKET_H_

#include <talk/base/asyncsocket.h>
#include <talk/base/bytebuffer.h>
#include <talk/base/sigslot.h>
#include <talk/xmpp/asyncsocket.h>

// The below define selects the SSLStreamAdapter implementation for
// SSL, as opposed to the SSLAdapter socket adapter.
// #define USE_SSLSTREAM 

namespace talk_base {
  class StreamInterface;
};
extern talk_base::AsyncSocket* cricket_socket_;

class XmppSocket : public buzz::AsyncSocket, public sigslot::has_slots<> {
public:
  XmppSocket(bool tls);
  ~XmppSocket();

  virtual buzz::AsyncSocket::State state();
  virtual buzz::AsyncSocket::Error error();
  virtual int GetError();

  virtual bool Connect(const talk_base::SocketAddress& addr);
  virtual bool Read(char * data, size_t len, size_t* len_read);
  virtual bool Write(const char * data, size_t len);
  virtual bool Close();
  virtual bool StartTls(const std::string & domainname);

  sigslot::signal1<int> SignalCloseEvent;

private:
#ifndef USE_SSLSTREAM
  void OnReadEvent(talk_base::AsyncSocket * socket);
  void OnWriteEvent(talk_base::AsyncSocket * socket);
  void OnConnectEvent(talk_base::AsyncSocket * socket);
  void OnCloseEvent(talk_base::AsyncSocket * socket, int error);
#else  // USE_SSLSTREAM
  void OnEvent(talk_base::StreamInterface* stream, int events, int err);
#endif  // USE_SSLSTREAM

  talk_base::AsyncSocket * cricket_socket_;
#ifdef USE_SSLSTREAM
  talk_base::StreamInterface *stream_;
#endif  // USE_SSLSTREAM
  buzz::AsyncSocket::State state_;
  talk_base::ByteBuffer buffer_;
  bool tls_;
};

#endif // _XMPPSOCKET_H_
