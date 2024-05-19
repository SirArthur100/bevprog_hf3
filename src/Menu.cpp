#include "../include/Menu.hpp"

/*

The Menu class is a modified version of the Dropdown list.

*/

Menu::Menu(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz, std::string storage_ref_name) 
: Dropdown(xx, yy, cx, cy, nn, d, b, nel, siz, storage_ref_name)
{

	current_selection = "Menu";
	collapse_list();

}

void Menu::event_handler(genv::event ev){

	
	Dropdown::event_handler(ev);
	
	if(element_clicked){
	
		current_selection = "Menu";
		collapse_list();
	
	}
	
}
