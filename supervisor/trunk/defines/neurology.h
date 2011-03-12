/*****************************************************************/
//  neurology.h
//	Created by Richard Towers at 17:44 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/
#ifndef INC_NEUROLOGY
#define INC_NEUROLOGY

//Architecture:
#define INPUTS 9
#define	HIDDENS 4
#define OUTPUTS 2
//Connection Probabilities:
/*I  H  O
I 0 14  1
H 0  4 14
O 0  0  0*/
#define pBASE 40
#define pII 0
#define pIH 14
#define pIO 1
#define pHI 0
#define pHH 2
#define pHO 14
#define pOI 0
#define pOH 0
#define pOO 0
#define pBIAS 5

//Range:
#define WEIGHT_RANGE 0x3F
#define BIAS_RANGE 0x3F
#define TCONST_RANGE 0x3F


#endif //INC_NEUROLOGY
