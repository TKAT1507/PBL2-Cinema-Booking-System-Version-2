#ifndef MOVIEMANAGER_H
#define MOVIEMANAGER_H

#include "core/DynamicArray.h"
#include "models/Movie.h"
#include <string>

class MovieManager {
private:
    DynamicArray<Movie> movies;
public:
    void load();
    void save();
    bool addMovie(const Movie& movie);
    bool updateMovie(const std::string& maPhim, const Movie& updated);
    bool deleteMovie(const std::string& maPhim);
    Movie* findById(const std::string& maPhim);
    DynamicArray<Movie> getActiveMovies() const;
    DynamicArray<Movie> searchByName(const std::string& keyword) const;
    DynamicArray<Movie>& getAllMovies();
};

#endif // MOVIEMANAGER_H
