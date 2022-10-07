#pragma once
#include <Stratega/Agent/ActionScripts/BaseActionScript.h>
#include <map>


namespace SGA {
class AttackKingScript : public BaseActionScript
{
  private:
   double getHealth(const Entity& entity, const GameState& gamestate) const;


  public:
   AttackKingScript() : BaseActionScript() {};

   Action getAction(const GameState& gameState, std::vector<Action>& actionSpace, int playerID) const override;
   Action getActionForUnit(const GameState& gameState, std::vector<Action>& actionSpace, int playerID, int unitID) const override;
   std::string toString() const override { return "AttackKingScript"; };

};

}