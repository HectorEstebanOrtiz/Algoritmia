#pragma once
#include <vector>
#include "Carretera.hpp"
#include <cmath>
#include <list>
#include <iostream>

using namespace std;

//Estructuras para el grafo expandido
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

//Definiciones de tipos para los grafos
using grafoNormal = vector<vector<Destinos>>;
using grafoExpandido = vector<vector<vector<DestinosExpandidos>>>;

//Funcion que expande un grafo normal a un grafo expandido
grafoExpandido Expandir(const grafoNormal& original);

//Funcion que busca un camino en un grafo expandido desde el nodo s hasta el nodo t
bool buscarCamino(grafoExpandido& gE, int s, int t, list<Carretera>& camino, int subnodo_s = 0);