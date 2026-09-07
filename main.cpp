#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>
using namespace std;

// ---- Course rule: "one class per file, no header files". Since plain classes with
// no header can't be split across separately-compiled translation units without
// redeclaring them, this project stays modular by keeping each class in its own .cpp
// file and having main.cpp #include them in dependency order. Build with:
//     g++ main.cpp -o ticketbooking
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_ShowSeat.cpp"
#include "04_Screen.cpp"
#include "05_Show.cpp"
#include "06_Cinema.cpp"
#include "07_Customer.cpp"
#include "08_Payment.cpp"
#include "09_PaymentTypes.cpp"
#include "10_Booking.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"

// builds a Screen with rows A(SILVER x4), B(GOLD x3), C(PLATINUM x2) -- matches the demo layout
Screen makeStandardScreen(int screenNumber) {
    Screen screen(screenNumber);
    for (int i = 1; i <= 4; i++) screen.addSeat(Seat("A" + to_string(i), SeatType::SILVER));
    for (int i = 1; i <= 3; i++) screen.addSeat(Seat("B" + to_string(i), SeatType::GOLD));
    for (int i = 1; i <= 2; i++) screen.addSeat(Seat("C" + to_string(i), SeatType::PLATINUM));
    return screen;
}

Cinema setupCinema() {
    Cinema cinema("Downtown Cinema");

    cinema.addMovie(Movie("3 Idiots", "Hindi", 170));
    cinema.addMovie(Movie("Interstellar", "English", 169));

    cinema.addScreen(makeStandardScreen(1));
    cinema.addScreen(makeStandardScreen(2));

    vector<Movie>& movies = cinema.getMovies();
    vector<Screen>& screens = cinema.getScreens();

    cinema.addShow(Show(&movies[0], &screens[0], "06:00 PM"));   // 3 Idiots  @ Screen-1
    cinema.addShow(Show(&movies[0], &screens[1], "09:00 PM"));   // 3 Idiots  @ Screen-2
    cinema.addShow(Show(&movies[1], &screens[0], "09:30 PM"));   // Interstellar @ Screen-1

    return cinema;
}

// reads an integer choice safely; returns -1 on non-numeric input (edge case: no crash)
int readIntChoice() {
    string line;
    getline(cin, line);
    try {
        return stoi(line);
    } catch (...) {
        return -1;
    }
}

void handleBookFlow(BookingService& service) {
    service.listMovies();
    cout << "Choose movie: ";
    int movieChoice = readIntChoice();

    vector<Show*> shows = service.listShowsForMovie(movieChoice);
    if (shows.empty()) {
        cout << "  No shows available for that choice.\n";
        return;
    }

    cout << "Choose show: ";
    int showChoice = readIntChoice();
    if (showChoice < 1 || showChoice > (int)shows.size()) {
        cout << "  Invalid show choice.\n";
        return;
    }
    Show* show = shows[showChoice - 1];

    service.printSeatLayout(show);

    cout << "\nSeats (e.g. A1,B2): ";
    string seatInput;
    getline(cin, seatInput);
    vector<string> seatNumbers = service.parseSeatNumbers(seatInput);

    cout << "\nPay by: 1.UPI  2.Card  3.Cash > ";
    int payChoice = readIntChoice();
    if (payChoice < 1 || payChoice > 3) {
        cout << "  Invalid payment choice. Booking cancelled.\n";
        return;
    }

    service.bookAndPay(show, seatNumbers, payChoice);
}

void handleCancelFlow(BookingService& service) {
    cout << "Enter Booking ID to cancel (e.g. 1001): BK";
    int id = readIntChoice();
    if (id < 0) {
        cout << "  Invalid booking id.\n";
        return;
    }
    service.cancelBooking(id);
}

int main() {
    srand((unsigned)time(nullptr));   // seed for simulated payment success/failure

    Cinema cinema = setupCinema();
    BookingService service(cinema);

    cout << "===== MOVIE TICKET BOOKING =====\n";

    bool running = true;
    while (running) {
        cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
        int choice = readIntChoice();

        switch (choice) {
            case 1:
                service.listMovies();
                break;
            case 2:
                handleBookFlow(service);
                break;
            case 3:
                handleCancelFlow(service);
                break;
            case 4:
                service.listMyBookings();
                break;
            case 0:
                running = false;
                cout << "Goodbye!\n";
                break;
            default:
                cout << "  Invalid menu choice. Please try again.\n";   // edge case: no crash
        }
    }

    return 0;
}
