#include "Header.h"

std::vector<std::shared_ptr<Object>> renderList;
std::map<cordinates, walls> wallMap;
std::map<cordinates, std::shared_ptr<Tile>> tileMap;
std::vector<cordinates> expressedwallMap;
std::pair<cordinates, cordinates> startAndEndCords;
std::vector<cordinates> path;
int sizeX = 20;
int sizeY = 20;
bool doesThisWork = false;

int main()
{

    srand(unsigned int(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mazes!");
    gridSetup(sizeX, sizeY, renderList, tileMap, wallMap, expressedwallMap);
    startAndEndCords = generateMaze(sizeX, sizeY, wallMap, tileMap);
    path = Astar(sizeX, sizeY, wallMap, startAndEndCords.first, startAndEndCords.second);
    for (int i = 0; i < expressedwallMap.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (wallMap[expressedwallMap[i]][j]->active)
            {
                renderList.emplace_back(wallMap[expressedwallMap[i]][j]);
            }
        }   
    } 

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }        
        window.clear(sf::Color::White);
        //
        
        for (int i = 0; i < renderList.size(); i++)
        {
            renderList[i]->draw(&window);
        }
        //
        window.display();
    }
    return 0;
}

//https://en.wikipedia.org/wiki/Maze_generation_algorithm
//https://stackoverflow.com/questions/13031462/difference-and-advantages-between-dijkstra-a-star
//https://en.wikipedia.org/wiki/Admissible_heuristic
//https://en.wikipedia.org/wiki/A*_search_algorithm
//https://www.youtube.com/watch?v=GC-nBgi9r0U
//https://www.youtube.com/watch?v=oCvb-Q5lXb8

//eyo is this working?