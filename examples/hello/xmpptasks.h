#ifndef _XMPPTASKS_H_
#define _XMPPTASKS_H_

#include <txmpp/base/taskparent.h>
#include <txmpp/xmpp/xmpptask.h>

class XmppTaskMessage : public txmpp::XmppTask {
  public:
    explicit XmppTaskMessage(txmpp::TaskParent *parent);
    virtual ~XmppTaskMessage();
    virtual int ProcessStart();
    virtual int ProcessResponse();
    bool HandleStanza(const txmpp::XmlElement *stanza);
};

#endif  // _XMPPTASK_H_
