#include <string>
using namespace std;

// SeatType is a property of the physical seat (SILVER/GOLD/PLATINUM row).
// Actual prices live in one place only: PriceCalculator (avoids magic numbers / duplication).
enum class SeatType { SILVER, GOLD, PLATINUM };

string seatTypeToString(SeatType type) {
    switch (type) {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
    }
    return "UNKNOWN";
}

// ================= Seat =================
// ONE responsibility: one physical seat -- its number and type.
// Must NOT know whether it is booked (that belongs to ShowSeat, per-show).
class Seat {
private:                                   // Encapsulation
    string seatNumber;                     // e.g. "A1"
    SeatType type;

public:
    Seat(string seatNumber, SeatType type) : seatNumber(seatNumber), type(type) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};
