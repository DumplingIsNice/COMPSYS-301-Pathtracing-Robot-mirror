#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "SLList/sllist.h"

/*
	Pathfinding

	...
*/

#ifndef MAP_SIZE
	#define MAP_SIZE_X	10	// This should be overridden by the actual map size!
	#define MAP_SIZE_Y	10
#endif

#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif


enum CellType {EMPTY, PATH, GOAL};



/* IsPosChecked */
int IsPosXChecked(int posx);
int IsPosYChecked(int posy);
void SetPosXChecked(int posx, int is_checked);
void SetPoxYChecked(int posy, int is_checked);

int IsCheckedNode(int posx, int posy);


/* Goal Cell */
int GetGoalPosX();
int GetGoalPosY();
//
void ReachedGoal();


/* Pathfinding Algorithm Functions */
//
void EvaluateAdjacentCells(int posx, int posy);
// Return the cell type, using the map and goal cell.
enum CellType GetCellType(int posx, int posy);

// Evaluate 
// Call this on each new cell.
void EvaluateCell(int posx, int posy);

void EvaluatePathNode(int posx, int posy);

void AddToNodeQueue(int posx, int posy)


#endif // !PATHFINDING_H
