#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <rapidjson/document.h>
#include <cpr/cpr.h>
#include "crypto.hpp"
#include "types.hpp"
#include "rpc/crypto/base58.hpp"
#include "solana/headers.hpp"
#include "solana/program.hpp"
#include "solana/accounts.hpp"

namespace crypto {

      /* Common error types */
      namespace error {

            enum class type : std::uint8_t {
                  none,                /* Nothing */
                  invalid_token,       /* Invalid input token */
                  invalid_qoute,       /* Invalid qoute */
                  invalid_status,      /* Invalid status return */
                  invalid_digits,      /* Invalid digits */
                  wrong_capacity,      /* Error comparing capacities */
                  invalid_mint,        /* Invalid mint address */
                  json_parse,          /* Invalid JSON */
                  json_outdated,       /* Outdated JSON */
                  no_route,            /* No transaction route plan */
                  conversion,          /* Digits converision */
                  unkown,              /* Unkown most likely with JSON */
                  insufficient_swap,   /* Not enough input amount to swap */
                  unsupported,         /* Unsupported exchange, swap, etc */
                  invalid_search,      /* Invalid search */
                  signing              /* Error signing */
            };

            struct error {
                  type t = type::none;

                  __inline bool err() const {
                        return (this->t != type::none);
                  }
                  __inline std::string what() const {
                        if (!this->what_str.empty()) {
                              return this->what_str;
                        }
                        switch (this->t) {
                              case type::invalid_token: {
                                    return "Invalid token";
                              }
                              case type::invalid_qoute: {
                                    return "Invalid qoute";
                              }
                              case type::invalid_status: {
                                    return "Invalid status";
                              }
                              case type::invalid_digits: {
                                    return "Invalid digits";
                              }
                              case type::wrong_capacity: {
                                  return "Wrong capacity";
                              }
                              case type::invalid_mint: {
                                    return "Invalid mint";
                              }
                              case type::json_parse: {
                                    return "Json parse error";
                              }
                              case type::json_outdated: {
                                    return "Outdated json";
                              }
                              case type::no_route: {
                                    return "No route";
                              }
                              case type::conversion: {
                                    return "Conversion error";
                              }
                              case type::unkown: {
                                    return "Unkown error";
                              }
                              case type::insufficient_swap: {
                                    return "Insufficient amount for swap";
                              }
                              case type::unsupported: {
                                    return "Unspported network";
                              }
                              case type::none:
                              default: {
                                    break;
                              }
                        }
                        return "No error";
                  }
                  std::string what_str = "";
            };
      } // namespace error

      namespace exchange {

            enum class supported : std::uint8_t {
                  none,      /* Nothing */
                  jupiter,   /* V6 (DEX) https://station.jup.ag/api-v6 */
                  kyberswap, /* V1 (DEX) https://kyberswap.com/swap */
                  uniswap    /* (DEX) https://uniswap.org/ */
            };

            /* Exchange qoute */
            struct exchange_qoute {

                  supported exchange = supported::none;               /* Qouted exchange */
                  std::string qoute = "";                             /* Qoute JSON */
                  types::biguint price_impact = types::biguint("0");  /* Price impact */
                  types::biguint swap_fee = types::biguint("0");      /* Swap fee */
                  std::string input_address = "";                     /* Input address */
                  types::biguint input_amount = types::biguint("0");  /* Input amount */
                  std::string output_address = "";                    /* Output address */
                  types::biguint output_amount = types::biguint("0"); /* Estimate output amount */
                  types::biguint slipage = types::biguint("0");       /* Slipage */

                  types::biguint output_threshold = types::biguint("0");        /* Estimate output treshold  */
                  types::biguint exchange_rate = types::biguint("0");           /* Exchange rate from Chain Coin -> Estimate Output */
                  types::biguint exchange_threshold_rate = types::biguint("0"); /* Exchange rate from Chain Coin -> Treshold Output */

                  void finalize() {
                        if (this->slipage != types::biguint(0u, 0u)) {
                              //this->output_threshold = crypto_calc_slipage_threshold(this->output_amount, this->slipage);
                        }
                        this->exchange_rate = this->input_amount / this->output_amount;
                        this->exchange_threshold_rate = this->input_amount / this->output_threshold;
                        return;
                  }
            };

            /* Exchange swap */
            struct exchange_swap {
                  supported exchange = supported::none;          /* Swapped exchange */
                  std::string swap = "";                         /* Swap Json */
                  std::string transaction = "";                  /* Transaction address */
                  std::vector<std::uint8_t> transaction_decoded; /* Decoded transaction */
            };

      } // namespace exchange

      namespace token {

            /* Supported trackers for tokens */
            enum class trackers : std::uint8_t {
                  none,          /* Nothing */
                  dexscreener,   /* https://dexscreener.com/ */
                  geckoterminal, /* https://www.geckoterminal.com/ */
            };

            /* Tacking tokens */
            struct token_tracking {

                  bool base = false;                 /* Just has network address and mint address. */
                  trackers tracker = trackers::none; /* Token tracker */
                  std::string dex_id = "";           /* Dex ID */
                  std::string chain_id = "";         /* Chain ID */
                  std::string pair_address = "";     /* Pair address */
                  std::string network_address = "";  /* Network address */
                  std::string network_name = "";     /* Network name */
                  std::string network_symbol = "";   /* Network symbol */
                  std::string mint_address = "";     /* Mint address */
                  std::string url = "";              /* Token url */
                  std::string creation = "";         /* Creation time */

                  std::string qoute_symbol = "";  /* Qoute symbol */
                  std::string qoute_address = ""; /* Qoute address */
                  std::string qoute_name = "";    /* Qoute name */

                  types::biguint liquid_usd = types::biguint("0");          /* Amount of liquid USD */
                  types::biguint liquid_token_amount = types::biguint("0"); /* Amount of liquid tokens */
                  types::biguint liquid_pooled = types::biguint("0");       /* Amount of pooled crypto */

                  types::biguint price_native = types::biguint("0"); /* Native price */
                  types::biguint price_usd = types::biguint("0");    /* Native USD price */

                  std::double_t price_change_minute_5 = 0; /* Price change in 5 minutes */
                  std::double_t price_change_hour_1 = 0;   /* Price change in 1 hours  */
                  std::double_t price_change_hour_6 = 0;   /* Price change in 6 hours */
                  std::double_t price_change_hour_24 = 0;  /* Price change in 24 hours */

                  std::double_t volume_minute_5 = 0; /* Volume change change in 5 minutes */
                  std::double_t volume_hour_1 = 0;   /* Volume change change in 1 hour */
                  std::double_t volume_hour_6 = 0;   /* Volume change change in 6 hour */
                  std::double_t volume_hour_24 = 0;  /* Volume change change in 24 hour */

                  std::double_t txns_minute_5_buys = 0;  /* Amount of buys in 5 minutes */
                  std::double_t txns_minute_5_sells = 0; /* Amount of sells in 5 minutes */
                  std::double_t txns_hour_1_buys = 0;    /* Amount of buys in 1 hour */
                  std::double_t txns_hour_1_sells = 0;   /* Amount of sell in 1 hour */
                  std::double_t txns_hour_6_buys = 0;    /* Amount of buys in 6 hour */
                  std::double_t txns_hour_6_sells = 0;   /* Amount of sell in 6 hour */
                  std::double_t txns_hour_24_buys = 0;   /* Amount of buys in 24 hour */
                  std::double_t txns_hour_24_sells = 0;  /* Amount of sell in 24 hour */

                  types::biguint est_supply = types::biguint("0"); /* Estimate supply */

                  types::biguint fdv = types::biguint("0"); /* Fully disolved volume */

                  std::string info_image = "";                                                   /* Image displayed for token on listing */
                  std::unordered_map<std::string /* Website */, std::string /* Url */> websites; /* Website */
                  std::unordered_map<std::string /* Website */, std::string /* Url */> socials;  /* Socials */
                  std::vector<std::string> labels;                                               /* Labels */
            };

            /* Searches for token on dex trackers */
            class token {

                public:
                  token(const trackers tracker, const std::string &mint_address, crypto::error::error &error) {
                        this->update(tracker, mint_address, error);
                        return;
                  }

                  token(const std::string &mint_address, const std::string &network_address, const std::string &network_symbol) {
                        const auto dex = std::make_shared<token_tracking>();
                        dex->base = true;
                        dex->network_symbol = network_symbol;
                        dex->mint_address = mint_address;
                        dex->network_address = network_address;
                        this->tracked.insert(std::make_pair("base", std::vector<std::shared_ptr<token_tracking>>{dex}));
                        return;
                  }

                  /* Change to specific token */
                  void change(const trackers tracker, const std::string &mint_address, crypto::error::error &error) {
                        this->tracked.clear();
                        this->update(tracker, mint_address, error);
                        return;
                  }

                  /* Update token information */
                  void update(const trackers tracker, const std::string &mint, crypto::error::error &error) {
                        switch (tracker) {
                              case trackers::dexscreener: {
                                    this->update_dexscreener(mint, error);
                                    break;
                              }
                              case trackers::geckoterminal: {
                                    this->update_geckoterminal(mint, error);
                                    break;
                              }
                              default: {
                                    break;
                              }
                        }
                        return;
                  }

                  /* Get token name */
                  std::string get_name() const {
                        return this->name;
                  }

                  /* Get token symbol */
                  std::string get_symbol() const {
                        return this->symbol;
                  }

                  /* Gets all dexes data raw */
                  std::unordered_map<std::string, std::vector<std::shared_ptr<token_tracking>>> get_trackers_raw() const {
                        return this->tracked;
                  }

                  /* Sees if token is valid */
                  bool valid_token() const {
                        return this->set_metadata;
                  }

                  /* Find dex based off dex_id and labels, if it is not indexed(just has mint and network) it will still fetch that. */
                  std::shared_ptr<token_tracking> find_dex(const std::string &tracker_id, const std::vector<std::string> &labels = {}) {
                        const auto iter = this->tracked.find(tracker_id);
                        if (iter != this->tracked.end()) {
                              for (const auto &i : iter->second) {
                                    auto found = true;
                                    for (const auto &s : labels) {
                                          if (std::find(i->labels.begin(), i->labels.end(), s) == i->labels.end()) {
                                                found = false;
                                                break;
                                          }
                                    }
                                    if (found || i->base) {
                                          return i;
                                    }
                              }
                        }
                        return nullptr;
                  }

                  /* Find dex based off dex_id, labels, and chain_id */
                  std::shared_ptr<token_tracking> find_dex(const std::string &dex_id, const std::string chain_id, const std::vector<std::string> &labels = {}) {
                        const auto iter = this->tracked.find(dex_id);
                        if (iter != this->tracked.end()) {
                              for (const auto &i : iter->second) {
                                    auto found = true;
                                    if (i->chain_id != chain_id) {
                                          continue;
                                    }
                                    for (const auto &s : labels) {
                                          if (std::find(i->labels.begin(), i->labels.end(), s) == i->labels.end()) {
                                                found = false;
                                                break;
                                          }
                                    }
                                    if (found || i->base) {
                                          return i;
                                    }
                              }
                        }
                        return nullptr;
                  }

                  void debug() {

                        for (const auto &i : this->tracked) {
                              std::printf("[%s]:\n", i.first.c_str());
                              for (const auto &s : i.second) {
                                    std::printf("   * %s\n", s->chain_id.c_str());
                                    std::printf("   * %s\n", s->mint_address.c_str());
                                    std::printf("   * 0.%s\n", s->price_native.get_data().c_str());
                              }
                        }
                        return;
                  }

                private:
                  bool set_metadata = false; /* Set metadata? */
                  std::string name = "";     /* Token name */
                  std::string symbol = "";
                  std::unordered_map<std::string /* Exchange name */, std::vector<std::shared_ptr<token_tracking>> /* Dexes (Seperate by labels) */> tracked; /* Tracked token */

                  /* Trackers */
                  void update_dexscreener(const std::string &mint, crypto::error::error &error);                                  /* Dexscreener */
                  void update_geckoterminal(const std::string &mint, crypto::error::error &error); /* Gecko Terminal */
            };

      } // namespace token

      struct spl_token_data {
          crypto_decoded_pkey ca;
          crypto_lamports supply = 0u;
          std::string ipsf_data = "";
      };
      struct spl_token_ownership {
          crypto_decoded_pkey ata_owner;
          crypto_decoded_pkey ata;
          crypto_decoded_pkey token_account;
          spl_token_data t;
          crypto_lamports owned = 0u;
      };

      /* Wallet data */
      class wallet {

                public:

                  wallet(const std::string &public_key, const std::string &private_key, const char* const end_point)
                      : end_point(end_point) {

                        this->s_pkey = static_cast<crypto_encoded_key>(public_key);
                        this->s_skey = static_cast<crypto_encoded_key>(private_key);

                        auto d = base58_decode(reinterpret_cast<std::uint8_t*>(this->s_pkey.data()), this->s_pkey.size());
                        std::copy(d.data.begin(), d.data.begin() + d.size, this->pkey.begin());

                        d = base58_decode(reinterpret_cast<std::uint8_t*>(this->s_skey.data()), this->s_skey.size());
                        std::copy(d.data.begin(), d.data.begin() + d.size, this->skey.begin());

                        this->update();
                        return;
                  }
                  wallet(const std::string& public_key, const char* const end_point)
                      : end_point(end_point) {                
                      
                      this->watching = true;
                      this->s_pkey = static_cast<crypto_encoded_key>(public_key);

                      const auto d = base58_decode(reinterpret_cast<std::uint8_t*>(this->s_pkey.data()), this->s_pkey.size());
                      std::copy(d.data.begin(), d.data.begin() + d.size, this->pkey.begin());

                      this->update();
                      return;
                  }
                  wallet(const crypto_decoded_pkey& public_key) : pkey(public_key) {
                      this->watching = true;
                      this->load = false;
                      return;
                  }
#pragma region ed25519

                  /* Random ed25519 keypair */
                  void random_kp_ed25519();

                  /* Random ed25519 key but it is not encoded.  */
                  void random_ed25519();

                  /* Sign unsigned ed25519 message */
                  signed_tx sign_ed25519(crypto::error::error& error, const unsigned_tx& msg);
#pragma endregion

                  void update() {
                      
                        return;
                  }

                  /* Get base58 encoded public key */
                  __inline crypto_decoded_pkey get_public_key() const {
                        return this->pkey;
                  }
                  /* Get base58 encoded private key */
                  __inline crypto_decoded_skey get_private_key() const {
                        return this->skey;
                  }
                  /* Get base58 encoded public key */
                  __inline crypto_encoded_key get_public_key_str() const {
                      return this->s_pkey;
                  }
                  /* Get base58 encoded private key */
                  __inline crypto_encoded_key get_private_key_str() const {
                      return this->s_skey;
                  }
                  /* Get endpoint */
                  __inline std::string get_end_point() const {
                        return this->end_point;
                  }
                  /* Get SOLs */
                  __inline crypto_lamports get_sol() const {
                      return this->sol;
                  }
                  __inline bool loaded() const {
                      return this->load;
                  }

                private:
                  crypto_decoded_pkey pkey;                                                            /* (Decoded) Public key */
                  crypto_decoded_skey skey;                                                            /* (Decoded) Private Key */
                  crypto_encoded_key s_pkey;                                                           /* (Encoded) Public key */
                  crypto_encoded_key s_skey;                                                           /* (Encoded) Private key */
                  const char* end_point = "";                                                          /* End point */
                  std::vector<spl_token_ownership> spl_tokens;                                         /* Owned spl tokens */
                  bool watching = false;                                                               /* No known skey */
                  crypto_lamports sol;                                                                 /* Amount of SOL in wallet */
                  bool load = false;                                                                   /* Loaded in data */
      };

} // namespace crypto