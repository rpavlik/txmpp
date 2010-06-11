#include "xmppauth.h"

#include <algorithm>
#include "../../saslcookiemechanism.h"
#include "../../saslplainmechanism.h"

namespace hello {

XmppAuth::XmppAuth() : done_(false) {
}

XmppAuth::~XmppAuth() {
}
  
void XmppAuth::StartPreXmppAuth(const txmpp::Jid & jid,
                                const txmpp::SocketAddress & server,
                                const txmpp::CryptString & pass,
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

txmpp::SaslMechanism* XmppAuth::CreateSaslMechanism(
    const std::string & mechanism) {
  if (mechanism == "PLAIN") {
    return new txmpp::SaslPlainMechanism(jid_, passwd_);
  } else {
    return NULL;
  }
}

}  // namespace hello
