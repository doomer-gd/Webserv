# include "main/main.hpp"
# include "parse/ConfigDefines.hpp"
# include "parse/ConfigParser.hpp"
# include "parse/ConfigSetters.hpp"
# include "parse/ConfigTokenizer.hpp"
# include "utils/StateMachine.hpp"
# include "utils/Codes.hpp"
# include <unistd.h>

template <typename T, typename L>
std::ostream& operator<<(std::ostream& os, const std::pair<T, L>& v)
{
	os << "[";
	os << v.first << ", " << v.second;
	os << "]\n";
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	for (size_t i = 0; i < v.size(); ++i) {
		os << v[i];
		if (i != v.size() - 1)
			os << ", ";
	}
	os << "]\n";
	return os;
}


void	printConfigContent(ConfigMain& config)
{
	std::cout << "client_header_buffer_size: " << config.bufferSize << std::endl;
	std::cout << "worker_connections: " << config.connectionsMax << std::endl;
	std::cout << "worker_rlimit_nofile: " << config.fdsMax << std::endl;
	std::cout << "error_log: " << config.logFileName << std::endl;
	std::cout << "client_header_timeout: " << config.timeOut.header << std::endl;
	std::cout << "client_body_timeout: " << config.timeOut.body << std::endl;
	std::cout << "send_timeout: " << config.timeOut.send << std::endl;
	std::cout << "keepalive_timeout: " << config.timeOut.keepAlive << std::endl;
	if (config.servers.size() > 0)
	{
		if (config.servers[0].portsArray.size() > 0)
			std::cout << "listen: " << config.servers[0].portsArray << std::endl;
		if (config.servers[0].serverNames.size() > 0)
			std::cout << "server_name: " << config.servers[0].serverNames << std::endl;
		if (config.servers[0].errorPages.size() > 0)
			std::cout << "error_page: " << *(config.servers[0].errorPages.begin()) << std::endl;
	}
}


int main(int argc, char** argv)
{
	ConfigParser	parser;
	ConfigMain		config;

	if (argc != 2)
		return (1);
	if (parser.ParseConfigFile(config, argv[1]) == E_SUCCESS)
		printConfigContent(config);
	else
		std::cerr << "Config file parsing failed" << std::endl;
	return (0);
}
