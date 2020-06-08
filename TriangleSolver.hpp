#pragma once
#include <CommandParser.hpp>
struct TSSATDAH
{
    TSSATDAH(double* ptr) :ptr(ptr), pos(0) {}
    double* ptr;
    size_t pos;
    void operator()(const std::string& s);
};
struct TriangleSolver :public Command
{
    std::string run(std::string* args, const size_t& size, const size_t& calls);
};
extern "C"
{
    void solve_triangle(double* Ap, double* Bp, double* Cp, double* ap, double* bp, double* cp);
}