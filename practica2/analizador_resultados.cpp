#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {
    bool cabecera = true;
    //Verificamos que se haya pasado el nombre del archivo como argumento
    if (argc < 2) {
        cerr << "Error: Faltan argumentos." << endl;
        cerr << "Uso: " << argv[0] << " <archivo_resultados.txt>" << endl;
        return 1;
    }else if(argc == 3){
        if(string(argv[2]) == "0"){
            cabecera = false; //Si se pasa un segundo argumento con valor 0, no mostramos la cabecera
        }
    }

    string filename = argv[1];
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo '" << filename << "'." << endl;
        return 1;
    }

    string line;
    string t_expander = "-", t_expander_sin = "-", l_expander = "-";
    string t_prob_total = "-", t_prob_medio = "-", l_prob = "-", prob = "-";

    //Expresiones regulares para capturar los datos   
    regex r_t_exp("Tiempo de ejecucion del algoritmo expander: ([0-9.eE+-]+) segundos");
    regex r_t_exp_sin("Tiempo de ejecucion del algoritmo expander \\(Sin expandir\\): ([0-9.eE+-]+) segundos");
    regex r_l_exp("Camino expander encontrado con una longitud de ([0-9]+)\\.");
    
    regex r_t_prob_medio("Tiempo medio de ejecucion del algoritmo probabilista: ([0-9.eE+-]+) segundos");
    regex r_t_prob_total("Tiempo total de ejecucion del algoritmo probabilista: ([0-9.eE+-]+) segundos");
    regex r_l_prob("Camino probabilista encontrado con una longitud de ([0-9]+)\\.");
    regex r_prob("Probabilidad de encontrar el camino mas corto > ([0-9.]+)%");
    
    smatch match;

    //Leemos el archivo linea por linea
    while (getline(file, line)) {
        if (regex_search(line, match, r_t_exp)) t_expander = match[1].str();
        else if (regex_search(line, match, r_t_exp_sin)) t_expander_sin = match[1].str();
        else if (regex_search(line, match, r_l_exp)) l_expander = match[1].str();
        else if (regex_search(line, match, r_t_prob_medio)) t_prob_medio = match[1].str();
        else if (regex_search(line, match, r_t_prob_total)) t_prob_total = match[1].str();
        else if (regex_search(line, match, r_l_prob)) l_prob = match[1].str();
        else if (regex_search(line, match, r_prob)) prob = "> " + match[1].str() + "%";
    }
    file.close();

    //Dibujamos la tabla de resultados
    if(cabecera){
        cout << string(70, '-') << endl;
        cout << left << setw(32) << "Metrica" 
             << "| " << setw(16) << "Expander" 
             << "| " << setw(16) << "Probabilista"
             << "| " << setw(16) << "Test" << endl;
        cout << string(88, '-') << endl;
    }
    
    cout << left << setw(32) << "Tiempo total (segundos)" 
         << "| " << setw(16) << t_expander 
         << "| " << setw(16) << t_prob_total 
         << "| " << setw(16) << filename << endl;
         
    cout << left << setw(32) << "Tiempo medio (segundos)" 
         << "| " << setw(16) << "N/A" 
         << "| " << setw(16) << t_prob_medio
         << "| " << setw(16) << endl;

    cout << left << setw(32) << "Tiempo sin Expandir (segundos)" 
         << "| " << setw(16) << t_expander_sin
         << "| " << setw(16) << "N/A"
         << "| " << setw(16) << endl;
         
    cout << left << setw(32) << "Longitud del camino" 
         << "| " << setw(16) << l_expander 
         << "| " << setw(16) << l_prob
         << "| " << setw(16) << endl;
         
    cout << left << setw(32) << "Probabilidad camino optimo" 
         << "| " << setw(16) << "N/A" 
         << "| " << setw(16) << prob
         << "| " << setw(16) << endl;
         
    cout << string(88, '-') << endl;

    return 0;
}
