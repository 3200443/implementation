#include <stdio.h>
#include <stdlib.h>
#include "test_mouvement.h"
#define NBIMAGES 199
#include "nrdef.h"
#include "nrutil.h"


#define OPTI 2 //1 pour optimisation 2 sans optimisation 3 pour tout 0 pour rien


int main(int argc, char* argv[])
{
#if OPTI & 0x1
    //test_routine_FrameDifference_SSE2(20);
    //test_routine_sigmaDelta_SSE2();
    //test_routine_FrameDifference_SSE2M(20);
#endif
#if OPTI & 0x2
    test_routine_FrameDifference(20);
    // test_routine_FrameDifferenceMorpho3x3ouverture(20);
    //test_routine_FrameDifferenceMorpho3x3fermeture(20);
    //test_routine_FrameDifferenceMorpho3x3ouvertureFermeture(20);
    //test_routine_FrameDifferenceMorpho3x3fermetureOuverture(20);
    //test_routine_FrameDifferenceMorpho3x3fermeturefermeture(20);
    test_routine_sigmaDelta();
#endif
    //creation_matrices_ROC("verite/car_3165.pgm", "car3Frame3x3FO/car_3165.pgm");
    //differenceImageScal_SIMD();
    //test_unitaire_SD_SSE2();

    return 0;
}
