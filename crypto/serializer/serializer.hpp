#pragma once
#include "../crypto.hpp"
#include "../rpc/rpc.hpp"
#include "../rpc/crypto/base58.hpp"

namespace crypto::solana {
     
      namespace transaction {

            struct message_address_table_lookup {
                 crypto_decoded_pkey account_key;
                 std::vector<std::uint8_t> writable_indexes;
                 std::vector<std::uint8_t> readonly_indexes;
            };
            struct compiled_instruction {
                  std::uint8_t program_id_index = 0u;
                  std::vector<std::uint8_t> data;
                  varray<MAX_ACCOUNT_IDX_INST> accounts;
                  program::disassembly dism;
            };
            struct message {

                  std::uint8_t version = 0u;
                  std::uint8_t num_required_signatures = 0u;
                  std::uint8_t num_readonly_signed_accounts = 0u;
                  std::uint8_t num_readonly_unsigned_accounts = 0u;
                  crypto_decoded_pkey recent_blockhash;
                  std::vector<crypto_decoded_pkey> account_keys;
                  std::vector<compiled_instruction> instructions;
                  std::vector<message_address_table_lookup> address_table_lookup;

                  /* Append account key for easy lookups */
                  template<bool non_program = false, bool unsigned_account = true>
                  void append_account_key(const crypto_decoded_pkey& key) {
                      if (std::find(this->account_keys.begin(), this->account_keys.end(), key) != this->account_keys.end()) {
                          return;
                      }
                      if (non_program) {
                          if (unsigned_account) {
                              ++this->num_readonly_unsigned_accounts;
                          }
                          else {
                              ++this->num_readonly_signed_accounts;
                          }
                      }
                      this->account_keys.emplace_back(key);
                      return;
                  }

                  /* Append account key for easy lookups string */
                  template<bool non_program = false, bool unsigned_account = true>
                  void append_account_key(const std::string& key) {
                      if (std::find(this->account_keys.begin(), this->account_keys.end(), key) != this->account_keys.end()) {
                          return;
                      }
                      if (non_program) {
                          if (unsigned_account) {
                              ++this->num_readonly_unsigned_accounts;
                          }
                          else {
                              ++this->num_readonly_signed_accounts;
                          }
                      }
                      this->account_keys.emplace_back(base58_decode(key));
                      return;
                  }

                  /* Index account key */
                  __inline std::uint8_t idx_account_key(const crypto_decoded_pkey& key) {
                      return static_cast<std::uint8_t>(std::distance(this->account_keys.begin(), std::find(this->account_keys.begin(), this->account_keys.end(), key)));
                  }
            };

      } // namespace transaction

      struct versioned_transaction {
            std::vector<crypto_decoded_signature> signatures; /* Need Base58 */
            transaction::message header;

            /* Update to recent blockhash */
            void update(wallet& wallet, error::error& error) {
              
                const auto data = api::solana::get_latest_blockhash(wallet, error);
                if (error.err()) {
                    return;
                }
                rapidjson::Document document;
                document.Parse(data.data());
                const auto str = document["result"]["value"]["blockhash"].GetString();
                const auto hash = base58_decode(reinterpret_cast<const std::uint8_t*>(str), std::strlen(str));
                if (!hash.copy(this->header.recent_blockhash.data(), sizeof(this->header.recent_blockhash))) {
                    error.t = error::type::wrong_capacity;
                }
                return;
            }
      };

      struct token_mint_data {
            std::uint32_t mint_authority_option;
            std::array<std::uint8_t, 32u> mint_authority;
            std::uint64_t supply;
            std::uint8_t decimals;
            bool is_initialized;
            std::uint32_t freeze_authority_option;
            std::array<std::uint8_t, 32u> freeze_authority;
      };

      namespace deserializer {

          /* Deserialize data from mint data. */
          token_mint_data mint_data(const std::vector<std::uint8_t>& data);

          /* Deserialize signed transaction. */
          versioned_transaction transaction(const signed_tx& data);

          /* Deserialize unsigned transaction */
          versioned_transaction transaction(const unsigned_tx& data);
      }

      namespace serializer {

          /* Serialize unsigned transaction */
          unsigned_tx unsigned_transaction(const versioned_transaction& transaction);

          /* Serialize signed transaction */
          signed_tx signed_transaction(const versioned_transaction& transaction);

      }
}