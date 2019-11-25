/**
 * ActorGraph.hpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This class is for saving informations about node.
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
using namespace std;

/** class header */
class Node {
  private:
    string name;
    vector<int> movies;
    int id;

  public:
    /* initialize variables */
    Node();

    Node(string name, int movieId, int id);

    void addMovie(int movieId);

    int getId();

    string getName();
};

#endif