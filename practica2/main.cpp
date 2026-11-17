#include "probabilista.hpp"
#include "expander.hpp"
#include <vector>
#include "Carretera.hpp"
#include <cmath>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

//Funcion para leer el archivo y generar el grafo
vector<vector<Destinos>> cargarGrafo(const string& nombreArchivo, int& s, int& t, int& M) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << "\n";
        return {};
    }

    int N;

    //Leemos N (ciudades), M (conexiones), s (origen) y t (destino)
    if (!(archivo >> N >> M >> s >> t)) {
        cerr << "Error leyendo la primera línea del archivo.\n";
        return {};
    }

    //Inicializamos el grafo con N + 1 para poder indexar directamente desde 1 hasta N
    vector<vector<Destinos>> grafo(N + 1);

    //Leemos las M conexiones
    for (int i = 0; i < M; ++i) {
        int u, v;
        double l_uv;
        archivo >> u >> v >> l_uv;

        //Añadimos la conexion u -> v
        grafo[u].push_back({v, l_uv});
        
        //Añadimos la conexion v -> u para hacer las carreteras bidireccionales
        grafo[v].push_back({u, l_uv});
    }

    archivo.close();
    return grafo;
}

int main(int argc, char *argv[]){
    clock_t inicio, fin, ini, f;
    int numeroIntentos = 1000;
    int M = 0;
    if (argc < 2) {
        printf("Error. Uso correcto: %s <nombre_del_fichero>\n", argv[0]);
        return 1; // Salir con codigo de error
    } else if (argc > 2) {
        numeroIntentos = stoi(argv[2]); //Modificar el número de intentos si se pasa como argumento
    }
    
    int origen_s, destino_t;
    
    //Extraemos el grafo del archivo y los nodos origen y destino
    vector<vector<Destinos>> grafo = cargarGrafo(argv[1], origen_s, destino_t, M);

    if (grafo.empty()) {
        return 1; // Terminar si hubo un error de lectura
    }

    cout << "Buscando camino desde " << origen_s << " hasta " << destino_t << ".\n\n";

    //Ejecutamos el algoritmo probabilista
    cout << "Ejecutando algoritmo probabilista...\n";
    double tiempoTotalProbabilista = 0.0;
    int vecesCaminoCorto = 0;
    bool encontradoProb = false;
    list<Carretera> caminoProbabilista = {};
    inicio = clock();
    double tiempoMedio=0.0;

    // Ejecutamos el algoritmo probabilista un número determinado de veces para calcular el tiempo medio y la probabilidad de encontrar el camino más corto (asumiendo que el camino mas corto encontrado es el más corto)
    for(int i = 0; i<numeroIntentos; i++){
        double tiempoPrueba = 0.0;
        bool encontradoP = false;
        ini = clock();
        list<Carretera> prueba = sacarCaminoRandom(origen_s, destino_t, grafo, tiempoPrueba, M, encontradoP);
        f = clock();
        tiempoMedio += (double)(f - ini) / CLOCKS_PER_SEC;

        //Si encontramos un camino y es más corto que el mejor encontrado hasta ahora, lo guardamos
        if (caminoProbabilista.empty() && encontradoP) {
            caminoProbabilista = prueba;
            tiempoTotalProbabilista = tiempoPrueba;
            encontradoProb = true;
        } else if(tiempoPrueba < tiempoTotalProbabilista && encontradoP){
            caminoProbabilista = prueba; //Actualizamos el camino más corto encontrado
            tiempoTotalProbabilista = tiempoPrueba; //Actualizamos el tiempo total del camino más corto encontrado
        } else if(tiempoPrueba == tiempoTotalProbabilista && encontradoP){
            vecesCaminoCorto++; //Incrementamos el contador
        }
    }
    tiempoMedio /= numeroIntentos;
    fin = clock();
    double tiempo_gastadoProb = (double)(fin - inicio) / CLOCKS_PER_SEC;

    //Mostramos el camino encontrado por el algoritmo probabilista
    for (const auto& carretera : caminoProbabilista) {
        cout << "De " << carretera.origen << " a " << carretera.destino.destino 
             << " con tiempo " << carretera.destino.tiempo << "\n";
    }

    //Ejecutamos el algoritmo expander
    cout << "Ejecutando algoritmo expander...\n";
    double tiempo_gastadoEx;
    inicio = clock();

    //Expandimos el grafo
    grafoExpandido gE = Expandir(grafo);
    list<Carretera> caminoExpander;
    double tiempoTotalExpander = 0.0; 
    double tiempoSinExpandir = 0.0;
    ini = clock();

    //Buscamos un camino desde el nodo origen hasta el nodo destino en el grafo expandido
    if(buscarCamino(gE, origen_s, destino_t, caminoExpander)){
        f = clock();
        fin = clock();
        tiempo_gastadoEx = (double)(fin - inicio) / CLOCKS_PER_SEC;
        tiempoSinExpandir = (double)(f - ini) / CLOCKS_PER_SEC;

        //Mostramos el camino encontrado por el algoritmo expander y calculamos el tiempo total del camino
        for (const auto& carretera : caminoExpander) {
            cout << "De " << carretera.origen << " a " << carretera.destino.destino 
                 << " con tiempo " << carretera.destino.tiempo << "\n";
            tiempoTotalExpander += carretera.destino.tiempo; // Acumular el tiempo
        }

        //Mostramos los resultados de tiempos
        cout << "RESULTADOS TIEMPOS:" << endl;
        cout << "Tiempo de ejecucion del algoritmo expander: " << tiempo_gastadoEx << " segundos\n";
        cout << "Tiempo de ejecucion del algoritmo expander (Sin expandir): " << tiempoSinExpandir << " segundos\n";
        cout << "Camino expander encontrado con una longitud de " << tiempoTotalExpander << ".\n";
    } else {
        f = clock();
        fin = clock();
        tiempo_gastadoEx = (double)(fin - inicio) / CLOCKS_PER_SEC;
        tiempoSinExpandir = (double)(f - ini) / CLOCKS_PER_SEC;

        //Mostramos los resultados de tiempos cuando no se encuentra un camino
        cout << "RESULTADOS TIEMPOS:" << endl;
        cout << "Tiempo de ejecucion del algoritmo expander: " << tiempo_gastadoEx << " segundos\n";  
        cout << "Tiempo de ejecucion del algoritmo expander (Sin expandir): " << tiempoSinExpandir << " segundos\n";      
        cout << "No se encontro camino con el algoritmo expander.\n";
    }
    
    cout << "Tiempo medio de ejecucion del algoritmo probabilista: " << tiempoMedio << " segundos\n";
    cout << "Tiempo total de ejecucion del algoritmo probabilista: " << tiempo_gastadoProb << " segundos\n";
    if(encontradoProb){
        cout << "Camino probabilista encontrado con una longitud de " << tiempoTotalProbabilista << ".\n";
        double probabilidad = (double)vecesCaminoCorto / 1000.0; //Calculamos la probabilidad de encontrar el camino más corto
        cout << "Probabilidad de encontrar el camino mas corto > " << probabilidad * 100 << "%\n\n";
    } else {
        cout << "No se encontro camino con el algoritmo probabilista.\n";
    }
    return 0;
}