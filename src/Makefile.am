#-------------------------------------------------------------------------
# 実行ファイル作成の例
#-------------------------------------------------------------------------

# この行は触らない
AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

#ビルドする実行ファイルの指定(exsample)
bin_PROGRAMS = aglcontainermanager


# ビルドする実行ファイル(exsample)を構成するソースファイルの指定
aglcontainermanager_SOURCES = \
		ContainerConfig.cpp \
		ConfigElement.cpp \
		lxc-runtime.cpp \
		agl-container-manager.cpp 



# ビルドする実行ファイル(exsample)のリンカに渡すライブラリの指定を行う。
# @XXX_LIBS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# この例はGLIBとZLIBを指定している。
aglcontainermanager_LDADD = \
		-lrt -lpthread \
		@JSONCPP_LIBS@ 


# ビルドする実行ファイル(exsample)のコンンパイラオプション(c)の指定を行う。
# @XXX_CFLAGS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# この例はGLIBとZLIBを指定している。
aglcontainermanager_CFLAGS = \
		-I$(top_srcdir)/include \
		-g \
		@JSONCPP_LIBS@ 

# ビルドする実行ファイル(exsample)のコンンパイラオプション(c++)の指定を行う。
# @XXX_CXXFLAGS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# CのライブラリはCXXFLAGSがないので、渡すパラメタはCFLAGSを指定する。
# この例はGLIBとZLIBを指定している。
aglcontainermanager_CXXFLAGS = \
		-I$(top_srcdir)/include \
		-std=c++11 -g \
		@JSONCPP_LIBS@ 

# ビルドする実行ファイル(exsample)のリンカオプションの指定を行う。
aglcontainermanager_LDFLAGS = \
		-lrt -lpthread \
		@JSONCPP_LIBS@


# configureオプションによって変更する設定の記述。
# この例は、configure.acで指定した、userdebugとprintfdebugの設定に応じて、defineマクロを設定している。
aglcontainermanager_CFLAGS   += @JSONCPP_CFLAGS@
aglcontainermanager_CXXFLAGS += @JSONCPP_CFLAGS@

if ENABLE_USERDEBUG
aglcontainermanager_CFLAGS   += -D_USER_DEBUG_
aglcontainermanager_CXXFLAGS += -D_USER_DEBUG_
endif

if ENABLE_PRINTFDEBUG
aglcontainermanager_CFLAGS   += -D_PRINTF_DEBUG_
aglcontainermanager_CXXFLAGS += -D_PRINTF_DEBUG_
endif

