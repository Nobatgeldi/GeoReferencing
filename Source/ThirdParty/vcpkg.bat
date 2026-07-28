@echo off

setlocal

:: this is a tag in the vcpkg repository
set VCPKG_VERSION=2025.12.12

:: this is where the artifacts get installed
set VCPKG_INSTALLED=vcpkg-installed

:: setup by Engine\Android\SetupAndroid.bat
set ANDROID_NDK_HOME=%NDKROOT%

:: cleanup the git repo
if exist "%~dp0vcpkg\" echo:
if exist "%~dp0vcpkg\" echo === Tidying up vcpkg ===
if exist "%~dp0vcpkg\" rmdir /s /q "%~dp0vcpkg"

:: cleanup the prior artifacts
if exist "%~dp0%VCPKG_INSTALLED%\" echo:
if exist "%~dp0%VCPKG_INSTALLED%\" echo === Tidying up %VCPKG_INSTALLED% ===
if exist "%~dp0%VCPKG_INSTALLED%\" rmdir /s /q "%~dp0%VCPKG_INSTALLED%"

echo:
echo === Cloning vcpkg to %~dp0vcpkg ===
git clone https://github.com/microsoft/vcpkg.git --depth 1 --branch %VCPKG_VERSION% "%~dp0vcpkg"

echo:
echo === Bootstrapping vcpkg ===
:: -disableMetrics in important to avoid Malwarebytes quarantine the vcpkg file. 
call "%~dp0vcpkg\bootstrap-vcpkg.bat" -disableMetrics

:: -------------------------------------------------------------------------
:: 7-zip.org only hosts the CURRENT release now; it 404s on old version
:: archives like 7z2107-extra.7z that this pinned vcpkg-tool version needs.
:: Pre-seed the download cache from the official GitHub mirror (ip7z/7zip)
:: so vcpkg.exe finds it locally and skips the dead 7-zip.org URL.
:: -------------------------------------------------------------------------
echo:
echo === Pre-fetching 7zip (7-zip.org no longer hosts old versions) ===
if not exist "%~dp0vcpkg\downloads\" mkdir "%~dp0vcpkg\downloads"
if not exist "%~dp0vcpkg\downloads\7z2107-extra.7z" (
    powershell -NoProfile -ExecutionPolicy Bypass -Command ^
        "$ProgressPreference='SilentlyContinue'; Invoke-WebRequest -UseBasicParsing -Uri 'https://github.com/ip7z/7zip/releases/download/21.07/7z2107-extra.7z' -OutFile '%~dp0vcpkg\downloads\7z2107-extra.7z'"
    if ERRORLEVEL 1 (
        echo:
        echo === WARNING: could not pre-fetch 7zip from GitHub mirror, falling back to SourceForge ===
        powershell -NoProfile -ExecutionPolicy Bypass -Command ^
            "$ProgressPreference='SilentlyContinue'; Invoke-WebRequest -UseBasicParsing -Uri 'https://sourceforge.net/projects/sevenzip/files/7-Zip/21.07/7z2107-extra.7z/download' -OutFile '%~dp0vcpkg\downloads\7z2107-extra.7z'"
    )
)

:: build for each triplet
:: --editable leaves the source in the buildtree for easy local debugging and patch generation
for %%x in (overlay-x64-windows overlay-x64-uwp overlay-arm64-uwp x64-android arm64-android) do (
    echo:
    echo === Running vcpkg for triplet %%x ===
	echo:
    "%~dp0vcpkg\vcpkg.exe" install --editable --x-install-root="%~dp0%VCPKG_INSTALLED%" --overlay-triplets=./overlay-triplets --triplet=%%x "proj[core]"
    if ERRORLEVEL 1 exit /b 1

    echo:
    echo === Reconciling %VCPKG_INSTALLED% artifacts for triplet %%x ===
    for /f "delims=" %%f in ("%~dp0%VCPKG_INSTALLED%\%%x") do p4 reconcile "%%~ff\..."
)

echo:
echo === Refreshing PROJ data files ===

:: destroy the target
attrib -r "%~dp0..\..\Resources\PROJ\*.*" /s
rmdir /s /q "%~dp0..\..\Resources\PROJ"

:: copy the files
robocopy /MIR /MT "%~dp0%VCPKG_INSTALLED%\overlay-x64-windows\share\proj" "%~dp0..\..\Resources\PROJ"

:: delete some extra stuff
del "%~dp0..\..\Resources\PROJ\*.cmake"
del "%~dp0..\..\Resources\PROJ\vcpkg*.*"

:: reconcile in p4 (for /f will handle relative paths that p4 can't handle)
for /f "delims=" %%f in ("%~dp0..\..\Resources\PROJ") do p4 reconcile "%%~ff\..."

echo:
echo === DONE ===

endlocal