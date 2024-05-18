#include "../include/WidgetBase.hpp"

#include <iostream>

int WidgetBase::id_counter = 0;

WidgetBase::WidgetBase(int xx, int yy, int cx, int cy, std::string nn, SIZE siz){

	set_size_x(xx);
	set_size_y(yy);
	rename(nn);
	this->id = id_counter;
	id_counter++;
	
	this->widget_size = siz;
	
	this->canvas_x = cx;
	this->canvas_y = cy;
	this->set_active(false);
	this->set_x(0);
	this->set_y(0);
	
	// all canvas is transparent, does not necessarily 
	// having font defined, the default color is white
	this->c.open(canvas_x, canvas_y);
	this->c.transparent(true);
	this->set_color( std::string("white") );
	
	this->visible = true;
	this->interactive = true;
	this->highlighted = false;
	this->destroyable = false;
}

void WidgetBase::set_size_x(int new_size){

	this->size_x = new_size;

}

void WidgetBase::set_size_y(int new_size){

	this->size_y = new_size;

}

void WidgetBase::set_x(int xx){

	this->x = std::max( xx, 0 );

}

void WidgetBase::set_y(int yy){

	this->y = std::max( yy, 0 );

}

void WidgetBase::rename(std::string nn){

	this->name = nn;
	
	// if we rename a widget, we should register the new name in the
	// data storage too
	if( !connected_data_storages.empty() ){
	
		connected_data_storages[0]->post(name, name);
		refresh();
	}

}

int WidgetBase::get_size_x(){

	return this->size_x;

}

int WidgetBase::get_size_y(){

	return this->size_y;

}

int WidgetBase::get_x(){

	return this->x;

}

int WidgetBase::get_y(){

	return this->y;

}

bool WidgetBase::get_active(){

	return this->active;

}

void WidgetBase::set_active(bool a){

	this->active = a;
	
	std::cout << "activity: " << this->name << " : " << get_active() << std::endl;
	
	// we need to refresh since there might be a change in appearance
	refresh();

}

bool WidgetBase::get_draggable(){

	return this->draggable;

}

void WidgetBase::set_draggable(bool a){

	this->draggable = a;

}

bool WidgetBase::clicked(int mx, int my)
{

	return mx > x && mx < x + size_x && my > y && my < y + size_y;
}

void WidgetBase::add_business_logic(BusinessLogic *bbll){

	this->bl = bbll;

}

void WidgetBase::add_widget(WidgetBase * new_widget)
{
	this->connected_widgets.push_back(new_widget);
}

void WidgetBase::add_data_storage(DataStorage * new_ds)
{
	this->connected_data_storages.push_back(new_ds);
	std::cout << "NEW STORAGE ADDED" << std::endl;
	
	// when we attach a storage to a widget element, we register it there, creating a new
	// key value pair, default the key and value are the name of the widget
	connected_data_storages[0]->post(name, name);
	
	refresh();
}

void WidgetBase::set_color(std::string color_picked){

	this->r = WIDGET_COLORS[color_picked][0];
	this->g = WIDGET_COLORS[color_picked][1];
	this->b = WIDGET_COLORS[color_picked][2];
	
	refresh();
}

void WidgetBase::set_fontsize(size_t new_font_size){

	this->c.load_font("../resources/LiberationSans-Regular.ttf", new_font_size);

}

void WidgetBase::set_highlighted(bool val){

	this->highlighted = val;

}




