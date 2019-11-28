/**
 * Node.hpp
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

/**
 * This class is for movie
 */
class Node {
  private:
    string name;
    vector<int> movies;
    int id;

  public:
    /**
     * This class represents an node which is an actor.
     */
    Node();

    /**
     * main constructor
     * name: the name of the actor
     * movieId: a movie this actor acted
     * id: the id of this actor.
     */
    Node(string name, int movieId, int id);

    /**
     *  Add a movie to this node.
     */
    void addMovie(int movieId);

    /**
     * Get the id of this node
     */
    int getId();

    /**
     * Get all the movies for this node
     */
    vector<int> getMovies();

    /**
     * Get the name of this actor
     */
    string getName();
};

#endif