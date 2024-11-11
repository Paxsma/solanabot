#pragma once
#include "../../program.hpp"
#include "../program_data.hpp"

namespace program::pump_fun {

	static constexpr crypto_lamports bonding_curve_supply = 800000000u;
	static constexpr crypto_sol bonding_curve_sol_bond = 86u;
	static constexpr crypto_sol bump_min = 0.01;

	static constexpr crypto_lamports initialVirtualSolReserves = 30000000000u;
	static constexpr crypto_lamports initialVirtualTokenReserves = 1073000000000000u;
	static constexpr crypto_lamports initialRealTokenReserves = 793100000000000u;
	static constexpr crypto_lamports feeBasisPoints = 18446744073709551615u;

	/*
		f(x) = (initialVirtualTokenReserves − (((initialVirtualSolReserves * initialVirtualTokenReserves)/(x + initialVirtualSolReserves)) + 1))
	*/
    #define pumpfun_calc_launch_price(x) static_cast<crypto_sol>(min(((crypto_calc_lamports_sol(program::pump_fun::initialVirtualTokenReserves) - (((crypto_calc_lamports_sol(program::pump_fun::initialVirtualSolReserves) * crypto_calc_lamports_sol(program::pump_fun::initialVirtualTokenReserves)) / (x + crypto_calc_lamports_sol(program::pump_fun::initialVirtualSolReserves))) + 1u)) * 1000u) + ((x > 10) ? -1000 : ((x < 10) ? 1000 : 0)), program::pump_fun::initialRealTokenReserves))
    #define pumpfun_calc_fee(x) static_cast<crypto_sol>(x * 0.01)
	
	enum class instruction : std::uint8_t {
		/*
			(0x66, 0x06, 0x3d, 0x12, 0x01, 0xda , 0xeb, 0xea)
			amount(u64),
			cost(u64)
		*/
		buy,

		/*
			(0x33, 0xe6, 0x85, 0xa4, 0x01, 0x7f, 0x83, 0xad)
			amount(u64),
			expected_price(u64)
		*/
		sell
	};
	namespace operands {
		struct buy {
			crypto_lamports amount = 0u;
			crypto_lamports cost = 0u;
		};
		struct sell {
			crypto_lamports amount = 0u;
			crypto_lamports expected_price = 0u;
		};
	}
	struct svm {

		instruction inst;
		varray<MAX_ACCOUNT_IDX_INST>* indexes;
		std::vector<crypto_decoded_pkey>* keys = nullptr;
		union {
			operands::buy buy;
			operands::sell sell;
		};

		svm() : inst(instruction::buy), buy{ 0 } {};

		__inline void inst_sell(const crypto_lamports amount, const crypto_lamports expected_price) {
			this->inst = instruction::sell;
			this->sell.amount = amount;
			this->sell.expected_price = expected_price;
			return;
		}
		__inline void inst_buy(const crypto_lamports amount, const crypto_lamports cost) {
			this->inst = instruction::buy;
			this->buy.amount = amount;
			this->buy.cost = cost;
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
			case instruction::buy: {
				this->buy.amount = buffer.buy.amount;
				this->buy.cost = buffer.buy.cost;
				break;
			}
			case instruction::sell: {
				this->sell.amount = buffer.sell.amount;
				this->sell.expected_price = buffer.sell.expected_price;
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

			std::uint8_t i = 0u;
			readt(i, data, std::uint8_t, offset);
			switch (i) {
			case 0x66: {
				this->inst = instruction::buy;
				break;
			}
			case 0x33: {
				this->inst = instruction::sell;
				break;
			}
			default: {
				return false;
			}
			}
			offset += 7u;

			switch (this->inst) {
			case instruction::buy: {
				readt(this->buy.amount, data, std::uint64_t, offset);
				readt(this->buy.cost, data, std::uint64_t, offset);
				break;
			}
			case instruction::sell: {
				readt(this->sell.amount, data, std::uint64_t, offset);
				readt(this->sell.expected_price, data, std::uint64_t, offset);
				break;
			}
			default: {
				return false;
			}
			}
			return true;
		}
		__inline void assemble(std::vector<std::uint8_t>& buffer) {
			switch (this->inst) {
			case instruction::buy: {
				emit_data(buffer, { 0x66, 0x06, 0x3d, 0x12, 0x01, 0xda , 0xeb, 0xea });
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->buy.amount));
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->buy.cost));
				break;
			}
			case instruction::sell: {
				emit_data(buffer, { 0x33, 0xe6, 0x85, 0xa4, 0x01, 0x7f, 0x83, 0xad });
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->buy.amount));
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->buy.cost));
				break;
			}
			default: {
				break;
			}
			}
			return;
		}

	};

	/* 6EF8rrecthR5Dkzon8Nwu78hRvfCKubJ14M5uBEwF6P */
	const crypto_decoded_pkey program_account = { 0x01, 0x56, 0xe0, 0xf6, 0x93, 0x66, 0x5a, 0xcf, 0x44, 0xdb, 0x15, 0x68, 0xbf, 0x17, 0x5b, 0xaa, 0x51, 0x89, 0xcb, 0x97, 0xf5, 0xd2, 0xff, 0x3b, 0x65, 0x5d, 0x2b, 0xb6, 0xfd, 0x6d, 0x18, 0xb0 };
}
