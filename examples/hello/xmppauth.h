#ifndef _XMPPAUTH_H_
#define _XMPPAUTH_H_

#include <vector>
#include <txmpp/base/cryptstring.h>
#include <txmpp/base/sigslot.h>
#include <txmpp/xmpp/jid.h>
#include <txmpp/xmpp/saslhandler.h>
#include <txmpp/xmpp/prexmppauth.h>

class XmppAuth: public txmpp::PreXmppAuth {
public:
  XmppAuth();
  virtual ~XmppAuth();

  virtual void StartPreXmppAuth(const txmpp::Jid & jid,
                                const txmpp::SocketAddress & server,
                                const txmpp::CryptString & pass,
                                const std::string & auth_cookie);

  virtual bool IsAuthDone() const { return done_; }
  virtual bool IsAuthorized() const { return true; }
  virtual bool HadError() const { return false; }
  virtual int  GetError() const { return 0; }
  virtual txmpp::CaptchaChallenge GetCaptchaChallenge() const {
      return txmpp::CaptchaChallenge();
  }
  virtual std::string GetAuthCookie() const { return auth_cookie_; }

  virtual std::string ChooseBestSaslMechanism(
      const std::vector<std::string> & mechanisms,
      bool encrypted);

  virtual txmpp::SaslMechanism * CreateSaslMechanism(
      const std::string & mechanism);

private:
  txmpp::Jid jid_;
  txmpp::CryptString passwd_;
  std::string auth_cookie_;
  bool done_;
};

#endif
