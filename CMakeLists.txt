# Full-featured Build System Sample
# CMake cross-platform build system recipe
# 2009 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(txmpp)

set(CPACK_PACKAGE_VENDOR "Ryan Pavlik")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-binary")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")

###
# Set up options
###

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	# Then we are our own project, not embedded
	option(INSTALL_TXMPP "Install the TXMPP headers and library." ON)
	
	option(BUILD_EXAMPLE "Build the example application." ON)
endif()

###
# Perform build configuration of dependencies
###

set(EXTRA_LIBS)

# Threading
if(UNIX AND NOT WIN32)
	find_package(Threads REQUIRED)
	list(APPEND EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT})
endif()

# Expat
find_package(EXPAT REQUIRED)
list(APPEND EXTRA_LIBS ${EXPAT_LIBRARIES})
include_directories(${EXPAT_INCLUDE_DIRS})

# OpenSSL
find_package(OpenSSL)
if(OPENSSL_FOUND)
	option(BUILD_WITH_OPENSSL "Build with SSL support using OpenSSL" ON)
	if(BUILD_WITH_OPENSSL)
		list(APPEND EXTRA_LIBS ${OPENSSL_LIBRARIES})
		include_directories(${OPENSSL_INCLUDE_DIRS})
		set(FEATURE_ENABLE_SSL 1)
		set(HAVE_OPENSSL_SSL_H 1)
		set(SSL_USE_OPENSSL 1)
		set(USE_SSLSTREAM 1) # what does this do?
	endif()
endif()

# Apple Frameworks
if(APPLE)
	foreach(framework CoreServices Carbon Security SystemConfiguration)
		find_library(MACOSX_${framework} ${framework})
		list(APPEND EXTRA_LIBS ${MACOSX_${framework}})
	endforeach()
endif()


# Platform Defines for the configuration file
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(LINUX 1)
elseif(APPLE)
	set(OSX 1)
endif()

if(WIN32)
	set(WIN32 1)
elseif(UNIX)
	set(POSIX 1)
endif()

if(MSVC)
	set(COMPILER_MSVC 1)
endif()

###
# Build the project
###

# The app is in the "src" subdirectory
add_subdirectory(src)
	
###
# Set packaging options (for CPack)
###

# Include the packaging system now that we have it all set up
include(CPack)

