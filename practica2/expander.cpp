#include <vector>
#include "Carretera.hpp"
#include <cmath>

using namespace std;

struct DestinosExpandidos {
    int destino;
    int sub_nodo;
    double tiempo;
};
using grafoNormal = vector<vector<Destinos>>;
using grafoExpandido = vector<vector<vector<DestinosExpandidos>>>;

grafoExpandido Expandir(const grafoNormal& original){
    grafoExpandido nuevoMapa;
    nuevoMapa.resize(original.size());
    for(int i = 0; i < original.size(); i++){
        int t = original[i].size();
        if(t == 0) continue;
        int dimensiones = ceil(log2(t));
        int tamagno = 1 << dimensiones;
        nuevoMapa[i].resize(tamagno);
        for(int j = 0; j < tamagno; j++){
            for (int bit = 0; bit < dimensiones; bit++) {
                int subnodo_vecino = j ^ (1 << bit); // Cambiar 1 bit (distancia Hamming = 1)
                
                DestinosExpandidos arista_interna{i,subnodo_vecino, 0.0}; // Tiempo/peso interno = 0
                nuevoMapa[i][j].push_back(arista_interna);
            }
        }
    }
    //AHORA TOCARIA CONECTAR LOS HIPERCUBOS
    //RAZON DE BUCLE DIFERENTE (CREEMOS): SI LO HACEMOS EN EL ANTERIOR, SOLAMENTE CONECTAMOS DE MENORES ID A MAYORES, LO QUE IMPIDIRIAMOS VOLVER HACIA ATRAS, ES DECIR, 1 -> 4 SI, PERO 4 -> 2 NO
    for(int i = 0; i < original.size(); i++){
        
    }
    return nuevoMapa;
}


