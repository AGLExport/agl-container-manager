#-------------------------------------------------------------------------
# Makefile.am
#-------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = aglcontainermanager


aglcontainermanager_SOURCES = \
		ContainerConfig.cpp \
		ContainerElement.cpp \
		ContainerCommands.cpp \
		lxc-runtime.cpp \
		agl-container-manager.cpp 



aglcontainermanager_LDADD = \
		-lrt -lpthread \
		@JSONCPP_LIBS@ \
		@LXC_LIBS@ \
		@LIBNLCLI3_LIBS@ 


aglcontainermanager_CFLAGS = \
		-I$(top_srcdir)/include \
		-g \
		@JSONCPP_LIBS@ \
		@LXC_LIBS@ \
		@LIBNLCLI3_LIBS@ 


aglcontainermanager_CXXFLAGS = \
		-I$(top_srcdir)/include \
		-std=c++11 -g \
		@JSONCPP_LIBS@ \
		@LXC_LIBS@ \
		@LIBNLCLI3_LIBS@ 


aglcontainermanager_LDFLAGS = \
		-lrt -lpthread \
		@JSONCPP_LIBS@ \
		@LXC_LIBS@ \
		@LIBNLCLI3_LIBS@ 


aglcontainermanager_CFLAGS   += @JSONCPP_CFLAGS@ @LXC_CFLAGS@ @LIBNLCLI3_CFLAGS@ 
aglcontainermanager_CXXFLAGS += @JSONCPP_CFLAGS@ @LXC_CFLAGS@ @LIBNLCLI3_CFLAGS@ 

if ENABLE_USERDEBUG
aglcontainermanager_CFLAGS   += -D_USER_DEBUG_
aglcontainermanager_CXXFLAGS += -D_USER_DEBUG_
endif

if ENABLE_PRINTFDEBUG
aglcontainermanager_CFLAGS   += -D_PRINTF_DEBUG_
aglcontainermanager_CXXFLAGS += -D_PRINTF_DEBUG_
endif

