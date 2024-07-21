#include "experiments.hpp"

#define SIM (*sim)

const std::string EXPERIMENTS_DESCRIPTION = 
			"-> experiment_0\n"
			"  description: run Outlying clones first for 20 years, then Meadow first for 20 years\n"
			"  precondition: initial weed population must be already set\n"
			"  output: yearly weedcount data in exp0_result.txt file\n"

			"-> experiment_1\n"
			"  description: run Outlying and Meadow for yearly budget values: 500, 1000, 1500\n"
			"  precondition: initial weed population must be already set\n"
			"  output: number of years how long it took to eradicate weed in exp1_result.txt file\n"
			
			"-> experiment_2\n"
			"  description: test seed suppresion for value 0.0, 0.5, 1.0 for weeds after x years of free grow where x is {10,15,20} years\n"
			"  precondition: initial weed population must be already set\n"
			"  output: in format 'applied after x years;seed_suppresion;current year;weed_cout;' in exp2_result.txt file\n"



            "\n"

;


void print_exp_start(int number){
	std::cout << "<--EXPERIMENT "<<number<<" START-->\n";
}

void print_exp_end(int number){
	std::cout << "<--EXPERIMENT "<<number<<" END-->\n";
}

void experiment_0(WeedSimulator * sim){
	print_exp_start(0);

	int year = 20;
	std::ofstream outputFile("exp0_result.txt");
	SIM.set_cleaning(0,0,bottomtop,1);
	for(int i =0;i<year;i++){
		SIM.iterate(1);
		outputFile << SIM.year_count << ";" << SIM.weed_cout << ";" << "\n";
	}
	SIM.save_grid();
	SIM.set_cleaning(1000,0,topbottom,1);
		for(int i =0;i<20;i++){
		SIM.iterate(1);
		outputFile << SIM.year_count << ";" << SIM.weed_cout << ";" <<SIM.clearing_direction<< "\n";
	}
	SIM.reload_grid();
	SIM.year_count = year;
	SIM.set_cleaning(1000,0,bottomtop,1);
		for(int i =0;i<20;i++){
		SIM.iterate(1);
		outputFile << SIM.year_count << ";" << SIM.weed_cout << ";" <<SIM.clearing_direction<< "\n";
	}
	outputFile.close();
	print_exp_end(0);
}
void experiment_1(WeedSimulator * sim){
	print_exp_start(1);

	std::ofstream outputFile("exp1_result.txt");
	Direction dir;
	//init
	SIM.reset();
	SIM.set_cleaning(0,0,bottomtop,1);
	for(int i =0;i<20;i++){
		SIM.iterate(1);
	}
	SIM.save_grid();
	for(int yearly_budget = 500;yearly_budget <=1500; yearly_budget+=500){
		for(int run = 0;run<10;run++){
			SIM.reload_grid();
			dir = topbottom;
			SIM.set_cleaning(yearly_budget,0,dir,1);
			for(int i =0;i<40;i++){
				SIM.iterate(1);
				if(SIM.weed_cout == 0){
					break;
				}
			}
			outputFile << dir << ";"<< yearly_budget << ";" << SIM.year_count << '\n';

		}
		for(int run = 0;run<10;run++){
			SIM.reset();
			SIM.reload_grid();
			dir = bottomtop;
			SIM.set_cleaning(yearly_budget,0,dir,1);
			for(int i =0;i<40;i++){
				SIM.iterate(1);
				if(SIM.weed_cout == 0){
					break;
				}
			}
			outputFile << dir << ";" << yearly_budget << ";" << SIM.year_count << '\n';
		}
	}
	outputFile.close();
	print_exp_end(1);
}

void experiment_2(WeedSimulator * sim){
	print_exp_start(2);
	std::ofstream outputFile("exp2_result.txt");
	SIM.save_grid();
	int checkpoint_grid_year_count = 0; // saves current year count for the saved grid
	for(int start_iteration_count = 10;start_iteration_count<=20;start_iteration_count+=5){
		SIM.reset();
        SIM.reload_grid();
		SIM.set_cleaning(0,0,bottomtop,1);
		for(int i =0;i<start_iteration_count-checkpoint_grid_year_count;i++){
			SIM.iterate(1);
            outputFile << -1 <<";" << 0 << ";" << checkpoint_grid_year_count + SIM.year_count <<";" << SIM.weed_cout << "\n";
		}
        checkpoint_grid_year_count = start_iteration_count;
		SIM.save_grid();
		for(float seed_suppresion = 0;seed_suppresion <1.1; seed_suppresion+=0.5){
			SIM.reset();
			SIM.reload_grid();
			SIM.set_cleaning(0,seed_suppresion,bottomtop,1);
			for(int i =0;i<20;i++){
				SIM.iterate(1);
				outputFile << checkpoint_grid_year_count <<";" << seed_suppresion << ";" << checkpoint_grid_year_count + SIM.year_count <<";" << SIM.weed_cout << "\n";
			}
		}
	}
	outputFile.close();
	print_exp_end(2);
}