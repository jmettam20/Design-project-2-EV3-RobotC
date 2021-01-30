#pragma config(Sensor, S2,     gyroSenso,      sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colorSensor,    sensorNone)
#pragma config(Sensor, S4,     sonarSensor,    sensorNone)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*------------------------------------------------------------------------------------------------
This program will move your robot forward for 2 seconds. At the end of the program, all motors
will shut down automatically and turn off.

ROBOT CONFIGURATION: LEGO EV3 REM Bot
MOTORS & SENSORS:
[I/O Port]          [Name]              [Type]                			[Location]
MotorC        			leftMotor           LEGO EV3 Motor		      Left side motor
MotorB       				rightMotor          LEGO EV3 Motor		      Right side motor (reversed)
------------------------------------------------------------------------------------------------*/

task main()
{
	resetGyro(S2);
		for (int i =0; i<4;i++)
		{
			setMotorSyncEncoder(leftMotor,rightMotor, 0, 2170, 50);//move 1m
			waitUntilMotorStop(leftMotor);
			setMotorSyncEncoder(leftMotor,rightMotor, 100, 195, 25);//turn 90
		waitUntilMotorStop(leftMotor);
		}
		while(1);
	}
