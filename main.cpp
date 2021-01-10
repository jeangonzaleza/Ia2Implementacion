#include "funciones.h"
#include "clase.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <iomanip>
using namespace std;


int main(){
    int Y, T, i, hosts;
    string boat_spec;
    ifstream p_instances;
    string out_path, out_path_true;
    time_t start, end;
    vector< vector< vector<int> > > solutions;

    for(i = 1; i < 11; ++i){
        if(i != 10){
            ifstream p_instances("./Instancias PPP/Instancias CSPLib/Ian0" + to_string(i) + ".txt");
            getline(p_instances, boat_spec);
            Y = stoi(boat_spec);
            getline(p_instances, boat_spec);
            T = stoi(boat_spec);
            getline(p_instances, boat_spec);
            p_instances.close();
            out_path = "./OutputPPP/out_Ian0" + to_string(i) + ".txt";
            out_path_true = "./OutputPPP/Ian0" + to_string(i) + ".out";
        }
        else{
            ifstream p_instances("./Instancias PPP/Instancias CSPLib/Ian10.txt");
            getline(p_instances, boat_spec);
            Y = stoi(boat_spec);
            getline(p_instances, boat_spec);
            T = stoi(boat_spec);
            getline(p_instances, boat_spec);
            p_instances.close();
            out_path = "./OutputPPP/out_Ian10.txt";
            out_path_true = "./OutputPPP/Ian10.out";
        }

        vector<string> specs = split(boat_spec, ";");
        vector<Boat> boats = init_boats(Y, specs);
        sort(boats.begin(), boats.begin()+Y, sort_funct);

        // ========== EXPERIMENTOS MODIFICANDO PARAMETROS ==========//

        ofstream File(out_path);

        for(hosts = 1; hosts < Y; ++hosts){
            HC(File, &boats, hosts, Y, T, 1, 1, 1, 100);
            HC(File, &boats, hosts, Y, T, 5, 1, 1, 100);
            HC(File, &boats, hosts, Y, T, 1, 5, 1, 100);
            HC(File, &boats, hosts, Y, T, 1, 1, 5, 100);

            HC(File, &boats, hosts, Y, T, 1, 1, 1, 1000);
            HC(File, &boats, hosts, Y, T, 5, 1, 1, 1000);
            HC(File, &boats, hosts, Y, T, 1, 5, 1, 1000);
            HC(File, &boats, hosts, Y, T, 1, 1, 5, 1000);
        }
        File.close();

        // ========== Instancias pequeñas ==========//

        ofstream output_file(out_path_true);
        time(&start);
        for(hosts = 1; hosts < Y; ++hosts){
            solutions.push_back(HC(File, &boats, hosts, Y, T, 5, 1, 2, 1000));
        }
        time(&end);
        
        write_output(output_file, &solutions, T, Y, &boats);
        double time_taken = double(end - start);
        output_file << "Tiempo de ejecución: " << fixed 
            << time_taken << setprecision(2); 
        output_file << " [s]" << endl;
        output_file.close();
        solutions.clear();
    }

    // ========== Instancias grandes ==========//

    string conf;
    vector<string> configuracion;
    unsigned int j;
    int host_asignado;

    ifstream p_big_instances("./Instancias PPP/Configuraciones/PPP.txt");
    
    getline(p_big_instances, boat_spec);
    Y = stoi(boat_spec);
    getline(p_big_instances, boat_spec);
    T = stoi(boat_spec);
    getline(p_big_instances, boat_spec);
    p_big_instances.close();
    
    out_path = "./BigOutputPPP/PPP.txt";
    out_path_true = "./BigOutputPPP/PPP.out";
    
    vector<string> specs = split(boat_spec, ";");
    vector<Boat> boats = init_boats(Y, specs);

    for(i = 1; i <= 6; ++i){
        ifstream config("./Instancias PPP/Configuraciones/config" + to_string(i) + ".txt");
        out_path = "./BigOutputPPP/config" + to_string(i) + ".txt";
        out_path_true = "./BigOutputPPP/config" + to_string(i) + ".out";
        getline(config, conf);
        configuracion = split(conf, " ");

        //cout << configuracion.size() << endl;

        for(j = 0; j < configuracion.size(); ++j){
            host_asignado = stoi(configuracion.at(j));
            asign_hosts_big(&boats.at(host_asignado-1));
        }
        sort(boats.begin(), boats.begin()+Y, sort_by_host);

        ofstream output_big(out_path_true);
        ofstream File_big(out_path);
        time(&start);
        solutions.push_back(HC(File_big, &boats, (int) configuracion.size(), Y, T, 1, 1, 1, 10));
        time(&end);
        
        write_output(output_big, &solutions, T, Y, &boats);
        double time_taken = double(end - start);
        output_big << "Tiempo de ejecución: " << fixed 
            << time_taken << setprecision(2); 
        output_big << " [s]" << endl;
        output_big.close();
        solutions.clear();
    }

    return 0;
}