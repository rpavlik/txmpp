import platform

def Abort(message):
    print message
    Exit(1)

defines = ['FEATURE_ENABLE_SSL', 'SSL_USE_OPENSSL', 'HAVE_OPENSSL_SSL_H=1', 'POSIX']
flags = '-Isrc'
frameworks = []
libraries = ['crypto', 'expat', 'pthread', 'ssl']
name = 'txmpp'
src_path = 'src/txmpp/'
system = platform.system().lower()

src = [
    "base/asynchttprequest.cc",
    "base/asyncpacketsocket.cc",
    "base/asynctcpsocket.cc",
    "base/asyncudpsocket.cc",
    "base/autodetectproxy.cc",
    "base/base64.cc",
    "base/bytebuffer.cc",
    "base/checks.cc",
    "base/common.cc",
    "base/diskcache.cc",
    "base/event.cc",
    "base/fileutils.cc",
    "base/firewallsocketserver.cc",
    "base/flags.cc",
    "base/helpers.cc",
    "base/host.cc",
    "base/httpbase.cc",
    "base/httpclient.cc",
    "base/httpcommon.cc",
    "base/httprequest.cc",
    "base/logging.cc",
    "base/md5c.c",
    "base/messagehandler.cc",
    "base/messagequeue.cc",
    "base/nethelpers.cc",
    "base/network.cc",
    "base/openssladapter.cc",
    "base/opensslidentity.cc",
    "base/opensslstreamadapter.cc",
    "base/pathutils.cc",
    "base/physicalsocketserver.cc",
    "base/proxydetect.cc",
    "base/proxyinfo.cc",
    "base/signalthread.cc",
    "base/socketadapters.cc",
    "base/socketaddress.cc",
    "base/socketpool.cc",
    "base/ssladapter.cc",
    "base/sslidentity.cc",
    "base/sslsocketfactory.cc",
    "base/sslstreamadapter.cc",
    "base/stream.cc",
    "base/stringdigest.cc",
    "base/stringencode.cc",
    "base/stringutils.cc",
    "base/task.cc",
    "base/taskparent.cc",
    "base/taskrunner.cc",
    "base/thread.cc",
    "base/time.cc",
    "base/urlencode.cc",
    "xmllite/qname.cc",
    "xmllite/xmlbuilder.cc",
    "xmllite/xmlconstants.cc",
    "xmllite/xmlelement.cc",
    "xmllite/xmlnsstack.cc",
    "xmllite/xmlparser.cc",
    "xmllite/xmlprinter.cc",
    "xmpp/constants.cc",
    "xmpp/jid.cc",
    "xmpp/ratelimitmanager.cc",
    "xmpp/saslmechanism.cc",
    "xmpp/xmppclient.cc",
    "xmpp/xmppengineimpl.cc",
    "xmpp/xmppengineimpl_iq.cc",
    "xmpp/xmpplogintask.cc",
    "xmpp/xmppstanzaparser.cc",
    "xmpp/xmpptask.cc",
]

darwin_src = [
    "base/macconversion.cc",
    "base/macutils.cc",
]

posix_src = [
    "base/unixfilesystem.cc",
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
    '--talk',
    dest='talk',
    action='store_true',
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

if GetOption('talk') and GetOption('examples'):
    Abort("talk can't be compiled with examples.")

if GetOption('talk'):
    src_path = 'src/talk/'

src = map(lambda p: src_path + p, src)
posix_src = map(lambda p: src_path + p, posix_src)
darwin_src = map(lambda p: src_path + p, darwin_src)

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
    src += posix_src + darwin_src

env = Environment(
    CXXFLAGS=flags,
    FRAMEWORKS=frameworks,
)

conf = Configure(env)

if system in ('darwin', 'linux'):
    if not conf.CheckCXXHeader('pthread.h'):
        Abort('pthread.h not found')

libtxmpp = env.SharedLibrary(name, src, CPPDEFINES=defines, LIBS=libraries)

if GetOption('examples'):
    hello_src = [
        'examples/hello/main.cc',
        'examples/hello/xmppauth.cc',
        'examples/hello/xmpppump.cc',
        'examples/hello/xmppsocket.cc',
        'examples/hello/xmpptasks.cc',
        'examples/hello/xmppthread.cc',
    ]
    hello = env.Program(target='hello-example', source=hello_src, CPPDEFINES=defines+['USE_SSLSTREAM'], LIBS=libtxmpp)
