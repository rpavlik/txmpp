set(SOURCES
	main.cc
	xmpppump.cc
	xmpppump.h
	xmpptasks.cc
	xmpptasks.h
	xmppthread.cc
	xmppthread.h)

add_executable(hello ${SOURCES})
target_link_libraries(hello txmpp)
