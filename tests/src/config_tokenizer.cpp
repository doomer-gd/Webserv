# include "main/main.hpp"
# include "parse/ConfigDefines.hpp"
# include "parse/ConfigParser.hpp"
# include "parse/ConfigSetters.hpp"
# include "parse/ConfigTokenizer.hpp"
# include "utils/StateMachine.hpp"
# include "utils/Codes.hpp"
# include <unistd.h>

static void	ReadAllTokens(std::ifstream& source, std::string& buffer);

int main(int argc, char** argv)
{
	std::ifstream				input_file;
	std::string					buffer;

	if (argc != 2)
		return 1;
	input_file.open(argv[1], std::ios_base::in);
	if (input_file.fail())
	{
		std::cerr << "Couldn't read file: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}
	ReadAllTokens(input_file, buffer);
	return 0;
}

static void	ReadAllTokens(std::ifstream& source, std::string& buffer)
{
	int result;
	int	failsafe = 0;
	ConfigTokenizer	tokenizer;

	result = tokenizer.GetNextToken(source, buffer);
	std::cout << result << " " << buffer << std::endl;
	while (!((result == FINISHED) && buffer.size() == 0))
	{
		failsafe++;
		result = tokenizer.GetNextToken(source, buffer);
		std::cout << result << " " << buffer << std::endl;
		if (result == ERROR || failsafe > 100)
		{
			std::cout << "\n";
			return ;
		}
	}
	std::cout << "\n";
}
