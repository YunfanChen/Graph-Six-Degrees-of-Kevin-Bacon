/*
 * ActorGraph.hpp
 * Author: Yunfan Chen
 * Date:   2019.11.25
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

using namespace std;

/**
 * This is the class for graph
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

    /**
     * Adding nodes and movies to an empty graph.
     */
    void addNodeAndMovie(string actor, string movie_title, int year);

    /**
     * Adding edges to a graph only has nodes and movies.
     */
    void buildEdges(bool use_weighted_edges);

    /**
     * For each movie, add edges between all the actors who involve.
     */
    void buildEdges4Movie(Movie movie, int weight);

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

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
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /**
     *  return all the nodes as a vector.
     */
    vector<Node>& getNodes();

    /**
     *  return all the movies as a vector.
     */
    vector<Movie>& getMovies();

    /**
     *  return all the edges as a vector.
     */
    vector<map<int, Edges>>& getEdges();

    /**
     *  return nodeinfo as a map.
     */
    map<string, int>& getNodeinfo();
};

#endif  // ACTORGRAPH_HPP
