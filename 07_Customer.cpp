#include <string>
using namespace std;

// ================= Customer =================
// ONE responsibility: name and phone. Nothing about bookings or payments -- Customer only
// ASSOCIATES with BookingService to get things done; it does not own or drive the booking.
class Customer {
private:                                   // Encapsulation
    string name;
    string phone;

public:
    Customer(string name, string phone) : name(name), phone(phone) {}
    Customer() : Customer("Guest", "NA") {}   // Compile-time polymorphism: overloaded ctor, delegates via this()

    string getName() const { return name; }
    string getPhone() const { return phone; }
};
