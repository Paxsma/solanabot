#include "crypto/crypto.hpp"
#include "crypto/serializer/serializer.hpp"
#include "crypto/exchanges/exchanges.hpp"
#include "crypto/rpc/crypto/base58.hpp"
#include "crypto/rpc/crypto/base64.hpp"
#include "crypto/rpc/rpc.hpp"
#include "crypto/trackers/trackers.hpp"
#include "crypto/types.hpp"
#include "crypto/checker/checker.hpp"
#include "crypto/rpc/crypto/base64.hpp"
#include "crypto/tx/tx.hpp"

std::int32_t main() {


	crypto::error::error err;
	crypto::wallet dest("PRIVATE_KEY", "NODERPC");
	crypto::wallet src("PKEY", "SKEY", "NODERPC");


	auto tx = crypto::tx::solana::send(src, dest, crypto_calc_lamports_sol(static_cast<crypto_lamports>(0.05)));
	tx.update(src, err);


	const auto serialized = crypto::solana::serializer::unsigned_transaction(tx);
	const auto sign = src.sign_ed25519(err, serialized);
	const auto signed_tx = base64_encode(reinterpret_cast <const char* const>(sign.data.data()), sign.size);
	std::cout << "RESULT " << crypto::api::solana::send_transaction(signed_tx, src, err) << std::endl;

	std::cin.get();
	return 0;
}