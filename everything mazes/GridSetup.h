#pragma once

void gridSetup(int sizeX, int sizeY, std::vector<std::shared_ptr<Object>>& renderList, std::map<cordinates, std::shared_ptr<Tile>>& tileMap, std::map<cordinates, walls>& wallMap, std::vector<cordinates>& expressedwallMap);

std::pair<cordinates, cordinates> recursiveBacktrackingMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap);

std::pair<cordinates, cordinates> primsMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap);

std::pair<cordinates, cordinates> kruskalsMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap);