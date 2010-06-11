/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TXMPP_PREXMPPAUTHIMPL_H_
#define _TXMPP_PREXMPPAUTHIMPL_H_

#ifndef NO_CONFIG_H
#include "config.h"
#endif

#include <vector>
#include "cryptstring.h"
#include "sigslot.h"
#include "jid.h"
#include "saslhandler.h"
#include "prexmppauth.h"

namespace txmpp {

class PreXmppAuthImpl : public PreXmppAuth {
  public:
    PreXmppAuthImpl();
    virtual ~PreXmppAuthImpl();

    virtual void StartPreXmppAuth(const Jid & jid,
                                  const SocketAddress & server,
                                  const CryptString & pass,
                                  const std::string & auth_cookie);

    virtual bool IsAuthDone() const { return done_; }
    virtual bool IsAuthorized() const { return true; }
    virtual bool HadError() const { return false; }
    virtual int  GetError() const { return 0; }

    virtual CaptchaChallenge GetCaptchaChallenge() const {
        return CaptchaChallenge();
    }
    virtual std::string GetAuthCookie() const { return auth_cookie_; }

    virtual std::string ChooseBestSaslMechanism(
        const std::vector<std::string> & mechanisms,
        bool encrypted);

    virtual SaslMechanism * CreateSaslMechanism(
        const std::string & mechanism);

  private:
    Jid jid_;
    CryptString passwd_;
    std::string auth_cookie_;
    bool done_;
};

}  // namespace txmpp

#endif  // _TXMPP_PREXMPPAUTHIMPL_H_
