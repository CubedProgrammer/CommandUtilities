#include<stdio.h>
size_t gcd(size_t x, size_t y)
{
    return y == 0 ? x : gcd(y, x % y);
}
size_t lcm(size_t x, size_t y)
{
    return x * y / gcd(x, y);
}
