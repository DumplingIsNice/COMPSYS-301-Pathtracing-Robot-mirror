//======================================================================
//Author: 
//Mr. Fung Yang
//Senior Technician Engineer Research and Design,
//Robotics and Control system signal processing Labs,
//Department of Electrical, Computer and Software Engineering,
//The University of Auckland.
//
//Written for teaching design course Compsys301 in ECSE department of UOA.
//
//This example program uses the pacman robot simulation library written by Mr. Fung Yang.
//
//Date 2012~2020
//=======================================================================

#define TESTMODE3
//#define TEST_MODE_MAP
#define TESTSENSOR

/* Starting Position Init */
//#define TESTMOVELEFT
//#define TESTMOVERIGHT
#define TESTALIGN

// Simulation parameters
//{------------------------------------
#define NITERATIONS 1000
#define STARTUPDELAY 2 //sec

// More in 'Project.h'
//}------------------------------------

/* Do not touch Includes */
//{------------------------------
#include "mainFungGLAppEngin.h" //a must
#include "mazeGen.h" //just include to use radnom number generation function
#include <vector>
#include <iostream>
#include "highPerformanceTimer.h"//just to include if timer function is required by user.
//}------------------------------------
/* Custom Includes */

extern "C"
{
	#include "pathfinding/PathfindingMain.h"
	#include "robot_simulation/project.h"
}

#include "robot_simulation/control.h"
#include "robot_simulation/sensor.h"

using namespace std;

//{------------------------------
//these global variables must be defined here with no modification.
float virtualCarLinearSpeed;
float virtualCarAngularSpeed;
float currentCarAngle;
float currentCarPosCoord_X, currentCarPosCoord_Y;

int sensorPopulationAlgorithmID;
float sensorSeparation;
float num_sensors;
extern int maxDarkDefValueTH;

vector<int> virtualCarSensorStates;

vector<ghostInfoPack> ghostInfoPackList;
//}-------------------------------

highPerformanceTimer myTimer;

//just a helper function
void setVirtualCarSpeed(float linearSpeed, float angularSpeed)
{
	virtualCarLinearSpeed = linearSpeed;
	virtualCarAngularSpeed = angularSpeed;
}

//The Only TWO functions Students need to modify to add their own sensor guided
//path following control and Map path search calculations.
//{=================================================
float virtualCarLinearSpeed_seed;		// maximum speed of your robot in mm/s
float virtualCarAngularSpeed_seed;		// maximum angular speed of your robot in degrees/s


//added2021_2_22
float virtualCarLinearSpeedFloor;
float currentCarPosFloor_X, currentCarPosFloor_Y;

/* Actuation Functions */
//{------------------------------------
void LinearForward() { virtualCarLinearSpeed_seed = DEFAULT_LINEAR_SPEED * floorToCoordScaleFactor; }
void LinearZero() { virtualCarLinearSpeed_seed = 0; }
void AngularLeft() { virtualCarAngularSpeed_seed = LEFT_TURNING_SPEED; }
void AngularRight() { virtualCarAngularSpeed_seed = RIGHT_TURNING_SPEED; }
void AngularZero() { virtualCarAngularSpeed_seed = 0; }
void AlignLeft() { virtualCarAngularSpeed_seed = LEFT_ALIGN_SPEED; }
void AlignRight() { virtualCarAngularSpeed_seed = RIGHT_ALIGN_SPEED; }
void AlignZero() { virtualCarAngularSpeed_seed = 0; }

void InitSpeedSeed()
{
	LinearZero();
	AngularZero();
}

///*Input speed : takes in a speed for the robot in mm/s*/
//void moveStraight(int speed) {
//
//	setVirtualCarSpeed(speed * floorToCoordScaleFactor, 0);
//
//}
//
//
//void turnLeft() {
//
//	setVirtualCarSpeed(0, 180);
//
//}
//
//void turnRight() {
//	setVirtualCarSpeed(0, -180);
//
//}
//
//void turnBack() {
//	setVirtualCarSpeed(0, 360);
//
//}
//
//
//void stopMovement() {
//	setVirtualCarSpeed(0, 0);
//}
//
//
///*Input direction : takes an integer for the directions
//					0 - straight
//					1- left
//					2- right
//					3- back
//					4 - stop
//This can be changed to the macros used in the other files for better integration
//Input speed is an optional parameter used for setting the speed when moving straight
//If no value is supplied it will assume default of 130 mm/s
//The turning functions that are called set the linear and angular speed of the car,
//it doesnt take into account the iterations the turns should be called for, this should be handled,
//by the called. When testing this the turns took two iterations to complete
//[we're trying to implement this in here as well]
//*/
//void movementDirection(int direction, int speed = 130) {
//
//	if (direction == 0) {
//		moveStraight(speed);
//	}
//	else if (direction == 1) {
//		turnLeft();
//	}
//	else if (direction == 2) {
//		turnRight();
//	}
//	else if (direction == 3) {
//		turnBack();
//	}
//	else {
//		stopMovement();
//	}
//
//}

//}------------------------------------
int virtualCarInit()
{
	cout << "default virtualCarLinearSpeed_seed:" << virtualCarLinearSpeed_seed << endl;
	cout << "default virtualCarAngularSpeed_seed:" << virtualCarAngularSpeed_seed << endl;
	cout << "default virtualCarLinearSpeedFloor:" << virtualCarLinearSpeedFloor << endl;
	cout << endl;

	InitDirectionSensed();

	// Three options for robot's sensor placement
	// Custom - read in ../config/sensorPos.txt
	sensorPopulationAlgorithmID = PLACE_SENSORS_USER_DEFINED;
	
	// Linear Distribution - Auto Linear distribution
	//sensorPopulationAlgorithmID = PLACE_SENSORS_AUTO_SEP;
	//num_sensors = 3;

	// Linear Distribution - Custom Linear distribution
	//sensorPopulationAlgorithmID = PLACE_SENSORS_SEP_USER_DEFINED;
	//num_sensors = 3;
	//sensorSeparation = 0.2;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//variables below can be initializes in config file,
	//or you can uncomment them to override config file settings.
	//----------------------------------------------
	virtualCarLinearSpeedFloor = 130;//mm
	currentCarPosCoord_X = cellToCoordX(1);
	currentCarPosCoord_Y = cellToCoordY(7);
	currentCarAngle = 0;

	//virtualCarAngularSpeed_seed = 40;//degree
	//currentCarAngle = 90;//degree
	//maxDarkDefValueTH = 20;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifdef TESTMODE3 //turn 180
	currentCarPosCoord_X = cellToCoordX(3);
	currentCarPosCoord_Y = cellToCoordY(7);
	currentCarAngle = 0;//degree
	virtualCarLinearSpeedFloor = 0; // mm/s
	virtualCarLinearSpeed_seed = virtualCarLinearSpeedFloor * floorToCoordScaleFactor;// coord/s
	virtualCarAngularSpeed_seed = 0;
	cout << "updated virtualCarLinearSpeed_seed:" << virtualCarLinearSpeed_seed << endl;
	cout << "updated virtualCarAngularSpeed_seed:" << virtualCarAngularSpeed_seed << endl;
#endif //TESTMODE3

#ifdef TESTSENSOR
	currentCarPosCoord_X = cellToCoordX(1);
	currentCarPosCoord_Y = cellToCoordY(8);
	currentCarAngle = 0;//degree
	virtualCarLinearSpeedFloor = 0;
	virtualCarLinearSpeed_seed = virtualCarLinearSpeedFloor * floorToCoordScaleFactor;//coord
	virtualCarAngularSpeed_seed = 0;
#endif //TESTSENSOR

#ifdef TESTALIGN
	currentCarPosCoord_X = cellToCoordX(1);
	currentCarPosCoord_Y = cellToCoordY(5);
#endif //TESTALIGN

#ifdef TESTMOVERIGHT
	currentCarPosCoord_X = cellToCoordX(4);
	currentCarPosCoord_Y = cellToCoordY(2);
#endif //TESTMOVERIGHT

#ifdef TESTMOVELEFT
	currentCarPosCoord_X = cellToCoordX(1);
	currentCarPosCoord_Y = cellToCoordY(2);
#endif //TESTMOVELEFT

	myTimer.resetTimer();
	return 1;
}


#pragma optimize( "", on )
int virtualCarUpdate()
{
	static int i = 0;
	static float prev_position = coordToFloorX(currentCarPosCoord_X);
	static float myspeed = virtualCarLinearSpeed_seed;
	static int skip = 1;

	if (skip == 1)
	{
		if (myTimer.getTimer() > STARTUPDELAY)
			skip = 0;
		else
			return 1;
	}

	HandleSensor();

//	if (i < NITERATIONS)
//	{
//		//		setVirtualCarSpeed(virtualCarLinearSpeed_seed * 0.10, 360/10);
//		virtualCarLinearSpeed = virtualCarLinearSpeed_seed;
//		virtualCarAngularSpeed = virtualCarAngularSpeed_seed;
//		cout << "=====================================" << endl;
//		cout << "iteration:" << i << " with speed_seed: " << virtualCarLinearSpeed_seed << " with angular speed_seed: " << virtualCarAngularSpeed_seed << endl;
//
//		cout << "current car floor X, Y, theta = " << coordToFloorX(currentCarPosCoord_X) << " , " << coordToFloorY(currentCarPosCoord_Y) << " , " << currentCarAngle << endl;
//		cout << "current Cell X, Y = " << coordToCellX(currentCarPosCoord_X) << " , " << coordToCellY(currentCarPosCoord_Y) << endl;
//		cout << "del pos:" << coordToFloorX(currentCarPosCoord_X) - prev_position << endl;
//		cout << "-----------------------------------------" << endl;
//
//		prev_position = coordToFloorX(currentCarPosCoord_X);
//		i++;
//}
//	else
//	{
//		stopMovement();
//		if (i == NITERATIONS)
//		{
//			cout << "=====================================" << endl;
//			cout << "iteration:" << i << " with speed_seed: " << virtualCarLinearSpeed_seed << endl;
//
//			cout << "current car floor X, Y, theta = " << coordToFloorX(currentCarPosCoord_X) << " , " << coordToFloorY(currentCarPosCoord_Y) << " , " << currentCarAngle << endl;
//			cout << "current Cell X, Y = " << coordToCellX(currentCarPosCoord_X) << " , " << coordToCellY(currentCarPosCoord_Y) << endl;
//			cout << "del pos:" << coordToFloorX(currentCarPosCoord_X) - prev_position << endl;
//			cout << "-----------------------------------------" << endl;
//		}
//		i++;
//	}

#ifdef TESTMODE3

	/*	This is a sample command structure : 
		
		The robot is driven by its currently sensed path + navagation command
		Navigation command is a MotionState enum stored in NextRobotMotionState.

		Ideally:
			Navigation is handled after HandleSensor() (above) before the following 
			section and a command is produced.

			The following section load the command DEPENDING on the encountered path.

				- A counter may be used to track each path met (and went straight)
				  on the same following trace.

		Currently, commands to deal with an intersection is fixed.

		If no command is loaded, robot drives automatically in the following priority:
			
			Follow a line					-> Straight and automatic line following logic. 
			Dead-end and floating off-line	-> U-turn (right)
	*/
	//}---------------------------------

	if (GetRobotMotionState() == FOLLOWING)
	{
		if (SENSED_CROSS_ROAD)
		{
			SetNextRobotMotionState(U_TURN); // Fixed.
		}
		else if (SENSED_T)
		{
			SetNextRobotMotionState(U_TURN); // Fixed.
		}
		else if (SENSED_L_BRANCH_T)
		{
			SetNextRobotMotionState(U_TURN); // Fixed.
		}
		else if (SENSED_R_BRANCH_T)
		{
			SetNextRobotMotionState(U_TURN); // Fixed.
		}
	}
	//}---------------------------------

  /* Update Routine: */
  
  // Pass command as current state.
  HandleCommands(GetNextRobotMotionState());
  // Perform actuation depending on current RobotMotionState
  HandleMovement();
  printf("######################\n");
#endif // TESTMODE3

	myTimer.resetTimer();

	return 1;
}
#pragma optimize( "", off )
int main(int argc, char** argv)
{

#ifdef TEST_MODE_MAP
	//FindShortestPath();
	//PrintOutputMap();
	//PrintFinalMap();
	//CreateFinalMap();
#endif

	FungGlAppMainFuction(argc, argv);

	return 0;
}
