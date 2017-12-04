#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "defneon.h"

#include "morpho_simd.h"

//ui8matrix();

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

			right = _mm_slli_si128(result2,1);
			right = vorrq_u8(right,_mm_srli_si128(result1,15));

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

void erosion3x3_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
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

	// corps de boucle
	for(i;i<=vi1;i++)
	{
		l1 = vld1q_u8(&It[i-1][j]);
		l2 = vld1q_u8(&It[i+0][j]);
		l3 = vld1q_u8(&It[i+1][j]);

		temp =  vandq_u8(l1,l2);
		result1 = vandq_u8(temp,l3);

		right = _mm_slli_si128(result1,1);

		//j vaut vj0 donc on insere un zero a gauche de left
		
		j++;
		for(j;j<=vj1;j+=16) //densité arithmétique de 2.5 : (10)/4
		{
			l1 = vld1q_u8(&It[i-1][j]);
			l2 = vld1q_u8(&It[i+0][j]);
			l3 = vld1q_u8(&It[i+1][j]);

			temp =  vandq_u8(l1,l2);
			result2 = vandq_u8(temp,l3);

			left = _mm_srli_si128(result1,1);
			left = vorrq_u8(left,_mm_slli_si128(result2,15)); // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

			result3 = vandq_u8(right,vandq_u8(left,result1));

			right = _mm_slli_si128(result2,1);
			right = vorrq_u8(right,_mm_srli_si128(result1,15));

			result1 = result2;

			vst1q_u8(&It1[i][j-1], result3);
		}

		left = _mm_srli_si128(result1,1);

		result3 = vandq_u8(right,vandq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

		vst1q_u8(&It1[i][j-1],result3);

		j= vj0;
	}
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

void dilatation3x3_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
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

	
	// corps de boucle
	for(i;i<=vi1;i++)
	{
		l1 = vld1q_u8(&It[i-1][j]);
		l2 = vld1q_u8(&It[i+0][j]);
		l3 = vld1q_u8(&It[i+1][j]);

		temp =  vorrq_u8(l1,l2);
		result1 = vorrq_u8(temp,l3);

		right = _mm_slli_si128(result1,1);

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

			left = _mm_srli_si128(result1,1);
			left = vorrq_u8(left,_mm_slli_si128(result2,15)); // On complete le vecteur gauche en ajoutant a sa doite la valeur de gauvhe du vecteur suivant

			result3 = vorrq_u8(right,vorrq_u8(left,result1));

			right = _mm_slli_si128(result2,1);
			right = vorrq_u8(right,_mm_srli_si128(result1,15));

			result1 = result2;

			vst1q_u8(&It1[i][j-1], result3);
		}

		left = _mm_srli_si128(result1,1);

		result3 = vorrq_u8(right,vorrq_u8(left,result1)); // Valeur finale de IT1[i][j-1]

		vst1q_u8(&It1[i][j-1],result3);

		j= vj0;
	}
}

void fermeture3x3_SIMD(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0, vi1, vj0, vj1);
	dilatation3x3_SIMD(It,vXVt,vi0,vi1,vj0,vj1);
	erosion3x3_SIMD(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0, vi1, vj0, vj1);
}

void fermeture3x3_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	dilatation3x3_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	erosion3x3_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}

void ouverture3x3_SIMD(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0, vi1, vj0, vj1);
	erosion3x3_SIMD(It,vXVt,vi0,vi1,vj0,vj1);
	dilatation3x3_SIMD(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0, vi1, vj0, vj1);
}

void ouverture3x3_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	erosion3x3_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	dilatation3x3_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}

void erosion5x5_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	erosion3x3_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	erosion3x3_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}

void dilatation5x5_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	dilatation3x3_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	dilatation3x3_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}

void fermeture5x5_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	dilatation5x5_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	erosion5x5_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}

void ouverture5x5_SIMD_B(vuint8 **It,vuint8 **It1,long vi0,long vi1,long vj0,long vj1)
{
	vuint8 ** vXVt = vui8matrix(vi0-1, vi1+1, vj0-1, vj1+1);
	erosion5x5_SIMD_B(It,vXVt,vi0,vi1,vj0,vj1);
	dilatation5x5_SIMD_B(vXVt,It1,vi0,vi1,vj0,vj1);
	free_vui8matrix(vXVt,vi0-1, vi1+1, vj0-1, vj1+1);
}