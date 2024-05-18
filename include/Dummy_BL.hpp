#ifndef Dummy_HPP
#define Dummy_HPP

#include "BusinessLogic.hpp"

/*

The Dummy_BL will do nothing with the recieved events.

*/

class Dummy_BL : public BusinessLogic{
	
public:

	Dummy_BL(std::string n, DataStorage * dstore);
	
	//void add_data_storage(DataStorage * ds) override;
	void widget_event_handler(std::string s, genv::event ev) override;
	void file_save(std::string filename, std::string map_key) override;

};

#endif
