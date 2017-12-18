#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "defneon.h"
#include "neon.h"

#include "car_3000.h"
#include "car_3001.h"
#include "car_3002.h"

#define SEUIL 20
#define N 2
#define VMIN 20
#define VMAX 255
#define VINI 35
#define NBIMAGES 3

#define FORCE_CAST(var, type) *(type*)&var


#define j0 0
#define j1 79
#define i0 0
#define i1 59
#define SIZE 80



int vj0b = 0;

typedef struct charbin
{
    uint8 e0:1;
    uint8 e1:1;
    uint8 e2:1;
    uint8 e3:1;
    uint8 e4:1;
    uint8 e5:1;
    uint8 e6:1;
    uint8 e7:1;
}Charbin;

void char2bin(uint8**m1, uint8**m2)// m2 is allocated
{
    Charbin temp;
    int i,j,c;
    for(i=i0; i <= i1; i++)
    {
        c = 0;
        for(j=j0;j<=j1-7;j+=8)
        {
            temp.e0 = m1[i][j+0] & 1;
            temp.e1 = m1[i][j+1] & 1;
            temp.e2 = m1[i][j+2] & 1;
            temp.e3 = m1[i][j+3] & 1;
            temp.e4 = m1[i][j+4] & 1;
            temp.e5 = m1[i][j+5] & 1;
            temp.e6 = m1[i][j+6] & 1;
            temp.e7 = m1[i][j+7] & 1;

            m2[i][c] = FORCE_CAST(temp,uint8);

            c++;
        }
        temp.e1 = 0;
        temp.e2 = 0;
        temp.e3 = 0;
        temp.e4 = 0;
        temp.e5 = 0;
        temp.e6 = 0;
        temp.e7 = 0;
        switch(j1 -j)
        {
            case (-1) :
            break;
            case 6:
            temp.e6 = m1[i][j+6] & 1;
            case 5:
            temp.e5 = m1[i][j+5] & 1;
            case 4:
            temp.e4 = m1[i][j+4] & 1;
            case 3:
            temp.e3 = m1[i][j+3] & 1;
            case 2:
            temp.e2 = m1[i][j+2] & 1;
            case 1:
            temp.e1 = m1[i][j+1] & 1;
            case 0:
            temp.e0 = m1[i][j+0] & 1;
            m2[i][c] = FORCE_CAST(temp,uint8);
            break;
        }
    }
}

void bin2char(uint8**m1, uint8**m2)// m2 is allocated
{
    Charbin temp,temp1;
    int i,j,k,l;
    for(i=i0; i <= i1, i++)
    {
        c = 0;
        for(j=j0;j<=j1-7;j+=8)
        {
            temp = FORCE_CAST(m1[i][c],Charbin);
            for(k = 0; k <8; k++)
            {
                temp1.e0 = 0;
                temp1.e1 = 0;
                temp1.e2 = 0;
                temp1.e3 = 0;
                temp1.e4 = 0;
                temp1.e5 = 0;
                temp1.e6 = 0;
                temp1.e7 = 0;

                for(l=0;l <8;l++)
                {
                    temp1 |= ((temp & (1 << k)) << l)
                }
                m2[i][j+k] = FORCE_CAST(temp1,uint8);
            }
            c++;
        }
        switch(j1 -j)
        {
            case (-1) :
                break;
            default:
                temp = FORCE_CAST(m1[i][c],Charbin);
                for(k = 0; k <=vj1; k++)
                {
                    temp1.e0 = 0;
                    temp1.e1 = 0;
                    temp1.e2 = 0;
                    temp1.e3 = 0;
                    temp1.e4 = 0;
                    temp1.e5 = 0;
                    temp1.e6 = 0;
                    temp1.e7 = 0;

                    for(l=0;l <8;l++)
                    {
                        temp1 |= ((temp & (1 << k)) << l)
                    }
                    m2[i][j+k] = FORCE_CAST(temp1,uint8);
                }
                break;
        }
    }
}


void routine_FrameDifference_SSE2(uint8 **It, uint8 **Itm1, uint8 **Et)
{
    vuint8 tmpIt;
    vuint8 tmpItm1;
    vuint8 tmpOt;
    vuint8 res;
    vuint8 vSeuil = vdupq_n_u8(SEUIL);

    int i, j;
    for(i = i0; i <= i1; i++ )
    {
        for(j = j0; j <= j1; j+=16)
        {
            //Calcul de Ot, image de difference
            tmpIt =  vld1q_u8(&It[i][j]);
            tmpItm1 =  vld1q_u8(&Itm1[i][j]);

            tmpOt = vabdq_u8(tmpIt,tmpItm1);

            res = vcgeq_u8(tmpOt,vSeuil); //Met 1 si inferieur au seuil et 0 si superieur

            vst1q_u8(&Et[i][j], res);
        }
    }
}

void erosion3x3_SIMD(uint8 **It,uint8 **It1)
{

    vuint8 l1;
    vuint8 l2;
    vuint8 l3;

    vuint8 result1,result2,result3;

    vuint8 temp;
    vuint8 right,left;

    //premiere ligne : prologue
    int j = j0;
    int i = i0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);

    result1 = vandq_u8(l1,l2);
    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //ici left contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=j1;j+=16)
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

    j= j0;
    i++;
    // corps de boucle
    for(i;i<i1;i++)
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
        for(j;j<=j1;j+=16)
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

        j= j0;
    }

    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vandq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //j vaut vj0 donc on insere un zero a gauche de left
    
    j++;
    for(j;j<=j1;j+=16)
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

void dilatation3x3_SIMD(uint8 **It,uint8 **It1)
{
    vuint8 l1;
    vuint8 l2;
    vuint8 l3;

    vuint8 result1,result2,result3;

    vuint8 temp;
    vuint8 right,left;


    //premiere ligne : prologue
    int j = j0;
    int i = i0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);

    result1 =  vorrq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //ici left contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=j1;j+=16)
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

    j= j0;
    i++;
    // corps de boucle
    for(i;i<i1;i++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);

        temp =  vorrq_u8(l1,l2);
        result1 = vorrq_u8(temp,l3);

        temp = vrev32q_u8(result1);
        right = vextq_u8(result1,temp,1);

        //j vaut j0 donc on insere un zero a gauche de left
        //left = vorrq_u8(left,or_droit);
        
        j++;
        for(j;j<=j1;j+=16)
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

        j= j0;
    }

    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vorrq_u8(l1,l2);

    temp = vrev32q_u8(result1);
    right = vextq_u8(result1,temp,1);

    //j vaut j0 donc on insere un zero a gauche de left
    
    j++;
    for(j;j<=j1;j+=16)
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

void test_routine_FrameDifference_SSE2(uint8 ***It, uint8** Et)
{
    int i;
    for( i = 1; i <= NBIMAGES; i++)
    {
        routine_FrameDifference_SSE2(It[i], It[i-1], Et);
    }
}


void dilatation3x3_SIMD_FB(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1) // vj1 doit etre le vj1b bianaire §!!!!§
{
    vuint8 l1;
    vuint8 l2;
    vuint8 l3;
    vuint8 l4;
    vuint8 l5;

    vuint8 result1,result2,result3;
    vuint8 resultb1,resultb2,resultb3;
    vuint8 resultc1,resultc2,resultc3;

    vuint8 temp;
    vuint8 left,right;
    vuint8 leftb,rightb;
    vuint8 leftc,rightc;

    //premiere ligne : prologue
    int j = vj0;
    int i = vi0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);
    l3 = vld1q_u8(&It[i+2][j]);
    l4 = vld1q_u8(&It[i+3][j]);

    result1     = vorrq_u8(l1,l2);
    resultb1    = vorrq_u8(result1,l3);
    resultc1    = vorrq_u8(vorrq_u8(l2,l3),l4);

    left  = result1 >>1;
    leftb = result1 >>;
    leftc = result1 >>1;

    //ici right contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=vj1;j++)
    {
        l1 = vld1q_u8(&It[i+0][j]);
        l2 = vld1q_u8(&It[i+1][j]);
        l3 = vld1q_u8(&It[i+2][j]);
        l4 = vld1q_u8(&It[i+3][j]);

        result2 = vorrq_u8(l1,l2);
        resultb2 = vorrq_u8(result2,l3);
        resultc2 = vorrq_u8(vorrq_u8(l2,l3),l4);

        right   = result1  << 1;
        right  |= (result2 >> 127);
        rightb  = resultb1 << 1;
        rightb |= resultb2 >> 127;
        rightc  = resultc1 << 1;
        rightc |= resultc2 >> 127;

        result3 = vorrq_u8(left,vorrq_u8(right,result1)); // Valeur finale de IT1[i][j-1]
        resultb3 = vorrq_u8(leftb,vorrq_u8(rightb,resultb1));
        resultc3 = vorrq_u8(leftc,vorrq_u8(rightc,resultc1));

        left   = result2  >> 1;
        left  |= result1  << 127;
        leftb  = resultb2 >> 1;
        leftb |= resultb1 << 127;
        leftc  = resultc2 >> 1;
        leftc |= resultc1 << 127;

        result1 = result2;
        resultb1 = resultb2;
        resultc1 = resultc2;

        vst1q_u8(&It1[i+0][j-1], result3);
        vst1q_u8(&It1[i+1][j-1], resultb3);
        vst1q_u8(&It1[i+2][j-1], resultc3);
    }

    right  = result1  << 1;
    rightb = resultb1 << 1;
    rightc = resultc1 << 1;

    result3 = vorrq_u8(left,vorrq_u8(right,result1)); // Valeur finale de IT1[i][j-1]
    resultb3 = vorrq_u8(leftb,vorrq_u8(rightb,resultb1));
    resultc3 = vorrq_u8(leftc,vorrq_u8(rightc,resultc1));

    vst1q_u8(&It1[i+0][j-1],result3);
    vst1q_u8(&It1[i+1][j-1],resultb3);
    vst1q_u8(&It1[i+2][j-1],resultc3);

    j= vj0;
    i+=3;
    // corps de boucle
    for(i;i<vi1-3;i+=3)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);
        l4 = vld1q_u8(&It[i+2][j]);
        l5 = vld1q_u8(&It[i+3][j]);

        temp =  vorrq_u8(l2,l3);
        result1 = vorrq_u8(temp,l1);
        resultb1 = vorrq_u8(temp,l4);
        temp =  vorrq_u8(l3,l4);
        resultc1 = vorrq_u8(temp,l5);

        left  = result1  >> 1;
        leftb = resultb1 >> 1;
        leftc = resultc1 >> 1;

        //j vaut vj0 donc on insere un zero a gauche de right
        
        j++;
        for(j;j<=vj1;j++)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);
            l4 = vld1q_u8(&It[i+2][j]);
            l5 = vld1q_u8(&It[i+3][j]);

            temp     =  vorrq_u8(l2,l3);
            result2  =  vorrq_u8(temp,l1);
            resultb2 =  vorrq_u8(temp,l4);
            temp     =  vorrq_u8(l3,l4);
            resultc2 =  vorrq_u8(temp,l5);

            right   = result1  << 1;
            right  |= (result2 >> 127); // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant
            rightb  = resultb1 << 1;
            rightb |= resultb2 >> 127;
            rightc  = resultc1 << 1;
            rightc |= resultc2 >> 127;

            result3 = vorrq_u8(left,vorrq_u8(right,result1));
            resultb3 = vorrq_u8(leftb,vorrq_u8(rightb,resultb1));
            resultc3 = vorrq_u8(leftc,vorrq_u8(rightc,resultc1));

            left   = result2  >> 1;
            left  |= result1  << 127;
            leftb  = resultb2 >> 1;
            leftb |= resultb1 << 127;
            leftc  = resultc2 >> 1;
            leftc |= resultc1 << 127;

            result1  = result2;
            resultb1 = resultb2;
            resultc1 = resultc2;

            vst1q_u8(&It1[i][j-1], result3);
            vst1q_u8(&It1[i+1][j-1], resultb3);
            vst1q_u8(&It1[i+2][j-1], resultc3);
        }

        right  = result1  << 1;
        rightb = resultb1 << 1;
        rightc = resultc1 << 1;

        result3  = vorrq_u8(left ,vorrq_u8(right ,result1  )); // Valeur finale de IT1[i][j-1]
        resultb3 = vorrq_u8(leftb,vorrq_u8(rightb,resultb1 ));
        resultc3 = vorrq_u8(leftc,vorrq_u8(rightc,resultc1 ));

        vst1q_u8(&It1[i+0][j-1],result3) ;
        vst1q_u8(&It1[i+1][j-1],resultb3);
        vst1q_u8(&It1[i+2][j-1],resultc3);

        j= vj0;
    }



    //fin normale
    for(i;i<vi1;i++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);

        temp    = vorrq_u8(l1,l2);
        result1 = vorrq_u8(temp,l3);

        left = result1 >> 1;

        //j vaut vj0 donc on insere un zero a gauche de right
        
        j++;
        for(j;j<=vj1;j++)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);

            temp    = vorrq_u8(l1,l2);
            result2 = vorrq_u8(temp,l3);

            right  = result1 << 1;
            right |= result2 >> 127; // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

            result3 = vorrq_u8(left,vorrq_u8(right,result1));

            left  = result2 >> 1;
            left |= result1 << 127;

            result1 = result2;

            vst1q_u8(&It1[i][j-1], result3);
        }

        right = result1 << 1;

        result3 = vorrq_u8(left,vorrq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

        vst1q_u8(&It1[i][j-1],result3);

        j= vj0;
    }



    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vorrq_u8(l1,l2);

    left = result1 >> 1;

    //j vaut vj0 donc on insere un zero a gauche de right
    
    j++;
    for(j;j<=vj1;j++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);

        result2 = vorrq_u8(l1,l2);

        right  = result1 << 1;
        right |= result2 >> 127; // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

        result3 = vorrq_u8(left,vorrq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

        left  = result2 >> 1;
        left |= result1 << 127;

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }

    right = result1 << 1;

    result3 = vorrq_u8(left,vorrq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);
}

void erosion3x3_SIMD_FB(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1) // vj1 doit etre le vj1b bianaire §!!!!§
{
    vuint8 l1;
    vuint8 l2;
    vuint8 l3;
    vuint8 l4;
    vuint8 l5;

    vuint8 result1,result2,result3;
    vuint8 resultb1,resultb2,resultb3;
    vuint8 resultc1,resultc2,resultc3;

    vuint8 temp;
    vuint8 left,right;
    vuint8 leftb,rightb;
    vuint8 leftc,rightc;

    //premiere ligne : prologue
    int j = vj0;
    int i = vi0;

    l1 = vld1q_u8(&It[i+0][j]);
    l2 = vld1q_u8(&It[i+1][j]);
    l3 = vld1q_u8(&It[i+2][j]);
    l4 = vld1q_u8(&It[i+3][j]);

    result1     = vandq_u8(l1,l2);
    resultb1    = vandq_u8(result1,l3);
    resultc1    = vandq_u8(vandq_u8(l2,l3),l4);

    left  = result1 >>1;
    leftb = result1 >>;
    leftc = result1 >>1;

    //ici right contient result1 decallé vers la droite avec 255 comme valeur de gauche
    //
    j++;
    for(j;j<=vj1;j++)
    {
        l1 = vld1q_u8(&It[i+0][j]);
        l2 = vld1q_u8(&It[i+1][j]);
        l3 = vld1q_u8(&It[i+2][j]);
        l4 = vld1q_u8(&It[i+3][j]);

        result2  = vandq_u8(l1,l2);
        resultb2 = vandq_u8(result2,l3);
        resultc2 = vandq_u8(vandq_u8(l2,l3),l4);

        right   = result1  << 1;
        right  |= (result2 >> 127);
        rightb  = resultb1 << 1;
        rightb |= resultb2 >> 127;
        rightc  = resultc1 << 1;
        rightc |= resultc2 >> 127;

        result3 = vandq_u8(left,vandq_u8(right,result1)); // Valeur finale de IT1[i][j-1]
        resultb3 = vandq_u8(leftb,vandq_u8(rightb,resultb1));
        resultc3 = vandq_u8(leftc,vandq_u8(rightc,resultc1));

        left   = result2  >> 1;
        left  |= result1  << 127;
        leftb  = resultb2 >> 1;
        leftb |= resultb1 << 127;
        leftc  = resultc2 >> 1;
        leftc |= resultc1 << 127;

        result1 = result2;
        resultb1 = resultb2;
        resultc1 = resultc2;

        vst1q_u8(&It1[i+0][j-1], result3);
        vst1q_u8(&It1[i+1][j-1], resultb3);
        vst1q_u8(&It1[i+2][j-1], resultc3);
    }

    right  = result1  << 1;
    rightb = resultb1 << 1;
    rightc = resultc1 << 1;

    result3 = vandq_u8(left,vandq_u8(right,result1)); // Valeur finale de IT1[i][j-1]
    resultb3 = vandq_u8(leftb,vandq_u8(rightb,resultb1));
    resultc3 = vandq_u8(leftc,vandq_u8(rightc,resultc1));

    vst1q_u8(&It1[i+0][j-1],result3);
    vst1q_u8(&It1[i+1][j-1],resultb3);
    vst1q_u8(&It1[i+2][j-1],resultc3);

    j= vj0;
    i+=3;

    // corps de boucle
    for(i;i<vi1-3;i+=3)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);
        l4 = vld1q_u8(&It[i+2][j]);
        l5 = vld1q_u8(&It[i+3][j]);

        temp =  vandq_u8(l2,l3);
        result1 = vandq_u8(temp,l1);
        resultb1 = vandq_u8(temp,l4);
        temp =  vandq_u8(l3,l4);
        resultc1 = vandq_u8(temp,l5);

        left  = result1  >> 1;
        leftb = resultb1 >> 1;
        leftc = resultc1 >> 1;

        //j vaut vj0 donc on insere un zero a gauche de right
        
        j++;
        for(j;j<=vj1;j++)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);
            l4 = vld1q_u8(&It[i+2][j]);
            l5 = vld1q_u8(&It[i+3][j]);

            temp     =  vandq_u8(l2,l3);
            result2  =  vandq_u8(temp,l1);
            resultb2 =  vandq_u8(temp,l4);
            temp     =  vandq_u8(l3,l4);
            resultc2 =  vandq_u8(temp,l5);

            right   = result1  << 1;
            right  |= (result2 >> 127); // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant
            rightb  = resultb1 << 1;
            rightb |= resultb2 >> 127;
            rightc  = resultc1 << 1;
            rightc |= resultc2 >> 127;

            result3  = vandq_u8(left,vandq_u8(right,result1));
            resultb3 = vandq_u8(leftb,vandq_u8(rightb,resultb1));
            resultc3 = vandq_u8(leftc,vandq_u8(rightc,resultc1));

            left   = result2  >> 1;
            left  |= result1  << 127;
            leftb  = resultb2 >> 1;
            leftb |= resultb1 << 127;
            leftc  = resultc2 >> 1;
            leftc |= resultc1 << 127;

            result1  = result2;
            resultb1 = resultb2;
            resultc1 = resultc2;

            vst1q_u8(&It1[i][j-1], result3);
            vst1q_u8(&It1[i+1][j-1], resultb3);
            vst1q_u8(&It1[i+2][j-1], resultc3);
        }

        right  = result1  << 1;
        rightb = resultb1 << 1;
        rightc = resultc1 << 1;

        result3  = vandq_u8(left ,vandq_u8(right ,result1  )); // Valeur finale de IT1[i][j-1]
        resultb3 = vandq_u8(leftb,vandq_u8(rightb,resultb1 ));
        resultc3 = vandq_u8(leftc,vandq_u8(rightc,resultc1 ));

        vst1q_u8(&It1[i+0][j-1],result3) ;
        vst1q_u8(&It1[i+1][j-1],resultb3);
        vst1q_u8(&It1[i+2][j-1],resultc3);

        j= vj0;
    }



    //fin normale
    for(i;i<vi1;i++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);
        l3 = vld1q_u8(&It[i+1][j]);

        temp    = vandq_u8(l1,l2);
        result1 = vandq_u8(temp,l3);

        left = result1 >> 1;

        //j vaut vj0 donc on insere un zero a gauche de right
        
        j++;
        for(j;j<=vj1;j++)
        {
            l1 = vld1q_u8(&It[i-1][j]);
            l2 = vld1q_u8(&It[i+0][j]);
            l3 = vld1q_u8(&It[i+1][j]);

            temp    = vandq_u8(l1,l2);
            result2 = vandq_u8(temp,l3);

            right  = result1 << 1;
            right |= result2 >> 127; // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

            result3 = vandq_u8(left,vandq_u8(right,result1));

            left  = result2 >> 1;
            left |= result1 << 127;

            result1 = result2;

            vst1q_u8(&It1[i][j-1], result3);
        }

        right = result1 << 1;

        result3 = vandq_u8(left,vandq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

        vst1q_u8(&It1[i][j-1],result3);

        j= vj0;
    }



    l1 = vld1q_u8(&It[i-1][j]);
    l2 = vld1q_u8(&It[i+0][j]);

    result1 = vandq_u8(l1,l2);

    left = result1 >> 1;

    //j vaut vj0 donc on insere un zero a gauche de right

    j++;
    for(j;j<=vj1;j++)
    {
        l1 = vld1q_u8(&It[i-1][j]);
        l2 = vld1q_u8(&It[i+0][j]);

        result2 = vandq_u8(l1,l2);

        right  = result1 << 1;
        right |= result2 >> 127; // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

        result3 = vandq_u8(left,vandq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

        left  = result2 >> 1;
        left |= result1 << 127;

        result1 = result2;

        vst1q_u8(&It1[i][j-1], result3);
    }

    right  = result1 << 1;

    result3 = vandq_u8(left,vandq_u8(right,result1)); // Valeur finale de IT1[i][j-1]

    vst1q_u8(&It1[i][j-1],result3);
}

int main(int argc, char* argv[])
{
    //TODO
    // 1) Nothing there yet :-)
    // 2) Declarer le tableau de matrice image de depart et resultat
    // 3) Assigner la valeur des images aux tableaux
    // 4) Appeler les fonctions :)
    // 
    // 
    vj1b = (j0%8 == 0 ? (j0>>3):((j0>>8)+1));
    while (vj1b%16)
        vj1b++; //dans le cas present on devrait avoir 48 a la fin de l'exectution

	uint8 *car_bundle[]={car_3000, car_3001, car_3002};
	uint8 Et[i1+1][j1+1];
	uint8 Et1[i1+1][j1+1];
	int k;

	for(k = 1; k <= NBIMAGES; k++)
	{
		routine_FrameDifference_SSE2(car_bundle[k], car_bundle[k-1], Et);
	}

    return 0;
}

/*
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
    routine_SigmaDelta_step0SSE2(vItm1, vMtm1,vVtm1);

    int i;
    for( i = 1; i <= NBIMAGES; i++)
    {
        sprintf(nomImageLoad, "car3/car_3%03d.pgm", i);//Image a t
        MLoadPGM_ui8matrix(nomImageLoad, nrl, nrh, ncl, nch, It);
        MatScal2MatSIMD(vIt, It,  vi0, vi1, vj0, vj1);

        routine_SigmaDelta_1stepSSE2(vIt, vItm1, vVt, vVtm1, vMt, vMtm1, vEt);
        MatSIMD2MatScal(vEt, Et, vi0, vi1, vj0, vj1);    //On fait la copie d'une matrice SIMD dans une image normale
        sprintf(nomImageSave, "car3SigmaSIMD/car_3%03d.pgm", i);
        SavePGM_ui8matrix(Et, nrl, nrh, ncl, nch, nomImageSave);
        dup_vui8matrix(vIt, vi0, vi1, vj0, vj1, vItm1);
        dup_vui8matrix(vVt, vi0, vi1, vj0, vj1, vVtm1);
        dup_vui8matrix(vMt, vi0, vi1, vj0, vj1, vMtm1);



    }
}
*/
/*
void routine_SigmaDelta_step0SSE2(uint8** I, uint8 **M, uint8 **V)
{
    vuint8 tmpM;
    vuint8 tmpV;
    vuint8 tmpI;
    vuint8 ecartTypeIni = vdupq_n_u8(VINI);

    int i, j;
    for(i = i0; i <= i1; i++ )
    {
        for(j = j0; j <= j1; j+=16)
        {

            tmpI = vld1q_u8(&I[i][j]); //M[i][j] = I[i][j];
            vst1q_u8(&M[i][j], tmpI);
            vst1q_u8(&V[i][j], ecartTypeIni);    //V[i][j] = VINI; //Au depart a VMIN mais il y avait beaucoup de mouvement des le debut, a 10 ça marche mieux
        }
    }
}

void routine_SigmaDelta_1stepSSE2(uint8 **It, uint8 **Itm1, uint8**Vt, uint8 **Vtm1, uint8**Mt, uint8 **Mtm1, uint8 **Et)
{
    vuint8 tmpIt, tmpMt, tmpVt;
    vuint8 tmpItm1, tmpMtm1, tmpVtm1;
    vuint8 tmpOt;
    vuint8 pixelBlanc = vdupq_n_u8(255);
    vuint8 tmpEt;
    vuint8 un = vdupq_n_u8(1);
    vuint8 VMAXSIMD = vdupq_n_u8(VMAX);
    vuint8 VMINSIMD = vdupq_n_u8(VMIN);
    vuint8 maxSChar = vdupq_n_u8(128);
    //Les comparaisons se font en signe donc il faut sub 128 pour que ça fasse une comparaison correcte
    //255 devient 128, 128 => 0 et 0 => -128
    //Si on ne fait pas ca, on a 128 < 128 qui est faux

    int i, j, k;
    for( i = i0; i <= i1; i++ )
    {
        for( j = j0; j <= j1; j++)
        {

            //Step1
            tmpMtm1 = vld1q_u8(&Mtm1[i][j]);
            tmpIt = vld1q_u8(&It[i][j]);
            tmpVtm1 = vld1q_u8(&Vtm1[i][j]);

            vuint8 Mtm1Plus1 = vaddq_u8(tmpMtm1, un);
            vuint8 Mtm1Moins1 = vsubq_u8(tmpMtm1, un);
            vuint8 NfoisOt = vdupq_n_u8(0);

            vuint8 res = vcltq_u8(vsubq_u8(tmpMtm1, maxSChar), vsubq_u8(tmpIt, maxSChar));
            tmpMt = vorrq_u8(vandq_u8(res, Mtm1Plus1), vmvnq_u8(vandq_u8(res, tmpMtm1))); //Mtm1< It

            res = vcgtq_u8(vsubq_u8(tmpMtm1, maxSChar), vsubq_u8(tmpIt, maxSChar));
            tmpMt = vorrq_u8(vandq_u8(res, Mtm1Moins1), vmvnq_u8(vandq_u8(res, tmpMt))); // //Mtm1 > It


            //Step 2 Calcul matrice difference |Mt-It|
            vuint8 max = vmaxq_u8(tmpIt,tmpMt);
            vuint8 min = vminq_u8(tmpIt, tmpMt);
            tmpOt = vsubq_u8(max, min); //Le max - min donne la valeur absolue
            //Step 3 Vt Update and clamping
            for( k = 0; k < N; k++)
            {
                NfoisOt = vqaddq_u8(NfoisOt, tmpOt);
            }

            vuint8 Vtm1Plus1 = vaddq_u8(tmpVtm1, un);
            vuint8 Vtm1Moins1 = vsubq_u8(tmpVtm1, un);

            res = vcltq_u8(vsubq_u8(tmpVtm1, maxSChar), vsubq_u8(NfoisOt, maxSChar));//On soustrait 128 car la comparaison est signee
            tmpVt = vorrq_u8(vandq_u8(res, Vtm1Plus1),vmvnq_u8(vandq_u8(res, tmpVtm1))); //Vtm1< N*Ot

            res = vcgtq_u8(vsubq_u8(tmpVtm1, maxSChar), vsubq_u8(NfoisOt, maxSChar));
            tmpVt = vorrq_u8(vandq_u8(res, Vtm1Moins1), vmvnq_u8(vandq_u8(res, tmpVt))); // //Vtm1 > N* Ot

            tmpVt = vmaxq_u8(vminq_u8(tmpVt, VMAXSIMD), VMINSIMD);

            //Step 4: Et estimation
            res = vcltq_u8(vsubq_u8(tmpOt,maxSChar), vsubq_u8(tmpVt,maxSChar)); //Met 255 si inferieur a Vt et 0 si superieur

            vuint8 dest = vmvnq_u8(res);//Inverse les 255 et 0 pour avoir la bonne couleur de pixel

            vst1q_u8(&Et[i][j], dest);
            vst1q_u8(&Vt[i][j], tmpVt);
            vst1q_u8(&Mt[i][j], tmpMt);
        }
    }
}
*/