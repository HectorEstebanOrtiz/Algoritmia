#include <iostream>
#include <random>
#include <list>
#include <vector>
#include "Carretera.hpp"

using namespace std;
static random_device rd;
static mt19937 gen(rd());

list<Carretera> sacarCaminoRandom(int origen, int destino, vector<list<Destinos>> destinos, double& tiempoTotal) {
    list<Carretera> camino = {};
    list<Destinos> destinosPosibles = destinos[origen];
    if (destinosPosibles.empty()) {
        return camino; 
    }
    uniform_int_distribution<int> c(0, destinosPosibles.size() - 1);
    int idx = c(gen);
    tiempoTotal += next(destinosPosibles.begin(), idx)->tiempo;
    camino.push_back({origen, *next(destinosPosibles.begin(), idx)});
    camino.splice(camino.end(), sacarCaminoRandom(camino.back().destino.destino, destino, destinos, tiempoTotal));
    return camino;
}

int main() {
    vector<list<Destinos>> destinos = {
        // Aquí pones los destinos del nodo 0 (por ejemplo, si va al 1 o al 2)
        {{1, 2.0}},                   // Índice 0: Nodo 0 
        {{2, 5.0}, {3, 10.0}},        // Índice 1: Nodo 1
        {{4, 3.0}, {5, 7.0}},         // Índice 2: Nodo 2
        {{5, 2.0}},                   // Índice 3: Nodo 3
        {{6, 1.0}},                   // Índice 4: Nodo 4
        {{6, 4.0}},                   // Índice 5: Nodo 5
        {}                            // Índice 6: Nodo 6 (final)
    };

    int origen = 1;
    int destino = 6;
    double tiempoTotal = 0.0;
    list<Carretera> camino = sacarCaminoRandom(origen, destino, destinos, tiempoTotal);
    cout << "Camino desde " << origen << " hasta " << destino << ":" << endl;
    for (const auto& carretera : camino) {
        cout << "Origen: " << carretera.origen << ", Destino: " << carretera.destino.destino 
             << ", Tiempo: " << carretera.destino.tiempo << endl;
    }
    cout << "Tiempo total: " << tiempoTotal << endl;
    return 0;
}