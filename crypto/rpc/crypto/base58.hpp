#pragma once
#include <iterator>
#include <string>
#include <vector>
#include "../../solana/headers.hpp"

static const char* const base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
static const std::int8_t base86_map[] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    -1,
    17,
    18,
    19,
    20,
    21,
    -1,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    -1,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};

std::vector<std::uint8_t> base58_decode(const std::vector<std::uint8_t>& input);
std::vector<std::uint8_t> base58_encode(const std::vector<std::uint8_t>& input);

/* Encode string to base58 */
__inline std::vector<std::uint8_t> base58_decode(const std::string& input) {
    std::vector<std::uint8_t> v;
    std::copy(input.begin(), input.end(), std::back_inserter(v));
    return base58_decode(v);
}

/* Decode string to base58, result string */
__inline std::string base58_decode_str(const std::string& input) {
    const auto decoded = base58_decode(input);
    return std::string(decoded.begin(), decoded.end());
}

/* Encode string to base58 */
__inline std::vector<std::uint8_t> base58_encode(const std::string& input) {
    std::vector<std::uint8_t> v;
    std::copy(input.begin(), input.end(), std::back_inserter(v));
    return base58_encode(v);
}

/* Encode string to base58, result string */
__inline std::string base58_encode_str(const std::vector<std::uint8_t>& input) {
    const auto decoded = base58_encode(input);
    return std::string(decoded.begin(), decoded.end());
}

/* Encode string to base58, result string */
__inline std::string base58_encode_str(const std::string &input) {
      const auto decoded = base58_encode(input);
      return std::string(decoded.begin(), decoded.end());
}

/* TODO change to template */
/* Encode string to base58, result string */
__inline std::string base58_encode_str(const crypto_decoded_pkey& input) {
    const auto decoded = base58_encode(std::vector<std::uint8_t>(input.begin(), input.end()));
    return std::string(decoded.begin(), decoded.end());
}
__inline std::string base58_encode_str(const crypto_decoded_skey& input) {
    const auto decoded = base58_encode(std::vector<std::uint8_t>(input.begin(), input.end()));
    return std::string(decoded.begin(), decoded.end());
}


template<std::uint16_t s>
varray<s>  base58_decode(const std::uint8_t* input, const std::size_t size) {
    varray<s> result;
    result.emplace_back(static_cast<std::uint8_t>(0u));

    for (auto i = 0u; i < size; ++i) {
        std::uint32_t carry = base86_map[static_cast<std::uint32_t>(input[i])];
        for (auto j = 0u; j < result.size; ++j) {
            carry += static_cast<std::uint32_t>(result.data[j]) * 58u;
            result.data[j] = static_cast<std::uint8_t>(carry & 0xFF);
            carry >>= 8u;
        }
        while (carry > 0u) {
            result.emplace_back(static_cast<std::uint8_t>(carry & 0xFF));
            carry >>= 8u;
        }
    }

    for (auto i = 0u; i < size; ++i) {
        if (input[i] != '1') {
            break;
        }
        result.emplace_back(static_cast<std::uint8_t>(0u));
    }

    for (auto i = 0u, l = result.size - 1u; i < l; ++i, --l) {
        const auto temp = result.data[i];
        result.data[i] = result.data[l];
        result.data[l] = temp;
    }

    return result;
}
__inline varray<MAX_TX_SIZE> base58_decode(const std::uint8_t* input, const std::size_t size) {
    return base58_decode<MAX_TX_SIZE>(input, size);
}


template<std::uint16_t s>
varray<s>  base58_encode(const std::uint8_t* input, const std::size_t size) {
    varray<s> result;
    if (size > 898u) { /* Too large? (size * 137 / 100 + 1) */
        return result;
    }

    auto len = 1u;
    varray<s> digits;

    for (auto i = 0u; i < size; ++i) {
        auto carry = static_cast<std::uint32_t>(input[i]);
        for (auto a = 0u; a < len; ++a) {
            carry += static_cast<std::uint32_t>(digits.data[a]) << 8u;
            digits.data[a] = static_cast<std::uint8_t>(carry % 58u);
            carry /= 58u;
        }
        while (carry > 0u) {
            digits.data[len++] = static_cast<std::uint8_t>(carry % 58u);
            carry /= 58u;
        }
    }
    for (auto i = 0u; i < size && !input[i]; ++i) {
        result.emplace_back('1');
    }
    for (auto i = 0u; i < len; ++i) {
        result.emplace_back(base58_chars[digits.data[len - 1u - i]]);
    }

    return result;
}
__inline varray<MAX_TX_SIZE> base58_encode(const std::uint8_t* input, const std::size_t size) {
    return base58_encode<MAX_TX_SIZE>(input, size);
}
