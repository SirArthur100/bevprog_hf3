#ifndef NumberWithArrows_HPP
#define NumberWithArrows_HPP

#include "BusinessLogic.hpp"

class NumberWithArrows : public BusinessLogic{
	
	int minimum = -100;
	int maximum = 100;
	
public:

	NumberWithArrows(std::string n);
	
	//void add_data_storage(DataStorage * ds) override;
	void widget_event_handler(std::string s, genv::event ev) override;
	void file_save(std::string filename, std::string map_key) override;
	
	void set_min_and_max(int minim, int maxim);

};

#endif
