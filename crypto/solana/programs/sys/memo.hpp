#pragma once
#include "../../program.hpp"
#include "../../../rpc/crypto/base58.hpp"
#include "../program_data.hpp"

namespace program::memo {

	struct svm {

		varray<MAX_ACCOUNT_IDX_INST>* indexes;
		std::vector<crypto_decoded_pkey>* keys = nullptr;
		varray<0xFF> str;

		__inline void inst_str(const char* const str) {
			this->str.emplace_back(reinterpret_cast<const std::uint8_t*>(str), std::strlen(str));
			return;
		}

		__inline void add_idx(const std::uint8_t idx) {
			this->indexes->emplace_back(idx);
			return;
		}
		__inline void add_keys(std::vector<crypto_decoded_pkey>* v) {
			this->keys = v;
			return;
		}
		__inline void copy(const svm& buffer) {
			this->indexes = buffer.indexes;
			this->keys = buffer.keys;
			this->str = buffer.str;
			return;
		}
		__inline bool disassemble(const std::uint8_t* data, const std::size_t size) {
			const auto decoded = base58_decode(data, size);
			this->str.emplace_back(decoded.data.data(), decoded.size);
			return true;
		}
		__inline void assemble(std::vector<std::uint8_t>& buffer) {
			const auto encoded = base58_encode(this->str.data.data(), this->str.size);
			for (auto i = 0u; i < encoded.size; ++i) {
				buffer.emplace_back(encoded[i]);
			}
			return;
		}

	};

	/* MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr */
	const crypto_decoded_pkey program = { 0x05, 0x4a, 0x53, 0x5a, 0x99, 0x29, 0x21, 0x06, 0x4d, 0x24, 0xe8, 0x71, 0x60, 0xda, 0x38, 0x7c, 0x7c, 0x35, 0xb5, 0xdd, 0xbc, 0x92, 0xbb, 0x81, 0xe4, 0x1f, 0xa8, 0x40, 0x41, 0x05, 0x44, 0x8d };
}
