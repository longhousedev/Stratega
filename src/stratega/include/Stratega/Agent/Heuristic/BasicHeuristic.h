#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

class BasicHeuristic: public StateHeuristic {
  public:
   BasicHeuristic();
   BasicHeuristic(GameState state);
   double evaluateGameState(
      const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
   std::string getName() const override { return "BasicHeuristic"; }
};
}  // namespace SGA