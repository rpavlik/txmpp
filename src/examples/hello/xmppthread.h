#ifndef _HELLO_XMPPTHREAD_H_
#define _HELLO_XMPPTHREAD_H_

#include "../../thread.h"
#include "../../xmppclientsettings.h"
#include "xmpppump.h"

namespace hello {

class XmppThread: public txmpp::Thread, XmppPumpNotify, txmpp::MessageHandler {
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

}  // namespace hello

#endif  // _HELLO_XMPPTHREAD_H_
