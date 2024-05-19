#include "../include/Dropdown.hpp"

Dropdown::Dropdown(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz, std::string storage_ref_name) 
: List(xx, yy, cx, cy, nn, d, b, nel, siz, storage_ref_name)
{

	show_full = false;
	original_y = size_y;
	collapse_list();
	this->set_size_y(display_y);
	//std::cout << "original_y: " << original_y << std::endl;
}

void Dropdown::event_handler(genv::event ev){

	element_clicked = false;
	
	if(show_full){
		
		List::event_handler(ev);
		
		// if an element from the list was selected, we collapse the list
		for (WidgetBase * &w: connected_widgets){

			if(ev.button == 1 && w->clicked(ev.pos_x, ev.pos_y)){
			
				element_clicked = true;
				this->set_active(false);
			
			}

		}

	} else if (ev.button == 1) {
	
		initialize_list();
		show_full = true;
		this->set_size_y(original_y);
		this->set_active(true);
		refresh();
	}
	
	if(element_clicked){
	
		collapse_list();
		
		show_full = false;
		
		this->set_size_y(display_y);
	}
	
}


void Dropdown::collapse_list(){

	// we first remove each Display from the list
	clear_list();
	
	// we add back one element to the list
	Display * list_elem = new Display(display_x, display_y, this->canvas_x, this->canvas_y, current_selection);
	list_elem->default_val(current_selection);
	list_elem->set_x(this->x+1);
	list_elem->set_y(this->y + 2);
	list_elem->add_business_logic(this->bl);
	list_elem->add_data_storage(this->connected_data_storages[0]);
	list_elem->set_fontsize(fontsiz);
	list_elem->set_background_color("light-gray");
	list_elem->set_highlight_color("red");
	list_elem->set_font_color("black");

	connected_widgets.push_back( list_elem );
	
	refresh();

}

void Dropdown::deactivate(){

	// we wrap the deactivate func of the List

	List::deactivate();
	
	collapse_list();
		
	show_full = false;
		
	this->set_size_y(display_y);

}




