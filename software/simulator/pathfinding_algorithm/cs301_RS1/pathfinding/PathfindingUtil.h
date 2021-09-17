#ifndef PATHFINDING_UTIL_H
#define PATHFINDING_UTIL_H

#include "NodeData/NodeList.h"

#include "NodeData/NodeData.h"	// only required in .c, but added for convenience of the typedef

/*
	Pathfinding Util

	Pathfinding functions with functionality independent from the
	current algorithm.


	Pseudo code:
		EvaluateCell(start_posx, start_posy);			// create the starting node

		while(!IsNodeQueueEmpty())
		{
			current_node = ExtractNextInNodeQueue();
			EvaluateAdjacentCells(..current_node posx and posy...);
			
			if (IsGoalReached()) {
				break;
			}
		}

		CompileShortestPath();
		// Last processed node (tail of ProcessedNodeQueue) is the cell that reached the goal cell.
		// Work backwards from there, through ProcessedNodeQueue, until there is a path from start
		// to goal.
*/

#ifndef MAP_SIZE_X
	#define MAP_SIZE_X	10	// This should be overridden by the actual map size!
#endif
#ifndef MAP_SIZE_Y
	#define MAP_SIZE_Y	10
#endif

#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif


typedef enum CellType {EMPTY, PATH, GOAL} CellType;


/* IsPosChecked */

int IsPosXChecked(int posx);
int IsPosYChecked(int posy);
void SetPosXChecked(int posx, int is_checked);
void SetPosYChecked(int posy, int is_checked);

int IsCheckedNode(int posx, int posy);


/* Goal Cell */

int GetGoalPosX();
int GetGoalPosY();
int IsGoalReached();
void SetGoalReached(int is_reached);


/* Pathfinding Algorithm Functions */

// Classify and return the CellType of the cell at the given coords.
CellType GetCellType(int posx, int posy);

// Used to discover and evaluate new cells.
// Called on a node to evaluate all orthogonally adjacent cells (within map bounds).
void EvaluateAdjacentCells(NodeData* instigating_node);

// Evaluate a newly discovered cell at the given coordinates.
// Controls the flow of evaluation depending on the CellType (empty, path, goal).
void EvaluateCell(NodeData* instigating_node, int posx, int posy);

// Called by EvaluateCell() if the newly discovered cell has the CellType PATH.
// If the path cell is new a node should be created to represent it, and sent to
// the NodeQueue for the pathfinding algorithm to handle.
// If the path cell is preexisting then we have found an alternate path to it, and
// should link the preexisting node to the instigating node that was adjacent to it.
void EvaluatePathCell(NodeData* instigating_node, int posx, int posy);

// Populate NodeData contextually. Uses PathfindingAlgorithm functions for calculating
// weight.
NodeData* PopulateNodeData(NodeData* instigating_node, NodeData* node, int posx, int posy);



/* Pathfinding Algorithm Functions */

// Insert a node in the NodeQueue. Wrapper for functionality in PathfindingAlgorithm.
void AddToNodeQueue(NodeData* node);

// Detach the NodeListElement from the top of the NodeQueue, and return it.
NodeListElement* ExtractNextInNodeQueue();

// Returns true if no NodeListElements left in NodeQueue.
int IsNodeQueueEmpty();


#endif // !PATHFINDING_UTIL_H
