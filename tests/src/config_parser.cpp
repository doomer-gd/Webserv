# include "main/main.hpp"
# include "parse/ConfigDefines.hpp"
# include "parse/ConfigParser.hpp"
# include "parse/ConfigSetters.hpp"
# include "parse/ConfigTokenizer.hpp"
# include "utils/StateMachine.hpp"
# include "utils/Codes.hpp"
# include <unistd.h>

void	printConfigContent(ConfigMain& config)
{
	std::cout << "client_header_buffer_size" << config.bufferSize << std::endl;
	std::cout << "worker_connections: " << config.connectionsMax << std::endl;
	std::cout << "worker_rlimit_nofile: " << config.fdsMax << std::endl;
	std::cout << "error_log: " << config.logFileName << std::endl;
}


int main(int argc, char** argv)
{
	ConfigParser	parser;
	ConfigMain		config;

	if (argc != 2)
		return (1);
	parser.ParseConfigFile(config, argv[1]);
	printConfigContent(config);
	return (0);
}
