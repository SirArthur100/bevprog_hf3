#ifndef Dropdown_HPP
#define Dropdown_HPP

#include "List.hpp"
#include "Display.hpp"
#include "Arrow.hpp"

class Dropdown : public List{

/*
	A dropdown menu consists of a simple display, and a list, from which the user can choose the content of the Display.
*/
	
	int original_y;
	
	
	Display * selected_widget;

public:

	bool show_full;
	bool element_clicked;

	Dropdown(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz = SMALL);
	
	void collapse_list();
	
	void event_handler(genv::event ev) override;
	void deactivate() override;
};

#endif
