#include "weedsim.hpp"

std::ostream& operator<<(std::ostream& os, const Direction& value) {
    switch (value) {
        case Direction::topbottom:
            os << "topbottom";
            break;
        case Direction::bottomtop:
            os << "bottomtop";
            break;
        case Direction::leftright:
            os << "Value3";
            break;
        case Direction::rightleft:
            os << "Value3";
            break;
        case Direction::tocenter:
            os << "Value3";
            break;

    }
    return os;
}


float WeedSimulator::get_tidal_fraction(int x,int y){
    switch (tide_type)
    {
    case topbottom:
        return static_cast<float>(y)/static_cast<float>(rows);
        break;
    
    default:
        return static_cast<float>(y)/static_cast<float>(rows);
        break;
    }
}


float WeedSimulator::get_seedling_speed(float distance, float tidal_fraction){
    return seedling_speed_help / pow(M_E,pow(distance/alfa,2)) * tidal_fraction;
}


void WeedSimulator::update_speed(){
    vegetative_speed = v;
    vegetative_corner_speed = 0.05 * vegetative_speed;

    seedling_speed_help = 1/(M_PI*alfa*alfa*pow(M_E,1/(alfa*alfa))) * seedling_survival * (1-seed_suppresion); 
}

float WeedSimulator::get_rand_float(){
    return static_cast<float>(uint_dist(rng))/static_cast<float>(uint_dist.max());

}

WeedSimulator::WeedSimulator(int rows, int cols){

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);

    this->cell_grid.reserve(rows);
    this->rows = rows;
    this->cols = cols;
    year_count = 0;

    for( int y = 0; y < rows; y++){
        this->cell_grid.emplace_back();
        this->cell_grid[y].resize(cols);
    }
    reset();

    update_speed();
}

void WeedSimulator::reset(){
    year_count = 0;
    total_costs = 0;
    thisyear_costs = 0;    

    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols;x++){
            set_cell(x,y,empty);
        }
    }
}

int WeedSimulator::iterate(int iterations,int until_budget){
    this->total_budget = until_budget;

    for(int i = 0;i<iterations;i++){
        year_count++;
        std::cout << "Year:" << year_count << std::endl;
        clean_weeds();
        populate();
        finalize_year();

        if(total_costs+cell_cost >= total_budget){
            std::cout << total_costs+cell_cost << ">=" << total_budget << std::endl;
            break;
        }

    }
    return year_count;
}

void WeedSimulator::set_cleaning(int yearly_budget,float seed_suppresion,Direction clearing_direction,int cell_cost){
    this->clearing_direction = clearing_direction;
    this->yearly_budget = yearly_budget;
    this->seed_suppresion = seed_suppresion;
    this->cell_cost = cell_cost;
    
    update_speed();
}

void WeedSimulator::init_weed(int x, int y){
    set_cell(x,y,weed);
}

void WeedSimulator::print(){

    std::cout << '\n';
    std::cout << "--------------------------------------\n";
    for(int y = 0;y<rows;y++){
        for(int x = 0; x<cols;x++){
            if(get_cell(x,y) == weed){
                std::cout << "X";
            }
            else if(get_cell(x,y) == empty){
                std::cout << " ";
            }
            else{
                std::cout << ".";
            }
        }
        std::cout << '\n';
    }


    std::cout << "--------------------------------------\n";

}

void WeedSimulator::info(){
    int weed_cell_count = 0;

    for(int y = 0;y<rows;y++){
        for(int x = 0; x<cols;x++){
            if(get_cell(x,y) == weed){
                weed_cell_count++;
            }
        }
    }
    std::cout << "<--INFO:-->\n";
    std::cout << "->SETTINGS INFO\n";
    std::cout << "Cell cost: " << cell_cost << '\n';
    std::cout << "Seed suppresion: " << seed_suppresion << '\n';
    std::cout << "Clearing direction: " << clearing_direction << '\n';
    std::cout << "Tide type:" << tide_type << '\n';
    std::cout << "Year budget: " << yearly_budget << '\n';

    std::cout << "->RUN INFO\n";
    std::cout << "Current year: " << year_count << '\n';
    std::cout << "Total costs: " << total_costs << '\n';
    std::cout << "Total budget: " << total_budget << '\n';
    std::cout << "Budget left: " << total_budget - total_costs << '\n';
    std::cout << "Weed count: " << weed_cell_count << '\n';

}


void WeedSimulator::change_sim_params(const float v,const float alfa,const float seedling_survival,Direction tide_type){
    this->v = v;
    this->alfa = alfa;
    this->seedling_survival = seedling_survival;
    this->tide_type = tide_type;
    update_speed();
}


int WeedSimulator::get_cell(int x,int y){
    if(0 <= x && x < cols && 0 <= y && y < rows ){
        return cell_grid[y][x];
    }
    else{
        return none;
    }
}

void WeedSimulator::set_cell(int x,int y,CellState state){
    if(0 <= x && x < cols && 0 <= y && y < rows ){
        this->cell_grid[y][x] = state;
    }
}

void WeedSimulator::populate_by_seedlings(int x,int y){
    float distance;
    float tidal_fraction;

    for(int target_y = 0;target_y<rows;target_y++){
        for(int target_x = 0;target_x<cols;target_x++){
            if(get_cell(target_x,target_y)==empty){
                distance = sqrt(pow(abs(x-target_x),2)+pow(abs(y-target_y),2));
                tidal_fraction = get_tidal_fraction(target_x,target_y);
                if(get_seedling_speed(distance,tidal_fraction) >= get_rand_float()){
                    set_cell(target_x,target_y,new_weed);
                }
            }
        }
    }
}
void WeedSimulator::populate_vegetatively(int x, int y){
    //indexes of immediate cell neighbours
    const int x_side[4] = {0,-1,0,1};
    const int y_side[4] = {1,0,-1,0};

    //indexes of corner cell neighbours
    const int x_corner[4] = {1,-1,1,-1};
    const int y_corner[4] = {1,-1,-1,1};

    int cur_x;
    int cur_y;

    for(int i = 0;i < 4; i++){
        //for side cell
        cur_x = x+x_side[i];
        cur_y = y+y_side[i];
        if(get_cell(cur_x,cur_y)==empty){
            if(vegetative_speed >= get_rand_float()){
                set_cell(cur_x,cur_y,new_weed);
            }
        }
        //for corner cell
        cur_x = x+x_corner[i];
        cur_y = y+y_corner[i];
        if(get_cell(cur_x,cur_y)==empty){
            if(vegetative_corner_speed >= get_rand_float()){
                set_cell(cur_x,cur_y,new_weed);
            }
        }

    }
}
void WeedSimulator::populate(){
    for(int y = 0;y<rows;y++){
        for(int x = 0;x<cols;x++){
            //std::cout << "[" << x << "," << y << "]"<< std::endl;
            if(get_cell(x,y)==weed){
                populate_vegetatively(x,y);
                populate_by_seedlings(x,y);
            }
        }
    }
}
/*
    If cell is infested, checks if there is still budget for year/at all. 
        If yes clears and returns 0, if there are no funds returns 0;
*/
bool WeedSimulator::clean_cell(int x, int y){
    if (get_cell(x,y) == weed){
        thisyear_costs+= cell_cost;
        if(thisyear_costs > yearly_budget || thisyear_costs+total_costs > total_budget){
            total_costs+=thisyear_costs-cell_cost;
            return 1;
        }
        else{
            set_cell(x,y,empty);
            return 0;
        }
        
    }
    return 0;
}

bool WeedSimulator::clean_weeds(){
    thisyear_costs = 0;
    switch (clearing_direction)
    {
    case topbottom: // default
        for(int y = 0; y<rows; y++){
            for(int x = 0; x<cols; x++){
                if(clean_cell(x,y)){
                    return 1;
                }
            }
        }
        break;
    case bottomtop: // reversed y
        for(int y = 0; y<rows; y++){
            for(int x = 0; x<cols; x++){
                if(clean_cell(x,rows-y-1)){
                    return 1;
                }
            }
        }
        break;
    case leftright: // default, switched x y
        for(int x = 0; x<cols; x++){
            for(int y = 0; y<rows; y++){
                if(clean_cell(x,y)){
                    return 1;
                }
            }
        }
        break;
    case rightleft: // reversed x, switched x y
        for(int x = 0; x<cols; x++){
            for(int y = 0; y<rows; y++){
                if(clean_cell(cols-x-1,y)){
                    return 1;
                }
            }
        }
        break;
    default:
        break;
    }
    return 0;
}
void WeedSimulator::finalize_year(){
    weed_cout = 0;
    for(int y = 0;y<rows;y++){
        for(int x = 0;x<cols;x++){
            if(get_cell(x,y) == new_weed){
                set_cell(x,y,weed);
            }
            if(get_cell(x,y) == weed){
                weed_cout++;
            }
        }
    }
}

CellGrid2D* WeedSimulator::get_grid(){
    return &cell_grid;
}

void WeedSimulator::remove_weed(int x, int y){
    set_cell(x,y,empty);
}

void WeedSimulator::save_grid(){
    checkpoint_grid = cell_grid;
}

void WeedSimulator::reload_grid(){
    cell_grid = checkpoint_grid;
}
