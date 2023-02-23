#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

class DefensiveHeuristic: public StateHeuristic {
  public:
   DefensiveHeuristic();
   DefensiveHeuristic(GameState state, int playerID);
   double evaluateGameState(
      const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
   std::string getName() const override { return "DefensiveHeuristic"; }

  protected:

   //Weights (Static)
   double opponentArmySizeMOD = 0.25;
   double playerArmySizeMOD = 1;
   double playerArmyHealthMOD = 1;
   double opponentArmyHealthMOD = 0.25;
   double playerKingHPMOD = 1;
   double opponentKingHPMOD = 0.25;
   double meanDistanceMOD = 0.25;
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
}  // namespace SGA