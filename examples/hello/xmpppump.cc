#include "xmpppump.h"

#include <txmpp/base/logging.h>
#include "xmppauth.h"
#include "xmpptasks.h"

XmppPump::XmppPump(XmppPumpNotify * notify) {
  state_ = txmpp::XmppEngine::STATE_NONE;
  notify_ = notify;
  client_ = new txmpp::XmppClient(this);  // NOTE: deleted by TaskRunner
}

void XmppPump::DoLogin(const txmpp::XmppClientSettings & xcs,
                       txmpp::XmppAsyncSocket* socket,
                       txmpp::PreXmppAuth* auth) {
  OnStateChange(txmpp::XmppEngine::STATE_START);
  if (!AllChildrenDone()) {
    client_->SignalStateChange.connect(this, &XmppPump::OnStateChange);
    if (client_->Connect(xcs, "", socket, auth) != txmpp::XMPP_RETURN_OK) {
      LOG(LS_ERROR) << "Failed to connect.";
    }
    client_->Start();

    // Owned by client_
    XmppTaskMessage *task_message = new XmppTaskMessage(client_);
    task_message->Start();
  }
}

void XmppPump::DoDisconnect() {
  if (!AllChildrenDone())
    client_->Disconnect();
  OnStateChange(txmpp::XmppEngine::STATE_CLOSED);
}

void XmppPump::OnStateChange(txmpp::XmppEngine::State state) {
  if (state_ == state)
    return;
  if (state == txmpp::XmppEngine::STATE_OPEN)
    client_->SendRaw("<presence/>");
  state_ = state;
  if (notify_ != NULL)
    notify_->OnStateChange(state);
}

void XmppPump::WakeTasks() {
  txmpp::Thread::Current()->Post(this);
}

int64 XmppPump::CurrentTime() {
  return (int64)txmpp::Time();
}

void XmppPump::OnMessage(txmpp::Message *pmsg) {
  RunTasks();
}

txmpp::XmppReturnStatus XmppPump::SendStanza(const txmpp::XmlElement *stanza) {
  if (!AllChildrenDone())
    return client_->SendStanza(stanza);
  return txmpp::XMPP_RETURN_BADSTATE;
}
