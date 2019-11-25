#include <ActorGraph.hpp>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    ActorGraph* ag = new ActorGraph();
    ag->loadFromFile("/Code/cse100_pa4/data/imdb_2019.tsv", false);
    delete ag;
}