import platform

def Abort(message):
    print message
    Exit(1)

def CreateConfig(define_list, path='src/config.h'):
    file = None
    try:
        file = open(path, 'w')
        file.write('#ifndef _TXMPP_CONFIG_H_\n')
        file.write('#define _TXMPP_CONFIG_H_\n\n')
        for define in define_list:
            file.write('#ifndef %s\n#define %s 1\n#endif  // %s\n\n'
                       % (define, define, define))
        file.write('#endif  // _TXMPP_CONFIG_H_\n')
    finally:
        if file is not None:
            file.close()

config = ['FEATURE_ENABLE_SSL', 'HAVE_OPENSSL_SSL_H', 'POSIX', 'SSL_USE_OPENSSL', 'USE_SSLSTREAM']
defines = []
flags = ''
frameworks = []
libraries = ['crypto', 'expat', 'pthread', 'ssl']
name = 'txmpp'
system = platform.system().lower()

src = [
    'src/asynchttprequest.cc',
    'src/asyncpacketsocket.cc',
    'src/asynctcpsocket.cc',
    'src/asyncudpsocket.cc',
    'src/autodetectproxy.cc',
    'src/base64.cc',
    'src/bytebuffer.cc',
    'src/checks.cc',
    'src/common.cc',
    'src/diskcache.cc',
    'src/event.cc',
    'src/fileutils.cc',
    'src/firewallsocketserver.cc',
    'src/flags.cc',
    'src/helpers.cc',
    'src/httpbase.cc',
    'src/httpclient.cc',
    'src/httpcommon.cc',
    'src/httprequest.cc',
    'src/logging.cc',
    'src/md5c.c',
    'src/messagehandler.cc',
    'src/messagequeue.cc',
    'src/nethelpers.cc',
    'src/network.cc',
    'src/openssladapter.cc',
    'src/opensslidentity.cc',
    'src/opensslstreamadapter.cc',
    'src/pathutils.cc',
    'src/prexmppauthimpl.cc',
    'src/physicalsocketserver.cc',
    'src/proxydetect.cc',
    'src/proxyinfo.cc',
    'src/signalthread.cc',
    'src/socketadapters.cc',
    'src/socketaddress.cc',
    'src/socketpool.cc',
    'src/ssladapter.cc',
    'src/sslidentity.cc',
    'src/sslsocketfactory.cc',
    'src/sslstreamadapter.cc',
    'src/stream.cc',
    'src/stringdigest.cc',
    'src/stringencode.cc',
    'src/stringutils.cc',
    'src/task.cc',
    'src/taskparent.cc',
    'src/taskrunner.cc',
    'src/thread.cc',
    'src/time.cc',
    'src/urlencode.cc',
    'src/qname.cc',
    'src/xmlbuilder.cc',
    'src/xmlconstants.cc',
    'src/xmlelement.cc',
    'src/xmlnsstack.cc',
    'src/xmlparser.cc',
    'src/xmlprinter.cc',
    'src/constants.cc',
    'src/jid.cc',
    'src/ratelimitmanager.cc',
    'src/saslmechanism.cc',
    'src/xmppasyncsocketimpl.cc',
    'src/xmppclient.cc',
    'src/xmppengineimpl.cc',
    'src/xmppengineimpl_iq.cc',
    'src/xmpplogintask.cc',
    'src/xmppstanzaparser.cc',
    'src/xmpptask.cc',
]

darwin_src = [
    'src/macconversion.cc',
    'src/macutils.cc',
]

posix_src = [
    'src/unixfilesystem.cc',
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
    config += ['_DEBUG']

if system == 'linux':
    config += ['LINUX']
    src += posix_src
elif system == 'darwin':
    config += ['OSX']
    frameworks += [
        'CoreServices',
        'Carbon',
        'Security',
        'SystemConfiguration',
    ]
    src += posix_src + darwin_src

CreateConfig(config)

env = Environment(
    CXXFLAGS=flags,
    FRAMEWORKS=frameworks,
)

conf = Configure(env)

if system in ('darwin', 'linux'):
    if not conf.CheckCXXHeader('expat.h'):
        Abort('expat not found')
    if not conf.CheckCXXHeader('pthread.h'):
        Abort('pthreads not found')
    if not conf.CheckCXXHeader('openssl/crypto.h'):
        Abort('openssl not found')

libtxmpp = env.SharedLibrary(name, src, CPPDEFINES=defines, LIBS=libraries)

if GetOption('examples'):
    hello_src = [
        'src/examples/hello/main.cc',
        'src/examples/hello/xmpppump.cc',
        'src/examples/hello/xmpptasks.cc',
        'src/examples/hello/xmppthread.cc',
    ]
    hello = env.Program(target='hello-example', source=hello_src, CPPDEFINES=defines, LIBS=libtxmpp)
