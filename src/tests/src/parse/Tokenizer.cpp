# include "../../../../include/main/main.hpp"
# include "../../../../include/parse/ConfigDefines.hpp"
# include "../../../../include/parse/ConfigParser.hpp"
# include "../../../../include/parse/ConfigSetters.hpp"
# include "../../../../include/parse/ConfigTokenizer.hpp"
# include "../../../../include/utils/StateMachine.hpp"
# include "../../../../include/utils/Codes.hpp"

int main(void)
{
	std::ifstream	file[2];
	ConfigTokenizer	tokenizer;
	std::string		buffer;
	int				result;

	file[0].open("../../input/empty.txt", std::ios_base::in);
	result = tokenizer.GetNextToken(file[0], buffer);
	if (result == FINISHED && buffer.compare("") == 0)
		std::cout << "Empty: OK\n";
	else
	{
		std::cout << "Empty: ERROR\n";
		return 1;
	}
	file[1].open("../../input/one_word.txt");
	result = tokenizer.GetNextToken(file[1], buffer);
	if (result == FINISHED && buffer.compare("one_word") == 0)
		std::cout << "one_word: OK\n";
	else
	{
		std::cout << "one_word: ERROR, got: " << buffer << std::endl;
		return 1;
	}
	file[0].close();
	file[1].close();
	return 0;
}