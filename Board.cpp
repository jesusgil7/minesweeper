#include "Board.h"

game_board::game_board(int row, int col) {
    for(int i =0; i < row; i++){
        for(int j = 0; j< col; j++) {
            alltiles.push_back(Tiles(j,i));
        }
    }
}

bool game_board::check_tile(int row, int col){
    for(int i =0; i < alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            cout << row << " " << col;
            return alltiles[i].check_flag();
        }
    }
}

vector<Tiles>& game_board::getter_tiles() {
    return alltiles;
}
void game_board::change_flag(bool x, int row, int col) {
    for(int i =0; i< alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            alltiles[i].setting_flag(x);
        }
    }
}

void game_board::set_flag(int row, int col) {
    for(int i =0; i< alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            if(!alltiles[i].check_flag()){
                alltiles[i].setting_flag(true);
            }
            else{
                alltiles[i].setting_flag(false);
            }
        }
    }
}

bool game_board::checking_flags(int row, int col) {
    for(int i =0; i< alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            return alltiles[i].check_flag();
        }
    }
}
void game_board::drawing_flags(sf::RenderWindow& window, sf::Texture& text,sf::Texture& tile_hidden) {
    for(int i =0; i< alltiles.size();i++){
if(alltiles[i].check_flag() == true){
    sf::Sprite sprite;
    sprite.setTexture(text);
    sprite.setPosition(alltiles[i].getter_row() * 32, alltiles[i].getter_col() * 32);
    window.draw(sprite);
}
else{
    sf::Sprite sprite;
    sprite.setTexture(tile_hidden);
    sprite.setPosition(alltiles[i].getter_row() * 32, alltiles[i].getter_col() * 32);
    window.draw(sprite);
    alltiles[i].setting_flag(false);

}
}
}

void game_board::setting_flags() {
    for(int i =0; i< alltiles.size();i++){
        alltiles[i].setting_flag(false);
    }
}

void game_board::revealing_tile(int row, int col,sf::RenderWindow& window, sf::Texture& texture) {
    for(int i =0; i< alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            if(alltiles[i].mine_tile()== true){
                game_over = true;
            }
            if (alltiles[i].check_flag() != true){
                sf::Sprite sprite;
                sprite.setTexture(texture);

// Draw the textured sprite
                sprite.setPosition(row * 32, col * 32);
                alltiles[i].set_revelead(true);
            }
        }
    }
}

void game_board::revealing_mines(sf::RenderWindow &window, sf::Texture &texture) {
        for(int i =0; i< alltiles.size();i++){
            if(alltiles[i].mine_tile() == true){
                sf::Sprite sprite;
                sprite.setTexture(texture);
                // Draw the textured sprite
                sprite.setPosition(32*alltiles[i].getter_row(), 32*alltiles[i].getter_col());
                window.draw(sprite);
        }
    }
}

bool game_board::game_over_getter() {
    return game_over;
}

void game_board::reveal_all_tiles(int row, int col, sf::RenderWindow &window, sf::Texture &texture) {
    for(int i =0; i< alltiles.size();i++){
            sf::Sprite sprite;
            sprite.setTexture(texture);
            // Draw the textured sprite
            sprite.setPosition(32*alltiles[i].getter_row(), 32*alltiles[i].getter_col());
            window.draw(sprite);
        }
}

void game_board::adjacent() {
    for (int j = 0 ; j< alltiles.size();j++){
        for(int i = 0; i<8; i++){
            alltiles[j].adjacent_mines.push_back(nullptr);
        }
    }
}



int game_board::find_adjacent( int num_col, int num_row) {

    for(int i =0 ; i < alltiles.size();i++) {
        int count = 0;
        row = alltiles[i].getter_row();
        col = alltiles[i].getter_col();

        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};


        for (int i = 0; i < 8; ++i) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            if (newRow >= 0 && newRow < num_row && newCol >= 0 && newCol < num_col) {
                // Check if the neighboring cell has a mine
                if (check_mine(newRow, newCol) == true) {
                    count++;
                }
            }
        }
        alltiles[i].print_num = count;

    }
    return 4;

}

void game_board::revealTiles(int row, int col, int num_col, int num_row) {
    // Check if the coordinates are within the valid range
    for (int i = 0; i < alltiles.size(); ++i) {
        if (row == alltiles[i].getter_row() && col == alltiles[i].getter_col()) {
            if (alltiles[i].get_isrevealed() || alltiles[i].mine_tile()) {
                return;
            }
            alltiles[i].set_revelead(true);
                if (alltiles[i].print_num == 0) {
                    for (int k = -1; k <= 1; ++k) {
                        for (int l = -1; l <= 1; ++l) {
                            revealTiles(row + k, col + l, num_row, num_col);

                    }
                }
            }
        }
    }
}

bool game_board::check_mine(int row, int col) {
    for(int i =0; i < alltiles.size();i++){
        if (row == alltiles[i].getter_row() and col == alltiles[i].getter_col()){
            return alltiles[i].mine_tile();
        }
    }
}


void game_board::print_numbers(sf::RenderWindow &window, sf::Texture& texture1, sf::Texture& texture2,
                               sf::Texture& texture3, sf::Texture& texture4, sf::Texture& texture5,
                               sf::Texture& texture6,sf::Texture& texture7,sf::Texture& texture8) {
    for(int i =0; i < alltiles.size();i++){
        int number_mines = alltiles[i].print_num;
        if(alltiles[i].get_isrevealed() == true and alltiles[i].mine_tile()==false)
        {
            sf::Sprite sprite;
            if(number_mines ==1){
                sprite.setTexture(texture1);

            }
            else if(number_mines ==2){
                sprite.setTexture(texture2);
            }
            else if(number_mines ==3){
                sprite.setTexture(texture3);
            }
            else if(number_mines ==4){
                sprite.setTexture(texture4);
            }
            else if(number_mines ==5){
                sprite.setTexture(texture5);
            }
            else if(number_mines == 6){
                sprite.setTexture(texture6);
            }
            else if(number_mines == 7){
                sprite.setTexture(texture7);
            }
            else if(number_mines ==8){
                sprite.setTexture(texture8);
            }


        sprite.setPosition(32*alltiles[i].getter_row(), 32*alltiles[i].getter_col());
        alltiles[i].setting_sprite(sprite);
        window.draw(sprite);
    }

}
}

void game_board::reveal(sf::RenderWindow& window ,sf::Texture& texture){
    for (int k = 0; k < alltiles.size(); k++){
        if(alltiles[k].get_isrevealed()==true){

            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setPosition(alltiles[k].getter_row() * 32, alltiles[k].getter_col() * 32);
            window.draw(sprite);
        }
    }
}

bool game_board::check_winner() {
    for (int i = 0; i < alltiles.size(); i++){
        if(alltiles[i].mine_tile()== false){
            if(alltiles[i].get_isrevealed()== false ){
                return false;
            }

        }
    }
    return true;
}

void game_board::setting_hidden_tiles() {
    for(int i =0; i < alltiles.size();i++){
        alltiles[i].set_revelead(false);
    }
}

void game_board::set_flag_with_mines() {
    for(int i =0; i < alltiles.size();i++){
        if(alltiles[i].mine_tile() == true){
            alltiles[i].setting_flag(true);
        }
    }
}




void game_board::drawing_flags_final_flags(sf::RenderWindow& window, sf::Texture& text) {
    for(int i =0; i< alltiles.size();i++){
        if(alltiles[i].check_flag() == true){
            sf::Sprite sprite;
            sprite.setTexture(text);
            sprite.setPosition(alltiles[i].getter_row() * 32, alltiles[i].getter_col() * 32);
            window.draw(sprite);
        }

}}
