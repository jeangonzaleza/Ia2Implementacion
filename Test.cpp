#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Boat{
    public:
    int id;
    bool guest;
    bool host;
    int crew_size;
    int total_capacity;
    int capacity;

    Boat(int i, bool g, bool h, int cs, int tc){
        id = i;
        guest = g;
        host = h;
        crew_size = cs;
        total_capacity = tc;
        capacity = tc - cs;
    }

    //void update_capacity();
    //void update_host();
    //void update_guest();
};

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
        Boat new_boat(i, false, false, crew_size, total_cap);
        boats.push_back(new_boat);
    }
    return boats;
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

    cout << Y << endl;
    cout << T << endl;
    cout << boat_spec << endl;

    vector<string> specs = split(boat_spec, ";");
    vector<Boat> boats = init_boats(Y, specs);

    cout << boats.at(10).id << endl;
    cout << boats.at(1).id << endl;
    return 0;
}