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

/**
 * This class is for movie
 */
class Movie {
  private:
    int year;            // movie act year
    int id;              // the id of the movie
    string name;         // the name of the movie
    vector<int> actors;  // actors involved in this movie

  public:
    /**
     * This class represents an edge between two actors.
     */
    Movie();

    /**
     * main constructor
     * year: the year of this movie
     * id: the id of the movie
     * name: the name of the movie
     */
    Movie(int year, int id, string& name);

    /**
     * Get the id of the movie
     */
    int getId();

    /**
     * Get the name of the movie
     */
    string getName();

    /**
     * Adding an actor to this movie
     */
    void addActor(int actorId);

    /**
     * Get the year of the movie
     */
    int getYear();

    /**
     * Get the actors involved in this movie
     */
    vector<int>& getActor();
};

#endif