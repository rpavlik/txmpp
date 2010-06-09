#ifndef _XMPPPUMP_H_
#define _XMPPPUMP_H_

#include <txmpp/base/messagequeue.h>
#include <txmpp/base/taskrunner.h>
#include <txmpp/base/thread.h>
#include <txmpp/base/time.h>
#include <txmpp/xmpp/xmppclient.h>
#include <txmpp/xmpp/xmppengine.h>
#include <txmpp/xmpp/xmpptask.h>

// Simple xmpp pump

class XmppPumpNotify {
public:
  virtual ~XmppPumpNotify() {}
  virtual void OnStateChange(txmpp::XmppEngine::State state) = 0;
};

class XmppPump : public txmpp::MessageHandler, public txmpp::TaskRunner {
public:
  XmppPump(XmppPumpNotify * notify = NULL);

  txmpp::XmppClient *client() { return client_; }

  void DoLogin(const txmpp::XmppClientSettings & xcs,
               txmpp::XmppAsyncSocket* socket,
               txmpp::PreXmppAuth* auth);
  void DoDisconnect();

  void OnStateChange(txmpp::XmppEngine::State state);

  void WakeTasks();

  int64 CurrentTime();

  void OnMessage(txmpp::Message *pmsg);

  txmpp::XmppReturnStatus SendStanza(const txmpp::XmlElement *stanza);

private:
  txmpp::XmppClient *client_;
  txmpp::XmppEngine::State state_;
  XmppPumpNotify *notify_;
};

#endif // _XMPPPUMP_H_
