#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../minisat/core/Solver.h"
#include "../minisat/core/SolverTypes.h"
#include <stdexcept>
#include <chrono>
#include "competencia/src/picross/include/picross/picross.h"

using namespace std;
using namespace std::chrono;
using namespace Minisat;

namespace Minisat {
    void throw_OutOfMemoryException() {
        throw std::runtime_error("MiniSAT se ha quedado sin memoria.");
    }
    void throw_OutOfMemoryException(const char* msg) {
        throw std::runtime_error(msg);
    }
}

//Función principal que genera las clausulas SAT para pasarselas a MiniSAT y que este nos de la solución del picross
void generarCNFPicrossMiniSat(Solver& solver, const vector<int>& pistas, int longitud, vector<Var>& matrizVars) {
    int numBloques = (int)pistas.size();
    vector<bool> casillas(longitud);
    vector<vector<Var>> inicios(numBloques, vector<Var>(longitud));
    for (int i = 0; i < numBloques; i++) {
        for (int j = 0; j < longitud; j++) {
            inicios[i][j] = solver.newVar();
        }
    }

    vector<int> minPosicion(numBloques); //Posiciones minimas de los inicios de cada bloque (empezando de izquierda a derecha con una casilla de separación)
    vector<int> maxPosicion(numBloques); //Posiciones maximas de los inicios de cada bloque (empezando de derecha a izquierda con una casilla de separación)
    for (int i = 0; i < numBloques; i++) {
        minPosicion[i] = (i == 0) ? 0 : minPosicion[i - 1] + pistas[i - 1] + 1;
    }
    int longtotal = 0;
    for (int i = numBloques - 1; i >= 0; i--) {
        maxPosicion[i] = longitud - pistas[i] - longtotal;
        longtotal += pistas[i] + 1;
    }
    //Primera clausula: al menos un bloque debe empezar en alguna de las posiciones posibles y solo en una de ellas, 
    //es decir que si un bloque empieza en la posición p entonces no puede empezar en la posición q
    for (int i = 0; i < numBloques; i++) {
        vec<Lit> alMenosUno;
        for (int j = minPosicion[i]; j <= maxPosicion[i]; j++) {
            alMenosUno.push(mkLit(inicios[i][j], false));
        }
        solver.addClause(alMenosUno); //En esta clausula se meten todos los psoibles inicios de un bloque, de modo que siempre sea cierto al menos uno de ellos
        for (int p = minPosicion[i]; p <= maxPosicion[i]; p++) {
            for (int q = p + 1; q <= maxPosicion[i]; q++) {
                solver.addClause(mkLit(inicios[i][p], true), mkLit(inicios[i][q], true));
                // (~P_{i,p} v ~P_{i,q}) -> Esto de aquí equivale a decir que si un bloque empieza en la posición p entonces no puede empezar en la posición q
                //(es un recordatorio de la lógica que vimos en introduccion a los computadores, a lo largo de las reglas se usará mucho esta lógica)
            }
        }
    }

    /*Segunda Clausula: Propagacion entre bloques adyacentes, es decir que si un bloque empieza
     en una posicion p entonces el siguiente bloque no puede empezar antes de 
     p + tamaño del bloque + 1. O viceversa, si un bloque empieza en una posicion q entonces el bloque
      anterior no puede empezar despues de q - tamaño del bloque - 1.
    */
    for (int i = 0; i < numBloques - 1; i++) {
        //Propagamos los bloques de izquierda a derecha (Bloque i -> Bloque i+1), probando todos los lugares donde se pueda poner el primer bloque
        for (int p = minPosicion[i]; p <= maxPosicion[i]; p++) {
            vec<Lit> ordenIzDe;
            ordenIzDe.push(mkLit(inicios[i][p], true)); //~P_{i,p}, variable de la clausuala que dicta el primer bloque del conjunto

            int minQ = p + pistas[i] + 1; //Primera posición posible para i+1 (que es a un espacio del primero)
            //Probamos todas las posiciones iniciales del siguiente bloque (1 casilla, 2 casillas, 3, ... despues del primero).
            for (int q = minQ; q <= maxPosicion[i + 1]; q++) {
                ordenIzDe.push(mkLit(inicios[i + 1][q], false)); // +P_{i+1,q}, variable de la clausula que dicta una de las posiciones del siguiente bloque.
            }
            solver.addClause(ordenIzDe); //Creamos la clausula con las variables creadas
        }

        //Propagamos los bloques de derecha a izquierda (Bloque i+1 -> Bloque i)
        for (int q = minPosicion[i + 1]; q <= maxPosicion[i + 1]; q++) {
            vec<Lit> ordenDeIz;
            ordenDeIz.push(mkLit(inicios[i + 1][q], true)); 

            int maxP = q - pistas[i] - 1; // Última posición posible para i (en la primera ejecución de este for, coincide con maxPosicion[i], pero en las siguientes iteraciones irá disminuyendo)
            //Probamos todas las posiciones iniciales del siguiente bloque (1 casilla, 2 casillas, 3, ... antes del primero).
            for (int p = minPosicion[i]; p <= maxP; p++) { 
                ordenDeIz.push(mkLit(inicios[i][p], false)); 
            }
            solver.addClause(ordenDeIz); 
        }
    }

    //Añadimos una clausula para cada casilla que diga que si el bloque empieza en la posición p entonces las casillas desde p hasta p + tamaño del bloque - 1 deben estar pintadas
    for (int i = 0; i < numBloques; i++) {
        for (int p = minPosicion[i]; p <= maxPosicion[i]; p++) {
            for (int k = p; k < p + pistas[i]; k++) {
                solver.addClause(mkLit(inicios[i][p], true), mkLit(matrizVars[k], false));
            }
        }
    }

    //Clausula que indica que después del final de cada bloque debe haber una casilla vacía, es decir la casilla de separación entre bloques
    for (int i = 0; i < numBloques; i++) {
        for (int p = minPosicion[i]; p <= maxPosicion[i]; p++) {
            int separador = p + pistas[i];
            if (separador < longitud) {
                solver.addClause(mkLit(inicios[i][p], true), mkLit(matrizVars[separador], true));
            }
        }
    }

    //Clausula que indica que si la casilla k está pintada entonces debe existir un bloque i y una posición p tal que p(i) <= k <= p(i) + tamaño(i) - 1
    for (int k = 0; k < longitud; k++) {
        vec<Lit> cobertura;
        cobertura.push(mkLit(matrizVars[k], true));
        for (int i = 0; i < numBloques; i++) {
            for (int p = minPosicion[i]; p <= maxPosicion[i]; p++) {
                if (p <= k && k < p + pistas[i]) {
                    cobertura.push(mkLit(inicios[i][p], false)); 
                }
            }
        }
        solver.addClause(cobertura);
    }

}


picross::InputGrid::Constraints convertirAConstraints(const std::vector<std::vector<int>>& pistas) {
    picross::InputGrid::Constraints constraints;
    constraints.reserve(pistas.size());
    for (const auto& linea : pistas) {
        constraints.emplace_back(linea.begin(), linea.end());
    }
    return constraints;
}

int main(int argc, char** argv)
{
    try{
        if(argc != 2)
        {
            cout << "Uso: " << argv[0] << " <archivo>" << endl;
            return 1;
        }
        ifstream archivo(argv[1]);
        if(!archivo.is_open())
        {
            cout << "No se pudo abrir el archivo: " << argv[1] << endl;
            return 1;
        }
        else{
            int numFilas, numColumnas;
            archivo >> numFilas >> numColumnas;
            string resto;
            getline(archivo, resto);

            //Los primeros dos numeros del archivo representan las dimensiones de la matriz
            vector<vector<int>> pistasHorizontal(numFilas);
            vector<vector<int>> pistasVertical(numColumnas);

            //Los siguientes son las pistas horizontales y verticales, que se almacenan en listas de listas de enteros. Cada lista representa una fila o columna y contiene los numeros de las pistas correspondientes.
            for(int i = 0; i < numFilas; i++){
                string secuencia;
                getline(archivo, secuencia, ',');
                //En el formato que pedimos usaremos las pistas separadas por comas y aquellas pistas que tengan mas de un numero seguido iran entre espacios
                stringstream ss(secuencia);
                int numero;
                while(ss >> numero) { // Extrae numero a numero ignorando los espacios
                    pistasHorizontal[i].push_back(numero);
                }
            }

            getline(archivo, resto);
            //Igual que antes con las numColumnas
            for(int i = 0; i < numColumnas; i++){
                string secuencia;
                getline(archivo, secuencia, ',');
                stringstream ss(secuencia);
                int numero;
                while(ss >> numero) { 
                    pistasVertical[i].push_back(numero);
                }
            }

            auto inicio = high_resolution_clock::now(); //Empezamos a contar para sacar el coste en tiempo de nuestro algoritmo

            //A continuacion creamos una matriz de booleanos que representara el mapa
            vector<vector<bool>> picross(numFilas, vector<bool>(numColumnas, false));
            Solver solver;

            //Creamos las variables de MiniSAT (una por cada casilla del tablero)
            vector<vector<Var>> matrizVars(numFilas, vector<Var>(numColumnas));
            for (int i = 0; i < numFilas; i++) {
                for (int j = 0; j < numColumnas; j++) {
                    matrizVars[i][j] = solver.newVar();
                }
            }

            //Aplicamos las reglas definidas anteriormente a las numFilas
            for (int i = 0; i < numFilas; i++) {
                vector<Var> filaVars(numColumnas);
                for (int j = 0; j < numColumnas; j++) {
                    filaVars[j] = matrizVars[i][j];
                }
                //Le pasamos directamente el vector de variables de la fila 'i'
                generarCNFPicrossMiniSat(solver, pistasHorizontal[i], numColumnas, filaVars);
            }

            //Y a las numColumnas también
            for (int j = 0; j < numColumnas; j++) {
                //Extraemos las variables de la columna 'j' para pasárselas a la función
                vector<Var> colVars(numFilas);
                for (int i = 0; i < numFilas; i++) {
                    colVars[i] = matrizVars[i][j];
                }
                generarCNFPicrossMiniSat(solver, pistasVertical[j], numFilas, colVars);
            }

            //Resulvemos y mostramos el resultado
            cout << "Buscando solucion al picross..." << endl;
            bool haySolucion = solver.solve();
            
            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<milliseconds>(fin - inicio);

            cout << "Tiempo de ejecucion: " << duracion.count() << " ms" << endl;

            if (haySolucion) {
                cout << "\nSolucion encontrada!\n" << endl;
                
                for (int i = 0; i < numFilas; i++) {
                    for (int j = 0; j < numColumnas; j++) {
                        //Leemos el valor que el solver le ha asignado a la casilla
                        if (solver.modelValue(matrizVars[i][j]) == l_True) {
                            picross[i][j] = true;
                            cout << "X "; //Imprimimos una X para los bloques
                        } else {
                            picross[i][j] = false;
                            cout << ". "; //Imprimimos un punto para las casillas vacías
                        }
                    }
                    cout << endl;
                }
            } else {
                cout << "\nNo se ha encontrado una solucion para el picross." << endl;
            }

            cout << "\nAhora probamos con la competencia" << endl;

            picross::InputGrid::Constraints rows = convertirAConstraints(pistasHorizontal);
            picross::InputGrid::Constraints cols = convertirAConstraints(pistasVertical);

            inicio = high_resolution_clock::now();
            picross::InputGrid puzzle(rows, cols, "MiPuzzle");

            // 3. Resolver
            const auto solver2 = picross::get_ref_solver();
            if (!solver2) {
                std::cerr << "Error al instanciar el solver de la librería." << std::endl;
                return 1;
            }
            //inicio del crono
            const auto result = solver2->solve(puzzle, 1);
            //fin del crono
            fin = high_resolution_clock::now();
            duracion = duration_cast<milliseconds>(fin - inicio);
            cout << "Tiempo de ejecucion competencia: " << duracion.count() << " ms" << endl;

            if (result.status == picross::Solver::Status::OK) {
                for (const auto& sol : result.solutions) {
                    std::cout << sol.grid << std::endl;
                }
            } else {
                std::cout << "UNSAT (Sin solución)" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        cerr << "\nExcepcion capturada: " << e.what() << endl;
    } catch (...) {
        cerr << "\nHA OCURRIDO UN ERROR" << endl;
    }
    return 0;
}