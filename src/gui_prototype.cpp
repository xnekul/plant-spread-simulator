#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "weedsim.hpp"
#include <string>
#include "experiments.hpp"




class Example : public olc::PixelGameEngine
{
	private:
		WeedSimulator * sim;
		CellGrid2D * grid;
		int rows;
		int cols;
		int iterations = 1;
		bool bg_color = true;
	public:
	Example(int rows, int cols)
	{
		sAppName = "Example";
		this->rows = rows;
		this->cols = cols;
	}
	~Example(){
		delete sim;
	}


public:
	bool OnUserCreate() override
	{
		
		sim = new WeedSimulator(rows,cols);
		if(sim == NULL){
			exit(-1);
		}


		grid = (*sim).get_grid();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		display_grid();




		//edit cells
		if (GetMouse(0).bHeld){
			(*sim).init_weed(	GetMouseX()/static_cast<float>(ScreenWidth())*cols,
							GetMouseY()/static_cast<float>(ScreenHeight())*rows);
    	}
		else if(GetMouse(1).bHeld){
			(*sim).remove_weed(	GetMouseX()/static_cast<float>(ScreenWidth())*cols,
							GetMouseY()/static_cast<float>(ScreenHeight())*rows);
		}

		if(GetKey(olc::Key::H).bPressed){
			std::cout <<
			"<--HELP-->\n"
			"H - help\n"
			"ENTER - start simulation\n"
			"ESC - reset simulation\n" 
			"I - enter number of iterations into CLI\n"
			"P - print info for current state\n"
			"C - set cleaning\n"
			"B - change background color colored/white\n"
			"SPACE - start experiment\n"
			"CTRL+C - save grid\n"
			"CTRL+V - reload grid\n"
			;
		}
		if(GetKey(olc::Key::P).bPressed){
			(*sim).info();
		}
		if(GetKey(olc::Key::C).bPressed){

			std::cout << "<--SET CLEANING-->\n";
			std::cout << "Use custom values? Y/N\n";
			std::cout << "Default values are: yearly_budget = 1000, seed_suppresion = (0.0F), clearing_direction = topbottom, cell_cost = 1\n";

			char custom_values;
			std::cin >> custom_values;
			if(custom_values == 'Y'){
				
				std::cout << "Set yearly budget (int):\n";
				int yearly_budget;
				std::cin >> yearly_budget;

				float seed_suppresion = (0.0F);
				std::cout << "Set seed_suppresion (0...1 float):\n";
				std::cin >> seed_suppresion;

				unsigned int clearing_direction = topbottom; 
				std::cout << "Set yearly clearing_direction:\n";
				std::cout << "(Options: topbottom-0, bottomtop-1, leftright-2, rightleft-3, tocenter-4\n";
				std::cin >> clearing_direction;

				int cell_cost = 1;
				std::cout << "Set cell_cost (int):\n";
				std::cin >> cell_cost;

				(*sim).set_cleaning(yearly_budget,seed_suppresion,static_cast<Direction>(clearing_direction),cell_cost);
			}
			else{
				std::cout << "Default values used: yearly_budget = 1000, seed_suppresion = (0.0F), clearing_direction = topbottom, cell_cost = 1\n";
				(*sim).set_cleaning();
			}
		}
		if(GetKey(olc::Key::ESCAPE).bPressed){
			std::cout << "<--RESETED-->\n";
			(*sim).reset();
		}
		if(GetKey(olc::Key::I).bPressed){
			std::cout << "<--ITERATIONS-->\n";
			std::cout << "enter number of iterations:\n";
			std::cin >> iterations;
			std::cout << "number of iterations is set to: " << iterations << '\n';
		}
		if(GetKey(olc::Key::B).bPressed){
			bg_color=!bg_color;
		}

		if(GetKey(olc::Key::CTRL).bHeld){
			if(GetKey(olc::Key::C).bPressed){
				std::cout << "<--SAVED-->\n";
				(*sim).save_grid();
			}
			else if(GetKey(olc::Key::V).bPressed){
				std::cout << "<--RELOADED-->\n";
				(*sim).reload_grid();
			}
			
		}

		if(GetKey(olc::Key::SPACE).bPressed){

			int experiment=0;
			std::cout << "<--SELECT EXPERIMENT NUMBER-->>\n";
			std::cout <<
			EXPERIMENTS_DESCRIPTION;
			std::cin >> experiment;
			std::cout << "<--STARTING EXPERIMENT-->>\n";
			switch (experiment)
			{
			case 0:
				experiment_0(sim);
				break;
			case 1:
				experiment_1(sim);
				break;
			case 2:
				experiment_2(sim);
				break;

			default:
				std::cout << "<--NO EXPERIMENT SELECTED-->>\n";
				break;
			}
		std::cout << "<--EXPERIMENT FINISHED-->\n";
		}
		if(GetKey(olc::Key::ENTER).bPressed){
			std::cout << "will be run for :"<< iterations << std::endl;
			(*sim).iterate(iterations);
			std::cout << "end" << std::endl;
			iterations = 1;
		}
		return true;
	}

	private:
		void display_grid(){
			//sim.print();
			Clear(olc::WHITE);

			for(int y = 0;y<rows;y++){
				for(int x = 0;x<cols;x++){
					if((*grid)[y][x]==weed){
		  				Draw(	x,y,
								//olc::Pixel(rand() % 255, rand() % 255, rand()% 255)
								olc::Pixel(0,0,0)
						);
						}
						else if(bg_color){
								Draw(	x,y,
								olc::Pixel(0,0,static_cast<float>(y)/static_cast<float>(rows)*125+125));

						}
					}			

				}
			}


	};

int main()
{
	int pixel_size = 100;
	int rows = 120;
	int cols = 120;


	Example demo(cols,rows);
	//Width and height = how many pixels fit into screen
	// pixel width and height = size of the pixel
	if (demo.Construct(cols, rows, pixel_size, pixel_size))
		demo.Start();

	return 0;
}