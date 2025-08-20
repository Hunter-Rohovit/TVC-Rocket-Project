echo off
set LOCALHOST=%COMPUTERNAME%
set KILL_CMD="C:\PROGRA~1\ANSYSI~1\ANSYSS~1\v252\fluent/ntbin/win64/winkill.exe"

start "tell.exe" /B "C:\PROGRA~1\ANSYSI~1\ANSYSS~1\v252\fluent\ntbin\win64\tell.exe" DESKTOP-K7OROKO 54981 CLEANUP_EXITING
timeout /t 1
"C:\PROGRA~1\ANSYSI~1\ANSYSS~1\v252\fluent\ntbin\win64\kill.exe" tell.exe
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 38676) 
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 40912) 
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 76528) 
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 43044) 
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 34780) 
if /i "%LOCALHOST%"=="DESKTOP-K7OROKO" (%KILL_CMD% 43360)
del "C:\Users\Hunter Rohovit\Desktop\TVC Rocket Project\Simulation\CFD\cleanup-fluent-DESKTOP-K7OROKO-34780.bat"
