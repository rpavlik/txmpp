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

#ifndef _TXMPP_PHYSICALSOCKETSERVER_H__
#define _TXMPP_PHYSICALSOCKETSERVER_H__

#ifndef NO_CONFIG_H
#include "config.h"
#endif

#include <vector>

#include "asyncfile.h"
#include "socketserver.h"
#include "criticalsection.h"

#ifdef POSIX
typedef int SOCKET;
#endif // POSIX

namespace txmpp { 

class Dispatcher;
class Signaler;
#ifdef POSIX
class PosixSignalDeliveryDispatcher;
#endif

// A socket server that provides the real sockets of the underlying OS.
class PhysicalSocketServer : public SocketServer {
public:
  PhysicalSocketServer();
  virtual ~PhysicalSocketServer();

  // SocketFactory:
  virtual Socket* CreateSocket(int type);
  virtual AsyncSocket* CreateAsyncSocket(int type);

  // Internal Factory for Accept
  AsyncSocket* WrapSocket(SOCKET s);

  // SocketServer:
  virtual bool Wait(int cms, bool process_io);
  virtual void WakeUp();

  void Add(Dispatcher* dispatcher);
  void Remove(Dispatcher* dispatcher);

#ifdef POSIX
  AsyncFile* CreateFile(int fd);

  // Sets the function to be executed in response to the specified POSIX signal.
  // The function is executed from inside Wait() using the "self-pipe trick"--
  // regardless of which thread receives the signal--and hence can safely
  // manipulate user-level data structures.
  // "handler" may be SIG_IGN, SIG_DFL, or a user-specified function, just like
  // with signal(2).
  // Only one PhysicalSocketServer may have user-level signal handlers.
  // Attempting to install a signal handler for a PhysicalSocketServer when
  // another already owns some will fail.
  // The signal mask is not modified. It is the caller's responsibily to
  // maintain it as desired.
  bool SetPosixSignalHandler(int signum, void (*handler)(int));
#endif

private:
  typedef std::vector<Dispatcher*> DispatcherList;
  typedef std::vector<size_t*> IteratorList;

#ifdef POSIX
  static bool InstallSignal(int signum, void (*handler)(int));
#endif

  DispatcherList dispatchers_;
  IteratorList iterators_;
  Signaler* signal_wakeup_;
  CriticalSection crit_;
  bool fWait_;
  uint32 last_tick_tracked_;
  int last_tick_dispatch_count_;
#ifdef WIN32
  WSAEVENT socket_ev_;
#endif
};

}  // namespace txmpp

#endif  // _TXMPP_PHYSICALSOCKETSERVER_H__
