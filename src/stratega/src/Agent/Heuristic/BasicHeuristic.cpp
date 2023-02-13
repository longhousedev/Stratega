#include <Stratega/Agent/Heuristic/BasicHeuristic.h>
#include <cmath>
#include <list>
#include <map>
#include <set>

namespace SGA {

BasicHeuristic::BasicHeuristic() {}

BasicHeuristic::BasicHeuristic(GameState state) {}

double BasicHeuristic::evaluateGameState(
   const ForwardModel& forwardModel, GameState& gameState, const int playerID)
{
   if(gameState.isGameOver()) {
      if(gameState.getWinnerID() == -1)
         return 0.5;
      if(gameState.getWinnerID() == playerID)
         return 1;
      else
         return 0;
   }

   int f = 1;
   return 0;
}
}  // namespace SGA