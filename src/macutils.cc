/*
 * libjingle
 * Copyright 2007--2009, Google Inc.
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

#include "macutils.h"

#include <sstream>

#include "common.h"
#include "logging.h"
#include "scoped_ptr.h"
#include "stringutils.h"

namespace txmpp {

///////////////////////////////////////////////////////////////////////////////

bool ToUtf8(const CFStringRef str16, std::string* str8) {
  if ((NULL == str16) || (NULL == str8))
    return false;
  size_t maxlen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str16),
                                                    kCFStringEncodingUTF8)
                  + 1;
  scoped_array<char> buffer(new char[maxlen]);
  if (!buffer.get()
      || !CFStringGetCString(str16, buffer.get(), maxlen,
                             kCFStringEncodingUTF8))
    return false;
  str8->assign(buffer.get());
  return true;
}

bool ToUtf16(const std::string& str8, CFStringRef* str16) {
  if (NULL == str16)
    return false;
  *str16 = CFStringCreateWithBytes(kCFAllocatorDefault,
                                   reinterpret_cast<const UInt8*>(str8.data()),
                                   str8.length(), kCFStringEncodingUTF8,
                                   false);
  return (NULL != *str16);
}

void DecodeFourChar(UInt32 fc, std::string* out) {
  std::stringstream ss;
  ss << '\'';
  bool printable = true;
  for (int i = 3; i >= 0; --i) {
    char ch = (fc >> (8 * i)) & 0xFF;
    if (isprint(static_cast<unsigned char>(ch))) {
      ss << ch;
    } else {
      printable = false;
      break;
    }
  }
  if (printable) {
    ss << '\'';
  } else {
    ss.str("");
    ss << "0x" << std::hex << fc;
  }
  out->append(ss.str());
}

std::string DecodeEvent(EventRef event) {
  std::string str;
  DecodeFourChar(::GetEventClass(event), &str);
  str.push_back(':');
  DecodeFourChar(::GetEventKind(event), &str);
  return str;
}

static bool GetGestalt(OSType ostype, int* value) {
  ASSERT(NULL != value);
  SInt32 native_value;
  OSStatus result = Gestalt(ostype, &native_value);
  if (noErr == result) {
    *value = native_value;
    return true;
  }
  std::string str;
  DecodeFourChar(ostype, &str);
  LOG_E(LS_ERROR, OS, result) << "Gestalt(" << str << ")";
  return false;
}

bool GetOSVersion(int* major, int* minor, int* bugfix) {
  ASSERT(major && minor && bugfix);
  if (!GetGestalt(gestaltSystemVersion, major))
    return false;
  if (*major < 0x1040) {
    *bugfix = *major & 0xF;
    *minor = (*major >> 4) & 0xF;
    *major = (*major >> 8);
    return true;
  }
  return GetGestalt(gestaltSystemVersionMajor, major)
      && GetGestalt(gestaltSystemVersionMinor, minor)
      && GetGestalt(gestaltSystemVersionBugFix, bugfix);
}

MacOSVersionName GetOSVersionName() {
  int major = 0, minor = 0, bugfix = 0;
  if (!GetOSVersion(&major, &minor, &bugfix))
    return kMacOSUnknown;
  if (major > 10)
    return kMacOSNewer;
  if ((major < 10) || (minor < 3))
    return kMacOSOlder;
  switch (minor) {
    case 3:
      return kMacOSPanther;
    case 4:
      return kMacOSTiger;
    case 5:
      return kMacOSLeopard;
  }
  return kMacOSNewer;
}

bool GetQuickTimeVersion(std::string* out) {
  int ver;
  if (!GetGestalt(gestaltQuickTimeVersion, &ver))
    return false;
  
  std::stringstream ss;
  ss << std::hex << ver;
  *out = ss.str();
  return true;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace txmpp
