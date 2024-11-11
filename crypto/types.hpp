#pragma once
#include "crypto.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>

namespace crypto::types {

      class biguint {

          public:
            biguint(const std::string &amount)
                : data(amount), numb(boost::multiprecision::cpp_dec_float_100(amount)) {
                  this->fix_up();
                  return;
            }
            biguint(const char *const amount)
                : data(amount), numb(boost::multiprecision::cpp_dec_float_100(amount)) {
                  this->fix_up();
                  return;
            }
            biguint(const std::double_t value, const std::size_t precision)
                : precision(precision) {
                  this->parse(value);
                  this->clean();
                  this->numb = boost::multiprecision::cpp_dec_float_100(this->data);
                  return;
            }
            biguint(const std::string &amount, const std::size_t precision)
                : data(amount), precision(precision) {
                  this->clean();
                  this->numb = boost::multiprecision::cpp_dec_float_100(this->apply_precision(precision));
                  return;
            }
            biguint(const boost::multiprecision::cpp_dec_float_100 &numb)
                : data(numb.str()), numb(numb) {
                  this->fix_up();
                  return;
            }

            biguint operator+(const biguint &l) const {
                  return biguint(this->numb + l.get_float_dec());
            }
            biguint operator-(const biguint &l) const {
                  return biguint(this->numb - l.get_float_dec());
            }
            biguint operator*(const biguint &l) const {
                  return biguint(this->numb * l.get_float_dec());
            }
            biguint operator/(const biguint &l) const {
                  return biguint(this->numb / l.get_float_dec());
            }

            void operator+=(const biguint &l) {
                  this->numb += l.numb;
                  this->data = this->numb.str();
                  this->fix_up();
                  return;
            }
            void operator-=(const biguint &l) {
                  this->numb -= l.numb;
                  this->data = this->numb.str();
                  this->fix_up();
                  return;
            }
            void operator*=(const biguint &l) {
                  this->numb *= l.numb;
                  this->data = this->numb.str();
                  this->fix_up();
                  return;
            }
            void operator/=(const biguint &l) {
                  this->numb /= l.numb;
                  this->data = this->numb.str();
                  this->fix_up();
                  return;
            }

            bool operator>(const biguint &i) const {
                  return this->numb > i.get_float_dec();
            }
            bool operator>=(const biguint &i) const {
                  return this->numb >= i.get_float_dec();
            }
            bool operator<(const biguint &i) const {
                  return this->numb < i.get_float_dec();
            }
            bool operator<=(const biguint &i) const {
                  return this->numb <= i.get_float_dec();
            }
            bool operator!=(const biguint &i) const {
                  return this->numb != i.get_float_dec();
            }
            bool operator==(const biguint &i) const {
                  return this->numb == i.get_float_dec();
            }

            bool operator>(const std::intptr_t i) const {
                  return this->numb > i;
            }
            bool operator>=(const std::intptr_t i) const {
                  return this->numb >= i;
            }
            bool operator<(const std::intptr_t i) const {
                  return this->numb < i;
            }
            bool operator<=(const std::intptr_t i) const {
                  return this->numb <= i;
            }
            bool operator!=(const std::intptr_t i) const {
                  return this->numb != i;
            }
            bool operator==(const std::intptr_t i) const {
                  return this->numb == i;
            }

            void operator=(const biguint &i) {
                  this->data = i.get_data();
                  this->precision = i.get_precision();
                  this->numb = i.get_float_dec();
                  return;
            }

            template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, std::string>>>
            void operator=(const T &i) {
                  this->data = i;
                  this->numb = boost::multiprecision::cpp_dec_float_100(i);
                  this->fix_up();
                  return;
            }

            /* Get boost float dec */
            boost::multiprecision::cpp_dec_float_100 get_float_dec() const {
                  return this->numb;
            }

            /* Gets amount as a string with precision */
            std::string get_data() const {
                  return this->apply_precision(this->precision);
            }

            /* Gets amount of precision */
            std::size_t get_precision() const {
                  return this->precision;
            }

            /* Get amount as whole string */
            std::string get_whole_data() const {

                  auto result = this->data;
                  result.erase(std::remove_if(result.begin(), result.end(), [](const auto c) {
                        return c == '.';
                  }),
                      result.end());
                  return result;
            }

            /* Apply precision to whole bignum returns data with trailing 0's */
            std::string apply_whole_precision(const std::size_t precision) const {
                  auto str = this->get_whole_data();
                  if (precision >= str.length()) {
                        str.insert(0u, std::string((precision + 1u) - str.length(), '0'));
                  }
                  return str;
            }

            /* Apply precision to whole bignum returns data */
            std::string apply_precision(const std::size_t precision) const {
                  auto s = this->apply_whole_precision(precision);
                  if (precision) {
                        s.insert(s.length() - precision, ".");
                  }
                  return s;
            }

            /* Sees if data is 0 */
            bool empty() {
                  if (this->numb == 0u) {
                        return true;
                  }
                  return false;
            }

            /* Gets double data */
            std::double_t get_double() {
                  return this->numb.convert_to<std::double_t>();  
            }

          private:
            std::string data = "";
            std::size_t precision = 0u;
            boost::multiprecision::cpp_dec_float_100 numb;

            /* Parse string with precision */
            void parse() {
                  this->data = this->apply_precision(this->precision);
                  return;
            }

            /* Parse double with precision */
            void parse(const std::double_t value) {
                  std::ostringstream oss;
                  oss << std::fixed << std::setprecision(this->precision) << value;
                  this->data = oss.str();
                  this->data.erase(std::remove(this->data.begin(), this->data.end(), '.'), this->data.end());
                  return;
            }

            /* Cleans up data */
            void clean() {

                  this->data.erase(std::remove_if(this->data.begin(), this->data.end(), [](const auto c) {
                        return !std::isdigit(c);
                  }),
                      this->data.end());
                  return;
            }

            /* Calculate percision if it is unkown */
            void calculate_precision() {
                  const auto pos = this->data.find('.');
                  if (pos != std::string::npos) {
                        this->precision = this->data.length() - pos - 1;
                  }
                  return;
            }

            /* Fixes up data when new one is applied. */
            void fix_up() {
                  this->calculate_precision();
                  this->clean();
                  return;
            }
      };

} // namespace crypto::types