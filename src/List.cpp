#include "../include/List.hpp"

List::List(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz) : WidgetContainer(xx,yy,cx,cy,nn, siz){

	elements_to_show = nel;
	this->set_color("dark-gray");
	current_selection = "";

	// set up sizes according to user input
	if(siz == SMALL){
	
		this->set_size_x(100);
		this->set_size_y(100);
		this->fontsiz = 10;
	
	} else if (siz == LARGE){
	
		this->set_size_x(160);
		this->set_size_y(160);
		this->fontsiz = 20;
	}
	
	this->display_x = get_size_x() - 2;
	this->display_y = get_size_y() / elements_to_show - 2;


	add_data_storage(d);
	add_business_logic(b);

	// we get the list elements from the data storage
	list_elements = connected_data_storages[0]->get_whole("list");
	
	idx_first = 0;
	idx_last = std::min(list_elements.size(), elements_to_show);
	
	// generate the Displays (list elements)
	initialize_list();
	
	refresh();

}

void List::initialize_list(){
/*
	This function will select the first elements from the attached database, and create the Displays for them.

*/
	// first remove everything from the list
	clear_list();

	for(int i=idx_first; i < idx_last ; i++){
		
		Display * list_elem = new Display(display_x, display_y, this->canvas_x, this->canvas_y, list_elements[i] );
		list_elem->set_x(this->x+1);
		list_elem->set_y(this->y + i * display_y + i + 2);
		list_elem->add_business_logic(this->bl);
		list_elem->add_data_storage(this->connected_data_storages[0]);
		list_elem->set_fontsize(fontsiz);
		list_elem->set_background_color("light-gray");
		list_elem->set_highlight_color("red");
		list_elem->set_font_color("black");

		connected_widgets.push_back( list_elem );

	}

}



void List::event_handler(genv::event ev){
/*
	Wrapper for the WidgetContainer event handler function, because
	we will need to handle mouse wheeling and up/down arrow events
	
	This handler will be responsible for the different behaviour for different list types.
*/	
	WidgetContainer::event_handler(ev);
	
	// our widget needs to remember the already selected element, if there is any
	for(WidgetBase * &w: connected_widgets){
			
		if(w->get_active()){
		
			current_selection = w->name;
			break;
		} 
	} 
	
	// we need to be able to navigate in the list with the up/down/pageup/pagedwn buttons
	// this navigation only becomes available once there is an already selected element
	
	if (ev.button == genv::btn_wheeldown){
		
		restructuring("down");
	
	} else if ( ev.button == genv::btn_wheelup ){
	
		restructuring("up");
	
	}
	
	
	for(WidgetBase * &w: connected_widgets){
		
		w->set_highlighted(false);
		
		if(w->name.compare(current_selection) == 0){
		
			w->set_highlighted(true);
		
		}
		w->refresh();
	} 

}

void List::restructuring(std::string s){
/*
	Function to handle list navigation events.
*/	
	
	if(s.compare("down") == 0){
	
		if( idx_last < list_elements.size() ){
		
			deselect_all();
			
			std::cout<<"DOWN"<<std::endl;
		
			idx_last += 1;
			idx_first += 1;
			
			int counter = idx_first;
			
			for(WidgetBase * &w: connected_widgets){
			
				w->rename(list_elements[counter]);
				counter++;
			}
			
		}
	
	} else if( s.compare("up") == 0 ) {
	
		std::cout<<"UP"<<std::endl;
	
		if( idx_first > 0 && idx_last > elements_to_show ){
		
			deselect_all();
		
			idx_last -= 1;
			idx_first -= 1;
			
			int counter = idx_first;
			
			for(WidgetBase * &w: connected_widgets){
			
				
				w->rename(list_elements[counter]);
				counter++;
			}
			
		}
	
	}

}

void List::deselect_all(){

	for(WidgetBase * &w: connected_widgets){
						
		w->set_active(false);

	}


}


void List::clear_list(){

	for(WidgetBase * &w: connected_widgets){
			
		delete w;
	}
	
	connected_widgets.clear();

	idx_first = 0;
	idx_last = std::min(list_elements.size(), elements_to_show);

}



