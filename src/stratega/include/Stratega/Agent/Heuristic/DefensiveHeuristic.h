#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

class DefensiveHeuristic: public StateHeuristic {
  public:
   DefensiveHeuristic();
   DefensiveHeuristic(GameState state);
   double evaluateGameState(
      const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
   std::string getName() const override { return "DefensiveHeuristic"; }
};
}  // namespace SGA