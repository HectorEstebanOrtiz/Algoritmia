#pragma once
#include <iostream>
#include <random>
#include <list>
#include <vector>
#include "Carretera.hpp"

using namespace std;

list<Carretera> sacarCaminoRandom(int origen, int destino, const vector<vector<Destinos>>& destinos, double& tiempoTotal, int k, bool& encontrado);
