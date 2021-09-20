#ifndef PATHFINDING_UTIL_C
#define PATHFINDING_UTIL_C

#include "PathfindingUtil.h"

#include "stdio.h"
#include "PathfindingAlgorithm.h"
#include "NodeData/NodeDataOps.h"
#include "FileUtility/ReadMap.h"

#ifndef MAP_SIZE_X				// This should be overridden by ReadMap.h!
	#define MAP_SIZE_X	0
#endif
#ifndef MAP_SIZE_Y
	#define MAP_SIZE_Y	0
#endif

// 'Private' variables
//static int PosXCheckedArray[MAP_SIZE_X] = { 0 };	// @TO REMOVE // Hao: replaced with NodeMap
//static int PosYCheckedArray[MAP_SIZE_Y] = { 0 };	// @TO REMOVE // Hao: replaced with NodeMap
static NodeList NodeQueue = { .tail = NULL };		// initialise .data to {0}
static int GoalReached = FALSE;


/* IsPosChecked */
int IsPosXChecked(NodeData* nodeData, int posx)
{
	return nodeData->isChecked;
}
void SetNodeChecked(NodeData* nodeData, int is_checked)
{
	nodeData->isChecked = is_checked;
}

/* Depreciated */
/*
void SetPosXChecked(int posx, int is_checked)
{
	PosXCheckedArray[posx] = is_checked;
}
void SetPosYChecked(int posy, int is_checked)
{
	PosYCheckedArray[posy] = is_checked;
}
void SetPosXChecked(int posx, int is_checked)
{
	PosXCheckedArray[posx] = is_checked;
}
void SetPosYChecked(int posy, int is_checked)
{
	PosYCheckedArray[posy] = is_checked;
}
*/

int IsCheckedNode(NodeMap checkedNodeMap, int posx, int posy)
{
	NodeData* nodeData = NodeMapGet(checkedNodeMap, NodeMapGenKey(posx, posy));
	if (nodeData == NULL) { return FALSE; }
	else {
		if (GetNodeDataIsChecked(nodeData)) { return TRUE; }
		return FALSE;
	}
}

/* Goal Cell */
// @For now, keep every input value as defined in ReadMap.h
int GetGoalPosX()
{
	return GOAL_X; // return pos_x of target cell
}
// @For now, keep every input value as defined in ReadMap.h
int GetGoalPosY()
{
	return GOAL_Y; // return pos_y of target cell
}

int IsGoalReached()
{
	return GoalReached;
}

void SetGoalReached(int is_reached)
{
	GoalReached = is_reached;
}



/* Pathfinding Algorithm Functions */

// @TODO
CellType GetCellType(int posx, int posy)
{
	if (posx == GetGoalPosX()) {
		if (posy == GetGoalPosY()) {
			return GOAL;
		}
	}
	if (GetMapValue(posy, posx) == 1) {
		return PATH;
	}
	// Else...
	return EMPTY;
}

void EvaluateAdjacentCells(NodeData* instigating_node, NodeMap* checkedNodeMap)
{
	int posx = GetNodeDataPosX(instigating_node);
	int posy = GetNodeDataPosY(instigating_node);

	if (posx + 1 < MAP_SIZE_X)	{ EvaluateCell(instigating_node, checkedNodeMap, posx + 1, posy); }
	if (posx - 1 >= 0)			{ EvaluateCell(instigating_node, checkedNodeMap, posx - 1, posy); }
	if (posy + 1 < MAP_SIZE_Y)	{ EvaluateCell(instigating_node, checkedNodeMap, posx, posy + 1); }
	if (posy - 1 >= 0)			{ EvaluateCell(instigating_node, checkedNodeMap, posx, posy - 1); }
}

void EvaluateCell(NodeData* instigating_node, NodeMap* checkedNodeMap, int posx, int posy)
{
	CellType cell_type = GetCellType(posx, posy);
	switch (cell_type)
	{
	case PATH:	EvaluatePathCell(instigating_node, checkedNodeMap, posx, posy);
	case GOAL:	SetGoalReached(TRUE);
	default:	return;
	}
}

// @TODO
void EvaluatePathCell(NodeData* instigating_node, NodeMap* checkedNodeMap, int posx, int posy)
{
	// replace IsNodeChecked() with IsNodeValid(GetNodeFromCoords())
	if (!IsCheckedNode(checkedNodeMap, posx, posy)) {
		// We have discovered a new node...
		// Create and populate the node, set its coords as checked, and
		// send it to the NodeQueue.
		NodeData* new_node = NewNodeData();
		PopulateNodeData(instigating_node, new_node, posx, posy, TRUE);
		AddToNodeQueue(new_node);
		NodeMapSet(checkedNodeMap, NodeMapGenKey(posx, posy), new_node);
		return;
	} else {
		// We have found a new path to an already discovered node...
		// Link the preexisting node to the instigating node.
		// 
		//NodeData* preexisting = GetNodeFromCoords(posx, posy);
		//AddToNodeDataAdjacentNode(preexisting, instigating_node);
		// DESIGN CHOICE: singly or doubly linked? Currently only singly.
		// 
		// todo: when changing if statement, save lookup result for reuse here ^	
		return;
	}
}

NodeData* PopulateNodeData(NodeData* instigating_node, NodeData* node, int posx, int posy, int isChecked)
{
	SetNodeDataPosX(node, posx);
	SetNodeDataPosY(node, posy);
	SetNodeChecked(node, isChecked);
	SetNodeDataWeight(node, CalculateNodeWeight(instigating_node));
	AddToNodeDataAdjacentNode(node, instigating_node);
	return node;
}


void AddToNodeQueue(NodeData* node)
{
	NodeListElement* element = NewNodeListElement(node);
	InsertInNodeQueue(&NodeQueue, element);					// <--- ACTUAL FUNCTIONALITY IN PATHFINDING.H
}

NodeListElement* ExtractNextInNodeQueue()
{
	return RemoveListHead(&NodeQueue);
}

int IsNodeQueueEmpty()
{
	return !IsElementValid(GetListHead(&NodeQueue));
}


#endif // !PATHFINDING_UTIL_C