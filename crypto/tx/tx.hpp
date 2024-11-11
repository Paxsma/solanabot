#pragma once
#include "../crypto.hpp"
#include "../serializer/serializer.hpp"

#define lamports_all static_cast<crypto_lamports>(0u)

namespace crypto::tx {

	namespace solana {

		crypto::solana::versioned_transaction send(wallet & src, wallet & dest, crypto_lamports amount = lamports_all);

	}

	namespace pumpfun {

		crypto::solana::versioned_transaction sell(wallet& src, spl_token_ownership& token, crypto_lamports amount = lamports_all);

	}
}