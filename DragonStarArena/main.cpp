// ================================================================
//
// Dragon Star Arena Project
// v0.1.0
//
// ================================================================

#include <memory>
#include "core/game.hpp"
#include "data/gameData.hpp"

int main() {
    gameData.LoadData();
    
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->Run();

    return 0;
}