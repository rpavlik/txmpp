import platform

defines = ['FEATURE_ENABLE_SSL', 'SSL_USE_OPENSSL', 'HAVE_OPENSSL_SSL_H=1', 'POSIX']
flags = '-Isrc'
frameworks = []
libraries = ['crypto', 'expat', 'pthread', 'ssl'],
name = 'txmpp'
system = platform.system().lower()

src = [
    "src/talk/base/asynchttprequest.cc",
    "src/talk/base/asyncpacketsocket.cc",
    "src/talk/base/asynctcpsocket.cc",
    "src/talk/base/asyncudpsocket.cc",
    "src/talk/base/autodetectproxy.cc",
    "src/talk/base/base64.cc",
    "src/talk/base/bytebuffer.cc",
    "src/talk/base/checks.cc",
    "src/talk/base/common.cc",
    "src/talk/base/diskcache.cc",
    "src/talk/base/event.cc",
    "src/talk/base/fileutils.cc",
    "src/talk/base/firewallsocketserver.cc",
    "src/talk/base/flags.cc",
    "src/talk/base/helpers.cc",
    "src/talk/base/host.cc",
    "src/talk/base/httpbase.cc",
    "src/talk/base/httpclient.cc",
    "src/talk/base/httpcommon.cc",
    "src/talk/base/httprequest.cc",
    "src/talk/base/logging.cc",
    "src/talk/base/md5c.c",
    "src/talk/base/messagehandler.cc",
    "src/talk/base/messagequeue.cc",
    "src/talk/base/nethelpers.cc",
    "src/talk/base/network.cc",
    "src/talk/base/openssladapter.cc",
    "src/talk/base/opensslidentity.cc",
    "src/talk/base/opensslstreamadapter.cc",
    "src/talk/base/pathutils.cc",
    "src/talk/base/physicalsocketserver.cc",
    "src/talk/base/proxydetect.cc",
    "src/talk/base/proxyinfo.cc",
    "src/talk/base/signalthread.cc",
    "src/talk/base/socketadapters.cc",
    "src/talk/base/socketaddress.cc",
    "src/talk/base/socketpool.cc",
    "src/talk/base/ssladapter.cc",
    "src/talk/base/sslidentity.cc",
    "src/talk/base/sslsocketfactory.cc",
    "src/talk/base/sslstreamadapter.cc",
    "src/talk/base/stream.cc",
    "src/talk/base/stringdigest.cc",
    "src/talk/base/stringencode.cc",
    "src/talk/base/stringutils.cc",
    "src/talk/base/task.cc",
    "src/talk/base/taskparent.cc",
    "src/talk/base/taskrunner.cc",
    "src/talk/base/thread.cc",
    "src/talk/base/time.cc",
    "src/talk/base/urlencode.cc",
    "src/talk/xmllite/qname.cc",
    "src/talk/xmllite/xmlbuilder.cc",
    "src/talk/xmllite/xmlconstants.cc",
    "src/talk/xmllite/xmlelement.cc",
    "src/talk/xmllite/xmlnsstack.cc",
    "src/talk/xmllite/xmlparser.cc",
    "src/talk/xmllite/xmlprinter.cc",
    "src/talk/xmpp/constants.cc",
    "src/talk/xmpp/jid.cc",
    "src/talk/xmpp/ratelimitmanager.cc",
    "src/talk/xmpp/saslmechanism.cc",
    "src/talk/xmpp/xmppclient.cc",
    "src/talk/xmpp/xmppengineimpl.cc",
    "src/talk/xmpp/xmppengineimpl_iq.cc",
    "src/talk/xmpp/xmpplogintask.cc",
    "src/talk/xmpp/xmppstanzaparser.cc",
    "src/talk/xmpp/xmpptask.cc",
]

mac_src = [
    "src/talk/base/macconversion.cc",
    "src/talk/base/macutils.cc",
]

posix_src = [
    "src/talk/base/unixfilesystem.cc",
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
