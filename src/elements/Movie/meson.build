Movie = library('Movie', sources : ['Movie.hpp', 'Movie.cpp'])
inc = include_directories('.')

movie_dep = declare_dependency(include_directories : inc,
  link_with : Movie)
