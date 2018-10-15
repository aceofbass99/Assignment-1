// This file uses a LinkedList of ListNodes to create a priority
//queue, where values with corresponding priorities are passed in
//and returned back in order of ascending priority. The list stores
//the values in order of priority, so peeking and dequeueing is
//very quick.

//Name: Noah Bailyn
//Section leader: Gianna Chien

#include "LinkedPriorityQueue.h"

//Initializes the list to have front start at a null pointer. O(1).
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = nullptr;
}

//Cleans up the queue after it is done being worked with. O(N).
LinkedPriorityQueue::~LinkedPriorityQueue() {
    ListNode* temp = front;
    while(front != nullptr) {
        temp = front;
        front = front->next;
        delete temp;
    }
}

//Changes the priority of a certain value in the queue, then reorders the queue
//to reflect the change in priority. This is done by deleting the value out
//of the queue, then enqueueing the value again using the enqueue method. O(N).
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* temp = front;
    if(temp->value == value) {
        temp = front->next;
        front->next = nullptr;
        delete front;
        front = temp;
    } else {
        while(temp->next->value != value) {
            temp = temp->next;
        }
        ListNode* ln = temp->next->next;
        temp->next->next = nullptr;
        delete temp->next;
        temp->next = ln;
    }
    enqueue(value, newPriority);
}

//Clears the queue. O(N).
void LinkedPriorityQueue::clear() {
    ListNode* temp = front;
    while(front != nullptr) {
        temp = front;
        front = front->next;
        delete temp;
    }
}

//Removes the first node off of the LinkedList, and then returns
//that node's value. O(1).
string LinkedPriorityQueue::dequeue() {
    if(size() == 0) throw "The queue has no elements.";
    string result = front->value;
    ListNode* temp = front;
    front = front->next;
    delete temp;
    return result;
}

//Enqueues a value at the given priority. This is done by iterating
//through the list until the list's priority is equal or greater
//than the given priority, and then inserting a node with the
//value and priority into the list. O(N).
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newNode = new ListNode(value, priority);
    if(front == nullptr) {
        front = newNode;
    } else if(priority < front->priority) {
        newNode->next = front;
        front = newNode;
    } else {
        ListNode* temp = front;
        while(temp->next->priority < priority || (temp->next->value < value && temp->next->priority == priority)) {
            temp = temp->next;
            if(temp->next == nullptr) break;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

//Returns whether the queue is empty or not. O(1).
bool LinkedPriorityQueue::isEmpty() const {
    return(front == nullptr);
}

//Returns the first value in the queue. O(1).
string LinkedPriorityQueue::peek() const {
    if(size() == 0) throw "The queue has no elements.";
    return front->value;
}

//Returns the priority of the next value. O(1).
int LinkedPriorityQueue::peekPriority() const {
    if(size() == 0) throw "The queue has no elements.";
    return front->priority;
}

//Returns the size of the queue. This is done by iterating through
//the queue until null is hit, and incrementing the counter by 1
//each time it loops. O(N).
int LinkedPriorityQueue::size() const {
    int counter = 0;
    ListNode* temp = front;
    while(temp != nullptr) {
        temp = temp->next;
        counter++;
    }
    return counter;
}

//Converts the list of nodes into a readable string displaying all
//of the values and priorities in order. O(N).
string LinkedPriorityQueue::listString() const {
    ListNode* temp = front;
    string result = "{";
    while(temp != nullptr) {
        result += "\"";
        result += temp->value;
        result += "\":";
        result += to_string(temp->priority);
        temp = temp->next;
        if(temp == nullptr) break;
        result += ", ";
    }
    result += "}";
    return result;
}

//Calls listString(), then returns the stream with the appended string. O(1).
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << queue.listString();
    return out;
}
