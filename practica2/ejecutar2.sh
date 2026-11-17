#!/bin/bash

#Autores: Ciro Fustero Zumeta (900506), Hector Esteban Ortiz (898030)

echo "Compilando el proyecto..."

#Limpiamos el build antiguo
rm -r -f build

#Compilacion del codigo con cmake
g++ expander.cpp probabilista.cpp main.cpp -o p2
g++ analizador_resultados.cpp -o analizador

if [ $? -ne 0 ]; then
    echo "Error al compilar"
    exit 1
fi

#Creamos el direcctorio de las pruebas automaticas
mkdir resultadoPruebasAuto > /dev/null 2>&1

rm tablaResultados.txt > /dev/null 2>&1
touch tablaResultados.txt

cabecera=1

#Aplicamos las pruebas a todos los txt que se encuentren en el directorio de test
for dict in tests/*.txt; do
    nombre_archivo=$(basename "$dict")
    
    nombre_sin_ext="${nombre_archivo%.*}"
    
    nombre_resultado="resultadoPruebasAuto/${nombre_sin_ext}_resultado.txt"
    rm "${nombre_resultado}" > /dev/null 2>&1
    touch ${nombre_resultado}
    
    echo "Procesando prueba con el fichero ${nombre_sin_ext}"
    
    ./p2 "$dict" > "${nombre_resultado}"
    
    if [ "$cabecera" -eq 1 ]; then
        ./analizador "${nombre_resultado}" >> tablaResultados.txt
        cabecera=0
    else
        ./analizador "${nombre_resultado}" 0 >> tablaResultados.txt
    fi
    
    echo "Prueba con el fichero ${nombre_sin_ext} TERMINADA"
    
done

echo "--- TERMINADO ---"