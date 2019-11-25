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

Node::Node() {}

Node::Node(string name, int movieId, int id) {
    this->name = name;
    this->addMovie(movieId);
    this->id = id;
}

void Node::addMovie(int movieId) { this->movies.push_back(movieId); }

int Node::getId() { return this->id; }

string Node::getName() { return this->name; }