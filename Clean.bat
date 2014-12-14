@echo off
echo ------------------------------------------
echo Clean Solution
echo.
echo Created 15 Jan 2010 by E
echo Updated 19 Apr 2014 by E
echo.
echo Removes Visual Studio cached data
echo ------------------------------------------
echo.
echo Cleaning...
del "*.ncb" /s
del "*.sdf" /s 
del "*.suo" /s /a
FOR /d /r %%x IN (ipch) DO rmdir /s /q "%%x"
REM --------------------------------------------
REM E project specific directories
REM --------------------------------------------
rmdir .hg /s /q
rmdir Lib /s /q
rmdir Obj /s /q
rmdir Bin /s /q
echo Done.
