#ifndef Arrow_HPP
#define Arrow_HPP

#include "WidgetBase.hpp"

/*

A very simple arrow GUI widget.

*/

enum ARROW_DIRECTION {
    up,
    down,
    left,
    right
};

class Arrow : public WidgetBase{

	ARROW_DIRECTION direction;

public:

	Arrow( int xx, int yy, int canvas_x, int canvas_y, std::string nn);
			
	//----------------------------overrides---------------------------------
	void draw() override;
	void refresh() override;
	void event_handler(genv::event ev) override;

	//----------------------------specifics---------------------------------
	
	// direction can be up/down/left/right
	void set_direction(ARROW_DIRECTION new_dir);
	ARROW_DIRECTION get_direction();
	
	void send_event_to_business_logic(genv::event ev) override;

};

#endif
