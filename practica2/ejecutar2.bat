@echo off
setlocal enabledelayedexpansion

:: Autores: Ciro Fustero Zumeta (900506), Hector Esteban Ortiz (898030)

echo Compilando el proyecto...

:: Limpiamos el build antiguo si existe
if exist build (
    rmdir /s /q build
)

:: Compilacion del codigo con g++
g++ expander.cpp probabilista.cpp main.cpp -o p2.exe
if %errorlevel% neq 0 (
    echo Error al compilar p2
    exit /b 1
)

g++ analizador_resultados.cpp -o analizador.exe
if %errorlevel% neq 0 (
    echo Error al compilar analizador
    exit /b 1
)

:: Creamos el directorio de las pruebas automaticas
if not exist resultadoPruebasAuto (
    mkdir resultadoPruebasAuto
)

:: Preparamos el fichero de resultados
if exist tablaResultados.txt del /q /f tablaResultados.txt
type nul > tablaResultados.txt

set cabecera=1

:: Aplicamos las pruebas a todos los txt que se encuentren en el directorio de test
for %%F in (tests\*.txt) do (
    :: Extraemos el nombre del archivo sin extension
    set "nombre_sin_ext=%%~nF"
    
    set "nombre_resultado=resultadoPruebasAuto\!nombre_sin_ext!_resultado.txt"
    
    if exist "!nombre_resultado!" del /q /f "!nombre_resultado!"
    type nul > "!nombre_resultado!"
    
    echo Procesando prueba con el fichero !nombre_sin_ext!
    
    p2.exe "%%F" > "!nombre_resultado!"
    
    if "!cabecera!"=="1" (
        analizador.exe "!nombre_resultado!" >> tablaResultados.txt
        set cabecera=0
    ) else (
        analizador.exe "!nombre_resultado!" 0 >> tablaResultados.txt
    )
    
    echo Prueba con el fichero !nombre_sin_ext! TERMINADA
)

echo --- TERMINADO ---