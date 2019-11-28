/*
 * linkpredictor.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.27
 *
 * This file means to find the two subparts - the first where we predict
 * future collaborations between actors who have collaborated in the past and
 * the second where we predict new collaborations between actors.
 */
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

/**
 * Comparator of Node. In priority queue, Node higher priority(int)
 * has higher priority, and if count is the same, Node.getName() with lower
 * symbol has higher prioruty.
 */
struct NodePtrComp {
    /* Implement a comparator of node in struct NodePtrComp. */
    bool operator()(P& p1, P& p2) const {
        if (p1.second == p2.second) {
            return p1.first.getName() > p2.first.getName();
        }
        return p1.second < p2.second;
    }
};

/**
 * For a given actor/actress graph and a given query Actor A, find out which
 * contains actors who have collaborated with Actor A in the past, and output
 * the top 4 priority of them.
 */
vector<string> predictCollaborated(int nodeId, ActorGraph* graph) {
    priority_queue<P, vector<P>, NodePtrComp> pq;  // priority queue
    vector<map<int, Edges>> edges =
        graph->getEdges();                     // get all edges in graph
    vector<Node> nodes = graph->getNodes();    // get all nodes in graph
    map<int, Edges> neighbor = edges[nodeId];  // given node's neighbor
    for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
        int neighborId = iter->first;
        int priority = 0;
        // all the neighbor initially set priority is 0
        pq.push(P(nodes[neighborId], 0));
        // then update each neighbor bode according to the rule
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
            pq.push(P(nodes[neighborId],
                      priority));  // push the node and its priority into pq
        }
    }
    set<string> top4;  // top4 node set, inorder to eliminate the same node
    vector<string> top4vec;  // actually top4 node for return
    for (int i = 0; top4.size() < 4; i++) {
        if (pq.empty()) break;
        P nodePair;
        nodePair = pq.top();
        pq.pop();
        if (top4.find(nodePair.first.getName()) == top4.end()) {
            top4vec.push_back(
                nodePair.first.getName());  // if not in set, then we haven't
                                            // collect this node
        }
        top4.insert(nodePair.first.getName());  // insert this node into the set
    }
    return top4vec;
}

/**
 * For a given actor/actress graph and a given query Actor A, find out which
 * have not yet collaborated with Actor A in the past, and output the top 4
 * priority of them.
 */
vector<string> predictUnCollaborated(int nodeId, ActorGraph* graph) {
    priority_queue<P, vector<P>, NodePtrComp> pq;  // priority queue
    vector<map<int, Edges>> edges =
        graph->getEdges();                     // get all edges from graph
    vector<Node> nodes = graph->getNodes();    // get all nodes from graph
    vector<int> priority(nodes.size(), 0);     // priority vector for all nodes
    map<int, Edges> neighbor = edges[nodeId];  // neighbor of the given node
    vector<bool> visited(nodes.size(),
                         false);  // mark if a node has been visited
    visited[nodeId] = true;       // set the starter as visited
    // first set all the neighbor of the start node as visited and save the
    // number of edges between every neighbor and start node
    for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
        int neighborLevel1Id = iter->first;
        visited[neighborLevel1Id] = true;
        priority[neighborLevel1Id] =
            neighbor[neighborLevel1Id].getShared_movie().size();
    }
    // then update every third node's priority
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
    set<string> top4;  // top4 node set, inorder to eliminate the same node
    vector<string> top4vec;  // actually top4 node for return
    for (int i = 0; top4.size() < 4; i++) {
        if (pq.empty()) break;
        P nodePair;
        nodePair = pq.top();
        pq.pop();
        if (top4.find(nodePair.first.getName()) == top4.end()) {
            top4vec.push_back(
                nodePair.first.getName());  // if not in set, then we haven't
                                            // collect this node
        }
        top4.insert(nodePair.first.getName());  // insert this node into the set
    }
    return top4vec;
}

/**
 * This method read test actors' name from input file, then run two prediction
 * methods, and finally output the result to an output file.
 */
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

/**
 * Main method.
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[1], false);
    readFromFile(argv[2], argv[3], argv[4], &graph);
}
