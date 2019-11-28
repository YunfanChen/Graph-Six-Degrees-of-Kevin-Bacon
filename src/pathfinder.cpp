/*
 * pathfinder.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.27
 *
 * This file means to find the shortest way from one actor to another. And
 * output the path in a file.
 */
#include <ActorGraph.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * This function is implementing BFS from one actor node in the graph. And
 * finally return a path save in a vector<string>.
 */
vector<string> bfs(int startId, int endId, ActorGraph* graph) {
    vector<Node> nodes = graph->getNodes();             // get the node list
    vector<map<int, Edges>> edges = graph->getEdges();  // get the edges list
    vector<Movie> movies = graph->getMovies();          // get the movie list
    vector<bool> visited(nodes.size(),
                         false);  // indicate whether this node has been visited
    map<int, int> pre;            // save for pre node
    bool find = false;            // flag indicate whether find the endpoint
    queue<int> q;                 // queue for bfs
    q.push(startId);              // add start node
    while (!find && !q.empty()) {
        int preId = q.front();
        q.pop();
        visited[preId] = true;  // this node has been visited
        map<int, Edges> neighbor = edges[preId];
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
        map<int, Edges> mapp = edges[preId];
        Edges edgee = mapp[nextId];
        vector<int> shared_movie = edgee.getShared_movie();
        int movieId = shared_movie[0];
        Movie movie = movies[movieId];
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
                  ActorGraph* graph) {
    ifstream infile(in_filename);

    bool have_header = false;

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
        map<string, int> nodeinfo = graph->getNodeinfo();
        path = bfs(nodeinfo[actorOne], nodeinfo[actorTwo], graph);

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
    readFromFile(argv[3], outFileName, &graph);
}