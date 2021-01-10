#ifndef funciones
#define funciones

#include <stdio.h>
#include <string>
#include <vector>
#include "clase.h"
using namespace std;

vector<string> split(string input, string delimiter);

vector<Boat> init_boats(int Y, vector<string> specs);

bool sort_funct(Boat i, Boat j);

bool sort_by_id(Boat i, Boat j);

bool sort_by_host(Boat i, Boat j);

void asign_hosts(vector<Boat> *boats, int cant_hosts);

void asign_hosts_big(Boat *boat);

int DIFF(vector< vector<int>> matrix, int T);

int meet(vector< vector<int>> matrix, int g1, int g2);

int ONCE(vector< vector<int>> matrix);

int CAPA(vector<Boat> *boats, vector< vector<int>> matrix, int T, int cant_hosts);

vector< vector<int>> generate_random_sol(int Y, int T, int cant_hosts);

int evaluation(int cant_hosts, int T, vector< vector<int>> *matrix, vector<Boat> *boats, int capa_penalty, int diff_penalty, int once_penalty);

//void print_matrix(vector< vector<int>> matrix, ofstream &File);

vector< vector<int>> movement(vector< vector<int>> matrix, vector<Boat> *boats, int cant_host, int *best_sol, int T, int op, int dp, int cp);

vector< vector<int>> HC(ofstream &File, vector<Boat> *boats, int cant_hosts, int Y, int T, int op, int dp, int cp,int MAX_RESETS);

void write_output(ofstream &File, vector< vector< vector<int> > > *solutions, int T, int Y, vector<Boat> *boats);

#endif
