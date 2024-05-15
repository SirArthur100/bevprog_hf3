#include "../include/DataStorageAPI.hpp"
#include "../include/WidgetBase.hpp"

void DataStorage::add_widget(WidgetBase * w){
    	
	connected_widgets.push_back(w);

}
    	
void DataStorage::refresh_connections(){

	for(WidgetBase * &w: connected_widgets){

		w->refresh();
		
	}
}
