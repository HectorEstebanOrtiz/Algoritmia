#include "probabilista.hpp"

static random_device rd;
static mt19937 gen(rd());

//Funcion que devuelve un camino aleatorio desde el nodo origen hasta el nodo destino en un grafo representado por una lista de Carreteras.
list<Carretera> sacarCaminoRandom(int origen, int destino, const vector<vector<Destinos>>& destinos, double& tiempoTotal, int k, bool& encontrado) {
    list<Carretera> camino = {};
    
    //Usamos una referencia constante para evitar copiar todo el vector en cada llamada recursiva
    const vector<Destinos>& destinosPosibles = destinos[origen];
    
    //Si llegamos al destino o no hay salidas desde el nodo actual o nos quedamos sin intentos, terminamos
    if (origen == destino || destinosPosibles.empty() || k <= 0) {
        encontrado = origen == destino;
        return camino;
    }

    //Cogemos un destino aleatorio de los posibles destinos desde el nodo actual
    uniform_int_distribution<int> c(0, destinosPosibles.size() - 1);
    int idx = c(gen);

    //Acumulamos el tiempo del camino y añadimos la carretera al camino
    tiempoTotal += destinosPosibles[idx].tiempo;
    camino.push_back({origen, destinosPosibles[idx]});
    
    //Llamada recursiva para continuar el camino desde el destino elegido
    camino.splice(camino.end(), sacarCaminoRandom(destinosPosibles[idx].destino, destino, destinos, tiempoTotal, k-1, encontrado));
    return camino;
}