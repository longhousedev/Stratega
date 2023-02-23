//
// Created by George Long on 29/09/2022.
//
#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

   class WizardWarsHeuristic : public StateHeuristic {
     public:
      WizardWarsHeuristic();
      WizardWarsHeuristic(GameState state, int playerID);
      double evaluateGameState(const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
      std::string getName() const override { return "WizardWarsHeuristic"; }

     protected:

      //Weights (Static)
      double opponentArmySizeMOD = 0.5;
      double playerArmySizeMOD = 0.5;
      double playerArmyHealthMOD = 0.5;
      double opponentArmyHealthMOD = 0.5;
      double playerKingHPMOD = 0.5;
      double opponentKingHPMOD = 0.5;
      double meanDistanceMOD = 0.5;
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
