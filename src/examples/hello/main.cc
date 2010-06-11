#include <iostream>
#include "../../cryptstring.h"
#include "../../logging.h"
#include "../../xmppclientsettings.h"
#include "xmppthread.h"

int main(int argc, char* argv[]) {

  bool reconnect = true;

  txmpp::LogMessage::LogToDebug(txmpp::LS_SENSITIVE);

  txmpp::InsecureCryptStringImpl password;
  password.password() = "test";

  while (reconnect) {

    // Start xmpp on a different thread
    hello::XmppThread thread;
    thread.Start();

    // Create client settings
    txmpp::XmppClientSettings xcs;
    xcs.set_user("test");
    xcs.set_pass(txmpp::CryptString(password));
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
