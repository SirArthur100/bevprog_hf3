#ifndef Colors_HPP
#define Colors_HPP
/*

This file defines the basic color schemes that should be used througout 
the program.

*/

#include <map>
#include <vector>

static std::map< std::string, std::vector< int > > WIDGET_COLORS = {

	{"white", {255, 255, 255}},

	{"red", {255, 0, 0}},

	{"green", {0, 255, 0}},

	{"blue", {0, 0, 255}},

	{"black", {0, 0, 0}},
	
	{"light-gray", {200, 200, 200}},
	
	{"dark-gray", {100, 100, 100}}

};


#endif
