#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

struct Day {
    int value;
    explicit Day(int other=1) {
        if (!(1 <= other && other <= 31)) {
            throw std::invalid_argument("Day value is invalid: " + std::to_string(other));
        }
        value = other;
    }
};

struct Month {
    int value;
    explicit Month(int other=1) {
        if (!(1 <= other && other <= 12)) {
            throw std::invalid_argument("Month value is invalid: " + std::to_string(other));
        }
        value = other;
    }
};

struct Year {
    int value;
    explicit Year(int other=1970): value(other) {}
};


class Date {
public:
    Date(): year(Year()), month(Month()), day(Day()) {}

    Date(const Year& year_new, const Month& month_new, const Day& day_new) {
        year = year_new;
        month = month_new;
        day = day_new;
    }
    int GetYear() const { return year.value; }
    int GetMonth() const { return month.value; }
    int GetDay() const { return day.value; }
private:
    Year year;
    Month month;
    Day day;
};

bool operator<(const Date& lhs, const Date& rhs) {
    int lhs_year = lhs.GetYear();
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetYear() < rhs.GetYear();
}

std::istream& operator>>(std::istream& stream, Date& date) {
    std::string input_date;
    int year, month, day;
    bool correct_date = true;
    
    stream >> input_date;
    std::stringstream ss(input_date);
    
    ss >> year;
    correct_date &= ss.peek() == '-';
    ss.ignore(1);

    ss >> month;
    correct_date &= ss.peek() == '-';
    ss.ignore(1);

    ss >> day;
    correct_date &= bool(ss);
    correct_date &= ss.eof();

    if (!correct_date) {
        throw std::invalid_argument("Wrong date format: " + input_date);
    }
    auto y = Year{year};
    auto m = Month{month};
    auto d = Day{day};
    date = Date(y, m, d);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    stream << std::setw(4) << std::setfill('0') << date.GetYear() << '-';
    stream << std::setw(2) << std::setfill('0') << date.GetMonth() << '-';
    stream << std::setw(2) << std::setfill('0') << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const std::string& event) {
        data[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const std::string& event) {
        auto& tmp = data[date];
        if (tmp.find(event) == tmp.end()) {
            return false;
        }
        tmp.erase(event);
        return true;
    }
    int  DeleteDate(const Date& date) {
        int result = data[date].size();
        data.erase(date);
        return result;
    }  
    std::set<std::string> Find(const Date& date) const {
        if (data.find(date) != data.end()) {
            return data.at(date);
        }
        return std::set<std::string>();
    }

    void Print() const {
        for (const auto& [date, events] : data) {
            for (const auto& event : events) {
                std::cout << date << ' ' << event << '\n';
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> data;
};

int main() {
    Database db;
    
    std::string command, cmd, event;
    Date date;
    while (std::getline(std::cin, command)) {
        std::stringstream ss(command);
        if (ss >> cmd) {
            try {
                if (cmd == "Add") {
                    ss >> date >> event;
                    db.AddEvent(date, event);
                } else if (cmd == "Del") {
                    ss >> date >> event;
                    if (ss) {
                        if (db.DeleteEvent(date, event)) {
                            std::cout << "Deleted successfully\n";
                        } else {
                            std::cout << "Event not found\n";
                        }
                    } else {
                        std::cout << "Deleted " << db.DeleteDate(date) << " events\n";
                    }
                } else if (cmd == "Find") {
                    ss >> date;
                    auto events = db.Find(date);
                    for (const auto& e : events) {
                        std::cout << e << '\n';
                    }
                } else if (cmd == "Print") {
                    db.Print();
                } else {
                    std::cout << "Unknown command: " << cmd << '\n';
                }
            } catch (const std::exception& ex) {
                std::cout << ex.what() << '\n';
            }
            std::cout << std::flush;
        }
    }
}
