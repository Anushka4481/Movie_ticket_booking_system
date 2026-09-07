#include <string>
using namespace std;

// ================= Movie =================
// ONE responsibility: title, language, duration -- nothing else.
// Must NOT know about shows, screens, seats or pricing.
class Movie {
private:                                   // Encapsulation: fields hidden, only reachable via getters
    string title;
    string language;
    int durationMinutes;

public:
    Movie(string title, string language, int durationMinutes)
        : title(title), language(language), durationMinutes(durationMinutes) {}

    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDuration() const { return durationMinutes; }
};
