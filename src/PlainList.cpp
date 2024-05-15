#include "../include/PlainList.hpp"
#include <fstream>

PlainList::PlainList(std::string s) : BusinessLogic(s){

}

void PlainList::widget_event_handler(std::string s, genv::event ev){

	// we want to modify the display element of the data storage
	// take the old value and increase it by one
	
	//std::cout << "LIST ELEMENT SELECTED" << std::endl;
	
	if(ev.button == genv::btn_left && s != "small_dropdown_list" && s != "large_dropdown_list" &&
					  s != "small_simple_list"   && s != "large_simple_list"){
	
		ds->update("selected", 0, s);
		file_save(name, "selected");
	
	}

}

void PlainList::file_save(std::string filename, std::string map_key){

	  std::ofstream MyFile( "../outputs/" + filename + ".txt");

	  MyFile << ds->get(map_key, 0);

	  MyFile.close();

}
