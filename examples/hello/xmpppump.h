#ifndef _XMPPPUMP_H_
#define _XMPPPUMP_H_

#include "talk/base/messagequeue.h"
#include "talk/base/taskrunner.h"
#include "talk/base/thread.h"
#include "talk/base/time.h"
#include "talk/xmpp/xmppclient.h"
#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmpptask.h"

// Simple xmpp pump

class XmppPumpNotify {
public:
  virtual ~XmppPumpNotify() {}
  virtual void OnStateChange(buzz::XmppEngine::State state) = 0;
};

class XmppPump : public talk_base::MessageHandler, public talk_base::TaskRunner {
public:
  XmppPump(XmppPumpNotify * notify = NULL);

  buzz::XmppClient *client() { return client_; }

  void DoLogin(const buzz::XmppClientSettings & xcs,
               buzz::AsyncSocket* socket,
               buzz::PreXmppAuth* auth);
  void DoDisconnect();

  void OnStateChange(buzz::XmppEngine::State state);

  void WakeTasks();

  int64 CurrentTime();

  void OnMessage(talk_base::Message *pmsg);

  buzz::XmppReturnStatus SendStanza(const buzz::XmlElement *stanza);

private:
  buzz::XmppClient *client_;
  buzz::XmppEngine::State state_;
  XmppPumpNotify *notify_;
};

#endif // _XMPPPUMP_H_
