#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include "funciones.h"

vector<string> split(string input, string delimiter){
    size_t pos = 0;
    std::string token;
    vector<string> result;

    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        result.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    token = input.substr(0, pos);
    result.push_back(token);
    input.erase(0, pos + delimiter.length());
    return result;
}

vector<Boat> init_boats(int Y, vector<string> specs){
    vector<Boat> boats;
    int i, crew_size, total_cap;
    vector<string> spec;
    for(i = 0; i < Y; ++i){
        spec = split(specs.at(i), ",");
        crew_size = stoi(spec.at(1));
        total_cap = stoi(spec.at(0));
        Boat new_boat(i+1, false, crew_size, total_cap);
        boats.push_back(new_boat);
    }
    return boats;
}

bool sort_funct(Boat i, Boat j){
    return i.capacity > j.capacity;
}

bool sort_by_id(Boat i, Boat j){
    return i.id < j.id;
}

bool sort_by_host(Boat i, Boat j){
    return i.host > j.host;
}

void asign_hosts(vector<Boat> *boats, int cant_hosts){
    for (vector<Boat>::iterator it=boats->begin(); it!=boats->begin()+cant_hosts; ++it){
        it->host = true;
    }
    return;
}

void asign_hosts_big(Boat *boat){
    boat->host = 1;
    return;
}

int DIFF(vector< vector<int>> matrix, int T){
    int count;
    int total_penalty = 0;
    for (vector< vector<int>>::iterator it=matrix.begin(); it!=matrix.end(); ++it){
        sort(it->begin(), it->end());
        count = distance(it->begin(), unique(it->begin(), it->end()));
        total_penalty = total_penalty + (T - count);
    }
    return total_penalty;
}

int meet(vector< vector<int>> matrix, int g1, int g2){
    int meetings = 0;
    unsigned int j;
    for(j = 0; j < matrix[g1].size(); ++j){
        if(matrix.at(g1).at(j) == matrix.at(g2).at(j))
            meetings++;
    }
    if (meetings == 1) return 0;
    else if(meetings == 0) return -1;
    return (meetings - 1);
}

int ONCE(vector< vector<int>> matrix){
    int total_penalty = 0;
    int guest = matrix.size();
    int i, j, meetings;
    for(i = 0; i < (guest - 1); ++i){
        for(j = i+1; j < guest; ++j){
            meetings = meet(matrix, i, j);
            total_penalty += meetings;
            if(meetings == -1)
                total_penalty += 2;
        }
    }
    return total_penalty;
}

int CAPA(vector<Boat> *boats, vector< vector<int>> matrix, int T, int cant_hosts){
    int total_penalty = 0;
    unsigned int i; 
    int j, k, host, cap;

    for(j = 0; j < T; ++j){
        for (i = 0; i < matrix.size(); ++i){
            host = matrix.at(i).at(j);
            boats->at(host).capacity -= boats->at(i + cant_hosts).crew_size;
        }
        for (k = 0; k < cant_hosts; ++k){
            cap = boats->at(k).capacity;
            if(cap < 0)
                total_penalty += 1 + (abs(cap)-1)/4;
            boats->at(k).reset_capacity();
        }
    }
    return total_penalty;
}

vector< vector<int>> generate_random_sol(int Y, int T, int cant_hosts){
    int i, j, cant_guest;
    cant_guest = Y - cant_hosts;
    vector< vector<int>> matrix;
    srand(time(NULL));
    for (i = 0; i < cant_guest; ++i){
        vector<int> aux_vector;
        for (j = 0; j < T; ++j)
            aux_vector.push_back(rand() % cant_hosts);
        matrix.push_back(aux_vector);
    }
    return matrix;
}

int evaluation(int cant_hosts, int T, vector< vector<int>> *matrix, vector<Boat> *boats, int capa_penalty = 2, int diff_penalty = 1, int once_penalty = 3){
    int capa = CAPA(boats, *matrix, T, cant_hosts);
    int diff = DIFF(*matrix, T);
    int once = ONCE(*matrix);
    return (capa*capa_penalty + diff*diff_penalty + once*once_penalty);
}

/*void print_matrix(vector< vector<int>> matrix, ofstream &File){
    unsigned int i, j;
    unsigned int rows = matrix.size();
    unsigned int cols = matrix.at(0).size();
    for(i = 0; i < rows; ++i){
        for(j = 0; j < cols; ++j)
            File << matrix.at(i).at(j) << " ";
        File << endl;
    }
}*/

vector< vector<int>> movement(vector< vector<int>> matrix, vector<Boat> *boats, int cant_host, int *best_sol, int T, int op, int dp, int cp){
    vector< vector<int>> copy_matrix = matrix;    
    unsigned int i, j;
    int k, new_sol;

    for(i = 0; i < copy_matrix.size(); ++i){
        for(j = 0; j < copy_matrix.at(0).size(); ++j){
            for(k = 0; k < cant_host; ++k){
                copy_matrix.at(i).at(j) = k % cant_host;
                new_sol = evaluation(cant_host, T, &copy_matrix, boats, cp, dp, op);
                if(new_sol < *best_sol){
                    *best_sol = new_sol;
                    return copy_matrix;
                }
                copy_matrix = matrix;
            }
        }
    }
    return matrix;
}

vector< vector<int>> HC(ofstream &File, vector<Boat> *boats, int cant_hosts, int Y, int T, int op, int dp, int cp,int MAX_RESETS=100){
    time_t start, end;
    time(&start);
    int stop = 0;
    int new_sol;
    int best_sol; //Candidata a mejor solucion
    int mejor_solucion_absoluta = 1000000;
    vector< vector<int>> matrix, best_matrix, candidata;
    File << "cant_hosts=" << cant_hosts << ";max_resets="<< MAX_RESETS << ";CAPA penalty=" << cp << ";DIFF penalty=" << dp << ";ONCE penalty=" << op <<endl; 
    while(stop < MAX_RESETS){
        best_sol = 1000000;
        matrix = generate_random_sol(Y, T, cant_hosts);
        new_sol = evaluation(cant_hosts, T, &matrix, boats, cp, dp, op);
        bool hay_mejora = true;
        if(new_sol < best_sol){
            best_sol = new_sol;
            File << best_sol << endl;
            candidata = matrix;
        }
        do{
            matrix = movement(matrix, boats, cant_hosts, &new_sol, T, op, dp, cp);
            if(new_sol < best_sol){
                best_sol = new_sol;
                File << best_sol << endl;
                candidata = matrix;
            }
            else{
                hay_mejora = false;
            }
        } while(hay_mejora);
        if(best_sol < mejor_solucion_absoluta){
            mejor_solucion_absoluta = best_sol;
            best_matrix = candidata;
        }
        stop++;

    }
    File << "BEST SOL EVAL: " << mejor_solucion_absoluta << endl;
    time(&end);

    double time_taken = double(end - start);
    File << "Time: " << fixed 
         << time_taken << setprecision(5); 
    File << " sec" << endl; 
    return best_matrix;
}

void write_output(ofstream &File, vector< vector< vector<int> > > *solutions, int T, int Y, vector<Boat> *boats){
    int best_i = 0;
    int best_eval = 100000;
    int eval;
    unsigned int i,j,k;
    for (i = 0; i < solutions->size(); ++i){
        eval = evaluation(i+1, T, &(solutions->at(i)), boats);
        if(eval < best_eval){
            best_i = i;
            best_eval = eval;
        }
    }
    vector< vector<int>> adjacent_list;
    vector<int> yates;
    int cant_hosts = best_i + 1;
    int yat;
    int host, id_host, id_guest;
    for(yat = 0; yat < Y; ++yat)
        adjacent_list.push_back(yates);
    vector< vector<int>> *best_mat = &(solutions->at(best_i));
    for(j = 0; j < best_mat->at(0).size(); ++j){
        for(i = 0; i < best_mat->size(); ++i){
            host = best_mat->at(i).at(j);
            id_host = boats->at(host).id;
            boats->at(host).capacity -= boats->at(cant_hosts+i).crew_size;
            id_guest = boats->at(cant_hosts+i).id;
            adjacent_list.at(id_host-1).push_back(id_guest);
            
        }
        sort(boats->begin(), boats->begin()+Y, sort_by_id);
        File << "t=" << j+1 << endl << "----------" << endl;
        for(i = 0; i < adjacent_list.size(); ++i){
            File << i+1 << " -> ";
            if(!adjacent_list.at(i).empty()){
                for(k = 0; k < adjacent_list.at(i).size(); ++k){
                    File << adjacent_list.at(i).at(k) << ",";
                }
                File << "[" << (boats->at(i).total_capacity - boats->at(i).capacity) << "/" << boats->at(i).total_capacity << "]" << endl;
            }
            else
                File << "0" << endl;
            boats->at(i).reset_capacity();
            adjacent_list.at(i).clear();
        }
        sort(boats->begin(), boats->begin()+Y, sort_funct);
    }
    File << "La cantidad de yates anfitriones óptimos es: " << cant_hosts << endl;

    return;
}