#include "base58.hpp"

std::vector<std::uint8_t> base58_encode(const std::vector<std::uint8_t>& input) {

    std::vector<std::uint8_t> result;

    auto len = 1u;
    std::vector<std::uint8_t> digits(input.size() * 137 / 100 + 1);
    for (const auto& byte : input) {
        auto carry = static_cast<std::uint32_t>(byte);
        for (auto i = 0u; i < len; ++i) {
            carry += static_cast<std::uint32_t>(digits[i]) << 8u;
            digits[i] = static_cast<std::uint8_t>(carry % 58u);
            carry /= 58u;
        }
        while (carry > 0u) {
            digits[len++] = static_cast<std::uint8_t>(carry % 58u);
            carry /= 58u;
        }
    }

    for (auto i = 0u; i < input.size() && !input[i]; ++i) {
        result.emplace_back('1');
    }

    for (auto i = 0u; i < len; ++i) {
        result.emplace_back(base58_chars[digits[len - 1u - i]]);
    }

    return result;
}

std::vector<std::uint8_t> base58_decode(const std::vector<std::uint8_t>& input) {
      std::vector<std::uint8_t> result;
      result.emplace_back(0);

      auto len = 1u;
      for (const auto &c : input) {
            std::uint32_t carry = base86_map[static_cast<std::uint32_t>(c)];
            for (auto j = 0u; j < len; ++j) {
                  carry += static_cast<std::uint32_t>(result[j]) * 58u;
                  result[j] = static_cast<std::uint8_t>(carry & 0xFF);
                  carry >>= 8u;
            }
            while (carry > 0u) {
                  result.emplace_back(static_cast<std::uint8_t>(carry & 0xFF));
                  ++len;
                  carry >>= 8u;
            }
      }

      for (const auto i : input) {
            if (i != '1') {
                  break;
            }
            result.emplace_back(0u);
            ++len;
      }

      for (auto i = 0u, l = len - 1u; i < l; ++i, --l) {
            const auto temp = result[i];
            result[i] = result[l];
            result[l] = temp;
      }

      return result;
}
