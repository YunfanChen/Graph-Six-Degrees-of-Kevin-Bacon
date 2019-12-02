/*
 * buildGraph.cpp
 * Author: Yunfan Chen
 * Date:   2019.11.27
 *
 * This file means to test building a graph according to the input file.
 */
#include <ActorGraph.hpp>
#include <iostream>

using namespace std;

/**
 * Main method.
 */
int main(int argc, char* argv[]) {
    ActorGraph ag;
    ag.loadFromFile("/Code/cse100_pa4/data/imdb_middle_sample.tsv", true);
    map<int, Edges>& edges = ag.getEdges()[907];
    vector<Movie>& movies = ag.getMovies();
    for (int i = 0; i < edges.size(); i++) {
        vector<int>& shared_movies = edges[i].getShared_movie();
        for (int j = 0; j < shared_movies.size(); j++) {
            Movie curMovie = movies[shared_movies[j]];
            cout << "From 0 to " << i << "'s movie:" << curMovie.getName()
                 << "'s weight is: " << curMovie.getWeight() << endl;
        }
    }
}