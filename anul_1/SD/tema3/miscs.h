/* SPORICI Dan - 315CB */

typedef struct TLine
{
    int a;
    int b;
    int c;
} TLine;

typedef struct TPoint
{
    int x;
    int y;
} TPoint;

double getSlope(TLine);
int getPlane(TLine, TPoint);
