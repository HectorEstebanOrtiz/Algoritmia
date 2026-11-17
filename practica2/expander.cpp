#include "expander.hpp"

//Funcion que expande un grafo normal a un grafo expandido
grafoExpandido Expandir(const grafoNormal& original){
    grafoExpandido nuevoMapa;
    nuevoMapa.resize(original.size());

    //Primero creamos los subgrafos internos (hipercubos) para cada nodo
    for(int i = 0; i < original.size(); i++){
        int t = original[i].size();
        
        //Si no hay destinos, simplemente dejamos el subgrafo vacío
        if(t == 0) {
            nuevoMapa[i].resize(1);
            continue;
        }
        
        //Calculamos el tamaño del hipercubo necesario para conectar todos los destinos del nodo i
        int dimensiones = ceil(log2(t));
        int tamagno = 1 << dimensiones;
        nuevoMapa[i].resize(tamagno);

        //Creamos las aristas internas del hipercubo (conexiones entre subnodos)
        for(int j = 0; j < tamagno; j++){
            for (int bit = 0; bit < dimensiones; bit++) {
                int subnodo_vecino = j ^ (1 << bit);
                
                DestinosExpandidos arista_interna{i,subnodo_vecino, 0.0};
                nuevoMapa[i][j].push_back(arista_interna);
            }
        }
    }
    //Luego añadimos las aristas externas (conexiones entre nodos diferentes)
    for(int i = 0; i < original.size(); i++){
        for(int j = 0; j < original[i].size(); j++){
            int vecino = original[i][j].destino;
            double tiempo = original[i][j].tiempo;
            int subnodo_destino = 0;
            for(int k = 0; k < original[vecino].size(); k++){
                //Comprobamos si el destino del vecino coincide con el nodo actual para determinar el subnodo de destino
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


//Funcion auxiliar que devuelve el primer camino que salga del hipercubo s partiendo del subnodo subnodo_s que no haya sido visitado
DestinosExpandidos buscarSubNodo(int s, int subnodo_s, grafoExpandido& gE) {
    int subnodo_actual = subnodo_s;
    while (true) {
        int primera_interna_libre = -1;

        //Inspecionamos todas las aristas del subnodo_actual
        for (size_t i = 0; i < gE[s][subnodo_actual].size(); ++i) {
            auto& arista = gE[s][subnodo_actual][i];

            //Si la arista no ha sido visitada, verificamos si es una salida externa o interna
            if (!arista.visitado) {
                //Si es una salida externa (hacia otro nodo), la retornamos inmediatamente
                if (arista.destino != s) {
                    arista.visitado = true; // Marcamos como visitada
                    return arista;          
                } 
                //Si es una salida interna (dentro del mismo nodo), la guardamos como posible camino a seguir si no hay salidas externas
                else if (primera_interna_libre == -1) {
                    primera_interna_libre = static_cast<int>(i);
                }
            }
        }

        //Si no había salida externa, avanzamos internamente a otro subnodo
        if (primera_interna_libre != -1) {
            auto& arista_interna = gE[s][subnodo_actual][primera_interna_libre];
            arista_interna.visitado = true;           // Consumimos el paso interno
            subnodo_actual = arista_interna.sub_nodo; // Nos desplazamos al nuevo subnodo
        } 
        //Si no hay aristas internas ni externas libres, no hay salida accesible
        else {
            break;
        }
    }

    return DestinosExpandidos{-1, -1, 0.0}; // No se encontró camino válido
}

//Funcion que busca un camino en un grafo expandido desde el nodo s hasta el nodo t
bool buscarCamino(grafoExpandido& gE, int s, int t, list<Carretera>& camino, int subnodo_s){
    //Caso base: si hemos llegado al nodo destino, retornamos verdadero (hay camino)
    if(s == t){
        return true;
    }else{ //Caso recursivo: buscamos un camino desde el nodo s hasta el nodo t
        //Buscamos un subnodo accesible desde el nodo s que no haya sido visitado
        DestinosExpandidos actual = buscarSubNodo(s, subnodo_s, gE);
        if(actual.sub_nodo == -1){
            return false;
        }
        actual.visitado = true; // Marcamos la arista como visitada
        //Mientras no encontremos un camino desde el subnodo actual hasta el nodo t, seguimos buscando subnodos accesibles
        while(!buscarCamino(gE, actual.destino, t, camino, actual.sub_nodo)){
            actual = buscarSubNodo(s, subnodo_s, gE);
            //Si no hay más subnodos accesibles, retornamos falso (no hay camino)
            if(actual.sub_nodo == -1){
                return false;
            }
            actual.visitado = true; // Marcamos la arista como visitada
        }
        //Si encontramos un camino, añadimos la carretera correspondiente al principio del camino y retornamos verdadero
        camino.push_front({s,{actual.destino, actual.tiempo}});
        return true;
    }
}
