#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

    class AggresiveHeuristic : public StateHeuristic {
      public:
       AggresiveHeuristic();
       AggresiveHeuristic(GameState state, int playerID);
       double evaluateGameState(
          const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
       std::string getName() const override { return "AggresiveHeuristic"; }

      protected:

       //Weights (Static)
       double opponentArmySizeMOD = 1;
       double playerArmySizeMOD = 0.25;
       double playerArmyHealthMOD = 0.25;
       double opponentArmyHealthMOD = 1;
       double playerKingHPMOD = 0.25;
       double opponentKingHPMOD = 1;
       double meanDistanceMOD = 1;
       double supportUnitsinRangeMOD = 0.5;

       // Upper Bounds
       double opponentArmySizeUB;
       double playerArmySizeUB;
       double playerArmyHealthUB;
       double opponentArmyHealthUB;
       double playerKingHPUB;
       double opponentKingHPUB;
       double meanDistanceUB;
       double supportUnitsinRangeUB;
    };
}