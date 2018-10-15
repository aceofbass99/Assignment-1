//This program implements a priority queue utilizing a heap to sort
//its members by priority. Each member of the heap has a lower priority
//than that of its children, which are at 2 times its index and 2 times
//its index plus one. Despite not being stored in order, the values are
//more efficiently returned in order than they are in the LinkedList version
//of the priority queue.

//Name: Noah Bailyn
//Section leader: Gianna Chien

#include "HeapPriorityQueue.h"
#include "PQEntry.h"

//See ArrayPriorityQueue.h for documentation member functions. O(1).
HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    numInQueue = 0;
    PQEntry* a = new PQEntry[capacity + 1];
    array = a;
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
HeapPriorityQueue::~HeapPriorityQueue() {
    delete array;
}

//See ArrayPriorityQueue.h for documentation member functions. O(N).
void HeapPriorityQueue::increaseArrayCapacity() {
    capacity *= 2;
    PQEntry* bigger = new PQEntry[capacity + 1];
    for(int i = 1; i <= numInQueue; i++) {
        bigger[i] = array[i];
    }
    delete array;
    array = bigger;
}

//See ArrayPriorityQueue.h for documentation member functions. O(N).
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    PQEntry pq(value, newPriority);
    int i = 1;
    for(i = 1; i <= numInQueue; i++) {
        if(array[i].value == value) {
            array[i] = pq;
            break;
        }
    }
    while((newPriority < array[i / 2].priority || (newPriority == array[i / 2].priority && value < array[i / 2].value)) && i != 1) {
        //swap
        array[i] = array[i / 2];
        array[i / 2] = pq;
        i /= 2;
    }
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
void HeapPriorityQueue::clear() {
    numInQueue = 0;
}

//See ArrayPriorityQueue.h for documentation member functions. O(logN).
string HeapPriorityQueue::dequeue() {
    if(numInQueue == 0) throw "The queue has no elements.";
    string returnValue = array[1].value;
    PQEntry pq = array[numInQueue];
    array[1] = pq;
    numInQueue--;
    int index = 1;
    if(index * 2 + 1 <= numInQueue) {
        //while either of the children has a lower priority
        while(index * 2 <= numInQueue && (pq.priority > array[index * 2].priority || pq.priority > array[index * 2 + 1].priority || (pq.priority == array[index * 2].priority && pq.value > array[index * 2].value) || (pq.priority == array[index * 2 + 1].priority && pq.value > array[index * 2 + 1].value))) {
            //if the child at index * 2 has a lower priority than the one at index * 2 + 1
            if(array[index * 2].priority < array[index * 2 + 1].priority || (array[index * 2].priority == array[index * 2 + 1].priority && array[index * 2].value < array[index * 2 + 1].value)) {
                //swap with index * 2
                array[index] = array[index * 2];
                array[index * 2] = pq;
                index *= 2;
            } else if(index * 2 + 1 <= numInQueue){
                //swap with index * 2 + 1
                array[index] = array[index * 2 + 1];
                array[index * 2 + 1] = pq;
                index = index * 2 + 1;
            }
        }
    } else if(index * 2 <= numInQueue) {
        if(pq.priority > array[index * 2].priority || (pq.priority == array[index * 2].priority && pq.value > array[index * 2].value)) {
            //swap with index * 2
            array[index] = array[index * 2];
            array[index * 2] = pq;
            index *= 2;
        }
    }
    return returnValue;
}

//See ArrayPriorityQueue.h for documentation member functions. O(logN).
void HeapPriorityQueue::enqueue(string value, int priority) {
    if(numInQueue == capacity) {
        increaseArrayCapacity();
    }
    numInQueue++;
    PQEntry newEntry(value, priority);
    array[numInQueue] = newEntry;
    int index = numInQueue;
    while((priority < array[index / 2].priority || (priority == array[index / 2].priority && value < array[index / 2].value)) && index != 1) {
        //swap
        array[index] = array[index / 2];
        array[index / 2] = newEntry;
        index /= 2;
    }
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
bool HeapPriorityQueue::isEmpty() const {
    return(numInQueue == 0);
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
string HeapPriorityQueue::peek() const {
    if(size() == 0) throw "The queue has no elements.";
    return array[1].value;
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
int HeapPriorityQueue::peekPriority() const {
    if(size() == 0) throw "The queue has no elements.";
    return array[1].priority;
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
int HeapPriorityQueue::size() const {
    return numInQueue;
}

//See ArrayPriorityQueue.h for documentation member functions. O(N).
string HeapPriorityQueue::queueToString() const {
    string result = "{";
    for(int i = 1; i <= numInQueue; i++) {
        result += "\"";
        result += array[i].value;
        result += "\"";
        result += ":";
        result += to_string(array[i].priority);
        if(i != numInQueue) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

//See ArrayPriorityQueue.h for documentation member functions. O(1).
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << queue.queueToString();
    return out;
}
