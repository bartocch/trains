#ifndef TRAINS_H
#define TRAINS_H

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <queue>
#include <initializer_list>
#include <string>
#include <utility>
#include <algorithm>
#include <compare>

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
    } else {
        throw std::runtime_error("Unhandled wagon type.");
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
public:
    using unique_id_t = size_t;

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

    Wagon& operator=(const Wagon& other) {
        if (other) {
            if (!*this) {
                number_ = build_new();
            }
            series_ = other.series_;
        } else {
            if (*this) {
                make_empty();
                in_service_--;
            }
        }

        return *this;
    }

    Wagon& operator=(Wagon&& other) {
        // Move only if rhs != rhs
        if (*this != other) {
            if (*this) {
                in_service_--;  // Since this wagon is getting replaced by another one
            }
            number_ = other.number_;
            series_ = other.series_;

            other.make_empty();
        }

        return *this;
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

    void make_empty() {
        number_ = 0;
        series_ = STANDARD_FIRST_CLASS;
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

    Locomotive& operator=(Locomotive&& other) {
        // Move only if lhs != rhs
        if (this != &other) {
            designation_ = other.designation_;
            max_wagons_ = other.max_wagons_;

            other.make_empty();
        }

        return *this;
    }

    // Returns true if not empty
    explicit operator bool() const {
        return !designation_.empty();
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
    if (locomotive) {
        os << locomotive.designation() << " (uciąg: " << locomotive.max_wagons() << ")";
    } else {
        os << "brak lokomotywy";
    }
    return os;
}

class Train {
public:
	Train()
        : seats_(0),
          locomotive_(),
          wagons_() {}

	explicit Train(Locomotive locomotive)
        : seats_(0),
          locomotive_(std::move(locomotive)) {

		wagons_.reserve(locomotive_.max_wagons());
	}

	Train(Locomotive locomotive, std::initializer_list<Wagon> wagons)
        : seats_(0),
          locomotive_(std::move(locomotive)) {

		size_t capacity = locomotive_.max_wagons();
		wagons_.reserve(capacity);

		for (const Wagon& wagon:wagons){
			if (capacity == 0) break;

			if (wagon) {
				seats_ += wagon.series().seats();
				capacity--;
				wagons_.push_back(wagon);
			}
		}
	}

	Train(Train&& other)
        : seats_(other.seats_),
          locomotive_(std::move(other.locomotive_)),
          wagons_(std::move(other.wagons_)) {

		other.make_empty();
	}

	Train& operator=(Train&& other) {
		// Move only if lhs != rhs
		if (this != &other) {
			seats_ = other.seats_;
			locomotive_ = std::move(other.locomotive_);
			wagons_ = std::move(other.wagons_);

			other.make_empty();
		}
		return *this;
	}

	Train& operator+=(const Wagon& wagon) {
		if (!wagon) return *this;
		if (wagons_.size() < locomotive_.max_wagons()) {
			seats_ += wagon.series().seats();
			wagons_.push_back(wagon);
		}
		return *this;
	}

	Train& operator+=(Wagon&& wagon) {
		if (!wagon) return *this;
		if (wagons_.size() < locomotive_.max_wagons()) {
			seats_ += wagon.series().seats();
			wagons_.push_back(std::move(wagon));
		}
		return *this;
	}

	const Wagon& operator[](size_t index) const {
		return wagons_[index];
	}

	size_t size(){
		return wagons_.size();
	}

	size_t capacity(){
		return locomotive_.max_wagons();
	}

	size_t seats(){
		return seats_;
	}

	const Locomotive& locomotive() const {
		return locomotive_;
	}

	// Returns true if a locomotive is present
	explicit operator bool() const {
		if (locomotive_) return true;
		return false;
	}

	// For printing a train onto an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Train& train) {
		os << "Skład: " << train.locomotive_;
		for (const Wagon& wagon : train.wagons_) {
			os << "\n\t" << wagon;
		}
		return os;
	}

private:
    // Makes this train empty.
    void make_empty() {
		seats_ = 0;
		wagons_.clear();
    }

	size_t seats_;
	Locomotive locomotive_;
	std::vector<Wagon> wagons_;
};

class Depot {
public:
	Depot() = default;

	Depot(const Depot& other) = delete;

	Depot& operator=(const Depot& other) = delete;

	Depot(Depot&& other) = default;

	Depot& operator=(Depot&& other) = default;

	Depot& operator+=(Wagon&& wagon) {
		if (!wagon) return *this;

		wagons_[wagon.series()].push(std::move(wagon));
		wagon_counter_++;

		return *this;
	}

	Depot& operator+=(Locomotive&& locomotive) {
		if (!locomotive) return *this;

		locomotives_.push(std::move(locomotive));
		locomotive_counter_++;

		return *this;
	}

	Locomotive release_locomotive(){
		if(locomotives_.empty()) return Locomotive();

		Locomotive locomotive = std::move(locomotives_.front());
		locomotives_.pop();
		locomotive_counter_--;

		return locomotive;
	}

	Wagon release_wagon(WagonSeries series) {
		auto it = wagons_.find(series);

		if (it == wagons_.end() || it->second.empty())
			return Wagon();

		Wagon wagon = std::move(it->second.front());

		it->second.pop();
		wagon_counter_--;

        return wagon;
	}

	size_t locomotives() {
        return locomotive_counter_;
    }

	size_t wagons() {
        return wagon_counter_;
    }

	size_t wagons(WagonSeries series) {
		auto it = wagons_.find(series);

		if (it == wagons_.end() || it->second.empty())
			return 0;
		return it->second.size();
	}

private:
	// comparator, nescessary to use map with WagonSeries
	// the specific order is not important
	struct WagonSeriesComparator {
        bool operator()(const WagonSeries& a, const WagonSeries& b) const {
            if (a.type() != b.type())
                return a.type() < b.type();

            return a.seats() < b.seats();
        }
    };

    std::map<WagonSeries, std::queue<Wagon>, WagonSeriesComparator> wagons_;
	std::queue<Locomotive> locomotives_;
	size_t locomotive_counter_ = 0;
	size_t wagon_counter_ = 0;
};

Depot& main_depot() {
    static Depot depot = [] {
		Depot result;

		depot += Locomotive("EU07-001", 8);
		depot += Locomotive("EP09-020", 12);

		depot += Wagon(STANDARD_FIRST_CLASS);

		depot += Wagon(STANDARD_SECOND_CLASS);
		depot += Wagon(STANDARD_SECOND_CLASS);
		depot += Wagon(STANDARD_SECOND_CLASS);
		depot += Wagon(STANDARD_SECOND_CLASS);

		depot += Wagon(STANDARD_SLEEPER);
		depot += Wagon(STANDARD_SLEEPER);

		depot += Wagon(STANDARD_DINING);
		return result;
	}();
	return depot;
}

class Dispatcher{
public:
	Dispatcher()
        : depot_(main_depot()) {}

	Dispatcher(Depot& other)
        : depot_(other) {}

	Dispatcher(const Dispatcher& other)
        : depot_(other.depot_) {}

	Dispatcher& operator=(const Dispatcher& other) = delete;

	Dispatcher(Dispatcher&& other)
        : depot_(other.depot_) {};

	Dispatcher& operator=(Dispatcher&& other) = delete;

	const Depot& yard(){
		return depot_;
	}

	Train assemble(WagonSeries series, size_t count){
		Locomotive locomotive = depot_.release_locomotive();
		if (!locomotive) return Train();

		Train train(std::move(locomotive));

		for (size_t i = 0; i < std::min(train.locomotive().max_wagons(), count); i++) {
			Wagon wagon = depot_.release_wagon(series);
			if (!wagon) break;
			train += std::move(wagon);
		}
		return train;
	}

private:
	Depot& depot_;
};

#endif
