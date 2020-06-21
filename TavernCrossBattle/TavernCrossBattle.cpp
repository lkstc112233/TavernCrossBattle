#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>

#include "Game.h"
#include "protocpp/Player.pb.h"

int main(int argc, char **argv)
{
    battle::Game game;
    if (argc < 3) {
        battle::Player player;
        player.add_minions()->set_power(3);
        game.Initialize(player, player);
    } else {
        battle::Player player1;
        battle::Player player2;
        bool retValue = false;

        std::ifstream player1_textproto(argv[1]);

        google::protobuf::io::IstreamInputStream player1_textproto_input(&player1_textproto);

        if (!google::protobuf::TextFormat::Parse(&player1_textproto_input, &player1)) {
            std::cerr << std::endl << "Failed to parse file!" << std::endl;
            return -1;
        }

        std::ifstream player2_textproto(argv[2]);

        google::protobuf::io::IstreamInputStream player2_textproto_input(&player2_textproto);
       
        if (!google::protobuf::TextFormat::Parse(&player2_textproto_input, &player2)) {
            std::cerr << std::endl << "Failed to parse file!" << std::endl;
            return -1;
        }
        game.Initialize(player1, player2 );
    }
    while (!game.IsGameOver()) {
        std::cout << game.GetBoardState().DebugString() << std::endl;
        game.LoopStep();
    }
    std::cout << "Game over: " << std::endl << game.GetBoardState().DebugString() << std::endl;
}
