#include <vector>
using namespace std;
// depends on: ShowSeat (03), SeatType (02)

// Constants, not magic numbers -- one source of truth for prices.
const double SILVER_PRICE   = 150.0;
const double GOLD_PRICE     = 250.0;
const double PLATINUM_PRICE = 400.0;

// ================= PriceCalculator =================
// ONE responsibility: turn a list of seats into a total amount.
// Pure calculation only -- no console I/O, no state changes, no side effects.
class PriceCalculator {
public:
    static double priceOf(SeatType type) {
        switch (type) {
            case SeatType::SILVER:   return SILVER_PRICE;
            case SeatType::GOLD:     return GOLD_PRICE;
            case SeatType::PLATINUM: return PLATINUM_PRICE;
        }
        return 0.0;
    }

    static double calculateTotal(const vector<ShowSeat*>& seats) {
        double total = 0.0;
        for (ShowSeat* s : seats) total += priceOf(s->getType());
        return total;
    }
};
