/*
 * ActorGraph.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.25
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<double> node_stat(4, 0.0);

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {}

/**
 *  return all the nodes as a vector.
 */
vector<Node>& ActorGraph::getNodes() { return nodes; }

/**
 *  return all the movies as a vector.
 */
vector<Movie>& ActorGraph::getMovies() { return movies; }

/**
 *  return all the edges as a vector.
 */
vector<map<int, Edges>>& ActorGraph::getEdges() { return edges; }

/**
 *  return nodeinfo as a map.
 */
unordered_map<string, int>& ActorGraph::getNodeinfo() { return nodeinfo; }

/**
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 -
 * movie_year), otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
#include <ctime>
bool ActorGraph::loadFromFile(const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    clock_t read_start = clock();
    bool have_header = false;

    // keep reading lines until the end of file is reached
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

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[2]);

        // add all the nodes and movies in this empty graph.
        addNodeAndMovie(actor, movie_title, year);
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    clock_t read_end = clock();
    clock_t edge_start = clock();
    buildEdges(use_weighted_edges);
    clock_t edge_end = clock();
    return true;
}

/**
 * Adding nodes and movies to an graph.
 */
void ActorGraph::addNodeAndMovie(string& actor, string& movie_title, int year) {
    // since may some movies have same name, I use title+year as key
    string movie_key = movie_title + " " + to_string(year);
    // two flag indicate if create a new object or just renew the object we
    // already have.
    bool hasNode = (nodeinfo.find(actor) == nodeinfo.end()) ? false : true;
    bool hasMovie =
        (movieinfo.find(movie_key) == movieinfo.end()) ? false : true;
    // for new movie and new actor
    clock_t stat_start = clock();
    int branch_case = 0;
    if (!hasNode && !hasMovie) {
        Node node(actor, movies.size(), nodes.size());
        Movie movie(year, movies.size(), movie_title);
        movie.addActor(node.getId());

        nodeinfo[actor] = nodes.size();
        movieinfo[movie_key] = movies.size();

        movies.push_back(movie);
        nodes.push_back(node);
        branch_case = 0;
    } else if (hasNode && !hasMovie) {  // for actor we collected and new movie
        Movie movie(year, movies.size(), movie_title);
        Node node = nodes[nodeinfo[actor]];

        movie.addActor(node.getId());
        nodes[nodeinfo[actor]].addMovie(movie.getId());

        movieinfo[movie_key] = movies.size();

        movies.push_back(movie);
        branch_case = 1;
    } else if (!hasNode && hasMovie) {  // for movie we collected and new actor
        Movie movie = movies[movieinfo[movie_key]];
        Node node(actor, movie.getId(), nodes.size());

        movies[movieinfo[movie_key]].addActor(node.getId());

        nodeinfo[actor] = nodes.size();

        nodes.push_back(node);
        branch_case = 2;
    } else if (hasNode && hasMovie) {  // for both actor and movie we collected
        Node node = nodes[nodeinfo[actor]];
        Movie movie = movies[movieinfo[movie_key]];
        movies[movieinfo[movie_key]].addActor(node.getId());
        nodes[nodeinfo[actor]].addMovie(movie.getId());
        branch_case = 3;
    } else {  // will not in this branch any time
        cout << "Branch error!" << endl;
    }

    clock_t stat_end = clock();
    node_stat[branch_case] +=
        stat_end / (double)CLOCKS_PER_SEC - stat_start / (double)CLOCKS_PER_SEC;
}

/**
 * Adding edges to a graph only has nodes and movies.
 */
void ActorGraph::buildEdges(bool use_weighted_edges) {
    edges.resize(nodes.size(), {});
    for (int i = 0; i < movies.size(); i++) {
        int weight =
            (use_weighted_edges) ? (1 - (2019 - movies[i].getYear())) : 1;
        buildEdges4Movie(movies[i], weight);
    }  // for every movie, build edges in it according to actors involved
}

/**
 * For each movie, add edges between all the actors who involve.
 */
void ActorGraph::buildEdges4Movie(Movie& movie, int weight) {
    vector<int>& actorsOfMovie = movie.getActor();
    int id = movie.getId();
    for (int i = 0; i < actorsOfMovie.size(); i++) {
        for (int j = i + 1; j < actorsOfMovie.size(); j++) {
            // two actors' id
            int actorIdOne = actorsOfMovie[i];
            int actorIdTwo = actorsOfMovie[j];
            // two actors' map
            map<int, Edges>& mapOfActorOne = edges[actorIdOne];
            map<int, Edges>& mapOfActorTwo = edges[actorIdTwo];
            // if they already built an edge before
            if (mapOfActorOne.find(actorIdTwo) == mapOfActorOne.end()) {
                Edges edge(actorIdOne, actorIdTwo, id, weight);
                mapOfActorOne[actorIdTwo] = edge;
            } else {
                mapOfActorOne[actorIdTwo].addSharedMovie(id);
            }
            // if they already built an edge before
            if (mapOfActorTwo.find(actorIdOne) == mapOfActorTwo.end()) {
                Edges edge(actorIdTwo, actorIdOne, id, weight);
                mapOfActorTwo[actorIdOne] = edge;
            } else {
                mapOfActorTwo[actorIdOne].addSharedMovie(id);
            }
            // push back the edge
            this->totalEdges += 2;  // count for total edge number
        }
    }
}