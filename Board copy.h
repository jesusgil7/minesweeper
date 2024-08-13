#include <iostream>
#include <string>
#include <vector>
#include "Tiles.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class game_board{
private:
    vector<sf::Sprite> bandera;
    vector<Tiles> alltiles;
    vector<vector<int>> coordinates;
    bool game_over = false;
    int row;
    int col;


public:
    game_board(int row, int col);

    bool check_tile(int row, int col);

    bool check_flag(int row, int col);

    void change_flag(bool x, int row, int col);

    bool game_over_getter();

    vector<Tiles>& getter_tiles();
    void setting_flags();

    void drawing_flags( sf::RenderWindow& window, sf::Texture& text, sf::Texture& tile_hidden);
    bool checking_flags(int row, int col);

    void revealing_tile(int row, int col, sf::RenderWindow& window ,sf::Texture& texture);
    bool check_mine(int row, int col);
    void reveal_all_tiles(int row, int col, sf::RenderWindow& window ,sf::Texture& texture);
    void revealing_mines(sf::RenderWindow& window ,sf::Texture& texture);
    void adjacent();
    void print_numbers(sf::RenderWindow &window, sf::Texture& texture1, sf::Texture& texture2,
                       sf::Texture& texture3, sf::Texture& texture4, sf::Texture& texture5,
                       sf::Texture& texture6,sf::Texture& texture7,sf::Texture& texture8);

    int find_adjacent(int num_row, int num_col);
    void set_flag(int row, int col);
    void reveal( sf::RenderWindow& window ,sf::Texture& texture);
    void revealTiles(int row, int col, int num_col,int num_row);
    void setting_hidden_tiles();
    void set_flag_with_mines();
    void drawing_flags_final_flags(sf::RenderWindow& window, sf::Texture& text);

    bool check_winner();

};