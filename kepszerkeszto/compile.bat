@echo off
set GCC_PATH=gcc

set OPTIMISE_FLAG=-O3

set RAM_handler_temp=temp/RAM_handler.o
set RAM_handler_compile_flags=-O0 -c src/RAM_handler.c -o %RAM_handler_temp%

set OUTPUT=-o kepszerkeszo

set SOURCE_FILES=main.c src/ERROR_handler.c src/generic_fuggvenyek.c src/input_handler.c ^
src/FILE_handler/FILE_handler.c src/FILE_handler/src/beolvas.c src/FILE_handler/src/parameter_beolvas.c src/FILE_handler/src/seged_fuggvenyek.c ^
src/GUI_handler/GUI_handler.c src/GUI_handler/src/window_handler.c src/GUI_handler/src/render.c src/GUI_handler/src/controll.c src/GUI_handler/src/input_window.c src/GUI_handler/src/viewport.c src/GUI_handler/src/seged_fuggvenyek.c ^
src/IMG_processing/img_processing.c src/IMG_processing/processing_units/blur.c src/IMG_processing/processing_units/brightness.c src/IMG_processing/processing_units/contrast_inc.c src/IMG_processing/processing_units/edge_detection.c src/IMG_processing/processing_units/generic.c src/IMG_processing/processing_units/sharpen.c

set INCLUDE_DIRS=-Iinc -ISDL2/include/SDL2

set LIBRARY_PATHS=-LSDL2/lib
set LIBRARIES=-lSDL2

set COMPILER_FLAGS=-Wall -Werror -mwindows

echo RAM_handler forditasa...

mkdir temp

%GCC_PATH% %RAM_handler_compile_flags%
if %errorlevel% neq 0 (
    echo Hiba  tortent
    exit /b 1
) else (
    echo RAM_handler forditasa sikeres
)

echo Teljes program forditasa...

%GCC_PATH% %COMPILER_FLAGS% %OPTIMISE_FLAG% %INCLUDE_DIRS% %LIBRARY_PATHS% %SOURCE_FILES%  %RAM_handler_temp% %OUTPUT% %LIBRARIES%
if %errorlevel% neq 0 (
    echo Hiba tortent
    exit /b 1
) else (
    echo Forditas sikeres
)

rmdir /s /q temp

pause