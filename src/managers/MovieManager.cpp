#include "managers/MovieManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"

void MovieManager::load() {
    movies.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("movies.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        movies.push_back(Movie::fromString(lines[i]));
    }
}

void MovieManager::save() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < movies.size(); ++i) {
        lines.push_back(movies[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("movies.txt"), lines);
}

bool MovieManager::addMovie(const Movie& movie) {
    DynamicArray<std::string> existingIDs;
    for (int i = 0; i < movies.size(); ++i) {
        existingIDs.push_back(movies[i].getMaPhim());
    }
    Movie m = movie;
    m.setMaPhim(IDGenerator::generateID("MV", existingIDs));
    movies.push_back(m);
    save();
    return true;
}

bool MovieManager::updateMovie(const std::string& maPhim, const Movie& updated) {
    for (int i = 0; i < movies.size(); ++i) {
        if (movies[i].getMaPhim() == maPhim) {
            movies[i] = updated;
            movies[i].setMaPhim(maPhim);
            save();
            return true;
        }
    }
    return false;
}

bool MovieManager::deleteMovie(const std::string& maPhim) {
    for (int i = 0; i < movies.size(); ++i) {
        if (movies[i].getMaPhim() == maPhim) {
            movies[i].setTrangThai(false);
            save();
            return true;
        }
    }
    return false;
}

Movie* MovieManager::findById(const std::string& maPhim) {
    for (int i = 0; i < movies.size(); ++i) {
        if (movies[i].getMaPhim() == maPhim) {
            return &movies[i];
        }
    }
    return nullptr;
}

DynamicArray<Movie> MovieManager::getActiveMovies() const {
    DynamicArray<Movie> res;
    for (int i = 0; i < movies.size(); ++i) {
        if (movies[i].getTrangThai()) {
            res.push_back(movies[i]);
        }
    }
    return res;
}

DynamicArray<Movie> MovieManager::searchByName(const std::string& keyword) const {
    DynamicArray<Movie> res;
    for (int i = 0; i < movies.size(); ++i) {
        if (movies[i].getTrangThai() && movies[i].getTenPhim().find(keyword) != std::string::npos) {
            res.push_back(movies[i]);
        }
    }
    return res;
}

DynamicArray<Movie>& MovieManager::getAllMovies() {
    return movies;
}
