#include <vector>
#include <string>
using namespace std;
// depends on: Movie (01), Seat (02), ShowSeat (03), Screen (04)

// ================= Show =================
// ONE responsibility: one screening = a Movie on a Screen at a time; owns its ShowSeats.
// Aggregation with Movie/Screen: a Show borrows them (they outlive/pre-exist the Show and
// are not deleted when the Show is). Composition with ShowSeat: the per-show seat states
// are created here and die with this Show.
// Must NOT print to the console (that is BookingService/TicketPrinter's job) and must NOT
// mutate seat status directly from outside its own ShowSeat objects.
class Show {
private:                                   // Encapsulation
    static int nextShowId;                 // Static member: one shared id sequence for all shows

    int showId;
    Movie* movie;                          // Aggregation
    Screen* screen;                        // Aggregation
    string time;                           // e.g. "06:00 PM"
    vector<ShowSeat> showSeats;             // Composition

public:
    Show(Movie* movie, Screen* screen, string time)
        : movie(movie), screen(screen), time(time) {
        showId = nextShowId++;
        for (Seat s : screen->getSeats()) {
            showSeats.push_back(ShowSeat(s));   // «create» -- every seat starts AVAILABLE for this show
        }
    }

    int getShowId() const { return showId; }
    Movie* getMovie() const { return movie; }
    Screen* getScreen() const { return screen; }
    string getTime() const { return time; }
    vector<ShowSeat>& getShowSeats() { return showSeats; }   // reference so callers can book()/release()
};

int Show::nextShowId = 1;
