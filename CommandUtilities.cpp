#include<iostream>
#include<sstream>
#include<vector>
#include<CommandParser.hpp>
#include <TriangleSolver.hpp>
struct StringHasher :public Command
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

struct ZProb :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        using namespace std;

        double z = stod(*args);
        double p = 0.5 + erf(z / sqrt(2)) / 2;

        ostringstream oss("");
        oss.precision(15);
        oss << p;
        return oss.str();
    }
};

struct ProbZ :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        using namespace std;

        double p = stod(*args);
        double z = 1, t = erf(z / sqrt(2)) / 2 + 0.5 - p;
        double g;

        while (abs(t) > 0.0000000000001)
        {
            g = exp(-z * z / 2) / sqrt(8);
            z -= t / g;
            t = erf(z / sqrt(2)) / 2 + 0.5 - p;
        }

        ostringstream oss("");
        oss.precision(15);
        oss << z;
        return oss.str();
    }
};

int main(int argl,char**argv)
{
    std::string names[] = { "jhash","zprob","probz","solvet" };
    Command* strh = new StringHasher();
    Command* zp = new ZProb();
    Command* pz = new ProbZ();
    Command* cmds[] = { strh, zp, pz, new TriangleSolver() };
    CommandParser parser(names, cmds, 4);
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
            tokens.push_back(command);
        }
        arr = new std::string[tokens.size()];
        std::copy(tokens.begin(), tokens.end(), arr);
        std::cout << parser(arr, tokens.size()) << std::endl;
        delete[]arr;
        getline(std::cin, command);
        is=std::istringstream(command);
    }
    std::cout << "Program has terminated." << std::endl;
    std::system("pause");
    return 0;
}