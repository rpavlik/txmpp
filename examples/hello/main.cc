#include <iostream>
#include <txmpp/base/cryptstring.h>
#include <txmpp/base/logging.h>
#include <txmpp/xmpp/xmppclientsettings.h>
#include "xmppthread.h"

int main(int argc, char* argv[]) {

  bool reconnect = true;

  txmpp::LogMessage::LogToDebug(txmpp::LS_SENSITIVE);

  txmpp::InsecureCryptStringImpl ipass;
  ipass.password() = "test";
  txmpp::CryptString password = txmpp::CryptString(ipass);

  while (reconnect) {

    // Start xmpp on a different thread
    XmppThread thread;
    thread.Start();

    // Create client settings
    txmpp::XmppClientSettings xcs;
    xcs.set_user("test");
    xcs.set_pass(password);
    xcs.set_host("example.org");
    xcs.set_resource("resource");
    xcs.set_use_tls(true);
    xcs.set_server(txmpp::SocketAddress("example.org", 5222));

    thread.Login(xcs);

    // Use main thread for console input
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line == "quit")
        reconnect = false;
      if (line == "continue" || line == "quit")
        break;
    }

    thread.Disconnect();
    thread.Stop();
  }

  return 0;
}
