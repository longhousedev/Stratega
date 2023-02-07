//
// Created by George Long on 29/09/2022.
//
#pragma once
#include <Stratega/Agent/Heuristic/StateHeuristic.h>

namespace SGA {

   class WizardWarsHeuristic : public StateHeuristic {
     public:
      WizardWarsHeuristic();
      WizardWarsHeuristic(GameState state);
      double evaluateGameState(const ForwardModel& forwardModel, GameState& gameState, const int playerID) override;
      std::string getName() const override { return "WizardWarsHeuristic"; }
   };
}
