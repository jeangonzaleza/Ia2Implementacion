#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

vector< vector<int>> generate_random_sol(int Y, int T, int cant_hosts, vector<Boat> *boats){

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

    /*cout << Y << endl;
    cout << T << endl;
    cout << boat_spec << endl;*/

    vector<string> specs = split(boat_spec, ";");
    vector<Boat> boats = init_boats(Y, specs);
    sort(boats.begin(), boats.begin()+Y, sort_funct);

    /*asign_hosts(&boats, 4);

    cout << "myvector contains:";
    for (vector<Boat>::iterator it=boats.begin(); it!=boats.end(); ++it)
        cout << ' ' << it->host;
    cout << '\n';
    return 0;*/
}