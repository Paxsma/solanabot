#include "tx.hpp"

namespace common {

	void budget(crypto::solana::versioned_transaction& tx, const crypto_microlamports microlamports, const crypto_lamports lamports) {
		program::budget::svm svm;
		svm.add_keys(&tx.header.account_keys);

		/* limit */
		crypto::solana::transaction::compiled_instruction limit;
		svm.inst_set_compute_unit_limit(microlamports);
	//	svm.assemble(limit.data);
		limit.dism.emit(svm);
		limit.program_id_index = tx.header.idx_account_key(program::budget::program_account);

		/* price */
		crypto::solana::transaction::compiled_instruction price;
		svm.inst_set_compute_unit_price(lamports);
		//svm.assemble(price.data);
		price.dism.emit(svm);
		price.program_id_index = tx.header.idx_account_key(program::budget::program_account);

	    tx.header.instructions.emplace_back(limit);
		tx.header.instructions.emplace_back(price);
		return;
	}

	void transfer(crypto::solana::versioned_transaction& tx, const crypto::wallet& src, const crypto::wallet& dest, const crypto_lamports amount) {
		
		crypto::solana::transaction::compiled_instruction inst;

		program::system::svm svm;
		svm.add_keys(&tx.header.account_keys);
		svm.add_idx(tx.header.idx_account_key(src.get_public_key()));
		svm.add_idx(tx.header.idx_account_key(dest.get_public_key()));
		svm.inst_transfer(amount);
		inst.dism.emit(svm);
		inst.program_id_index = tx.header.idx_account_key(program::system::program_account);

		tx.header.instructions.emplace_back(inst);
		return;
	}

	namespace pumpfun {

		void sell(crypto::solana::versioned_transaction& tx, const crypto::spl_token_ownership& token, const crypto::wallet& src, const crypto_lamports amount, const crypto_lamports expected_price) {

			crypto::solana::transaction::compiled_instruction inst;

			program::pump_fun::svm svm;
			svm.add_keys(&tx.header.account_keys);
			svm.add_idx(tx.header.idx_account_key(accounts::deployers::pumpfun_global));
			svm.add_idx(tx.header.idx_account_key(accounts::deployers::pumpfun_fee));
			svm.add_idx(tx.header.idx_account_key(token.t.ca));
			svm.add_idx(tx.header.idx_account_key(token.ata_owner));
			svm.add_idx(tx.header.idx_account_key(token.ata));
			svm.add_idx(tx.header.idx_account_key(token.token_account));
			svm.add_idx(tx.header.idx_account_key(program::system::program_account));
			svm.add_idx(tx.header.idx_account_key(program::associated_token_account::program_account));
			svm.add_idx(tx.header.idx_account_key(program::spl_token::token::program_account));
			svm.add_idx(tx.header.idx_account_key(accounts::deployers::pumpfun_eventauthority));
			svm.add_idx(tx.header.idx_account_key(program::pump_fun::program_account));
			svm.inst_sell(amount, expected_price);
			inst.dism.emit(svm);
			inst.program_id_index = tx.header.idx_account_key(program::pump_fun::program_account);

			tx.header.instructions.emplace_back(inst);
			return;
		}

	}
}


crypto::solana::versioned_transaction crypto::tx::solana::send(wallet& src, wallet& dest, crypto_lamports amount) {

	crypto::solana::versioned_transaction tx;

	if (amount == lamports_all) {
		amount = src.get_sol();
	}
	tx.header.num_required_signatures = 1u;
	tx.header.version = DEFAULT_TX_VERSION;

	tx.header.append_account_key<true>(src.get_public_key());
	tx.header.append_account_key<true>(dest.get_public_key());
	tx.header.append_account_key(program::system::program_account);
	tx.header.append_account_key(program::budget::program_account);

	common::budget(tx, DEFAULT_BUDGET_MICROLAMPORTS, DEFAULT_BUDGET_LAMPORTS);
	common::transfer(tx, src, dest, amount);
	return tx;
}

crypto::solana::versioned_transaction crypto::tx::pumpfun::sell(wallet& src, spl_token_ownership& token, crypto_lamports amount) {

	crypto::solana::versioned_transaction tx;

	if (amount == lamports_all) {
		amount = token.owned;
	}
	tx.header.num_required_signatures = 1u;
	tx.header.version = DEFAULT_TX_VERSION;

	tx.header.append_account_key(accounts::deployers::pumpfun_global);
	tx.header.append_account_key(accounts::deployers::pumpfun_fee);
	tx.header.append_account_key(accounts::deployers::pumpfun_eventauthority);
	tx.header.append_account_key(program::budget::program_account);
	tx.header.append_account_key(program::system::program_account);
	tx.header.append_account_key(program::pump_fun::program_account);
	tx.header.append_account_key(program::associated_token_account::program_account);
	tx.header.append_account_key(program::spl_token::token::program_account);
	tx.header.append_account_key(accounts::router::bloxroute);
	tx.header.append_account_key(token.ata_owner);
	tx.header.append_account_key(token.ata);
	tx.header.append_account_key(token.token_account);
	tx.header.append_account_key(token.t.ca);
	tx.header.append_account_key(src.get_public_key());

	common::budget(tx, DEFAULT_BUDGET_MICROLAMPORTS, DEFAULT_BUDGET_LAMPORTS);
	common::transfer(tx, src, wallet(accounts::router::bloxroute), 300000);
	common::pumpfun::sell(tx, token, src, amount, 0u);
	return tx;
}

