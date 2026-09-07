#include <string>
using namespace std;
// depends on: Seat (02)

enum class SeatStatus { AVAILABLE, BOOKED };

// ================= ShowSeat =================
// ONE responsibility: the status of ONE seat FOR ONE show.
// WHY this class exists and not just Seat: physical seat A1 exists once on the screen,
// but its status is different for every show (booked for 6 PM, free for 9 PM).
// Status belongs to the Show, not to the physical chair.
// Must NOT change status except through book()/release() -- no direct field access from outside.
class ShowSeat {
private:                                   // Encapsulation
    Seat seat;                             // this show's own copy of the seat's number/type
    SeatStatus status;

public:
    ShowSeat(Seat seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    string getSeatNumber() const { return seat.getSeatNumber(); }
    SeatType getType() const { return seat.getType(); }
    SeatStatus getStatus() const { return status; }
    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    void book()    { status = SeatStatus::BOOKED; }      // called by BookingService only
    void release() { status = SeatStatus::AVAILABLE; }   // used for cancellation & failed-payment rollback
};
