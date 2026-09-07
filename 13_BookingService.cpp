#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
// depends on: Cinema (06), Show (05), ShowSeat (03), Booking (10), Payment (08/09),
//             PriceCalculator (11), TicketPrinter (12)

// ================= BookingService =================
// ONE responsibility: the orchestrator -- runs the booking flow end to end (F1-F8),
// coordinating the entity classes below it. Association with Cinema: BookingService
// uses it but does not own it.
//
// Open/Closed: adding NetBanking only means adding a new Payment subclass; this class
// (and Booking, Show, etc.) needs zero edits because it depends on Payment (the abstraction),
// never on a concrete payment type directly.
class BookingService {
private:
    Cinema& cinema;                        // Association (this=BookingService); doesn't own Cinema
    vector<Booking*> allBookings;          // bookings made in this session (for cancel / "my tickets")

    ShowSeat* findShowSeat(Show* show, string seatNumber) {
        for (ShowSeat& s : show->getShowSeats()) {
            if (s.getSeatNumber() == seatNumber) return &s;
        }
        return nullptr;
    }

    void printSeatRow(Show* show, SeatType type, string label) {
        cout << "  " << label << "\t";
        for (ShowSeat& s : show->getShowSeats()) {
            if (s.getType() == type) {
                cout << s.getSeatNumber() << (s.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        cout << "\n";
    }

public:
    BookingService(Cinema& cinema) : cinema(cinema) {}

    // ---------- F1 ----------
    void listMovies() {
        vector<Movie>& movies = cinema.getMovies();
        cout << "\n";
        for (size_t i = 0; i < movies.size(); i++) {
            cout << "  [" << (i + 1) << "] " << movies[i].getTitle()
                 << "\t" << movies[i].getLanguage()
                 << "\t" << movies[i].getDuration() << " min\n";
        }
    }

    // ---------- F2 ----------
    vector<Show*> listShowsForMovie(int movieIndex1Based) {
        vector<Movie>& movies = cinema.getMovies();
        if (movieIndex1Based < 1 || movieIndex1Based > (int)movies.size()) {
            cout << "  Invalid movie choice.\n";
            return {};
        }
        Movie* movie = &movies[movieIndex1Based - 1];
        vector<Show*> shows = cinema.showsForMovie(movie);
        for (size_t i = 0; i < shows.size(); i++) {
            cout << "  [" << (i + 1) << "] Screen-" << shows[i]->getScreen()->getScreenNumber()
                 << "\t" << shows[i]->getTime() << "\n";
        }
        return shows;
    }

    // ---------- F3 ----------
    void printSeatLayout(Show* show) {
        cout << "\n  SCREEN-" << show->getScreen()->getScreenNumber()
             << "  " << show->getTime() << "  |  " << show->getMovie()->getTitle() << "\n";
        printSeatRow(show, SeatType::SILVER, "SILVER");
        printSeatRow(show, SeatType::GOLD, "GOLD");
        printSeatRow(show, SeatType::PLATINUM, "PLATINUM");
        cout << "\n  ( [ ] = available   [X] = booked )\n";
    }

    // splits "A1,B2" into {"A1","B2"}
    vector<string> parseSeatNumbers(string input) {
        vector<string> result;
        stringstream ss(input);
        string token;
        while (getline(ss, token, ',')) {
            // trim whitespace
            while (!token.empty() && token.front() == ' ') token.erase(token.begin());
            while (!token.empty() && token.back() == ' ') token.pop_back();
            if (!token.empty()) result.push_back(token);
        }
        return result;
    }

    // ---------- F4 + F5 + F6 + F7, orchestrated together ----------
    // FR4: if ANY selected seat is invalid or already booked, whole booking is rejected,
    //      no seat changes state.
    // FR6: booking is confirmed only AFTER payment succeeds; a failed payment releases
    //      the seats and marks the booking FAILED.
    Booking* bookAndPay(Show* show, vector<string> seatNumbers, int paymentChoice) {
        if (seatNumbers.empty()) {
            cout << "  No seats entered. Booking rejected.\n";
            return nullptr;
        }

        // Validate everything FIRST -- nothing is booked until we know the whole request is valid.
        vector<ShowSeat*> chosen;
        for (string num : seatNumbers) {
            ShowSeat* found = findShowSeat(show, num);
            if (found == nullptr) {
                cout << "  Invalid seat number: " << num << ". Booking rejected, no seat changes state.\n";
                return nullptr;
            }
            if (!found->isAvailable()) {
                cout << "  Seat " << num << " is already BOOKED. Booking rejected, no seat changes state.\n";
                return nullptr;
            }
            chosen.push_back(found);
        }

        // F5: price the booking
        double total = PriceCalculator::calculateTotal(chosen);
        cout << "\n";
        for (ShowSeat* s : chosen) {
            cout << "  " << s->getSeatNumber() << " " << seatTypeToString(s->getType())
                 << " Rs." << PriceCalculator::priceOf(s->getType()) << "\n";
        }
        cout << "  TOTAL          Rs." << total << "\n";

        // Reserve seats tentatively; rolled back below if payment fails (FR6).
        for (ShowSeat* s : chosen) s->book();

        Booking* booking = new Booking(show, chosen, total);   // «create»
        allBookings.push_back(booking);

        // F6: pay by UPI, Card or Cash
        Payment* payment;
        if (paymentChoice == 1) payment = new UpiPayment();        // «create»
        else if (paymentChoice == 2) payment = new CardPayment();  // «create»
        else payment = new CashPayment();                          // «create»

        bool success = payment->pay(total);   // runtime polymorphism: exact override picked at runtime

        if (success) {
            booking->confirm();
            TicketPrinter::printTicket(booking);   // F7
        } else {
            for (ShowSeat* s : chosen) s->release();   // FR6: release seats on failure
            booking->fail();
            cout << "  Booking NOT confirmed. Seats released back to AVAILABLE.\n";
        }

        delete payment;
        return booking;
    }

    // ---------- F8 ----------
    bool cancelBooking(int bookingId) {
        for (Booking* b : allBookings) {
            if (b->getBookingId() == bookingId) {
                if (b->getStatus() != BookingStatus::CONFIRMED) {
                    cout << "  Booking BK" << bookingId << " is not in CONFIRMED state, cannot cancel.\n";
                    return false;
                }
                for (ShowSeat* s : b->getSeats()) s->release();
                b->cancel();
                cout << "  Booking BK" << bookingId << " cancelled. Seats are now AVAILABLE again.\n";
                return true;
            }
        }
        cout << "  No booking found with ID BK" << bookingId << ".\n";
        return false;
    }

    // "My tickets"
    void listMyBookings() {
        bool any = false;
        for (Booking* b : allBookings) {
            if (b->getStatus() == BookingStatus::CONFIRMED) {
                any = true;
                cout << "  BK" << b->getBookingId() << "  " << b->getShow()->getMovie()->getTitle()
                     << "  " << b->getShow()->getTime() << "  Rs." << b->getTotalAmount()
                     << "  " << bookingStatusToString(b->getStatus()) << "\n";
            }
        }
        if (!any) cout << "  No confirmed bookings yet.\n";
    }
};
