/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <Edges.hpp>
#include <Movie.hpp>
#include <Node.hpp>
#include <iostream>
#include <map>

// Maybe include some data structures here

using namespace std;

/**
 * TODO: add class header
 */
class ActorGraph {
  protected:
    // Maybe add class data structure(s) here
    vector<Movie> movies;           // save index and movie object relations
    vector<Node> nodes;             // save index and node object relations
    vector<map<int, Edges>> edges;  // save index of node and its edges
    map<string, int> nodeinfo;      // key is actor's name, value is the index
    map<string, int> movieinfo;     // key is movie's name, value is the index
    int totalEdges;                 // the number of total edges

    void addNodeAndMovie(string actor, string movie_title, int year);

    void buildEdges(bool use_weighted_edges);

    void buildEdges4Movie(Movie movie, int weight);

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    // Maybe add some more methods here

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
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
};

#endif  // ACTORGRAPH_HPP
