#include "probabilista.hpp"
#include "expander.hpp"
#include <vector>
#include "Carretera.hpp"
#include <cmath>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Funcion para leer el archivo y generar el grafo
vector<vector<Destinos>> cargarGrafo(const string& nombreArchivo, int& s, int& t, int& M, bool bidireccional) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << "\n";
        return {};
    }

    int N;
    // Leer N (ciudades), M (conexiones), s (origen) y t (destino)
    if (!(archivo >> N >> M >> s >> t)) {
        cerr << "Error leyendo la primera línea del archivo.\n";
        return {};
    }

    // Inicializamos el grafo con N + 1 para poder indexar directamente desde 1 hasta N
    vector<vector<Destinos>> grafo(N + 1);

    // Leer las M conexiones
    for (int i = 0; i < M; ++i) {
        int u, v;
        double l_uv;
        archivo >> u >> v >> l_uv;

        // Añadir la conexion u -> v
        grafo[u].push_back({v, l_uv});
        
        // Añadir la conexion v -> u (asumiendo carreteras bidireccionales)
        if (bidireccional) {
            grafo[v].push_back({u, l_uv});
        }
    }

    archivo.close();
    return grafo;
}

int main(int argc, char *argv[]){
    bool bidireccional = false; // Variable para determinar si el grafo es bidireccional
    int numeroIntentos = 1000;
    int M = 0;
    if (argc < 2) {
        printf("Error. Uso correcto: %s <nombre_del_fichero>\n", argv[0]);
        return 1; // Salir con codigo de error
    } else if (argc > 2) {
        string modo = argv[2];
        if(modo == "bi"){
            bidireccional = true;
        } else if(modo == "uni"){
            bidireccional = false;
        } else {
            printf("Error. Segundo argumento debe ser 'bi' o 'uni'.\n");
            return 1; // Salir con codigo de error
        }
        if(argc > 3){
            numeroIntentos = stoi(argv[3]);
        }
    }
    // Variables para guardar el nodo origen y final del camino que vienen en el archivo
    int origen_s, destino_t;
    
    // Cambia "entrada.txt" por el nombre real de tu fichero
    vector<vector<Destinos>> grafo = cargarGrafo(argv[1], origen_s, destino_t, M, bidireccional);

    if (grafo.empty()) {
        return 1; // Terminar si hubo un error de lectura
    }

    // Comprobacion rápida para ver si se ha guardado correctamente
    cout << "Se ha cargado un grafo con " << grafo.size() - 1 << " nodos.\n";
    cout << "Buscando camino desde " << origen_s << " hasta " << destino_t << ".\n\n";

    // Probabilista
    cout << "Ejecutando algoritmo probabilista...\n";
    double tiempoTotalProbabilista = 0.0;
    int vecesCaminoCorto = 0;
    bool encontradoProb = false;
    list<Carretera> caminoProbabilista = {};
    for(int i = 0; i<numeroIntentos; i++){
        double tiempoPrueba = 0.0;
        bool encontradoP = false;
        list<Carretera> prueba = sacarCaminoRandom(origen_s, destino_t, grafo, tiempoPrueba, M, encontradoP);
        if (caminoProbabilista.empty() && encontradoP) {
            caminoProbabilista = prueba;
            tiempoTotalProbabilista = tiempoPrueba;
            encontradoProb = true;
        } else if(tiempoPrueba < tiempoTotalProbabilista && encontradoP){
            caminoProbabilista = prueba;
            tiempoTotalProbabilista = tiempoPrueba;
        } else if(tiempoPrueba == tiempoTotalProbabilista && encontradoP){
            vecesCaminoCorto++;
        }
    }
    for (const auto& carretera : caminoProbabilista) {
        cout << "De " << carretera.origen << " a " << carretera.destino.destino 
             << " con tiempo " << carretera.destino.tiempo << "\n";
    }

    // Expander
    cout << "Ejecutando algoritmo expander...\n";
    grafoExpandido gE = Expandir(grafo);
    list<Carretera> caminoExpander;
    double tiempoTotalExpander = 0.0; // Variable para acumular el tiempo total del camino encontrado
    if(buscarCamino(gE, origen_s, destino_t, caminoExpander)){
        for (const auto& carretera : caminoExpander) {
            cout << "De " << carretera.origen << " a " << carretera.destino.destino 
                 << " con tiempo " << carretera.destino.tiempo << "\n";
            tiempoTotalExpander += carretera.destino.tiempo; // Acumular el tiempo
        }
        cout << "RESULTADOS TIEMPOS:" << endl;
        cout << "Camino expander encontrado con una longitud de " << tiempoTotalExpander << ".\n";
    } else {
        cout << "RESULTADOS TIEMPOS:" << endl;
        cout << "No se encontro camino con el algoritmo expander.\n";
    }
    
    if(encontradoProb){
        cout << "Camino probabilista encontrado con una longitud de " << tiempoTotalProbabilista << ".\n";
        double probabilidad = (double)vecesCaminoCorto / 1000.0;
        cout << "Probabilidad de encontrar el camino mas corto > " << probabilidad * 100 << "%\n\n";
    } else {
        cout << "No se encontro camino con el algoritmo probabilista.\n";
    }
    return 0;
}