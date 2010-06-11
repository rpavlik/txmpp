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

#include "prexmppauthimpl.h"

#include <algorithm>
#include "saslcookiemechanism.h"
#include "saslplainmechanism.h"

namespace txmpp {

PreXmppAuthImpl::PreXmppAuthImpl() : done_(false) {
}

PreXmppAuthImpl::~PreXmppAuthImpl() {
}

void PreXmppAuthImpl::StartPreXmppAuth(const Jid & jid,
                                       const SocketAddress & server,
                                       const CryptString & pass,
                                       const std::string & auth_cookie) {
  jid_ = jid;
  passwd_ = pass;
  auth_cookie_ = auth_cookie;
  done_ = true;

  SignalAuthDone();
}

std::string PreXmppAuthImpl::ChooseBestSaslMechanism(
    const std::vector<std::string> & mechanisms,
    bool encrypted) {
  std::vector<std::string>::const_iterator it;

  it = std::find(mechanisms.begin(), mechanisms.end(), "PLAIN");
  if (it != mechanisms.end()) {
    return "PLAIN";
  }

 return "";  // No good mechanism found
}

SaslMechanism* PreXmppAuthImpl::CreateSaslMechanism(
    const std::string & mechanism) {
  if (mechanism == "PLAIN") {
    return new SaslPlainMechanism(jid_, passwd_);
  } else {
    return NULL;
  }
}

}  // namespace txmpp
