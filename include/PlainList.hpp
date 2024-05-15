#ifndef PlainList_HPP
#define PlainList_HPP

#include "BusinessLogic.hpp"

class PlainList : public BusinessLogic{
	
public:

	PlainList(std::string n);
	
	//void add_data_storage(DataStorage * ds) override;
	void widget_event_handler(std::string s, genv::event ev) override;
	void file_save(std::string filename, std::string map_key) override;

};

#endif
