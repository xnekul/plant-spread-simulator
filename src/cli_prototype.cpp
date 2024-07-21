#include "weedsim.hpp"
#include "experiments.hpp"

int main(){

    int x = 120;
    int y = 120;

    WeedSimulator sim(y,x);
    sim.init_weed(x/4,y-20);
    sim.init_weed(x/2,y-20);
    sim.init_weed(x/4*3,y-20);
    
    void (*exp_list[3])(WeedSimulator *) = {experiment_0,experiment_1,experiment_2};
    for (size_t i = 0; i < 3; i++)
    {
        sim.reset();
        sim.set_cleaning(0,1,topbottom,1);

        sim.init_weed(x/5*1,y-20);
        sim.init_weed(x/5*2,y-20);
        sim.init_weed(x/5*3,y-20);
        sim.init_weed(x/5*4,y-20);

        sim.iterate(2);
        sim.year_count=0;

        exp_list[i](&sim);
    }
    

    return 0;
}