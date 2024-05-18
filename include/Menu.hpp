#ifndef Menu_HPP
#define Menu_HPP

#include "Dropdown.hpp"

class Menu : public Dropdown{

/*
	A dropdown menu consists of a simple display, and a list, from which the user can choose the content of the Display.
*/

public:

	Menu(int xx, int yy, int cx, int cy, std::string nn, DataStorage * d, BusinessLogic * b, size_t nel, SIZE siz = SMALL, std::string storage_ref_name = "list");
	
	void event_handler(genv::event ev) override;
};

#endif
