#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "defneon.h"


#define N 2
#define VMIN 20
#define VMAX 255 //V est entre 2 et 2^m-1 avec m le nombre de bits des donnees ici 8 => https://hal.inria.fr/hal-01130889/document
#define VINI 35



void routine_FrameDifference_SSE2(uint8 **It, uint8 **Itm1, uint8 **Et, long vi0,long vi1,long vj0,long vj1, uint8 seuil)
{
    vuint8 tmpIt;
    vuint8 tmpItm1;
    vuint8 tmpOt;
    vuint8 res;

    for(int i = vi0; i <= vi1; i++ )
    {
        for(int j = vj0; j <= vj1; j+=16)
        {
            //Calcul de Ot, image de difference
            tmpIt =  vld1q_u8(&It[i][j]);
            tmpItm1 =  vld1q_u8(&Itm1[i][j]);

            tmpOt = vabdq_u8(tmpIt,tmpItm1);

            res = vcgeq_u8(tmpOt,seuil); //Met 1 si inferieur au seuil et 0 si superieur

            vst1q_u8(&Et[i][j], res);
        }
    }
}


void routine_SigmaDelta_step0SSE2(uint8** I, uint8 **M, uint8 **V, long vi0, long vi1, long vj0, long vj1)
{
    vuint8 tmpM;
    vuint8 tmpV;
    vuint8 tmpI;
    vuint8 ecartTypeIni = vsetq_lane_u8(VINI);
    for(int i = vi0; i <= vi1; i++ )
    {
        for(int j = vj0; j <= vj1; j+=16)
        {

            tmpI = vld1q_u8(&I[i][j]); //M[i][j] = I[i][j];
            vst1q_u8(&M[i][j], tmpI);
            vst1q_u8(&V[i][j], ecartTypeIni);    //V[i][j] = VINI; //Au depart a VMIN mais il y avait beaucoup de mouvement des le debut, a 10 ça marche mieux
        }
    }
}



void routine_SigmaDelta_1stepSSE2(uint8 **It, uint8 **Itm1, uint8**Vt, uint8 **Vtm1, uint8**Mt, uint8 **Mtm1, uint8 **Et,  long vi0, long vi1, long vj0, long vj1 )
{
    vuint8 tmpIt, tmpMt, tmpVt;
    vuint8 tmpItm1, tmpMtm1, tmpVtm1;
    vuint8 tmpOt;
    vuint8 pixelBlanc = vsetq_lane_u8(255);
    vuint8 tmpEt;
    vuint8 un = vsetq_lane_u8(1);
    vuint8 VMAXSIMD = vsetq_lane_u8(VMAX);
    vuint8 VMINSIMD = vsetq_lane_u8(VMIN);
    vuint8 maxSChar = vsetq_lane_u8(128);
    //Les comparaisons se font en signe donc il faut sub 128 pour que ça fasse une comparaison correcte
    //255 devient 128, 128 => 0 et 0 => -128
    //Si on ne fait pas ca, on a 128 < 128 qui est faux

    for(int i = vi0; i <= vi1; i++ )
    {
        for(int j = vj0; j <= vj1; j++)
        {

            //Step1
            tmpMtm1 = vld1q_u8(&Mtm1[i][j]);
            tmpIt = vld1q_u8(&It[i][j]);
            tmpVtm1 = vld1q_u8(&Vtm1[i][j]);

            vuint8 Mtm1Plus1 = vaddq_u8(tmpMtm1, un);
            vuint8 Mtm1Moins1 = vsubq_s8(tmpMtm1, un);
            vuint8 NfoisOt = vsetq_lane_u8(0);

            vuint8 res = vcltq_u8(vsubq_s8(tmpMtm1, maxSChar), vsubq_s8(tmpIt, maxSChar));
            tmpMt = vorrq_u8(vandq_u8(res, Mtm1Plus1), vmvnq_u8(res, tmpMtm1)); //Mtm1< It

            res = vcgtq_u8(vsubq_s8(tmpMtm1, maxSChar), vsubq_s8(tmpIt, maxSChar));
            tmpMt = vorrq_u8(vandq_u8(res, Mtm1Moins1), vmvnq_u8(res, tmpMt)); // //Mtm1 > It


            //Step 2 Calcul matrice difference |Mt-It|
            vuint8 max = vmaxq_u8(tmpIt,tmpMt);
            vuint8 min = vminq_u8(tmpIt, tmpMt);
            tmpOt = vsubq_s8(max, min); //Le max - min donne la valeur absolue
            //Step 3 Vt Update and clamping
            for(int k = 0; k < N; k++)
            {
                NfoisOt = vqaddq_u8(NfoisOt, tmpOt);
            }

            vuint8 Vtm1Plus1 = vaddq_u8(tmpVtm1, un);
            vuint8 Vtm1Moins1 = vsubq_s8(tmpVtm1, un);

            res = vcltq_u8(vsubq_s8(tmpVtm1, maxSChar), vsubq_s8(NfoisOt, maxSChar));//On soustrait 128 car la comparaison est signee
            tmpVt = vorrq_u8(vandq_u8(res, Vtm1Plus1), vmvnq_u8(res, tmpVtm1)); //Vtm1< N*Ot

            res = vcgtq_u8(vsubq_s8(tmpVtm1, maxSChar), vsubq_s8(NfoisOt, maxSChar));
            tmpVt = vorrq_u8(vandq_u8(res, Vtm1Moins1), vmvnq_u8(res, tmpVt)); // //Vtm1 > N* Ot

            tmpVt = vmaxq_u8(vminq_u8(tmpVt, VMAXSIMD), VMINSIMD);

            //Step 4: Et estimation
            res = vcltq_u8(vsubq_s8(tmpOt,maxSChar), vsubq_s8(tmpVt,maxSChar)); //Met 255 si inferieur a Vt et 0 si superieur

            vuint8 dest = vmvnq_u8(res, pixelBlanc);//Inverse les 255 et 0 pour avoir la bonne couleur de pixel

            vst1q_u8(&Et[i][j], dest);
            vst1q_u8(&Vt[i][j], tmpVt);
            vst1q_u8(&Mt[i][j], tmpMt);
        }
    }

}
