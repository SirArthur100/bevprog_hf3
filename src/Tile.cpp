#include "../include/Tile.hpp"

using namespace genv;
		
Tile::Tile(int xx, int yy, int cx, int cy, std::string nn)
: WidgetBase(xx, yy, cx, cy, nn, SMALL)
{
	
}

void Tile::draw(){
	
	if(this->visible){
		
		if(this->get_active()){

			gout << stamp(this->c, 0, 0, size_x+1, size_y+1, x, y);
		
		} else {
		
			gout << stamp(this->c, 0, 0, size_x, size_y, x, y);
		
		}
	}
}

void Tile::event_handler(genv::event ev){

	if( ev.button == 1 && clicked(ev.pos_x, ev.pos_y) && this->interactive ){
	
		this->set_active(true);
		
		if(!connected_data_storages.empty()){
		
			send_event_to_business_logic(ev);
		
		}
	
	}

}

void Tile::refresh(){
	
	// clear canvas
	c.open(this->canvas_x, this->canvas_y);
	
	// draw display box
	if(this->active || this->highlighted){
	
		//td::cout << "highlight_color: " << highlight_color <<  std::endl;
		//set_color("red");
		c << move_to(0,0) << color(200, 200, 200) << box_to(this->size_x-1, this->size_y-1);
	
	} else {
	
		//set_color(background_color);
		c << move_to(0,0) << color(r, g, b) << box_to(this->size_x-1, this->size_y-1);
	
	}
	
	// if it is active, add green borders
	if(this->active){

		c << genv::color(0,100,0) << genv::move_to(0, 0) << genv::line_to(0+size_x, 0) << genv::line_to(0+size_x, 0+size_y) << genv::line_to(0, 0+size_y) << genv::line_to(0, 0);
	
	}
}

void Tile::send_event_to_business_logic(genv::event ev){

	bl->widget_event_handler(name, ev);

}

void Tile::set_background_color(std::string col){

	this->background_color = col;

}


void Tile::set_highlight_color(std::string col){

	this->highlight_color = col;

}






