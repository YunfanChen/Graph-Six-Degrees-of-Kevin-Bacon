#include <ActorGraph.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef pair<Node, int> P;

/* Comparator of HCNode pointer. In priority queue, HCNode ptr with lower count
 * has higher priority, and if count is the same, HCNode ptr with larger symbol
 * has higher prioruty.
 */
struct NodePtrComp {
    /* Implement a comparator of node in struct NodePtrComp. */
    bool operator()(P p1, P p2) const {
        if (p1.second == p2.second) {
            return p1.first.getName() > p1.first.getName();
        }
        return p1.second < p2.second;
    }
};

vector<string> predictCollaborated(int nodeId, ActorGraph* graph) {
    priority_queue<P, vector<P>, NodePtrComp> pq;
    vector<map<int, Edges>> edges = graph->getEdges();
    vector<Node> nodes = graph->getNodes();
    map<int, Edges> neighbor = edges[nodeId];
    for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
        int neighborId = iter->first;
        int priority = 0;
        for (auto iter2 = neighbor.begin(); iter2 != neighbor.end(); iter2++) {
            if (iter2 == iter) continue;
            int thirdNodeId = iter2->first;
            map<int, Edges> neighborMap = edges[neighborId];
            if (neighborMap.find(thirdNodeId) == neighborMap.end()) {
                continue;
            } else {
                priority += neighborMap[thirdNodeId].getShared_movie().size() *
                            neighbor[thirdNodeId].getShared_movie().size();
            }
            //(nodes[neighborId],priority)
            pq.push(P(nodes[neighborId], priority));
        }
    }
    P popNode;
    vector<string> top4;
    for (int i = 0; i < 4; i++) {
        if (pq.empty()) break;
        popNode = pq.top();
        top4.push_back(popNode.first.getName());
        pq.pop();
    }
    return top4;
}

vector<string> predictUnCollaborated(int nodeId) {}

int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile("/Code/cse100_pa4/data/imdb_small_sample.tsv", false);
    vector<string> predictCol;
    predictCol = predictCollaborated(1, &graph);
    for (int i = 0; i < predictCol.size(); i++) {
        cout << predictCol[i] << " ";
    }
    cout << endl;
}