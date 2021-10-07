#ifndef NODEDATAOPS_C
#define NODEDATAOPS_C

#include "NodeDataOps.h"
#include <stddef.h>
#include "NodeList.h"

void DestroyNodeDataAndContents(NodeData* node)
{
	NodeListElement* current_element = GetNodeDataAdjacentNodeListElement(node);
	NodeListElement* prev_element;

	while (IsElementValid(current_element)) {
		prev_element = current_element;
		current_element = current_element->tail;
		DestroyListElement(prev_element);
	}
	DestroyNodeData(node);
}

int GetNodeDataPosX(NodeData* node)
{
	return node->posx;
	//return (node->pos) >> 16;
}
int GetNodeDataPosY(NodeData* node)
{
	return node->posy;
	//return (node->pos) & 0x00FF;
}
int GetNodeDataIsChecked(NodeData* node)
{
	return node->isChecked;
}
int GetNodeDataAdjacentPaths(NodeData* node)
{
	if (IsNodeDataValid(node)) { return node->num_adjacent_paths; }
	return -1;
}
int GetNodeDataWeight(NodeData* node)
{
	return node->weight;
}
NodeListElement* GetNodeDataAdjacentNodeListElement(NodeData* node)
{
	return node->adjacent_nodes;
}

void SetNodeDataPosX(NodeData* node, int pos_x)
{
	node->posx = pos_x;
	//node->pos = node->pos & 0x00FF;
	//node->pos = node->pos | (pos_x << 16);
}
void SetNodeDataPosY(NodeData* node, int pos_y)
{
	node->posy = pos_y;
	//node->pos = node->pos & 0xFF00;
	//node->pos = node->pos | pos_y;
}
void SetNodeDataIsChecked(NodeData* node, int isChecked)
{
	node->isChecked = isChecked;
}
void SetNodeDataAdjacentPaths(NodeData* node, int adj_paths)
{
	if (IsNodeDataValid(node)) { node->num_adjacent_paths = adj_paths; }
}
void IncrementNodeDataAdjacentpaths(NodeData* node)
{
	if (IsNodeDataValid(node)) { node->num_adjacent_paths = 1 + node->num_adjacent_paths; }
}
void SetNodeDataWeight(NodeData* node, int weight)
{
	node->weight = weight;
}

void AddToNodeDataAdjacentNode(NodeData* node, NodeData* node_to_add)
{
	// If the list is empty...

	// Start Node Logic
	/*if (node_to_add == NULL)
	{
		node->adjacent_nodes = NULL;
		return;
	} else if (node == NULL) {
		return;
	}*/

	if (!IsNodeDataValid(node)) { return; }
	if (!IsNodeDataValid(node_to_add)) { return; }

	if (!IsElementValid(node->adjacent_nodes)) {
		// If list is empty...
		node->adjacent_nodes = NewNodeListElement(node_to_add);

		IncrementNodeDataAdjacentpaths(node);
		return;
	}
	// Otherwise add a new node (if it is unique)...
	NodeListElement* current_node = node->adjacent_nodes;
	NodeListElement* prev_node = current_node;

	while (IsElementValid(current_node)) {
		if (current_node->node == node_to_add) { return; }
		prev_node = current_node;
		current_node = current_node->tail;
	}
	Insert(prev_node, NewNodeListElement(node_to_add));

	IncrementNodeDataAdjacentpaths(node);
}

#endif // !NODEDATAOPS_C
