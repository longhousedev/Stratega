//
// Created by George Long on 29/09/2022.
//

#include <Stratega/Agent/Heuristic/WizardWarsHeuristic.h>
#include <map>
#include <set>

namespace SGA {

double WizardWarsHeuristic::evaluateGameState(
   const ForwardModel& /*forwardModel*/, GameState& gameState, int playerID)
{
   double score = 0.0;

   std::map< int, Vector2f > positions;
   std::set< int > opponentEntities = std::set< int >();
   std::set< int > playerEntities = std::set< int >();

   // King Info
   double allyKingHP = 20, enemyKingHP = 20;
   int kingX = -1, kingY = -1;
   double totalDistance = 0.0, meanDistance = 0.0;

   // Army Info
   int allyArmySize = 0;
   int enemyArmySize = 0;
   double allyArmyHealth = 0;
   double enemyArmyHealth = 0;

   // Get King HP
   for(const auto& entity : gameState.getEntities()) {
      positions.emplace(entity.getID(), entity.getPosition());
      if(entity.getOwnerID() != gameState.getCurrentTBSPlayer()) {
         opponentEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            kingX = static_cast< int >(entity.x());
            kingY = static_cast< int >(entity.y());
            enemyKingHP = entity.getParameter("Health");
         }
      } else {
         playerEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            allyKingHP = entity.getParameter("Health");
         }
      }
   }

   // Ally Army
   for(const auto& entity : playerEntities) {
      if((gameState.getGameInfo()
             ->getEntityType(gameState.getEntity(entity)->getEntityTypeID())
             .getName()
          != "King")) {
         totalDistance += abs(positions[entity].x - kingX) + abs(positions[entity].y - kingY);
      }
      allyArmySize++;
      allyArmyHealth = gameState.getEntity(entity)->getParameter("Health");
   }

   // Enemy Army
   for(const auto& entity : opponentEntities) {
      enemyArmySize++;
      enemyArmyHealth = gameState.getEntity(entity)->getParameter("Health");
   }

   meanDistance = totalDistance / static_cast< double >(playerEntities.size());

   if(gameState.isGameOver()) {
      if(gameState.getWinnerID() == playerID)
         return 10000000000000;  // since this score should be minimized we need to deduct points
                                 // for winning
      else
         return -10000000000000;
   }

   // Looks at damage dealt / taken
   int allyUnitsKilled = armySizes[playerID] - allyArmySize;
   double allyDamageTaken = armyHealth[playerID] - allyArmyHealth;
   int enemyUnitsKilled = armySizes[! playerID] - enemyArmySize;
   double enemyDamageTaken = armyHealth[! playerID] - enemyArmyHealth;

   double maximum_distance = 10;
   score += (1 / (meanDistance + 1))
            * 20;  // set to the mean of distance of our units to opponent units, divided by a
                   // constant 50 to make it slower than the gameOver score

   // Score to encourage units to move towards the enemy king
   score -= meanDistance;

   // Ally to enemy unit ratio, helps to keep your units alive
   // double unitRatio = (allyUnitsLeft +1 ) / (enemyUnitsLeft + 1);
   // double damageBalance = enemyDamageTaken - allyDamageTaken;

   // score += unitRatio * enemyDamageTaken * 4;
   // score -= ((enemyUnitsLeft + 1) / (allyUnitsLeft + 1)) * allyDamageTaken * 0.25;

   score += enemyDamageTaken / (allyUnitsKilled + 1) * 4;
   score -= allyDamageTaken / (enemyUnitsKilled + 1) * 0.25;

   // King Damage, Most Important
   score += allyKingHP * 0.25;

   // HARDCODED
   score += (1 / (enemyKingHP + 1)) * 50;

   return score;
}
WizardWarsHeuristic::WizardWarsHeuristic(GameState state)
{
   for(const auto& entity : state.getEntities()) {
      if(entity.getOwnerID() == 0) {
         armySizes[0]++;
         armyHealth[0] += entity.getParameter("Health");
      } else if(entity.getOwnerID() == 1) {
         armySizes[1]++;
         armyHealth[1] += entity.getParameter("Health");
      }
   }
}
WizardWarsHeuristic::WizardWarsHeuristic() {}
}  // namespace SGA
