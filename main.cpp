#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

void alterCellstate(int j);
void movement();

sf::RenderWindow window(sf::VideoMode(350,350), "simulation");    

sf::RectangleShape cell(sf::Vector2f(25,25));
std::vector<sf::RectangleShape> grid;
int rowSize = 12;
int CellPosY = 0;

sf::CircleShape cursor;
sf::Vector2i mousePos = sf::Mouse::getPosition( window );

int cellstate[36] = {0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0};
// possible cellstates
// 0 air, static. with no special abilities
// 1 stone, static. no special abilities
// 2 water/sand, dynamic. (solid collision)
// other possibilities (for future)

int main(){
    // map
    cell.setOutlineThickness(-2);
    cell.setOutlineColor(sf::Color(1,1,1));
    
    // setting up the grid
    for(int i=0; i<sizeof(cellstate); i++){
        grid.push_back(cell);
        if(i<=10){
            grid[i].setPosition(i*25, CellPosY);
        }else if(i%rowSize==0){
            CellPosY=CellPosY+25;
        }else{
            grid[i].setPosition(grid[i-rowSize].getPosition().x, CellPosY);
        }
    }

    // cursor
    cursor.setRadius(1);
    cursor.setOrigin(1,1);

    while(window.isOpen()){
        // update
        mousePos = sf::Mouse::getPosition(window);
        cursor.setPosition(mousePos.x, mousePos.y);
        movement();

        //button events
        for(int j=0; j<grid.size(); j++){
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds())){
                    cellstate[j]=1;
                }
            }else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                if(cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds())){
                    cellstate[j]=2;
                }
            }else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)){
                if(cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds())){
                    cellstate[j]=0;
                }
            }
        }
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        window.clear();
        for(int o=0; o<grid.size(); o++){
            window.draw(grid[o]);
            alterCellstate(o);
        }
        window.display();
        std::this_thread::sleep_for(0.08s);
    }
}

void alterCellstate(int j){
    if(cellstate[j] == 1){
        grid[j].setFillColor(sf::Color(125,125,130));   // stone
    }else if(cellstate[j] == 2){
        grid[j].setFillColor(sf::Color(55,150,215));    // water/sand
    }else if(cellstate[j] == 0 || cellstate[j] >= 3){
        grid[j].setFillColor(sf::Color(252,252,252));   // air
        cellstate[j] = 0;
    }
}

void movement(){
    for(int p=0; p<grid.size(); p++){
        if(cellstate[p]==2 && p <= sizeof(cellstate)-rowSize){
            // check cellstate of cell below
            if(cellstate[p+rowSize]==0){                      
                cellstate[p]-=2;
                cellstate[p+rowSize]=2;
            }
            
            if(cellstate[p+rowSize]==1){
                //check cell state of cells next to the stone cell
                if(cellstate[p+(rowSize-1)]==0 && cellstate[p-1]!=1){
                    cellstate[p]-=2;
                    cellstate[p+(rowSize-1)]=2;
                }else if(cellstate[p+(rowSize+1)]==0 && cellstate[p+1]!=1){
                    cellstate[p]-=2;
                    cellstate[p+(rowSize+1)]=2;
                }
            }

            if(cellstate[p+rowSize]==2){
                // make this work
            }
        }
    }
}

/*
BUGS:
    - side cells going on on the other side of the grid -> make grid one layer bigger off bounce so that the side cannot be used
    - map drawing -> i cannot be greater than 10 (?)
    - water falling over water needs to be designed
*/