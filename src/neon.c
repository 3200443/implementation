#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "defneon.h"
#include "neon.h"

#define N 2
#define VMIN 20
#define VMAX 255 //V est entre 2 et 2^m-1 avec m le nombre de bits des donnees ici 8 => https://hal.inria.fr/hal-01130889/document
#define VINI 35
#define NBIMAGES 3


#define j0 0
#define j1 320
#define i0 0
#define i1 240



void dup_vui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
/* --------------------------------------------------------------------- */
{
    int i, j;
    
    vuint8 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u8(&X[i][j]);
            vst1q_u8(&Y[i][j], x);
        }
    }
}

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

void erosion3x3_SIMD(uint8 **It,uint8 **It1,long vi0,long vi1,long vj0,long vj1)
{

    vuint8 l1;
    vuint8 l2;
    vuint8 l3;

    vuint8 result1,result2,result3;

    vuint8 temp;
    vuint8 right,left;

    //premiere ligne : prologue
    int j = vj0;
    int i = vi0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);

    result1 = vandq_u8(l1,l2);
    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //ici left contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=vj1;j+=16)
    {
        l1 = vld1q_u8(&It[i+0][j]);
        l2 = vld1q_u8(&It[i+1][j]);

        result2 = vandq_u8(l1,l2);

        left = vextq_u8(result2,result1,15);
    

        result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        right = vextq_u8(result2,result1,1);

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }
    temp = vrev32q_u8(result1);
    left = vextq_u8(temp,result1,15);

    result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);

    j= vj0;
    i++;
    // corps de boucle
    for(i;i<vi1;i++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);

        temp =  vandq_u8(l1,l2);
        result1 = vandq_u8(temp,l3);

        temp = vrev32q_u8(result1);
        right = vextq_u8(result1,temp,1);

        //j vaut vj0 donc on insere un zero a gauche de left
        
        j++;
        for(j;j<=vj1;j+=16)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);

            temp =  vandq_u8(l1,l2);
            result2 = vandq_u8(temp,l3);

            left = vextq_u8(result2,result1,15);

            result3 = vandq_u8(right,vandq_u8(left,result1));

            right = vextq_u8(result2,result1,1);

            result1 = result2;

            vst1q_u8(&It1[i][j-1], result3);
        }

        temp = vrev32q_u8(result1);
        left = vextq_u8(temp,result1,15);

        result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        vst1q_u8(&It1[i][j-1],result3);

        j= vj0;
    }

    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vandq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //j vaut vj0 donc on insere un zero a gauche de left
    
    j++;
    for(j;j<=vj1;j+=16)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);

        result2 = vandq_u8(l1,l2);

        left = vextq_u8(result2,result1,15);

        result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        right = vextq_u8(result2,result1,1);

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }

    temp = vrev32q_u8(result1);
    left = vextq_u8(temp,result1,15);

    result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);
}

void dilatation3x3_SIMD(uint8 **It,uint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
    vuint8 l1;
    vuint8 l2;
    vuint8 l3;

    vuint8 result1,result2,result3;

    vuint8 temp;
    vuint8 right,left;


    //premiere ligne : prologue
    int j = vj0;
    int i = vi0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);

    result1 =  vorrq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //ici left contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=vj1;j+=16)
    {
        l1 = vld1q_u8(&It[i+0][j]);
        l2 = vld1q_u8(&It[i+1][j]);

        result2 = vorrq_u8(l1,l2);

        left = vextq_u8(result2,result1,15);

        result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        right = vextq_u8(result2,result1,1);

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }

    temp = vrev32q_u8(result1);
    left = vextq_u8(temp,result1,15);

    result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);

    j= vj0;
    i++;
    // corps de boucle
    for(i;i<vi1;i++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);

        temp =  vorrq_u8(l1,l2);
        result1 = vorrq_u8(temp,l3);

        temp = vrev32q_u8(result1);
        right = vextq_u8(result1,temp,1);

        //j vaut vj0 donc on insere un zero a gauche de left
        //left = vorrq_u8(left,or_droit);
        
        j++;
        for(j;j<=vj1;j+=16)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);

            temp =  vorrq_u8(l1,l2);
            result2 = vorrq_u8(temp,l3);

            left = vextq_u8(result2,result1,15);

            result3 = vorrq_u8(right,vorrq_u8(left,result1));

            right = vextq_u8(result2,result1,1);

            result1 = result2;

            vst1q_u8(&It1[i][j-1], result3);
        }

        temp = vrev32q_u8(result1);
        left = vextq_u8(temp,result1,15);

        result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        vst1q_u8(&It1[i][j-1],result3);

        j= vj0;
    }

    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vorrq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //j vaut vj0 donc on insere un zero a gauche de left
    
    j++;
    for(j;j<=vj1;j+=16)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);

        result2 = vorrq_u8(l1,l2);

        left = vextq_u8(result2,result1,15);
        
        result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

        right = vextq_u8(result2,result1,1);

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }
    //derniere ligne


    temp = vrev32q_u8(result1);
    left = vextq_u8(temp,result1,15);

    result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);
}

void test_routine_FrameDifference_SSE2(int seuil,uint8***It, uint8*** Et,uint8*** It1)
{
    for(int i = 1; i <= NBIMAGES; i++)
    {
        routine_FrameDifference_SSE2(It[i], Itm1[i], Et|i], vi0, vi1, vj0, vj1, seuil);
        dup_vui8matrix(vIt, vi0, vi1, vj0, vj1, vItm1);
    }
}
void test_routine_sigmaDelta_SSE2()
{
    char nomImageLoad[50];// = "car3/car_3";
    char nomImageSave[50];// = "car3Sigma/car_3"
    long nrl, nrh, ncl, nch;

    //Partie scalaire
    uint8 **Itm1 =  LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
    uint8 **It = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Et = ui8matrix(nrl, nrh, ncl, nch);

    // Partie vecteur
    int vi0, vi1, vj0, vj1; //Indices SIMD
    s2v(nrl, nrh, ncl, nch, 16, &vi0, &vi1, &vj0, &vj1); //Recuperation des seuils SIMD
    int nrow=vi1-vi0+1,ncol=vj1-vj0+1;

    vuint8 ** vItm1 = vui8matrix_s(nrl, nrh, ncl, nch); //Creation d'une matrice SIMD avec les indices scalaires
    vuint8 ** vIt = vui8matrix_s(nrl, nrh, ncl, nch);
    vuint8 ** vEt = vui8matrix_s(nrl, nrh, ncl, nch);

    vuint8 ** vMt = vui8matrix_s(nrl, nrh, ncl, nch);
    vuint8 ** vMtm1 = vui8matrix_s(nrl, nrh, ncl, nch);

    vuint8 ** vVt = vui8matrix_s(nrl, nrh, ncl, nch);
    vuint8 ** vVtm1 = vui8matrix_s(nrl, nrh, ncl, nch);


    MatScal2MatSIMD(vItm1, Itm1, vi0, vi1, vj0, vj1);    //On fait la copie de l'image dans une matrice SIMD
    routine_SigmaDelta_step0SSE2(vItm1, vMtm1,vVtm1, vi0, vi1, vj0, vj1);

    for(int i = 1; i <= NBIMAGES; i++)
    {
        sprintf(nomImageLoad, "car3/car_3%03d.pgm", i);//Image a t
        MLoadPGM_ui8matrix(nomImageLoad, nrl, nrh, ncl, nch, It);
        MatScal2MatSIMD(vIt, It,  vi0, vi1, vj0, vj1);

        routine_SigmaDelta_1stepSSE2(vIt, vItm1, vVt, vVtm1, vMt, vMtm1, vEt, vi0, vi1, vj0, vj1);
        MatSIMD2MatScal(vEt, Et, vi0, vi1, vj0, vj1);    //On fait la copie d'une matrice SIMD dans une image normale
        sprintf(nomImageSave, "car3SigmaSIMD/car_3%03d.pgm", i);
        SavePGM_ui8matrix(Et, nrl, nrh, ncl, nch, nomImageSave);
        dup_vui8matrix(vIt, vi0, vi1, vj0, vj1, vItm1);
        dup_vui8matrix(vVt, vi0, vi1, vj0, vj1, vVtm1);
        dup_vui8matrix(vMt, vi0, vi1, vj0, vj1, vMtm1);



    }
}

int main(int argc, char* argv[])
{
    //TODO
    // 1) Nothing there yet :-)
    // 2) Declarer le tableau de matrice image de depart et resultat
    // 3) Assigner la valeur des images aux tableaux
    // 4) Appeler les fonctions :)
    
    return 0;
}