#include "clase.h"

Boat::Boat(int i, bool h, int cs, int tc){
    id = i;
    host = h;
    crew_size = cs;
    total_capacity = tc;
    capacity = tc - cs;
}

void Boat::reset_capacity(){
    capacity = total_capacity - crew_size;
}