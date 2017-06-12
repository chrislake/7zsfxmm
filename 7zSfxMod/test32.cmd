@echo off

set modules=LZMA LZMA2 Deflate PPMd All LZMA_Dialogs

cd ..
FOR %%a in (%modules%) DO (
echo output\7zsd_%%a.sfx -sfxversion
output\7zsd_%%a.sfx -sfxversion
)
