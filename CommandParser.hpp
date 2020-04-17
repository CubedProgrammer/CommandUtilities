#include<string>
#include<unordered_map>
#include<utility>
class Command
{
private:
    size_t calls;
public:
    Command();
    Command(const Command&) = default;
    Command(Command&&) = default;
    Command& operator=(Command);
    virtual std::string run(std::string*,const size_t&,const size_t&);
    std::string run0(std::string*, const size_t& len);
    std::string operator()(std::string* args, const size_t& len)
    {
        return run0(args,len);
    }
};
using cmd_parser_map = std::unordered_map<std::string, Command*>;
class CommandParser
{
private:
    cmd_parser_map* cmds;
public:
    CommandParser();
    CommandParser(std::string* , Command**, const size_t&);
    CommandParser(const CommandParser&);
    CommandParser(CommandParser&&);
    CommandParser& operator=(CommandParser);
    ~CommandParser();
    Command& operator[](const std::string&);
    std::string operator()(std::string*, const size_t&);
};