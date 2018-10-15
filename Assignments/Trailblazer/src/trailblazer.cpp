// This program implements Trailblazer, which looks for paths across
// mazes or terrain using several different methods that either do or do not
// acount for cost of each of those paths.

//Name: Noah Bailyn
//Section leader: Gianna Chien

#include "trailblazer.h"
#include "hashset.h"
#include "queue.h"
#include "pqueue.h"
#include "point.h"

using namespace std;
void recurse(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Vertex* nextAdd, bool& doneSearching, HashSet<Vertex*>& visited);

/*The recursive depthFirstSearchHelper function uses a method similar to the Boggle method that painstakingly
 * travles across each possible path, and returns the first path that it sees.
*/
void depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Vertex* nextAdd, bool& doneSearching, HashSet<Vertex*>& visited) {
    if((path.size() == 0 && doneSearching) || visited.contains(nextAdd)) {
    } else if(path.size() == 0) {
        recurse(graph, start, end, path, nextAdd, doneSearching, visited);
    } else if(path.get(0) == start && path.get(path.size() - 1) == end) {
        return;
    } else {
        recurse(graph, start, end, path, nextAdd, doneSearching, visited);
    }
}

/*The recursive call to depthFirstSearchHelper. Because it is called more than once in the recursive function,
 * a helper fucntion is used.
*/
void recurse(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Vertex*nextAdd, bool& doneSearching, HashSet<Vertex*>& visited) {
    if(path.size() >= 1) {
        if(path.get(0) == start && path.get(path.size() - 1) == end) return;
    }
    path.add(nextAdd);
    nextAdd->setColor(GREEN);
    Set<Vertex*> neighbors = graph.getNeighbors(nextAdd);
    visited.add(nextAdd);
    int j = neighbors.size();
    for(int i = 0; i < j; i++) {
        depthFirstSearchHelper(graph, start, end, path, neighbors.first(), doneSearching, visited);
        neighbors.remove(neighbors.first());
        if(path.size() >= 1) {
            if(path.get(0) == start && path.get(path.size() - 1) == end) return;
        }
    }
    neighbors.remove(nextAdd);
    path.removeBack();
    nextAdd->setColor(GRAY);
    doneSearching = true;
}

/*Sets parameters, calls the recursive depthFirstSearchHelper, and returns the path
 * to the main.
*/
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    Vertex* nextAdd = start;
    bool doneSearching = false;
    HashSet<Vertex*> visited;
    depthFirstSearchHelper(graph, start, end, path, nextAdd, doneSearching, visited);
    return path;
}

/*This method retraces steps for BFS, Dijkstra's, and A* by looking at the previous neighbors visited
 * for each vertex, and then continuing that process until the start is reached. This path
 * is stored in the path vector, and returned by each individual function.
*/
void retraceSteps(Vector<Vertex*>& visited, Vector<Vertex*>& path, BasicGraph& graph, Vertex*& currentV, Vertex* start) {
    bool f = false;
    bool g = true;
    while(!f) {
        path.insert(0, currentV);
        for(int i = 0; i < visited.size(); i++) {
            if(graph.getNeighbors(visited.get(i)).contains(currentV) && g) {
                currentV = visited.get(i);
                visited.remove(i);
                g = false;
            }
        }
        g = true;
        if(graph.getNeighbors(start).contains(path.get(0))) {
            f = true;
        }
    }
}

/*Breadth-first search expands out in every direction to look for paths to the
 * end, regardless of cost. This will always find the shortest path. After the end
 * has been found by chance, the retraceSteps function is called in order to recount
 * the steps taken to arrive at the end.
*/
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    Vector<Vertex*> visited;
    Vertex* currentV = end;
    queue.enqueue(start);
    visited.add(start);
    while(queue.size() != 0 && queue.peek() != end) {
        Vertex* v = queue.dequeue();
        v->setColor(GREEN);
        visited.add(v);
        Set<Vertex*> neighbors = graph.getNeighbors(v);
        int j = neighbors.size();
        for(int i = 0; i < j; i++) {
            if(!visited.contains(neighbors.first())) {
                queue.enqueue(neighbors.first());
                neighbors.first()->setColor(YELLOW);
            }
            neighbors.remove(neighbors.first());
        }
    }
    visited.add(queue.peek());
    end->setColor(GREEN);
    retraceSteps(visited, path, graph, currentV, start);
    path.insert(0, start);
    return path;
}

/*Like BFS, Dijsktra's algorithm also expands in every direction, though only expands
 * on the values first in its priority queue in order to find more efficient routes.
 * It calls the same "retraceSteps" function as BFS. I would have liked to include
 * a price checker for the Dijkstra's retraceSteps, but I ran into a bug that I couldn't
 * find the source of where the path would skip over its neighbors, even though it seemed impossible.
*/
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> queue;
    Vertex* vert;
    Vector<Vertex*> visited;
    HashMap<Vertex*, double> lookedAt;
    queue.enqueue(start, 0);
    while(queue.size() >= 1 && queue.peek() != end) {
        int a = queue.peekPriority();
        vert = queue.dequeue();
        visited.add(vert);
        vert->setColor(GREEN);
        Set<Vertex*> neighbors = graph.getNeighbors(vert);
        int j = neighbors.size();
        for(int i = 0; i < j; i++) {
            Vertex* vert2 = neighbors.first();
            double targetCost = graph.getEdge(vert, vert2)->weight;
            targetCost += a;
            if(!visited.contains(vert2)) {
                vert2->setColor(YELLOW);
                if(lookedAt.containsKey(vert2)) {
                    if(lookedAt.get(vert2) > targetCost) {
                        queue.changePriority(vert2, targetCost);
                        lookedAt.put(vert2, targetCost);
                    }
                } else {
                    queue.enqueue(vert2, targetCost);
                    lookedAt.put(vert2, targetCost);
                }
            }
            neighbors.remove(neighbors.first());
        }
    }
    end->setColor(GREEN);
    retraceSteps(visited, path, graph, end, start);
    path.insert(0, start);
    return path;
}

/*The heuristic for the A* search. The heuristic used is simply
 * the distance between the two points given.
*/
double heuristic(Vertex* start, Vertex* end) {
    double distance = 0;
    Point* startingPoint = (Point*) start->extraData;
    Point* endingPoint = (Point*) end->extraData;
    distance = sqrt((endingPoint->getX() - startingPoint->getX()) * (endingPoint->getX() - startingPoint->getX()) + (endingPoint->getY() - startingPoint->getY()) * (endingPoint->getY() - startingPoint->getY()));
    return distance;
}

/*The same method as Dijsktra's, though each of the priorities is given an added heuristic, as explained
 * above, in order to search with better efficiency. I ran into the exact
 * same bug for retraceSteps.
*/
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> queue;
    Vertex* vert;
    Vector<Vertex*> visited;
    HashMap<Vertex*, double> lookedAt;
    queue.enqueue(start, heuristic(start, end));
    while(queue.size() >= 1 && queue.peek() != end) {
        int a = queue.peekPriority();
        vert = queue.dequeue();
        visited.add(vert);
        vert->setColor(GREEN);
        Set<Vertex*> neighbors = graph.getNeighbors(vert);
        int j = neighbors.size();
        for(int i = 0; i < j; i++) {
            Vertex* vert2 = neighbors.first();
            double targetCost = graph.getEdge(vert, vert2)->weight;
            targetCost += a;
            if(!visited.contains(vert2)) {
                vert2->setColor(YELLOW);
                if(lookedAt.containsKey(vert2)) {
                    if(lookedAt.get(vert2) > targetCost) {
                        queue.changePriority(vert2, targetCost + heuristic(vert2, end));
                        lookedAt.put(vert2, targetCost);
                    }
                } else {
                    queue.enqueue(vert2, targetCost + heuristic(vert2, end));
                    lookedAt.put(vert2, targetCost);
                }
            }
            neighbors.remove(neighbors.first());
        }
    }
    end->setColor(GREEN);
    retraceSteps(visited, path, graph, end, start);
    path.insert(0, start);
    return path;
}

/*A helper function for Kruskal's, which determines whether or not
 * v1 and v2 are already connected (and by extension, in the same set).
*/
bool inSameSet(Map<Vertex*, Set<Vertex*>> map, Vertex* v1, Vertex* v2) {
    Set<Vertex*> s = map.get(v1);
    Set<Vertex*> t = map.get(v2);
    return (s.contains(v2) || t.contains(v1));
}

/*My attempt at Kruskal's algorithm. I couldn't quite get it to work properly, since
 * I can't really pinpoint exactly where the error is happening. Commented out is my
 * first attempt at implementing Kruskal's using a vector, which ended up looking slightly
 * closer to a maze but was a lot messier. The Kruskal's algorithm shown here implements a map
 * in order to keep track of clusters. For each edge, it is determined whether that edge is not
 * already in a cluster, would create a cycle because it already exists inside a cluster, should be
 * added to an existing cluster, or should merge two clusters. The original code implemented a vector
 * of sets that were clusters.
*/
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> edgeBank = graph.getEdgeSet();
    Set<Edge*> mst;
    //Vector<Set<Vertex*>> clusters;
    Map<Vertex*, Set<Vertex*>> clusterMap;
    PriorityQueue<Edge*> pqueue;
    //bool checked = false;
    for(Edge* v : edgeBank) {
        pqueue.enqueue(v, v->weight);
    }
    while(pqueue.size() > 0) {
        Edge* e = pqueue.dequeue();
        if(!clusterMap.containsKey(e->start) && !clusterMap.containsKey(e->finish)) {
            Set<Vertex*> newCluster;
            newCluster.add(e->start);
            newCluster.add(e->finish);
            clusterMap.put(e->start, newCluster);
            clusterMap.put(e->finish, newCluster);;
        } else if(clusterMap.containsKey(e->start) && clusterMap.containsKey(e->finish)) {
            if(clusterMap.get(e->start) == clusterMap.get(e->finish)) {
                mst.add(e);
            } else {
                Set<Vertex*> set1 = clusterMap.get(e->start);
                Set<Vertex*> set2 = clusterMap.get(e->finish);
                for(Vertex* v : set2) {
                    set1.add(v);
                }
                Map<Vertex*, Set<Vertex*>> proxyMap = clusterMap;
                for(Vertex* v : clusterMap) {
                    if(clusterMap.get(v).contains(e->start) || clusterMap.get(v).contains(e->finish)) {
                        proxyMap.remove(v);
                        proxyMap.put(v, set1);
                    }
                }
                clusterMap = proxyMap;
            }
        } else {
            Set<Vertex*> newCluster;
            if(clusterMap.containsKey(e->start)) {
                newCluster = clusterMap.get(e->start);
                newCluster.add(e->finish);
            } else {
                newCluster = clusterMap.get(e->finish);
                newCluster.add(e->start);
            }
            Map<Vertex*, Set<Vertex*>> proxyMap = clusterMap;
            for(Vertex* v : clusterMap) {
                if(clusterMap.get(v).contains(e->start) || clusterMap.get(v).contains(e->finish)) {
                    proxyMap.remove(v);
                    proxyMap.put(v, newCluster);
                }
            }
            clusterMap = proxyMap;
        }
    }
    /*
    while(pqueue.size() > 0) {
        Edge* e = pqueue.dequeue();
        if(!vectorContains(clusters, e->start) && !vectorContains(clusters, e->finish) && !checked) {
            Set<Vertex*> newCluster;
            newCluster.add(e->start);
            newCluster.add(e->finish);
            clusters.add(newCluster);
            cout << "new cluster" << endl;
            checked = true;
        } else {
            for(int i = 0; i < clusters.size(); i++) {
                if(clusters.get(i).contains(e->start) && clusters.get(i).contains(e->finish)) {
                    mst.remove(e);
                    cout << "blank" << endl;
                } else {
                    if(!checked && ((clusters.get(i).contains(e->start) && vectorContains(clusters, e->finish)) || (clusters.get(i).contains(e->finish) && vectorContains(clusters, e->start)))) {
                        for(int j = 0; j < clusters.size(); j++) {
                            if((clusters.get(j).contains(e->finish) && clusters.get(i).contains(e->start)) || (clusters.get(j).contains(e->start) && clusters.get(i).contains(e->finish))) {
                                mergeClusters(clusters, i, j);
                                checked = true;
                            }
                        }
                        cout << "merge cluster" << endl;
                    } else if(clusters.get(i).contains(e->finish) && !vectorContains(clusters, e->start) && !checked) {
                        Set<Vertex*> s = clusters.get(i);
                        s.add(e->start);
                        clusters.remove(i);
                        clusters.add(s);
                        cout << "new node" << endl;
                        checked = true;
                    } else if(clusters.get(i).contains(e->start) && !vectorContains(clusters, e->finish) && !checked) {
                        Set<Vertex*> s = clusters.get(i);
                        s.add(e->finish);
                        clusters.remove(i);
                        clusters.add(s);
                        cout << "new node" << endl;
                        checked = true;
                    }
                }
            }
        }
        checked = false;
    }*/
    return mst;
}
