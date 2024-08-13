
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Tiles.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;
class Tiles{
private:
    int row;
    int col;
    bool flag_here;
    bool mine_here;
    bool isreveal;

    sf::Sprite sprite;

    int number_of_adjacent_mines;

public:
    int print_num = 0;
    vector<Tiles*> adjacent_mines;
    Tiles(int _row,int _col){
        row = _row;
        col = _col;
        flag_here = false;
        mine_here = false;
        isreveal = false;
        number_of_adjacent_mines = 0;
    }
    void set_revelead(bool x);
    bool get_isrevealed();
    void setting_flag(bool x);
    bool check_flag();
    void setting_mine(bool x);
    bool mine_tile();
    int getter_row();
    int getter_col();
    void set_null(Tiles* x);
    void set_pointer(Tiles* x, int pos);
    void setting_sprite(sf::Sprite& a);
    void sum_adjacent_mines();
    int adjacent_getter();
    vector<Tiles*> getter_vector();
    vector<Tiles*> &getter_adjacent();
    sf::Sprite getter_sprite();


};