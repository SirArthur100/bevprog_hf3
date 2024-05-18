#ifndef BusinessLogic_HPP
#define BusinessLogic_HPP

/*

This object connects the widget with the data. Each functional widget group shares a Business Logic.

*/

#include "DataStorageAPI.hpp"
#include "../lib/graphics.hpp"

#include <vector>

class BusinessLogic{

protected:

	std::string name;
	DataStorage * ds;
	
public:

	BusinessLogic(std::string n, DataStorage * dstore){
	
		this->name = n;
		add_data_storage(dstore);
	
	}

	void add_data_storage(DataStorage * dstore){

		this->ds = dstore;

	}

	//----------------------------virtuals---------------------------------
	virtual void widget_event_handler(std::string s, genv::event ev){};
	virtual void file_save(std::string filename, std::string map_key){};

};


#endif
