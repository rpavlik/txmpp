#include "xmppauth.h"

#include <algorithm>
#include <talk/xmpp/saslcookiemechanism.h>
#include <talk/xmpp/saslplainmechanism.h>

XmppAuth::XmppAuth() : done_(false) {
}

XmppAuth::~XmppAuth() {
}
  
void XmppAuth::StartPreXmppAuth(const buzz::Jid & jid,
                                const talk_base::SocketAddress & server,
                                const talk_base::CryptString & pass,
                                const std::string & auth_cookie) {
  jid_ = jid;
  passwd_ = pass;
  auth_cookie_ = auth_cookie;
  done_ = true;

  SignalAuthDone();
}
  
std::string XmppAuth::ChooseBestSaslMechanism(const std::vector<std::string> & mechanisms, bool encrypted) {
  std::vector<std::string>::const_iterator it;

  it = std::find(mechanisms.begin(), mechanisms.end(), "PLAIN");
  if (it != mechanisms.end()) {
    return "PLAIN";
  }

  // No good mechanism found
 return "";
}

buzz::SaslMechanism* XmppAuth::CreateSaslMechanism(
    const std::string & mechanism) {
  if (mechanism == "PLAIN") {
    return new buzz::SaslPlainMechanism(jid_, passwd_);
  } else {
    return NULL;
  }
}
