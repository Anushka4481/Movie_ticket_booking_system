#include <iostream>
#include <vector>
#include <string>
using namespace std;
// depends on: Booking (10), Show (05), Movie (01), Screen (04), ShowSeat (03)

// ================= TicketPrinter =================
// ONE responsibility: format and print a ticket -- PRINTING ONLY.
// Must NOT confirm, cancel, or otherwise change a booking's state; a function named
// printTicket() should have no side effects on the objects it reads (clean-code rule).
class TicketPrinter {
public:
    static void printTicket(Booking* booking) {
        Show* show = booking->getShow();
        vector<ShowSeat*> seats = booking->getSeats();

        cout << "\n  ================ TICKET ================\n";
        cout << "    Booking ID : BK" << booking->getBookingId() << "\n";
        cout << "    Movie      : " << show->getMovie()->getTitle() << "\n";
        cout << "    Screen     : Screen-" << show->getScreen()->getScreenNumber()
             << "   " << show->getTime() << "\n";
        cout << "    Seats      : ";
        for (size_t i = 0; i < seats.size(); i++) {
            cout << seats[i]->getSeatNumber();
            if (i + 1 < seats.size()) cout << ", ";
        }
        cout << "\n    Amount     : Rs." << booking->getTotalAmount()
             << "        Status: " << bookingStatusToString(booking->getStatus()) << "\n";
        cout << "  =========================================\n";
    }
};
