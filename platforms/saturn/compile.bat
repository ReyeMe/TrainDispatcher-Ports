@ECHO Off
SET EMULATOR_DIR=Z:\Programming\SDK\Saturn\Emulators
SET COMPILER_DIR=Z:\Programming\SDK\Saturn\Compiler
SET JO_ENGINE_SRC_DIR=Z:\Programming\SDK\Saturn\jo_engine
SET PATH=%COMPILER_DIR%\SH_COFF\Other Utilities;%COMPILER_DIR%\SH_COFF\sh-coff\bin;%COMPILER_DIR%\TOOLS;%PATH%

IF "%1" == "run" GOTO run

:clean
del /S *.o
rm -f ./cd/0.bin
rm -f %JO_ENGINE_SRC_DIR%/*.o
rm -f ./BuildDrop/Saturn/sl_coff.bin
rm -f ./BuildDrop/Saturn/sl_coff.coff
rm -f ./BuildDrop/Saturn/sl_coff.map
rm -f ./BuildDrop/Saturn/sl_coff.iso
rm -f ./BuildDrop/Saturn/sl_coff.cue
IF "%1" == "cleanOnly" GOTO end
IF NOT EXIST == "./BuildDrop/sl_coff.iso" GOTO compile

:compile
IF "%1" == "cleanOnly" GOTO end
copy ./platforms/saturn/makefile "%cd%"
make
JoEngineCueMaker
rm makefile
del /S *.o
rm -f ./cd/0.bin
rm -f %JO_ENGINE_SRC_DIR%/*.o
GOTO moveBuild
:compileEnd
IF NOT "%1" == "alsoRun" GOTO end

:run
SET EMULATOR_DIR=Z:\Programming\SDK\Saturn\Emulators
IF EXIST ".\BuildDrop\sl_coff.iso" "%EMULATOR_DIR%\yabause\yabause.exe" -a -i .\BuildDrop\sl_coff.cue
IF NOT EXIST ".\BuildDrop\sl_coff.iso" GOTO closeError
GOTO end

:closeError
ECHO "Task Fail"
GOTO end

:moveBuild
mkdir -p BuildDrop\Saturn
move .\sl_coff.bin .\BuildDrop\Saturn\
move .\sl_coff.coff .\BuildDrop\Saturn\
move .\sl_coff.map .\BuildDrop\Saturn\
move .\sl_coff.iso .\BuildDrop\Saturn\
move .\sl_coff.cue .\BuildDrop\Saturn\
GOTO compileEnd

:close
ECHO "Task done"

:end