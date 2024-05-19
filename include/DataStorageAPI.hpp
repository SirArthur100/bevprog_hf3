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
    	
    	void post(std::string key, std::string new_data){
    		
    		std::map< std::string , std::vector< std::string > >::iterator it;
    		
    		// make sure the key exists
    		it = data.find(key);
		if (it == data.end()){
		
		    data.insert({ key, {} });
		
		}
    		
    		data[key].push_back(new_data); 
    		
    		refresh_single_connection(key);
    	
    	}
    	
    	void update(std::string key, size_t idx, std::string new_data){

    		data[key][idx] = new_data;
    		refresh_single_connection(key);
    	
    	}
    	
    	std::string get(std::string key, size_t idx){
    		
    		return data[key][idx]; 
    	
    	}
    	
    	void del(std::string key){
    		
    		data.erase(key);
    	
    	}
    	
    	void purge(){
    		
    		data.clear();
    		connected_widgets.clear();
    	
    	}
    	
    	
    	std::vector< std::string > get_whole(std::string key){
    		
    		return data[key]; 
    	
    	}

    	void del_widget_by_name(std::string widget_name);
    	
    	void add_widget(WidgetBase * w);
    	
    	void refresh_connections();
    	
    	void refresh_single_connection(std::string key);
    	
    	void write_content_to_file();

};


#endif
