## Requirement Analysis

### Functional Requirements

- **FR1** – Display movies: The system shall display all currently playing movies with their title, language and duration.
- **FR2** – Display show: The system shall display all available shows for a selected movie, including screen number and start time.
- **FR3** – Display seat layout: The system shall display the seat layout of a selected show and indicate whether each seat is available or booked.
- **FR4** – Book seats: The system shall allow a customer to select one or more available seats and reject seats that are already booked.
- **FR5** – Calculate ticket price: The system shall calculate the total ticket price according to seat type — silver = ₹150, gold = ₹250, platinum = ₹400.
- **FR6** – Make payment: The system shall allow payment using UPI, card, cash.
- **FR7** – Print ticket: The system shall print a ticket containing booking id, movie, screen, time, seat numbers and total amount.
- **FR8** – Cancel booking: The system shall allow a confirmed booking to be cancelled and make its seats available again.

### Non-Functional Requirements

- **NRF1** – Modularity: Each major class shall be implemented separately, with one class per file.
- **NRF2** – Extensibility: The payment system shall allow a new payment method such as NetBanking to be added without modifying `BookingService`.
- **NRF3** – Input validation: Invalid menu choices and invalid seat numbers shall be handled with clear error messages without crashing the program.
- **NRF4** – Maintainability: Each class and method shall have a single clear responsibility and avoid unnecessary duplication.

---

## Noun–Verb Analysis

| Noun | Class? | Reason |
|---|---|---|
| Movie | Yes | Has title, language and duration |
| Cinema | Yes | Represents the theatre |
| Screen | Yes | Represents one auditorium |
| Seat | Yes | Has seat number and type |
| Show | Yes | Represents one movie screening |
| ShowSeat | Yes | Stores seat status for a specific show |
| Customer | Yes | Stores customer details |
| Booking | Yes | Represents a reservation |
| Payment | Yes | Represents payment behaviour |
| Ticket | No | Ticket is output, handled by `TicketPrinter` |
| Seat Layout | No | It is only a display of `ShowSeat`s |

**Important verbs/methods:** `listMovies()`, `listShows()`, `displaySeats()`, `bookSeat()`, `cancelSeat()`, `calculate()`, `pay()`, `printTicket()`, `cancelBooking()`

---

## Class Responsibilities

| Class | Knows | Does | Must NOT do |
|---|---|---|---|
| Movie | Title, language and duration. | Provides movie information. | Must not handle booking or payment. |
| Seat | Seat number and type. | Provides seat information. | Must not know show-specific booking status. |
| Screen | Screen number and owns seats. | Provides seats and screen information. | Must not calculate prices. |
| Cinema | Cinema name and owns screens. | Provides screens. | Must not process payments. |
| Show | Movie, screen, time and ShowSeats. | Displays/finds seats for that show. | Must not process payment. |
| ShowSeat | A Seat and its status for one Show. | Books/cancels a seat and reports availability. | Must not calculate total booking price. |
| Customer | Name and phone. | Provides customer information. | Must not manage cinema inventory. |
| Booking | Booking ID, customer, show, seats, amount and status. | Stores booking state and cancellation. | Must not print tickets or perform payment. |
| UpiPayment / CardPayment / CashPayment | How their payment method works. | Perform payment. | Must not manage seats or tickets. |
| PriceCalculator | Seat-type prices. | Calculates total amount. | Must not book or print. |
| TicketPrinter | Ticket formatting. | Prints ticket. | Must not change booking state. |
| BookingService | Coordinates booking operations. | Runs booking flow and cancellation. | Must not own UI/menu input logic. |
| Main/Menu | Console interaction. | Reads input and invokes services. | Must not contain domain/business rules. |

---

## Relationship Table

| Pair | Relationship | Lifetime Justification |
|---|---|---|
| Cinema — Screen | Composition | A cinema owns its screens; when the cinema is destroyed, its screens are part of that cinema and no longer exist in this model. |
| Screen — Seat | Composition | Seats are physical parts of a screen/auditorium in this model; their lifetime is owned by the Screen. |
| Show — Movie | Aggregation | A Movie exists independently and can be associated with multiple Shows. |
| Show — Screen | Aggregation | A Screen exists independently and can host multiple Shows at different times. |
| Show — ShowSeat | Composition | ShowSeats are created for a particular show and represent its seat statuses. |
| Booking — Customer | Association | The Customer exists independently of a particular booking. |
| Booking — ShowSeat | Aggregation | The ShowSeat belongs to the show and can exist even after a booking is cancelled. |
| Booking — Payment | Association | Payment is a service interaction used to complete the booking; the payment implementation is independent. |
| Payment — UpiPayment | Inheritance | UpiPayment is a type of Payment. |
| Payment — CardPayment | Inheritance | CardPayment is a type of Payment. |
| Payment — CashPayment | Inheritance | CashPayment is a type of Payment. |
| BookingService — Booking | Association | BookingService manages/creates bookings but bookings represent independent domain records. |
