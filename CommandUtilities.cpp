#include<iostream>
#include<sstream>
#include<vector>
#include<CommandParser.hpp>
#include <TriangleSolver.hpp>

extern"C"
{
    void vector_polar_addition(double angles[], double mags[], unsigned size, double* angle, double* mag);
}

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

struct MeanStdevProb :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 3)
            return "You must pass in the mean, standard deviation, and the value, in that order.";
        using namespace std;
        double mean = stod(args[0], nullptr), stdev = stod(args[1], nullptr), v = stod(args[2], nullptr);
        double z = (v - mean) / stdev;
        double ans = 0.5 + erf(z / sqrt(2)) / 2;

        ostringstream oss("");
        oss.precision(15);
        oss << "z-score is " << z << endl;
        oss << "probability is " << ans;
        return oss.str();
    }
};

struct VectorPolarAddition :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size % 2 != 0)
            return "You must pass in an even number of args.";
        double* angles = new double[size / 2];
        double* mags = new double[size / 2];
        double angle, mag;
        for (size_t i = 0; i < size; i++)
        {
            if (i % 2 != 0)
                mags[i / 2] = std::stod(args[i]);
            else
                angles[i / 2] = std::stod(args[i]) * 3.1415926535897932 / 180;
        }
        vector_polar_addition(angles, mags, size / 2, &angle, &mag);
        delete[]angles;
        delete[]mags;

        angle *= 180 / 3.1415926535897932;
        std::ostringstream oss;
        oss << angle << "degrees , " << mag;
        return oss.str();
    }
};

int main(int argl,char**argv)
{
    std::string names[] = { "jhash","zprob","probz","mean_stdev_prob","solvet","vector_polar_addition" };
    Command* strh = new StringHasher();
    Command* zp = new ZProb();
    Command* pz = new ProbZ();
    Command* cmds[] = { strh, zp, pz, new MeanStdevProb(), new TriangleSolver(), new VectorPolarAddition() };
    CommandParser parser(names, cmds, 6);
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