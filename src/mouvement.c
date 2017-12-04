#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#define N 2
#define VMIN 20
#define VMAX 255 //V est entre 2 et 2^m-1 avec m le nombre de bits des donnees ici 8 => https://hal.inria.fr/hal-01130889/document
#define VINI 35
#define SIZE_TAB 80

__attribute__ ((always_inline))int inline minO(int a, int b)
{
	return a<b ? a:b;
}
int min(int a, int b)
{
	return a<b ? a:b;
}


__attribute__ ((always_inline))int inline maxO(int a, int b)
{
	return a>b ? a:b;
}

int max(int a, int b)
{
	return a>b ? a:b;
}

void routine_FrameDifference(uint8 It[][SIZE_TAB], uint8 Itm1[][SIZE_TAB], uint8 Et[][SIZE_TAB], long nrl,long nrh,long ncl,long nch, int seuil)
{
    //m[nrl..nrh][ncl..nch]
	int i, j;
	uint8 Ot [nrh+1][nch+1] ;

	for(i = nrl; i <= nrh; i++ )
	{
		for(j = ncl; j <= nch; j++)
		{
			Ot[i][j] = abs(It[i][j] - Itm1[i][j]);
			if(Ot[i][j] < seuil)
				Et[i][j] = 0;
			else
				Et[i][j] = 255;
		}
	}
}


void routine_SigmaDelta_step0(uint8 I[][SIZE_TAB], uint8 M[][SIZE_TAB], uint8 V[][SIZE_TAB], long nrl, long nrh, long ncl, long nch)
{
	printf("Entering SD_step0\n\r");
	int i, j;
	for(i = nrl; i <= nrh; i++ )
	{
		for(j = ncl; j <= nch; j++)
		{
			M[i][j] = I[i][j];
            V[i][j] = VINI; //Au depart a VMIN mais il y avait beaucoup de mouvement des le debut, a VINI ça marche mieux
        }
    }
}

void routine_SigmaDelta_1step(uint8 It[][SIZE_TAB], uint8 Vt[][SIZE_TAB], uint8 Vtm1[][SIZE_TAB], uint8 Mt[][SIZE_TAB], uint8 Mtm1[][SIZE_TAB], uint8 Et[][SIZE_TAB],  long nrl, long nrh, long ncl, long nch )
{
	printf("Entering SD_1step\n\r");
	uint8 Ot[nrh+1][nch+1];
	uint8 tmpMtm1, tmpVtm1;
	int i, j;
    for(i = nrl; i <= nrh; i++ ) //Step1 Mt Estimation
    {
    	for(j = ncl; j <= nch; j++)
    	{
    		tmpMtm1 = Mtm1[i][j];
    		tmpVtm1 = Vtm1[i][j];

    		if(tmpMtm1 < It[i][j])
    			Mt[i][j]  = tmpMtm1 + 1;

    		else if(tmpMtm1 > It[i][j])
    			Mt[i][j] = tmpMtm1 - 1;

    		else
    			Mt[i][j] = tmpMtm1;


    		//Step 2 difference Computation
    		Ot[i][j] = abs(Mt[i][j] - It[i][j]);


    		//Step 3 Update and clamping
    		if(tmpVtm1 < N * Ot[i][j])
    			Vt[i][j] = tmpVtm1 + 1;

    		else if(tmpVtm1 > N * Ot[i][j])
    			Vt[i][j] = tmpVtm1 - 1;

    		else
    			Vt[i][j] = tmpVtm1;


    		Vt[i][j] = max( min(Vt[i][j], VMAX), VMIN);


    		//Step 4 Et estimation
    		if(Ot[i][j] < Vt[i][j])
    			Et[i][j] = 0;
    		else
    			Et[i][j] = 255;
    		printf("%d   ", Et[i][j]);
    	}
    	printf("\n\r");
    }

}
