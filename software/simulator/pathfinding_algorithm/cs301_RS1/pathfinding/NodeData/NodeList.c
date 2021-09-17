#ifndef NODELIST_C
#define NODELIST_C

#include "NodeList.h"

#include "stdlib.h"		// malloc(), NULL


NodeList* NewNodeList()
{
	NodeList* list = (NodeList*)malloc(sizeof list);
	return list;
}

NodeListElement* NewNodeListElement(struct NodeData* node)
{
	NodeListElement* element = (NodeListElement*)malloc(sizeof element);
	element->node = node;
	return element;
}

int IsListValid(NodeList* list)
{
	return (list != NULL);
}

int IsElementValid(NodeListElement* element)
{
	return (element != NULL);
}

void Insert(NodeListElement* current, NodeListElement* new)
{
	new->tail = current->tail;
	current->tail = new;
	return;
}

NodeListElement* DetachNext(NodeListElement* preceding_node)
{
	NodeListElement* to_remove = preceding_node->tail;
	preceding_node->tail = to_remove->tail;
	to_remove->tail = NULL;
	return to_remove;
}

void AppendToList(NodeList* list, NodeListElement* element)
{
	if (IsElementValid(list->tail)) {
		// If NodeList is not empty then add as normal...
		Insert(list->tail, element);
	} else {
		element->tail = NULL;
	}
	list->tail = element;

	if (!IsElementValid(list->head)) {
		list->head = element;	// If the only element in the list, assign as head too.
	}
}

void PrependToList(NodeList* list, NodeListElement* element)
{
	if (IsElementValid(list->head)) {
		// If NodeList is not empty then add as normal...
		Insert(list->head, element);
	}
	else {
		element->tail = NULL;
	}
	list->head = element;

	if (!IsElementValid(list->tail)) {
		list->tail = element;	// If the only element in the list, assign as tail too.
	}
}

NodeListElement* RemoveListHead(NodeList* list)
{
	NodeListElement* to_remove = list->head;
	list->head = to_remove->tail;
	if (list->tail == to_remove) {
		list->tail = to_remove->tail;		// Clean up if the list has only one element.
	}
	return to_remove;
}

NodeListElement* RemoveListTail(NodeList* list)
{
	NodeListElement* to_remove = list->tail;
	list->tail = to_remove->tail;
	if (list->head == to_remove) {
		list->head = to_remove->tail;			// Clean up if the list has only one element.
	}
	return to_remove;
}

NodeListElement* GetListHead(NodeList* list)
{
	return list->head;
}

NodeListElement* GetListTail(NodeList* list)
{
	return list->tail;
}

#endif // !NODELIST_C