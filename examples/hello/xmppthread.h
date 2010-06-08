#ifndef _XMPPTHREAD_H_
#define _XMPPTHREAD_H_

#include <talk/base/thread.h>
#include <talk/xmpp/xmppclientsettings.h>
#include "xmpppump.h"
#include "xmppsocket.h"

class XmppThread:
    public talk_base::Thread, XmppPumpNotify, talk_base::MessageHandler {
public:
  XmppThread();
  ~XmppThread();

  buzz::XmppClient* client() { return pump_->client(); }

  void ProcessMessages(int cms);

  void Login(const buzz::XmppClientSettings & xcs);
  void Disconnect();

private:
  XmppPump* pump_;

  void OnStateChange(buzz::XmppEngine::State state);
  void OnMessage(talk_base::Message* pmsg);
};

#endif // _XMPPTHREAD_H_
