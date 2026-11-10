#include <cstddef>
#include <ostream>
#include <stdexcept>

#ifndef TRAINS_H
#define TRAINS_H

enum class WagonType {
    FIRST_CLASS,
    SECOND_CLASS,
    SLEEPER,
    DINING,
};

// For printing a wagon type onto an output stream.
std::ostream& operator<<(std::ostream& os, const WagonType& type) {
    if (type == WagonType::FIRST_CLASS) {
        os << "1 klasa";
    } else if (type == WagonType::SECOND_CLASS) {
        os << "2 klasa";
    } else if (type == WagonType::SLEEPER) {
        os << "sypialny";
    } else if (type == WagonType::DINING) {
        os << "restauracyjny";
    }
    return os;
}

namespace trains_details {

const size_t FIRST_CLASS_SEATS = 54;
const size_t SECOND_CLASS_SEATS = 80;
const size_t SLEEPER_SEATS = 30;
const size_t DINING_SEATS = 32;

constexpr size_t seats_from_type(WagonType type) {
    if (type == WagonType::FIRST_CLASS) {
        return FIRST_CLASS_SEATS;
    } else if (type == WagonType::SECOND_CLASS) {
        return SECOND_CLASS_SEATS;
    } else if (type == WagonType::SLEEPER) {
        return SLEEPER_SEATS;
    } else if (type == WagonType::DINING) {
        return DINING_SEATS;
    }
    throw std::runtime_error("Unhandled wagon type.");
}

} // namespace

class WagonSeries {
public:
    constexpr size_t seats() const {
        return seats_;
    }

    constexpr WagonType type() const {
        return type_;
    }

    constexpr WagonSeries(WagonType type)
        : seats_(trains_details::seats_from_type(type)),
          type_(type) {}

    constexpr WagonSeries(WagonType type, size_t seats)
        : seats_(seats),
          type_(type) {}

    constexpr bool operator==(const WagonSeries& other) const {
        return type_ == other.type_ && seats_ == other.seats_;
    }

private:
    size_t seats_;
    WagonType type_;
};

// For printing a wagon series onto an output stream.
std::ostream& operator<<(std::ostream& os, const WagonSeries& series) {
    os << "[" << series.type() << ", miejsca: " << series.seats() << "]";
    return os;
}

inline constexpr WagonSeries STANDARD_FIRST_CLASS(WagonType::FIRST_CLASS);
inline constexpr WagonSeries STANDARD_SECOND_CLASS(WagonType::SECOND_CLASS);
inline constexpr WagonSeries STANDARD_SLEEPER(WagonType::SLEEPER);
inline constexpr WagonSeries STANDARD_DINING(WagonType::DINING);

class Wagon {
    using unique_id_t = size_t;

public:
    static size_t ever_built() {
        return last_number_;
    }

    static size_t in_service() {
        return in_service_;
    }

    WagonSeries series() const {
        return series_;
    }

    unique_id_t number() const {
        return number_;
    }

    Wagon()
        : series_(STANDARD_FIRST_CLASS),
          number_(0) {}

    ~Wagon() {
        if (*this) {
            in_service_--;
        }
    }

    Wagon(WagonSeries series)
        : series_(series),
          number_(build_new()) {}

    Wagon(const Wagon &other)
        : series_(other ? other.series_ : STANDARD_FIRST_CLASS),
          number_(other ? build_new()   : 0) {}

    Wagon(Wagon&& other)
        : series_(other.series_),
          number_(other.number_) {

        other.make_empty();
    }

    void operator=(const Wagon& other) {
        if (other) {
            if (!*this) {
                number_ = build_new();
            }
            series_ = other.series_;
        } else {
            make_empty();
        }
    }

    void operator=(Wagon&& other) {
        // Move only if rhs != rhs
        if (*this != other) {
            number_ = other.number_;
            series_ = other.series_;

            other.make_empty();
        }
    }

    std::strong_ordering operator<=>(const Wagon& other) const {
        if (number_ < other.number_) {
            return std::strong_ordering::less;
        } else if (number_ > other.number_) {
            return std::strong_ordering::greater;
        } else {
            return std::strong_ordering::equal;
        }
    }

    bool operator==(const Wagon& other) const {
        return number_ == other.number_;
    }

    operator WagonSeries() const {
        return series_;
    }

    // Returns true if not empty
    explicit operator bool() const {
        return number_ != 0;
    }

private:
    inline static unique_id_t last_number_ = 0;
    inline static size_t in_service_ = 0;

    WagonSeries series_;
    unique_id_t number_;

    // Increases the amount of wagons in service.
    // Returns an id for the newly built wagon.
    unique_id_t build_new() {
        in_service_++;
        return ++last_number_;
    }

    // Makes this wagon empty and decreases the number of wagons in service.
    // Does nothing if the wagon is already empty.
    void make_empty() {
        if (*this) {
            number_ = 0;
            series_ = STANDARD_FIRST_CLASS;
            in_service_--;
        }
    }
};

// For printing a wagon onto an output stream.
std::ostream& operator<<(std::ostream& os, const Wagon& wagon) {
    if (wagon) {
        os << "Wagon nr " << wagon.number() << " " << wagon.series();
    } else {
        os << "Wagon pusty";
    }
    return os;
}

class Locomotive {
public:
    std::string designation() const {
        return designation_;
    }

    size_t max_wagons() const {
        return max_wagons_;
    }

    Locomotive()
        : designation_(""),
          max_wagons_(0) {}

    Locomotive(std::string designation, size_t max_wagons)
        : designation_(designation.empty() ? "?" : designation),
          max_wagons_(max_wagons) {}

    Locomotive(Locomotive&& other)
        : designation_(other.designation_),
          max_wagons_(other.max_wagons_) {

        other.make_empty();
    }

    void operator=(Locomotive&& other) {
        // Move only if lhs != rhs
        if (this != &other) {
            designation_ = other.designation_;
            max_wagons_ = other.max_wagons_;

            other.make_empty();
        }
    }

    // Returns true if not empty
    operator bool() {
        return designation_.empty();
    }

private:
    std::string designation_;
    size_t max_wagons_;

    // Makes this locomotive empty.
    void make_empty() {
        designation_ = "";
        max_wagons_ = 0;
    }
};

// For printing a locomotive onto an output stream.
std::ostream& operator<<(std::ostream& os, const Locomotive& locomotive) {
    os << locomotive.designation() << " (uciąg: " << locomotive.max_wagons() << ")";
    return os;
}

#endif
