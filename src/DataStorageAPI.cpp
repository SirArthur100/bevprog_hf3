#include "../include/DataStorageAPI.hpp"
#include "../include/WidgetBase.hpp"

#include <fstream>

void DataStorage::add_widget(WidgetBase * w){
    	
	connected_widgets.push_back(w);

}
    	
void DataStorage::refresh_single_connection(std::string s){

	for(WidgetBase * &w: connected_widgets){

		if( w->name.compare( s ) == 0 ){
		
			w->refresh();
			break;
		}
		
	}
	
	write_content_to_file();
	
}

void DataStorage::refresh_connections(){

	for(WidgetBase * &w: connected_widgets){

		w->refresh();
		
	}
	
	write_content_to_file();
	
}

void DataStorage::write_content_to_file(){

 	std::ofstream MyFile( "../outputs/tic_tac_storage.txt");

	for (const auto & [key, value] : data){
	
		MyFile << key << " : ";
	
		for(size_t j = 0; j < value.size() ; j++){
	
			MyFile << value[j] << " ; ";
	
		}
		
		MyFile << "\n";
	
	}

	MyFile.close();

}

void DataStorage::del_widget_by_name(std::string widget_name){
	
	for (std::vector<WidgetBase *>::iterator it = connected_widgets.begin(); it != connected_widgets.end(); ++it) {
	    if ((*it)->name.compare(widget_name) == 0) {
		connected_widgets.erase(it);
		break;
	    }
	}
}
    	
    	
    	
