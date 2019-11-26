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

/** class header */
class Edges {
  private:
    int node_one, node_two;
    vector<int> shared_movie;
    int weight;

  public:
    Edges();

    Edges(int node_one, int node_two, int movieId, int weight);

    Edges(const Edges& edge);

    void addSharedMovie(int movieId);

    void setWeight(int weight);

    vector<int>& getShared_movie();
};

#endif