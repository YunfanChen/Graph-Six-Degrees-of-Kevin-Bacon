/*
 * movietraveler.cpp
 * Author: Yunfan Chen
 * Date:   2019.12.2
 *
 * This file means to find the minimum spanning tree of the graph.
 */
#include <ActorGraph.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define INF 0x7fffffff
typedef pair<Edges, int> P;
vector<vector<int>> unionNodeSet;
unordered_map<int, int> father;
vector<Edges> treePath;
int nodeNum = 0, edgeNum = 0, weightNum = 0;
/**
 * Comparator of Edges. In priority queue, Node lower weight(int)
 * has higher priority.
 */
struct EdgesPtrComp {
    /* Implement a comparator of edge in struct NodePtrComp. */
    bool operator()(P& p1, P& p2) const { return p1.second > p2.second; }
};

int find(int nodeId) {
    while (father[nodeId] != nodeId) nodeId = father[nodeId];
    return nodeId;
}

void setFather(vector<int> child, int fatherId) {
    for (int i = 0; i < child.size(); i++) {
        father[child[i]] = fatherId;
    }
    return;
}

void Union(Node* nodeOne, Node* nodeTwo, Edges& edge, vector<Node>& nodes) {
    int fatherOne = find(nodeOne->getId());
    int fatherTwo = find(nodeTwo->getId());
    bool flag1 = false, flag2 = false;
    if (unionNodeSet[fatherOne].size() == 0) {
        unionNodeSet[fatherOne].push_back(nodeOne->getId());
        nodeOne->setTotalWeight(edge.getSmallestWeight());
        flag1 = true;
    }
    if (unionNodeSet[fatherTwo].size() == 0) {
        unionNodeSet[fatherTwo].push_back(nodeTwo->getId());
        nodeTwo->setTotalWeight(edge.getSmallestWeight());
        flag2 = true;
    }
    if (fatherOne != fatherTwo) {
        vector<int>& child = unionNodeSet[fatherTwo];
        setFather(child, fatherOne);
        unionNodeSet[fatherOne].insert(unionNodeSet[fatherOne].end(),
                                       unionNodeSet[fatherTwo].begin(),
                                       unionNodeSet[fatherTwo].end());
        int curWeight = nodes[fatherOne].getTotalWeight() +
                        nodes[fatherTwo].getTotalWeight();
        if (flag1 && flag2) curWeight = curWeight / 2;
        // cout << "for node " << nodeOne->getId() << " and " <<
        // nodeTwo->getId()
        //      << " , the weight of father " << fatherOne << " is " <<
        //      curWeight
        //      << endl;
        nodes[fatherOne].setTotalWeight(curWeight);
        unionNodeSet[fatherTwo].clear();
        treePath.push_back(edge);
        // cout << "-------" << edge.getSmallestWeight() << "-----" << endl;
    }
}

Movie getSmallestWeight(ActorGraph* graph, Edges& edge) {
    vector<int>& sharedMovie = edge.getShared_movie();
    vector<Movie>& movies = graph->getMovies();
    int minWeight = INF;
    int minId = -1;
    for (int i = 0; i < sharedMovie.size(); i++) {
        if (minWeight > movies[sharedMovie[i]].getWeight()) {
            minWeight = movies[sharedMovie[i]].getWeight();
            minId = movies[sharedMovie[i]].getId();
        }
    }
    return movies[minId];
}

void kruskal(ActorGraph* graph) {
    vector<Node>& nodes = graph->getNodes();
    priority_queue<P, vector<P>, EdgesPtrComp> pq;
    vector<map<int, Edges>>& edges = graph->getEdges();

    for (int i = 0; i < nodes.size(); i++) {
        father[i] = i;
    }
    // sort the Edges according to the weights
    for (int i = 0; i < nodes.size(); i++) {
        map<int, Edges>& neighbor = edges[i];
        for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
            Edges& neighborEdge = neighbor[iter->first];
            Movie movie = getSmallestWeight(graph, neighborEdge);
            neighborEdge.setWeightMovie(movie.getId());
            // cout << "set edge is: " << neighborEdge.getWeightMovie() << endl;
            neighborEdge.setSmallestWeight(movie.getWeight());
            pq.emplace(neighborEdge, movie.getWeight());
        }
    }
    while (!pq.empty()) {
        P pair = pq.top();
        pq.pop();
        Edges& e = pair.first;
        int nodeOne = e.getNodeOne();
        int nodeTwo = e.getNodeTwo();
        // cout << "weight movie of " << nodeOne << " and " << nodeTwo << ": "
        //      << e.getWeightMovie() << endl;
        Union(&nodes[nodeOne], &nodes[nodeTwo], e, nodes);
    }
    return;
}

void writeFile(string outFileName, ActorGraph* graph) {
    ofstream fileout;
    fileout.open(outFileName, std::ofstream::app | std::ofstream::out);

    fileout << "(actor)<--[movie#@year]-->(actor)"
            << "\n";

    vector<Node>& nodes = graph->getNodes();
    vector<Movie>& movies = graph->getMovies();

    kruskal(graph);

    for (int i = 0; i < treePath.size(); i++) {
        Edges& edge = treePath[i];
        int nodeOne = edge.getNodeOne();
        int nodeTwo = edge.getNodeTwo();
        int movieId = edge.getWeightMovie();
        fileout << "(" << nodes[nodeOne].getName() << ")<--["
                << movies[movieId].getName() << "#@"
                << movies[movieId].getYear() << "]-->("
                << nodes[nodeTwo].getName() << ")" << endl;
    }
    int rootId = find(0);
    fileout << "#NODE CONNECTED: " << unionNodeSet[rootId].size() << endl;
    fileout << "#EDGE CHOSEN: " << treePath.size() << endl;
    fileout << "TOTAL EDGE WEIGHTS: " << nodes[rootId].getTotalWeight();
    return;
}

/**
 * Main method.
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[1], true);
    int size = graph.getNodes().size();
    unionNodeSet.resize(size);
    string outFileName(argv[2]);
    writeFile(outFileName, &graph);
}