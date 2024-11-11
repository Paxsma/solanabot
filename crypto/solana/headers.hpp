#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

using crypto_lamports = std::uint64_t;
using crypto_microlamports = std::uint32_t;
using crypto_sol = double;
using crypto_decoded_sk = std::array<std::uint8_t, 32u>;
using crypto_decoded_pkey = std::array<std::uint8_t, 32u>;
using crypto_decoded_skey = std::array<std::uint8_t, 64u>;
using crypto_decoded_signature = std::array<std::uint8_t, 64u>;
using crypto_encoded_key = std::string;
using crypto_seed = std::string;
#define crypto_lamports_little_endian static_cast<crypto_lamports>(1000000000u)

/* NULL */
static constexpr crypto_decoded_pkey crypto_null_pkey = { 0u };
static constexpr crypto_decoded_skey crypto_null_skey = { 0u };
static constexpr crypto_decoded_signature crypto_null_signature = { 0u };

/* Calc */
#define crypto_calc_slipage_threshold(amount, slipage) ((amount) * (slipage)) - (amount))
#define crypto_calc_fee(x, fee) ((x) - ((x) * (fee)))
#define crypto_calc_sol_lamports(x) (static_cast<crypto_lamports>(x) * crypto_lamports_little_endian)
#define crypto_calc_lamports_sol(x) (static_cast<crypto_sol>(x) / crypto_lamports_little_endian)
#define crypto_calc_lamport_decplace 9u

/* Limits */
#define MAX_TX_SIZE 1232u
#define MAX_TX_USIZE (MAX_TX_SIZE - 64u)
#define MAX_SIGNATURE_SIZE 88u
#define MAX_HASH_SIZE 44u
#define MAX_ACCOUNT_IDX_INST 44u
#define MAX_INST_DATA_SIZE 0xFF

/* Defaults */
#define DEFAULT_BUDGET_LAMPORTS static_cast<crypto_lamports>(100000u)
#define DEFAULT_BUDGET_MICROLAMPORTS static_cast<crypto_microlamports>(500u)
#define DEFAULT_TX_VERSION 128u

/* Stack pointer that keeps track of size */
struct sptr {
	std::uint8_t* data = nullptr;
	std::size_t size = 0u;
};

/* Stack array keeps track of size */
template<std::uint16_t data_size>
struct varray {
	std::array<std::uint8_t, data_size> data;
	std::uint16_t size = 0u;

	__inline void emplace_back(const std::uint8_t e) {
		this->data[this->size++] = e;
		return;
	}
	template<std::uint16_t size>
	__inline void emplace_back(const varray<size>& v) {
		for (std::uint16_t i = 0u; i < v.size; ++i) {
			this->emplace_back(v.data[i]);
		}
		return;
	}
	template<typename t>
	__inline void emplace_back(const t v) {
		for (auto i = 0u; i < sizeof(v); ++i) {
			this->emplace_back(static_cast<std::uint8_t>((v >> (i * 8u)) & 0xFF));
		}
		return;
	}
	__inline void emplace_back(const std::vector<std::uint8_t>& v) {
		for (const auto& i : v) {
			this->emplace_back(i);
		}
		return;
	}
	template <class it, std::enable_if_t<std::_Is_iterator_v<it>, int> = 0>
	__inline void emplace_back(const it& begin, const it& end) {
		for (auto i = begin; i != end; ++i) {
			this->emplace_back(*i);
		}
		return;
	}
	__inline void emplace_back(const std::uint8_t* arr, const std::size_t size) {
		for (auto i = 0u; i < size; ++i) {
			this->emplace_back(arr[i]);
		}
		return;
	}
	__inline std::string str() const {
		return std::string(this->data.begin(), this->data.begin() + this->size);
	}
	__inline bool empty() const {
		return (!this->size);
	}
	__inline bool copy(std::uint8_t* data, const std::size_t capacity) const {
		if (this->size > capacity) {
			return false;
		}
		for (auto i = 0u; i < this->size; ++i) {
			data[i] = this->data[i];
		}
		return true;
	}
	__inline std::uint8_t operator[] (const std::uint16_t n) const {
		return this->data[n];
	}
	template<std::uint16_t size>
	__inline bool operator== (const varray<size>& v) const {
		if (v.size != this->size) {
			return false;
		}
		for (std::uint16_t i = 0u; i < v.size; ++i) {
			if (v.data[i] != this->data[i]) {
				return false;
			}
		}
		return true;
	}
	template<std::uint16_t size>
	__inline bool operator!= (const varray<size>& v) const {
		return !(this == v);
	}
};
using signed_tx = varray<MAX_TX_SIZE>;
using unsigned_tx = varray<MAX_TX_USIZE>;

/* Deserialization */
#define read(ptr, size, data, offset) std::memcpy(ptr, data + offset, size); \
                                      offset += size;
#define readt(ptr, data, t, offset) ptr = *reinterpret_cast<const t*>(data + offset); \
                                    offset += sizeof(t);
#define readtc(ptr, data, t, cast, offset) ptr = static_cast<cast>(*reinterpret_cast<const t*>(data + offset)); \
										   offset += sizeof(t);

#define read_tx(ptr, size, data, offset) std::memcpy(ptr, data.data.data() + offset, size); \
                                         offset += size;
#define readt_tx(ptr, data, t, offset) ptr =  *reinterpret_cast<const t*>(data.data.data() + offset); \
                                       offset += sizeof(t);

#define watermark "x3 swap made by https://x.com/MythicShadowss"