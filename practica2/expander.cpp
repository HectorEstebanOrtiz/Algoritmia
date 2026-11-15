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
        for(int j = 0; j < original[i].size(); j++){
            int vecino = original[i][j].destino;
            double tiempo = original[i][j].tiempo;
            int subnodo_destino = 0;
            for(int k = 0; k < original[vecino].size(); k++){
                //comprueba si el destino del vecino es el nodo i, si es asi, guarda el subnodo destino
                if(original[vecino][k].destino == i){
                    subnodo_destino = k;
                    break;
                }
            }
            DestinosExpandidos arista_externa{vecino,subnodo_destino,tiempo};
            nuevoMapa[i][j].push_back(arista_externa);
        }
    }
    return nuevoMapa;
}


//devuelve el primer camino que salga del hipercubo s partiendo del subnodo subnodo_s que no haya sido visitado
DestinosExpandidos buscarSubNodo(int s, int subnodo_s, grafoExpandido& gE) {
    int subnodo_actual = subnodo_s;
    while (true) {
        int primera_interna_libre = -1;

        // 1. Inspeccionar todas las aristas del subnodo_actual
        for (size_t i = 0; i < gE[s][subnodo_actual].size(); ++i) {
            auto& arista = gE[s][subnodo_actual][i];

            if (!arista.visitado) {
                // PRIORIDAD: Si encontramos carretera EXTERNA (destino != s)
                if (arista.destino != s) {
                    arista.visitado = true; // Marcamos como visitada
                    return arista;          // ¡Éxito! Retornamos la salida
                } 
                // PLAN B: Guardamos la primera arista INTERNA libre para movernos
                else if (primera_interna_libre == -1) {
                    primera_interna_libre = static_cast<int>(i);
                }
            }
        }

        // 2. Si no había salida externa, avanzamos internamente a otro subnodo
        if (primera_interna_libre != -1) {
            auto& arista_interna = gE[s][subnodo_actual][primera_interna_libre];
            arista_interna.visitado = true;           // Consumimos el paso interno
            subnodo_actual = arista_interna.sub_nodo; // Nos desplazamos al nuevo subnodo
        } 
        // 3. Si no hay aristas internas ni externas libres, no hay salida accesible
        else {
            break;
        }
    }

    return DestinosExpandidos{-1, -1, 0.0}; // No se encontró camino válido
}

bool buscarCamino(grafoExpandido& gE, int s, int t, list<Carretera>& camino, int subnodo_s = 0){
    if(s == t){
        return true;
    }else{
        DestinosExpandidos actual = buscarSubNodo(s, subnodo_s, gE);
        if(actual.sub_nodo == -1){
            return false;
        }
        while(!buscarCamino(gE, actual.destino, t, camino, actual.sub_nodo)){
            actual = buscarSubNodo(s, subnodo_s, gE);
            if(actual.sub_nodo == -1){
                return false;
            }
        }
        camino.push_front({s,{actual.destino, actual.tiempo}});
        return true;
    }
}

int main() {
    vector<vector<Destinos>> destinos = {
        // Aquí pones los destinos del nodo 0 (por ejemplo, si va al 1 o al 2)
        {{1, 2.0}},                   // Índice 0: Nodo 0 
        {{2, 5.0}, {0, 10.0}},        // Índice 1: Nodo 1
        {{4, 3.0}, {1, 7.0}},         // Índice 2: Nodo 2
        {{6, 2.0}},                   // Índice 3: Nodo 3
        {{1, 1.0}},                   // Índice 4: Nodo 4
        {{6, 4.0}},                   // Índice 5: Nodo 5
        {}                            // Índice 6: Nodo 6 (final)
    };

    int origen = 1;
    int destino = 6;
    double tiempoTotal = 0.0;
    grafoExpandido exp = Expandir(destinos);
    list<Carretera> camino = {};
    if(buscarCamino(exp, origen, destino, camino)){
        cout << "Camino desde " << origen << " hasta " << destino << ":" << endl;
        for (const auto& carretera : camino) {
            cout << "Origen: " << carretera.origen << ", Destino: " << carretera.destino.destino 
                << ", Tiempo: " << carretera.destino.tiempo << endl;
            tiempoTotal += carretera.destino.tiempo;
        }
        cout << "Tiempo total: " << tiempoTotal << endl;
    }else{
        cout << "No hay camino" << endl;
    }
    return 0;
}