#ifndef Tile_HPP
#define Tile_HPP

/*

This widget will represent one tile in the tic-tac-toe game.

Each Tile can have three states:
EMTPY, CIRCLE, X, 

Player 1 will draw X, Player 2 will draw Y, otherwise, the Tile will be emtpy.

*/

#include "WidgetBase.hpp"
#include <iostream>

class Tile : public WidgetBase{

	std::string background_color;
	std::string highlight_color;
	
public:

	Tile( int xx, int yy, int canvas_x, int canvas_y, std::string nn);
			
	//----------------------------overrides---------------------------------
	void draw() override;
	void refresh() override;
	void event_handler(genv::event ev) override;
	void send_event_to_business_logic(genv::event ev) override;

	//----------------------------specifics---------------------------------
	
	void set_background_color(std::string col);
	void set_highlight_color(std::string col);

};

#endif
