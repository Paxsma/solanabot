#pragma once
#include "../program.hpp"
#include "program_data.hpp"

namespace program::associated_token_account {

	enum class instruction : std::uint8_t {
		/*
			(accounts on index data not required)
			create
		*/
		create
	};
	struct svm {


		instruction inst;
		varray<MAX_ACCOUNT_IDX_INST>* indexes;
		std::vector<crypto_decoded_pkey>* keys = nullptr;

		svm() : inst(instruction::create) {};

		__inline void inst_create() {
			this->inst = instruction::create;
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
			this->inst = buffer.inst;
			this->indexes = buffer.indexes;
			this->keys = buffer.keys;
			return;
		}
		__inline bool disassemble(const std::uint8_t* data) {
			return true;
		}
		__inline void assemble(std::vector<std::uint8_t>& buffer) {
			switch (this->inst) {
				case instruction::create:
				default: {
					break;
				}
			}
			return;
		}

	};

	/* ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL */
	const crypto_decoded_pkey program_account = { 0x8c, 0x97, 0x25, 0x8f, 0x4e, 0x24, 0x89, 0xf1, 0xbb, 0x3d, 0x10, 0x29, 0x14, 0x8e, 0x0d, 0x83, 0x0b, 0x5a, 0x13, 0x99, 0xda, 0xff, 0x10, 0x84, 0x04, 0x8e, 0x7b, 0xd8, 0xdb, 0xe9, 0xf8, 0x59 };
}
