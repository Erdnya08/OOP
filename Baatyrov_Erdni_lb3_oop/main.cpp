#include "segment.h"
#include "ships_manager.h"
#include "ship.h"
#include "playing_field.h"
#include "info_holder.h"
#include "skills_manager.h"
#include "out_of_field_exception.h"
#include "skills/scanner_creator.h"
#include "game_state.h"
#include "game.h"

#include <iostream>
#include <fstream>

using namespace std;


int main(){
    try{
        vector<short unsigned int> lens = {1, 2, 3, 4, 4};
        ShipsManager player_manager(5, lens);
        ShipsManager enemy_manager(5, lens);
        PlayingField playground_player(4, 4), playground_enemy(4, 4);
        auto playground_player_ptr = make_shared<PlayingField>(playground_player);
        auto player_manager_ptr = make_shared<ShipsManager>(player_manager);
        auto playground_enemy_ptr = make_shared<PlayingField>(playground_enemy);
        auto enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);
        Game game(playground_player_ptr, playground_enemy_ptr, player_manager_ptr, enemy_manager_ptr);
        game.start();
        //game.load("save1.txt");
        while (true){
            while (!game.isEnemyWin() && !game.isPlayerWin()){
                game.game_state.player_field->printFull();
                cout<<endl;
                cout<<*game.game_state.enemy_field;
                size_t x, y;
                cin>>x>>y;
                game.playerTurn(x, y);
                game.enemyTurn();
            }
            cout<<"Игрок выйграл: "<<game.isPlayerWin()<<endl;
            cout<<"Враг выйграл: "<<game.isEnemyWin()<<endl;
            game.save("save2.txt");
            if (game.isEnemyWin()){
                player_manager = ShipsManager(5, lens);
                enemy_manager = ShipsManager(5, lens);
                playground_player = PlayingField(4, 4);
                playground_enemy = PlayingField(4, 4);
                playground_player_ptr = make_shared<PlayingField>(playground_player);
                player_manager_ptr = make_shared<ShipsManager>(player_manager);
                playground_player_ptr->addShip(0, 0, player_manager_ptr->operator[](4), Ship::Orientation::VERTICAL);
                playground_enemy_ptr = make_shared<PlayingField>(playground_enemy);
                enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);
                playground_enemy_ptr->addShip(0, 0, enemy_manager_ptr->operator[](0), Ship::Orientation::HORIZONTAL);
                game.reload_game(playground_player_ptr, playground_enemy_ptr, player_manager_ptr, enemy_manager_ptr);
            }
            else{
                enemy_manager = ShipsManager(5, lens);
                playground_enemy = PlayingField(4, 4);
                playground_enemy_ptr = make_shared<PlayingField>(playground_enemy);
                enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);
                playground_enemy_ptr->addShip(0, 0, enemy_manager_ptr->operator[](0), Ship::Orientation::HORIZONTAL);
                game.reload_enemy(playground_enemy_ptr, enemy_manager_ptr);
            }
        }
    }
    catch(exception& e){
        cerr<<e.what()<<endl;
        return 1;
    }

    return 0;
}