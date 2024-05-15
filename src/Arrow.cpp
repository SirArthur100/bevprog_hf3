#include "../include/Arrow.hpp"

using namespace genv;

Arrow::Arrow(int xx, int yy, int cx, int cy, std::string nn) : WidgetBase(xx, yy, cx, cy, nn){

	set_direction(up);

}

void Arrow::draw(){

	if(this->get_active()){

		gout << stamp(this->c, 0, 0, size_x+1, size_y+1, x, y);
	
	} else {
	
		gout << stamp(this->c, 0, 0, size_x, size_y, x, y);
	
	}

}

void Arrow::refresh(){

	// clear canvas
	c.open(this->canvas_x, this->canvas_y);
	
	// draw display box
	c << move_to(0,0) << color(r, g, b) << box_to(this->size_x, this->size_y);
	
	//draw arrow
	switch(direction){
	
		case up:
			c << color(1,1,1) << move_to(0, size_y) << line_to(size_x/2, 0) << line_to(size_x, size_y);
			c << move_to(1, size_y) << line_to(size_x/2, 1) << line_to(size_x-1, size_y);
			c << move_to(2, size_y) << line_to(size_x/2, 2) << line_to(size_x-2, size_y);
			c << move_to(3, size_y) << line_to(size_x/2, 3) << line_to(size_x-3, size_y);
			break;
			
		case down:
			c << color(1,1,1) << move_to(0, 0) << line_to(size_x/2, size_y) << line_to(size_x, 0);
			c << color(1,1,1) << move_to(1, 0) << line_to(size_x/2, size_y-1) << line_to(size_x-1, 0);
			c << color(1,1,1) << move_to(2, 0) << line_to(size_x/2, size_y-2) << line_to(size_x-2, 0);
			c << color(1,1,1) << move_to(3, 0) << line_to(size_x/2, size_y-3) << line_to(size_x-3, 0);
			break;
	
	
	}
	
	if(this->active){

		c << color(0,255,0) << move_to(0, 0) << line_to(0+size_x, 0) << line_to(0+size_x, 0+size_y) << line_to(0, 0+size_y) << line_to(0, 0);
	
	}
}

void Arrow::event_handler(event ev){




	if( ev.button == 1 && clicked(ev.pos_x, ev.pos_y) ){
	
		std::cout << "ARROW EVENT HANDLER WAS CALLED!!!!" << std::endl;
		
		this->set_active(true);
		
		send_event_to_business_logic(ev);
	}
	
	

}

void Arrow::set_direction(ARROW_DIRECTION new_dir){

	direction = new_dir;
	refresh();
}

ARROW_DIRECTION Arrow::get_direction(){

	return direction;
}

void Arrow::send_event_to_business_logic(genv::event ev){

	bl->widget_event_handler(name, ev);

}






