#include "../include/PlainList.hpp"
#include <fstream>

PlainList::PlainList(std::string s) : BusinessLogic(s){

}

void PlainList::widget_event_handler(std::string s, genv::event ev){

	// we want to modify the display element of the data storage
	
	if(ev.button == genv::btn_left && s != "menu"){
	
		ds->update("selected", 0, s);
		file_save(name, "selected");
	
	}

}

void PlainList::file_save(std::string filename, std::string map_key){

	  std::ofstream MyFile( "../outputs/" + filename + ".txt");

	  MyFile << ds->get(map_key, 0);

	  MyFile.close();

}
