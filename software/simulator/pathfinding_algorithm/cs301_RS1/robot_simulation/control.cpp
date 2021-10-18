#include "sensor.h"
#include "control.h"
#include "actuation.h"

#include <cstdio>

static MotionState NextMotionState = NO_STATE;
static MotionState RobotMotionState = FOLLOWING;

extern float virtualCarLinearSpeed_seed;
extern float virtualCarAngularSpeed_seed;

MotionState GetRobotMotionState() 
{
	return RobotMotionState;
}

MotionState GetNextRobotMotionState()
{
	return NextMotionState;
}

void SetRobotMotionState(const MotionState s) 
{
	RobotMotionState = s;
}

void SetNextRobotMotionState(const MotionState s)
{
	NextMotionState = s;
}

void HandleCommands(MotionState command)
{
	printf("Next Robot Motion State is: ");
	PrintRobotState(GetNextRobotMotionState());
	if (command != NO_STATE)
	{
		SetRobotMotionState(command);
		SetNextRobotMotionState(NO_STATE);
	}
}

void HandleMovement()
{
	printf("Current Robot Motion State is: ");
	PrintRobotState(GetRobotMotionState());

	Directions* validDirections = GetDirectionsSensed();

	static int toExit = 0;
	static int leaveCounter = 0;
	InitSpeedSeed();

	switch (GetRobotMotionState())
	{
	case FOLLOWING:
		// Dead_end
		if ((!validDirections->left && !validDirections->right) && !validDirections->forward)
		{
			AngularRight();
			SetRobotMotionState(U_TURN);
		}
		else if (validDirections->forward)
		{
			HandleAlignment();
			LinearForward();
		}
		else if (validDirections->left)
		{
			AngularLeft();
			SetRobotMotionState(LEFT_TURNING);
		}
		else if (validDirections->right)
		{
			AngularRight();
			SetRobotMotionState(RIGHT_TURNING);
		}
		break;
	case LEFT_TURNING:
		AngularLeft();
		if (!validDirections->forward)
		{
			toExit = 1;
		}
		if (toExit && validDirections->forward)
		{
			SetRobotMotionState(LEAVING);
			toExit = 0;
		}
		break;
	case RIGHT_TURNING:
		AngularRight();
		if (!validDirections->forward)
		{
			toExit = 1;
		}
		if (toExit && validDirections->forward)
		{
			SetRobotMotionState(LEAVING);
			toExit = 0;
		}
		break;
	case LEAVING:
		HandleAlignment();
		LinearForward();
		leaveCounter++;
		if (leaveCounter > LEAVING_COUNT)
		{
			SetRobotMotionState(FOLLOWING);
			leaveCounter = 0;
		}
		break;
	case U_TURN:
		AngularRight();
		if (validDirections->forward)
		{
			SetRobotMotionState(LEAVING);
		}
	default:
		;
	}

	setVirtualCarSpeed(virtualCarLinearSpeed_seed, virtualCarAngularSpeed_seed);
}

//{------------------------------------
// Rudenmentry self aligning system
// Alignment sensor located at back for least duration oscillation. 
// Prob Need to be fix condition to while the robot is under the travel straight command.
// Requires dynamic readjustment magnitude (PID??)
	// Needs to be fast when center returns to path.
// Critical note: **We cannot turn on a dime**.
//updat linear and rotational speed based on sensor information

void HandleAlignment() {

	if ((GetRobotMotionState() == FOLLOWING) || (GetRobotMotionState() == LEAVING))
	{
		if (!GetRASensor() != !GetLASensor())
		{
			if (!GetLASensor())
				AlignRight();
			else if (!GetRASensor())
				AlignLeft();
		}
		else {
			AlignZero();
		}
	}
	//}---------------------------------------
}

void PrintRobotState(MotionState m)
{
    switch (m)
    {
    case FOLLOWING:
        printf("FOLLOWING\n");
        break;
    case LEFT_TURNING:
        printf("LEFT_TURNING\n");
        break;
    case RIGHT_TURNING:
        printf("RIGHT_TURNING\n");
        break;
	case LEAVING:
		printf("LEAVING\n");
		break;
	case U_TURN:
		printf("U_TURN\n");
		break;
	case NO_STATE:
		printf("NO_STATE\n");
		break;
    default:
        printf("NULL\n");
        break;
    }
}