//
// Created by George Long on 29/09/2022.
//
#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

   class WizardWarsHeuristic : public StateHeuristic {
     public:
      int armySizes[2] = {0, 0};
      double armyHealth[2] = {0, 0};
      WizardWarsHeuristic();
      WizardWarsHeuristic(GameState state);
      double evaluateGameState(const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
      std::string getName() const override { return "WizardWarsHeuristic"; }
   };
}
