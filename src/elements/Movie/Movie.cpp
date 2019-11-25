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

Movie::Movie() {}

Movie::Movie(int year, int id, string name) {
    this->year = year;
    this->id = id;
    this->name = name;
}

int Movie::getId() { return this->id; }

string Movie::getName() { return this->name; }

void Movie::addActor(int actorId) { actors.push_back(actorId); }

int Movie::getYear() { return this->year; }

vector<int> Movie::getActor() { return this->actors; }