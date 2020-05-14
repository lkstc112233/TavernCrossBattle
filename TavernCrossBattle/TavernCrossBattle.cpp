#include <iostream>

#include "protocpp/Minion.pb.h"
#include "protocpp/Board.pb.h"

int main()
{
    battle::Board board; 
    std::cout << board.DebugString() << std::endl;
}
