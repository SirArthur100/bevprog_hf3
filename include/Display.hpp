#ifndef Display_HPP
#define Display_HPP

/*

This widget has one purpose: show text, with or without background.

*/

#include "WidgetBase.hpp"
#include <iostream>

class Display : public WidgetBase{

	std::string text_contained;
	std::string background_color;
	std::string highlight_color;
	std::string font_color;
	
public:

	Display( int xx, int yy, int canvas_x, int canvas_y, std::string nn, SIZE siz = SMALL);
			
	//----------------------------overrides---------------------------------
	void draw() override;
	void refresh() override;
	void event_handler(genv::event ev) override;

	//----------------------------specifics---------------------------------
	void set_text(std::string ss);
	std::string get_text();
	
	void set_background_color(std::string col);
	void set_font_color(std::string col);
	void set_highlight_color(std::string col);
	void send_event_to_business_logic_2(std::string s);
	
	void send_event_to_business_logic(genv::event ev) override;

};

#endif
