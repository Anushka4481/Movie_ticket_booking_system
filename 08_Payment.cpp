#include <string>
using namespace std;

// ================= Payment (abstract) =================
// Abstraction: defines the payment CONTRACT only -- pay(amount) and methodName().
// No concrete "how money moves" logic belongs here; that is each subclass's job.
// Liskov substitution: every Payment child must work correctly through a Payment*
// with no extra setup call needed beyond construction.
class Payment {
public:
    virtual bool pay(double amount) = 0;      // pure virtual -> makes this class abstract
    virtual string methodName() const = 0;
    virtual ~Payment() {}                     // virtual destructor: safe delete through base pointer
};
