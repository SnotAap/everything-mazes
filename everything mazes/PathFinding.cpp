#include "Header.h"

double distance(cordinates start, cordinates end)
{
	return double(sqrt((pow(end.first, 2) - pow(start.first, 2)) + (pow(end.second, 2) - pow(start.second, 2))));
}


