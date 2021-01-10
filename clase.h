#ifndef clase
#define clase

class Boat{
    public:
    int id;
    bool host;
    int crew_size;
    int total_capacity;
    int capacity;

    Boat(int i, bool h, int cs, int tc);
    void reset_capacity();
};
#endif