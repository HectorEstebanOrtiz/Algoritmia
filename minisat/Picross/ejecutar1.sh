#!/bin/bash

#Autores: Ciro Fustero Zumeta (900506), Hector Esteban Ortiz (898030)

echo "Compilando el proyecto..."

#Limpiamos el build antiguo
rm -r -f build

#Compilacion del codigo con cmake
cmake -B build -G "Unix Makefiles"
cmake --build build

if [ $? -ne 0 ]; then
    echo "Error al compilar"
    exit 1
fi

#Creamos el direcctorio de las pruebas automaticas
mkdir resultadoPruebasAuto > /dev/null 2>&1

#Aplicamos las pruebas a todos los txt que se encuentren en el directorio de test
for dict in testPicross/*.txt; do
    nombre_archivo=$(basename "$dict")
    
    nombre_sin_ext="${nombre_archivo%.*}"
    
    nombre_resultado="resultadoPruebasAuto/${nombre_sin_ext}_resultado.txt"
    rm "${nombre_resultado}" > /dev/null 2>&1
    touch ${nombre_resultado}
    
    echo "Procesando prueba con el fichero ${nombre_sin_ext}"
    
    ./build/picross_solver "$dict" > "${nombre_resultado}"
    
    echo "Prueba con el fichero ${nombre_sin_ext} TERMINADA"
    
done

echo "--- TERMINADO ---"