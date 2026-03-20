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
	std::ifstream				test_data;
	std::string					buffer;
	std::string					absolute_path;
	std::string					test_name;

	if (argc != 2)
		return 1;
	test_data.open(argv[1], std::ios_base::in);
	getline(test_data, absolute_path);
	while (getline(test_data, test_name))
	{
		std::cout << test_name << ": \n";
	input_file.open((absolute_path + "/" + test_name).c_str(), std::ios_base::in);
		ReadAllTokens(input_file, buffer);
		input_file.close();
	}
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
