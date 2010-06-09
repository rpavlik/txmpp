#include "xmppthread.h"

#include <txmpp/xmpp/xmppclientsettings.h>
#include "xmppauth.h"

namespace {

const uint32 MSG_LOGIN = 1;
const uint32 MSG_DISCONNECT = 2;

struct LoginData: public txmpp::MessageData {
  LoginData(const txmpp::XmppClientSettings& s) : xcs(s) {}
  virtual ~LoginData() {}

  txmpp::XmppClientSettings xcs;
};

} // namespace

XmppThread::XmppThread() {
  pump_ = new XmppPump(this);
}

XmppThread::~XmppThread() {
  delete pump_;
}

void XmppThread::ProcessMessages(int cms) {
  txmpp::Thread::ProcessMessages(cms);
}

void XmppThread::Login(const txmpp::XmppClientSettings& xcs) {
  Post(this, MSG_LOGIN, new LoginData(xcs));
}

void XmppThread::Disconnect() {
  Post(this, MSG_DISCONNECT);
}

void XmppThread::OnStateChange(txmpp::XmppEngine::State state) {
}

void XmppThread::OnMessage(txmpp::Message* pmsg) {
  if (pmsg->message_id == MSG_LOGIN) {
    assert(pmsg->pdata);
    LoginData* data = reinterpret_cast<LoginData*>(pmsg->pdata);
    pump_->DoLogin(data->xcs, new XmppSocket(true), new XmppAuth());
    delete data;
  } else if (pmsg->message_id == MSG_DISCONNECT) {
    pump_->DoDisconnect();
  } else {
    assert(false);
  }
}
