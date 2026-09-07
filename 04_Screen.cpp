#include <vector>
#include <string>
using namespace std;
// depends on: Seat (02)

// ================= Screen =================
// ONE responsibility: one auditorium -- a screen number; owns its seats.
// Composition with Seat: if a Screen is destroyed, its Seats have no reason to exist independently.
class Screen {
private:                                   // Encapsulation
    int screenNumber;
    vector<Seat> seats;                    // Composition: seats are created for, and belong to, this screen

public:
    Screen(int screenNumber) : screenNumber(screenNumber) {}

    void addSeat(Seat seat) { seats.push_back(seat); }

    int getScreenNumber() const { return screenNumber; }
    vector<Seat> getSeats() const { return seats; }
};
