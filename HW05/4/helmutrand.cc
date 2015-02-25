#include <stdlib.h>
#include <limits.h>

int	rseed;
double	randnum;

double  helmutrand(void) 
{

    int	  lag 	= 72;
    int	  multi = 77;

    /* values for RANDU
    int	  lag   = 0;
    int	  multi = 65539;
    */

    rseed = (rseed*multi  + lag) % (INT_MAX - 1);      

    if(rseed < 0){ 
	rseed = rseed + INT_MAX + 1;
    }
    
    randnum  	= ((double) rseed)/((double) (INT_MAX - 1));

    return randnum;
}

void sethelmutrand(long seed)
{
    rseed = seed;
}  

