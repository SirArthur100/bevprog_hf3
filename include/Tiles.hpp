#ifndef Tiles_HPP
#define Tiles_HPP

#include "WidgetContainer.hpp"
#include "Tile.hpp"

/*

The Tiles container will group together the Tile widgets of the game.

*/

class Tiles : public WidgetContainer{

	// this is a debugging variable
	int map_size;
	int sx;
	int sy;
	

public:

	Tiles(int xx, int yy, int cx, int cy, std::string nn, SIZE siz, DataStorage * d, BusinessLogic * b, int game_size);
	~Tiles();
	
	// specifics
	void generate_tiles();
	void clear_widgets();
	
	// overrides
	void event_handler(genv::event ev) override;
	void refresh() override;


};

#endif
