/*
 * Movie.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This file is implementation of method in Movie.hpp, define a Movie.
 */

#include "Movie.hpp"
#include <iostream>
#include <vector>

/**
 * This class represents an edge between two actors.
 */
Movie::Movie() {}

/**
 * main constructor
 * year: the year of this movie
 * id: the id of the movie
 * name: the name of the movie
 */
Movie::Movie(int year, int id, string& name) : name(name) {
    this->year = year;
    this->id = id;
    // this->name = name;
}

/**
 * Get the id of the movie
 */
int Movie::getId() { return this->id; }

/**
 * Get the name of the movie
 */
string Movie::getName() { return this->name; }

/**
 * Adding an actor to this movie
 */
void Movie::addActor(int actorId) { actors.push_back(actorId); }

/**
 * Get the year of the movie
 */
int Movie::getYear() { return this->year; }

/**
 * Get the actors involved in this movie
 */
vector<int>& Movie::getActor() { return this->actors; }

/**
 * Reset the weight of this movie
 */
void Movie::setWeight(int weight) { this->weight = weight; }

/**
 * Get the weight of this movie
 */
int Movie::getWeight() { return this->weight; }