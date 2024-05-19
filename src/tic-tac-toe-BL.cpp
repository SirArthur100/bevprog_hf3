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
win_state	: 0=no, 1=yes

The map points are originally 0, with each incoming step, the currently selected coordinate
is checked by the BL if it is valid. If it is valid, then the BL will check winning conditions
for the player. If no win is present, then check draw. If no end condition is reached, then the game will continue.

*/

TicTacBL::TicTacBL(std::string s, DataStorage * dstore, std::vector<WidgetContainer *> *containers, bool *q)
: BusinessLogic(s, dstore)
{
	
	game_container = containers;
	
	quit = q;
	
	construct_menu();
		
	construct_display();
	
	ds->post("win", "0");
}

TicTacBL::~TicTacBL(){
	
	for(WidgetContainer * c: *game_container){
	
		delete c;
	
	}
}

void TicTacBL::construct_display(){

	// create container for display
	WidgetContainer *cont = new WidgetContainer(200, 52, 400, 200, "game_display_container", LARGE);

	Display *disp = new Display(200, 52, 400, 200, "game_display", LARGE);
	disp->set_fontsize(20);
	disp->add_data_storage(ds);
	disp->add_business_logic(this);
	ds->update("game_display", 0, "Use menu!");
	ds->add_widget(disp);
	
	cont->add_widget(disp);
	cont->add_business_logic(this);
	cont->add_data_storage(ds);
	
	ds->add_widget(cont);
	
	cont->move_widget(161, 0);
	cont->set_moveable(false);
	cont->interactive = false;
	
	game_container->push_back(cont);

}

void TicTacBL::construct_menu(){

	ds->post("menu_contents", "Small game");
	ds->post("menu_contents", "Large game");
	ds->post("menu_contents", "Quit game");
	ds->post("menu_selected", "");

	Menu * game_menu = new Menu(50, 100, 200, 200, "menu", ds, this, 3, LARGE, "menu_contents");
	game_menu->move_widget(0, 0);
	game_menu->set_moveable(false);
	
	ds->add_widget(game_menu);
	game_container->push_back(game_menu);

}

void TicTacBL::construct_tiles(){

	// remove old tiles if any
	remove_elements();
	

	// create new tiles

	Tiles * game_tiles = new Tiles(800, 548, 1000, 1000, "tiles", SMALL, ds, this, game_size);
	game_tiles->move_widget(0, 52);
	game_tiles->set_moveable(false);
	
	
	ds->add_widget(game_tiles);
	game_container->push_back(game_tiles);
	
	
	initiate_current_player();
	
	generate_starting_state();
	
	game_tiles->refresh();

}

void TicTacBL::widget_event_handler(std::string s, genv::event ev){


	// -----------------------
	// handle menu selections
	// -----------------------
	if(ev.button == 1 && parent_event == "menu"){
	
		if(s.compare("Quit game") == 0){
			
			*quit = !(*quit);
		
		} else if(s.compare("Small game") == 0){
		
			//std::cout << "START CREATING SMALL MAP!! " << std::endl;
			set_game_size(15);
			construct_tiles();
			ds->update("game_display", 0, "Move: Player 1");
			
		
		} else if (s.compare("Large game") == 0){
		
			set_game_size(30);
			construct_tiles();
			ds->update("game_display", 0, "Move: Player 1");
		}
		
		ds->update("menu_selected", 0, s);
		
	}
	
	// -----------------------
	// handle tile selections
	// -----------------------
	
	if(ev.button == 1 && parent_event == "tiles" && s != "tiles" && ds->get("win",0).compare("0") == 0){
		
		// we can only place thingies to empty spaces
		if(ds->get("map", std::stoi(s)).compare("0") == 0){
			
			ds->update( "map", std::stoi(s), ds->get( "current_player", 0 ) );
			ds->update( s, 0, ds->get( "current_player", 0 ) );
			
			// after we updated the game positions
			// we check winning conditions for current player
			if(is_player_win()){
			
				ds->update("win",0,"1");
				
				ds->update("game_display", 0, "Player " + ds->get( "current_player", 0 ) + " won!");
				
				for (auto &t: winner_tiles){
				
					ds->post("winner_tiles",t);
				
				}
				
				ds->update("current_player", 0, "3");
			
			// switch players
			} else if (ds->get( "current_player", 0 ).compare("1") == 0){
		
				ds->update("current_player", 0, "2");
				ds->update("game_display", 0, "Move: Player 2");
			
			} else {
			
				ds->update("current_player", 0, "1");
				ds->update("game_display", 0, "Move: Player 1");
			
			}
			
		}
		
		// check for draw
		if (is_full() && ds->get("win",0).compare("0") == 0){
		
			ds->update("win",0,"2");
			ds->update("game_display", 0, "Draw!");
		
		}
	
	}

	// parent event holds the container of the widget (if there is any)
	parent_event = s;
}

void TicTacBL::remove_elements(){
	
	//game_container->clear();
	for (std::vector<WidgetContainer *>::iterator it = game_container->begin(); it != game_container->end(); ++it) {
	    
	    if ((*it)->name.compare("tiles") == 0) {
	    
		delete *it;
	    
		game_container->erase(it);
		break;
	    }
	}
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
	this->ds->del("map");
	for(size_t i=0 ; i < game_size * game_size ; i++){
	
		this->ds->post("map", "0");
		
	}
}

void TicTacBL::initiate_current_player(){


	this->ds->del("winner_tiles");
	winner_tiles.clear();
	
	this->ds->del("current_player");
	this->ds->del("win");
	
	ds->post("win", "0");
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
				winner_tiles.push_back( std::to_string(j*game_size+i) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
			
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
				winner_tiles.push_back( std::to_string(i*game_size+j) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
			
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
				winner_tiles.push_back( std::to_string((i+j)*game_size+j) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
		
		counter = 0;
		
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[j][i+j].compare(current_player) == 0){
			
				counter++;
				winner_tiles.push_back( std::to_string( j*game_size + (i+j) ) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
	}
	
	// check upper diag
	for (size_t i = 0; i < game_size ; i++){
	
		counter = 0;
	
		for (size_t j = 0; j < i+1 ; j++){
	
			if (map[i-j][j].compare(current_player) == 0){
			
				counter++;
				winner_tiles.push_back( std::to_string( (i-j)*game_size + (j) ) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
			}
			
			if (counter == WIN_TILES){
			
				return true;
			
			}
		}
		
		counter = 0;
		
		for (size_t j = 0; j < game_size - i ; j++){
	
			if (map[game_size - 1 - j][i+j].compare(current_player) == 0){
			
				counter++;
				winner_tiles.push_back( std::to_string( (game_size - 1 - j)*game_size + (i+j) ) );
			
			} else {
			
				counter = 0;
				winner_tiles.clear();
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
