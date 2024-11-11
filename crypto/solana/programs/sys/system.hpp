#pragma once
#include "../../program.hpp"
#include "../program_data.hpp"

namespace program::system {

	enum class instruction : std::uint32_t {
		/*
			lamports(u64),
			space(u64),
			owner(accountkey)
		*/
		create_account,
		/* pub(accountkey) */
		assign,
		/* lamports(u64) */
		transfer,
		/*
			base(accountkey),
			seed(string MAX_SEED_LEN),
			lamports(u64),
			space(u64),
			owner(accountkey)
		*/
		create_account_with_seed,
		advance_nonce_account,
		/* lamports(u64) */
		withdraw_nonce_account,
		/* address(accountkey) */
		initialize_nonce_account,
		/* address(accountkey) */
		authorize_nonce_account,
		/* space(u64) */
		allocate,
		/*
			base(accountkey),
			seed(string MAX_SEED_LEN),
			space(u64),
			owner(accountkey),
		*/
		allocate_with_seed,
		/*
			base(accountkey),
			seed(string MAX_SEED_LEN),
			owner(accountkey),
		*/
		assign_with_seed,
		/*
			lamports(u64),
			from_seed(string MAX_SEED_LEN),
			from_owner(accountkey),
		*/
		transfer_with_seed,
		upgrade_nonce_account
	};
	namespace operands {
		struct create_account {
			crypto_lamports lamports = 0u;
			std::uint64_t space = 0u;
			crypto_decoded_pkey owner = { 0 };
		};
		struct assign {
			crypto_decoded_pkey pub = { 0 };
		};
		struct transfer {
			crypto_lamports lamports = 0u;
		};
		struct create_account_with_seed {
			crypto_decoded_pkey base = { 0 };
			crypto_seed seed = "";
			crypto_lamports lamports = 0u;
			std::uint64_t space = 0u;
			crypto_decoded_pkey account = { 0 };
		};
		struct withdraw_nonce_account {
			crypto_lamports lamports = 0u;
		};
		struct initialize_nonce_account {
			crypto_decoded_pkey pub = { 0 };
		};
		struct authorize_nonce_account {
			crypto_decoded_pkey pub = { 0 };
		};
		struct allocate {
			std::uint64_t space = 0u;
		};
		struct allocate_with_seed {
			crypto_decoded_pkey base = { 0 };
			crypto_seed seed = "";
			std::uint64_t space = 0u;
			crypto_decoded_pkey account = { 0 };
		};
		struct assign_with_seed {
			crypto_decoded_pkey base = { 0 };
			crypto_seed seed = "";
			crypto_decoded_pkey owner = { 0 };
		};
		struct transfer_with_seed {
			crypto_lamports lamports = 0u;
			crypto_seed seed = "";
			crypto_decoded_pkey owner = { 0 };
		};
	}
	struct svm {

		instruction inst;
		varray<MAX_ACCOUNT_IDX_INST>* indexes;
		std::vector<crypto_decoded_pkey>* keys = nullptr;
		union {
			operands::create_account create_account;
			operands::assign assign;
			operands::transfer transfer;
			operands::create_account_with_seed create_account_with_seed;
			operands::withdraw_nonce_account withdraw_nonce_account;
			operands::initialize_nonce_account initialize_nonce_account;
			operands::authorize_nonce_account authorize_nonce_account;
			operands::allocate allocate;
			operands::allocate_with_seed allocate_with_seed;
			operands::assign_with_seed assign_with_seed;
			operands::transfer_with_seed transfer_with_seed;
		};

		svm() : inst(instruction::transfer), transfer({ 0u }) {};
		~svm() {
			switch (this->inst) {
			case instruction::create_account_with_seed: {
				this->create_account_with_seed.seed.~basic_string();
				break;
			}
			case instruction::allocate_with_seed: {
				this->allocate_with_seed.seed.~basic_string();
				break;
			}
			case instruction::assign_with_seed: {
				this->assign_with_seed.seed.~basic_string();
				break;
			}
			case instruction::transfer_with_seed: {
				this->transfer_with_seed.seed.~basic_string();
				break;
			}
			default: {
				break;
			}
			}
			return;
		};

		__inline void inst_create_account(const crypto_lamports lamports, const std::uint64_t space, const crypto_decoded_pkey& owner) {
			this->inst = instruction::create_account;
			this->create_account.lamports = lamports;
			this->create_account.space = space;
			this->create_account.owner = owner;
			return;
		}
		__inline void inst_assign(const crypto_decoded_pkey& pub) {
			this->inst = instruction::assign;
			this->assign.pub = pub;
			return;
		}
		__inline void inst_transfer(const crypto_lamports lamports) {
			this->inst = instruction::assign;
			this->transfer.lamports = lamports;
			return;
		}
		__inline void inst_allocate(const std::uint64_t space) {
			this->inst = instruction::allocate;
			this->allocate.space = space;
			return;
		}
		__inline void inst_create_account_with_seed(const crypto_decoded_pkey& base, const crypto_seed seed, const crypto_lamports lamports, const std::uint64_t space, const crypto_decoded_pkey& account) {
			this->inst = instruction::create_account_with_seed;
			this->create_account_with_seed.base = base;
			this->create_account_with_seed.seed = seed;
			this->create_account_with_seed.lamports = lamports;
			this->create_account_with_seed.space = space;
			this->create_account_with_seed.account = account;
			return;
		}
		__inline void inst_withdraw_nonce_account(const crypto_lamports lamports) {
			this->inst = instruction::withdraw_nonce_account;
			this->withdraw_nonce_account.lamports = lamports;
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
			case instruction::create_account: {
				this->create_account.lamports = buffer.create_account.lamports;
				this->create_account.space = buffer.create_account.space;
				this->create_account.owner = buffer.create_account.owner;
				break;
			}
			case instruction::assign: {
				this->assign.pub = buffer.assign.pub;
				break;
			}
			case instruction::transfer: {
				this->transfer.lamports = buffer.transfer.lamports;
				break;
			}
			case instruction::create_account_with_seed: {
				this->create_account_with_seed.base = buffer.create_account_with_seed.base;
				this->create_account_with_seed.seed = buffer.create_account_with_seed.seed;
				this->create_account_with_seed.lamports = buffer.create_account_with_seed.lamports;
				this->create_account_with_seed.space = buffer.create_account_with_seed.space;
				this->create_account_with_seed.account = buffer.create_account_with_seed.account;
				break;
			}
			case instruction::withdraw_nonce_account: {
				this->withdraw_nonce_account.lamports = buffer.withdraw_nonce_account.lamports;
				break;
			}
			case instruction::initialize_nonce_account: {
				this->initialize_nonce_account.pub = buffer.initialize_nonce_account.pub;
				break;
			}
			case instruction::authorize_nonce_account: {
				this->authorize_nonce_account.pub = buffer.authorize_nonce_account.pub;
				break;
			}
			case instruction::allocate: {
				this->allocate.space = buffer.allocate.space;
				break;
			}
			case instruction::allocate_with_seed: {
				this->allocate_with_seed.base = buffer.allocate_with_seed.base;
				this->allocate_with_seed.seed = buffer.allocate_with_seed.seed;
				this->allocate_with_seed.space = buffer.allocate_with_seed.space;
				this->allocate_with_seed.account = buffer.allocate_with_seed.account;
				break;
			}
			case instruction::assign_with_seed: {
				this->assign_with_seed.base = buffer.assign_with_seed.base;
				this->assign_with_seed.seed = buffer.assign_with_seed.seed;
				this->assign_with_seed.owner = buffer.assign_with_seed.owner;
				break;
			}
			case instruction::transfer_with_seed: {
				this->transfer_with_seed.lamports = buffer.transfer_with_seed.lamports;
				this->transfer_with_seed.seed = buffer.transfer_with_seed.seed;
				this->transfer_with_seed.owner = buffer.transfer_with_seed.owner;
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

			readtc(this->inst, data, std::uint32_t, instruction, offset);
			switch (this->inst) {
			case instruction::create_account: {
				readtc(this->create_account.lamports, data, std::uint64_t, crypto_lamports, offset);
				readt(this->create_account.space, data, std::uint64_t, offset);
				read(this->create_account.owner.data(), sizeof(crypto_decoded_pkey), data, offset);
				break;
			}
			case instruction::transfer: {
				readtc(this->transfer.lamports, data, std::uint64_t, crypto_lamports, offset);
				break;
			}
			case instruction::assign: {
				read(this->assign.pub.data(), sizeof(crypto_decoded_pkey), data, offset);
				break;
			}
			case instruction::allocate: {
				readt(this->allocate.space, data, std::uint64_t, offset);
				break;
			}
			case instruction::withdraw_nonce_account: {
				readtc(this->withdraw_nonce_account.lamports, data, std::uint64_t, crypto_lamports, offset);
				break;
			}
			case instruction::initialize_nonce_account: {
				read(this->initialize_nonce_account.pub.data(), sizeof(crypto_decoded_pkey), data, offset);
				break;
			}
			case instruction::authorize_nonce_account: {
				read(this->authorize_nonce_account.pub.data(), sizeof(crypto_decoded_pkey), data, offset);
				break;
			}

			case instruction::create_account_with_seed: {
				break;
			}
			case instruction::allocate_with_seed: {
				break;
			}
			case instruction::assign_with_seed: {
				break;
			}
			case instruction::transfer_with_seed: {
				break;
			}

			case instruction::advance_nonce_account:
			case instruction::upgrade_nonce_account: {
				break;
			}
			default: {
				return false;
			}
			}
			return true;
		}
		__inline void assemble(std::vector<std::uint8_t>& buffer) {
			emit_data<std::uint32_t>(buffer, static_cast<std::uint32_t>(this->inst));
			switch (this->inst) {
			case instruction::create_account: {
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->create_account.lamports));
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->create_account.space));
				emit_data(buffer, this->create_account.owner.begin(), this->create_account.owner.end());
				break;
			}
			case instruction::assign: {
				emit_data(buffer, this->assign.pub.begin(), this->assign.pub.end());
				break;
			}
			case instruction::transfer: {
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->transfer.lamports));
				break;
			}
			case instruction::withdraw_nonce_account: {
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->withdraw_nonce_account.lamports));
				break;
			}
			case instruction::allocate: {
				emit_data<std::uint64_t>(buffer, static_cast<std::uint64_t>(this->allocate.space));
				break;
			}
			case instruction::advance_nonce_account:
			case instruction::upgrade_nonce_account:
			default: {
				break;
			}
			}
			return;
		}
	};

	/* 11111111111111111111111111111111 */
	const crypto_decoded_pkey program_account = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
}
