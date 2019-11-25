ActorGraph = library('ActorGraph', sources : ['ActorGraph.hpp', 'ActorGraph.cpp'], dependencies: [elements_dep, movie_dep, edges_dep, node_dep])
inc = include_directories('.')

actorgraph_dep = declare_dependency(include_directories : inc,
  link_with : ActorGraph)