#include <iostream>

#include "Game.h"
#include "protocpp/Player.pb.h"

int main()
{
    battle::Game game;
    battle::Player player;
    player.add_minions()->set_power(3);
    game.Initialize(player, player);
    std::cout << game.GetBoardState().DebugString() << std::endl;
}
