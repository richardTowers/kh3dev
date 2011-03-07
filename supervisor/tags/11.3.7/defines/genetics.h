/*****************************************************************/
//  genetics.h
//	Created by Richard Towers at 11:03 on 09/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/
#ifndef INC_GENETICS
#define INC_GENETICS

#define POP_SIZE 10			//The number of individuals to be evaluated before reproduction
#define GENERATIONS 100		//The number of generations to evaluate
#define EVAL_TIME 30		//The max time to spend evaluating an individual
#define SURVIVAL_BONUS 20	//The bonus an individual recieves for not crashing
#define TOUR_SIZE 3			//The number of individuals to compete in a tournament

#define pMUTATE 1000
#define pSET_ZERO 5
#define pADD_WEIGHT 5
#define pFLIP_NEG 5
#define pCHANGE_WEIGHT 20
#define pFLIP_THIS_BIT 100 


#endif //INC_GENETICS
