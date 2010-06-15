txmpp
=====

_txmpp_ is a C++ XMPP library created from the non-[XEP-0166][xep0166] portions
of [libjingle][libjingle] with various bits removed and tweaked.

The motivating facter for "creating" _txmpp_ was the lack of [permissively
licenced alternatives][permissive].

If you don't mind a library with a [copyleft][copyleft] license you might also
try [gloox][gloox].

### Build

    scons --with-examples

### Getting Started

See the `src/examples` directory.

### License

This work is licensed under a BSD-style License (see the LICENSE file).

[xep0166]: http://xmpp.org/extensions/xep-0166.html
[libjingle]: http://code.google.com/p/libjingle/
[permissive]: http://en.wikipedia.org/wiki/Permissive_license
[copyleft]: http://en.wikipedia.org/wiki/Copyleft
[gloox]: http://camaya.net/gloox/
