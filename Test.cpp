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

class Boat{
    public:
    int id;
    bool host;
    int crew_size;
    int total_capacity;
    int capacity;

    Boat(int i, bool h, int cs, int tc){
        id = i;
        host = h;
        crew_size = cs;
        total_capacity = tc;
        capacity = tc - cs;
    }

    void reset_capacity();
};

void Boat::reset_capacity(){
    capacity = total_capacity - crew_size;
}

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

void asign_hosts(vector<Boat> *boats, int cant_hosts){
    for (vector<Boat>::iterator it=boats->begin(); it!=boats->begin()+cant_hosts; ++it){
        it->host = true;
    }
    return;
}

int DIFF(vector< vector<int>> matrix, int T){
    int count;
    int total_penalty = 0;
    for (vector< vector<int>>::iterator it=matrix.begin(); it!=matrix.end(); ++it){
        sort(it->begin(), it->end());
        count = distance(it->begin(), unique(it->begin(), it->end()));
        //cout << count << endl;
        total_penalty = total_penalty + (T - count);
    }
    //cout << "DIFF total penalty: " << total_penalty << endl;
    return total_penalty;
}

int meet(vector< vector<int>> matrix, int g1, int g2){
    int meetings = 0;
    unsigned int j;
    for(j = 0; j < matrix[g1].size(); ++j){
        if(matrix.at(g1).at(j) == matrix.at(g2).at(j))
            meetings++;
        //cout << matrix.at(g1).at(j) << " y " << matrix.at(g2).at(j) << endl;
    }
    //cout << "meetings: " << meetings << endl;
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
    //cout << "ONCE total penalty: " << total_penalty << endl;
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
            //cout << "capacity of " << k << " " << cap << endl;
            if(cap < 0)
                total_penalty += 1 + (abs(cap)-1)/4;
            boats->at(k).reset_capacity();
        }
    }
    //cout << "CAPA total penalty: " << total_penalty << endl;
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
    //cout << "none" << endl;
    int capa = CAPA(boats, *matrix, T, cant_hosts);
    //cout << "capa" << endl;
    int diff = DIFF(*matrix, T);
    //cout << "diff" << endl;
    int once = ONCE(*matrix);
    //cout << "once" << endl;
    return (capa*capa_penalty + diff*diff_penalty + once*once_penalty);
}

void print_matrix(vector< vector<int>> matrix, ofstream &File){
    unsigned int i, j;
    unsigned int rows = matrix.size();
    unsigned int cols = matrix.at(0).size();
    for(i = 0; i < rows; ++i){
        for(j = 0; j < cols; ++j)
            File << matrix.at(i).at(j) << " ";
        File << endl;
    }
}

vector< vector<int>> movement(vector< vector<int>> matrix, vector<Boat> *boats, int cant_host, int *best_sol, int T){
    vector< vector<int>> copy_matrix = matrix;    
    unsigned int i, j;
    int k, new_sol;

    for(i = 0; i < copy_matrix.size(); ++i){
        for(j = 0; j < copy_matrix.at(0).size(); ++j){
            for(k = 0; k < cant_host; ++k){
                copy_matrix.at(i).at(j) = k % cant_host;
                new_sol = evaluation(cant_host, T, &copy_matrix, boats);
                //print_matrix(copy_matrix);
                //cout << "Eval: " << new_sol << endl;
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

void HC(ofstream &File, vector<Boat> *boats, int cant_hosts, int Y, int T, int MAX_ITERS=100, int MAX_RESETS=100){
    time_t start, end;
    time(&start);
    int stop = 0;
    int stuck, new_sol;
    int best_sol = 10000;
    vector< vector<int>> matrix, best_matrix;
    File << "cant_hosts=" << cant_hosts << ";max_iters=" << MAX_ITERS << ";max_resets="<< MAX_RESETS << endl; 
    while(stop < MAX_RESETS){
        matrix = generate_random_sol(Y, T, cant_hosts);
        new_sol = evaluation(cant_hosts, T, &matrix, boats);
        if(stop == 0){
            File << "INITIAL MATRIX: "<< endl; 
            print_matrix(matrix, File); 
            File << "EVAL: " << new_sol << endl;
        }
        if(new_sol < best_sol){
            best_sol = new_sol;
            File << best_sol << endl;
            best_matrix = matrix;
        }
        stuck = 0;
        do{
            matrix = movement(matrix, boats, cant_hosts, &new_sol, T);
            if(new_sol < best_sol){
                best_sol = new_sol;
                File << best_sol << endl;
                best_matrix = matrix;
            }
            stuck++;
        } while(stuck < MAX_ITERS && best_sol <= new_sol);
        stop++;
    }
    File << "FINAL MATRIX: " << endl;
    print_matrix(best_matrix, File);
    File << "BEST SOL EVAL: " << best_sol << endl;
    time(&end);

    double time_taken = double(end - start);
    File << "Time: " << fixed 
         << time_taken << setprecision(5); 
    File << " sec" << endl; 

    return;
}

vector< vector<int>> HC_out(vector<Boat> *boats, int cant_hosts, int Y, int T, int MAX_ITERS=100, int MAX_RESETS=100){
    int stop = 0;
    int stuck, new_sol;
    int best_sol = 10000;
    vector< vector<int>> matrix, best_matrix;
    while(stop < MAX_RESETS){
        matrix = generate_random_sol(Y, T, cant_hosts);
        new_sol = evaluation(cant_hosts, T, &matrix, boats);
        if(new_sol < best_sol){
            best_sol = new_sol;
            best_matrix = matrix;
        }
        stuck = 0;
        do{
            matrix = movement(matrix, boats, cant_hosts, &new_sol, T);
            if(new_sol < best_sol){
                best_sol = new_sol;
                best_matrix = matrix;
            }
            stuck++;
        } while(stuck < MAX_ITERS && best_sol <= new_sol);
        stop++;
    }
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
        ofstream File(out_path);

        vector<string> specs = split(boat_spec, ";");
        vector<Boat> boats = init_boats(Y, specs);
        sort(boats.begin(), boats.begin()+Y, sort_funct);
        //HC(File, &boats, 4, Y, T, 25, 25);
        /*for(hosts = 1; hosts < Y; ++hosts){
            HC(File, &boats, hosts, Y, T, 25, 25);
            HC(File, &boats, hosts, Y, T, 50, 25);
            HC(File, &boats, hosts, Y, T, 25, 50);
            HC(File, &boats, hosts, Y, T, 50, 50);

            HC(File, &boats, hosts, Y, T, 25, 100);
            HC(File, &boats, hosts, Y, T, 100, 25);
            HC(File, &boats, hosts, Y, T, 100, 50);
            HC(File, &boats, hosts, Y, T, 50, 100);

            HC(File, &boats, hosts, Y, T, 100, 100);
        }
        File.close();*/

        ofstream output_file(out_path_true);
        time(&start);
        for(hosts = 1; hosts < Y; ++hosts){
            solutions.push_back(HC_out(&boats, hosts, Y, T, 100, 100));
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

    /*vector< vector<int>> test = {{1,2,3,2,0}, 
                                 {3,1,3,0,2}, 
                                 {2,1,0,3,3}, 
                                 {3,2,1,3,1}, 
                                 {2,0,2,1,1},
                                 {1,3,2,0,3},
                                 {0,3,1,1,2}};*/
    /*vector< vector<int>> test = generate_random_sol(Y, T, 4);*/
    /*vector< vector<int>> test = {{0,2,3,1,3}, 
                                 {3,0,1,1,2}, 
                                 {2,3,1,0,0}, 
                                 {1,3,0,3,2}, 
                                 {1,0,2,0,3},
                                 {0,1,2,3,1},
                                 {2,1,3,2,2}};*/
    /*vector< vector<int>> test = {{0,2,2,3,1}, 
                                 {0,1,0,2,3}, 
                                 {3,0,1,2,1}, 
                                 {3,2,0,1,0}, 
                                 {1,0,0,3,2},
                                 {2,3,1,3,0},
                                 {2,1,3,0,1}};
    int best_sol = evaluation(4, T, &test, &boats);
    print_matrix(test);
    cout << "EVAL: " << best_sol << endl;*/
    //test = movement(test, &boats, 4, &best_sol, T);
    //cout << "BEST 2: " << best_sol << endl;
    return 0;
}