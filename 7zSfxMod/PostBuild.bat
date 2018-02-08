@echo on

if not exist %1 rem: goto :usage
if not exist %2 rem: goto :usage

REM Enable echo lines below if you need to debug this script
REM echo %0
REM echo $(IntDir) = %1
REM echo $(OutDir) = %2
REM echo $(PlatformName) = %3
REM echo $(Configuration) = %4

goto :PlatformName$%3

:PlatformName$Win32
ping 127.0.0.1 -n 2 -w 1000
editbin /subsystem:windows,4.0 /osversion:4.0 "%~27zsd_%4.exe"
goto :PlatformName$

:PlatformName$x64
ping 127.0.0.1 -n 2 -w 1000
editbin /subsystem:windows,4.0 /osversion:4.0 "%~27zsd_%4_x64.exe"
goto :PlatformName$

:PlatformName$

exit

:usage
echo ############################################################################
echo # Merge.vcproj post-build script                                           #
echo # Not intended for direct invocation through user interface                #
echo # Post-build command line:                                                 #
echo # PostBuild.bat "$(IntDir)" "$(OutDir)" $(PlatformName) $(Configuration)   #
echo ############################################################################
pause
