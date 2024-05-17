#include "../lib/graphics.hpp"

// graphical elements
#include "../include/Display.hpp"
#include "../include/Arrow.hpp"
#include "../include/WidgetContainer.hpp"
#include "../include/List.hpp"
#include "../include/Dropdown.hpp"
#include "../include/Menu.hpp"

// controllers
#include "../include/NumberWithArrows.hpp"
#include "../include/PlainList.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace genv;

/*

Schematic for the program backend:  CONTROLLER - MODEL - VIEW



                                     ┏━━━━━━━━┓
                             ┏━━━━┫ widget  ┣━━━┓                                 
                             ┃       ┃             ┃     ┃                                 
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

               containes        containes 
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
 │ Widget  ┼───┼───╴Arrow
 │   Base  │   │
 └─────────┘   │
               └───╴Widget Container────List───Dropdown



  ┌──────────┐
  │ Business ┼───┬─────╴NumberWithArrows
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



The implementation is kind of messy, however, with the already created widget it is like lego
building to create new functionalities.

The business logic is responsible for the file outputs.

The WidgetContainers are movable.


KNOWN ISSUE IS WHEN MOVING WIDGET NEXT TO SCREEN THEN UNEXPECTED/WRONG BEHAVIOUR IS PRESENT!!!!!!!!!!

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
		
			std::cout << "DEACTIVATING: " << w->name << std::endl;
			w->deactivate();
			break;
		}
	}
	
	for(WidgetContainer * &w: UI_components){
	
		// WE ACTIVATE OR REACTIVATE A WIDGET
		// this is done by detecting the clicks
		if(ev.button == 1 && w->clicked(ev.pos_x, ev.pos_y) && w->visible && w->interactive){
		
			std::cout << "ACTIVATING: " << w->name << std::endl;
			w->event_handler(ev);
			//break;
		
		} else {
		
			// WE HANDLE ACTIVE WIDGETS (ACTIONS)
			if(w->get_active()){
				//std::cout << "ADDING EVENT TO: " << w->name << std::endl;
				w->event_handler(ev);
			//	break;
			
			}
		}
	}
	
	//-------------------------------------------
	// here we could handle any extra requests...
	//-------------------------------------------
	
	
	
	// lastly, we draw our widgets
	for(WidgetContainer * &w: UI_components){
	
		w->draw();
	
	}

}

Menu * dropdown_list(std::string name_of_widget, SIZE s, int x, int y){

	// initiate new data storage with a display value of 0 ------------------------------
	DataStorage * ds = new DataStorage();
	ds->post("list", "Small game (15x15)");
	ds->post("list", "Large game (30x30)");
	ds->post("list", "Quit game");
	ds->post("selected", "");
	
	// initiate business logic ----------------------------------------------------------
	PlainList * list_bl = new PlainList(name_of_widget);
	list_bl->add_data_storage(ds);

	// initiate list --------------------------------------------------------------------
	Menu* l_1 = new Menu(50, 100, 200, 200, name_of_widget, ds, list_bl, 3, s);
	l_1->move_widget(x, y);
	l_1->set_moveable(false);
	
	
	// add the main UI container widgets to the storage ---------------------------------
	ds->add_widget(l_1);

	return l_1;

}

void draw_background(){

	gout << color(100, 100, 100) << move_to(0, 0) << box_to(XX-1, 52);

}


int main(){

	gout.open(XX, YY);
	event ev;
	gin.timer(100);
	
	gout.load_font("../resources/LiberationSans-Regular.ttf", 20);

	Menu * menu = dropdown_list("menu", LARGE, 0, 0);

    	UI_components.push_back(menu);
    
    	while (gin >> ev) {
    		
    		 clear_screen();
    		 draw_background();
		 UI_event_handler(ev);
    		 gout<<refresh;
    	}
}
