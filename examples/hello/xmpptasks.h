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

class XmppTaskPresence : public txmpp::XmppTask {
  public:
    explicit XmppTaskPresence(txmpp::TaskParent *parent);
    virtual ~XmppTaskPresence();
    virtual int ProcessStart();
    virtual int ProcessResponse();
    bool HandleStanza(const txmpp::XmlElement *stanza);
};

class XmppTaskIq : public txmpp::XmppTask {
  public:
    explicit XmppTaskIq(txmpp::TaskParent *parent);
    virtual ~XmppTaskIq();
    virtual int ProcessStart();
    virtual int ProcessResponse();
    bool HandleStanza(const txmpp::XmlElement *stanza);
};

#endif  // _XMPPTASK_H_
