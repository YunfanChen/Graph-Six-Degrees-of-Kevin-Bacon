/*
 * pathfinder.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.27
 *
 * This file means to find the shortest way from one actor to another. And
 * output the path in a file.
 */
#include <ActorGraph.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#define INF 0x7fffffff

using namespace std;
typedef pair<Node, int> P;

/**
 * Comparator of Node. In priority queue, Node lower weight(int)
 * has higher priority, and if count is the same, Node.getName() with lower
 * symbol has higher prioruty.
 */
struct NodePtrComp {
    /* Implement a comparator of node in struct NodePtrComp. */
    bool operator()(P& p1, P& p2) const {
        if (p1.second == p2.second) {
            return p1.first.getName() > p2.first.getName();
        }
        return p1.second > p2.second;
    }
};

Movie getSmallestWeight(ActorGraph* graph, Edges* edge) {
    vector<int>& sharedMovie = edge->getShared_movie();
    vector<Movie>& movies = graph->getMovies();
    int minWeight = 100000;
    int minId = -1;
    for (int i = 0; i < sharedMovie.size(); i++) {
        if (minWeight > movies[sharedMovie[i]].getWeight()) {
            minWeight = movies[sharedMovie[i]].getWeight();
            minId = movies[sharedMovie[i]].getId();
        }
    }
    return movies[minId];
}

vector<string> dijkstra(ActorGraph* graph, int startNode, int endNode) {
    vector<Node>& nodes = graph->getNodes();
    vector<int> distance(nodes.size(), INF);
    distance[startNode] = 0;
    vector<map<int, Edges>>& edges = graph->getEdges();
    map<int, Edges>& neighbor = edges[startNode];
    priority_queue<P, vector<P>, NodePtrComp> pq;
    map<int, int> parent;

    pq.emplace(nodes[startNode], 0);
    while (!pq.empty()) {
        P pair = pq.top();
        pq.pop();
        Node curNode = pair.first;
        int curWeight = pair.second;
        if (curWeight >= distance[endNode]) break;
        int curNodeId = curNode.getId();
        map<int, Edges>& neighborOfNeighbor = edges[curNodeId];
        for (auto iter = neighborOfNeighbor.begin();
             iter != neighborOfNeighbor.end(); iter++) {
            cout << "If " << distance[iter->first] << " > "
                 << distance[curNodeId] << " + "
                 << getSmallestWeight(graph, &iter->second).getWeight() << "?"
                 << endl;
            if (distance[iter->first] >
                distance[curNodeId] +
                    getSmallestWeight(graph, &iter->second).getWeight()) {
                cout << "yes" << endl;
                distance[iter->first] =
                    distance[curNodeId] +
                    getSmallestWeight(graph, &iter->second).getWeight();
                cout << "now distance is : " << distance[iter->first] << endl;
                parent[iter->first] = curNodeId;
                pq.emplace(nodes[iter->first], distance[iter->first]);
            }
            cout << "no" << endl;
        }
    }
    vector<string> path;
    if (distance[endNode] == INF) return path;
    if (startNode == endNode) return path;
    int curNode = endNode;
    while (curNode != startNode) {
        path.push_back(nodes[curNode].getName());
        Movie curMovie =
            getSmallestWeight(graph, &edges[curNode][parent[curNode]]);
        string curMovieInfo =
            curMovie.getName() + "#@" + to_string(curMovie.getYear());
        path.push_back(curMovieInfo);
        curNode = parent[curNode];
    }
    path.push_back(nodes[curNode].getName());
    return path;
}

/**
 * This function is implementing BFS from one actor node in the graph. And
 * finally return a path save in a vector<string>.
 */
vector<string> bfs(int startId, int endId, ActorGraph* graph) {
    vector<Node>& nodes = graph->getNodes();             // get the node list
    vector<map<int, Edges>>& edges = graph->getEdges();  // get the edges list
    vector<Movie>& movies = graph->getMovies();          // get the movie list
    vector<bool> visited(nodes.size(),
                         false);  // indicate whether this node has been visited
    unordered_map<int, int> pre;  // save for pre node
    bool find = false;            // flag indicate whether find the endpoint
    queue<int> q;                 // queue for bfs
    q.push(startId);              // add start node
    while (!find && !q.empty()) {
        int preId = q.front();
        q.pop();
        visited[preId] = true;  // this node has been visited
        map<int, Edges>& neighbor = edges[preId];
        // iter all the neighbor node
        for (auto iter = neighbor.begin(); iter != neighbor.end(); iter++) {
            int neighborId = iter->first;

            // only enqueue the node we have not visited
            if (visited[neighborId] == false) {
                pre[neighborId] = preId;
                q.push(neighborId);
                visited[neighborId] = true;
                // when we find the end node
                if (neighborId == endId) {
                    find = true;
                    break;
                }
            }
        }
    }
    vector<string> path;  // save the path for both movie an actor
    if (find == false) {
        return path;  // cannot find the endnode, then they are unconnected
    }
    int preId = endId;
    path.push_back(nodes[preId].getName());
    while (preId != startId) {
        int nextId = pre[preId];
        map<int, Edges>& mapp = edges[preId];
        Edges& edgee = mapp[nextId];
        vector<int>& shared_movie = edgee.getShared_movie();
        int movieId = shared_movie[0];
        Movie& movie = movies[movieId];
        string movie_info =
            movie.getName() + "#@" +
            to_string(movie.getYear());  // the movie they shared
        path.push_back(movie_info);
        path.push_back(nodes[nextId].getName());
        preId = nextId;
    }
    return path;
}

/**
 * Read test actors from input file, through BFS method get the path and write
 * in a output file.
 */
void readFromFile(const char* in_filename, string outFileName,
                  ActorGraph* graph, bool weighted) {
    ifstream infile(in_filename);

    bool have_header = false;
    clock_t bfs_start = clock();
    ofstream fileout;
    fileout.open(outFileName, std::ofstream::app | std::ofstream::out);

    fileout << "(actor)--[movie#@year]-->(actor)--..."
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

        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }

        string actorOne(record[0]);
        string actorTwo(record[1]);

        vector<string> path;
        unordered_map<string, int>& nodeinfo = graph->getNodeinfo();
        if (weighted) {
            path = dijkstra(graph, nodeinfo[actorOne], nodeinfo[actorTwo]);
        } else {
            path = bfs(nodeinfo[actorOne], nodeinfo[actorTwo], graph);
        }

        for (int i = path.size() - 1; i >= 0; i--) {
            if (i != path.size() - 1 && i % 2 == 0) fileout << ">";
            if (i % 2 == 0)
                fileout << "(" << path[i] << ")";
            else
                fileout << "[" << path[i] << "]";
            if (i != 0) fileout << "--";
        }
        fileout << "\n";
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return;
    }
    clock_t bfs_end = clock();
    infile.close();
    fileout.close();
}

/**
 * Main method.
 */
int main(int argc, char* argv[]) {
    string ifWeighted(argv[2]);  // save "u" or "w"
    bool weighted = false;
    if (ifWeighted == "w") weighted = true;
    ActorGraph graph;
    graph.loadFromFile(argv[1], weighted);
    string outFileName(argv[4]);
    readFromFile(argv[3], outFileName, &graph, weighted);
}
// /Code/cse100_pa4/data/my_out_paths_unweighted.tsv