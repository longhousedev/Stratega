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
