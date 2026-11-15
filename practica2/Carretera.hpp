#pragma once
#include <list>

struct Destinos {
    int destino;
    double tiempo;
};

struct Carretera {
    int origen;
    Destinos destino;
};