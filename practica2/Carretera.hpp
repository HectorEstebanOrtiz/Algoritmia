#pragma once

//Estructura para representar el grafo normal
struct Destinos {
    int destino;
    double tiempo;
};

//Estructura para representar una carretera entre dos nodos
struct Carretera {
    int origen;
    Destinos destino;
};