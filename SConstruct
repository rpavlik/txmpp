import glob
import os
import platform
import shutil

#
# Command line options
#

AddOption(
    '--prefix',
    dest='prefix',
    type='string',
    nargs=1,
    action='store',
    default='/usr/local',
    metavar='DIR',
    help='The common installation prefix for all files.',
)

AddOption(
    '--includedir',
    dest='includedir',
    type='string',
    nargs=1,
    action='store',
    default='${PREFIX}/include',
    metavar='DIR',
    help='The directory for installing header files.',
)

AddOption(
    '--libdir',
    dest='libdir',
    type='string',
    nargs=1,
    action='store',
    default='${PREFIX}/lib',
    metavar='DIR',
    help='The directory for installing object code libraries.',
)

AddOption(
    '--flags',
    dest='flags',
    type='string',
    nargs=1,
    action='store',
    default='',
    metavar='DIR',
    help='compiler flags',
)

AddOption(
    '--install',
    dest='install',
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

AddOption(
    '--with-devel',
    dest='devel',
    action='store_true',
)

#
# Helper functions
#

def Abort(message):
    print message
    Exit(1)

def Copy(src, dst):
    print 'Copying %s to %s' % (src, dst)
    shutil.copy2(src, dst)

def CreateConfigHeader(define_list, path='src/config.h'):
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

def CreateDirectory(path):
    if not os.path.exists(path):
        print 'Creating directory %s' % path
        os.makedirs(path)

def Link(src, dst):
    print 'Linking %s to %s' % (src, dst)
    os.symlink(src, dst)

_push_pop = []

def Pushd(path):
    _push_pop.insert(0, os.getcwd())
    print 'Pushd to %s' % path
    os.chdir(path)

def Popd():
    if len(_push_pop) > 0:
        os.chdir(_push_pop[0])
        print 'Popd to %s' % _push_pop[0]
        del _push_pop[0]
    else:
        print 'Popd stack empty'

def Remove(path):
    for p in glob.glob(path):
        print 'Deleting %s' % p
        os.remove(p)


def SetupEnvironment(env):
    # Include paths
    path_list = []
    if 'C_INCLUDE_PATH' in os.environ:
        path_list.extend(os.environ['C_INCLUDE_PATH'].split(':'))
    if 'CPLUS_INCLUDE_PATH' in os.environ:
        path_list.extend(os.environ['CPLUS_INCLUDE_PATH'].split(':'))
    env.Append(CPPPATH=path_list)
    # Library paths
    path_list = []
    if 'LIBRARY_PATH' in os.environ:
        path_list.extend(os.environ['LIBRARY_PATH'].split(':'))
    env.Append(LIBPATH=path_list)

#
# Setup environment
#

env = Environment()

#
# Default settings
#

defines = [
    'FEATURE_ENABLE_SSL',
    'HAVE_OPENSSL_SSL_H',
    'POSIX',
    'SSL_USE_OPENSSL',
    'USE_SSLSTREAM',
]
flags = '-Wall'
frameworks = []
libraries = ['crypto', 'expat', 'pthread', 'ssl']
link = ''
name = 'txmpp'
prefix = GetOption('prefix')
system = platform.system().lower()
version = '0.0.2'

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
    'src/constants.cc',
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
    'src/jid.cc',
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
    'src/physicalsocketserver.cc',
    'src/prexmppauthimpl.cc',
    'src/proxydetect.cc',
    'src/proxyinfo.cc',
    'src/qname.cc',
    'src/ratelimitmanager.cc',
    'src/saslmechanism.cc',
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
    'src/xmlbuilder.cc',
    'src/xmlconstants.cc',
    'src/xmlelement.cc',
    'src/xmlnsstack.cc',
    'src/xmlparser.cc',
    'src/xmlprinter.cc',
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

#
# Apply various options
#

if GetOption('debug'):
    flags += ' -g'
    defines += ['_DEBUG']

if GetOption('flags'):
    flags += ' %s' % GetOption('flags')

if system == 'linux':
    defines += ['LINUX']
    soname = 'lib%s.so.%s' % (name, version)
    link += ' -Wl,-soname,%s' % soname
    src += posix_src
elif system == 'darwin':
    defines += ['OSX']
    soname = 'lib%s.dylib.%s' % (name, version)
    link += ' -compatibility_version %s' % version
    link += ' -current_version %s' % version
    frameworks += [
        'CoreServices',
        'Carbon',
        'Security',
        'SystemConfiguration',
    ]
    src += posix_src + darwin_src

if 'POSIX' in defines:
    flags += ' -pthread'
    libraries += ['pthread']

if not GetOption('devel'):
    flags += ' -O2'

if GetOption('flags'):
    env.Append(CFLAGS=GetOption('flags'))
    env.Append(CXXFLAGS=GetOption('flags'))

env.Append(CFLAGS=flags)
env.Append(CXXFLAGS=flags)
env.Append(FRAMEWORKS=frameworks)

SetupEnvironment(env)

#
# Create build configuration file
#

CreateConfigHeader(defines)

#
# Configure environment
#

conf = Configure(env)

if system in ('darwin', 'linux'):
    if not conf.CheckCXX():
        Abort('No C++ compiler found.')
    for library in libraries:
        if not conf.CheckLib(library):
            Abort('Unable to find library %s.' % name)

env = conf.Finish()

#
# Build library
#

txmpp_library = env.SharedLibrary(
    name,
    src,
    CPPDEFINES=defines,
    LIBS=libraries,
    LINKFLAGS=link,
)

#
# Build examples
#

if GetOption('examples'):
    hello_src = [
        'src/examples/hello/main.cc',
        'src/examples/hello/xmpppump.cc',
        'src/examples/hello/xmpptasks.cc',
        'src/examples/hello/xmppthread.cc',
    ]
    hello = env.Program(
        target='hello-example',
        source=hello_src,
        CPPDEFINES=defines,
        LIBS=txmpp_library,
    )

if GetOption('install'):

    includedir = GetOption('includedir').replace('${PREFIX}', prefix)
    includedir = os.path.join(includedir, name)
    libdir = GetOption('libdir').replace('${PREFIX}', prefix)

    txmpp_library = str(txmpp_library[0])

    # Install library
    CreateDirectory(libdir)
    Remove(os.path.join(libdir, 'libtxmpp.*'))
    Copy(txmpp_library, os.path.join(libdir, soname))
    Pushd(libdir)
    for x in range(1, 4):
        lname = '.'.join(soname.split('.')[:-x])
        Link(soname, lname)
    Popd()

    # Install header files
    CreateDirectory(includedir)
    Remove(os.path.join(includedir, '*.h'))
    for path in glob.glob(os.path.join('src', '*.h')):
        Copy(path, includedir)
