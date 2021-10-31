#include<math.h>
#include<stddef.h>
int horizon_dist(double radius, double height, double *direct, double *foot)
{
    if(radius <= 0 || height <= 0 || direct == NULL || foot == NULL)
        return-1;
    else
    {
        *direct = sqrt((2 * radius + height) * height);
        *foot = radius * asin(*direct / (radius + height));
        return 0;
    }
}
