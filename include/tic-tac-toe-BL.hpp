#ifndef TicTacBL_HPP
#define TicTacBL_HPP

#include "BusinessLogic.hpp"
#include "WidgetContainer.hpp"
#include "Tiles.hpp"
#include "Menu.hpp"
#include "Display.hpp"

#include <vector>

const int WIN_TILES = 5;

class TicTacBL : public BusinessLogic{
	
public:

	// game elements
	std::vector<WidgetContainer *> *game_container;
	
	std::vector< std::string > winner_tiles;

	int game_size;
	int win;
	bool * quit;
	
	std::string parent_event;

	void remove_elements();

	// initiators
	TicTacBL(std::string n, DataStorage * dstore, std::vector<WidgetContainer *> *containers, bool *q);
	~TicTacBL();
	void set_game_size(size_t s);
	void generate_starting_state();
	void initiate_current_player();
	void construct_menu();
	void construct_tiles();
	void construct_display();
	
	// game state checkers
	bool is_valid_move(size_t pos);
	bool is_player_win();
	bool is_full();
	
	// game state transitions
	void set_move(size_t pos);
	
	// utilities
	std::vector< std::vector< std::string > > load_map();
	
	// event and file handlers
	void widget_event_handler(std::string s, genv::event ev) override;
	void file_save(std::string filename, std::string map_key) override;

};

#endif
