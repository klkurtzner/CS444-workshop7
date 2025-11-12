/*
 Name: Katy Kurtzner
 Email:kkurtzner@crimson.ua.edu
 Course Section: Fall 2024 CS 201
 Homework #: 3
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;
//create the maps 
unordered_map<string, vector<string>> movieActor;
unordered_map<string, vector<string>> actorMovies;

string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}//make lowercase to prevent errors
//parse through the file
void parseFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string movie, actor;
        getline(ss, movie, '/');

        if (movie.empty()) {
            continue;
        }

        while (getline(ss, actor, '/')) {
            movie.erase(remove_if(movie.begin(), movie.end(), ::isspace), movie.end());
            actor.erase(remove_if(actor.begin(), actor.end(), ::isspace), actor.end());

            if (actor.empty()) {
                continue;
            }

            movieActor[toLowerCase(movie)].push_back(actor);
            actorMovies[toLowerCase(actor)].push_back(movie);
        }
    }
    infile.close();
}
//search for actors by the movie name
void searchMovie(const string& movie) {
    auto it = movieActor.find(toLowerCase(movie));
    if (it != movieActor.end()) {
        for (const string& actor : it->second) {
            cout << actor << endl;
        }
    } else {
        cout << "Not Found" << endl;
    }
}
//search for movies by the actor name
void searchActor(const string& actor) {
    auto it = actorMovies.find(toLowerCase(actor));
    if (it != actorMovies.end()) {
        for (const string& movie : it->second) {
            cout << movie << endl;
        }
    } else {
        cout << "Not Found" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <db_file> <query_file>" << endl;
        return 1;
    }

    string dbFilename = argv[1];
    string queryFilename = argv[2];

    parseFile(dbFilename);

    ifstream queryFile(queryFilename);
    if (!queryFile) {
        cerr << "Error opening query file: " << queryFilename << endl;
        return 1;
    }
//give the answer to the inputs
    string query;
    while (getline(queryFile, query)) {
        if (movieActor.find(toLowerCase(query)) != movieActor.end()) {
            cout << "\nActors in movie \"" << query << "\":" << endl;
            searchMovie(query);
        }
        else if (actorMovies.find(toLowerCase(query)) != actorMovies.end()) {
            cout << "\nMovies with actor \"" << query << "\":" << endl;
            searchActor(query);
        }
        else {
            cout << "Not Found" << endl;
        }
    }
    queryFile.close();
    return 0;
}