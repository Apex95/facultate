/* SPORICI Dan - 315CB */

#include <stdio.h>
#include "miscs.h"

// returneaza rezultatul dreptei 'line' in punctul 'point'
int getPlane(TLine line, TPoint point)
{
    return line.a * point.x + line.b * point.y + line.c;
}

// returneaza panta unei drepte
double getSlope(TLine line)
{
    return (-line.a)/((double)line.b);
}
