/*
 * Node.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This file is implementation of method in Node.hpp, define a Node.
 */

#include "Node.hpp"
#include <iostream>
#include <vector>

/**
 * This class represents an node which is an actor.
 */
Node::Node() {}

/**
 * main constructor
 * name: the name of the actor
 * movieId: a movie this actor acted
 * id: the id of this actor.
 */
Node::Node(string& name, int movieId, int id) : name(name) {
    // this->name = name;
    this->addMovie(movieId);
    this->id = id;
}

/**
 *  Add a movie to this node.
 */
void Node::addMovie(int movieId) { this->movies.push_back(movieId); }

/**
 * Get the id of this node
 */
vector<int> Node::getMovies() { return this->movies; }

/**
 * Get all the movies for this node
 */
int Node::getId() { return this->id; }

/**
 * Get the name of this actor
 */
string Node::getName() { return this->name; }

/**
 * Set total number of edge.
 */
void Node::setTotalEdge(int edgeNum) { this->totalEdge = edgeNum; }

/**
 * Get the total number of Edge.
 */
int Node::getTotalEdge() { return this->totalEdge; }

/**
 * Set total number of node.
 */
void Node::setTotalNode(int nodeNum) { this->totalNode = nodeNum; }

/**
 * Get the total number of node.
 */
int Node::getTotalNode() { return this->totalNode; }

/**
 * Set total weight.
 */
void Node::setTotalWeight(int weightNum) { this->totalWeight = weightNum; }

/**
 * Get the total weight.
 */
int Node::getTotalWeight() { return this->totalWeight; }