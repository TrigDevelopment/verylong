#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

class verylong {
public:
	verylong() : is_negative(false), is_set(false) { }
	verylong(std::string const & string) {
		string_set(string);
	}

#pragma region Base numeric type constructors
		verylong(signed char const c) {
			if (c < 0) this->is_negative = true;
			else this->is_negative = false;
			this->digits.push_back(std::abs(c));
		}
		verylong(unsigned char c) {
			this->is_negative = false;
			this->digits.push_back(c);
		}
		verylong(signed short s) {
			if (s < 0) this->is_negative = true;
			else this->is_negative = false;
			this->digits.push_back(std::abs(s));
		}
		verylong(unsigned short c) {
			this->is_negative = false;
			this->digits.push_back(c);
		}
		verylong(signed int i) {
			if (i < 0) this->is_negative = true;
			else this->is_negative = false;
			this->digits.push_back(std::abs(i) % verylong::BASE);
			i /= verylong::BASE;
			if (i != 0) this->digits.push_back(std::abs(i));
		}
		verylong(unsigned int number) : verylong() {
			digits.push_back(number % verylong::BASE);
			number /= verylong::BASE;
			if (number != 0) {
				digits.push_back(number);
			}
		}
		verylong(size_t number) : verylong() {
			digits.push_back(number % verylong::BASE);
			number /= verylong::BASE;
			if (number != 0) {
				digits.push_back(number);
			}
		}
		verylong(signed long l) {
			if (l < 0) this->is_negative = true;
			else this->is_negative = false;
			this->digits.push_back(std::abs(l) % verylong::BASE);
			l /= verylong::BASE;
			if (l != 0) this->digits.push_back(std::abs(l));
		};
		verylong(unsigned long l) {
			this->digits.push_back(l % verylong::BASE);
			l /= verylong::BASE;
			if (l != 0) this->digits.push_back(l);
		};
		verylong(signed long long l) {
			if (l < 0) { this->is_negative = true; l = -l; }
			else this->is_negative = false;
			do {
				this->digits.push_back(l % verylong::BASE);
				l /= verylong::BASE;
			} while (l != 0);
		};
		verylong(unsigned long long l) {
			this->is_negative = false;
			do {
				this->digits.push_back(l % verylong::BASE);
				l /= verylong::BASE;
			} while (l != 0);
		};
#pragma endregion

	void output(std::ostream& stream = std::cout) const {
		if (digits.empty()) {
			stream << 0;
		}
		else {
			if (is_negative) stream << '-';
			stream << digits.back();
			char old_fill = stream.fill('0');
			for (int i = digits.size() - 2; i >= 0; --i) {
				stream << std::setw(9) << digits[i];
			}
			stream.fill(old_fill);
		}
	}
	void safe_input_size_t(std::istream& stream = std::cin) {
		std::string string_number;
		stream >> string_number;
		if (is_unsigned_int(string_number)) {
			clear();

			for (auto ch : string_number) {
				*this = *this * 10 + ch - '0';
				if (*this > SIZE_MAX) {
					is_set = false;
					break;
				}
			}
			is_set = true;
		}
		else {
			is_set = false;
		}
	}
	std::ostream& operator << (std::ostream& os) {
		if (digits.empty()) {
			os << 0;
		}
		else {
			if (is_negative) os << '-';
			os << digits.back();
			char old_fill = os.fill('0');
			for (int i = digits.size() - 2; i >= 0; --i) {
				os << std::setw(9) << digits[i];
			}
			os.fill(old_fill);
		}

		return os;
	}

	#pragma region Comparsion operators
	bool operator == (verylong const & right) const {
		if (is_negative != right.is_negative) return false;
		if (digits.empty()) {
			if (right.digits.empty() || (right.digits.size() == 1 && right.digits[0] == 0)) return true;
			else return false;
		}

		if (right.digits.empty()) {
			if (digits.size() == 1 && digits[0] == 0) return true;
			else return false;
		}

		if (digits.size() != right.digits.size()) return false;
		for (size_t i = 0; i < digits.size(); ++i) if (digits[i] != right.digits[i]) return false;

		return true;
	}
	bool operator != (verylong const & right) const {
		return !((*this) == right);
	}
	bool operator < (verylong const & right) const {
		if ((*this) == right) return false;
		if (is_negative) {
			if (right.is_negative) return ((-right) < (-(*this)));
			else return true;
		}
		else if (right.is_negative) return false;
		else {
			if (digits.size() != right.digits.size()) {
				return digits.size() < right.digits.size();
			}
			else {
				for (int i = digits.size() - 1; i >= 0; --i) {
					if (digits[i] != right.digits[i]) return digits[i] < right.digits[i];
				}

				return false;
			}
		}
	}
	bool operator > (verylong const & right) const {
		return !((*this) < right || (*this) == right);
	};
	#pragma endregion
	#pragma region Arithmetic operators
	const verylong operator - () const {
		verylong result(*this);
		result.is_negative = !result.is_negative;
		return result;
	}

	const verylong operator + (const verylong& right) const {
		if (is_negative) {
			if (right.is_negative) return -(-(*this) + (-right));
			else return right - (-(*this));
		}
		else if (right.is_negative) {
			return (*this) - (-right);
		}
		else {
			verylong result(*this);
			int carry = 0;
			for (size_t i = 0;
				i < std::max((*this).digits.size(), right.digits.size()) || carry != 0; i++) {
				if (i == (*this).digits.size()) {
					result.digits.push_back(0);
				}
				result.digits[i] += carry + (i < right.digits.size() ? right.digits[i] : 0);
				carry = result.digits[i] >= verylong::BASE;
				if (carry != 0) result.digits[i] -= verylong::BASE;
			}

			return result;
		}
	};
	const verylong operator - (const verylong& right) const {
		/*if (right.is_negative) return left + (-right);
		else if (left.is_negative) return -(-left + right);
		else if (left < right) return -(right - left);
		int carry = 0;
		for (size_t i = 0; i < right.digits.size() || carry != 0; ++i) {
		left.digits[i] -= carry + (i < right.digits.size() ? right.digits[i] : 0);
		carry = left.digits[i] < 0;
		if (carry != 0) left.digits[i] += verylong::BASE;
		}

		left._remove_leading_zeros();*/
		return right;
	}
	const verylong operator * (const verylong& right) const {
		/*verylong result;
		result.digits.resize(left.digits.size() + right.digits.size());
		for (size_t i = 0; i < left.digits.size(); ++i) {
		int carry = 0;
		for (size_t j = 0; j < right.digits.size() || carry != 0; ++j) {
		long long cur = result.digits[i + j] +
		left.digits[i] * 1LL * (j < right.digits.size() ? right.digits[j] : 0) + carry;
		result.digits[i + j] = static_cast<int>(cur % verylong::BASE);
		carry = static_cast<int>(cur / verylong::BASE);
		}
		}

		result.is_negative = left.is_negative != right.is_negative;
		result._remove_leading_zeros();*/
		return right;
	};
	const verylong operator / (const verylong& right) const {
		verylong result;
		if (right == 0) {
			return result;
		}
		verylong b = right;
		b.is_negative = false;
		verylong current;
		result.digits.resize(digits.size());
		for (int i = digits.size() - 1; i >= 0; --i) {
			current.shift_right();
			current.digits[0] = digits[i];
			current.remove_leading_zeros();
			int x = 0, l = 0, r = verylong::BASE;
			while (l <= r) {
				int m = (l + r) / 2;
				verylong t = b * m;
				if (t < current || t == current) {
					x = m;
					l = m + 1;
				}
				else r = m - 1;
			}

			result.digits[i] = x;
			current = current - b * x;
		}

		result.is_negative = is_negative != right.is_negative;
		result.remove_leading_zeros();
		return result;
	}
	const verylong operator % (const verylong& right) const {
		verylong result = (*this) - ((*this) / right) * right;
		if (result.is_negative) {
			result = result + right;
		}
		return result;
	}

	verylong& operator += (const verylong& right) {
		return *this = (*this + right);
	}
	verylong& operator -= (const verylong& right) {
		return *this = (*this - right);
	}
	verylong& operator *= (const verylong& right) {
		return *this = (*this * right);
	};
	verylong& operator /= (const verylong& right) {
		return *this = (*this / right);
	};
	verylong& operator %= (const verylong& right) {
		return *this = (*this % right);
	};

	void operator--(int) {
		*this -= 1;
	}
	void operator++(int) {
		*this += 1;
	}
	#pragma endregion

	bool set() {
		return is_set;
	}
	bool odd() const {
		if (this->digits.size() == 0) return false;
		return this->digits[0] & 1;
	};
	bool even() const {
		return !odd();
	};
	const verylong pow(verylong & n) const {
		verylong a(*this), result(1);
		while (n != 0) {
			if (n.odd()) result *= a;
			a *= a;
			n /= 2;
		}
		return result;
	};
private:
	static const int BASE = 1000 * 1000 * 1000;
	std::vector<unsigned int> digits;
	bool is_negative;
	bool is_set;

	void clear() {
		digits.clear();
		is_negative = false;
		is_set = false;
	}
	void remove_leading_zeros() {
		while (digits.size() > 1 && digits.back() == 0) {
			digits.pop_back();
		}

		if (digits.size() == 1 && digits[0] == 0) is_negative = false;
	};
	void shift_right() {
		if (digits.size() == 0) {
			digits.push_back(0);
			return;
		}
		digits.push_back(digits[digits.size() - 1]);
		for (size_t i = digits.size() - 2; i > 0; --i) digits[i] = digits[i - 1];
		digits[0] = 0;
	};
	void string_set(std::string string) {
		if (string.length() == 0) {
			is_negative = false;
		}
		else {
			if (string[0] == '-') {
				string = string.substr(1);
				is_negative = true;
			}
			else {
				is_negative = false;
			}

			for (size_t i = string.length(); true; i -= 9) {
				if (i < 9) {
					digits.push_back(atoi(string.substr(0, i).c_str()));
					break;
				}
				else {
					digits.push_back(atoi(string.substr(i - 9, 9).c_str()));
				}
			}
			remove_leading_zeros();
		}
	}
	bool is_unsigned_int(std::string const & string) {
		return string.find_first_not_of("0123456789") == std::string::npos;
	}
};
