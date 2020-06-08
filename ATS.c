#include <math.h>
void solve_triangle(double* Ap, double* Bp, double* Cp, double* ap, double* bp, double* cp)
{
	double A = *Ap, B = *Bp, C = *Cp;
	double a = *ap, b = *bp, c = *cp;
	unsigned int au = (A == -1) + (B == -1) + (C == -1);
	unsigned int su = (a == -1) + (b == -1) + (c == -1);
	if (A >= 0)
		A *= 3.1415926535897932 / 180;
	if (B >= 0)
		B *= 3.1415926535897932 / 180;
	if (C >= 0)
		C *= 3.1415926535897932 / 180;
	if (au <= 1)
	{
		if (au == 1)
		{
			if (A == -1)
				A = 3.1415926335897932 - B - C;
			else if (B == -1)
				B = 3.1415926535897932 - A - C;
			else
				C = 3.1415926535897932 - A - B;
			au--;
		}
		if (su <= 2)
		{
			if (a >= 0)
			{
				b = a * sin(B) / sin(A);
				c = a * sin(C) / sin(A);
			}
			else if (b >= 0)
			{
				a = b * sin(A) / sin(B);
				c = b * sin(C) / sin(B);
			}
			else
			{
				b = c * sin(B) / sin(C);
				a = c * sin(A) / sin(C);
			}
		}
	}
	else if (au == 2)
	{
		if (su <= 1)
		{
			if (A >= 0)
			{
				if (a >= 0)
				{
					if (b >= 0)
					{
						B = asin(b * sin(A) / a);
						C = 3.1415926535897932 - A - B;
						c = a * sin(C) / sin(A);
					}
					else
					{
						C = asin(c * sin(A) / a);
						B = 3.1415926535897932 - A - C;
						b = a * sin(B) / sin(A);
					}
				}
				else
				{
					a = sqrt(b * b + c * c - 2 * b * c * cos(A));
					B = asin(b * sin(A) / a);
					C = asin(c * sin(A) / a);
				}
			}
			else if (B >= 0)
			{
				if (b >= 0)
				{
					if (a >= 0)
					{
						A = asin(a * sin(B) / b);
						C = 3.1415926535897932 - A - B;
						c = a * sin(C) / sin(A);
					}
					else
					{
						C = asin(c * sin(B) / b);
						A = 3.1415926535897932 - B - C;
						a = b * sin(A) / sin(B);
					}
				}
				else
				{
					b = sqrt(a * a + c * c - 2 * a * c * cos(B));
					A = asin(a * sin(B) / b);
					C = asin(c * sin(B) / b);
				}
			}
			else
			{
				if (c >= 0)
				{
					if (a >= 0)
					{
						A = asin(a * sin(B) / b);
						B = 3.1415926535897932 - A - C;
						b = a * sin(B) / sin(A);
					}
					else
					{
						B = asin(b * sin(C) / c);
						A = 3.1415926535897932 - B - C;
						a = b * sin(A) / sin(B);
					}
				}
				else
				{
					c = sqrt(a * a + b * b - 2 * a * b * cos(C));
					A = asin(a * sin(C) / c);
					B = asin(b * sin(C) / c);
				}
			}
		}
	}
	else if (su == 0)
	{
		A = acos((b * b + c * c - a * a) / (2 * b * c));
		B = acos((a * a + c * c - b * b) / (2 * a * c));
		C = acos((a * a + b * b - c * c) / (2 * a * b));
	}
	A *= 180 / 3.1415926535897932;
	B *= 180 / 3.1415926535897932;
	C *= 180 / 3.1415926535897932;
	*Ap = A, * Bp = B, * Cp = C;
	*ap = a, * bp = b, * cp = c;
}