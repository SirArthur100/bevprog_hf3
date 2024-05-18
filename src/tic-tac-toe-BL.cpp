#include "../include/tic-tac-toe-BL.hpp"
#include <fstream>

/*

This file contains the implementation of the tic-tac-toe game. The TicTacBL class will be responsible
to ensure a consistent gameplay and communication with the data storage. 

This class recieves events as inputs from the UI widgets. After recieving the event from the UI, 
this class will use the data from the storage to check whether the move is allowed or not. 
If it is allowed, then make the changes in the storage. 

On change, the storage will automatically update the UI. So this TicTacBL class will guarantee us that
we will always be in a valid state.

The state of a game can be defined by:
- map (with size) - positions (NONE=0, PLAYER 1 = 1, PLAYER 2 = 2)
- end status (NO/WIN/DRAW)
- current player

Allowed map sizes are 15 and 30.

Storage for a game will be represented as:

key 		: value
-----------
map 		: vector of coordinates (in one row)
current_player 	: 1 or 2
win_state	: 0=no, 1=player1, 2=player2, 3=draw

The map points are originally 0, with each incoming step, the currently selected coordinate
is checked by the BL if it is valid. If it is valid, then the BL will check winning conditions
for the player. If no win is present, then check draw. If no end condition is reached, then the game will continue.

*/

TicTacBL::TicTacBL(std::string s, DataStorage * dstore, std::vector<WidgetContainer *> *containers) : BusinessLogic(s, dstore){
	
	game_container = containers;
	
	construct_menu();
}

void TicTacBL::construct_menu(){

	ds->post("menu_contents", "Small game");
	ds->post("menu_contents", "Large game");
	ds->post("menu_contents", "Quit game");
	ds->post("menu_selected", "");

	Menu* menu = new Menu(50, 100, 200, 200, "menu", ds, this, 3, LARGE, "menu_contents");
	menu->move_widget(0, 0);
	menu->set_moveable(false);
	
	game_menu = menu;
	ds->add_widget(game_menu);
	game_container->push_back(game_menu);

}

void TicTacBL::widget_event_handler(std::string s, genv::event ev){

	std::cout << "CURRENT EVENT: " << s << std::endl;

	// handle menu selections
	if(ev.button == 1 && parent_event == "menu"){
	
		if(s.compare("Quit") == 0){
		
		
		} else if(s.compare("Small game") == 0){
		
			std::cout << "START CREATING SMALL MAP!! " << std::endl;
			set_game_size(3);
			
		
		} else if (s.compare("Large game") == 0){
		
			set_game_size(15);
		}
		
		// remove old tiles if any
		if (game_tiles != NULL){
		
			remove_elements();
		
		}
		
		std::cout << "Elements removed!" << std::endl;
	
		// create new tiles
	
		Tiles * tiles = new Tiles(800, 548, 1000, 1000, "tiles", SMALL, ds, this, game_size);
		tiles->move_widget(0, 52);
		tiles->set_moveable(false);
		
		std::cout << "TILES CREATED!! " << std::endl;
		
		game_tiles = tiles;
		ds->add_widget(game_tiles);
		game_container->push_back(game_tiles);
		
		std::cout << "TILES ADDED!! " << std::endl;
		
		initiate_current_player();
		
		generate_starting_state();
		
		std::cout << "SETUP COMPLETE!! " << std::endl;
	
		std::cout << "BEFORE UPDATE: " << std::endl;
		ds->update("menu_selected", 0, s);
	}

	
	// handle 

	// parent event checks the container of the widget (if there is any)
	parent_event = s;
}

void TicTacBL::remove_elements(){

	game_tiles->clear_widgets();
	ds->del_widget_by_name(game_tiles->name);
	ds->del(game_tiles->name);
	delete game_tiles;
	game_tiles = NULL;
	
	std::cout << "PURGED!! " << std::endl;
	
	//game_container->clear();
	for (std::vector<WidgetContainer *>::iterator it = game_container->begin(); it != game_container->end(); ++it) {
	    if ((*it)->name.compare("tiles") == 0) {
		game_container->erase(it);
		break;
	    }
	}
	
	std::cout << "CONTAINER CLEAR!! " << std::endl;
	
	//construct_menu();
	
	std::cout << "NEW MENU CONSTRUCTED!! " << std::endl;
}

void TicTacBL::file_save(std::string filename, std::string map_key){

	  std::ofstream MyFile( "../outputs/" + filename + ".txt");

	  MyFile << ds->get(map_key, 0);

	  MyFile.close();

}
	
void TicTacBL::set_game_size(size_t s){

	this->game_size = s;

}

void TicTacBL::generate_starting_state(){
	
	// initiate each and every map positions with zeros	
	for(size_t i=0 ; i < game_size * game_size ; i++){
	
		this->ds->post("map", "0");
		
	}
}

void TicTacBL::initiate_current_player(){

	this->ds->post("current_player", "1");

}

bool TicTacBL::is_valid_move(size_t pos){

	// simply check whether the selected tile is empty or not 
	return ds->get("map", pos).compare("0") == 0;

}

std::vector< std::vector< std::string > > TicTacBL::load_map(){

	// this function will load the map into 2D vector
	// first we get the whole map from the data storage
	std::vector< std::string > row_wise_map = ds->get_whole("map");
	
	// we need to slice the map such that it fits into the proper 2D structure
	std::vector< std::vector< std::string > > two_d_map;
	
	for (size_t i = 0; i < game_size ; i++){
	
		std::vector< std::string > row;
		
		for (size_t j = i * game_size ; j < i * game_size + game_size ; j++){
		
			row.push_back(row_wise_map[j]);
		
		}
	
		two_d_map.push_back(row);
	
	}
	
	return two_d_map;

}


bool TicTacBL::is_player_win(){

	// a player automatically wins if he/she has five own shape in
	// a straight line continously (horizontally/vertically/diagonally)
	// these are four directions to check
	
	// first we load the whole map
	std::vector< std::vector< std::string > > map = load_map();
	
	// get current player
	std::string current_player = ds->get("current_player", 0);
	
	int counter = 0;
	
	// check vertically
	for (size_t i = 0; i < game_size ; i++){
	
		counter = 0;
	
		for (size_t j = 0; j < game_size ; j++){
	
			if (map[j][i].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		
		}
	}
	
	// check horizontally
	for (size_t i = 0; i < game_size ; i++){
	
		counter = 0;
	
		for (size_t j = 0; j < game_size ; j++){
	
			if (map[i][j].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		
		}
	}
	
	// check lower diag
	for (size_t i = 0; i < game_size ; i++){
	
		counter = 0;
	
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[i+j][j].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
		
		counter = 0;
		
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[j][i+j].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
	}
	
	// check upper diag
	for (size_t i = 0; i < game_size ; i++){
	
		counter = 0;
	
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[i-j][j].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
		
		counter = 0;
		
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[game_size - j][i+j].compare(current_player) == 0){
			
				counter++;
			
			} else {
			
				counter = 0;
			
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
	}
	
	
	// if we did not return yet, return false, no winning condition is satisfied
	return false;
	

}
bool TicTacBL::is_full(){

	// we check whether every tile is occupied (we suppose that we checked that neither
	// of the player)
	
	std::vector< std::string > row_wise_map = ds->get_whole("map");
	
	for (size_t i = 0 ; i < game_size * game_size ; i++){
	
		if (row_wise_map[i].compare("0") == 0){
		
			return false;
		
		}
	
	}
	
	return true;

}
