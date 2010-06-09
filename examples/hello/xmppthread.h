#ifndef _XMPPTHREAD_H_
#define _XMPPTHREAD_H_

#include <txmpp/base/thread.h>
#include <txmpp/xmpp/xmppclientsettings.h>
#include "xmpppump.h"
#include "xmppsocket.h"

class XmppThread:
    public txmpp::Thread, XmppPumpNotify, txmpp::MessageHandler {
public:
  XmppThread();
  ~XmppThread();

  txmpp::XmppClient* client() { return pump_->client(); }

  void ProcessMessages(int cms);

  void Login(const txmpp::XmppClientSettings & xcs);
  void Disconnect();

private:
  XmppPump* pump_;

  void OnStateChange(txmpp::XmppEngine::State state);
  void OnMessage(txmpp::Message* pmsg);
};

#endif // _XMPPTHREAD_H_
