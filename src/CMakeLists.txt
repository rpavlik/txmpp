
# Generate config.h
configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Common sources and API files
set(SOURCE
	asynchttprequest.cc
    asyncpacketsocket.cc
    asynctcpsocket.cc
    asyncudpsocket.cc
    autodetectproxy.cc
    base64.cc
    bytebuffer.cc
    checks.cc
    common.cc
    constants.cc
    diskcache.cc
    event.cc
    fileutils.cc
    firewallsocketserver.cc
    flags.cc
    helpers.cc
    httpbase.cc
    httpclient.cc
    httpcommon.cc
    httprequest.cc
    jid.cc
    logging.cc
    md5c.c
    messagehandler.cc
    messagequeue.cc
    nethelpers.cc
    network.cc
    openssladapter.cc
    opensslidentity.cc
    opensslstreamadapter.cc
    pathutils.cc
    physicalsocketserver.cc
    prexmppauthimpl.cc
    proxydetect.cc
    proxyinfo.cc
    qname.cc
    ratelimitmanager.cc
    saslmechanism.cc
    signalthread.cc
    socketadapters.cc
    socketaddress.cc
    socketpool.cc
    ssladapter.cc
    sslidentity.cc
    sslsocketfactory.cc
    sslstreamadapter.cc
    stream.cc
    stringdigest.cc
    stringencode.cc
    stringutils.cc
    task.cc
    taskparent.cc
    taskrunner.cc
    thread.cc
    time.cc
    urlencode.cc
    xmlbuilder.cc
    xmlconstants.cc
    xmlelement.cc
    xmlnsstack.cc
    xmlparser.cc
    xmlprinter.cc
    xmppasyncsocketimpl.cc
    xmppclient.cc
    xmppengineimpl.cc
    xmppengineimpl_iq.cc
    xmpplogintask.cc
    xmppstanzaparser.cc
    xmpptask.cc)
    
set(API
	asyncfile.h
	asynchttprequest.h
	asyncpacketsocket.h
	asyncsocket.h
	asynctcpsocket.h
	asyncudpsocket.h
	autodetectproxy.h
	base64.h
	basicdefs.h
	basictypes.h
	bytebuffer.h
	byteorder.h
	checks.h
	common.h
	constants.h
	constructormagic.h
	criticalsection.h
	cryptstring.h
	diskcache.h
	Equifax_Secure_Global_eBusiness_CA-1.h
	event.h
	fileutils.h
	firewallsocketserver.h
	flags.h
	helpers.h
	httpbase.h
	httpclient.h
	httpcommon.h
	httpcommon-inl.h
	httprequest.h
	jid.h
	linked_ptr.h
	linux.h
	logging.h
	macconversion.h
	macutils.h
	md5.h
	messagehandler.h
	messagequeue.h
	nethelpers.h
	network.h
	openssladapter.h
	opensslidentity.h
	opensslstreamadapter.h
	pathutils.h
	physicalsocketserver.h
	plainsaslhandler.h
	prexmppauth.h
	prexmppauthimpl.h
	proxydetect.h
	proxyinfo.h
	qname.h
	ratelimitmanager.h
	saslcookiemechanism.h
	saslhandler.h
	saslmechanism.h
	saslplainmechanism.h
	schanneladapter.h
	scoped_ptr.h
	sec_buffer.h
	signalthread.h
	sigslot.h
	sigslotrepeater.h
	socketadapters.h
	socketaddress.h
	socketfactory.h
	socket.h
	socketpool.h
	socketserver.h
	socketstream.h
	ssladapter.h
	sslidentity.h
	sslsocketfactory.h
	sslstreamadapter.h
	stream.h
	stringdigest.h
	stringencode.h
	stringutils.h
	task.h
	taskparent.h
	taskrunner.h
	thread.h
	time.h
	urlencode.h
	xmlbuilder.h
	xmlconstants.h
	xmlelement.h
	xmlnsstack.h
	xmlparser.h
	xmlprinter.h
	xmppasyncsocket.h
	xmppasyncsocketimpl.h
	xmppclient.h
	xmppclientsettings.h
	xmppengine.h
	xmppengineimpl.h
	xmpplogintask.h
	xmppstanzaparser.h
	xmpptask.h
	${CMAKE_CURRENT_BINARY_DIR}/config.h)

###
# Per-platform sources and API files
###
if(WIN32)
	list(APPEND SOURCE
		win32.cc
		win32filesystem.cc
		win32securityerrors.cc
		win32socketinit.cc
		win32socketserver.cc
		win32window.cc
		winfirewall.cc
		winping.cc)
	list(APPEND API
		win32filesystem.h
		win32.h
		win32socketinit.h
		win32socketserver.h
		win32window.h
		winfirewall.h
		winping.h)

elseif(UNIX)
	list(APPEND SOURCE
		unixfilesystem.cc)
	list(APPEND API
		unixfilesystem.h)

	if(APPLE)
		list(APPEND SOURCE
			macconversion.cc
			macutils.cc)
		list(APPEND SOURCE
			macconversion.h
			macutils.h)
	endif()
endif()

###
# Creating the Target
###
add_library(txmpp ${SOURCE} ${API})
target_link_libraries(txmpp ${EXTRA_LIBS})

set_property(TARGET txmpp PROPERTY PUBLIC_HEADER ${API})
set_property(TARGET txmpp PROPERTY COMPILE_DEFINITIONS_DEBUG _DEBUG)

###
# Installation
###
if(INSTALL_TXMPP)
	install(TARGETS txmpp
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include/txmpp)
endif()

###
# Subdirectories
###
if(BUILD_EXAMPLE)
	add_subdirectory(examples/hello)
endif()


