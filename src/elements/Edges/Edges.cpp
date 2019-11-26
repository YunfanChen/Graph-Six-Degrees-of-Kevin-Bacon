/*
 * Edges.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This file is implementation of method in Edges.hpp, define an Edge.
 */

#include "Edges.hpp"
#include <iostream>
#include <vector>

Edges::Edges() {}

vector<int>& Edges::getShared_movie() { return shared_movie; }

Edges::Edges(int node_one, int node_two, int movieId, int weight) {
    this->node_one = node_one;
    this->node_two = node_two;
    addSharedMovie(movieId);
    this->weight = weight;
}

Edges::Edges(const Edges& edge) {
    this->node_one = edge.node_one;
    this->node_two = edge.node_two;
    this->shared_movie = edge.shared_movie;
    this->weight = edge.weight;
}

void Edges::addSharedMovie(int movieId) {
    this->shared_movie.push_back(movieId);
}

void Edges::setWeight(int weight) { this->weight = weight; }