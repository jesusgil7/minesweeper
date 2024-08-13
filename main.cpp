#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <chrono>
#include "TextureManager.h"
#include "Tiles.h"
#include "Board.h"
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <thread>

struct Person {
    std::string name;
    int minutes;
    int seconds;
};

bool compareTime(const Person& a, const Person& b) {
    if(a.minutes == b.minutes){
        return a.seconds < b.seconds;
    }
    else{
        return a.minutes < b.minutes;
    }

}

void setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
void generate_mines(game_board& myboard, int num_mine, int num_row, int num_col){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist1(0, num_row-1);
    std::uniform_int_distribution<int> dist2(0, num_col-1);

    for (int i = 0; i < num_mine; i++) {


        // Generate random numbers
        int randomNumber1 = dist1(gen);
        int randomNumber2 = dist2(gen);
        for (int i = 0; i < myboard.getter_tiles().size(); i++) {
            if (myboard.getter_tiles()[i].getter_row() == randomNumber2 and
                myboard.getter_tiles()[i].getter_col() == randomNumber1) {
                myboard.getter_tiles()[i].setting_mine(true);
            }
        }
    }
}
void erase_mines(game_board& myboard){
    for (int i = 0; i < myboard.getter_tiles().size(); i++) {
        {
            myboard.getter_tiles()[i].setting_mine(false);
        }
    }
}

void drawing_counter(game_board& myboard,  sf::Texture& texture, sf::RenderWindow& window, int num_row,  vector<int> &digitss ) {

    for (int i = 0; i < 3; ++i) {
        sf::Sprite digits;
        digits.setTexture(texture);
        digits.setTextureRect(sf::IntRect(21 * digitss[i], 0, 21, 32));
        digits.setPosition(33 + (21 * i), 32 * (num_row + 0.5) + 16);
        window.draw(digits);
    }
}

void drawing_timer(sf::Texture& texture, sf::RenderWindow& window, int _first_second, int _second_second, int _first_mine, int _second_minute, int num_row, int num_col){
    sf::Sprite first_second;
    first_second.setTexture(texture);
    first_second.setTextureRect(sf::IntRect(21 * _first_second , 0, 21, 32));
    first_second.setPosition(num_col* 32 -54 , 32 * (num_row + 0.5) + 16);
    window.draw(first_second);

    sf::Sprite second_second;
    second_second.setTexture(texture);
    second_second.setTextureRect(sf::IntRect(21 * _second_second , 0, 21, 32));
    second_second.setPosition(num_col* 32 -54+ 21, 32 * (num_row + 0.5) + 16);
    window.draw(second_second);

    sf::Sprite first_minute;
    first_minute.setTexture(texture);
    first_minute.setTextureRect(sf::IntRect(21 * _first_mine , 0, 21, 32));
    first_minute.setPosition(num_col* 32 -97, 32 * (num_row + 0.5) + 16);
    window.draw(first_minute);

    sf::Sprite second_minute;
    second_minute.setTexture(texture);
    second_minute.setTextureRect(sf::IntRect(21 * _second_minute , 0, 21, 32));
    second_minute.setPosition(num_col* 32 -97 + 21 , 32 * (num_row + 0.5) + 16);
    window.draw(second_minute);
}
void change_counter(bool erase, vector<int> &digitss){
    if (erase == true){
        if(digitss[0] == 0 and digitss[1] == 0 and digitss[2] == 0){
            digitss[0] = 10;
            digitss[1] = 0;
            digitss[2] = 0;
        }
        else if(digitss[2] == 9 and digitss[0] == 10  ){
            digitss[1] = digitss[1] + 1;
            digitss[2] = -1;
        }
        else if(digitss[2] == 0 and digitss[0] != 10){
            digitss[2] = 10;
            digitss[1] = digitss[1] -1;
        }

        if(digitss[0]!=10){
            digitss[2] = digitss[2]-1;
        }
        else{
            digitss[2] = digitss[2]+1;
        }

    }

    if (erase == false){
        if(digitss[2] == 9 and digitss[0]==10){
            digitss[2] = -1;
            digitss[1] = digitss[1] - 1;
        }
        else if(digitss[2] == 9){
            digitss[2] = -1;
            digitss[1] = digitss[1] + 1;
        }

        if(digitss[0]==10){
            digitss[2] = digitss[2]-1;
        }
        else {
            digitss[2] = digitss[2]+1;

        }
        if(digitss[0] == 10 and digitss[1] == 0 and digitss[2] == 0){
            digitss[0] = 0;
            digitss[1] = 0;
            digitss[2] = 0;
        }
    }

}
int main() {
    // Reading the cfg
    std::ifstream cfg_file("config.cfg");

    if(!cfg_file.is_open()){
        std::cout << "Error" << std::endl;
    }
    int value;
    std::vector<int> config_values;

    while (cfg_file >> value){
        config_values.push_back(value);
    }
    int num_col = config_values[0];
    int num_row = config_values[1];
    int num_mine = config_values[2];

    int width = num_col*32;
    int height = (num_row*32) +100;


    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");

    sf::Font font;
    font.loadFromFile("font.ttf");
    if(!font.loadFromFile("font.ttf")){
        std::cout << "error" << std::endl;
    }

    sf::Text welcome_text;

    welcome_text.setString("WELCOME TO MINESWEEPER!");
    welcome_text.setFont(font);
    welcome_text.setCharacterSize(24);
    welcome_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcome_text.setFillColor(sf::Color::White);
    setText(welcome_text,width/2, height/2-150);



    sf::Text enter_name;

    enter_name.setString("Enter your name:");
    enter_name.setFont(font);
    enter_name.setCharacterSize(20);
    enter_name.setStyle(sf::Text::Bold);
    enter_name.setFillColor(sf::Color::White);
    setText(enter_name,width/2, height/2-75);

    sf::Text user_name;

    user_name.setString("|");
    user_name.setFont(font);
    user_name.setCharacterSize(20);
    user_name.setStyle(sf::Text::Bold);
    user_name.setFillColor(sf::Color::White);






    std::string temp = "";
    bool stargame = false;
    vector<int>digitss;
    if(num_mine>10){
        digitss = {0};
    }
    else{
         digitss={0,0};
    }


    // Convert the number to a string
    int number = num_mine;
    std::string numberString = std::to_string(number);
    // Process each digit from left to right
    for (char digit : numberString) {
        // Convert char to integer
        int digitValue = digit - '0';
        // Print or process the digit as needed
        digitss.push_back(digitValue);
    }

    auto original = digitss;



    while(window.isOpen()) {
        sf::Event event;


        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered)
            {
                if(user_name.getString().getSize() < 11){
                    if (std::isalpha(event.text.unicode)){
                        if(user_name.getString().getSize()==1){
                            temp = temp + static_cast<char>(std::toupper(event.text.unicode));
                            user_name.setString(temp+"|");

                        }
                        else if(user_name.getString().getSize()>1) {
                            temp = temp + static_cast<char>(std::tolower(event.text.unicode));
                            user_name.setString(temp + "|");
                        }
                    }
                }
            }

            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::BackSpace){

                if (user_name.getString().getSize() != 1){
                    temp.erase(temp.size()-1,1);
                    user_name.setString(temp+"|");
                }
            }
                else if (event.key.code == sf::Keyboard::Enter){
                    if (user_name.getString().getSize() > 1){

                        stargame = true;
                        window.close();
                    }
                }
            }
        }
        setText(user_name,width/2, height/2-45);
        window.clear(sf::Color::Blue);
        window.draw(welcome_text);
        window.draw(enter_name);
        window.draw(user_name);
        window.display();
    }

    //Second window
    sf::RenderWindow game_window(sf::VideoMode(width,height), "Minesweeper");



    sf::Texture text_face_happy = TextureManager::getTexture("face_happy");
    sf::Sprite face_happy;
    face_happy.setTexture(text_face_happy);
    face_happy.setPosition((num_col/2.0)*32-32,32*(num_row+0.5));

    sf::Texture text_debug = TextureManager::getTexture("debug");
    sf::Sprite debug;
    debug.setTexture(text_debug);
    debug.setPosition((num_col*32)-304,32*(num_row+0.5));


    sf::Texture text_play_button = TextureManager::getTexture("play");
    sf::Sprite play_button;
    play_button.setTexture(text_play_button);
    play_button.setPosition((num_col*32)-240,32*(num_row+0.5));

    sf::Texture text_leaderboard = TextureManager::getTexture("leaderboard");
    sf::Sprite leaderboard;
    leaderboard.setTexture(text_leaderboard);
    leaderboard.setPosition((num_col*32)-176,32*(num_row+0.5));


    sf::Texture text_digits = TextureManager::getTexture("digits");




    sf::Texture text_tiles_hidden = TextureManager::getTexture("tile_hidden");
    vector<sf::Sprite> hiddentiles;
    for(int i =0; i < num_col; i++){
        for(int j = 0; j< num_row; j++){
            sf::Sprite atile;
            atile.setTexture(text_tiles_hidden);
            atile.setPosition(32*i,32*j);
            hiddentiles.push_back(atile);
        }
    }
    sf::Texture text_tiles_revealed = TextureManager::getTexture("tile_revealed");
    vector<sf::Sprite> tile_revealed;
    for(int i =0; i < num_col; i++){
        for(int j = 0; j< num_row; j++){
            sf::Sprite atile;
            atile.setTexture(text_tiles_hidden);
            atile.setPosition(32*i,32*j);
            tile_revealed.push_back(atile);
        }
    }


    sf::Texture text_flag = TextureManager::getTexture("flag");




    sf::Texture text_mine = TextureManager::getTexture("mine");
    sf::Texture number_1 = TextureManager::getTexture("number_1");
    sf::Texture number_2 = TextureManager::getTexture("number_2");
    sf::Texture number_3 = TextureManager::getTexture("number_3");
    sf::Texture number_4 = TextureManager::getTexture("number_4");
    sf::Texture number_5 = TextureManager::getTexture("number_5");
    sf::Texture number_6 = TextureManager::getTexture("number_6");
    sf::Texture number_7 = TextureManager::getTexture("number_7");
    sf::Texture number_8 = TextureManager::getTexture("number_8");

    sf::Texture text_pause = TextureManager::getTexture("pause");
    sf::Sprite pause_button;
    pause_button.setTexture(text_pause);
    pause_button.setPosition((num_col*32)-240,32*(num_row+0.5));


    sf::Texture text_face_lose = TextureManager::getTexture("face_lose");
    sf::Sprite face_lose;
    face_lose.setTexture(text_face_lose);
    face_lose.setPosition((num_col/2.0)*32-32,32*(num_row+0.5));


    sf::Texture text_face_win = TextureManager::getTexture("face_win");
    sf::Sprite face_win;
    face_win.setTexture(text_face_win);
    face_win.setPosition((num_col/2.0)*32-32,32*(num_row+0.5));



    game_board myboard(num_row, num_col);
    generate_mines(myboard,num_mine, num_row, num_col);
    myboard.adjacent();
    myboard.find_adjacent(num_row,num_col);

    std::ifstream inputFile("Leaderboard.txt");


    bool game_ends;
    bool debug_method = false;
    bool erase = true;
    bool paused = false;
    auto start_time = chrono::high_resolution_clock::now();
    int number_mines;
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now()-pauseTime).count();
    bool winner = false;
    bool show_leaderboard =false;
    sf::RenderWindow leader_window;
    bool victory = false;

    while(game_window.isOpen()) {
        sf::Event event;
        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {

                game_window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Get the position of the click
                    sf::Vector2i clickPosition = sf::Mouse::getPosition(game_window);


                    int position_x = clickPosition.x;
                    int position_y = clickPosition.y;
                    int max_y = num_row * 32;
                    int max_x = num_col * 32;

                    int col = 0;
                    int row = 0;


                    if (position_x < max_x and position_y < max_y) {
                        while (position_x > 32) {
                            position_x -= 32;
                            col += 1;
                        }

                        while (position_y > 32) {
                            position_y -= 32;
                            row += 1;
                        }
                        if (game_ends == false and myboard.checking_flags(col,row) == false and paused == false and victory == false) {
                            myboard.revealTiles(col, row, num_col, num_row);
                            game_ends = myboard.check_mine(col, row);
                        }

                    }
                    int debug_pos_x = (num_col * 32) - 304;
                    int debug_pos_y = 32 * (num_row + 0.5);
                    if (position_x > debug_pos_x and position_x < debug_pos_x + 64 and position_y > debug_pos_y and
                        position_y < debug_pos_y + 64 and game_ends == false and victory == false) {
                        if (debug_method == false) {
                            debug_method = true;
                        } else {
                            debug_method = false;
                        }
                    }
                    int happy_face_x = (num_col / 2.0) * 32 - 32;
                    int happy_face_y = 32 * (num_row + 0.5);
                    if (position_x > happy_face_x and position_x < happy_face_x + 64 and position_y > happy_face_y and
                        position_y < happy_face_y + 64) {
                        erase_mines(myboard);
                        generate_mines(myboard, num_mine, num_row, num_col);
                        myboard.adjacent();
                        myboard.find_adjacent(num_row, num_col);
                        myboard.setting_hidden_tiles();
                        myboard.setting_flags();
                        digitss = original;
                        game_ends = false;
                        paused= true;
                        victory = false;
                    }
                    int pause_buttom_x = (num_col * 32) - 240;
                    int pause_buttom_y = 32 * (num_row + 0.5);
                    if (position_x > pause_buttom_x and position_x < pause_buttom_x + 64 and
                        position_y > pause_buttom_y and position_y < pause_buttom_y + 64 and game_ends == false and victory == false) {
                        if (paused == false) {
                            paused = true;
                            pauseTime = chrono::high_resolution_clock::now();
                        } else {
                            auto unPausedTimed = chrono::steady_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                    unPausedTimed - pauseTime).count();
                            paused = false;
                        }
                    }
                    int leaderboard_x = (num_col * 32) - 176;
                    int leaderboard_y = 32 * (num_row + 0.5);
                    if (position_x > leaderboard_x and position_x < leaderboard_x + 64 and
                        position_y > leaderboard_y and position_y < leaderboard_y + 64) {
                        show_leaderboard = true;
                        paused = true;
                        stargame = false;
                    }
                }
                //Printing flags
                if (event.mouseButton.button == sf::Mouse::Right) {
                    // Get the position of the click
                    sf::Vector2i clickPosition = sf::Mouse::getPosition(game_window);

                    // Display the position in the console

                    int position_x = clickPosition.x;
                    int position_y = clickPosition.y;
                    int max_y = num_row * 32;
                    int max_x = num_col * 32;

                    int col = 0;
                    int row = 0;


                    if (position_x < max_x or position_y < max_y) {
                        while (position_x > 32) {
                            position_x -= 32;
                            col += 1;
                        }

                        while (position_y > 32) {
                            position_y -= 32;
                            row += 1;
                        }

                        if (myboard.check_tile(col, row) == true) {
                            change_counter(false, digitss);
                            myboard.set_flag(col, row);

                        } else {
                            myboard.set_flag(col, row);
                            change_counter(true, digitss);

                        }
                    }
                }
            }
        }
        vector<Person> people;

            int minutes;
            int seconds;

            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now()-start_time);
            int total_time = game_duration.count();

            if(!paused) {
                total_time = total_time - elapsed_paused_time;
                minutes = total_time / 60;
                seconds = total_time % 60;
            }


            int minutes0 = minutes / 10 % 10; //minutes index 0
            int minutes1 = minutes % 10; // minutes index 1
            int seconds0 = seconds / 10 % 10; // seconds index 0
            int seconds1 = seconds % 10; // seconds index 1


        game_window.clear(sf::Color::White);
            if(paused == false){
                game_window.draw(play_button);
            }
            else {
                game_window.draw(pause_button);
            }

            drawing_timer(text_digits, game_window, seconds0, seconds1, minutes0, minutes1, num_row, num_col);

            game_window.draw(debug);
            game_window.draw(leaderboard);
            for (int i = 0; i < hiddentiles.size(); i++) {
                game_window.draw(hiddentiles[i]);
            }
            myboard.drawing_flags(game_window, text_flag, text_tiles_hidden);
            myboard.reveal(game_window,text_tiles_revealed);
            winner = myboard.check_winner();

            if (game_ends == true){
                myboard.reveal_all_tiles(num_col, num_row,game_window,text_tiles_revealed);
                myboard.revealing_mines(game_window, text_mine);
                game_window.draw(face_lose);
                paused =true;
                }

            else if (debug_method == true){
                myboard.revealing_mines(game_window, text_mine);
                game_window.draw(face_happy);

            }
            else if (winner == true){
                game_window.draw(face_win);
                myboard.set_flag_with_mines();
                myboard.drawing_flags_final_flags(game_window, text_flag);
                victory = true;
                paused=true;


            }
            else{
                game_window.draw(face_happy);
            }
            myboard.print_numbers(game_window,number_1,number_2,number_3,
                                  number_4,number_5,number_6,number_7,number_8);

            drawing_counter(myboard,text_digits,game_window,num_row,digitss);
            //Leaderboard window
            if(show_leaderboard == true){
                show_leaderboard= false;
                if(winner == true){
                    ofstream outputfile("Leaderboard.txt", std::ios::app);
                    outputfile <<  minutes0 << minutes1 << ":" << seconds0 << seconds1 << "," << temp << endl;

                }

                leader_window.create(sf::VideoMode(num_col*16,num_row*16 +50), "Minesweeper");
                sf::Text leader_text;
                leader_text.setString("LEADERBOARD");
                leader_text.setFont(font);
                leader_text.setCharacterSize(20);
                leader_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                leader_text.setFillColor(sf::Color::White);
                setText(leader_text,(num_col*16)/2, (num_row*16 +50)/2-120);


                sf::Event aevent;
                ifstream inputFile("Leaderboard.txt");
                string line;
                int counter = 1;
                string show_values = "";
                while (std::getline(inputFile, line)) {
                    // Use a stringstream to tokenize the line based on commas
                    std::stringstream ss(line);
                    std::string time;
                    std::string name;
                    std::string minutes;
                    std::string seconds;

                    std::getline(ss, minutes, ':');

                    std::getline(ss, seconds, ',');


                    std::getline(ss, name, ',');


                    Person person;
                    person.name = name;
                    person.minutes = stoi(minutes);
                    person.seconds = stoi(seconds);
                    people.push_back(person);

                }
                std::sort(people.begin(), people.end(), compareTime);

                int lenght;

                if(people.size()<5){
                    lenght = people.size();
                }
                else{
                    lenght = 5;
                }

                for(int i = 0; i < lenght ;i++){
                    string min;
                    string sec;
                    if(people[i].minutes == 0){
                        min = "0"+std::to_string(people[i].minutes);
                    }
                    else{
                        min = std::to_string(people[i].minutes);
                    }
                    if(people[i].seconds <10){
                        sec = "0"+std::to_string(people[i].seconds);
                    }
                    else{
                        sec = std::to_string(people[i].seconds);
                    }

                    show_values += std::to_string(i+1) + ".\t" + min  +":"+  sec + "\t" + people[i].name + "\n\n";

                }

                sf::Text user;
                user.setString(show_values);
                user.setFont(font);
                user.setCharacterSize(18);
                user.setFillColor(sf::Color::White);
                user.setStyle(sf::Text::Bold);
                setText(user,(num_col*16)/2, (num_row*16 +50)/2+20);

                while(leader_window.isOpen()) {
                    while (leader_window.pollEvent(aevent)) {
                        if (aevent.type == sf::Event::Closed) {
                            leader_window.close();
                        }
                    }
                    leader_window.clear(sf::Color::Blue);
                    leader_window.draw(leader_text);
                    leader_window.draw(user);
                    leader_window.display();
                }
            }
            game_window.display();
    }
    return 0;
}
