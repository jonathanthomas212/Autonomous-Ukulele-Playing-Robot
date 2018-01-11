#include "EV3_FileIO.c"
const int CHORDAMOUNTC = 14;
const int CHORDAMOUNTV = 18;
const int CHORDAMOUNTB = 12;
void readInChorus(int* MusicC, int* MusicCT)
{
	TFileHandle fin;
	bool fileOkay=openReadPC(fin,"Chorus.txt");
	if(!fileOkay)
	{
		return;
	}
	for(int count=0;count<CHORDAMOUNTC;count++)
	{
		int chord = 0;
		int numStrums=0;
		readIntPC(fin,chord);
		displayString(3,"%i",chord);
		wait1Msec(1000);
		MusicC[count]=chord;
		readIntPC(fin,numStrums);
		MusicCT[count]=numStrums;
	}
	closeFilePC(fin);
}
void readInVerse(int* MusicV, int* MusicVT)
{
	TFileHandle fin;
	bool fileOkay=openReadPC(fin,"Verse.txt");
	if(!fileOkay)
	{
		return;
	}
	for(int count=0;count<CHORDAMOUNTV;count++)
	{
		int chord = 0;
		int numStrums=0;
		readIntPC(fin,chord);
		MusicV[count]=chord;
		readIntPC(fin,numStrums);
		MusicVT[count]=numStrums;
	}
	closeFilePC(fin);
}
void readInBridge(int* MusicB, int* MusicBT)
{
	TFileHandle fin;
	bool fileOkay=openReadPC(fin,"Bridge.txt");
	if(!fileOkay)
	{
		return;
	}
	for(int count=0;count<CHORDAMOUNTB;count++)
	{
		int chord = 0;
		int numStrums=0;
		readIntPC(fin,chord);
		MusicB[count]=chord;
		readIntPC(fin,numStrums);
		MusicBT[count]=numStrums;
	}
	closeFilePC(fin);
}
void PID(int target, int motorPort)
{
	   float kp = 0.85; // PID constants
	   float ki = 5;
	   float kd = 1;

	   float kpValue = 0;
	   float kiValue = 0;
	   float kdValue = 0;

	    int error_last = 0;
	    int error_diff = 0;
	    int error_sum = 0;
	    int maxTime = 750;

	    clearTimer(T1);
	    nMotorEncoder[motorPort] = 0;

	    int motorPower;

	    while (time1[T1] < maxTime)
	    {
	        int currEncoder;

	        if (target > 0)
	            currEncoder = abs(nMotorEncoder[motorPort]);//might need to change if wrong direction
	        else
	            currEncoder = nMotorEncoder[motorPort];

	        int error = target - currEncoder;

	        error_diff = error - error_last;
	        error_last = error;

	        kpValue = kp * error;
	        kiValue = ki * error_sum;
	        kdValue = kd * error_diff;

	        // motorPower cap
	        if (kpValue + kiValue + kdValue > 100)
	            motorPower = 100;
	        else if (kpValue + kiValue + kdValue < -100)
	            motorPower = -100;
	        else
	            motorPower = (kpValue + kiValue + kdValue);

	        motor[motorPort] = motorPower;

	        delay(50);
	    }
	    motor[motorPort]=0;
	}
void chordSelect(int chord)
	{

	if (chord == 1)
	PID(-250, motorA);

	else if (chord == 2)
	PID(-70, motorA);

	else if (chord == 3)
	PID(-170, motorA);

	else if (chord == 4)
	PID(0, motorA);

}
void returnToHome(int lastChord)
{
	if (lastChord==1)
	{
		PID(-110, motorA);
	}
	else if (lastChord==2)
	{
		PID(-290, motorA);
	}
	else if (lastChord==3)
	{
		PID(-190, motorA);
	}
}

void doStrum(int power,int numOfStrums)
{
	for (int count=0;count<numOfStrums;count++)
	{
		motor[motorB]=power;
		while(SensorValue[S1]!=0)
			{}
		while(SensorValue[S1]!=1)
			{}
		motor[motorB]=0;
	}
}
void doChorusBridgeVerse(int* musicC,int* musicB,int* musicV,int* musicCT,int* musicBT,int* musicVT,int indicator)
{

	int chord = -1;
	int numOfStrum =0;
	if (indicator==1)
	{
			for (int index = 0;index<CHORDAMOUNTV;index++)
			{
				chord = musicV[index];
				numOfStrum= musicVT[index];
				chordSelect(chord);
				doStrum(100,numOfStrum*8);
				returnToHome(chord);
			}
	}

	else if (indicator==2)
	{
			for (int index = 0;index<CHORDAMOUNTC;index++)
			{
				chord = musicC[index];
				numOfStrum= musicCT[index];
				chordSelect(chord);
				doStrum(100,numOfStrum*8);
				returnToHome(chord);
			}
	}
	else if (indicator==3)
	{
			for (int index = 0;index<CHORDAMOUNTB;index++)
			{
				chord = musicB[index];
				numOfStrum= musicBT[index];
				chordSelect(chord);
				doStrum(60,numOfStrum*2);
				wait1Msec(1500)
				returnToHome(chord);
			}
	}

}
task main()
{
	SensorType[S1]=sensorEV3_Touch;
	SensorType[S3]=sensorEV3_Touch;
	SensorType[S2]=sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S2]=modeEV3Color_RGB_Raw;
	wait1Msec(50);
	int musicC[CHORDAMOUNTC]={1,3,4,4,1,3,4,1,3,4,4,1,3,4};
	int musicV[CHORDAMOUNTV]={1,3,4,1,3,4,1,3,4,1,3,4,1,3,4,1,3,4};
	int musicB[CHORDAMOUNTB]={1,3,4,1,3,4,1,3,4,1,3,4};
	int musicCT[CHORDAMOUNTC]={3,3,3,3,3,3,3,3,3,3,3,3,3,3};
	int musicVT[CHORDAMOUNTV]={1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3};
	int musicBT[CHORDAMOUNTB]={1,1,1,1,1,1,1,1,1,1,1,1};
	readInChorus(musicC,musicCT);
//	readInVerse(musicV,musicVT);
//	readInBridge(musicB,musicBT);
//	readInVerse(musicV,musicVT);
//	readInBridge(musicB,musicBT);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,1);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,2);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,1);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,2);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,3);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,2);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,2);
	doChorusBridgeVerse(musicC,musicB, musicV,musicCT,musicBT,musicVT,2);

}
