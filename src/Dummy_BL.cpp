#include "../include/Dummy_BL.hpp"
#include <fstream>

Dummy_BL::Dummy_BL(std::string s, DataStorage * dstore) : BusinessLogic(s, dstore){

}

void Dummy_BL::widget_event_handler(std::string s, genv::event ev){


}

void Dummy_BL::file_save(std::string filename, std::string map_key){


}
