#include<math.h>
void vector_polar_addition(double angles[], double mags[], unsigned size, double *angle, double* mag)
{
    double x = 0, y = 0;
    for(unsigned i=0;i<size;i++)
    {
        x+=mags[i]*cos(angles[i]);
        y+=mags[i]*sin(angles[i]);
    }
    *angle=atan2(y,x);
    *mag=sqrt(x*x+y*y);
}