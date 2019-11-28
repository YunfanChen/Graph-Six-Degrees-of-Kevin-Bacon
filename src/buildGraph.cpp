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
    ag.loadFromFile("/Code/cse100_pa4/data/imdb_middle_sample.tsv", false);
}