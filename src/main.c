#include <stdio.h>
#include <stdlib.h>
#include "test_mouvement.h"
#include "test_mouvement_SSE2.h"
#define NBIMAGES 199
#include "nrdef.h"
#include "nrutil.h"
#include "matric_roc.h"
#include <string.h>



void differenceImageScal_SIMD()
{

    long nrl, nrh, ncl, nch;
    uint8 **ItNonSIMD = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
    uint8 **ItSIMD =  ui8matrix(nrl, nrh, ncl, nch);;
    uint8 **ItNormal = ui8matrix(nrl, nrh, ncl, nch);
    char nomImageLoadNonSIMD[100];//Pour initialiser nrl, etc
    char nomImageLoadSIMD[100];
    char nomImageLoadNormal[100];


    for(int i = 1; i <= NBIMAGES; i++)
    {
        sprintf(nomImageLoadNonSIMD,"car3Sigma/car_3%03d.pgm",i);//Image a t
        sprintf(nomImageLoadSIMD,"car3SigmaSIMD/car_3%03d.pgm",i);//Image a t
        sprintf(nomImageLoadNormal,"car3/car_3%03d.pgm",i);//Image a t

        MLoadPGM_ui8matrix(nomImageLoadNonSIMD, nrl, nrh, ncl, nch, ItNonSIMD);
        MLoadPGM_ui8matrix(nomImageLoadSIMD, nrl, nrh, ncl, nch, ItSIMD);
        MLoadPGM_ui8matrix(nomImageLoadNormal, nrl, nrh, ncl, nch, ItNormal);

        for(int k = nrl; k <= nrh; k++)
        {
            for(int l = ncl; l <= nch; l++)
            {
                if(ItNonSIMD[k][l] != ItSIMD[k][l]){
                    printf("Probleme ! i = %d, j = %d, ItNormal = %d\n",k , l, ItNormal[k][l]);
                }
            }
        }
    }
}

void Implementation(char nom[]){
    long nrl, nrh, ncl, nch;
    char nomSauvegardeH[100];
    char nomImage[100];
    strcpy(nomImage, nom);
    strcpy(nomSauvegardeH, nomImage);
    strcat(nomSauvegardeH,".h");
    char nomLecture[100] = "car3/";
    strcat(nomLecture, nomImage);
    strcat(nomLecture,".pgm");
    char nomType[100] = "unsigned char ";
    strcat(nomType, nomImage);
    uint8 **ItNonSIMD = LoadPGM_ui8matrix(nomLecture, &nrl, &nrh, &ncl, &nch);
    FILE* fichier = fopen(nomSauvegardeH, "w");
    fprintf(fichier, "\n%s", nomType);
    fprintf(fichier, "[][] = {");
    printf("nrh =  %d, nch = %d\n", nrh, nch);

    for(int i = nrl; i <= nrh; i++)
    {
        fprintf(fichier, "{");
        for(int j = ncl; j <= nch; j++)
        {
            if(j != nch)
                fprintf(fichier, "%d, ", ItNonSIMD[i][j]);
            else
                fprintf(fichier, "%d ", ItNonSIMD[i][j]);


        }
        if(i == nrh)
            fprintf(fichier, "} ");
        else
            fprintf(fichier, "},\n");
    }
    fprintf(fichier, "};");

}

#define OPTI 2 //1 pour optimisation 2 sans optimisation 3 pour tout 0 pour rien


int main(int argc, char* argv[])
{
#if OPTI & 0x1
    //test_routine_FrameDifference_SSE2(20);
    //test_routine_sigmaDelta_SSE2();
    test_routine_FrameDifference_SSE2M(20);
#endif
#if OPTI & 0x2
    //test_routine_FrameDifference(20);
    // test_routine_FrameDifferenceMorpho3x3ouverture(20);
    test_routine_FrameDifferenceMorpho3x3fermeture(20);
    //test_routine_FrameDifferenceMorpho3x3ouvertureFermeture(20);
    //test_routine_FrameDifferenceMorpho3x3fermetureOuverture(20);
    //test_routine_FrameDifferenceMorpho3x3fermeturefermeture(20);
    //test_routine_sigmaDelta();
#endif
    //creation_matrices_ROC("verite/car_3165.pgm", "car3Frame3x3FO/car_3165.pgm");
    //differenceImageScal_SIMD();
    //test_unitaire_SD_SSE2();
    Implementation(argv[1]);


    return 0;
}
