#pragma once
#include "headers.hpp"

#pragma region programs
#include "programs/dex/jupiter.hpp"
#include "programs/dex/orca.hpp"
#include "programs/dex/pumpfun.hpp"
#include "programs/dex/raydium.hpp"
#include "programs/associated_token_account.hpp"
#include "programs/sys/budget.hpp"
#include "programs/loaner/marginfi.hpp"
#include "programs/sys/memo.hpp"
#include "programs/program_data.hpp"
#include "programs/sys/system.hpp"
#include "programs/sys/token.hpp"
#pragma endregion

namespace program {

	enum class progam_kind : std::uint8_t {
		unknown,
		system,
		budget,
		spl_token_associated_token_account,
		pump_fun,
		memo
	};

	namespace spl_token {

		namespace token {

			enum class instruction : std::uint8_t {
				initialize_mint,
				initialize_account,
				initialize_multisig,
				transfer,
				approve,
				revoke,
				set_authority,
				mint_to,
				burn,
				close_account,
				freeze_account,
				thaw_account,
				transfer_checked,
				approve_checked,
				mint_to_checked,
				burn_checked,
				initialize_account2,
				sync_native,
				initialize_account3,
				initialize_multisig2,
				initialize_mint2,
				get_account_data_size,
				initialize_immutable_owner,
				amount_to_ui_amount,
				ui_amount_to_amount,
			};

			/* TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA */
			const crypto_decoded_pkey program_account = { 0x06, 0xdd, 0xf6, 0xe1, 0xd7, 0x65, 0xa1, 0x93, 0xd9, 0xcb, 0xe1, 0x46, 0xce, 0xeb, 0x79, 0xac, 0x1c, 0xb4, 0x85, 0xed, 0x5f, 0x5b, 0x37, 0x91, 0x3a, 0x8c, 0xf5, 0x85, 0x7e, 0xff, 0x00, 0xa9 };
		}

	}

	namespace unknown {

		struct svm {
			std::vector<std::uint8_t> data;
			varray<MAX_ACCOUNT_IDX_INST>* indexes;
		};
	}

	struct disassembly {
		progam_kind t = progam_kind::unknown;
		union {
			budget::svm budget;
			system::svm system;
			pump_fun::svm pump;
			memo::svm memo;
			associated_token_account::svm associated_account;
			unknown::svm unknown;
		};

		disassembly(const disassembly& rv) : t(rv.t) {
			switch (this->t) {
				case progam_kind::budget: {
					this->budget.copy(rv.budget);
					break;
				}
				case progam_kind::system: {
					this->system.copy(rv.system);
					break;
				}
				case progam_kind::pump_fun: {
					this->pump.copy(rv.pump);
					break;
				}
				case progam_kind::spl_token_associated_token_account: {
					this->associated_account.copy(rv.associated_account);
					break;
				}
				default: {
				//	new (&this->unknown.data) std::vector<std::uint8_t>(rv.unknown.data);
					break;
				}
			}
		}
		disassembly() : t(progam_kind::unknown) {
			new (&this->unknown) std::vector<std::uint8_t>(); 
		};
		~disassembly() {
			switch (this->t) {
				case progam_kind::unknown: {
					break;
				}
				default: {
					break;
				}
			}
			return;
		}

		__inline void emit(const budget::svm& budget) {
			this->t = progam_kind::budget;
			this->budget.copy(budget);
			return;
		}
		__inline void emit(const system::svm& sys) {
			this->t = progam_kind::system;
			this->system.copy(sys);
			return;
		}
		__inline void emit(const pump_fun::svm& p) {
			this->t = progam_kind::pump_fun;
			this->pump.copy(p);
			return;
		}
		__inline void emit(const memo::svm& p) {
			this->t = progam_kind::memo;
			this->memo.copy(p);
			return;
		}
		__inline void emit(const associated_token_account::svm& a) {
			this->t = progam_kind::spl_token_associated_token_account;
			this->associated_account.copy(a);
			return;
		}
		__inline void disassemble(const crypto_decoded_pkey& program, const std::vector<std::uint8_t>& data, const varray<MAX_ACCOUNT_IDX_INST>& idxs) {

			return;
		}
	};
}
