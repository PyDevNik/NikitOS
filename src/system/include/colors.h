#pragma once

enum VGA_COLOR {
	BLACK 			= 0,
	BLUE 			= 1,
	GREEN 			= 2,
	CYAN 			= 3,
	RED 			= 4,
	MAGENTA 		= 5,
	BROWN 			= 6,
	LIGHT_GREY 		= 7,
	DARK_GREY 		= 8,
	LIGHT_BLUE 		= 9,
	LIGHT_GREEN 	= 10,
	LIGHT_CYAN 		= 11,
	LIGHT_RED 		= 12,
	LIGHT_MAGENTA 	= 13,
	LIGHT_BROWN 	= 14,
	WHITE 			= 15,
};

#define WHITE_ON_BLACK 	WHITE 	| BLACK << 4
#define RED_ON_WHITE 	RED 	| WHITE << 4
#define CYAN_ON_BLACK 	CYAN 	| BLACK << 4