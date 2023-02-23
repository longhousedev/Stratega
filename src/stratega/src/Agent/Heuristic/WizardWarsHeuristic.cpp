//
// Created by George Long on 29/09/2022.
//

#include <Stratega/Agent/Heuristic/WizardWarsHeuristic.h>
#include <list>
#include <map>
#include <set>

namespace SGA {

double WizardWarsHeuristic::evaluateGameState(
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

   // Win check
   if(gameState.isGameOver()) {
      if(gameState.getWinnerID() == playerID)
         return 1000;
      else
         return -1000;
   }

   // Register Entities
   for(const auto& entity : gameState.getEntities()) {
      positions.emplace(entity.getID(), entity.getPosition());

      if(entity.getOwnerID() != playerID) {
         opponentEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());

         // Get Enemy King pos and health
         if(entityType.getName() == "King") {
            kingX = static_cast< int >(entity.x());
            kingY = static_cast< int >(entity.y());
            opponentKingHP = entity.getParameter("Health");
         }
         else {
            opponentArmyHealth += entity.getParameter("Health");
         }

      } else if(entity.getOwnerID() == playerID) {
         playerEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            playerKingHP = entity.getParameter("Health");
         }
         // Support Units
         else if(entityType.getName() == "Druid" || entityType.getName() == "Engineer") {
            supportPositions.push_front(entity.getPosition());
            playerArmyHealth += entity.getParameter("Health");

         }
         else {
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
            if(distance <= 3) {
               supportUnitsinRange += 1;
               break;
            }
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

   
   meanDistance = totalDistance / static_cast< double >(playerEntities.size());

   // Negative modifiers
   score += ((meanDistanceUB - meanDistance) / meanDistanceUB) * meanDistanceMOD;
   score += ((opponentKingHPUB - opponentKingHP) / opponentKingHPUB) * opponentKingHPMOD;
   score += ((opponentArmyHealthUB - opponentArmyHealth) / opponentArmyHealthUB) * opponentArmyHealthMOD;
   score += ((opponentArmySizeUB - static_cast<double>(opponentEntities.size())) / opponentArmySizeUB) * opponentArmySizeMOD;

   // Positive modifiers
   score += (playerArmyHealth / playerArmyHealthUB) * playerArmyHealthMOD;
   score += (playerKingHP  / playerKingHPUB) * playerKingHPMOD;
   score += (static_cast<double>(playerEntities.size()) / playerArmySizeUB) * playerArmySizeMOD;
   score += (supportUnitsinRange / supportUnitsinRangeUB) * supportUnitsinRangeMOD;

   score /= 8;
   return score;
}
WizardWarsHeuristic::WizardWarsHeuristic(GameState gameState, int playerID) :
      opponentArmySizeUB(0), playerArmySizeUB(0), playerArmyHealthUB(0), opponentArmyHealthUB(0),
      playerKingHPUB(0), opponentKingHPUB(0), meanDistanceUB(20), supportUnitsinRangeUB(0)
{

   //MEAN DISTANCE UB IN LIST INIT

   // Variables
   std::set< int > opponentEntities = std::set< int >();
   std::set< int > playerEntities = std::set< int >();
   std::map< int, Vector2f > positions;
   std::list< Vector2f > supportPositions;

   // Register Entities
   for(const auto& entity : gameState.getEntities()) {
      positions.emplace(entity.getID(), entity.getPosition());

      if(entity.getOwnerID() != playerID) {
         opponentEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());

         // Get Enemy King pos and health
         if(entityType.getName() == "King") {
            opponentKingHPUB = entity.getParameter("Health");
         }
         else {
            opponentArmyHealthUB += entity.getParameter("Health");
         }

      } else if(entity.getOwnerID() == playerID) {
         playerEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            playerKingHPUB = entity.getParameter("Health");
         }
         // Support Units
         else if(entityType.getName() == "Druid" || entityType.getName() == "Engineer") {
            supportPositions.push_front(entity.getPosition());
            playerArmyHealthUB += entity.getParameter("Health");
         }
         else
         {
            playerArmyHealthUB += entity.getParameter("Health");
         }
      }
   }

   //Army size and support units in range

   //TODO Include king in army or not?
   playerArmySizeUB = static_cast< double >(playerEntities.size());
   opponentArmySizeUB = static_cast< double >(opponentEntities.size());
   supportUnitsinRangeUB = static_cast< double >(playerEntities.size() - supportPositions.size());


}


WizardWarsHeuristic::WizardWarsHeuristic() {}
}  // namespace SGA
