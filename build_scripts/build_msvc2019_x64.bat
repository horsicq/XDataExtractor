@echo off
REM Search for Visual Studio vcvarsall.bat in order of preference
set VCVARSALL_FOUND=0

REM Check VS 2022 (latest)
IF EXIST "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE IF EXIST "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE IF EXIST "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE IF EXIST "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE IF EXIST "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE IF EXIST "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" (
	call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VCVARSALL_FOUND=1
) ELSE (
	echo ERROR: Visual Studio vcvarsall.bat not found!
	echo Please install Visual Studio 2019 or 2022 with C++ tools.
	pause
	exit /b 1
)

IF %VCVARSALL_FOUND%==1 (
	echo Visual Studio environment initialized successfully.
) ELSE (
	echo ERROR: Failed to initialize Visual Studio environment.
	pause
	exit /b 1
)

REM Store the source directory before changing to temp build directory
set SOURCE_DIR=%CD%\..\
cd ..
set TEMP_BUILD_DIR=%TEMP%\tmp_build
mkdir "%TEMP_BUILD_DIR%"
cd "%TEMP_BUILD_DIR%"

REM Search for Qt MSVC installation
set QT_FOUND=0
set QT_PREFIX_PATH=

REM Check Qt 5.15.x versions with MSVC 2019
IF EXIST "C:\\Qt\\5.15.2\\msvc2019_64" (
	set QT_PREFIX_PATH=C:\\Qt\\5.15.2\\msvc2019_64
	set QT_FOUND=1
	echo Found Qt 5.15.2 MSVC 2019 64-bit
) ELSE IF EXIST "C:\\Qt\\5.15.1\\msvc2019_64" (
	set QT_PREFIX_PATH=C:\\Qt\\5.15.1\\msvc2019_64
	set QT_FOUND=1
	echo Found Qt 5.15.1 MSVC 2019 64-bit
) ELSE IF EXIST "C:\\Qt\\5.15.0\\msvc2019_64" (
	set QT_PREFIX_PATH=C:\\Qt\\5.15.0\\msvc2019_64
	set QT_FOUND=1
	echo Found Qt 5.15.0 MSVC 2019 64-bit
) ELSE IF EXIST "C:\\Qt\\5.14.2\\msvc2017_64" (
	set QT_PREFIX_PATH=C:\\Qt\\5.14.2\\msvc2017_64
	set QT_FOUND=1
	echo Found Qt 5.14.2 MSVC 2017 64-bit
) ELSE IF EXIST "C:\\Qt\\5.12.12\\msvc2017_64" (
	set QT_PREFIX_PATH=C:\\Qt\\5.12.12\\msvc2017_64
	set QT_FOUND=1
	echo Found Qt 5.12.12 MSVC 2017 64-bit
) ELSE (
	REM Check alternative installation paths
	FOR /D %%D IN ("C:\\Qt\\5.*") DO (
		IF EXIST "%%D\\msvc2019_64" (
			set QT_PREFIX_PATH=%%D\\msvc2019_64
			set QT_FOUND=1
			echo Found Qt in %%D\\msvc2019_64
			GOTO :qt_search_done
		)
		IF EXIST "%%D\\msvc2017_64" (
			set QT_PREFIX_PATH=%%D\\msvc2017_64
			set QT_FOUND=1
			echo Found Qt in %%D\\msvc2017_64
			GOTO :qt_search_done
		)
	)
)

:qt_search_done
IF %QT_FOUND%==0 (
	echo ERROR: Qt MSVC installation not found!
	echo Please install Qt 5.12+ with MSVC compiler support.
	echo Expected locations: C:\\Qt\\5.x.x\\msvc2019_64 or C:\\Qt\\5.x.x\\msvc2017_64
	pause
	exit /b 1
)

cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_PREFIX_PATH="%QT_PREFIX_PATH%" -G "NMake Makefiles" "%SOURCE_DIR%"
nmake
cpack

cd "%SOURCE_DIR%"
xcopy /y /E "%TEMP_BUILD_DIR%\packages\" packages\
