#ifndef _XMPPAUTH_H_
#define _XMPPAUTH_H_

#include <vector>
#include <talk/base/cryptstring.h>
#include <talk/base/sigslot.h>
#include <talk/xmpp/jid.h>
#include <talk/xmpp/saslhandler.h>
#include <talk/xmpp/prexmppauth.h>

class XmppAuth: public buzz::PreXmppAuth {
public:
  XmppAuth();
  virtual ~XmppAuth();

  virtual void StartPreXmppAuth(const buzz::Jid & jid,
                                const talk_base::SocketAddress & server,
                                const talk_base::CryptString & pass,
                                const std::string & auth_cookie);

  virtual bool IsAuthDone() const { return done_; }
  virtual bool IsAuthorized() const { return true; }
  virtual bool HadError() const { return false; }
  virtual int  GetError() const { return 0; }
  virtual buzz::CaptchaChallenge GetCaptchaChallenge() const {
      return buzz::CaptchaChallenge();
  }
  virtual std::string GetAuthCookie() const { return auth_cookie_; }

  virtual std::string ChooseBestSaslMechanism(
      const std::vector<std::string> & mechanisms,
      bool encrypted);

  virtual buzz::SaslMechanism * CreateSaslMechanism(
      const std::string & mechanism);

private:
  buzz::Jid jid_;
  talk_base::CryptString passwd_;
  std::string auth_cookie_;
  bool done_;
};

#endif
