/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
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

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {}

vector<Node>& ActorGraph::getNodes() { return nodes; }

vector<Movie>& ActorGraph::getMovies() { return movies; }

vector<map<int, Edges>>& ActorGraph::getEdges() { return edges; }

map<string, int>& ActorGraph::getNodeinfo() { return nodeinfo; }

/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 -
 * movie_year), otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

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

        // TODO: we have an actor/movie relationship, now what?
        addNodeAndMovie(actor, movie_title, year);
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    buildEdges(use_weighted_edges);
    // cout << "Total nodes are: " << nodes.size() << endl;
    // cout << "Total movies are: " << movies.size() << endl;

    return true;
}

void ActorGraph::addNodeAndMovie(string actor, string movie_title, int year) {
    string movie_key = movie_title + " " + to_string(year);
    bool hasNode = (nodeinfo.find(actor) == nodeinfo.end()) ? false : true;
    bool hasMovie =
        (movieinfo.find(movie_key) == movieinfo.end()) ? false : true;
    if (!hasNode && !hasMovie) {
        Node node(actor, movies.size(), nodes.size());
        Movie movie(year, movies.size(), movie_title);
        movie.addActor(node.getId());

        nodeinfo[actor] = nodes.size();
        movieinfo[movie_key] = movies.size();

        movies.push_back(movie);
        nodes.push_back(node);
    } else if (hasNode && !hasMovie) {
        Movie movie(year, movies.size(), movie_title);
        Node node = nodes[nodeinfo[actor]];

        movie.addActor(node.getId());
        nodes[nodeinfo[actor]].addMovie(movie.getId());

        movieinfo[movie_key] = movies.size();

        movies.push_back(movie);
    } else if (!hasNode && hasMovie) {
        Movie movie = movies[movieinfo[movie_key]];
        Node node(actor, movie.getId(), nodes.size());

        movies[movieinfo[movie_key]].addActor(node.getId());

        nodeinfo[actor] = nodes.size();

        nodes.push_back(node);
    } else if (hasNode && hasMovie) {
        Node node = nodes[nodeinfo[actor]];
        Movie movie = movies[movieinfo[movie_key]];
        movies[movieinfo[movie_key]].addActor(node.getId());
        nodes[nodeinfo[actor]].addMovie(movie.getId());
    } else {
        cout << "Branch error!" << endl;
    }
}

void ActorGraph::buildEdges(bool use_weighted_edges) {
    for (int i = 0; i < movies.size(); i++) {
        map<int, Edges> newmap;
        edges.push_back(newmap);
    }

    for (int i = 0; i < movies.size(); i++) {
        int weight =
            (use_weighted_edges) ? (1 - (2019 - movies[i].getYear())) : 1;
        buildEdges4Movie(movies[i], weight);
    }
}

void ActorGraph::buildEdges4Movie(Movie movie, int weight) {
    vector<int> actorsOfMovie = movie.getActor();
    int id = movie.getId();
    for (int i = 0; i < actorsOfMovie.size(); i++) {
        for (int j = i + 1; j < actorsOfMovie.size(); j++) {
            int actorIdOne = actorsOfMovie[i];
            int actorIdTwo = actorsOfMovie[j];
            map<int, Edges> mapOfActorOne = edges[actorIdOne];
            map<int, Edges> mapOfActorTwo = edges[actorIdTwo];
            if (mapOfActorOne.find(actorIdTwo) == mapOfActorOne.end()) {
                Edges edge(actorIdOne, actorIdTwo, id, weight);
                mapOfActorOne[actorIdTwo] = edge;
            } else {
                mapOfActorOne[actorIdTwo].addSharedMovie(id);
            }

            if (mapOfActorTwo.find(actorIdOne) == mapOfActorTwo.end()) {
                Edges edge(actorIdTwo, actorIdOne, id, weight);
                mapOfActorTwo[actorIdOne] = edge;
            } else {
                mapOfActorTwo[actorIdOne].addSharedMovie(id);
            }
            edges[actorIdOne] = mapOfActorOne;
            edges[actorIdTwo] = mapOfActorTwo;
            this->totalEdges += 2;
        }
    }
}