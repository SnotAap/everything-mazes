#include "Header.h"

std::vector<std::shared_ptr<Object>> renderList;
std::vector<cordinates> expressedwallMap;
std::pair<cordinates, cordinates> startAndEndCords;
std::vector<cordinates> path;
int sizeX = 20;
int sizeY = 20;

#define generateState 0
#define solveState 1
int state = 0;

unsigned long currentTime;
unsigned long lastTime;
unsigned int deltaTime;

int main()
{
    lastTime = timeGetTime();
    srand((unsigned int)timeGetTime());
    Grid grid(sizeX, sizeY);
    grid.setup(renderList, expressedwallMap);

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mazes!");

    startAndEndCords = grid.recursiveBacktrackingMaze(renderList, deltaTime);
    //startAndEndCords = grid.primsMaze(renderList, deltaTime);

    for (int i = 0; i < expressedwallMap.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
                renderList.emplace_back(grid.wallMap[expressedwallMap[i]][j]);
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
        currentTime = timeGetTime();
        deltaTime = currentTime - lastTime;
        //
        for (int i = 0; i < renderList.size(); i++)
        {
            renderList[i]->draw(&window);
        }
        //
        window.display();
        lastTime = currentTime;
    }
    return 0;
}




