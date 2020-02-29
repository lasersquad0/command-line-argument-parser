@ECHO OFF
setlocal EnableDelayedExpansion

set LOCAL_PATH=%~dp0
set "FILE_N=-[%~n0]:"

set BUILD_TYPE=Release
set REMOVE_INTERMEDIATE=false

:arg-parse
if not "%1"=="" (
    if "%1"=="--build-type" (
        set BUILD_TYPE=%2
        shift
    )

    if "%1"=="--clean" (
        set REMOVE_INTERMEDIATE=true
    )

    if "%1"=="-h" (
        goto help
    )

    if "%1"=="--help" (
        goto help
    )

    shift
    goto arg-parse
)

REM
REM Check if arguments are valid
REM
if %BUILD_TYPE% neq Release if %BUILD_TYPE% neq Debug (
    goto unknown_build_type
)

REM
REM Check if Visual Studio envirorment is loaded, if not, load it
REM
where cl >nul 2>nul
if %ERRORLEVEL% neq 0 (
    where vswhere >nul 2>nul
    if !ERRORLEVEL! neq 0 (
        goto vswhere_not_found
    )

    for /F "delims=" %%i in ('vswhere.exe -latest -property installationPath') do set VS_PATH=%%i
    call "!VS_PATH!\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
)

REM
REM Clean build directory
REM
if %REMOVE_INTERMEDIATE% == true (
    rmdir /s /q %BUILD_PATH% 2>nul
    del /s /q %BIN_PATH%*.exe 2>NUL

    del /s /q %BIN_PATH%*.pdb 2>NUL
    del /s /q %BIN_PATH%*.ilk 2>NUL
)

REM
REM Create build directories
REM
set BIN_PATH=%LOCAL_PATH%bin\%BUILD_TYPE%
set BUILD_PATH=%LOCAL_PATH%build\%BUILD_TYPE%

if not exist %BIN_PATH% MKDIR %BIN_PATH%
if not exist %BUILD_PATH% MKDIR %BUILD_PATH%

REM
REM Set compiler flags in function of the build type
REM
set CompilerFlags= /nologo /MP /EHsc /W4 /Oi /GR /Fo"%BUILD_PATH%\\" /Fe"%BIN_PATH%\\"
set LinkerFlags= /NOLOGO /SUBSYSTEM:CONSOLE /INCREMENTAL:NO

if %BUILD_TYPE% == Debug (
    set CompilerFlags= /Od /MTd /Z7 /GS /Gs /RTC1 !CompilerFlags! /Fd"%BIN_PATH%\\"
) else (
    set CompilerFlags= /WX /O2 /MT !CompilerFlags!
)

REM
REM Compile
REM
cl build.cpp %CompilerFlags% /link %LinkerFlags%
if %ERRORLEVEL% neq 0 (
    goto bad_exit
)

goto good_exit
REM ============================================================================
REM -- Messages and Errors -----------------------------------------------------
REM ============================================================================

:help
    echo build.bat [--build-type=Release^|Debug] [--clean]
    echo By default: --build-type=Release
    echo    --build-type: type of build, release or debug
    echo    --clean     : clean build directories
    goto good_exit

:vswhere_not_found
    echo .
    echo %FILE_N% [ERROR] vswhere.ext not found
    echo %FILE_N% [INFO ] Either you don't have it installed or is not in the environment path
    echo %FILE_N% [INFO ] Download link: https://github.com/microsoft/vswhere/releases
    goto bad_exit

:unknown_build_type
    echo.
    echo %FILE_N% [ERROR] Unknown build type
    echo %FILE_N% [INFO ] Allowd values are "Release" or "Debug"
    goto bad_exit

:good_exit
    endlocal
    exit /b 0

:bad_exit
    endlocal
    exit /b %ERRORLEVEL%
