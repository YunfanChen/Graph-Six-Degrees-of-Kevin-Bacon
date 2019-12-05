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

/**
 * Default constructor
 */
Edges::Edges() {}

/**
 * Get the shared_movie
 */
vector<int>& Edges::getShared_movie() { return shared_movie; }

/**
 * main constructor
 * node_one: first actor's id
 * node_two: second actor's id
 * movieId: one movie id they act together
 * weight: the weight of this edge
 */
Edges::Edges(int node_one, int node_two, int movieId) {
    this->node_one = node_one;
    this->node_two = node_two;
    addSharedMovie(movieId);
    this->smallestWeight = 0;
    this->weightMovie = 0;
}

/**
 * copy constructor
 */
Edges::Edges(const Edges& edge) {
    this->node_one = edge.node_one;
    this->node_two = edge.node_two;
    this->shared_movie = edge.shared_movie;
    this->weightMovie = edge.weightMovie;
    this->smallestWeight = edge.smallestWeight;
}

/**
 * Add a movie id to an edge exist.
 */
void Edges::addSharedMovie(int movieId) {
    this->shared_movie.push_back(movieId);
}

/**
 * Set a movie as the Edge's weighted movie.
 */
void Edges::setWeightMovie(int movieId) { this->weightMovie = movieId; }

/**
 * Get the weighted movie of this Edge.
 */
int Edges::getWeightMovie() { return this->weightMovie; }

/**
 * Get one of the node in this edge.
 */
int Edges::getNodeOne() { return node_one; }

/**
 * Get one of the node in this edge.
 */
int Edges::getNodeTwo() { return node_two; }

/**
 * Set a smallest weight for an edge.
 */
void Edges::setSmallestWeight(int weight) { this->smallestWeight = weight; }

/**
 * Get the smallest weight of this Edge.
 */
int Edges::getSmallestWeight() { return this->smallestWeight; }
