#include <vector>
#include <string>
using namespace std;
// depends on: Show (05), ShowSeat (03), Customer (07)

enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::FAILED:    return "FAILED";
        case BookingStatus::CANCELLED: return "CANCELLED";
        default:                       return "PENDING";
    }
}

// ================= Booking =================
// ONE responsibility: booking id, which show, which seats, which customer, total amount, status.
// Must NOT calculate price (PriceCalculator's job) and must NOT print tickets (TicketPrinter's job).
// Aggregation with Show/ShowSeat/Customer: Booking refers to them, does not own their lifetime.
class Booking {
private:                                   // Encapsulation
    static int nextBookingId;              // Static member: one shared id sequence for all bookings

    int bookingId;
    Show* show;                            // Aggregation
    vector<ShowSeat*> seats;               // Aggregation
    Customer customer;
    double totalAmount;
    BookingStatus status;

public:
    Booking(Show* show, vector<ShowSeat*> seats, double totalAmount, Customer customer = Customer())
        : show(show), seats(seats), customer(customer),
          totalAmount(totalAmount), status(BookingStatus::PENDING) {
        bookingId = nextBookingId++;
    }

    int getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    vector<ShowSeat*> getSeats() const { return seats; }
    Customer getCustomer() const { return customer; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    void confirm() { status = BookingStatus::CONFIRMED; }   // called only by BookingService
    void fail()    { status = BookingStatus::FAILED; }
    void cancel()  { status = BookingStatus::CANCELLED; }
};

int Booking::nextBookingId = 1001;   // -> BK1001, BK1002, ...
