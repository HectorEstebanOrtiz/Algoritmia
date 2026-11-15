#include "probabilista.hpp"

static random_device rd;
static mt19937 gen(rd());

list<Carretera> sacarCaminoRandom(int origen, int destino, const vector<vector<Destinos>>& destinos, double& tiempoTotal, int k, bool& encontrado) {
    list<Carretera> camino = {};
    
    // Usamos una referencia constante para evitar copiar todo el vector en cada llamada recursiva
    const vector<Destinos>& destinosPosibles = destinos[origen];
    
    // Si llegamos al destino o no hay salidas desde el nodo actual o nos quedamos sin intentos, terminamos
    if (origen == destino || destinosPosibles.empty() || k <= 0) {
        encontrado = origen == destino;
        return camino;
    }

    uniform_int_distribution<int> c(0, destinosPosibles.size() - 1);
    int idx = c(gen);

    // Con vector podemos acceder directamente mediante [idx] en tiempo constante O(1)
    tiempoTotal += destinosPosibles[idx].tiempo;
    camino.push_back({origen, destinosPosibles[idx]});
    
    camino.splice(camino.end(), sacarCaminoRandom(destinosPosibles[idx].destino, destino, destinos, tiempoTotal, k-1, encontrado));
    
    return camino;
}