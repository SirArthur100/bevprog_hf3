#ifndef DataStorageAPI_HPP
#define DataStorageAPI_HPP


/*

A very simple data storage for storing basic data types for the widgets.

*/

#include <map>
#include <vector>
#include <iostream>

class WidgetBase;

class DataStorage{

	// data can be achieved in a dictionary style, where the value can be a vector of strings
	
	std::map< std::string, std::vector< std::string >> data;
	
	std::vector<WidgetBase *> connected_widgets;
	
	
public:
	
	//DataStorage& operator[](std::size_t idx)       		{ return data[idx]; }
    	//const DataStorage& operator[](std::size_t idx) const 		{ return data[idx]; }
    	
    	void post(std::string key, std::string new_data){
    	
    		//std::cout << "POST" << std::endl;
    		//std::cout << "key: " << key << "   value: " << new_data << std::endl;
    		
    		std::map< std::string , std::vector< std::string > >::iterator it;
    		
    		// make sure the key exists
    		it = data.find(key);
		if (it == data.end()){
		
		    data.insert({ key, {} });
		
		}
    		
    		data[key].push_back(new_data); 
    		
    		refresh_connections();
    	
    	}
    	
    	void update(std::string key, size_t idx, std::string new_data){

    		std::cout << "UPDATE" << std::endl;
    		std::cout << "key: " << key << "   value: " << new_data << std::endl;
    		data[key][idx] = new_data;
    		refresh_connections();
    	
    	}
    	
    	std::string get(std::string key, size_t idx){
    		
    		// std::cout << "GET" << std::endl;
    		return data[key][idx]; 
    	
    	}
    	
    	std::vector< std::string > get_whole(std::string key){
    		
    		return data[key]; 
    	
    	}
    	
    	void add_widget(WidgetBase * w);
    	
    	void refresh_connections();

};


#endif
