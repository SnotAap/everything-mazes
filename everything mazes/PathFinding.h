#pragma once
std::vector<cordinates> Astar(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, cordinates start, cordinates end);

double distance(cordinates start, cordinates end);