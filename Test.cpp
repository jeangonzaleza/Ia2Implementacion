#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
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

void HC(){
    return;
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
        Boat new_boat(i, false, crew_size, total_cap);
        boats.push_back(new_boat);
    }
    return boats;
}

bool sort_funct(Boat i, Boat j){
    return i.capacity > j.capacity;
}

void asign_hosts(vector<Boat> *boats, int cant_hosts){
    for (vector<Boat>::iterator it=boats->begin(); it!=boats->begin()+cant_hosts; ++it){
        it->host = true;
    }
    return;
}

int DIFF(vector< vector<int>> matrix, int cant_host){
    int count;
    int total_penalty = 0;
    for (vector< vector<int>>::iterator it=matrix.begin(); it!=matrix.end(); ++it){
        sort(it->begin(), it->end());
        count = distance(it->begin(), unique(it->begin(), it->end()));
        //cout << count << endl;
        total_penalty = total_penalty + (cant_host - count);
    }
    cout << "DIFF total penalty: " << total_penalty << endl;
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
    if (meetings <= 1) return 0;
    return (meetings - 1);
}

int ONCE(vector< vector<int>> matrix){
    int total_penalty = 0;
    int guest = matrix.size();
    int i, j;
    for(i = 0; i < (guest - 1); ++i){
        for(j = i+1; j < guest; ++j)
            total_penalty += meet(matrix, i, j);
    }
    cout << "ONCE total penalty: " << total_penalty << endl;
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
    cout << "CAPA total penalty: " << total_penalty << endl;
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

int evaluation(int cant_hosts, int T, vector< vector<int>> *matrix, vector<Boat> *boats, int capa_penalty = 1, int diff_penalty = 1, int once_penalty = 1){
    int capa = CAPA(boats, *matrix, T, cant_hosts);
    int diff = DIFF(*matrix, cant_hosts);
    int once = ONCE(*matrix);
    return (capa + diff + once);
}

void print_matrix(vector< vector<int>> matrix){
    unsigned int i, j;
    unsigned int rows = matrix.size();
    unsigned int cols = matrix.at(0).size();
    for(i = 0; i < rows; ++i){
        for(j = 0; j < cols; ++j)
            cout << matrix.at(i).at(j) << " ";
        cout << endl;
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
                print_matrix(copy_matrix);
                cout << "Eval: " << new_sol << endl;
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

int main(){
    int Y;
    int T;
    string boat_spec;
    ifstream p_instances("Ian07.txt");

    getline(p_instances, boat_spec);
    Y = stoi(boat_spec);
    getline(p_instances, boat_spec);
    T = stoi(boat_spec);
    getline(p_instances, boat_spec);

    p_instances.close();

    vector<string> specs = split(boat_spec, ";");
    vector<Boat> boats = init_boats(Y, specs);
    sort(boats.begin(), boats.begin()+Y, sort_funct);

    vector< vector<int>> test = generate_random_sol(Y, T, 4);
    int best_sol = evaluation(4, T, &test, &boats);
    print_matrix(test);
    cout << "BEST 1: " << best_sol << endl;
    test = movement(test, &boats, 4, &best_sol, T);
    cout << "BEST 2: " << best_sol << endl;
    return 0;
}