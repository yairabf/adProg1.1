
#include <iostream>
#include "Map.h"

void Map::setObstacle(Point point) {

}

void Map::updateChildren() {

}

Map::Map(int rows, int columns) {
    Map::columns = columns;
    Map::rows = rows;
    nodes.resize((unsigned long)Map::columns);
    for(int i = 0; i< columns; i++)
        nodes[i].resize((unsigned long)Map::rows);
    create();
}

Map::~Map() {
    std::cout << "Destructing map" << std::endl;
    for(int i=0;i<nodes.size();i++){
        for(int j=0;j<nodes[i].size();j++){
            delete nodes[i][j];
        }
    }
}

void Map::create() {
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            nodes[x][y] = (new NodeBlock(Point(x,y)));
        }
    }
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            if(x - 1 >= 0)
                nodes[x][y]->setLeft(nodes[x - 1][y]);
            if(y + 1 < rows)
                nodes[x][y]->setUp(nodes[x][y + 1]);
            if(x + 1 < columns )
                nodes[x][y]->setRight(nodes[x + 1][y]);
            if(y - 1 >= 0)
                nodes[x][y]->setDown(nodes[x][y - 1]);
        }
    }
}

Node* Map::getBlock(Point point) {
    return nodes[point.getX()][point.getY()];
}
