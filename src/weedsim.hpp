#ifndef _WEEDSIM
#define _WEEDSIM

#include <cstdio>
#include <vector>
#include <list>
#include <iostream>
#include <random>
#include <chrono>

enum CellState{
    empty,weed,new_weed,none
};

enum Direction{
    topbottom, bottomtop, leftright, rightleft,tocenter
};

typedef std::mt19937 RNG;  
typedef std::vector<std::vector<CellState>> CellGrid2D;

struct coordinates{
    int x;
    int y;
};

class WeedSimulator{
    public:
        WeedSimulator(int rows, int cols);
        ~WeedSimulator(){};

        void change_sim_params(const float v=0.7,const float alfa=50,const float seedling_survival=0.012,Direction tide_type=topbottom);
        int iterate(int iterations = 1,int until_budget=INT32_MAX);
        void set_cleaning(int yearly_budget = 1000,float seed_suppresion = 0,
                            Direction clearing_direction=topbottom,int cell_cost = 1);
        void init_weed(int x, int y);
        void remove_weed(int x, int y);
        void reset();
        void print();
        void info();
        CellGrid2D* get_grid();

        void save_grid();
        void reload_grid();


    //private:
        CellGrid2D cell_grid;
        CellGrid2D checkpoint_grid;

        std::uniform_int_distribution<uint32_t> uint_dist;
        RNG rng;


        float vegetative_speed;
        float vegetative_corner_speed;
        float seedling_speed_help; // do not use, use get_seedling_speed()
        float get_seedling_speed(float distance, float tidal_fraction);


        float v = 0.7;
        float alfa = 50;
        float seedling_survival = 0.012;
        Direction tide_type = topbottom;
        
        
        int rows;
        int cols;

        Direction clearing_direction = topbottom;
        
        int total_budget = 0;
        int yearly_budget = 0;
        float seed_suppresion = 0;
        int cell_cost = 1;

        int total_costs = 0;
        int thisyear_costs = 0;
        int year_count = 0;
        int weed_cout = 0;



        void set_cell(int x,int y,CellState state);
        int get_cell(int x,int y);

        void populate();
        void populate_by_seedlings(int x,int y);
        void populate_vegetatively(int x, int y);
        void finalize_year();
        bool clean_weeds();
        bool clean_cell(int x, int y);
        float get_rand_float(); // returns random float from range 0...1
        void update_speed();
        float get_tidal_fraction(int x,int y);


};


#endif