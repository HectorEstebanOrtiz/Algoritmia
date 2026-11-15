#pragma once
#include <vector>
#include "Carretera.hpp"
#include <cmath>
#include <list>
#include <iostream>

using namespace std;

struct DestinosExpandidos {
    int destino;
    int sub_nodo;
    double tiempo;
    bool visitado = false;
};
struct nodo {
    int nodo;
    int sub_nodo;
};
using grafoNormal = vector<vector<Destinos>>;
using grafoExpandido = vector<vector<vector<DestinosExpandidos>>>;

grafoExpandido Expandir(const grafoNormal& original);
bool buscarCamino(grafoExpandido& gE, int s, int t, list<Carretera>& camino, int subnodo_s = 0);