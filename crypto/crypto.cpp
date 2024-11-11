#include "crypto.hpp"
#include "curve25519/include/ed25519_signature.h"
#include <chrono>
#include <sodium.h>

__inline crypto_decoded_sk gen_secret() {

    crypto_decoded_sk result;
    
    for (auto i = 0u; i < 4u; ++i) {
        *reinterpret_cast<std::uint64_t*>(result.data() + (i * 8u)) = (static_cast<std::uint64_t>(std::rand()) << 32u) | std::rand();
    }
    for (auto& i : result) { /* Salt */
        i = static_cast<std::uint8_t>(((i << 3u) + 12u) * 2u);
    }
    return result;
}

void crypto::wallet::random_kp_ed25519() {

    crypto_decoded_pkey pkey = { 0 };
    crypto_decoded_skey skey = { 0 };
    ed25519_CreateKeyPair(pkey.data(), skey.data(), NULL, gen_secret().data());

    this->pkey = pkey;
    this->skey = skey;
    this->s_pkey = base58_encode(pkey.data(), sizeof(pkey)).str();
    this->s_skey = base58_encode(skey.data(), sizeof(skey)).str();
    return;
}

void crypto::wallet::random_ed25519() {

    crypto_decoded_pkey pkey;
    crypto_decoded_skey skey;
    ed25519_CreateKeyPair(pkey.data(), skey.data(), NULL, gen_secret().data());

    this->pkey = pkey;
    this->skey = skey;
    return;
}

signed_tx crypto::wallet::sign_ed25519(crypto::error::error& error, const unsigned_tx& msg) {

    signed_tx result;

    std::uint8_t sig[crypto_sign_BYTES];
    std::uint64_t len = 0u;
    if (crypto_sign_detached(sig, &len, msg.data.data(), msg.size, this->skey.data())) {
        error.t = crypto::error::type::signing;
        error.what_str = "Signing TX error";
    }
    else {
        result.emplace_back(static_cast<std::uint8_t>(1u));
        result.emplace_back(sig, len);
        result.emplace_back(msg);
    }
    return result;
}