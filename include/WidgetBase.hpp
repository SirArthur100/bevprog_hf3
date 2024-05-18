#ifndef WidgetBase_HPP
#define WidgetBase_HPP

/*
The WidgetBase class defines the most basic element of the GUI.
All other GUI elements should inherit from this class. 
*/

#include "../lib/graphics.hpp"
#include "Colors.hpp"
#include "DataStorageAPI.hpp"
#include "BusinessLogic.hpp"

#include <vector>

enum SIZE {

	SMALL,
	MEDIUM,
	LARGE
};


class WidgetBase{

public:

	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//--------------------------STATIC MEMEBERS----------------------------

	static int id_counter;
	
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//------------------------INSTANCE MEMEBERS----------------------------

	int size_x;
	int size_y;
	int x;
	int y;
	
	int canvas_x;
	int canvas_y;
	
	int id;
	
	std::string name;
	
	SIZE widget_size;
	
	bool active;
	bool visible;
	bool interactive;
	bool draggable;
	bool highlighted;
	bool destroyable;
	
	int r;
	int g;
	int b;
	
	std::vector<WidgetBase *> connected_widgets;
	std::vector<DataStorage *> connected_data_storages;
	BusinessLogic * bl;
	
	genv::canvas c;
		
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//----------------------------METHODS----------------------------------

	WidgetBase(int xx, int yy, int cx, int cy, std::string nn, SIZE siz = SMALL);
	
	// attach connections to this widget
	void add_widget(WidgetBase * new_widget);
	
	// attach DataStorage to widget
	void add_data_storage(DataStorage * new_ds);
	
	// size and position setters
	void set_size_x(int new_size);
	void set_size_y(int new_size);
	void set_x(int xx);
	void set_y(int yy);
	
	// size and position getters
	int get_size_x();
	int get_size_y();
	int get_x();
	int get_y();
	
	// UI interactions
	bool clicked(int mx, int my);
	
	bool get_active();
	void set_active(bool a);
	
	bool get_draggable();
	void set_draggable(bool a);
	
	// rename widget
	void rename(std::string nn);
	
	// color setter
	void set_color(std::string);
	void set_highlighted(bool val);
	
	// text
	void set_fontsize(size_t new_font_size);
	
	// business logic
	void add_business_logic(BusinessLogic *bbll);
	
	//----------------------------virtuals---------------------------------
	virtual void draw(){};
	virtual void refresh(){};
	virtual void event_handler(genv::event ev){};
	virtual void send_event_to_business_logic(genv::event ev){};
	
};

#endif
