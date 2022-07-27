@echo off
rem Makefile

if "%1"=="" goto all
if "%1"=="clean" goto clean
goto usage

:all
echo Building the obv/point example ...
call idl2java map.idl
vbjc Client.java
vbjc Server.java
vbjc Map\\*.java
goto end

:clean
del /q Map\*.class
del Map\ConnectedPoint.java
del Map\ConnectedPointH*.java
del Map\ConnectedPointSeq*.java
del Map\Duplicate*.java
del Map\Invalid*.java
del Map\Label*.java
del Map\PointManager*.java
del Map\PointSeq*.java
del Map\Unknown*.java
del Map\_PointManagerStub.java
del Map\Point.java
del Map\PointH*.java
del /q *.class
del /q *.module
goto end

:usage
echo Usage: vbmake [clean]

:end
