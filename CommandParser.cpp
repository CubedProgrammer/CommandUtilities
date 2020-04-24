#include<CommandParser.hpp>
#include<iostream>
Command::Command()
{
    this->calls = 0;
}
Command& Command::operator=(Command other)
{
    this->calls = other.calls;
    return*this;
}
std::string Command::run0(std::string* args, const size_t& len)
{
    return run(args, len, calls++);
}
CommandParser::CommandParser()
{
    this->cmds = new cmd_parser_map();
}
CommandParser::CommandParser(std::string* names, Command** cmdptrs, const size_t& len):CommandParser()
{
    for (size_t i = 0; i < len; i++)
    {
        (*this->cmds)[names[i]] = cmdptrs[i];
    }
}
CommandParser::CommandParser(const CommandParser&other)
{
    this->cmds = new cmd_parser_map(*other.cmds);
}
CommandParser::CommandParser(CommandParser&&other)
{
    this->cmds = other.cmds;
    other.cmds = nullptr;
}
CommandParser& CommandParser::operator=(CommandParser other)
{
    std::swap(*this, other);
    return*this;
}
CommandParser::~CommandParser()
{
    delete this->cmds;
}
Command& CommandParser::operator[](const std::string& str)
{
    return*(*cmds)[str];
}
std::string CommandParser::operator()(std::string* args, const size_t& len)
{
    std::string ans = "That command was not found!";
    if (cmds->find(args[0]) != cmds->end())
    {
        ans = (*(*cmds)[args[0]])(args + 1, len);
    }
    return ans;
}