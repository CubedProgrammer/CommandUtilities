#include<algorithm>
#include<array>
#include<cmath>
#include<complex>
#include<ctime>
#include<fstream>
#include<iostream>
#include<random>
#include<sstream>
#include<vector>
#include<CommandParser.hpp>
#include <TriangleSolver.hpp>
extern"C"
{
    void vector_polar_addition(double angles[], double mags[], unsigned size, double* angle, double* mag);
    void vector_cylindrical_addition(double angles[], double mags[], double zs[], unsigned size, double* angle, double* mag, double* z);
    int file_word_counter(const char *fn, const char *wrd);
    void file_word_replace(const char *fn, const char *wrd, const char *rep);
    void list_file_generator(int lns, const char *fn);
    int move(const char *from, const char *to);
    int del(const char *file);
    int rmdir(const char *dir);
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

struct StringReversal :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Pass in a string.";
        using namespace std;
        reverse(args->begin(), args->end());
        return *args;
    }
};

struct WordPyramid :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Pass in a string.";
        using namespace std;
        auto& str = *args;
        ostringstream oss;
        for (size_t i = 0; i < str.size(); i++)
            oss << str.substr(0, i + 1) << "\n";
        for (size_t i = str.size() - 1; i > 0; i--)
            oss << str.substr(0, i) << "\n";
        return oss.str();
    }
};

struct StringDuper :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 2)
            return "Pass in a string and a number.";
        using namespace std;
        auto& str = *args;
        int x = stoi(args[1]);
        string o;
        for (int i = 0; i < x; i++)
            o += str;
        return o;
    }
};

struct SetTrash :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Name a directory to be your trash directory.";
        using namespace std;
        auto& str = *args;
        ofstream ofs("trash.txt");
        ofs << str << endl;
        ofs.close();
        return "If you only see this message, it worked.";
    }
};

struct TempDelete :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Name a directory to be your trash directory.";
        using namespace std;
        auto& str = *args;
        string dir;
        ifstream ifs("trash.txt");
        if (!ifs)
            return "Set the trash folder first you imbecile";
        ifs >> dir;
        ifs.close();
        int r = move(str.c_str(), dir.c_str());
        if (r)
            return "Something went terribly wrong, check if you have permission, or if the file is read only.";
        return "It worked.";
    }
};

struct FileWordCounter :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 2)
            return "Name a file and a word.";
        using namespace std;
        auto& fn = args[0];
        auto& wrd = args[1];
        int cnt = file_word_counter(fn.c_str(), wrd.c_str());
        ostringstream oss;
        oss << cnt;
        return oss.str();
    }
};

struct VectorCylindricalAddition :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size % 3 != 0)
            return "You must pass in a multiple of three number of args.";
        double* angles = new double[size / 3];
        double* mags = new double[size / 3];
        double* zs = new double[size / 3];
        double angle, mag, z;
        for (size_t i = 0; i < size; i++)
        {
            if (i % 3 == 1)
                mags[i / 3] = std::stod(args[i]);
            else if (i % 3 == 0)
                angles[i / 3] = std::stod(args[i]) * 3.1415926535897932 / 180;
            else
                zs[i / 3] = std::stod(args[i]);
        }
        vector_cylindrical_addition(angles, mags, zs, size / 2, &angle, &mag, &z);
        delete[]angles;
        delete[]mags;
        delete[]zs;

        angle *= 180 / 3.1415926535897932;
        std::ostringstream oss;
        oss << angle << " degrees , " << mag << ", with a height of " << z;
        return oss.str();
    }
};

struct FileWordReplace :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 3)
            return "Name a file and two words.";
        using namespace std;
        auto& fn = args[0];
        auto& wrd = args[1];
        auto& rep = args[2];
        file_word_replace(fn.c_str(), wrd.c_str(), rep.c_str());
        return "Replaced the words in your file.";
    }
};

struct ListFileGenerator :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 2)
            return "Put the number and then output file.";
        list_file_generator(std::stoi(args[0]), args[1].c_str());
        return "Look at your file.";
    }
};

struct PrimeNumberListGenerator :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 2)
            return "Put the number and then output file.";
        long long num = std::stoll(args[0]);
        std::vector<bool>sieve(num + 1, true);
        for(long long i = 2; i <= num; i++)
        {
            if(sieve[i])
            {
                for(long long j = i * i; j <= num; j += i)
                    sieve[j] = false;
            }
        }
        std::ofstream ofs(args[1]);
        for(long long i = 2; i <= num; i++)
        {
            if(sieve[i])
                ofs << i << '\n';
        }
        ofs.close();
        return "Look at your file.";
    }
};

struct CompositeTest :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Put in a number";
        using namespace std;
        long long x = stoll(args[0]);
        long long y = sqrt(x);
        if(x <= 1)
            return "neither";
        if(x == 2 || x == 3)
            return "prime";
        else if(x % 2 == 0 || x % 3 == 0)
            return "composite";
        for(long long i = 5, j = 0; i <= y; i += 2 + (j & 1) * 2, j++)
        {
            if(x % i == 0)
                return "composite";
        }
        return "prime";
    }
};

struct DoubleBits :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 1)
            return "Put in a number";
        using namespace std;
        double x = stod(args[0]);
        ostringstream oss;
        oss << hex << *(long long unsigned*) & x;
        auto str = oss.str();
        reverse(str.begin(),str.end());
        while(str.size()<16)
            str.push_back('0');
        reverse(str.begin(),str.end());
        return "0x" + str;
    }
};

struct CrossProduct :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 6)
            return "Put in the three components of the two vectors.";
        using namespace std;
        array<double, 3>vec1{ stod(args[0]), stod(args[1]), stod(args[2]) };
        array<double, 3>vec2{ stod(args[3]), stod(args[4]), stod(args[5]) };
        ostringstream oss;
        oss << (vec1[1] * vec2[2] - vec1[2] * vec2[1]) << ", " << (vec2[0] * vec1[2] - vec2[2] * vec1[0]) << ", " << (vec1[0] * vec2[1] - vec1[1] * vec2[0]);
        return oss.str();
    }
};

struct DotProduct :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size % 2 == 1)
            return "Put in the components of the two vectors.";
        using namespace std;
        vector<double>vec1(size / 2);
        vector<double>vec2(size / 2);
        for (size_t i = 0; i < size / 2; i++)
        {
            vec1[i] = stod(args[i]);
            vec2[i] = stod(args[i + size / 2]);
        }

        double ans = 0;
        for (size_t i = 0; i < size / 2; i++)
            ans += vec1[i] * vec2[i];
        
        ostringstream oss;
        oss << ans;
        return oss.str();
    }
};

struct TimeStamp :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        std::ostringstream oss;
        oss << std::time(nullptr);
        return oss.str();
    }
};

struct GetChar :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        std::string chars;
        for (size_t i = 0; i < size; i++)
            chars += (char)std::stoi(args[i]);
        return chars;
    }
};

struct CharVal :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        auto& str = *args;
        std::string vals;
        for (size_t i = 0; i < str.size(); i++)
            vals += std::to_string((int)str[i]) + ", ";
        return vals.substr(0, vals.size() - 2);
    }
};

struct ComplexMultiply :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if(size % 2 == 1)
            return"Enter an even number of arguments.";
        else if(size == 0)
            return"Each pair of arguments will be parsed as a complex number and multiplied.";
        using namespace std::complex_literals;
        std::complex<double>result = 1, tmp;
        for (size_t i = 0; i < size; i++)
        {
            if(i % 2 == 1)
            {
                tmp += std::stod(args[i]) * 1i;
                result *= tmp;
            }
            else
                tmp = std::stod(args[i]);
        }
        std::ostringstream oss;
        oss << result;
        return oss.str();
    }
};

std::mt19937_64 dice(time(nullptr));

struct RNG :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls)
    {
        if (size < 3)
            return"Put in the lower and upper bound, and number of numbers.";
        long long lo = std::stoll(args[0]), hi = std::stoll(args[1]);
        long long range = hi - lo + 1;
        size_t cnt = std::stoll(args[2]);
        std::string str;
        for (size_t i = 0; i < cnt; i++)
            str += std::to_string(dice() % range + lo) + " ";
        return str.substr(0, str.size() - 1);
    }
};

int main(int argl,char**argv)
{
    std::string names[] = { "jhash","zprob","probz","mean_stdev_prob","solvet","vector_polar_addition","reversal","pyramid","dupe","settrash","tmpdel","file_word_counter", "vector_cylindrical_addition","file_word_replace","list_file_generator","primes","prime","double_raw_bits", "crossmult", "dotmult", "timestamp", "char", "ascii", "compmult", "rand" };
    Command* strh = new StringHasher();
    Command* zp = new ZProb();
    Command* pz = new ProbZ();
    Command* cmds[] = { strh, zp, pz, new MeanStdevProb(), new TriangleSolver(), new VectorPolarAddition(), new StringReversal(), new WordPyramid(), new StringDuper(), new SetTrash(), new TempDelete(), new FileWordCounter(), new VectorCylindricalAddition(), new FileWordReplace(), new ListFileGenerator(), new PrimeNumberListGenerator(), new CompositeTest(), new DoubleBits(), new CrossProduct(), new DotProduct(), new TimeStamp(), new GetChar(), new CharVal(), new ComplexMultiply(), new RNG() };
    CommandParser parser(names, cmds, 25);
    std::string command, tmpc;
    std::vector<std::string>tokens(0);
    std::string* arr = nullptr;
    std::getline(std::cin, command);
    bool esc = false;
    while (command != "exit" && command != "quit")
    {
        tokens.clear();
        for(unsigned i = 0; i < command.size(); i++)
        {
            if(command[i] == ' ')
            {
                if(!esc)
                {
                    tokens.push_back(tmpc);
                    tmpc.clear();
                }
                else
                {
                    tmpc.back() = ' ';
                    esc = false;
                }
            }
            else
            {
                if(command[i] == '\\')
                    esc = true;
                else
                    esc = false;
                tmpc.push_back(command[i]);
            }
        }
        if(tmpc.size())
            tokens.push_back(std::move(tmpc));
        arr = new std::string[tokens.size()];
        std::copy(tokens.begin(), tokens.end(), arr);
        std::cout << parser(arr, tokens.size()) << std::endl;
        delete[]arr;
        getline(std::cin, command);
    }
    std::cout << "Program has terminated." << std::endl;
#ifdef _WIN32
    std::system("pause");
#else
    std::cout << "Press enter to continue.";
    std::cout.flush();
    char __tmp__;
    std::cin.get(__tmp__);
#endif
    return 0;
}
