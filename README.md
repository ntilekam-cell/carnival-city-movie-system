# Carnival City Movie Event System (C++)

A menu-driven C++ console application designed to manage, analyze, and generate sorted financial and booking reports for Carnival City movie reservations.

🌐 **Developer Portfolio:** [ntilekam-cell.github.io/my-portfolio](https://github.io)

---

## 🚀 Features

1. **File Analytics:** Counts total lines, words, and specific character occurrences across the reservation dataset.
2. **Batch Reference Updates:** Validates and updates the first two digits of all client reference numbers.
3. **Category Filtering & Revenue Tracking:** Counts bookings and calculates total revenue (in ZAR `R`) for specific movie genres (Action, Horror, Animation, etc.) as well as overall grand totals.
4. **Sorted Report Generation:** Exports a cleanly formatted summary report to an external output file (`movie_report.txt`).

---

## 🛠️ Key C++ Concepts Demonstrated

* **Object-Oriented Design:** Encapsulates reservation attributes (`surname`, `name`, `movieTitle`, `movieType`, `seatNumber`, `price`, `refNumber`) in a `Client` class with constructors, getters, and setters.
* **CSV / Delimited File Parsing (`<fstream>` & `<sstream>`):** Parses comma-separated records line by line with whitespace trimming and `stod()` exception handling.
* **Interactive CLI Menu:** Input validation and loop-driven menu navigation (`switch-case` & `do-while`).

---

## 📂 Project Files

* `main.cpp` — Complete source code for the `Client` class, file parsers, analytics functions, and CLI menu.
* `clients_info.dat` — Sample comma-separated client reservation data.

---

## 💻 How to Compile & Run

```bash
g++ main.cpp -o movie_system
./movie_system
```
