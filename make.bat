@echo off
echo Be sure you have MinGW-w64 installed via msys64 in C:\msys64\mingw64, glfw3 precompiled libraries (the mingw ones) in C:\msys64\glfw\ and make in C:\msys64\usr\bin
echo Building rmold3D for Microsoft Windows     
C:\msys64\usr\bin\make -fMakefileWin64 game
copy C:\msys64\glfw\glfw3.dll .