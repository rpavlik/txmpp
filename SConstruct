import platform

defines = ['FEATURE_ENABLE_SSL', 'SSL_USE_OPENSSL', 'HAVE_OPENSSL_SSL_H=1', 'POSIX']
flags = '-I.'
frameworks = []
libraries = ['crypto', 'expat', 'pthread', 'ssl'],
name = 'txmpp'
system = platform.system().lower()

src = [
    "talk/base/asynchttprequest.cc",
    "talk/base/asyncpacketsocket.cc",
    "talk/base/asynctcpsocket.cc",
    "talk/base/asyncudpsocket.cc",
    "talk/base/autodetectproxy.cc",
    "talk/base/base64.cc",
    "talk/base/bytebuffer.cc",
    "talk/base/checks.cc",
    "talk/base/common.cc",
    "talk/base/diskcache.cc",
    "talk/base/event.cc",
    "talk/base/fileutils.cc",
    "talk/base/firewallsocketserver.cc",
    "talk/base/flags.cc",
    "talk/base/helpers.cc",
    "talk/base/host.cc",
    "talk/base/httpbase.cc",
    "talk/base/httpclient.cc",
    "talk/base/httpcommon.cc",
    "talk/base/httprequest.cc",
    "talk/base/logging.cc",
    "talk/base/md5c.c",
    "talk/base/messagehandler.cc",
    "talk/base/messagequeue.cc",
    "talk/base/nethelpers.cc",
    "talk/base/network.cc",
    "talk/base/openssladapter.cc",
    "talk/base/opensslidentity.cc",
    "talk/base/opensslstreamadapter.cc",
    "talk/base/pathutils.cc",
    "talk/base/physicalsocketserver.cc",
    "talk/base/proxydetect.cc",
    "talk/base/proxyinfo.cc",
    "talk/base/signalthread.cc",
    "talk/base/socketadapters.cc",
    "talk/base/socketaddress.cc",
    "talk/base/socketpool.cc",
    "talk/base/ssladapter.cc",
    "talk/base/sslidentity.cc",
    "talk/base/sslsocketfactory.cc",
    "talk/base/sslstreamadapter.cc",
    "talk/base/stream.cc",
    "talk/base/stringdigest.cc",
    "talk/base/stringencode.cc",
    "talk/base/stringutils.cc",
    "talk/base/task.cc",
    "talk/base/taskparent.cc",
    "talk/base/taskrunner.cc",
    "talk/base/thread.cc",
    "talk/base/time.cc",
    "talk/base/urlencode.cc",
    "talk/xmllite/qname.cc",
    "talk/xmllite/xmlbuilder.cc",
    "talk/xmllite/xmlconstants.cc",
    "talk/xmllite/xmlelement.cc",
    "talk/xmllite/xmlnsstack.cc",
    "talk/xmllite/xmlparser.cc",
    "talk/xmllite/xmlprinter.cc",
    "talk/xmpp/constants.cc",
    "talk/xmpp/jid.cc",
    "talk/xmpp/ratelimitmanager.cc",
    "talk/xmpp/saslmechanism.cc",
    "talk/xmpp/xmppclient.cc",
    "talk/xmpp/xmppengineimpl.cc",
    "talk/xmpp/xmppengineimpl_iq.cc",
    "talk/xmpp/xmpplogintask.cc",
    "talk/xmpp/xmppstanzaparser.cc",
    "talk/xmpp/xmpptask.cc",
]

mac_src = [
    "talk/base/macconversion.cc",
    "talk/base/macutils.cc",
]

posix_src = [
    "talk/base/unixfilesystem.cc",
]

AddOption(
    '--prefix',
    dest='prefix',
    type='string',
    nargs=1,
    action='store',
    default='',
    metavar='DIR',
    help='installation prefix',
)

AddOption(
    '--with-examples',
    dest='examples',
    action='store_true',
)

AddOption(
    '--with-debug',
    dest='debug',
    action='store_true',
)

if GetOption('debug'):
    flags += ' -g'
    defines += ['_DEBUG']

if system == 'linux':
    defines += ['LINUX']
    src += posix_src
elif system == 'darwin':
    defines += ['OSX']
    frameworks += [
        'CoreServices',
        'Carbon',
        'Security',
        'SystemConfiguration',
    ]
    src += posix_src + mac_src

env = Environment(
    CXXFLAGS=flags,
    FRAMEWORKS=frameworks,
)

libtxmpp = env.SharedLibrary(name, src, CPPDEFINES=defines, LIBS=libraries)

if GetOption('examples'):
    hello_src = [
        'examples/hello/main.cc',
        'examples/hello/xmppauth.cc',
        'examples/hello/xmpppump.cc',
        'examples/hello/xmppsocket.cc',
        'examples/hello/xmppthread.cc',
    ]
    hello = env.Program(target='hello-example', source=hello_src, CPPDEFINES=defines+['USE_SSLSTREAM'], LIBS=libtxmpp)
