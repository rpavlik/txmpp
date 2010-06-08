#include <iostream>
#include "talk/xmpp/xmppclientsettings.h"
#include "xmppthread.h"
#include "xmppauth.h"

namespace {

const uint32 MSG_LOGIN = 1;
const uint32 MSG_DISCONNECT = 2;

struct LoginData: public talk_base::MessageData {
  LoginData(const buzz::XmppClientSettings& s) : xcs(s) {}
  virtual ~LoginData() {}

  buzz::XmppClientSettings xcs;
};

} // namespace

XmppThread::XmppThread() {
  pump_ = new XmppPump(this);
}

XmppThread::~XmppThread() {
  delete pump_;
}

void XmppThread::ProcessMessages(int cms) {
  talk_base::Thread::ProcessMessages(cms);
}

void XmppThread::Login(const buzz::XmppClientSettings& xcs) {
  Post(this, MSG_LOGIN, new LoginData(xcs));
}

void XmppThread::Disconnect() {
  Post(this, MSG_DISCONNECT);
}

void XmppThread::OnStateChange(buzz::XmppEngine::State state) {
}

void XmppThread::OnMessage(talk_base::Message* pmsg) {
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
