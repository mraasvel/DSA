#pragma once

template <typename T>
class Value {
public:
	Value() {}

	Value(const T& x)
	: value(x) {}

	Value(T&& x)
	: value(std::move(x)) {}

	explicit Value(const Value& x)
	: value(x.value) {}

	explicit Value(Value&& x)
	: value(std::move(x.value)) {}

	~Value() {}

	Value& operator=(const Value& rhs) {
		if (this == &rhs) {
			return *this;
		}
		value = rhs.value;
		return *this;
	}

	Value& operator=(Value&& rhs) {
		std::swap(value, rhs.value);
	}

	bool operator<(const Value& rhs) const {
		return value < rhs.value;
	}

	bool operator<(const T& rhs) const {
		return value < rhs;
	}

	bool operator==(const Value& rhs) const {
		return value == rhs.value;
	}

	bool operator==(const T& rhs) const {
		return value == rhs;
	}

	bool operator!=(const Value& rhs) const {
		return !(*this == rhs);
	}

	bool operator!=(const T& rhs) const {
		return !(*this == rhs);
	}


private:
	T value;
};

// template <typename T>
// bool operator==(const Value<T>& a, const T& b) {
// 	return a.value == b;
// }

template <typename T>
bool operator==(const T& a, const Value<T>& b) {
	return b == a;
}
