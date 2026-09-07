#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
// depends on: Payment (08)

const int SIMULATED_SUCCESS_RATE_PERCENT = 90;   // no magic numbers scattered around

// Inheritance: UpiPayment / CardPayment / CashPayment each extend Payment.
// Runtime polymorphism: calling pay() through a Payment* / Payment& invokes the
// override that matches the object's ACTUAL type, decided at run time, not compile time.

// ================= UpiPayment =================
class UpiPayment : public Payment {
public:
    bool pay(double amount) override {
        cout << "  Processing UPI payment of Rs." << amount << "...\n";
        bool success = (rand() % 100) < SIMULATED_SUCCESS_RATE_PERCENT;
        if (success) cout << "  [UPI] Rs." << amount << " paid successfully\n";
        else         cout << "  [UPI] Payment FAILED - please retry\n";
        return success;
    }
    string methodName() const override { return "UPI"; }
};

// ================= CardPayment =================
class CardPayment : public Payment {
public:
    bool pay(double amount) override {
        cout << "  Processing Card payment of Rs." << amount << "...\n";
        bool success = (rand() % 100) < SIMULATED_SUCCESS_RATE_PERCENT;
        if (success) cout << "  [Card] Rs." << amount << " paid successfully\n";
        else         cout << "  [Card] Payment FAILED - please retry\n";
        return success;
    }
    string methodName() const override { return "Card"; }
};

// ================= CashPayment =================
// Cash paid in person at the counter -- modeled here as always succeeding.
class CashPayment : public Payment {
public:
    bool pay(double amount) override {
        cout << "  Processing Cash payment of Rs." << amount << "...\n";
        cout << "  [Cash] Rs." << amount << " paid successfully\n";
        return true;
    }
    string methodName() const override { return "Cash"; }
};
