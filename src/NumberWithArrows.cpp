#include "../include/NumberWithArrows.hpp"
#include <fstream>

NumberWithArrows::NumberWithArrows(std::string n) : BusinessLogic(n) {
	
	}
	
//void NumberWithArrows::add_data_storage(DataStorage * dstore){

//	this->ds = dstore;

//}

void NumberWithArrows::set_min_and_max(int minim, int maxim){

	this->minimum = minim;
	this->maximum = maxim;

}

void NumberWithArrows::widget_event_handler(std::string s, genv::event ev){

	// we want to modify the display element of the data storage
	// take the old value and increase it by one
	
	std::cout << "NumberWithArrows event start" << std::endl;
	std::cout << "s: " << s << std::endl;
	std::cout << "ev.keycode: " << ev.keycode << std::endl;
	
	if(s.compare("small_counter") == 0){
		
		if (ev.keycode == genv::key_up){
		
			int old_val = std::stoi(ds->get("display", 0));
			ds->update("display", 0, std::to_string( std::min(old_val + 1, maximum)));
			file_save(name, "display");
			
		} else if (ev.keycode == genv::key_down) {
		
			int old_val = std::stoi(ds->get("display", 0));
			ds->update("display", 0, std::to_string( std::max(old_val - 1, minimum)));
			file_save(name, "display");
		
		} else if (ev.keycode == genv::key_pgup) {
		
			int old_val = std::stoi(ds->get("display", 0));
			ds->update("display", 0, std::to_string( std::min(old_val + 10, maximum)));
			file_save(name, "display");
		
		} else if (ev.keycode == genv::key_pgdn) {
		
			int old_val = std::stoi(ds->get("display", 0));
			ds->update("display", 0, std::to_string( std::max(old_val - 10, minimum)));
			file_save(name, "display");
		}
	
	} else if(s.compare("arrow_up") == 0){
		
		int old_val = std::stoi(ds->get("display", 0));
		ds->update("display", 0, std::to_string( std::min(old_val + 1, maximum)));
		file_save(name, "display");
	
	} else if(s.compare("arrow_down") == 0){
	
		int old_val = std::stoi(ds->get("display", 0));
		ds->update("display", 0, std::to_string( std::max(old_val - 1, minimum)));
		file_save(name, "display");
	}
	
	//std::cout << "NumberWithArrows event end" << std::endl;

}

void NumberWithArrows::file_save(std::string filename, std::string map_key){

	  std::ofstream MyFile("../outputs/" + filename + ".txt");

	  MyFile << ds->get(map_key, 0);

	  MyFile.close();

}



