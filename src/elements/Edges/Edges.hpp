/**
 * Edges.hpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This class is for saving informations about Edge.
 */
#ifndef EDGES_HPP
#define EDGES_HPP

#include <iostream>
#include <vector>
using namespace std;

/**
 * This class represents an edge between two actors.
 */
class Edges {
  private:
    int node_one, node_two;    // two actors' node ID
    vector<int> shared_movie;  // the movie history they act together
    int weightMovie;
    int smallestWeight;

  public:
    /**
     * Default constructor
     */
    Edges();

    /**
     * main constructor
     * node_one: first actor's id
     * node_two: second actor's id
     * movieId: one movie id they act together
     * weight: the weight of this edge
     */
    Edges(int node_one, int node_two, int movieId);

    /**
     * copy constructor
     */
    Edges(const Edges& edge);

    /**
     * Add a movie id to an edge exist.
     */
    void addSharedMovie(int movieId);

    /**
     * Get the shared_movie
     */
    vector<int>& getShared_movie();

    /**
     * Set a movie as the Edge's weighted movie.
     */
    void setWeightMovie(int movieId);

    /**
     * Get the weighted movie of this Edge.
     */
    int getWeightMovie();

    /**
     * Get one of the node in this edge.
     */
    int getNodeOne();

    /**
     * Get one of the node in this edge.
     */
    int getNodeTwo();

    /**
     * Set a smallest weight for an edge.
     */
    void setSmallestWeight(int weight);

    /**
     * Get the smallest weight of this Edge.
     */
    int getSmallestWeight();
};

#endif