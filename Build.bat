
call "%~dp0\Vars.bat"

echo %UPROJECT_PATH%
call "%BUILD_PATH%" %PROJECT%Editor Win64 Development "%UPROJECT_PATH%" -waitmutex -NoHotReload