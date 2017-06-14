@ECHO off
SETLOCAL ENABLEDELAYEDEXPANSION

SET DEVELOP_WINSDK_ROOT=C:\Program Files (x86^)\Microsoft SDKs\Windows\v7.0A
SET DEVELOP_VS_ROOT=C:\Program Files (x86^)\Microsoft Visual Studio 10.0\VC
SET Path="%DEVELOP_VS_ROOT%\Bin\amd64";"%DEVELOP_WINSDK_ROOT%\Bin\x64";%SystemRoot%;%SystemRoot%\system32

SET INCLUDE_WINSDK=%DEVELOP_WINSDK_ROOT%\Include
SET INCLUDE_VS=%DEVELOP_VS_ROOT%\include
SET INCLUDE_VS_MFC=%DEVELOP_VS_ROOT%\atlmfc\include
SET INCLUDE=%INCLUDE_VS%;%INCLUDE_VS_MFC%;%INCLUDE_WINSDK%;
::SET C_INCLUDE_PATH=%Include%
::SET CPLUS_INCLUDE_PATH=%Include%

SET LIB_WINSDK=%DEVELOP_WINSDK_ROOT%\Lib
SET LIB_VS=%DEVELOP_VS_ROOT%\lib
SET LIB_VS_MFC=%DEVELOP_VS_ROOT%\atlmfc\lib
SET TARGET_CPU=
SET MODULE_SUBNAMES=
SET BUILD_OPTIONS=
SET COMPILER=USE_MSVC

:Parse_Args

IF /I "%~1"=="/x86"        SET TARGET_CPU=%TARGET_CPU% x86     & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/I386"       SET TARGET_CPU=%TARGET_CPU% x86     & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/x64"        SET TARGET_CPU=%TARGET_CPU% x64     & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/amd64"      SET TARGET_CPU=%TARGET_CPU% x64     & SHIFT & GOTO Parse_Args

IF /I "%~1"=="/deflate"    SET MODULE_SUBNAMES=%MODULE_SUBNAMES% Deflate            & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/lzma"       SET MODULE_SUBNAMES=%MODULE_SUBNAMES% LZMA               & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/lzma2"      SET MODULE_SUBNAMES=%MODULE_SUBNAMES% LZMA2              & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/ppmd"       SET MODULE_SUBNAMES=%MODULE_SUBNAMES% PPMd               & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/All"        SET MODULE_SUBNAMES=%MODULE_SUBNAMES% All                & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/Dialogs"    SET MODULE_SUBNAMES=%MODULE_SUBNAMES% LZMA_Dialogs       & SHIFT & GOTO Parse_Args

IF /I "%~1"=="/size"       SET BUILD_OPTIONS=%BUILD_OPTIONS% METHODS_OPTIMIZATION=-O1                    & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/speed"      SET BUILD_OPTIONS=%BUILD_OPTIONS% METHODS_OPTIMIZATION=-O2                    & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/mapfile"    SET BUILD_OPTIONS=%BUILD_OPTIONS% _MAPFILE=1                                  & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/crypto"     SET BUILD_OPTIONS=%BUILD_OPTIONS% SFX_CRYPTO=1                                & SHIFT & GOTO Parse_Args
IF /I "%~1"=="/volumes"    SET BUILD_OPTIONS=%BUILD_OPTIONS% SFX_VOLUMES=1 _SFX_USE_VOLUME_NAME_STYLE=1  & SHIFT & GOTO Parse_Args

IF /I "%~1"=="/clang"      SET COMPILER=USE_CLANG              & SHIFT & GOTO Parse_Args

IF /I "%~1"=="-h"          GOTO Error_Usage
IF /I "%~1"=="/?"          GOTO Error_Usage
IF /I "x%~1" == "x"        GOTO process_make
SET NMAKE_CMDS=%NMAKE_CMDS% %1
SHIFT
GOTO Parse_args

:process_make
IF "x%TARGET_CPU%" == "x" SET TARGET_CPU=x86 x64
IF "x%MODULE_SUBNAMES%" == "x" SET MODULE_SUBNAMES=Deflate LZMA LZMA2 PPMd All LZMA_Dialogs
SET %COMPILER%=1

FOR %%c IN (%TARGET_CPU%) DO (
    FOR %%m IN (%MODULE_SUBNAMES%) DO (
        IF "%%m" == "All"          SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_BCJ=1 COMPRESS_BCJ2=1 COMPRESS_DELTA=1 COMPRESS_LZMA=1 COMPRESS_LZMA2=1 COMPRESS_DEFLATE=1 COMPRESS_PPMD=1
        IF "%%m" == "Deflate"      SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_DEFLATE=1
        IF "%%m" == "LZMA"         SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_BCJ=1 COMPRESS_BCJ2=1 COMPRESS_DELTA=1 COMPRESS_LZMA=1
        IF "%%m" == "LZMA2"        SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_BCJ=1 COMPRESS_BCJ2=1 COMPRESS_DELTA=1 COMPRESS_LZMA=1 COMPRESS_LZMA2=1
        IF "%%m" == "PPMd"         SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_PPMD=1
        IF "%%m" == "LZMA_Dialogs" SET COMPRESSION_METHODS=COMPRESS_COPY=1 COMPRESS_BCJ=1 COMPRESS_BCJ2=1 COMPRESS_DELTA=1 COMPRESS_LZMA=1 _SFXTOOLS=1
        ECHO ===============================
        ECHO %%m-%%c
        ECHO ===============================

        IF "%%c" == "x86" (
            SET LIB=!LIB_WINSDK!;!LIB_VS!;!LIB_VS_MFC!;
        ) ELSE (
            SET LIB=!LIB_WINSDK!\x64;!LIB_VS!\amd64;!LIB_VS_MFC!\amd64;
        )

        nmake -nologo SFX_CRYPTO=1 SFX_VOLUMES=1 _SFX_MAINTAINER=1 CPU=%%c MODULE_SUBNAME=%%m !COMPRESSION_METHODS! %BUILD_OPTIONS% %NMAKE_CMDS%
    )
)

GOTO end

:Error_Usage
ECHO Usage: "Make7zsd [/x86|/x64] [/deflate|/lzma|/lzma2|/ppmd|/all|/Dialogs] [/size|/speed] [/mapfile] [/clang] [-h | /?] [make_options]"
ECHO.
ECHO                 /x86     - Create 32-bit x86 applications
ECHO                 /x64     - Create 64-bit x64 applications

:end

