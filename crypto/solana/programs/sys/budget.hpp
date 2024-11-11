#pragma once
#include "../../program.hpp"
#include "../program_data.hpp"

namespace program::budget {

	enum class instruction : std::uint8_t {
		/*
			units(u32),
			additional_fee(u32)
		*/
		request_units_deprecated,
		/*
			frame(u32)
		*/
		request_heap_frame,
		/*
			micro_lamports(u32)
		*/
		set_compute_unit_limit,
		/*
			lamports(u64)
		*/
		set_compute_unit_price
	};
	namespace operands {
		struct request_units_deprecated {
			std::uint32_t units = 0u;
			crypto_microlamports additional_fee = 0u;
		};
		struct request_heap_frame {
			std::uint32_t frame = 0u;
		};
		struct set_compute_unit_limit {
			crypto_microlamports micro_lamports = 0u;
		};
		struct set_compute_unit_price {
			crypto_lamports lamports = 0u;
		};
	}
	struct svm {

		instruction inst;
		varray<MAX_ACCOUNT_IDX_INST>* indexes;
		std::vector<crypto_decoded_pkey>* keys = nullptr;
		union {
			operands::request_units_deprecated request_units_deprecated;
			operands::request_heap_frame request_heap_frame;
			operands::set_compute_unit_limit set_compute_unit_limit;
			operands::set_compute_unit_price set_compute_unit_price;
		};

		svm() : inst(instruction::request_heap_frame), request_heap_frame{ 0u } {};

		__inline void inst_request_units_deprecated(const std::uint32_t units, const crypto_microlamports additional_fee) {
			this->inst = instruction::request_units_deprecated;
			this->request_units_deprecated.units = units;
			this->request_units_deprecated.additional_fee = additional_fee;
			return;
		}
		__inline void inst_request_heap_frame(const std::uint32_t frame) {
			this->inst = instruction::request_heap_frame;
			this->request_heap_frame.frame = frame;
			return;
		}
		__inline void inst_set_compute_unit_limit(const crypto_microlamports micro_lamports) {
			this->inst = instruction::set_compute_unit_limit;
			this->set_compute_unit_limit.micro_lamports = micro_lamports;
			return;
		}
		__inline void inst_set_compute_unit_price(const crypto_lamports lamports) {
			this->inst = instruction::set_compute_unit_price;
			this->set_compute_unit_price.lamports = lamports;
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
			switch (this->inst) {
			case instruction::request_units_deprecated: {
				this->request_units_deprecated.additional_fee = buffer.request_units_deprecated.additional_fee;
				this->request_units_deprecated.units = buffer.request_units_deprecated.units;
				break;
			}
			case instruction::request_heap_frame: {
				this->request_heap_frame.frame = buffer.request_heap_frame.frame;
				break;
			}
			case instruction::set_compute_unit_limit: {
				this->set_compute_unit_limit.micro_lamports = buffer.set_compute_unit_limit.micro_lamports;
				break;
			}
			case instruction::set_compute_unit_price: {
				this->set_compute_unit_price.lamports = buffer.set_compute_unit_price.lamports;
				break;
			}
			default: {
				break;
			}
			}
			return;
		}
		__inline bool disassemble(const std::uint8_t* data) {
			std::size_t offset = 0u;

			readtc(this->inst, data, std::uint8_t, instruction, offset);
			switch (this->inst) {
			case instruction::request_units_deprecated: {
				readt(this->request_units_deprecated.units, data, std::uint32_t, offset);
				readtc(this->request_units_deprecated.additional_fee, data, std::uint32_t, crypto_microlamports, offset);
				break;
			}
			case instruction::request_heap_frame: {
				readt(this->request_heap_frame.frame, data, std::uint32_t, offset);
				break;
			}
			case instruction::set_compute_unit_limit: {
				readtc(this->set_compute_unit_limit.micro_lamports, data, std::uint32_t, crypto_microlamports, offset);
				break;
			}
			case instruction::set_compute_unit_price: {
				readtc(this->set_compute_unit_price.lamports, data, std::uint64_t, crypto_lamports, offset);
				break;
			}
			default: {
				return false;
			}
			}
			return true;
		}
		__inline void assemble(std::vector<std::uint8_t>& buffer) {
			emit_data<std::uint8_t>(buffer, static_cast<std::uint8_t>(this->inst));
			switch (this->inst) {
			case instruction::request_units_deprecated: {
				emit_data<std::uint32_t>(buffer, static_cast<std::uint32_t>(this->request_units_deprecated.units));
				emit_data<std::uint32_t>(buffer, static_cast<std::uint32_t>(this->request_units_deprecated.additional_fee));
				break;
			}
			case instruction::request_heap_frame: {
				emit_data<std::uint32_t>(buffer, static_cast<std::uint32_t>(this->request_heap_frame.frame));
				break;
			}
			case instruction::set_compute_unit_limit: {
				emit_data<std::uint32_t>(buffer, static_cast<std::uint32_t>(this->set_compute_unit_limit.micro_lamports));
				break;
			}
			case instruction::set_compute_unit_price: {
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->set_compute_unit_price.lamports));
				break;
			}
			}
			return;
		}

	};

	/* ComputeBudget111111111111111111111111111111 */
	const crypto_decoded_pkey program_account = { 0x03, 0x06, 0x46, 0x6f, 0xe5, 0x21, 0x17, 0x32, 0xff, 0xec, 0xad, 0xba, 0x72, 0xc3, 0x9b, 0xe7, 0xbc, 0x8c, 0xe5, 0xbb, 0xc5, 0xf7, 0x12, 0x6b, 0x2c, 0x43, 0x9b, 0x3a, 0x40, 0x00, 0x00, 0x00 };
}
