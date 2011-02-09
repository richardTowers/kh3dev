/*****************************************************************/
//  errors.h
//	Created by richard at 10:33 on 08/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_ERRORS
#define INC_ERRORS

#include <stdio.h>
#include <stdlib.h>

//Error Codes
#define ERROR_NONE 0
#define ERROR_GENERAL 1
//Specific errors...
#define ERROR_MALLOC 2
#define ERROR_MOTOR_INIT 3
#define ERROR_KHEPERA_INIT 4
#define WARNING_GETIR_FAIL 5
#define ERROR_IPDDRESS 6
#define WARNING_SCP_FAIL 7

//Globals
static FILE *errorFile;


void startLoggingErrors(void);
void stopLoggingErrors(void);
const char * errorLookup(int errorCode);
void error(int errorCode);
void warning(int errorCode);

#endif //INC_ERRORS
