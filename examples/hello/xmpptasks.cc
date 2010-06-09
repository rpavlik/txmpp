#include <txmpp/xmpp/constants.h>
#include <txmpp/base/logging.h>
#include "xmpptasks.h"

XmppTaskMessage::XmppTaskMessage(txmpp::TaskParent *parent)
    : txmpp::XmppTask(parent, txmpp::XmppEngine::HL_ALL) {}

XmppTaskMessage::~XmppTaskMessage() {
}

int XmppTaskMessage::ProcessStart() {
  return STATE_RESPONSE;
}

int XmppTaskMessage::ProcessResponse() {
  const txmpp::XmlElement* stanza = NextStanza();
  if (stanza == NULL) {
    return STATE_BLOCKED;
  }

  std::string from = "Someone";
  std::string body = "something too quiet to hear.";

  if (stanza->HasAttr(txmpp::QN_FROM))
    from = stanza->Attr(txmpp::QN_FROM);

  const txmpp::XmlElement *body_stanza = stanza->FirstNamed(txmpp::QN_BODY);
  if (body_stanza != NULL) {
    body = body_stanza->BodyText();
  }

  LOG(INFO) << from << " says " << body;

  return STATE_RESPONSE;
}

bool XmppTaskMessage::HandleStanza(const txmpp::XmlElement *stanza) {

  if (stanza->Name() == txmpp::QN_MESSAGE) {
    QueueStanza(stanza);
    return true;
  }
  return false;
}
