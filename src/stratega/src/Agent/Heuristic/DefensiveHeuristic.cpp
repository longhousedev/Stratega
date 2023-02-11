#include <Stratega/Agent/Heuristic/DefensiveHeuristic.h>
#include <cmath>
#include <list>
#include <map>
#include <set>

namespace SGA {

double DefensiveHeuristic::evaluateGameState(
   const ForwardModel& /*forwardModel*/, GameState& gameState, int playerID)
{
   // Variables
   std::map< int, Vector2f > positions;
   std::list< Vector2f > supportPositions;
   double totalDistance = 0;
   int kingX = 0;
   int kingY = 0;
   double score = 0;

   // Modifiers
   std::set< int > opponentEntities = std::set< int >();
   std::set< int > playerEntities = std::set< int >();
   double playerArmyHealth = 0;
   double opponentArmyHealth = 0;
   double playerKingHP = 0;
   double opponentKingHP = 0;
   double meanDistance = 0;
   double supportUnitsinRange = 0;

   // Weights
   double opponentArmySizeMOD = 1;
   double playerArmySizeMOD = 4;
   double playerArmyHealthMOD = 0.4;
   double opponentArmyHealthMOD = 0.1;
   double playerKingHPMOD = 8;
   double opponentKingHPMOD = 1;
   double meanDistanceMOD = 0.5;
   double supportUnitsinRangeMOD = 1;

   // If playerID is not the current player
   if(playerID != gameState.getCurrentTBSPlayer()) {
      return -1000000000000;
   }

   // Win check
   if(gameState.isGameOver()) {
      if(gameState.getWinnerID() == playerID)
         return 10000000000000;
      else
         return -1000000000000;
   }

   // Register Entities
   for(const auto& entity : gameState.getEntities()) {
      positions.emplace(entity.getID(), entity.getPosition());

      if(entity.getOwnerID() != gameState.getCurrentTBSPlayer()) {
         opponentEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());

         // Get Enemy King pos and health
         if(entityType.getName() == "King") {
            kingX = static_cast< int >(entity.x());
            kingY = static_cast< int >(entity.y());
            opponentKingHP = entity.getParameter("Health");
         }
         // Support Units
         else if(entityType.getName() == "Druid" || entityType.getName() == "Engineer") {
            supportPositions.push_front(entity.getPosition());
         } else {
            opponentArmyHealth += entity.getParameter("Health");
         }

      } else if(entity.getOwnerID() == gameState.getCurrentTBSPlayer()) {
         playerEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            playerKingHP = entity.getParameter("Health");
         } else {
            playerArmyHealth += entity.getParameter("Health");
         }
      }
   }

   // Gets the total distance of player units (apart from king!) and enemy king
   for(const auto& entity : playerEntities) {
      std::string name = gameState.getGameInfo()
                            ->getEntityType(gameState.getEntity(entity)->getEntityTypeID())
                            .getName();

      if(name != "Druid" || name != "Engineer") {
         // Check how many units are in range of a support
         for(Vector2f supportPosition : supportPositions) {
            double distance = supportPosition.manhattanDistance(positions[entity]);
            if(distance <= 3)
               supportUnitsinRange += 1;
            break;
         }
      }

      // Checks the distance of offensive units from the opposing king
      if(name != "King" || name != "Druid" || name != "Engineer") {
         // Manhattan Method for distance
         double distance = (abs(positions[entity].x - kingX) + abs(positions[entity].y - kingY))
                           - gameState.getEntity(entity)->getParameter("AttackRange");
         totalDistance += distance;
      }
   }

   // Negative modifiers
   score -= meanDistance * meanDistanceMOD;
   score -= opponentKingHP * opponentKingHPMOD;
   score -= opponentArmyHealth * opponentArmyHealthMOD;
   score -= opponentEntities.size() * opponentArmySizeMOD;

   // Positive modifiers
   score += playerArmyHealth * playerArmyHealthMOD;
   score += playerKingHP * playerKingHPMOD;
   score += playerEntities.size() * playerArmySizeMOD;
   score += supportUnitsinRange * supportUnitsinRangeMOD;

   return score *= 1;
}
DefensiveHeuristic::DefensiveHeuristic(GameState state)
{
   int playerID = state.getCurrentTBSPlayer();
   int test = 1;
}
DefensiveHeuristic::DefensiveHeuristic() {}
}  // namespace SGA