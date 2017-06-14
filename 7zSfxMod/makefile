# CPU: x86, i386, x64, amd64
# MODULE_SUBNAME: lzma, lzma2, deflate, ppmd, all
# SFX_METHODS: -DCOMPRESS_COPY -DCOMPRESS_BCJ -DCOMPRESS_BCJ2 -DCOMPRESS_DELTA -DCOMPRESS_LZMA -DCOMPRESS_LZMA2 -DCOMPRESS_DEFLATE -DCOMPRESS_PPMD
# METHODS_OPTIMIZATION: -O1 | -O2

LINK = link
!IFDEF USE_CLANG
CC = clang
!ELSE
!IFDEF USE_GCC
# may be gcc here
!ELSE
USE_MSVC = 1
!ENDIF
!ENDIF

!IFNDEF CPU
CPU=x86
!ENDIF

!IFDEF _SFX_MAINTAINER
CFLAGS = $(CFLAGS) -D_SFX_MAINTAINER
!ENDIF

!IFDEF SFX_VOLUMES
CFLAGS = $(CFLAGS) -DSFX_VOLUMES
!ENDIF

!IFDEF SFX_CRYPTO
CFLAGS = $(CFLAGS) -DSFX_CRYPTO
!ENDIF

!IFDEF COMPRESS_COPY
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_COPY
!ENDIF

!IFDEF COMPRESS_BCJ
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_BCJ
!ENDIF

!IFDEF COMPRESS_BCJ2
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_BCJ2
!ENDIF

!IFDEF COMPRESS_DELTA
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_DELTA
!ENDIF

!IFDEF COMPRESS_DEFLATE
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_DEFLATE
!ENDIF

!IFDEF COMPRESS_LZMA
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_LZMA
!ENDIF

!IFDEF COMPRESS_LZMA2
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_LZMA2
!ENDIF

!IFDEF COMPRESS_PPMD
SFX_METHODS = $(SFX_METHODS) -DCOMPRESS_PPMD
!ENDIF

!IFDEF SFXBUILD_OPTIONS1
CFLAGS= $(CFLAGS) -DSFXBUILD_OPTIONS1=$(SFXBUILD_OPTIONS1)
!ENDIF

!IFDEF SFXBUILD_OPTIONS2
CFLAGS= $(CFLAGS) -DSFXBUILD_OPTIONS2=$(SFXBUILD_OPTIONS2)
!ENDIF

!IFDEF SFXBUILD_OPTIONS3
CFLAGS= $(CFLAGS) -DSFXBUILD_OPTIONS3=$(SFXBUILD_OPTIONS3)
!ENDIF

!IFNDEF PLATFORM_BITS
!IF "$(CPU)" == "x64" || "$(CPU)" == "amd64"
PLATFORM_BITS=64
RFLAGS=/d _WIN64
SIGNNAME=$(MODULE_SUBNAME) amd64
!ELSE
PLATFORM_BITS=32
SIGNNAME=$(MODULE_SUBNAME) x86
!ENDIF
!ENDIF

!IFNDEF MODULE_SUBNAME
MODULE_NAME=7zsd
!ELSE
MODULE_NAME=7zsd_$(MODULE_SUBNAME)
!ENDIF

!IF $(PLATFORM_BITS) == 64
MODULE_NAME=$(MODULE_NAME)_x64
!ENDIF

!IFNDEF METHODS_OPTIMIZATION
METHODS_OPTIMIZATION = -O1
!ENDIF

CFLAGS = $(CFLAGS)			\
	-c				\
	-D_WIN32			\
	-DUNICODE			\
	-D_UNICODE			\
	-DEXTRACT_ONLY			\
	-D_NO_EXCEPTIONS		\
	-D_SFX				\
	-D_MT				\
	-D_DLL				\
	-I ../7-Zip/CPP			\
	$(SFX_METHODS)

!IFDEF USE_MSVC
CFLAGS=$(CFLAGS)			\
	-nologo				\
	-WX -GR- -Zl -GS- -MD		\
	-GF -Gy -wd4530 		\
	-Oi -Gr
!ENDIF

!IFDEF USE_CLANG
# CLANG
CFLAGS=$(CFLAGS)			\
	-I sources			\
	-fexceptions			\
	-Wno-invalid-source-encoding	\
	-Wno-microsoft-exception-spec	\
	-Wno-reserved-id-macro		\
	-Os -fno-rtti 		\
	-fno-stack-protector 		\
	-fomit-frame-pointer 		\
	-ffunction-sections		\
	-fno-standalone-debug		\
	-fno-unwind-tables		\
	-fno-asynchronous-unwind-tables  \
	-fms-compatibility
!ENDIF


LFLAGS = -nologo -opt:REF -opt:ICF -subsystem:WINDOWS
RFLAGS = $(RFLAGS) -nologo -l0

!IF "$(CPU)" == "x64" || "$(CPU)" == "amd64"
CFLAGS=$(CFLAGS) -D_WIN64 -D_AMD64_ -D_M_AMD64
!IFDEF USE_CLANG
CFLAGS = $(CFLAGS) -target amd64-pc-win64
!ENDIF
LFLAGS = $(LFLAGS) -machine:x64
MY_ML=ml64
MLFLAGS=$(MLFLAGS) -Dx64
!ELSE
!IFDEF USE_CLANG
CFLAGS = $(CFLAGS) -target i386-pc-win32
!ENDIF
LFLAGS = $(LFLAGS) -machine:x86
MY_ML=ml
!ENDIF

!IFDEF _MAPFILE
LFLAGS = $(LFLAGS) /MAP:$(OUTPUT)\$(MODULE_NAME).map
!ENDIF

!IFDEF _SFXTOOLS
RFLAGS = $(RFLAGS) /D_SFXTOOLS
!ENDIF

INTERMEDIATE=..\Intermediate\$(MODULE_NAME)
OUTPUT=..\output
PROGNAME=$(MODULE_NAME).sfx
PROG=$(OUTPUT)\$(PROGNAME)
MANIFEST_FILE=$(SOURCES_SFX)\res\manifest$(PLATFORM_BITS).xml

# custom
MKDIR=mkdir
SOURCES_SFX=sources
SOURCES_7ZIP=..\7-Zip

#########################################################################################################################
COMPL_PCH1 = @$(CC) $(CFLAGS) -O1 -Yc"StdAfx.h" -Fp$(INTERMEDIATE)/stdafx1.pch -Fo$(INTERMEDIATE)/stdafx1.obj $**
COMPL_PCH2 = @$(CC) $(CFLAGS) $(METHODS_OPTIMIZATION) -Yc"StdAfx.h" -Fp$(INTERMEDIATE)/stdafx2.pch -Fo$(INTERMEDIATE)/stdafx2.obj $**

!IFDEF USE_CLANG
# CLANG
COMPL_CPP = @echo $** && @$(CC) $(CFLAGS) -o$(INTERMEDIATE)/$(*B).obj $**
COMPL_METHODS = @$(CC) $(CFLAGS) $(METHODS_OPTIMIZATION) -o$(INTERMEDIATE)/$(*B).obj $**
COMPL_C   = @$(CC) $(CFLAGS) -o$(INTERMEDIATE)/$(*B).obj $**
!ELSE
# MSVC
COMPL_CPP = @$(CC) $(CFLAGS) -O1 -Fo$(INTERMEDIATE)/ $**
COMPL_METHODS = @$(CC) $(CFLAGS) $(METHODS_OPTIMIZATION) -Yu"StdAfx.h" -Fp$(INTERMEDIATE)/stdafx2.pch -Fo$(INTERMEDIATE)/ $**
COMPL_C   = @$(CC) $(CFLAGS) $(METHODS_OPTIMIZATION) -Fo$(INTERMEDIATE)/ $**
!ENDIF

COMPL_ASM = @$(MY_ML) $(MLFLAGS) -nologo -c -Fo$(INTERMEDIATE)/ $**
COMPL_RC  = @rc $(RFLAGS) -fo$@ $**

SFX_OBJS = 					\
	$(INTERMEDIATE)\archive.obj		\
	$(INTERMEDIATE)\cmdline.obj		\
	$(INTERMEDIATE)\Config.obj		\
	$(INTERMEDIATE)\Exceptions.obj		\
	$(INTERMEDIATE)\execute.obj		\
	$(INTERMEDIATE)\ExtractEngine.obj	\
	$(INTERMEDIATE)\Helpers.obj		\
	$(INTERMEDIATE)\LangStrs.obj		\
	$(INTERMEDIATE)\main.obj		\
	$(INTERMEDIATE)\Sfx7zHacks.obj		\
	$(INTERMEDIATE)\sfxapi.obj		\
	$(INTERMEDIATE)\SfxDialogs.obj		\
	$(INTERMEDIATE)\SfxInStream.obj		\
	$(INTERMEDIATE)\SfxTest.obj		\
	$(INTERMEDIATE)\strings.obj		\

SFX_OBJS_METHODS =				\
	$(INTERMEDIATE)\AES.obj			\
	$(INTERMEDIATE)\BCJ.obj			\
	$(INTERMEDIATE)\BCJ2.obj		\
	$(INTERMEDIATE)\Copy.obj		\
	$(INTERMEDIATE)\Deflate.obj		\
	$(INTERMEDIATE)\Delta.obj		\
	$(INTERMEDIATE)\LZMA.obj		\
	$(INTERMEDIATE)\LZMA2.obj		\
	$(INTERMEDIATE)\PPMd.obj		\

SFX_RESOURCES =					\
	$(INTERMEDIATE)\7zSfxMod.res		\
	
7ZIP_OBJS_WINDOWS =				\
	$(INTERMEDIATE)\FileIO.obj		\
	$(INTERMEDIATE)\PropVariant.obj		\

7ZIP_OBJS_COMMON =				\
	$(INTERMEDIATE)\MyString.obj		\
	$(INTERMEDIATE)\MyVector.obj		\
	$(INTERMEDIATE)\WildCard.obj		\

7ZIP_OBJS_7ZIP_COMMON =				\
	$(INTERMEDIATE)\CreateCoder.obj		\
	$(INTERMEDIATE)\FileStreams.obj		\
	$(INTERMEDIATE)\FilterCoder.obj		\
	$(INTERMEDIATE)\InBuffer.obj		\
	$(INTERMEDIATE)\LimitedStreams.obj	\
	$(INTERMEDIATE)\LockedStream.obj	\
	$(INTERMEDIATE)\MethodId.obj		\
	$(INTERMEDIATE)\OutBuffer.obj		\
	$(INTERMEDIATE)\ProgressUtils.obj	\
	$(INTERMEDIATE)\StreamBinder.obj	\
	$(INTERMEDIATE)\StreamObjects.obj	\
	$(INTERMEDIATE)\StreamUtils.obj		\
	$(INTERMEDIATE)\VirtThread.obj		\

7ZIP_OBJS_7ZIP_ARCHIVE_7Z =			\
	$(INTERMEDIATE)\7zDecode.obj		\
	$(INTERMEDIATE)\7zExtract.obj		\
	$(INTERMEDIATE)\7zHandler.obj		\
	$(INTERMEDIATE)\7zIn.obj		\

7ZIP_OBJS_7ZIP_ARCHIVE_COMMON =			\
	$(INTERMEDIATE)\CoderMixer2.obj		\
	$(INTERMEDIATE)\ItemNameUtils.obj	\
	$(INTERMEDIATE)\OutStreamWithCRC.obj	\

7ZIP_OBJS_C =					\
	$(INTERMEDIATE)\7zCrc.obj		\
	$(INTERMEDIATE)\Alloc.obj		\
	$(INTERMEDIATE)\CpuArch.obj		\

7ZIP_OBJS_ASM =					\
	$(INTERMEDIATE)\7zCrcOpt.obj		\
	$(INTERMEDIATE)\AesOpt.obj		\

!IFDEF USE_MSVC
OBJS =						\
	$(INTERMEDIATE)\StdAfx1.obj		\
	$(INTERMEDIATE)\StdAfx2.obj
!ENDIF

OBJS =	$(OBJS)					\
	$(SFX_OBJS)				\
	$(SFX_OBJS_METHODS)			\
	$(7ZIP_OBJS_WINDOWS)			\
	$(7ZIP_OBJS_COMMON)			\
	$(7ZIP_OBJS_7ZIP_COMMON)		\
	$(7ZIP_OBJS_7ZIP_ARCHIVE_7Z)		\
	$(7ZIP_OBJS_7ZIP_ARCHIVE_COMMON)	\
	$(7ZIP_OBJS_C)				\
	$(7ZIP_OBJS_ASM)			\
	$(SFX_RESOURCES)			\

LIBS =						\
	comctl32.lib				\
	shell32.lib				\
	gdi32.lib				\
	advapi32.lib				\
	user32.lib				\
	ole32.lib				\
	oleaut32.lib				\
	kernel32.lib				\

build: $(PROG)

$(INTERMEDIATE)\StdAfx1.obj: $(SOURCES_SFX)/stdafx.cpp
	$(COMPL_PCH1)

$(INTERMEDIATE)\StdAfx2.obj: $(SOURCES_SFX)/stdafx.cpp
	$(COMPL_PCH2)

$(SFX_OBJS): $(SOURCES_SFX)/$(*B).cpp
	$(COMPL_CPP)

$(SFX_OBJS_METHODS): $(SOURCES_SFX)/methods/$(*B).cpp
	$(COMPL_METHODS)

$(INTERMEDIATE)\7zSfxMod.res: $(SOURCES_SFX)/7zSfxMod.rc
	$(COMPL_RC)

$(7ZIP_OBJS_WINDOWS): $(SOURCES_7ZIP)\CPP\Windows\$(*B).cpp
	$(COMPL_CPP)

$(7ZIP_OBJS_COMMON): $(SOURCES_7ZIP)\CPP\Common\$(*B).cpp
	$(COMPL_CPP)

$(7ZIP_OBJS_7ZIP_COMMON): $(SOURCES_7ZIP)\CPP\7zip\Common\$(*B).cpp
	$(COMPL_CPP)

$(7ZIP_OBJS_7ZIP_ARCHIVE_7Z): $(SOURCES_7ZIP)\CPP\7zip\Archive\7z\$(*B).cpp
	$(COMPL_CPP)

$(7ZIP_OBJS_7ZIP_ARCHIVE_COMMON): $(SOURCES_7ZIP)\CPP\7zip\Archive\Common\$(*B).cpp
	$(COMPL_CPP)

$(7ZIP_OBJS_C): $(SOURCES_7ZIP)\C\$(*B).c
	$(COMPL_C)

$(7ZIP_OBJS_ASM): $(SOURCES_7ZIP)\Asm\x86\$(*B).asm
	$(COMPL_ASM)

clean:
	@echo Clearing ...
	-@del /Q $(PROG) $(INTERMEDIATE)\*.obj $(INTERMEDIATE)\*.exp $(INTERMEDIATE)\*.res $(INTERMEDIATE)\*.pch 2>NUL

rebuild: clean build


$(INTERMEDIATE):
	@if not exist "$(INTERMEDIATE)" (
		@echo Creating $(INTERMEDIATE)
		@$(MKDIR) "$(INTERMEDIATE)"
	@)

$(OUTPUT):
	@if not exist "$(OUTPUT)" (
		@echo Creating $(OUTPUT)
		@$(MKDIR) "$(OUTPUT)"
	@)


$(PROG): $(INTERMEDIATE) $(OBJS) $(OUTPUT)
	@echo Linking ...
	@$(LINK) $(LFLAGS) -out:$(PROG) -STUB:$(SOURCES_SFX)/small.stub -RELEASE $(OBJS) $(LIBS)
