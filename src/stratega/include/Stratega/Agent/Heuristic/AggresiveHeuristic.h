#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

    class AggresiveHeuristic : public StateHeuristic {
      public:
       int armySizes[2] = {0, 0};
       double armyHealth[2] = {0, 0};
       AggresiveHeuristic();
       AggresiveHeuristic(GameState state);
       double evaluateGameState(
          const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
       std::string getName() const override { return "AggresiveHeuristic"; }
    };
}