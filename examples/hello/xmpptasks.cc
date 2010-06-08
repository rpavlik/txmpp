#include <talk/xmpp/constants.h>
#include <talk/base/logging.h>
#include "xmpptasks.h"

XmppTaskMessage::XmppTaskMessage(talk_base::TaskParent *parent)
    : buzz::XmppTask(parent, buzz::XmppEngine::HL_ALL) {}

XmppTaskMessage::~XmppTaskMessage() {
}

int XmppTaskMessage::ProcessStart() {
  return STATE_RESPONSE;
}

int XmppTaskMessage::ProcessResponse() {
  const buzz::XmlElement* stanza = NextStanza();
  if (stanza == NULL) {
    return STATE_BLOCKED;
  }

  std::string from = "Someone";
  std::string body = "something too quiet to hear.";

  if (stanza->HasAttr(buzz::QN_FROM))
    from = stanza->Attr(buzz::QN_FROM);

  const buzz::XmlElement *body_stanza = stanza->FirstNamed(buzz::QN_BODY);
  if (body_stanza != NULL) {
    body = body_stanza->BodyText();
  }

  LOG(INFO) << from << " says " << body;

  return STATE_RESPONSE;
}

bool XmppTaskMessage::HandleStanza(const buzz::XmlElement *stanza) {

  if (stanza->Name() == buzz::QN_MESSAGE) {
    QueueStanza(stanza);
    return true;
  }
  return false;
}
