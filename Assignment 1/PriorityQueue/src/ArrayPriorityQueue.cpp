//Name: Noah Bailyn
//Section leader: Gianna Chien

//This file implements the Priority Queue, which accepts strings
//at different positive integer priorities and spits them back
//out in order of lowest-digit priority number.

#include "ArrayPriorityQueue.h"
#include "PQEntry.h"

//This initializes the array by setting a starting capacity and setting
//the number in queue to 0. O(1).
ArrayPriorityQueue::ArrayPriorityQueue() {
    capacity = 10;
    numInQueue = 0;
    PQEntry* a = new PQEntry[capacity];
    array = a;

}

//This deletes the array after the ArrayPriorityQueue stops being used. O(1).
ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete array;
}

//This doubles the array's capcity once it is full. O(N).
void ArrayPriorityQueue::increaseArrayCapacity() {
    capacity *= 2;
    PQEntry* bigger = new PQEntry[capacity];
    for(int i = 0; i < numInQueue; i++) {
        bigger[i] = array[i];
    }
    delete array;
    array = bigger;
}

//This changes the priority of a specific value in the array to a different priority, meaning
//it will be dequeued in a different order. O(N).
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    for(int i = 0; i < numInQueue; i++) {
        if(array[i].value == value) {
            PQEntry pq(value, newPriority);
            array[i] = pq;
        }
    }
}

//This sets the number of spaces in the array that the other functions look at to 0,
//effectively clearing the array. O(1).
void ArrayPriorityQueue::clear() {
    numInQueue = 0;
}

//This dequeues the element with the lowest priority, and if there are two elements
//with the same priority, dequeues the one that comes first in the alphabet. The
//dequeued value is returned, and the array's "size" is reduced by one to accommodate. O(N).
string ArrayPriorityQueue::dequeue() {
    if(numInQueue == 0) throw "The queue has no elements.";
    int priority = array[0].priority;
    string value = array[0].value;
    int index = 0;
    for(int i = 0; i < numInQueue; i++) {
        if(array[i].priority < priority || (array[i].priority == priority && array[i].value < value)) {
            index = i;
            priority = array[i].priority;
            value = array[i].value;
        }
    }
    for(int i = index + 1; i < numInQueue; i++) {
        array[i - 1] = array[i];
    }
    numInQueue--;
    return value;
}

//This enqueues the value the user enters at the priority enters. These
//are stored in a PQEntry which is put into the array at the last index. O(1).
void ArrayPriorityQueue::enqueue(string value, int priority) {
    if(numInQueue == capacity) {
        increaseArrayCapacity();
    }
    PQEntry pq(value, priority);
    array[numInQueue] = pq;
    numInQueue++;
}

//Returns whether the array is empty. O(1).
bool ArrayPriorityQueue::isEmpty() const {
    return(numInQueue == 0);
}

//Uses the same code as dequeue, but does not manipulate the queue after finding
//the value with the lowest priority integer and the lowest place in the alphabet. O(N).
string ArrayPriorityQueue::peek() const {
    if(size() == 0) throw "The queue has no elements.";
    int priority = array[0].priority;
    string value = array[0].value;
    for(int i = 0; i < numInQueue; i++) {
        if(array[i].priority < priority || (array[i].priority == priority && array[i].value < value)) {
            priority = array[i].priority;
            value = array[i].value;
        }
    }
    return value;
}

//Looks to see what the next value's priority is. O(N).
int ArrayPriorityQueue::peekPriority() const {
    if(size() == 0) throw "The queue has no elements.";
    int priority = array[0].priority;
    for(int i = 0; i < numInQueue; i++) {
        if(array[i].priority < priority) {
            priority = array[i].priority;
        }
    }
    return priority;
}

//Returns the size of the array. O(1).
int ArrayPriorityQueue::size() const {
    return numInQueue;
}

//Creates a readable version of the array. Inside a pair of braces, each
//value is listed alongside its priority number in the order they were
//enqueued. O(N).
string ArrayPriorityQueue::queueToString() const {
    string result = "{";
    for(int i = 0; i < numInQueue; i++) {
        result += "\"";
        result += array[i].value;
        result += "\"";
        result += ":";
        result += to_string(array[i].priority);
        if(i != numInQueue - 1) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

//Returns the ostream with the queue converted to a string. O(1).
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << queue.queueToString();
    return out;
}


