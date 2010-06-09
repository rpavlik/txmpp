#include <iostream>
#include <sstream>
#include <txmpp/xmpp/xmppclient.h>
#include <txmpp/xmpp/constants.h>
#include <txmpp/base/logging.h>
#include "xmpptasks.h"

/**
 * Example message handler
 */

XmppTaskMessage::XmppTaskMessage(txmpp::TaskParent *parent)
    : txmpp::XmppTask(parent, txmpp::XmppEngine::HL_TYPE) {}

XmppTaskMessage::~XmppTaskMessage() {}

int XmppTaskMessage::ProcessStart() {
  return STATE_RESPONSE;
}

int XmppTaskMessage::ProcessResponse() {

  const txmpp::XmlElement* stanza = NextStanza();
  if (stanza == NULL) {
    return STATE_BLOCKED;
  }

  std::string from = "Someone";
  std::string text = "something too quiet to hear.";

  if (stanza->HasAttr(txmpp::QN_FROM))
    from = stanza->Attr(txmpp::QN_FROM);

  const txmpp::XmlElement *body_stanza = stanza->FirstNamed(txmpp::QN_BODY);
  if (body_stanza != NULL) {
    text = body_stanza->BodyText();
  }

  std::cout << "Message: " << from << " says " << text << std::endl;

  // Send response message
  if (from != "Someone") {

    std::stringstream response_text;
    response_text << "The thing you said was " << text.length()
                  << " characters long.";

    txmpp::scoped_ptr<txmpp::XmlElement> message(
        new txmpp::XmlElement(txmpp::QN_MESSAGE));
    message->SetAttr(txmpp::QN_TO, from);

    txmpp::XmlElement *body = new txmpp::XmlElement(txmpp::QN_BODY);
    body->SetBodyText(response_text.str());

    message->AddElement(body);

    SendStanza(message.get());
  }

  return STATE_RESPONSE;
}

bool XmppTaskMessage::HandleStanza(const txmpp::XmlElement *stanza) {

  if (stanza->Name() == txmpp::QN_MESSAGE) {
    QueueStanza(stanza);
    return true;
  }

  return false;
}

/**
 * Example presence handler
 */

XmppTaskPresence::XmppTaskPresence(txmpp::TaskParent *parent)
    : txmpp::XmppTask(parent, txmpp::XmppEngine::HL_TYPE) {}

XmppTaskPresence::~XmppTaskPresence() {}

int XmppTaskPresence::ProcessStart() {

  txmpp::scoped_ptr<txmpp::XmlElement> presence(
      new txmpp::XmlElement(txmpp::QN_PRESENCE));
  SendStanza(presence.get());

  return STATE_RESPONSE;
}

int XmppTaskPresence::ProcessResponse() {

  const txmpp::XmlElement* stanza = NextStanza();
  if (stanza == NULL) {
    return STATE_BLOCKED;
  }

  std::string from = "Someone";

  if (stanza->HasAttr(txmpp::QN_FROM))
    from = stanza->Attr(txmpp::QN_FROM);

  std::cout << "Presence: " << from << std::endl;

  return STATE_RESPONSE;
}

bool XmppTaskPresence::HandleStanza(const txmpp::XmlElement *stanza) {

  if (stanza->Name() == txmpp::QN_PRESENCE) {
    QueueStanza(stanza);
    return true;
  }

  return false;
}

/**
 * Example iq handler
 */

XmppTaskIq::XmppTaskIq(txmpp::TaskParent *parent)
    : txmpp::XmppTask(parent, txmpp::XmppEngine::HL_SINGLE) {}

XmppTaskIq::~XmppTaskIq() {}

int XmppTaskIq::ProcessStart() {

  set_task_id(GetClient()->NextId());

  txmpp::scoped_ptr<txmpp::XmlElement> privacy(
      MakeIq("get", txmpp::JID_EMPTY, task_id()));
  txmpp::XmlElement *query = new txmpp::XmlElement(txmpp::QN_PRIVACY_QUERY);
  privacy->AddElement(query);

  SendStanza(privacy.get());

  return STATE_RESPONSE;
}

int XmppTaskIq::ProcessResponse() {

  const txmpp::XmlElement* stanza = NextStanza();
  if (stanza == NULL) {
    return STATE_BLOCKED;
  }

  std::string from = "Someone";

  if (stanza->HasAttr(txmpp::QN_FROM))
    from = stanza->Attr(txmpp::QN_FROM);

  std::cout << "Iq: " << from << std::endl;

  return STATE_RESPONSE;
}

bool XmppTaskIq::HandleStanza(const txmpp::XmlElement *stanza) {

  if (MatchResponseIq(stanza, txmpp::JID_EMPTY, task_id())) {
    QueueStanza(stanza);
    return true;
  }

  return false;
}
