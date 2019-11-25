subdir('elements/Node')
subdir('elements/Movie')
subdir('elements/Edges')
subdir('elements')
subdir('ActorGraph')

#  Define autocomplete_exe to output executable file named 
#       autocomplete.cpp.executable

buildGraph_exe = executable('buildGraph.cpp.executable', 
    sources: ['buildGraph.cpp'],
    dependencies : [actorgraph_dep, elements_dep, movie_dep, edges_dep, node_dep],
    install : true)