@echo off
echo ========================================
echo Building FinSync...
echo ========================================

REM Check if Visual Studio is installed
where cl.exe >nul 2>nul
if %ERRORLEVEL% EQU 0 goto :build_with_cl

REM Check if MinGW is installed
where g++.exe >nul 2>nul
if %ERRORLEVEL% EQU 0 goto :build_with_mingw

echo Error: No C++ compiler found!
echo Please install Visual Studio or MinGW-w64
pause
exit /b 1

:build_with_cl
echo Using Microsoft Visual C++ Compiler...
cl.exe /EHsc /std:c++17 /D UNICODE /D _UNICODE FinSyncWin32_Fixed.cpp /Fe:FinSync.exe /link user32.lib gdi32.lib comctl32.lib
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful! Run FinSync.exe
    echo ========================================
    del *.obj
) else (
    echo Build failed!
)
pause
exit /b

:build_with_mingw
echo Using MinGW-w64 Compiler...
g++.exe -std=c++17 -municode -mwindows FinSyncWin32_Fixed.cpp -o FinSync.exe -lcomctl32 -lgdi32
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful! Run FinSync.exe
    echo ========================================
) else (
    echo Build failed!
)
pause
exit /b
