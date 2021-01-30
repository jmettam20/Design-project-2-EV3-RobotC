#pragma config(Sensor, S1,     ultrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colourSensorBottom, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     touchSensor,    sensorEV3_Touch, modeEV3Bump)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorB,          armMotor,      tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

volatile int gyro;//global vairable, constantly being updated

volatile int colour;//global variable, contantly being updated

task gyroValue()
{
	while(1){
		gyro = getGyroDegrees(gyroSensor);
	}
}

task colourValue()
{ 
	while(1){
		colour = getColorName(colourSensorBottom);
	}
}

void home()
{
	/*This function centres the robot inside the red box.
	 * It first locates the front edge, then bounces back to the back edge, measuring the distance travelled. 
	 * it then moves forward again for half the distance, turns to the right and repeats for the two 
	 * side edges, before turning left to return to a forward facing, central position.  
	 * 
	*/
	resetMotorEncoder(leftMotor);//give 0 start val
	int edge = colour;
	while(edge != 5)//jump in to loop
	{
		edge = colour;
		setMotorSync(leftMotor, rightMotor, 0, 25);//move forward
	}
	//jump out of loop
	resetMotorEncoder(leftMotor);//reset measurement
	setMotorSync(leftMotor,rightMotor, 0 , -25);//move backwards
	wait1Msec(500);//get off the line
	//jump in to loop
	edge = colour;
	while(edge != 5)
	{
	edge = colour;
	setMotorSync(leftMotor,rightMotor, 0, -25);//move backwards
	}
	//jump out of loop
	setMotorSpeed(leftMotor, 0);//stop
	setMotorSpeed(rightMotor,0);
	int distanceBack = fabs(getMotorEncoder(leftMotor));//calculate distance moved back
	resetMotorEncoder(leftMotor);//reset measurement
	setMotorSyncEncoder(leftMotor,rightMotor,0,(distanceBack/2), 25);//move forward half total
	waitUntilMotorStop(leftMotor);//wait
	resetGyro(gyroSensor);//zero start value
	int gyroTarget = -90;
	while(gyro <= gyroTarget)
	{//turn
	setMotorSpeed(leftMotor, 25);
	setMotorSpeed(rightMotor, -25);
	}
	//stop
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
	resetMotorEncoder(leftMotor);
	//right edge
	edge = colour;
	while(edge != 5)
	{
	edge = colour;
	setMotorSync(leftMotor, rightMotor, 0, 25);
	}
	//save distance
	resetMotorEncoder(leftMotor);//go back
	setMotorSync(leftMotor,rightMotor, 0 , -25);
	wait1Msec(700);//get off line
	//wait for line detection
	edge = colour;
	while(edge != 5)
	{
	edge = colour;
	setMotorSync(leftMotor,rightMotor, 0, -25);
	}
	//stop
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor,0);
	distanceBack = fabs(getMotorEncoder(leftMotor));//save distance travelled
	resetMotorEncoder(leftMotor);//reset
	setMotorSyncEncoder(leftMotor,rightMotor,0,(distanceBack/2), 25);//move forward half total width
	waitUntilMotorStop(leftMotor);//wait
	gyroTarget = 90;//turn left 
	while(gyro <= gyroTarget)
	{//turn
	setMotorSpeed(leftMotor, -25);
	setMotorSpeed(rightMotor, 25);
	}
	//stop
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
}

task main()
{
	startTask (gyroValue); startTask (colourValue); home();
}