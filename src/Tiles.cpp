#include "../include/Tiles.hpp"

Tiles::Tiles(int xx, int yy, int cx, int cy, std::string nn, SIZE siz, DataStorage * d, BusinessLogic * b)
: WidgetContainer(xx,yy,cx,cy,nn, siz)
{

	add_data_storage(d);
	add_business_logic(b);
	
	generate_tiles();

}

void Tiles::clear_widgets(){

	for(WidgetBase * &w: connected_widgets){
			
		delete w;
	}
	
	connected_widgets.clear();
}	

void Tiles::generate_tiles(){

	std::cout << "GENERATE TILES!!" << std::endl;

	clear_widgets();
	
	int minim = std::min( get_size_x(), get_size_y() );
	int maxim = std::max( get_size_x(), get_size_y() );
	
	int tile_x = minim / map_size;
	int tile_y = minim / map_size;
	
	int x_shift = (maxim - minim) / 2;

	for(size_t i = 0; i < map_size ; i++){
	
		for(size_t j = 0; j < map_size ; j++){
	
			Tile * list_elem = new Tile(tile_x, tile_y, tile_x*2, tile_y*2, std::to_string( i * map_size + j ) );
			list_elem->set_x(this->x + x_shift + i * tile_x);
			list_elem->set_y(this->y + 3 + j * tile_y);
			list_elem->add_business_logic(this->bl);
			list_elem->add_data_storage(this->connected_data_storages[0]);
			list_elem->set_background_color("light-gray");
			list_elem->set_highlight_color("red");

			connected_widgets.push_back( list_elem );
		
		}
	}

}

void Tiles::refresh(){
	
	WidgetContainer::refresh();


}
	
void Tiles::event_handler(genv::event ev){

	WidgetContainer::event_handler(ev);

}
