# Overwrite this variable with your wxWidgets path
YOUR_WXDIR = C:\dev\wxWidgets-3.2.1

# Option at building your wxWidgets library
# (In my case, I build wxWidgets library with mingw-w64 "mingw32-make.exe SHELL=CMD.exe -f makefile.gcc BUILD=release UNICODE=1 SHARED=0 MONOLITHIC=1"
# See https://docs.wxwidgets.org/latest/plat_msw_install.html
BUILD = release
UNICODE = 1
SHARED = 0
MONOLITHIC = 1

# Compiler options
#CXXFLAGS = -std=c++20 -finput-charset=CP932 -fexec-charset=CP932 -Wno-sign-compare
CXXFLAGS = -std=c++20 -finput-charset=CP932 -fexec-charset=CP932
CPPFLAGS =
OPT_LINK = -static-libgcc -static-libstdc++

include $(YOUR_WXDIR)\build\msw\config.gcc

CPPDEPS = -MT$@ -MF$@.d -MD -MP
WX_RELEASE_NODOT = 32
COMPILER_PREFIX = gcc
OBJS = \
	$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = \
	$(YOUR_WXDIR)\lib\$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = \
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
FBEDITOR_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG_2) $(__THREADSFLAG) \
	-D__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) \
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) \
	$(__UNICODE_DEFINE_p) -I$(SETUPHDIR) -I$(YOUR_WXDIR)\include \
	$(____CAIRO_INCLUDEDIR_FILENAMES_p) -W -Wall -I. $(__DLLFLAG_p) \
	-DNOPCH $(__RTTIFLAG_5) $(__EXCEPTIONSFLAG_6) \
	-Wno-ctor-dtor-privacy $(CPPFLAGS) $(CXXFLAGS)

ifeq ($(USE_GUI),0)
PORTNAME = base
endif
ifeq ($(USE_GUI),1)
PORTNAME = msw$(TOOLKIT_VERSION)
endif
ifeq ($(OFFICIAL_BUILD),1)
COMPILER_VERSION = ERROR-COMPILER-VERSION-MUST-BE-SET-FOR-OFFICIAL-BUILD
endif
ifeq ($(BUILD),debug)
WXDEBUGFLAG = d
endif
ifeq ($(UNICODE),1)
WXUNICODEFLAG = u
endif
ifeq ($(WXUNIV),1)
WXUNIVNAME = univ
endif
ifeq ($(SHARED),1)
WXDLLFLAG = dll
endif
ifeq ($(SHARED),0)
LIBTYPE_SUFFIX = lib
endif
ifeq ($(SHARED),1)
LIBTYPE_SUFFIX = dll
endif
ifeq ($(MONOLITHIC),0)
EXTRALIBS_FOR_BASE = 
endif
ifeq ($(MONOLITHIC),1)
EXTRALIBS_FOR_BASE =   
endif
ifeq ($(BUILD),debug)
__OPTIMIZEFLAG_2 = -O0
endif
ifeq ($(BUILD),release)
__OPTIMIZEFLAG_2 = -O2
endif
ifeq ($(USE_RTTI),0)
__RTTIFLAG_5 = -fno-rtti
endif
ifeq ($(USE_RTTI),1)
__RTTIFLAG_5 = 
endif
ifeq ($(USE_EXCEPTIONS),0)
__EXCEPTIONSFLAG_6 = -fno-exceptions
endif
ifeq ($(USE_EXCEPTIONS),1)
__EXCEPTIONSFLAG_6 = 
endif
ifeq ($(WXUNIV),1)
__WXUNIV_DEFINE_p = -D__WXUNIVERSAL__
endif
ifeq ($(WXUNIV),1)
__WXUNIV_DEFINE_p_1 = --define __WXUNIVERSAL__
endif
ifeq ($(DEBUG_FLAG),0)
__DEBUG_DEFINE_p = -DwxDEBUG_LEVEL=0
endif
ifeq ($(DEBUG_FLAG),0)
__DEBUG_DEFINE_p_1 = --define wxDEBUG_LEVEL=0
endif
ifeq ($(BUILD),release)
__NDEBUG_DEFINE_p = -DNDEBUG
endif
ifeq ($(BUILD),release)
__NDEBUG_DEFINE_p_1 = --define NDEBUG
endif
ifeq ($(USE_EXCEPTIONS),0)
__EXCEPTIONS_DEFINE_p = -DwxNO_EXCEPTIONS
endif
ifeq ($(USE_EXCEPTIONS),0)
__EXCEPTIONS_DEFINE_p_1 = --define wxNO_EXCEPTIONS
endif
ifeq ($(USE_RTTI),0)
__RTTI_DEFINE_p = -DwxNO_RTTI
endif
ifeq ($(USE_RTTI),0)
__RTTI_DEFINE_p_1 = --define wxNO_RTTI
endif
ifeq ($(USE_THREADS),0)
__THREAD_DEFINE_p = -DwxNO_THREADS
endif
ifeq ($(USE_THREADS),0)
__THREAD_DEFINE_p_1 = --define wxNO_THREADS
endif
ifeq ($(UNICODE),0)
__UNICODE_DEFINE_p = -DwxUSE_UNICODE=0
endif
ifeq ($(UNICODE),1)
__UNICODE_DEFINE_p = -D_UNICODE
endif
ifeq ($(UNICODE),0)
__UNICODE_DEFINE_p_1 = --define wxUSE_UNICODE=0
endif
ifeq ($(UNICODE),1)
__UNICODE_DEFINE_p_1 = --define _UNICODE
endif
ifeq ($(USE_CAIRO),1)
____CAIRO_INCLUDEDIR_FILENAMES_p = -I$(CAIRO_ROOT)\include\cairo
endif
ifeq ($(USE_CAIRO),1)
__CAIRO_INCLUDEDIR_p = --include-dir $(CAIRO_ROOT)/include/cairo
endif
ifeq ($(SHARED),1)
__DLLFLAG_p = -DWXUSINGDLL
endif
ifeq ($(SHARED),1)
__DLLFLAG_p_1 = --define WXUSINGDLL
endif
ifeq ($(MONOLITHIC),0)
__WXLIB_CORE_p = \
	-lwx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core
endif
ifeq ($(MONOLITHIC),0)
__WXLIB_BASE_p = \
	-lwxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)
endif
ifeq ($(MONOLITHIC),1)
__WXLIB_MONO_p = \
	-lwx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)
endif
ifeq ($(MONOLITHIC),1)
ifeq ($(USE_STC),1)
__LIB_SCINTILLA_IF_MONO_p = -lwxscintilla$(WXDEBUGFLAG)
endif
endif
ifeq ($(USE_GUI),1)
__LIB_TIFF_p = -lwxtiff$(WXDEBUGFLAG)
endif
ifeq ($(USE_GUI),1)
__LIB_JPEG_p = -lwxjpeg$(WXDEBUGFLAG)
endif
ifeq ($(USE_GUI),1)
__LIB_PNG_p = -lwxpng$(WXDEBUGFLAG)
endif
ifeq ($(USE_CAIRO),1)
__CAIRO_LIB_p = -lcairo
endif
ifeq ($(USE_CAIRO),1)
____CAIRO_LIBDIR_FILENAMES_p = -L$(CAIRO_ROOT)\lib
endif
ifeq ($(BUILD),debug)
ifeq ($(DEBUG_INFO),default)
__DEBUGINFO = -g
endif
endif
ifeq ($(BUILD),release)
ifeq ($(DEBUG_INFO),default)
__DEBUGINFO = 
endif
endif
ifeq ($(DEBUG_INFO),0)
__DEBUGINFO = 
endif
ifeq ($(DEBUG_INFO),1)
__DEBUGINFO = -g
endif
ifeq ($(USE_THREADS),0)
__THREADSFLAG = 
endif
ifeq ($(USE_THREADS),1)
__THREADSFLAG = -mthreads
endif





all: $(OBJS)
$(OBJS):
	-if not exist $(OBJS) mkdir $(OBJS)

all: $(OBJS)\app.exe

clean: 
	-if exist $(OBJS)\*.o del $(OBJS)\*.o
	-if exist $(OBJS)\*.d del $(OBJS)\*.d
	-if exist $(OBJS)\app.exe del $(OBJS)\app.exe

$(OBJS)\app.exe: \
$(OBJS)\resource.o $(OBJS)\App.o $(OBJS)\InitialController.o $(OBJS)\InitialFrame.o $(OBJS)\CustomGrid.o $(OBJS)\GridDialog.o $(OBJS)\MyAboutDialogInfo.o $(OBJS)\FBParser.o $(OBJS)\FBAttrs.o
	$(CXX) -o $@ $^ $(__DEBUGINFO) $(__THREADSFLAG) -L$(LIBDIRNAME)  -Wl,--subsystem,windows -mwindows $(____CAIRO_LIBDIR_FILENAMES_p) $(LDFLAGS)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) -lwxzlib$(WXDEBUGFLAG) -lwxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG) -lwxexpat$(WXDEBUGFLAG) $(EXTRALIBS_FOR_BASE) $(__CAIRO_LIB_p) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lws2_32 -lwininet -loleacc -luxtheme $(OPT_LINK)

$(OBJS)\resource.o: .\resource\resource.rc
	$(WINDRES) -i$< -o$@    --define __WXMSW__ $(__WXUNIV_DEFINE_p_1) $(__DEBUG_DEFINE_p_1) $(__NDEBUG_DEFINE_p_1) $(__EXCEPTIONS_DEFINE_p_1) $(__RTTI_DEFINE_p_1) $(__THREAD_DEFINE_p_1) $(__UNICODE_DEFINE_p_1) --include-dir $(SETUPHDIR) --include-dir $(YOUR_WXDIR)\include $(__CAIRO_INCLUDEDIR_p) --include-dir . $(__DLLFLAG_p_1) --define wxUSE_DPI_AWARE_MANIFEST=$(USE_DPI_AWARE_MANIFEST) --define NOPCH

$(OBJS)\App.o: .\App.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<

$(OBJS)\InitialController.o: .\controller\initial\InitialController.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<

$(OBJS)\InitialFrame.o: .\view\initial\InitialFrame.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<	

$(OBJS)\CustomGrid.o: .\view\custom\CustomGrid.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<

$(OBJS)\GridDialog.o: .\view\custom\GridDialog.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<
	
$(OBJS)\MyAboutDialogInfo.o: .\view\custom\MyAboutDialogInfo.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<
	
$(OBJS)\FBParser.o: .\model\FBParser.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<
	
$(OBJS)\FBAttrs.o: .\common\FBAttrs.cpp
	$(CXX) -c -o $@ $(FBEDITOR_CXXFLAGS) $(CPPDEPS) $<

.PHONY: all clean

SHELL := $(COMSPEC)

-include $(OBJS)\*.d