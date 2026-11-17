#pragma once
#include <iostream>
#include <random>
#include <list>
#include <vector>
#include "Carretera.hpp"

using namespace std;

//Funcion que devuelve un camino aleatorio desde el nodo origen hasta el nodo destino en un grafo representado por una lista de Carreteras.
list<Carretera> sacarCaminoRandom(int origen, int destino, const vector<vector<Destinos>>& destinos, double& tiempoTotal, int k, bool& encontrado);
