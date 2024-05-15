#include "../include/Display.hpp"

using namespace genv;
		
Display::Display(int xx, int yy, int cx, int cy, std::string nn, SIZE siz)
: WidgetBase(xx, yy, cx, cy, nn, siz)
{

	this->set_fontsize(10);
	set_text(nn);

}

void Display::set_text(std::string ss){

	this->text_contained = ss;
}

std::string Display::get_text(){

	return this->text_contained;
}

void Display::draw(){
	
	if(this->visible){
		
		if(this->get_active()){

			gout << stamp(this->c, 0, 0, size_x+1, size_y+1, x, y);
		
		} else {
		
			gout << stamp(this->c, 0, 0, size_x, size_y, x, y);
		
		}
	}
}

void Display::event_handler(genv::event ev){

	if( ev.button == 1 && clicked(ev.pos_x, ev.pos_y) && this->interactive ){
	
		this->set_active(true);
		
		if(!connected_data_storages.empty()){
		
			send_event_to_business_logic(ev);
		
		}
	
	}

}

void Display::refresh(){
	
	if( !connected_data_storages.empty() ){
	
		//std::cout << "setting text from storage... " <<  std::endl;
		set_text(connected_data_storages[0]->get(name, 0));
	}
	
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
	// draw text
	// for now, text is only placed left aligned
	int text_start_x = 0;
	int text_start_y = (size_y / 2) - ( ( c.cdescent() + c.cascent() ) / 2 );
	
	//set_color(font_color);
	c << move_to( 0, text_start_y ) << color(0, 0, 0) << text(this->text_contained);
	
	// if it is active, add green borders
	if(this->active){

		c << genv::color(0,100,0) << genv::move_to(0, 0) << genv::line_to(0+size_x, 0) << genv::line_to(0+size_x, 0+size_y) << genv::line_to(0, 0+size_y) << genv::line_to(0, 0);
	
	}
}

void Display::send_event_to_business_logic(genv::event ev){

	bl->widget_event_handler(name, ev);

}

void Display::set_background_color(std::string col){

	this->background_color = col;

}


void Display::set_font_color(std::string col){

	this->font_color = col;

}


void Display::set_highlight_color(std::string col){

	this->highlight_color = col;

}






