#include "Tiles.h"

void Tiles::setting_flag(bool x) {
    flag_here = x;
}

bool Tiles::check_flag() {
    return flag_here;
}
int Tiles::getter_row() {
    return row;
}
int Tiles::getter_col() {
    return col;
}

void Tiles::setting_sprite(sf::Sprite& a) {
        sprite = a ;
}

sf::Sprite Tiles::getter_sprite() {
    return sprite;
}

void Tiles::setting_mine(bool x) {
    mine_here = x;
}

bool Tiles::mine_tile() {
    return mine_here;
}

void Tiles::set_null(Tiles* x) {
    adjacent_mines.push_back(x);
}

vector<Tiles *> &Tiles::getter_adjacent() {
    return adjacent_mines;
}

void Tiles::sum_adjacent_mines() {
    number_of_adjacent_mines += 1;

}

int Tiles::adjacent_getter() {
    return number_of_adjacent_mines;
}

void Tiles::set_pointer(Tiles *x, int pos) {
    adjacent_mines[pos] = x;
}

void Tiles::set_revelead(bool x) {
    isreveal = x;
}

bool Tiles::get_isrevealed() {
    return isreveal;
}

