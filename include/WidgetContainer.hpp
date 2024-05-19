#ifndef WidgetContainer_HPP
#define WidgetContainer_HPP

/*

The purpose of a WidgetContainer is to contain and encapsulate other functionally
connected widgets. This is the first object to recieve the event from the GUI, and it distributes the events
to its contained widgets as needed.

Because of this hub-like function, the WidgetContainer will be responsible for the movement, refreshing and 
drawing of its contained objects.

*/

#include "WidgetBase.hpp"
#include <vector>

class WidgetContainer : public WidgetBase{

public:

	WidgetContainer(int xx, int yy, int cx, int cy, std::string nn, SIZE siz = SMALL);
	~WidgetContainer();
	
	bool is_dragged;
	bool is_moveable;
	
	// helper variables for dragging
	int distance_x;
	int distance_y;

	//----------------------------overrides---------------------------------
	void draw() override;
	void refresh() override;
	virtual void event_handler(genv::event ev) override;
	virtual void send_event_to_business_logic(genv::event ev);
	
	//----------------------------specifics---------------------------------
	void position_contained_widget(std::string n, int new_x, int new_y);
	void move_widget(int nx, int ny);
	virtual void deactivate();
	
	void set_moveable(bool val);
	bool get_moveable();
	
	void set_dragged(bool val);
	bool get_dragged();
	
	void forward_event(genv::event ev);

};

#endif
