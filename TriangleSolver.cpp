#include <algorithm>
#include <sstream>
#include <TriangleSolver.hpp>
void TSSATDAH::operator()(const std::string& s)
{
	ptr[pos] = std::stod(s, nullptr);
	pos++;
}
std::string TriangleSolver::run(std::string* args, const size_t& size, const size_t& calls)
{
	if (size != 6)
		return "Must have 6 arguments, the three angles and three corresponding sides.";

	double da[6];
	TSSATDAH helper(da);
	std::for_each(args, args + 6, helper);
	solve_triangle(da, da + 1, da + 2, da + 3, da + 4, da + 5);

	std::ostringstream oss;
	for (size_t i = 0; i < 6; i++)
		oss << da[i] << ", ";

	std::string s = oss.str();
	return s.substr(0, s.size() - 2);
}