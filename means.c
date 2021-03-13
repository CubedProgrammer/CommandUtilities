#include<math.h>
double amean(double x, double y)
{
    return (x + y) / 2;
}
double gmean(double x, double y)
{
    return sqrt(x * y);
}
double hmean(double x, double y)
{
    return 2 / (1 / x + 1 / y);
}
double rmsq(double x, double y)
{
    return sqrt((x * x + y * y) / 2);
}
double agmean(double x, double y)
{
    double u, v;
    while(fabs(x - y) > 0.000000000001)
    {
        u = amean(x, y);
        v = gmean(x, y);
        x = u, y = v;
    }
    return x;
}