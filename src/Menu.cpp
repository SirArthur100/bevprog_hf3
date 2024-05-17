#include "../include/Menu.hpp"

Menu::Menu(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz) 
: Dropdown(xx, yy, cx, cy, nn, d, b, nel, siz)
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
