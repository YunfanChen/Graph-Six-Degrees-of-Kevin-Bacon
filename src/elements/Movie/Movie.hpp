/**
 * Movie.hpp
 * Author: Yunfan Chen
 * Date:   2019.11.23
 *
 * This class is for saving informations about Movie.
 */
#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <iostream>
#include <vector>
using namespace std;

/** class header */
class Movie {
  private:
    int year;
    int id;
    string name;
    vector<int> actors;

  public:
    Movie();

    Movie(int year, int id, string name);

    int getId();

    string getName();

    void addActor(int actorId);

    int getYear();

    vector<int> getActor();
};

#endif