#pragma config(Sensor, S1,     ultrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Sensor, S2,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S3,     colourSensorBottom, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          armMotor,      tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Ultrasonic sensor measures 15cm wide
//Measured object 121cm away



///Global Variables///
int speedFast = 100;
int speedMed = 25;
int speedSlow = 10;

volatile bool objectDetected;

int objectRanges[3] = {0,0,0};//distance,angle
int objectGyros[3] = {0,0,0};//distance,angle

volatile int gyro;

volatile int colour;

volatile int range =0;


int position[2] = { 0, 0};

//Gyeo sensor task//
task gyroValue()
{
	while(1){
		gyro = getGyroDegrees(gyroSensor);
	}
}

//Colour sensor task//
task colourValue()
{ while(1){
		colour = getColorName(colourSensorBottom);
	}
}

//Ultrasinic sensor task//
task ultraSonicValue()
{
	int objectCount = 0;
	int rangeLast = 0;
	while(1)
	{
		rangeLast = range;
		range = getUSDistance(ultrasonicSensor);
		displayBigTextLine(1, "%d, %d",range, rangeLast);
		if ((range<(rangeLast-30))&&(range<160))
			{
				objectRanges[objectCount] = range;
				objectGyros[objectCount] = gyro;
				objectCount++;
			}
			displayBigTextLine(5,"%d , %d",objectRanges[0], objectGyros[0]);
}
}



//Turning function//
void turnTarget(bool direction,int degrees)
{
	resetGyro(gyroSensor);
	wait1Msec(5);
	if (direction == 0)
	{//left
		int gyroTarget = -degrees;
		while(gyro >= gyroTarget)
		{//turn
			setMotorSpeed(leftMotor, -75);
			setMotorSpeed(rightMotor, 75);
			displayBigTextLine(3, "%d", gyro);
		}
		//stop
		setMotorSpeed(leftMotor, 0);
		setMotorSpeed(rightMotor, 0);
	}
	if (direction == 1)
	{//right
		int gyroTarget = degrees;
		while(gyro <= gyroTarget)
		{//turn
			setMotorSpeed(leftMotor, 75);
			setMotorSpeed(rightMotor, -75);
			displayBigTextLine(3, "%d", gyro);
		}
		//stop
		setMotorSpeed(leftMotor, 0);
		setMotorSpeed(rightMotor, 0);
	}
}

//Home funtion//
void home()
{
	int xOffset =0;
	int yOffset =0;
	resetMotorEncoder(leftMotor);//give 0 start val
	int edge = colour;
	while(edge != 5)//jump in to loop
	{
		edge = colour;
		displayBigTextLine(5, "%d", colour);
		setMotorSync(leftMotor, rightMotor, 0, 75);//move forward
	}
	//jump out of loop
	resetMotorEncoder(leftMotor);//reset measurement
	setMotorSync(leftMotor,rightMotor, 0 , -75);//move backwards
	wait1Msec(700);//get off the line
	//jump in to loop
	edge = colour;
	while(edge != 5)
	{
		edge = colour;
		setMotorSync(leftMotor,rightMotor, 0, -75);//move backwards
	}
	//jump out of loop
	setMotorSpeed(leftMotor, 0);//stop
	setMotorSpeed(rightMotor,0);
	int distanceBack = fabs(getMotorEncoder(leftMotor));//calculate distance moved back
	resetMotorEncoder(leftMotor);//reset measurement
	int distanceTot = distanceBack;//total distance moved
	setMotorSyncEncoder(leftMotor,rightMotor,0,(distanceTot/2), 75);//move forward half total
	yOffset = (distanceTot/2)+1;
	waitUntilMotorStop(leftMotor);//wait
	turnTarget(0,90);
	resetMotorEncoder(leftMotor);
	//right edge
	edge = colour;
	while(edge != 5)
	{
		edge = colour;
		setMotorSync(leftMotor, rightMotor, 0, 75);
	}
	//save distance
	resetMotorEncoder(leftMotor);//go back
	setMotorSync(leftMotor,rightMotor, 0 , -75);
	wait1Msec(700);//get off line
	//wait for line detection
	edge = colour;
	while(edge != 5)
	{
		edge = colour;
		setMotorSync(leftMotor,rightMotor, 0, -75);
	}
	//stop
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor,0);
	distanceBack = fabs(getMotorEncoder(leftMotor));//save distance travelled
	resetMotorEncoder(leftMotor);//reset
	int distanceWidth = fabs(distanceBack);//find total width
	setMotorSyncEncoder(leftMotor,rightMotor,0,(distanceWidth/2), 75);//move forward half total width
	xOffset = (distanceWidth/2)+21;
	waitUntilMotorStop(leftMotor);//wait
	turnTarget(1,90);
	position[0]=xOffset;
	position[1]=yOffset;
}

void search()
{
	startTask (ultraSonicValue);
	resetGyro(gyroSensor);

	while (gyro > -90){
		setMotorSpeed(leftMotor, -10);
		setMotorSpeed(rightMotor, 10);
	}
	stopMultipleMotors(leftMotor,rightMotor);

	while (gyro < 90){
		displayBigTextLine(5,"%d gyro", gyro);
		setMotorSpeed(leftMotor, 10);
		setMotorSpeed(rightMotor, -10);
	}
	stopMultipleMotors(leftMotor,rightMotor);
	stopTask(ultraSonicValue);


}






task main()
{
	startTask (gyroValue);
	startTask (colourValue);
	search();//
	//home();

}
