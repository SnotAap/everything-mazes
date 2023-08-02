#include "Header.h"

std::vector<std::shared_ptr<Object>> renderList;
std::map<cordinates, walls> wallMap;
std::map<cordinates, std::shared_ptr<Tile>> tileMap;
std::vector<cordinates> expressedwallMap;
std::pair<cordinates, cordinates> startAndEndCords;
std::vector<cordinates> path;
int sizeX = 20;
int sizeY = 20;


int main()
{

    srand(unsigned int(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mazes!");
    gridSetup(sizeX, sizeY, renderList, tileMap, wallMap, expressedwallMap);
    //startAndEndCords = recursiveBacktrackingMaze(sizeX, sizeY, wallMap, tileMap);
    //startAndEndCords = primsMaze(sizeX, sizeY, wallMap, tileMap);
    startAndEndCords = kruskalsMaze(sizeX, sizeY, wallMap, tileMap);
    
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




