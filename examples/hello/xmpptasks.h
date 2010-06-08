#ifndef _XMPPTASKS_H_
#define _XMPPTASKS_H_

#include <talk/base/taskparent.h>
#include <talk/xmpp/xmpptask.h>

class XmppTaskMessage : public buzz::XmppTask {
  public:
    explicit XmppTaskMessage(talk_base::TaskParent *parent);
    virtual ~XmppTaskMessage();
    virtual int ProcessStart();
    virtual int ProcessResponse();
    bool HandleStanza(const buzz::XmlElement *stanza);
};

#endif  // _XMPPTASK_H_
