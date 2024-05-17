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

	if(!interactive){
		return;
	}

	if( ev.button == 1 && clicked(ev.pos_x, ev.pos_y) ){
	
		this->set_active(true);
		
		if(!connected_data_storages.empty()){
		
			send_event_to_business_logic(ev);
		
		}
	
	} else if (clicked(ev.pos_x, ev.pos_y)){
	
		this->set_active(true);
	
	} else {
	
		this->set_active(false);
	
	}

}

void Tile::refresh(){

	
	
	// clear canvas
	c.open(this->canvas_x, this->canvas_y);
	
	// draw box
	if(this->active || this->highlighted){
	
		if (false){
			c << genv::color(1, 1, 1) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::move_to(size_x-1, 0) << genv::line_to(0, 0+size_y-1);
			c << genv::move_to(1, 0) << genv::line_to(0+size_x-1, 0+size_y-2) << genv::move_to(size_x-1, 1) << genv::line_to(1, 0+size_y-1);
			//c << genv::move_to(1, 0) << genv::line_to(0+size_x-2, 0+size_y-1) << genv::move_to(size_x-1, 1) << genv::line_to(1, 0+size_y-1);
		}
		
		if (true){
			circle_algo( (get_size_x() / 2) - 2 , (get_size_x() / 2), (get_size_x() / 2));
			circle_algo( (get_size_x() / 2) - 3 , (get_size_x() / 2), (get_size_x() / 2));
		}
	
		// c << move_to(0,0) << color(200, 200, 200) << box_to(this->size_x-1, this->size_y-1);
	
	} else {
	
		// c << move_to(0,0) << color(r, g, b) << box_to(this->size_x-1, this->size_y-1);
	
	}
	
	// draw border
	c << genv::color(150,150,150) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::line_to(0, 0+size_y-1) << genv::line_to(0, 0);

	// player position

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

void Tile::circle_algo(int r, int x_centre, int y_centre){

    int x = r, y = 0;
     
     c << genv::color(100,100,100);
     
    // Printing the initial point on the axes 
    // after translation
    c << genv::move_to( x + x_centre ,y + y_centre )<< genv::dot;
     
    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
        c << genv::move_to( x + x_centre, -y + y_centre )<< genv::dot;
        c << genv::move_to( y + x_centre, x + y_centre )<< genv::dot;
        c << genv::move_to( -y + x_centre, x + y_centre )<< genv::dot;
    }
     
    // Initialising the value of P
    int P = 1 - r;
    while (x > y)
    { 
        y++;
         
        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2*y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }
         
        // All the perimeter points have already been printed
        if (x < y)
            break;
         
        // Printing the generated point and its reflection
        // in the other octants after translation
        c << genv::move_to( x + x_centre, y + y_centre )<< genv::dot;
        c << genv::move_to( -x + x_centre, y + y_centre )<< genv::dot;
        c << genv::move_to( x + x_centre, -y + y_centre )<< genv::dot;
        c << genv::move_to( -x + x_centre, -y + y_centre )<< genv::dot;
         
        // If the generated point is on the line x = y then 
        // the perimeter points have already been printed
        if (x != y)
        {
            c << genv::move_to( y + x_centre, x + y_centre )<< genv::dot;
            c << genv::move_to( -y + x_centre, x + y_centre )<< genv::dot;
            c << genv::move_to( y + x_centre, -x + y_centre )<< genv::dot;
            c << genv::move_to( -y + x_centre, -x + y_centre )<< genv::dot;
        }
    }

}




