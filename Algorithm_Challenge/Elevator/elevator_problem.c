/****************************************************************************
	[Project] Controls Technical Interview
*****************************************************************************
	[Lead developer] Rachel Harris, rharris at dephy dot com.
	[Contributors] 
*****************************************************************************
	[This file] elevator_problem: Moves an elevator through a building to get 
	people to the correct floor in the least amount of time. 
****************************************************************************
	[Created] 2022-11-07 | rharris | Initial creation
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************
#include "elevator_problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************************************************
// Variable(s)
//****************************************************************************
struct building_s myBuilding;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void initBuilding(void);
static int8_t setNextElevatorStop(struct building_s building);
static void moveElevator(struct elevator_s * elevator);
static void stopElevator(struct building_s * building);
static void drawBuilding(struct building_s building, int8_t door_status);
static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus);
static void drawElevator(struct elevator_s elevator, int8_t doorStatus);
static void delay(int16_t ms);

//****************************************************************************
// Functions You (the Interviewee) Should Edit:
//****************************************************************************

//Returns the floor the elevator should STOP at next
//To stop at a floor means to open the doors and let passengers on and off. It 
//is possible to pass through a floor without stopping there.
//Note: The output should be a number between 0 and (BUILDING_HEIGHT-1), inclusive


/*
 * The algorithm prioritizes minimizing travel distance by first serving the closest destination
 * of any current passengers in the elevator. If the elevator is empty, it moves to the closest
 * floor that has waiting passengers. This greedy approach of always choosing the nearest target
 * ensures continuous movement while keeping travel distances minimal.
 */

static int8_t setNextElevatorStop(struct building_s building) {
    int8_t currentFloor = building.elevator.currentFloor;
    int8_t nextStop = -1;
    int8_t minDistance = BUILDING_HEIGHT + 1;

    // Check destinations of current passengers
    for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++) {
        if (building.elevator.passengers[i] != -1) {
            int8_t destination = building.elevator.passengers[i];
            int8_t distance = abs(destination - currentFloor);
            if (distance < minDistance) {
                minDistance = distance;
                nextStop = destination;
            }
        }
    }

    // If no passengers, check for floors with waiting passengers
    if (nextStop == -1) {
        for (int8_t f = 0; f < BUILDING_HEIGHT; f++) {
            for (int8_t j = 0; j < 2; j++) {
                if (building.floors[f].departures[j] != -1) {
                    int8_t distance = abs(f - currentFloor);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextStop = f;
                    }
                }
            }
        }
    }

    // If still no target, default to staying at the current floor
    return nextStop != -1 ? nextStop : currentFloor;
}

//****************************************************************************
//YOU CAN REVIEW THE CODE BELOW BUT DO NOT EDIT IT UNLESS YOU'RE 3000% SURE 
//THERE'S A BUG IN MY CODE! (which admittedly, is possible)
//****************************************************************************


//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the elevator simulation
void main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the building
	initBuilding();

	//Draw the initial state of the building, with elevator doors closed
	system("clear");
	drawBuilding(myBuilding,1);
	fflush(stdout);
	delay(1000);

	//Run the elevator for a set period of time
	for(int8_t i = 0; i < 60; i++)
	{
		//Choose the next floor to stop at
		myBuilding.elevator.nextStop = setNextElevatorStop(myBuilding);

		//Move the elevator closer to the next stop
		moveElevator(&myBuilding.elevator);

		//Draw the building, with elevator doors closed
		system("clear");
		drawBuilding(myBuilding,1);
		fflush(stdout);
		delay(1000);

		//If necessary, stop the elevator to let people on/off
		if(myBuilding.elevator.currentFloor == myBuilding.elevator.nextStop)
		{
			//Redraw the building, with elevator doors open
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Let people on/off the elevator
			stopElevator(&myBuilding);

			//Redraw the building, with elevator doors open, showing the change
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Redraw the building, with elevator doors closed
			system("clear");
			drawBuilding(myBuilding,1);
			fflush(stdout);
			delay(1000);
			i++;
		}

		//Check if all people have been taken to their correct floors
		int8_t numPeopleAtDestination = 0;
		for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
		{
			numPeopleAtDestination += myBuilding.floors[f].arrivals;
		}

		if(numPeopleAtDestination == 10)
		{
			printf("SUCCESS: You got all the people through! The total time was: %i seconds!\n", i);
			return;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t numPeopleAtDestination = 0;
	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		numPeopleAtDestination += myBuilding.floors[f].arrivals;
	}
	printf("FAIL: You ran out of time. You got %i/10 people through in 60 seconds.\n", numPeopleAtDestination);
}

//****************************************************************************
// Private function(s):
//****************************************************************************
static void initBuilding(void)
{
	memset(&myBuilding, -1, sizeof(struct building_s));

	myBuilding.elevator.currentFloor = rand() % BUILDING_HEIGHT;
	myBuilding.elevator.nextStop = myBuilding.elevator.currentFloor;

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		for(int8_t j = 0; j < 2; j++)
		{
			int destination = rand() % BUILDING_HEIGHT;
			while(destination == f)
			{
				destination = rand() % BUILDING_HEIGHT;
			}
			myBuilding.floors[f].departures[j] = destination;
		}
		myBuilding.floors[f].arrivals = 0;
	}


// // worst case simulation:

// memset(&myBuilding, -1, sizeof(struct building_s));

//     // Start elevator at bottom floor
//     myBuilding.elevator.currentFloor = 0;
//     myBuilding.elevator.nextStop = myBuilding.elevator.currentFloor;

//     // Place passengers in a way that forces maximum travel:
//     // - Put people at bottom floor (0) wanting to go to top floor (4)
//     // - Put people at top floor (4) wanting to go to bottom floor (0)
//     // This forces the elevator to make multiple trips
    
//     // Floor 0: Two people going to top floor
//     myBuilding.floors[0].departures[0] = 4;
//     myBuilding.floors[0].departures[1] = 4;
    
//     // Floor 1: Two people going to top floor
//     myBuilding.floors[1].departures[0] = 4;
//     myBuilding.floors[1].departures[1] = 4;
    
//     // Floor 2: Two people, one going up, one going down
//     myBuilding.floors[2].departures[0] = 4;
//     myBuilding.floors[2].departures[1] = 0;
    
//     // Floor 3: Two people going to bottom floor
//     myBuilding.floors[3].departures[0] = 0;
//     myBuilding.floors[3].departures[1] = 0;
    
//     // Floor 4: Two people going to bottom floor
//     myBuilding.floors[4].departures[0] = 0;
//     myBuilding.floors[4].departures[1] = 0;
    
//     // Initialize arrivals to 0
//     for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
//     {
//         myBuilding.floors[f].arrivals = 0;
//     }



}

static void moveElevator(struct elevator_s * elevator)
{
	if(elevator->nextStop < elevator->currentFloor)
	{
		elevator->currentFloor--;
	}
	else if(elevator->nextStop > elevator->currentFloor)
	{
		elevator->currentFloor++;
	}
}

static void stopElevator(struct building_s * building)
{
	//Let out anyone who needs to get off
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == building->elevator.currentFloor) 
		{
			building->floors[building->elevator.currentFloor].arrivals++;
			building->elevator.passengers[i] = -1;
		}
	}

	//Let in anyone who needs to get on, but only if capacity allows
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == -1) 
		{
			for(int8_t j = 0; j < 2; j++)
			{
				if(building->floors[building->elevator.currentFloor].departures[j] != -1)
				{
					building->elevator.passengers[i] = building->floors[building->elevator.currentFloor].departures[j];
					building->floors[building->elevator.currentFloor].departures[j] = -1;
					break;
				}
			}
		}
	}
}

static void drawBuilding(struct building_s building, int8_t doorStatus)
{
	printf("          DEPARTURES|     |ARRIVALS\n");

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		drawFloor(building.floors[f],f,building.elevator,doorStatus);
	}
}

static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus)
{

	printf("FLOOR %i:          ", floorNumber);
	
	for(int8_t i = 0; i < 2; i++)
	{
		if(floor.departures[i] == -1){ printf(" "); }
		else{ printf("%i",floor.departures[i]); }
	}

	if(elevator.currentFloor == floorNumber){ drawElevator(elevator, doorStatus); }
	else{printf("|     |"); }

	for(int8_t i = 0; i < floor.arrivals; i++)
	{
		printf("x");
	}

	printf("\n");
}

static void drawElevator(struct elevator_s elevator, int8_t doorStatus)
{
	if(doorStatus == 1){ printf("| "); }
	else{ printf("  "); }

	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(elevator.passengers[i] == -1){ printf("_"); }
		else{ printf("%i",elevator.passengers[i]); }
	}

	if(doorStatus == 1){ printf(" |"); }
	else{ printf("  "); }
}

static void delay(int16_t ms)
{
    //Storing start time
    clock_t startTime = clock();
 
    //Looping till required time is not achieved
    while(clock() < startTime + ms);
}
