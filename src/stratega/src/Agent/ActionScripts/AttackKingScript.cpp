#include <Stratega/Agent/ActionScripts/AttackKingScript.h>
#include <Stratega/ForwardModel/ForwardModel.h>
#include <set>
#pragma warning(disable: 5045)

namespace SGA {

   double AttackKingScript::getHealth(const Entity& entity, const GameState& gamestate) const
   {
      for (const auto& param : (gamestate.getGameInfo()->getEntityTypes()).at(entity.getEntityTypeID()).getParameters())
      {
         if (param.second.getName() == "Health")
         {
            auto it = entity.getParamValues().find(param.second.getIndex());
            if(it != entity.getParamValues().end())
               return it->second;
         }
      }
      return 0;
   }

   Action AttackKingScript::getAction(const GameState& gameState, std::vector<Action>& actionSpace, int playerID) const
   {
      int kingID = -1;
      Vector2f kingPos;

      if (actionSpace.size() > 1)
      {
         // create a map of action types to filter relevant actions
         std::map<int, std::string> actionTypeIDToActionTypeString;

         for (const auto& a : gameState.getGameInfo()->getActionTypes())
         {
            actionTypeIDToActionTypeString[a.first] = a.second.getName();
         }
         actionTypeIDToActionTypeString[-1] = "EndTurn";

         // Get enemy king
         for (auto& entity : gameState.getEntities()) {
            if (entity.getOwnerID() != gameState.getCurrentTBSPlayer()
               && gameState.getGameInfo()->getEntityType(entity.getEntityTypeID()).getName() == "King")
            {
               kingID = entity.getID();
               kingPos = entity.getPosition();
            }
         }

         //prioritize attacks, if at least one attack action targeting an opponent entity is available, return the attack action to the closest opponent
         auto subActions = filterActionTypes(actionSpace, "Attack", actionTypeIDToActionTypeString);
         if (!subActions.empty())
         {
            int bestAction = -1;
            for (size_t i = 0; i < subActions.size(); i++)
            {
               auto& action = subActions.at(i);
               const bool contains = action.getTargets()[1].getEntityID() == kingID;

               if (contains)
               //if (contains(opponentUnits, action.getTargets()[1].getEntityID()))
               //if (opponentUnits.contains(action.getTargets()[1].getEntityID()))
               {
                  bestAction = static_cast<int>(i);
               }
            }
            if (bestAction != -1)
               return subActions[static_cast<size_t>(bestAction)];
         }

         //else, try to move closer to the weakest opponent
         subActions = filterActionTypes(actionSpace, "Move", actionTypeIDToActionTypeString);
         if (!subActions.empty())
         {
            int bestAction = -1;
            double actionDistance = std::numeric_limits<double>::max();
            for (size_t i = 0; i < subActions.size(); i++)
            {
               auto& action = subActions.at(i);
               const double dist = action.getTargets()[1].getPosition(gameState).manhattanDistance(kingPos);
               if (dist < actionDistance)
               {
                  actionDistance = dist;
                  bestAction = static_cast<int>(i);
               }
            }

            if (bestAction != -1)
               return subActions[static_cast<size_t>(bestAction)];
         }
      }

      return actionSpace[actionSpace.size() - 1];
   }

   Action AttackKingScript::getActionForUnit(const GameState& gameState, std::vector<Action>& actionSpace, int playerID, int unitID) const
   {
      int kingID;
      Vector2f kingPos;
      std::vector<Action> suitableActions;
      for (const auto& action : actionSpace)
      {
         if (action.getTargets()[0].getEntityID() == unitID)
         {
            suitableActions.push_back(action);
         }
      }

      if (suitableActions.size() > 1)
      {
         // create a map of action types to filter relevant actions
         std::map<int, std::string> actionTypeIDToActionTypeString;
         for (const auto& a : gameState.getGameInfo()->getActionTypes())
         {
            actionTypeIDToActionTypeString[a.first] = a.second.getName();
         }
         actionTypeIDToActionTypeString[-1] = "EndTurn";


         for (auto& entity : gameState.getEntities()) {
            if (entity.getOwnerID() != gameState.getCurrentTBSPlayer()
               && gameState.getGameInfo()->getEntityType(entity.getEntityTypeID()).getName() == "King")
            {
               kingID = entity.getID();
               kingPos = entity.getPosition();
            }
         }


         //prioritize attacks, if at least one attack action targeting an opponent entity is available, return the attack action to the closest opponent
         double minimalTargetHealthPoints = std::numeric_limits<double>::max();
         auto subActions = filterActionTypes(suitableActions, "Attack", actionTypeIDToActionTypeString);
         if (!subActions.empty())
         {
            int bestAction = -1;
            for (size_t i = 0; i < subActions.size(); i++)
            {
               auto& action = subActions.at(i);

               const bool contains = action.getTargets()[1].getEntityID() == kingID;

               if (contains)
               //if (contains(opponentUnits, action.getTargets()[1].getEntityID()))
               //if (opponentUnits.contains(action.getTargets()[1].getEntityID()))
               {
                  bestAction = static_cast<int>(i);
               }
            }
            if (bestAction != -1)
               return subActions[static_cast<size_t>(bestAction)];
         }

         //else, try to move closer to the weakest opponent
         subActions = filterActionTypes(suitableActions, "Move", actionTypeIDToActionTypeString);
         if (!subActions.empty())
         {
            int bestAction = -1;
            double actionDistance = std::numeric_limits<double>::max();
            for (size_t i = 0; i < subActions.size(); i++)
            {
               auto& action = subActions.at(i);
               const double dist = action.getTargets()[1].getPosition(gameState).manhattanDistance(kingPos);
               if (dist < actionDistance)
               {
                  actionDistance = dist;
                  bestAction = static_cast<int>(i);
               }
            }

            if (bestAction != -1)
               return subActions[static_cast<size_t>(bestAction)];
         }
      }
      return actionSpace[static_cast<size_t>(static_cast<int>(actionSpace.size())-1)];
   }
}


