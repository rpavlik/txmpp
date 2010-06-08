#include <cstdio>
#include <iostream>

#include "talk/xmpp/xmppclientsettings.h"
#include "talk/base/cryptstring.h"
#include "talk/base/logging.h"

#include "xmppthread.h"

int main(int argc, char **argv) {

  talk_base::LogMessage::LogToDebug(talk_base::LS_SENSITIVE);

  talk_base::InsecureCryptStringImpl ipass;
  ipass.password() = "test";
  talk_base::CryptString password = talk_base::CryptString(ipass);

  // Start xmpp on a different thread
  XmppThread thread;
  thread.Start();

  // Create client settings
  buzz::XmppClientSettings xcs;
  xcs.set_user("test");
  xcs.set_pass(password);
  xcs.set_host("example.org");
  xcs.set_use_tls(true);
  xcs.set_server(talk_base::SocketAddress("example.org", 5222));

  thread.Login(xcs);

  // Use main thread for console input
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "quit")
      break;
  }

  return 0;
}
