#include <Stratega/Agent/Heuristic/AggresiveHeuristic.h>
#include <map>
#include <set>
#include <cmath>

namespace SGA {

double AggresiveHeuristic::evaluateGameState(
   const ForwardModel& /*forwardModel*/, GameState& gameState, int playerID)
{
   // Find Strongest Unit
   // Make Strongest Unit Attack King
   double score = 0;
   if (playerID != gameState.getCurrentTBSPlayer()) {
      return -1000000000000;
   }

   if(gameState.isGameOver()) {
   if(gameState.getWinnerID() == playerID)
      return 10000000000000;  // since this score should be minimized we need to deduct points
                                 // for winning
   else
      return -1000000000000;
   }

   std::map< int, Vector2f > positions;
   std::set< int > opponentEntities = std::set< int >();
   std::set< int > playerEntities = std::set< int >();

   double playerArmyHealth = 0;
   double enemyArmyHealth = 0;
   double playerKingHP = 0;
   double enemyKingHP = 0;
   int kingX = 0;
   int kingY = 0;

   double totalDistance = 0;
   double meanDistance = 0;

   int bob = gameState.getCurrentTBSPlayer();

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
            enemyKingHP = entity.getParameter("Health");
         } 
         else {
            enemyArmyHealth += entity.getParameter("Health");
         }

      } else if(entity.getOwnerID() == gameState.getCurrentTBSPlayer()) {
         playerEntities.insert(entity.getID());
         auto& entityType = gameState.getGameInfo()->getEntityType(entity.getEntityTypeID());
         if(entityType.getName() == "King") {
            playerKingHP = entity.getParameter("Health");
         } 
         else {
            playerArmyHealth += entity.getParameter("Health");
         }
      }
   }

   // Gets the total distance of player units (apart from king!) and enemy king
   for(const auto& entity : playerEntities) {
      if((gameState.getGameInfo()
             ->getEntityType(gameState.getEntity(entity)->getEntityTypeID())
             .getName()
          != "King")) {
         double deltaX = pow((positions[entity].x - kingX), 2);
         double deltaY = pow((positions[entity].y - kingY), 2);
         double distance = (abs(positions[entity].x - kingX) + abs(positions[entity].y - kingY)) - gameState.getEntity(entity)->getParameter("AttackRange");
         totalDistance += distance;
         //totalDistance += abs(positions[entity].x - kingX) + abs(positions[entity].y - kingY);
      }
   }

   //meanDistance = totalDistance / static_cast< double >(playerEntities.size() - 1);
   //score -= meanDistance * 100;
   score -= totalDistance;

   score -= enemyKingHP * 2;
   score -= enemyArmyHealth;
   score += playerArmyHealth;
   score += playerKingHP;

   score -= opponentEntities.size();
   score += playerEntities.size();


   //score -= enemyKingHP;

   return score;
}
AggresiveHeuristic::AggresiveHeuristic(GameState state)
{
   int playerID = state.getCurrentTBSPlayer();
   int test = 1;
}
AggresiveHeuristic::AggresiveHeuristic() {}
}  // namespace SGA