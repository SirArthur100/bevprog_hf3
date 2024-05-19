#include "../lib/graphics.hpp"

// graphical elements
#include "../include/Display.hpp"
#include "../include/Arrow.hpp"
#include "../include/WidgetContainer.hpp"
#include "../include/List.hpp"
#include "../include/Dropdown.hpp"
#include "../include/Menu.hpp"
#include "../include/Tiles.hpp"
#include "../include/Dummy_BL.hpp"
#include "../include/tic-tac-toe-BL.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace genv;

/*

Schematic for the program backend:  CONTROLLER - MODEL - VIEW



                                     ┏━━━━━━━━┓
                             ┏━━━━┫ widget  ┣━━━┓ ---------------------------- registering widgets                               
                             ┃       ┃             ┃     ┃                             \|
    ╔═════════╗     ┃      ┣━━━━━━━━┫     ┃      refreshing UI      ┏━━━━━━━━━━━┓
    ║   UI    ║━━━━╋━━━━┫ widget  ┃     ┣━━━━━━━━━<━━━━━━━<━━━━━┫ data store ┃
    ║         ║     ┃      ┃              ┣━━━╋                                     ┨                  ┃ MODEL
    ╚═════════╝     ┃      ┣━━━━━━━━┫      ┃                                     ┗━━━━┳━━━━━━┛
                             ┗━━━━┫ widget  ┣━━━━┻━━━┓                                       ┃    
                                      ┗━━━━━━━━┛             ┃         ┏━━━━━━━━┓              ┃    
                           VIEW              ┃         ┃business ┃              ┃ manipulate data
                                                                    ┗━━>━━┫ logic   ┣━━━━>━━━━┛
                                                                              ┃              ┃
                                                                              ┗━━━━━━━━┛ 
                                                     CONTROLLER


       EVENT FLOW           

               contains        contains 
   ┌────────────┐      ┌──────────┐   ┌─────────┐
   │ Widget 1.  │      │ Widg 2.  │   │  Widg 3.│
   │            ├──────┤          ├───┤         │......
   └─────┬──────┘      └─────┬────┘   └────┬────┘
         │ event             │event        │
         │      ┌────────────┴┐            │ event
         │      │ business    │            │
         └───>──┤     logic   ├───────<────┘...........
                └───────┬─────┘
                        │
                        │
                ┌───────┴──────┐
                │  data        │
                │      store   │
                └──────────────┘



       Inheritance           
                
               ┌───╴Display
               │
 ┌─────────┐   │
 │ Widget  ┼───┼───╴Tile           ----- Tiles
 │   Base  │   │                  |
 └─────────┘   │                 |
               └───╴Widget Container────List───Dropdown---Menu



  ┌──────────┐
  │ Business ┼───┬─────╴TicTac_BL
  │  Logic   │   │
  └──────────┘   │
                 └─────╴PlainList
                 
                 
                 
         DATA STORAGE
         
MAP:

key ----> value
key ----> value
key ----> value
      .
      .
      .
      .
key ----> value

Each widget component is registered on in the map, plus the model data.


The business logic is responsible for the file outputs.

I TRIED TO ELIMINATE MEMORY LEAKAGE FOR MY OWN CLASSES BY IMPLEMENTING THE PROPER DESTRUCTORS, 
BUT VALGRIND TELLS ME THAT THERE IS SOMETHING WEIRD
GOING ON WITH THE FONTS AND CONVAS DRAWING, ITS NOT A BIG PROBLEM, IS JUST NOTICED IT.

*/

const int XX = 800;
const int YY = 600;

// this will contain the main UI components
std::vector<WidgetContainer *> UI_components;

// utility cleaner
void clear_screen(){

	gout << color(0, 0, 0) << move_to(0, 0) << box_to(XX-1, YY-1);

}

void UI_event_handler(event ev){

/*
	
The MAIN PRINCIPLE OF THE UI is: there can be ONLY 1 active WidgetContainer on the level of the UI
(sub WidgetContainers does not count)

THERE ARE THREE POSSIBLE STATES FOR THE WIDGETS:

DEACTIVATED <--------------> ACTIVATED <--------------> ACTION

This function is responsible for changing between these states.

*/
	
	for(WidgetContainer * &w: UI_components){
	
		// WE DEACTIVATE AN ACTIVE WIDGET
		if ( w->get_active() && ev.button == 1 && !w->clicked(ev.pos_x, ev.pos_y) ){
		
			w->deactivate();
			break;
		}
	}
	
	for(WidgetContainer * &w: UI_components){
	
		// WE ACTIVATE OR REACTIVATE A WIDGET
		// this is done by detecting the clicks
		if(w->clicked(ev.pos_x, ev.pos_y) && w->visible && w->interactive){
		
			w->event_handler(ev);
			break;
		
		} else {
		
			// WE HANDLE ACTIVE WIDGETS (ACTIONS)
			if(w->get_active()){

				w->event_handler(ev);
				break;
			
			}
		}
	}
	
	//-------------------------------------------
	// here we could handle any extra requests...
	//-------------------------------------------
	
	
	// lastly, we draw our widgets in reverse order, so the to element gets drawn lastly
	for(size_t i = UI_components.size() - 1 ; UI_components.size() > i ; --i){
	
		UI_components[i]->draw();
	
	}

}

void draw_background(){

	gout << color(100, 100, 100) << move_to(0, 0) << box_to(XX-1, 52);

}

void graphical_runtime(){

	gout.open(XX, YY);
	event ev;
	gin.timer(100);
	
	// to create the game, we create the DataStorage and the BL
	// we initiate a variable (quit) so we can close the window from inside 
	// the game
	bool quit = false;
	DataStorage * ds = new DataStorage();
	TicTacBL * bl = new TicTacBL("menu_BL", ds, &UI_components, &quit);
    
    	while (gin >> ev && !quit) {
    		 
    		 clear_screen();
    		 draw_background();
		 UI_event_handler(ev);
    		 gout<<refresh;
    	}
    	
    	// we delete the DataStorage and the BL
    	ds->purge();
    	delete ds;
    	
    	delete bl;

}

int main(){

	graphical_runtime();

}
