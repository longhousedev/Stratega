#pragma once
#include <vector>
#include <Stratega/ForwardModel/ActionTarget.h>
#include <Stratega/Representation/EntityType.h>
#include <Stratega/Representation/Player.h>
#include <Stratega/Representation/TechnologyTree.h>
#include <Stratega/Representation/TileType.h>
#include <Stratega/Representation/BuffType.h>
#include <boost/variant.hpp>
#include <regex>
namespace SGA
{
	struct GameState;
	
	class FunctionParameter
	{
	public:
		enum class Type
		{
			Constant, // Values like 1, 1.5, etc
			ParameterReference, // References the parameter of an action-target. Source.Gold references to the gold of the source of an action
			EntityPlayerReference, // References the player indirectly. Like Source.Player, only works if Source is a entity
			EntityPlayerParameterReference, // References the parameter of an player indirectly. Like Source.Player.Gold, only works if Source is a entity
			ArgumentReference, // References an action-argument, like Source or Target. Can be used for example if you want to pass a position-target
			EntityTypeReference, // References EntityTypes defined in the Game. Like Warrior, GoldMine, etc.
			TechnologyTypeReference, // References TechnologyTypes defined in the Game,
			TimeReference, // References the Time for specific Entities or Technologies,
			TileTypeReference, // References TileTypes defined in the Game. Like Plain, Forest...
			BuffTypeReference, // References BuffTypes defined in the Game. Like Plain, Forest...
			GameStateParameterReference, // References BuffTypes defined in the Game. Like Plain, Forest...
			Expression, //List of diferent string + function parameters (constant + parameter reference)
			DiceAnotation // 4d12
		};

		struct ExpressionStruct
		{
			std::unordered_map<std::string, FunctionParameter> variable;
			std::string expression;

			void addParameter(FunctionParameter parameter, std::string newVariable)
			{
				variable.insert(std::make_pair(newVariable, parameter));
			}

			void setExpression(std::string string)
			{
				expression = string;
			}

			std::string getExpression(const GameState& state, const std::vector<ActionTarget>& actionTargets)
			{
				std::string ex = expression;
				std::cout << "Before change: " << ex << std::endl;
				for (auto& var : variable)
				{
					double temp = var.second.getConstant(state, actionTargets);
					ex = std::regex_replace(ex, std::regex(var.first), std::to_string(temp));
				}
				std::cout << "After change: " << ex << std::endl;
				return ex;
			}

			~ExpressionStruct()
			{
				expression.~basic_string();
			}
		};

	private:
		

		struct ParameterReference
		{
			ParameterID parameterID;
			size_t argumentIndex;

			ParameterReference(ParameterID newParameterID, int newArgumentIndex):
				parameterID(newParameterID),
				argumentIndex(static_cast<size_t>(newArgumentIndex))
			{
			}
		};

		struct DiceAnotation
		{
			int diceNumber;
			int diceFaceNumber;

			DiceAnotation(int newDiceNumber, int newDiceFaceNumber):
				diceNumber(newDiceNumber),
				diceFaceNumber(newDiceFaceNumber)
			{
			}
		};

		struct ContinuousActionReference
		{
			int sourceEntity;
			int actionID;

			ContinuousActionReference(int newSourceEntity, int newActionID) :
				sourceEntity(newSourceEntity),
				actionID(newActionID)
			{
			}
		};
		
		union Data
		{
			Data()
			{
				//not used
			}

			Data(double data)
			{
				constValue = data;
			}
			Data(ExpressionStruct newExpression)
			{
				new (&expression) auto(newExpression);
			}
			Data(DiceAnotation newDiceAnotation)
			{
				diceAnotation = newDiceAnotation;
			}
			Data(ParameterReference data)
			{
				parameterData = data;
			}
			Data(ContinuousActionReference data)
			{
				continuousActionData = data;
			}
			Data (Type type, int data)
			{
				switch (type)
				{
				case Type::ArgumentReference: argumentIndex = static_cast<size_t>(data); break;
				case Type::EntityTypeReference: entityTypeID = data; break;
				case Type::TileTypeReference: tileTypeID = data; break;
				case Type::TechnologyTypeReference: technologyTypeID = data; break;
				case Type::BuffTypeReference:   buffTypeID = data; break;
				case Type::TimeReference:   argumentIndex = static_cast<size_t>(data); break;
				case Type::EntityPlayerReference:   argumentIndex = static_cast<size_t>(data); break;
				default:
					throw std::runtime_error("Unknown Type");
				}
			}

			ExpressionStruct expression;
			double constValue;
			ParameterReference parameterData;
			size_t argumentIndex;
			int entityTypeID;
			int tileTypeID;
			int technologyTypeID;
			int buffTypeID;
			ContinuousActionReference continuousActionData;
			DiceAnotation diceAnotation;

			~Data()
			{
				//expression.~ExpressionStruct();
			}
		};
		
		Type parameterType;
		Data data;

		// Private since this class should only be constructed using the static methods
		//FunctionParameter(const Type& newType, const boost::variant<double, int, ContinuousActionReference, size_t, ParameterReference, DiceAnotation, ExpressionStruct>& newData) : parameterType(newType), data(newData) {};
		FunctionParameter(const Type& newType, const Data& newData) : parameterType(newType)/*, data(newData)*/
		{
			switch (parameterType)
			{
			case Type::Constant: data.constValue = newData.constValue; break;
			case Type::EntityPlayerReference: data.argumentIndex = newData.argumentIndex; break;
			case Type::EntityPlayerParameterReference: data.parameterData = newData.parameterData; break;
			case Type::Expression:  new (&data.expression) auto(newData.expression); break;
			case Type::ParameterReference: data.parameterData = newData.parameterData; break;
			case Type::GameStateParameterReference: data.parameterData = newData.parameterData; break;
			case Type::ArgumentReference: data.argumentIndex = newData.argumentIndex; break;
			case Type::EntityTypeReference: data.entityTypeID = newData.entityTypeID; break;
			case Type::TileTypeReference: data.tileTypeID = newData.tileTypeID; break;
			case Type::TechnologyTypeReference: data.technologyTypeID = newData.technologyTypeID; break;
			case Type::BuffTypeReference: data.buffTypeID = newData.buffTypeID; break;
			case Type::TimeReference: data.argumentIndex = newData.argumentIndex; break;
			case Type::DiceAnotation: data.diceAnotation = newData.diceAnotation; break;
			default:
				throw std::runtime_error("Parameter type " + std::to_string(int(parameterType)) + " not recognised in function parameter.");
			}
		};
		
	public:
		FunctionParameter(const FunctionParameter& other)
		{
			parameterType = other.parameterType;
			switch (parameterType)
			{
			case Type::Constant: data.constValue = other.data.constValue; break;
			case Type::EntityPlayerReference: data.argumentIndex = other.data.argumentIndex; break;
			case Type::EntityPlayerParameterReference: data.parameterData = other.data.parameterData; break;
			case Type::Expression:  new (&data.expression) auto(other.data.expression); break;
			case Type::ParameterReference: data.parameterData = other.data.parameterData; break;
			case Type::GameStateParameterReference: data.parameterData = other.data.parameterData; break;
			case Type::ArgumentReference: data.argumentIndex = other.data.argumentIndex; break;
			case Type::EntityTypeReference: data.entityTypeID = other.data.entityTypeID; break;
			case Type::TileTypeReference: data.tileTypeID = other.data.tileTypeID; break;
			case Type::TechnologyTypeReference: data.technologyTypeID = other.data.technologyTypeID; break;
			case Type::BuffTypeReference: data.buffTypeID = other.data.buffTypeID; break;
			case Type::TimeReference: data.argumentIndex = other.data.argumentIndex; break;
			case Type::DiceAnotation: data.diceAnotation = other.data.diceAnotation; break;
			default:
				throw std::runtime_error("Parameter type " + std::to_string(int(parameterType)) + " not recognised in function parameter.");
			}
		}
		FunctionParameter operator=(const FunctionParameter& other)
		{
			return *this;
		}
		~FunctionParameter()
		{

		}

		static FunctionParameter createConstParameter(double constValue);
		static FunctionParameter createArgumentReference(int argumentIndex);
		static FunctionParameter createParameterReference(ParameterReference ref);
		static FunctionParameter createDiceAnotation(DiceAnotation dic);
		static FunctionParameter createEntityPlayerReference(int argumentIndex);
		static FunctionParameter createTimeReference(int argumentIndex);
		static FunctionParameter createEntityPlayerParameterReference(ParameterReference ref);
		static FunctionParameter createGameStateParameterReference(ParameterReference ref);
		static FunctionParameter createEntityTypeReference(int entityTypeID);
		static FunctionParameter createTileTypeReference(int tileTypeID);
		static FunctionParameter createTechnologyTypeReference(int technologyTypeID);
		static FunctionParameter createBuffTypeReference(int buffTypeID);
		static FunctionParameter createExpression(ExpressionStruct expression);

		Type getType() const;
		const ActionTarget& getActionTarget(const std::vector<ActionTarget>& actionTargets) const;
		double getConstant(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const Parameter& getParameter(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		double getParameterValue(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		double getParameterValue(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		double& getRawParameterValue(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		Vector2f getPosition(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		Entity& getEntity(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		Entity& getObject(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		Entity& getSlotObject(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const Entity& getObject(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const Entity& getSlotObject(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const Entity& getEntity(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		Player& getPlayer(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		int getPlayerID(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const std::unordered_set<EntityTypeID>& getSpawnableEntities(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const Player& getPlayer(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const EntityType& getEntityType(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const BuffType& getBuffType(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		
		const TechnologyTreeNode& getTechnology(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const std::unordered_map<ParameterID, double>& getCost(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const std::unordered_map<ParameterID, Parameter>& getParameterLookUp(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		double getTime(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		std::vector<double>& getParameterList(GameState& state, const std::vector<ActionTarget>& actionTargets) const;
		const std::vector<double>& getParameterList(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;

		const TileType& getTileType(const GameState& state, const std::vector<ActionTarget>& actionTargets) const;

		bool isPlayerParameter(const std::vector<ActionTarget>& actionTargets) const;
		bool isEntityParameter(const std::vector<ActionTarget>& actionTargets) const;
		bool isStateParameter(const std::vector<ActionTarget>& actionTargets) const;
	};
}
