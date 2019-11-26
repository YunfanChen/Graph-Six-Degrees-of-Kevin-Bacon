#include <ActorGraph.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
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

vector<string> predictUnCollaborated(int nodeId, ActorGraph* graph) {
    priority_queue<P, vector<P>, NodePtrComp> pq;
    vector<map<int, Edges>> edges = graph->getEdges();
    vector<Node> nodes = graph->getNodes();
    vector<int> priority(nodes.size(), 0);
    map<int, Edges> neighbor = edges[nodeId];
    vector<bool> visited(nodes.size(), false);
    visited[nodeId] = true;
    for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
        int neighborLevel1Id = iter->first;
        visited[neighborLevel1Id] = true;
        priority[neighborLevel1Id] =
            neighbor[neighborLevel1Id].getShared_movie().size();
    }
    for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
        int neighborLevel1Id = iter->first;
        map<int, Edges> neighborOfNeighbor = edges[neighborLevel1Id];
        for (auto iter2 = neighborOfNeighbor.begin();
             iter2 != neighborOfNeighbor.end(); iter2++) {
            int neighborLevel2Id = iter2->first;

            if (visited[neighborLevel2Id] == true) continue;
            priority[neighborLevel2Id] +=
                priority[neighborLevel1Id] *
                neighborOfNeighbor[neighborLevel2Id].getShared_movie().size();

            pq.push(P(nodes[neighborLevel2Id], priority[neighborLevel2Id]));
        }
    }
    vector<string> top4;
    for (int i = 0; top4.size() <= 4; i++) {
        if (pq.empty()) break;
        P nodePair;
        nodePair = pq.top();
        pq.pop();
        for (int i = 0; i < top4.size(); i++) {
            if (nodePair.first.getName() == top4[i])
                continue;
            else {
                top4.push_back(nodePair.first.getName());
                break;
            }
        }
    }
    return top4;
}

void readFromFile(const char* in_filename, string outFileName,
                  string outFileName2, ActorGraph* graph) {
    ifstream infile(in_filename);

    bool have_header = false;

    ofstream fileout;
    fileout.open(outFileName, std::ofstream::app | std::ofstream::out);
    ofstream fileout2;
    fileout2.open(outFileName2, std::ofstream::app | std::ofstream::out);

    fileout << "Actor1,Actor2,Actor3,Actor4"
            << "\n";
    fileout2 << "Actor1,Actor2,Actor3,Actor4"
             << "\n";
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        if (record.size() != 1) {
            // we should have exactly 2 columns
            continue;
        }

        string actorName(record[0]);

        vector<string> predictCol;
        vector<string> predictUnCol;
        map<string, int> nodeinfo = graph->getNodeinfo();
        predictCol = predictCollaborated(nodeinfo[actorName], graph);
        predictUnCol = predictUnCollaborated(nodeinfo[actorName], graph);

        for (int i = 0; i < predictCol.size(); i++) {
            fileout << predictCol[i];
            if (i != predictCol.size() - 1) fileout << "\t";
        }
        fileout << "\n";

        for (int i = 0; i < predictUnCol.size(); i++) {
            fileout2 << predictUnCol[i];
            if (i != predictUnCol.size() - 1) fileout2 << "\t";
        }
        fileout2 << "\n";
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return;
    }
    infile.close();
    fileout.close();
    fileout2.close();
}

int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[1], false);
    readFromFile(argv[2], argv[3], argv[4], &graph);
}
// /Code/cse100_pa4/data/myoutput_path_u.tsv
