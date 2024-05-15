#ifndef List_HPP
#define List_HPP

#include "WidgetContainer.hpp"
#include "Display.hpp"

enum ListType {

	SIMPLE,
	DROPDOWN,
	ROLLER
};

class List : public WidgetContainer{

	std::vector< std::string > list_elements;
	
	size_t elements_to_show;
	
	int idx_first;
	int idx_last;
	
	
	ListType type;

public:

	size_t fontsiz;
	std::string current_selection;
	int display_x;
	int display_y;

	List(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz = SMALL);
	
	void initialize_list();
	void deselect_all();
	void clear_list();
	void restructuring(std::string s);
	
	//void refresh() override;
	
	void event_handler(genv::event ev) override;

};


#endif
