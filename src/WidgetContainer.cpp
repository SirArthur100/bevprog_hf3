#include "../include/WidgetContainer.hpp"

#include <iostream>

WidgetContainer::WidgetContainer(int xx, int yy, int cx, int cy, std::string nn, SIZE siz) : WidgetBase(xx, yy, cx, cy, nn, siz){

	this->set_dragged(false);
	this->set_moveable(false);
	this->set_draggable(true);
	this->distance_x = 0;
	this->distance_y = 0;
	this->set_color("light-gray");
	refresh();
}

void WidgetContainer::event_handler(genv::event ev){

/*
	The WidgetContainer can be dragged around the UI. If it is clicked_down, first it
	checks whether it should give the event forward or not. If the click falls on a content,
	then this WidgetContainer and its content state gets active.
	
	If the click does not fall on a contained widget, then this WidgetContainer with its contents
	can be dragged around the UI.
	
	A click within the WidgetContainer activates it, and leaves it in that state until another
	WidgetContainer gets clicked on the UI level.
	
	THIS FUNCTION IS ONLY CALLED IF THIS WidgetContainer AND/OR ONE OF ITS COMPONENT IS ACTIVE (eg. by a click).
*/

	std::cout << "is_moveable: " << get_moveable() << std::endl;
	if(get_dragged() && ev.type == genv::ev_mouse){
		
		std::cout << "dragging " << this->name << std::endl;
		// we need to adjust the current position if the WidgetContainer is dragged
		move_widget(ev.pos_x, ev.pos_y);
		
		// finally release dragging
		if(ev.button == -1){
		
			set_dragged(false);
			
			// we forward the event to the contained widgets
			forward_event(ev);

			std::cout << "dragging FINISHED..." << std::endl;
		
		}
	
	// we check whether there is a click on one of the components
	}else if(ev.button == 1 && clicked(ev.pos_x, ev.pos_y)){
		
		std::cout << this->name << " clicked and set active" << std::endl;
		
		bool is_content_selected = false;
		
		this->set_active(true);

		for(WidgetBase * &w: connected_widgets){
		
			if( w->clicked(ev.pos_x, ev.pos_y) ){
				
				std::cout << "content clicked..." << std::endl;
				
				// if a content object is selected, we set the flag so we wont drag this WidgetContainer
				// additionally, we give the event to the clicked object
				is_content_selected = true;
				w->event_handler(ev);

			} else {
				// if its not a selected subcomponent, we deactivate it
				w->set_active(false);
			
			}
		}
		
		// if no content is selected, then we make dragging available
		if(!is_content_selected && get_moveable()){
		
			set_dragged(true);
			distance_x = ev.pos_x - get_x();
			distance_y = ev.pos_y - get_y();
		
		}
	
	} else if (ev.type == genv::ev_mouse){
	
		// because subcomponents might get dragged independently, we must forward the event
		// is this is a mouse event
		forward_event(ev);
	
	
	}
	
	send_event_to_business_logic(ev);
	
}

void WidgetContainer::position_contained_widget(std::string n, int new_x, int new_y){

	for (WidgetBase * &w: connected_widgets){

		if(w->name.compare(n) == 0){
		
			w->set_x(new_x);
			w->set_y(new_y);
			break;
		}

	}

}

void WidgetContainer::refresh(){

	// clear canvas
	c.open(canvas_x, canvas_y);

	// we redraw this container on the its canvas
	
	//std::cout << r << " " << g << " " << b << std::endl;
	
	c << genv::color(r,g,b) << genv::move_to(0, 0) << genv::box_to(size_x, size_y);
	
	// if it is active, add green borders
	if(this->active){

		c << genv::color(0,255,0) << genv::move_to(0, 0) << genv::line_to(0+size_x, 0) << genv::line_to(0+size_x, 0+size_y) << genv::line_to(0, 0+size_y) << genv::line_to(0, 0);
	
	}
	
	// we redraw the contained widgets on their own canvas
	for(WidgetBase * &w: connected_widgets){
	
		std::cout << "refreshed: " << w->name << std::endl;
		w->refresh();
	
	}
	
	//std::cout << "refreshed: " << this->name << std::endl;

}

void WidgetContainer::draw(){

	// we stamp this container on the main UI
	genv::gout << genv::stamp(c,0,0, size_x+1, size_y+1, x, y);

	// we stamp the contained widgets on the main UI
	for(WidgetBase * &w: connected_widgets){
	
		w->draw();
	
	}

}

void WidgetContainer::deactivate(){

	// when we deactivate a WidgetContainer, we must also deactivate its
	// content Widgets

	this->set_active(false);
	this->set_highlighted(false);

	for(WidgetBase * &w: connected_widgets){
	
		w->set_active(false);
		w->set_highlighted(false);
	
	}

}

void WidgetContainer::set_moveable(bool val){

	this->is_moveable = val;

}

bool WidgetContainer::get_moveable(){

	return this->is_moveable;

}

void WidgetContainer::set_dragged(bool val){

	this->is_dragged = val;

}

bool WidgetContainer::get_dragged(){

	return this->is_dragged;

}

void WidgetContainer::forward_event(genv::event ev){

	for(WidgetBase * &w: connected_widgets){

		w->event_handler(ev);
		
	}

}

void WidgetContainer::move_widget(int nx, int ny){

	
	// we need to realtively move any contained widget as well
	for(WidgetBase * &w: connected_widgets){

		w->set_x( nx - (distance_x) + ( w->get_x() - get_x() ) );
		w->set_y( ny - (distance_y) + ( w->get_y() - get_y() ) );
		
	}
	
	// lastly , reposition this widget
	set_x( nx - (distance_x) );
	set_y( ny - (distance_y) );
	
	//std::cout  << "set_x( nx - (distance_x) ): " << get_x() << std::endl;
	//std::cout  << "set_y( ny - (distance_y) ): " << get_y() << std::endl;

}

void WidgetContainer::send_event_to_business_logic(genv::event ev){

	bl->widget_event_handler(name, ev);

}




