#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

void alterCellstate(int j);
void movement();

sf::RenderWindow window(sf::VideoMode(300,300), "");    

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
        if(i<=rowSize-1){
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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for(int j=0; j<grid.size(); j++){
                if (cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds()))
                {
                    cellstate[j]=1;
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for(int j=0; j<grid.size(); j++){
                if (cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds()))
                {
                    cellstate[j]=2;
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        {
            for(int j=0; j<grid.size(); j++){
                if (cursor.getGlobalBounds().intersects(grid[j].getGlobalBounds()))
                {
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
        for(int i=0; i<grid.size(); i++){
            window.draw(grid[i]);
            alterCellstate(i);
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
        if(cellstate[p] == 2 && p <= sizeof(cellstate)-rowSize){
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
                }
                if(cellstate[p+(rowSize+1)]==0 && cellstate[p+1]!=1){
                    cellstate[p]-=2;
                    cellstate[p+(rowSize+1)]=2;
                }
            }
        }
    }
}

/*
BUGS:
    side cells going on on the other side of the grid

BACKUP:
        // setting up the grid
        if(i<=5){
            grid[i].setPosition(i*25, 0);
        }else if(i>5 && i<12){
            grid[i].setPosition(grid[i-6].getPosition().x, 25);
        }else if(i>=12 && i<18){
            grid[i].setPosition(grid[i-6].getPosition().x, 50);
        }else if(i>=18 && i<24){
            grid[i].setPosition(grid[i-6].getPosition().x, 75);
        }else if(i>=24 && i<30){
            grid[i].setPosition(grid[i-6].getPosition().x, 100);
        }else if(i>=30 && i<36){
            grid[i].setPosition(grid[i-6].getPosition().x, 125);
        }
*/