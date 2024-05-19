#include "../include/Tile.hpp"

#include <algorithm>

using namespace genv;

/*

A tile will be the atomic unit of the playing area.

For drawing circles fast I used the "Midpoint circle algorithm".
https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
for this I partly used a code snippet from here:
https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/

*/
		
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

void Tile::set_storage_value(std::string new_val){

	connected_data_storages[0]->update(name, 0, new_val);

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
	
	// draw position
	if ( connected_data_storages[0]->get(this->name, 0).compare("1") == 0 ){
		c << genv::color(1, 1, 1) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::move_to(size_x-1, 0) << genv::line_to(0, 0+size_y-1);
		c << genv::move_to(1, 0) << genv::line_to(0+size_x-1, 0+size_y-2) << genv::move_to(size_x-1, 1) << genv::line_to(1, 0+size_y-1);
	} else if ( connected_data_storages[0]->get(this->name, 0).compare("2") == 0 ){
		circle_algo( (get_size_x() / 2) - 2 , (get_size_x() / 2), (get_size_x() / 2));
		circle_algo( (get_size_x() / 2) - 3 , (get_size_x() / 2), (get_size_x() / 2));
	} 
	
	// hover draw
	if( ( this->active || this->highlighted ) && connected_data_storages[0]->get(this->name, 0).compare("0") == 0 ){
	
		if ( connected_data_storages[0]->get("current_player", 0).compare("1") == 0 ){
			c << genv::color(1, 1, 1) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::move_to(size_x-1, 0) << genv::line_to(0, 0+size_y-1);
			c << genv::move_to(1, 0) << genv::line_to(0+size_x-1, 0+size_y-2) << genv::move_to(size_x-1, 1) << genv::line_to(1, 0+size_y-1);
		}
		
		if ( connected_data_storages[0]->get("current_player", 0).compare("2") == 0 ){
			circle_algo( (get_size_x() / 2) - 2 , (get_size_x() / 2), (get_size_x() / 2));
			circle_algo( (get_size_x() / 2) - 3 , (get_size_x() / 2), (get_size_x() / 2));
		}
	
	}
	
	// draw border
	if (connected_data_storages[0]->get("win", 0).compare("1") == 0){
	
		std::vector< std::string > temp = connected_data_storages[0]->get_whole("winner_tiles");
	
		if (std::find(temp.begin(), temp.end(), name) != temp.end())
		{
		  	c << genv::color(255,0,0) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::line_to(0, 0+size_y-1) << genv::line_to(0, 0);
		} else {
		
			c << genv::color(150,150,150) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::line_to(0, 0+size_y-1) << genv::line_to(0, 0);
		
		}
	
	} else {
	
		c << genv::color(150,150,150) << genv::move_to(0, 0) << genv::line_to(0+size_x-1, 0) << genv::line_to(0+size_x-1, 0+size_y-1) << genv::line_to(0, 0+size_y-1) << genv::line_to(0, 0);
		
	}

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




