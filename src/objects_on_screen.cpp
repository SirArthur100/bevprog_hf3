#include "../lib/graphics.hpp"

// graphical elements
#include "../include/Display.hpp"
#include "../include/Arrow.hpp"
#include "../include/WidgetContainer.hpp"
#include "../include/List.hpp"
#include "../include/Dropdown.hpp"

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

WidgetContainer *counter(std::string name_of_widget, SIZE s, int x, int y, int mi, int ma){

	// initiate new data storage with a display value of 0 ------------------------------
	DataStorage * ds = new DataStorage();
	ds->post("display", "0");
	
	// initiate business logic ----------------------------------------------------------
	NumberWithArrows * n_arrow_bl = new NumberWithArrows(name_of_widget);
	n_arrow_bl->add_data_storage(ds);
	n_arrow_bl->set_min_and_max(mi, ma);

    	// initiate widget container --------------------------------------------------------
	WidgetContainer* w_1 = new WidgetContainer(70, 40, 200, 200, name_of_widget);
	w_1->set_moveable(true);
	
	// initiate display -----------------------------------------------------------------
	Display* d1 = new Display(38, 38, 100, 100, "display");
	d1->set_x(1);
	d1->set_y(1);
	d1->set_fontsize(20);
	d1->set_color("red");
	
	// attach data storage to display
	d1->add_data_storage(ds);
	d1->add_business_logic(n_arrow_bl);
	
	// initiate up and down arrows ------------------------------------------------------
	Arrow * arr_up = new Arrow(15, 15, 100, 100, "arrow_up");
	arr_up->set_x(50);
	arr_up->set_y(4);
	arr_up->set_color("red");
	arr_up->set_direction(up);
	arr_up->add_data_storage(ds);
	arr_up->add_business_logic(n_arrow_bl);

	Arrow * arr_down = new Arrow(15, 15, 100, 100, "arrow_down");
	arr_down->set_x(50);
	arr_down->set_y(21);
	arr_down->set_color("red");
	arr_down->set_direction(down);
	arr_down->add_data_storage(ds);
	arr_down->add_business_logic(n_arrow_bl);
	
	// mount the subwidgets to the main UI widget ---------------------------------------
	w_1->add_widget(d1);
	w_1->add_widget(arr_up);
	w_1->add_widget(arr_down);
	
	// mount business logic on WidgetContainer ------------------------------------------
	w_1->add_business_logic(n_arrow_bl);
	
	// add the main UI container widgets to the storage ---------------------------------
	ds->add_widget(w_1);
	
	w_1->move_widget(x, y);
	
	return w_1;
}

List * simple_list(std::string name_of_widget, SIZE s, int x, int y){

	// initiate new data storage with a display value of 0 ------------------------------
	DataStorage * ds = new DataStorage();
	ds->post("list", "0000000000");
	ds->post("list", "1101111111");
	ds->post("list", "2222222222");
	ds->post("list", "3333333333");
	ds->post("list", "4444444444");
	ds->post("list", "5555555555");
	ds->post("list", "6666666666");
	ds->post("list", "7777777777");
	ds->post("list", "8888888888");
	ds->post("list", "9999999999");
	ds->post("list", "1010101010");
	ds->post("list", "1111111111");
	ds->post("list", "1212121212");
	ds->post("list", "1313131313");
	ds->post("list", "1414141414");
	ds->post("list", "1515151515");
	ds->post("list", "1616161616");
	ds->post("selected", "");
	
	// initiate business logic ----------------------------------------------------------
	PlainList * list_bl = new PlainList(name_of_widget);
	list_bl->add_data_storage(ds);

	// initiate list --------------------------------------------------------------------
	List* l_1 = new List(50, 100, 200, 200, name_of_widget, ds, list_bl, 5, s);
	l_1->move_widget(x, y);
	l_1->set_moveable(true);
	
	
	// add the main UI container widgets to the storage ---------------------------------
	ds->add_widget(l_1);

	return l_1;

}

Dropdown * dropdown_list(std::string name_of_widget, SIZE s, int x, int y){

	// initiate new data storage with a display value of 0 ------------------------------
	DataStorage * ds = new DataStorage();
	ds->post("list", "0000000000");
	ds->post("list", "1111101111");
	ds->post("list", "2222222222");
	ds->post("list", "3333333333");
	ds->post("list", "4444444444");
	ds->post("list", "5555555555");
	ds->post("list", "6666666666");
	ds->post("list", "7777777777");
	ds->post("list", "8888888888");
	ds->post("list", "9999999999");
	ds->post("list", "1010101010");
	ds->post("list", "1111111111");
	ds->post("list", "1212121212");
	ds->post("list", "1313131313");
	ds->post("list", "1414141414");
	ds->post("list", "1515151515");
	ds->post("list", "1616161616");
	ds->post("selected", "");
	
	// initiate business logic ----------------------------------------------------------
	PlainList * list_bl = new PlainList(name_of_widget);
	list_bl->add_data_storage(ds);

	// initiate list --------------------------------------------------------------------
	Dropdown* l_1 = new Dropdown(50, 100, 200, 200, name_of_widget, ds, list_bl, 5, s);
	l_1->move_widget(x, y);
	l_1->set_moveable(true);
	
	
	// add the main UI container widgets to the storage ---------------------------------
	ds->add_widget(l_1);

	return l_1;

}

void draw_annotations(){

	gout<<move_to(20,20) << color(255,255,255) << text("small simple list:");
	gout<<move_to(200,20) << color(255,255,255) << text("large simple list:");
	gout<<move_to(380,20) << color(255,255,255) << text("small dropdown list:");
	gout<<move_to(600,20) << color(255,255,255) << text("large dropdown list:");
	gout<<move_to(20,300) << color(255,255,255) << text("small_counter:");

}


int main(){

	gout.open(XX, YY);
	event ev;
	gin.timer(100);
	
	gout.load_font("../resources/LiberationSans-Regular.ttf", 20);
	
	WidgetContainer * small_counter = counter("small_counter", SMALL, 50, 400, -100, 100);
	
	List * small_simple_list = simple_list("small_simple_list", SMALL, 70, 50);
	List * large_simple_list = simple_list("large_simple_list", LARGE, 230, 50);
	Dropdown * small_dropdown_list = dropdown_list("small_dropdown_list", SMALL, 430, 50);
	Dropdown * large_dropdown_list = dropdown_list("large_dropdown_list", LARGE, 650, 50);
    
    	UI_components.push_back(small_simple_list);
    	UI_components.push_back(large_simple_list);
    	UI_components.push_back(small_dropdown_list);
    	UI_components.push_back(large_dropdown_list);
    	UI_components.push_back(small_counter);
    
    	while (gin >> ev) {
    		
    		 clear_screen();
    		 draw_annotations();
		 UI_event_handler(ev);
    		 gout<<refresh;
    	}
}
