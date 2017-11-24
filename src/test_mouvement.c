#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"
#include "mouvement.h"
#include "car_3080.h"
#include "car_3081.h"

#define NBIMAGES 1
#define BORD 2
//I0 = It et I1 = It-1 : pareil pour tout


void test_routine_FrameDifference(int seuil)
{


    long nrl=0, nrh=59, ncl=0, nch=79;
    uint8 Et[nrh+1][nch+1];

    int i;
    for(i = 1; i <= NBIMAGES; i++)
    {
        routine_FrameDifference(car_3080, car_3081, Et, nrl,nrh,ncl,nch, seuil);
        printf("FD fini\n\r");
    }

}


void test_routine_sigmaDelta()
{
	//m[nrl..nrh][ncl..nch]
    //long nrl=0, nrh=59, ncl=0, nch=79;
	long nrl=0, nrh=0, ncl=0, nch=15;
	uint8 Et[nrh+1][nch+1];

    uint8 Mtm1[] = {127, 255, 0, 0, 1, 255, 254, 50, 70, 50, 104, 17, 195,   6,  90, 156};
    uint8 It[] = {  128, 255, 0, 1, 0, 254, 255, 60, 45, 50, 133, 67, 149, 198, 191,  68};

	uint8 Vt[nrh+1][nch+1];
	uint8 Vtm1[nrh+1][nch+1];

	uint8 Mt[nrh+1][nch+1];
	//uint8 Mtm1[nrh+1][nch+1];

	//routine_SigmaDelta_step0(car_3081, Mtm1, Vtm1, nrl, nrh, ncl, nch);
	routine_SigmaDelta_step0(It, Mtm1, Vtm1, nrl, nrh, ncl, nch);


	int i;
	for(i = 1; i <= NBIMAGES; i++)
	{
		routine_SigmaDelta_1step(It, Vt, Vtm1, Mt, Mtm1, Et, nrl, nrh, ncl, nch);
		//routine_SigmaDelta_1step(car_3080, car_3081, Vt, Vtm1, Mt, Mtm1, Et, nrl, nrh, ncl, nch);
		printf("Fin SD\n\r");
	    printf("Resultat attendu de Et:\n preparer resultat attendu en fct de mtm1 et it dans test mouvement\n");

		//quand travaille plusieurs tours de boucle, rotation de variables
		/*
		copy_ui8matrix_ui8matrix(Mt, nrl, nrh, ncl, nch, Mtm1);
		copy_ui8matrix_ui8matrix(Vt, nrl, nrh, ncl, nch, Vtm1);
		copy_ui8matrix_ui8matrix(It, nrl, nrh, ncl, nch, Itm1);
		 */

		/*memcpy(Mtm1[nrl], Mt[nrl], sizeof(uint8)*(nrow*ncol));
		memcpy(Vtm1[nrl], Vt[nrl], sizeof(uint8)*(nrow*ncol));
		memcpy(Itm1[nrl], It[nrl], sizeof(uint8)*(nrow*ncol));*/

	}
}
