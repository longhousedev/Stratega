#include <Stratega/Configuration/GameConfigParser.h>
#include <Stratega/Logging/Log.h>
#include <Stratega/Arena/Arena.h>
#include <Stratega/Arena/InputParser.h>
#include <Stratega/Utils/cparse/shunting-yard.h>
#include <Stratega/Utils/cparse/builtin-features.h>

int main(int argc, char** argv)
{
	cparse_startup();

        std::string mapName = "long_map.yaml";
        std::string configName = "/TBS/hi.yaml";
        std::string logName = "long_mirror_mcts.yaml";

	InputParser parser(argc, argv);
	auto seed = parser.getCmdOption<unsigned int>("-seed", 0);
	auto numberOfGames = parser.getCmdOption<unsigned int>("-gamesNumber", 4);
	auto playerCount = parser.getCmdOption<int>("-playerCount", 2);
	//auto logPath = parser.getCmdOption<std::string>("-logPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/out/build/x64-Release/bin/long_map_MCTS.yaml");
	//Currently obsolete but configPath shouldn't have a default value. So we keep it until then
	#ifdef _WIN32
		auto configPath = parser.getCmdOption< std::string >("-configPath","C:/Users/Georg/Documents/Personal Projects/Stratega2022/resources/gameConfigurations/TBS/hi.yaml");
                auto logPath = parser.getCmdOption<std::string>("-logPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/out/build/x64-Release/bin/long_map_MCTS.yaml");
                auto mapsPath = parser.getCmdOption<std::string>("-mapsPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/maps/debug/long_map.yaml");
	#elif __APPLE__
		auto configPath = parser.getCmdOption< std::string >("-configPath", "/Users/george/Documents/GAIG/Stratega/resources/gameConfigurations/"+configName);
                auto mapsPath = parser.getCmdOption<std::string>("-mapsPath", "/Users/george/Documents/GAIG/Stratega/maps/debug/"+mapName);
                auto logPath = parser.getCmdOption<std::string>("-logPath", "/Users/george/Documents/GAIG/Stratega/results/debug/"+logName);
	#elif __linux__
		auto configPath = parser.getCmdOption< std::string >("-configPath", "../resources/gameConfigurations/TBS/hi.yaml");
                auto mapsPath = parser.getCmdOption<std::string>("-mapsPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/maps/debug/long_map.yaml");
                auto logPath = parser.getCmdOption<std::string>("-logPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/out/build/x64-Release/bin/long_map_MCTS.yaml")
	#endif
	//Optional
	//auto mapsPath = parser.getCmdOption<std::string>("-mapsPath", "C:/Users/Georg/Documents/Personal Projects/Stratega2022/maps/debug/long_map.yaml");

	if (configPath.empty())
	{
		std::cout << "You have to provide the argument -configPath" << std::endl;
		return 0;
	}

	// Read Config
	auto gameConfig = SGA::loadConfigFromYAML(configPath);

	// Run games
	SGA::setDefaultLogger(std::make_unique<SGA::FileLogger>(logPath));
	Arena arena(*gameConfig);
	if (mapsPath.empty())
	{
		//Run single map defined in the game config
		arena.runGames(playerCount, seed, numberOfGames);
	}
	else
	{
		//Load level definitions and exchange them in the game config
		gameConfig->levelDefinitions = SGA::loadLevelsFromYAML(mapsPath, *gameConfig);
		//Run combinations per map
		const int mapNumber = static_cast<int>(gameConfig->levelDefinitions.size());
		arena.runGames(playerCount, seed, numberOfGames, mapNumber);
	}

	return 0;
}