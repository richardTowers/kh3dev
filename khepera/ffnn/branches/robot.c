/*****************************************************************/
/*  robot.c
/*	Created by richard at 12:28 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "robot.h"

//Based on initkh3 and initMot from khepera3_test.c
void initialiseRobot(void)
{
	//Aparantly "This is required". Not our place to ask what it does.
	kh3_init();
	
	//Again, not sure what these do, have tried to give them more descriptive names
  khepera = knet_open( "Khepera3:dsPic" , KNET_BUS_I2C , 0 , NULL );
  leftMotor  = knet_open( "Khepera3:mot1" , KNET_BUS_I2C , 0 , NULL );
  rightMotor  = knet_open( "Khepera3:mot2" , KNET_BUS_I2C , 0 , NULL );

	if(khepera!=NULL)
	{
		if( (leftMotor!=NULL) && (rightMotor!=NULL) )
		{
			//Set up both motors explicitly
			kmot_SetMode( leftMotor , kMotModeIdle );
			kmot_SetMode( rightMotor , kMotModeIdle );
		  	kmot_SetSampleTime( leftMotor , 1550 );
			kmot_SetSampleTime( rightMotor , 1550 );
		  	kmot_SetMargin( leftMotor , 6 );
			kmot_SetMargin( rightMotor , 6 );
	   	kmot_SetOptions( leftMotor , 0x0 , kMotSWOptWindup | kMotSWOptStopMotorBlk | kMotSWOptDirectionInv);
	    	kmot_SetOptions( rightMotor , 0x0 , kMotSWOptWindup | kMotSWOptStopMotorBlk );
		  	kmot_ResetError( leftMotor );
			kmot_ResetError( rightMotor );
		  	kmot_SetBlockedTime( leftMotor , 10 );
		  	kmot_SetBlockedTime( rightMotor , 10 );
		  	kmot_SetLimits( leftMotor , kMotRegCurrent , 0 , 500 );
		  	kmot_SetLimits( rightMotor , kMotRegCurrent , 0 , 500 );
		  	kmot_SetLimits( leftMotor , kMotRegPos , -10000 , 10000 );
		  	kmot_SetLimits( rightMotor , kMotRegPos , -10000 , 10000 );
		
		  	// PID
		  	kmot_ConfigurePID( leftMotor , kMotRegSpeed , 400 , 0 , 10 );
		  	kmot_ConfigurePID( rightMotor, kMotRegSpeed , 400 , 0 , 10 );
		  	kmot_ConfigurePID( leftMotor , kMotRegPos, 620, 3, 10);
		  	kmot_ConfigurePID( rightMotor, kMotRegPos, 620, 3, 10);
		  	kmot_SetSpeedProfile( leftMotor, 30, 10);
		  	kmot_SetSpeedProfile( rightMotor, 30, 10);
		}
		else error(ERROR_MOTOR_INIT);
	}
	else error(ERROR_KHEPERA_INIT);
}

//Returns a range between 0 and 1
float getIRRange(int sensorNumber)
{
	//Loosely based on getProxIR() from khepera3_test.c
	char Buffer[25];
	char rangeString[4];
	float range;

  if(kh3_proximity_ir((char *)Buffer, khepera ))
	{
			sprintf(rangeString, "%4u",(Buffer[2*sensorNumber+1] | Buffer[2*sensorNumber+2]<<8));
	  	range = atof(rangeString)/1024;
	}
  else warning(WARNING_GETIR_FAIL);

  return range;
}

void setMotor(char theMotor)
{
	//No idea how to do this yet...
	
}
