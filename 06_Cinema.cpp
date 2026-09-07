#include <vector>
#include <string>
using namespace std;
// depends on: Movie (01), Screen (04), Show (05)

// ================= Cinema =================
// ONE responsibility: the theatre -- its name; owns its screens. For this single-cinema
// system it is also the natural home for the movie catalog and the scheduled shows.
// Composition with Screen/Show: destroy the cinema and its screens/shows go with it.
// Must NOT contain booking, pricing or payment logic -- it is a container, not a service.
class Cinema {
private:                                   // Encapsulation
    string name;
    vector<Screen> screens;                // Composition
    vector<Movie> movies;
    vector<Show> shows;                    // Composition

public:
    Cinema(string name) : name(name) {
        // reserve capacity up-front: Show stores Movie*/Screen* pointers into these vectors,
        // so we avoid pointer invalidation from reallocation when more entries are added.
        movies.reserve(10);
        screens.reserve(10);
        shows.reserve(20);
    }

    string getName() const { return name; }

    void addMovie(Movie movie) { movies.push_back(movie); }
    void addScreen(Screen screen) { screens.push_back(screen); }
    void addShow(Show show) { shows.push_back(show); }

    vector<Movie>& getMovies() { return movies; }
    vector<Screen>& getScreens() { return screens; }
    vector<Show>& getShows() { return shows; }

    // shows scheduled for a given movie (by title, since Movie has no id field)
    vector<Show*> showsForMovie(Movie* movie) {
        vector<Show*> result;
        for (auto& s : shows) if (s.getMovie() == movie) result.push_back(&s);
        return result;
    }

    Show* findShowById(int id) {
        for (auto& s : shows) if (s.getShowId() == id) return &s;
        return nullptr;
    }
};
