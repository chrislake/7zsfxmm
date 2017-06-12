@echo off

set modules=LZMA LZMA2 Deflate PPMd All LZMA_Dialogs

cd ..
FOR %%a in (%modules%) DO (
echo output\7zsd_%%a_x64.sfx -sfxversion
output\7zsd_%%a_x64.sfx -sfxlang:1033 -sfxversion
)
