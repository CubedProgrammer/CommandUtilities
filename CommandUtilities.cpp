#include<iostream>
#include<sstream>
#include<vector>
#include<CommandParser.hpp>
class StringHasher :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        // hashes the string using java's hashing algorithm
        size_t h = 0;
        for (std::string::iterator it = args[0].begin(); it != args[0].end(); ++it)
        {
            h *= 31;
            h += *it;
        }
        // outputs the hash
        std::ostringstream oss("");
        oss << h;
        return oss.str();
    }
};
int main(int argl,char**argv)
{
    std::string names[] = {"jhash"};
    Command* strh = new StringHasher();
    CommandParser parser(names,&strh,1);
    std::string command;
    std::vector<std::string>tokens(0);
    std::string* arr = nullptr;
    std::getline(std::cin, command);
    std::istringstream is(command);
    while (command != "exit" && command != "quit")
    {
        tokens.clear();
        while (!is.eof())
        {
            is >> command;
            std::cout << "command was " << command << std::endl;
            tokens.push_back(command);
        }
        arr = new std::string[tokens.size()];
        std::copy(tokens.begin(), tokens.end(), arr);
        delete[]arr;
        getline(std::cin, command);
        is=std::istringstream(command);
    }
    std::cout << "Program has terminated." << std::endl;
    std::system("pause");
    return 0;
}