#ifndef TicTacBL_HPP
#define TicTacBL_HPP

#include "BusinessLogic.hpp"

#include <vector>

const int WIN_TILES = 5;

class TicTacBL : public BusinessLogic{
	
public:

	int game_size;

	// initiators
	TicTacBL(std::string n, std::string map_size);
	void set_game_size(size_t s);
	void generate_starting_state();
	void initiate_current_player();
	
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
