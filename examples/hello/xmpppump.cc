#include "talk/base/logging.h"
#include "xmpppump.h"
#include "xmppauth.h"

XmppPump::XmppPump(XmppPumpNotify * notify) {
  state_ = buzz::XmppEngine::STATE_NONE;
  notify_ = notify;
  client_ = new buzz::XmppClient(this);  // NOTE: deleted by TaskRunner
}

void XmppPump::DoLogin(const buzz::XmppClientSettings & xcs,
                       buzz::AsyncSocket* socket,
                       buzz::PreXmppAuth* auth) {
  OnStateChange(buzz::XmppEngine::STATE_START);
  if (!AllChildrenDone()) {
    client_->SignalStateChange.connect(this, &XmppPump::OnStateChange);
    if (client_->Connect(xcs, "", socket, auth) != buzz::XMPP_RETURN_OK) {
      LOG(LS_ERROR) << "Failed to connect.";
    }
    client_->Start();
  }
}

void XmppPump::DoDisconnect() {
  if (!AllChildrenDone())
    client_->Disconnect();
  OnStateChange(buzz::XmppEngine::STATE_CLOSED);
}

void XmppPump::OnStateChange(buzz::XmppEngine::State state) {
  if (state_ == state)
    return;
  state_ = state;
  if (notify_ != NULL)
    notify_->OnStateChange(state);
}

void XmppPump::WakeTasks() {
  talk_base::Thread::Current()->Post(this);
}

int64 XmppPump::CurrentTime() {
  return (int64)talk_base::Time();
}

void XmppPump::OnMessage(talk_base::Message *pmsg) {
  RunTasks();
}

buzz::XmppReturnStatus XmppPump::SendStanza(const buzz::XmlElement *stanza) {
  if (!AllChildrenDone())
    return client_->SendStanza(stanza);
  return buzz::XMPP_RETURN_BADSTATE;
}
