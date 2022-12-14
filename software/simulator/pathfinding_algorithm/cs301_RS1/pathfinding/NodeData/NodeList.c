//#ifndef NODELIST_C
//#define NODELIST_C
//
//#include "NodeList.h"
//
//#include "stdlib.h"		// malloc(), NULL
//#include "NodeDataOps.h" // for destroy contents function
//
//NodeList* NewList()
//{
//	return (NodeList*)NewList();
//}
//
//NodeListElement* NewNodeListElement(const struct NodeData* node)
//{
//	return (NodeListElement*)NewListElement(node);
//}
//
////int IsListValid(const NodeList* list)
////{
////	return Is
////}
//
////int IsElementValid(const NodeListElement* element)
////{
////	return (element != NULL);
////}
//
////void Insert(NodeListElement* current, NodeListElement* add)
////{
////	add->tail = current->tail;
////	current->tail = add;
////	return;
////}
//
////NodeListElement* DetachNext(NodeListElement* preceding_node)
////{
////	NodeListElement* to_remove = preceding_node->tail;
////	preceding_node->tail = to_remove->tail;
////	to_remove->tail = NULL;
////	return to_remove;
////}
//
////void AppendToList(NodeList* list, NodeListElement* element)
////{
////	if (IsElementValid(list->tail)) {
////		// If NodeList is not empty then add as normal...
////		Insert(list->tail, element);
////	} else {
////		element->tail = NULL;
////	}
////	list->tail = element;
////
////	if (!IsElementValid(list->head)) {
////		list->head = element;	// If the only element in the list, assign as head too.
////	}
////}
//
////void PrependToList(NodeList* list, NodeListElement* element)
////{
////	if (IsElementValid(list->head)) {
////		// If NodeList is not empty then link tail as normal...
////
////		if (!IsElementValid(list->tail)) {
////			// If there is only one element in the list then update tail...
////			list->tail = list->head;
////		}
////		element->tail = list->head;
////	}
////	else {
////		element->tail = NULL;
////	}
////	list->head = element;
////
////	if (!IsElementValid(list->tail)) {
////		list->tail = element;	// If this is the only element in the list, assign as tail too.
////	}
////}
//
////void InsertInList(NodeList* list, NodeListElement* current, NodeListElement* add)
////{
////	if (current == list->tail) {
////		AppendToList(list, add);
////	} else {
////		Insert(current, add);
////	}
////}
//
////NodeListElement* RemoveListHead(NodeList* list)
////{
////	NodeListElement* to_remove = list->head;
////	list->head = to_remove->tail;
////	if (list->tail == to_remove) {
////		list->tail = to_remove->tail;		// Clean up if the list has only one element.
////	}
////	return to_remove;
////}
//
////NodeListElement* RemoveListTail(NodeList* list)
////{
////	NodeListElement* to_remove = list->tail;
////	list->tail = to_remove->tail;
////	if (list->head == to_remove) {
////		list->head = to_remove->tail;			// Clean up if the list has only one element.
////	}
////	return to_remove;
////}
//
////NodeListElement* GetListHead(const NodeList* list)
////{
////	return list->head;
////}
//
////NodeListElement* GetListTail(const NodeList* list)
////{
////	return list->tail;
////}
//
//
////void DestroyListElements(NodeList* list)
////{
////	NodeListElement* element = GetListHead(list);
////	NodeListElement* prev_element;
////
////	while (IsElementValid(element)) {
////		prev_element = element;
////		element = element->tail;
////		free(prev_element);
////	}
////
////	list->head = NULL;
////	list->tail = NULL;
////}
//
//void DestroyNodeListElementsAndContents(NodeList* list)
//{
//	NodeListElement* element = GetListHead(list);
//	NodeListElement* prev_element;
//
//	while (IsElementValid(element)) {
//		// DestroyNodeData() is used here to prevent memory leaks.
//		if (element->node != NULL) { DestroyNodeDataAndContents(element->node); }
//		prev_element = element;
//		element = element->tail;
//		free(prev_element);
//	}
//
//	list->head = NULL;
//	list->tail = NULL;
//}
//
////void DestroyList(NodeList* list)
////{
////	if (IsListValid(list)) { free(list); }
////}
//
////void DestroyListElement(NodeListElement* element)
////{
////	if (IsElementValid(element)) { free(element); }
////}
//
//#endif // !NODELIST_C
