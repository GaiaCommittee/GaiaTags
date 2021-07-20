cmake_minimum_required(3.10)

find_path(GaiaTags_INCLUDE_DIRS "GaiaTags")
find_library(GaiaTags_LIBS "GaiaTags")

if (GaiaTags_INCLUDE_DIRS AND GaiaTags_LIBS)
    set(GaiaTags_FOUND TRUE)
endif()