#include "serializer.hpp"
#include "../rpc/crypto/base58.hpp"

namespace crypto::solana::deserializer {

    crypto::solana::token_mint_data mint_data(const std::vector<std::uint8_t>& data) {

        std::size_t offset = 0u;
        token_mint_data result = { 0 };
        if (!data.empty()) {

            const auto ptr = data.data();

            /* Deserialize mintAuthorityOption as u32 */
            read(&result.mint_authority_option, sizeof(std::uint32_t), ptr, offset);

            /* Deserialize mintAuthority as publicKey */
            read(result.mint_authority.data(), result.mint_authority.size(), ptr, offset);

            /* Deserialize supply as u64 */
            read(&result.supply, sizeof(std::uint64_t), ptr, offset);

            /* Deserialize decimals as u8 */
            read(&result.decimals, sizeof(std::uint8_t), ptr, offset);

            /* Deserialize isInitialized as bool */
            read(&result.is_initialized, sizeof(bool), ptr, offset);

            /* Deserialize freezeAuthorityOption as u32 */
            read(&result.freeze_authority_option, sizeof(std::uint32_t), ptr, offset);

            /* Deserialize freezeAuthority as publicKey */
            read(result.freeze_authority.data(), result.freeze_authority.size(), ptr, offset);
        }
        return result;
    }

    crypto::solana::versioned_transaction transaction(const signed_tx& data) {

        std::size_t offset = 0u;
        versioned_transaction result;
        if (!data.empty()) {

            /* Signatures */
            std::uint8_t signature_amount = 0u;
            readt_tx(signature_amount, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < signature_amount; ++i) {
                crypto_decoded_signature key = { 0 };
                read_tx(&key, sizeof(key), data, offset);
                result.signatures.emplace_back(key);
            }
           
            /* Header data */
            readt_tx(result.header.version, data, std::uint8_t, offset);
            readt_tx(result.header.num_required_signatures, data, std::uint8_t, offset);
            readt_tx(result.header.num_readonly_signed_accounts, data, std::uint8_t, offset);
            readt_tx(result.header.num_readonly_unsigned_accounts, data, std::uint8_t, offset);

            /* Accounts */
            std::uint8_t accounts_keys = 0u;
            readt_tx(accounts_keys, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < accounts_keys; ++i) {
                crypto_decoded_pkey key = { 0 };
                read_tx(&key, sizeof(key), data, offset);
                result.header.account_keys.emplace_back(key);
            }

            /* Blockhash */
            crypto_decoded_pkey recent_hash = { 0 };
            read_tx(&recent_hash, sizeof(recent_hash), data, offset);
            result.header.recent_blockhash = recent_hash;

            /* Compiled instruction */
            std::uint8_t instructions = 0u;
            readt_tx(instructions, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < instructions; ++i) {

                transaction::compiled_instruction inst;
                readt_tx(inst.program_id_index, data, std::uint8_t, offset);

                /* Accounts */
                std::uint8_t accounts = 0u;
                readt_tx(accounts, data, std::uint8_t, offset);
                for (std::uint8_t j = 0u; j < accounts; ++j) {
                    std::uint8_t account = 0u;
                    readt_tx(account, data, std::uint8_t, offset);
                    inst.accounts.emplace_back(account);
                }

                /* Data */
                std::uint8_t datas = 0u;
                readt_tx(datas, data, std::uint8_t, offset);
                for (std::uint8_t j = 0u; j < datas; ++j) {
                    std::uint8_t x = 0u;
                    readt_tx(x, data, std::uint8_t, offset);
                    inst.data.emplace_back(x);
                }

                result.header.instructions.emplace_back(inst);
            }

            /* Address lookup table */
            std::uint8_t lookups = 0u;
            readt_tx(lookups, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < lookups; ++i) {

                transaction::message_address_table_lookup s;

                /* Account Key */
                crypto_decoded_pkey key = { 0 };
                read_tx(&key, sizeof(key), data, offset);
                s.account_key = key;

                /* Writable indexes */
                std::uint8_t w_indxs = 0u;
                readt_tx(w_indxs, data, std::uint8_t, offset);
                for (std::uint8_t i = 0u; i < w_indxs; ++i) {
                    std::uint8_t idx = 0u;
                    readt_tx(idx, data, std::uint8_t, offset);
                    s.writable_indexes.emplace_back(idx);
                }

                /* Readonly indexes */
                std::uint8_t r_indxs = 0u;
                readt_tx(r_indxs, data, std::uint8_t, offset);
                for (std::uint8_t i = 0u; i < r_indxs; ++i) {
                    std::uint8_t idx = 0u;
                    readt_tx(idx, data, std::uint8_t, offset);
                    s.readonly_indexes.emplace_back(idx);
                }

                result.header.address_table_lookup.emplace_back(s);
            }
        }

        return result;
    }

    crypto::solana::versioned_transaction transaction(const unsigned_tx& data) {

        std::size_t offset = 0u;
        versioned_transaction result;
        if (!data.empty()) {

            /* Header data */
            readt_tx(result.header.version, data, std::uint8_t, offset);
            readt_tx(result.header.num_required_signatures, data, std::uint8_t, offset);
            readt_tx(result.header.num_readonly_signed_accounts, data, std::uint8_t, offset);
            readt_tx(result.header.num_readonly_unsigned_accounts, data, std::uint8_t, offset);

            /* Accounts */
            std::uint8_t accounts_keys = 0u;
            readt_tx(accounts_keys, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < accounts_keys; ++i) {
                crypto_decoded_pkey key = { 0 };
                read_tx(&key, sizeof(key), data, offset);
                result.header.account_keys.emplace_back(key);
            }

            /* Blockhash */
            crypto_decoded_pkey recent_hash = { 0 };
            read_tx(&recent_hash, sizeof(recent_hash), data, offset);
            result.header.recent_blockhash = recent_hash;

            /* Compiled instruction */
            std::uint8_t instructions = 0u;
            readt_tx(instructions, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < instructions; ++i) {

                transaction::compiled_instruction inst;
                readt_tx(inst.program_id_index, data, std::uint8_t, offset);
              

                /* Accounts */
                std::uint8_t accounts = 0u;
                readt_tx(accounts, data, std::uint8_t, offset);
                for (std::uint8_t j = 0u; j < accounts; ++j) {
                    std::uint8_t account = 0u;
                    readt_tx(account, data, std::uint8_t, offset);
                    inst.accounts.emplace_back(account);
                }

                /* Data */
                std::uint8_t datas = 0u;
                readt_tx(datas, data, std::uint8_t, offset);
                for (std::uint8_t j = 0u; j < datas; ++j) {
                    std::uint8_t x = 0u;
                    readt_tx(x, data, std::uint8_t, offset);
                    inst.data.emplace_back(x);
                }

                result.header.instructions.emplace_back(inst);
            }

            /* Address lookup table */
            std::uint8_t lookups = 0u;
            readt_tx(lookups, data, std::uint8_t, offset);
            for (std::uint8_t i = 0u; i < lookups; ++i) {

                transaction::message_address_table_lookup s;

                /* Account Key */
                crypto_decoded_pkey key = { 0 };
                read_tx(&key, sizeof(key), data, offset);
                s.account_key = key;

                /* Writable indexes */
                std::uint8_t w_indxs = 0u;
                readt_tx(w_indxs, data, std::uint8_t, offset);
                for (std::uint8_t i = 0u; i < w_indxs; ++i) {
                    std::uint8_t idx = 0u;
                    readt_tx(idx, data, std::uint8_t, offset);
                    s.writable_indexes.emplace_back(idx);
                }

                /* Readonly indexes */
                std::uint8_t r_indxs = 0u;
                readt_tx(r_indxs, data, std::uint8_t, offset);
                for (std::uint8_t i = 0u; i < r_indxs; ++i) {
                    std::uint8_t idx = 0u;
                    readt_tx(idx, data, std::uint8_t, offset);
                    s.readonly_indexes.emplace_back(idx);
                }

                result.header.address_table_lookup.emplace_back(s);
            }
        }

        return result;
    }
}


namespace crypto::solana::serializer {

    unsigned_tx unsigned_transaction(const versioned_transaction& transaction) {

        unsigned_tx result;

        /* Header data */
        result.emplace_back(transaction.header.version);
        result.emplace_back(transaction.header.num_required_signatures);
        result.emplace_back(transaction.header.num_readonly_signed_accounts);
        result.emplace_back(transaction.header.num_readonly_unsigned_accounts);

        /* Accounts */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.account_keys.size()));
        for (const auto& i : transaction.header.account_keys) {
            result.emplace_back(i.begin(), i.end());
        }

        /* Blockhash */
        result.emplace_back(transaction.header.recent_blockhash.begin(), transaction.header.recent_blockhash.end());

        /* Compiled instructions */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.instructions.size()));
        for (const auto& i : transaction.header.instructions) {

            /* Program ID */
            result.emplace_back(i.program_id_index);

            /* Accounts */
            result.emplace_back(static_cast<std::uint8_t>(i.accounts.size));
            for (auto a = 0u; a < i.accounts.size; ++a) {
                result.emplace_back(i.accounts[a]);
            }

            /* Data */
            result.emplace_back(static_cast<std::uint8_t>(i.data.size()));
            result.emplace_back(i.data.begin(), i.data.end());
        }

        /* Address lookup table */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.address_table_lookup.size()));
        for (const auto& i : transaction.header.address_table_lookup) {

            /* Account key */
            result.emplace_back(i.account_key.begin(), i.account_key.end());

            /* Writable indexes */
            result.emplace_back(static_cast<std::uint8_t>(i.writable_indexes.size()));
            result.emplace_back(i.writable_indexes.begin(), i.writable_indexes.end());

            /* Readonly indexes */
            result.emplace_back(static_cast<std::uint8_t>(i.readonly_indexes.size()));
            result.emplace_back(i.readonly_indexes.begin(), i.readonly_indexes.end());
        }

        return result;
    }
 
    signed_tx signed_transaction(const versioned_transaction& transaction) {

        signed_tx result;

        /* Signatures */
        result.emplace_back(static_cast<std::uint8_t>(transaction.signatures.size()));
        for (const auto& i : transaction.signatures) {
            result.emplace_back(i.begin(), i.end());
        }

        /* Header data */
        result.emplace_back(transaction.header.version);
        result.emplace_back(transaction.header.num_required_signatures);
        result.emplace_back(transaction.header.num_readonly_signed_accounts);
        result.emplace_back(transaction.header.num_readonly_unsigned_accounts);

        /* Accounts */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.account_keys.size()));
        for (const auto& i : transaction.header.account_keys) {
            result.emplace_back(i.begin(), i.end());
        }

        /* Blockhash */
        result.emplace_back(transaction.header.recent_blockhash.begin(), transaction.header.recent_blockhash.end());

        /* Compiled instructions */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.instructions.size()));
        for (const auto& i : transaction.header.instructions) {

            /* Program ID */
            result.emplace_back(i.program_id_index);

            /* Accounts */
            result.emplace_back(static_cast<std::uint8_t>(i.accounts.size));
            for (auto a = 0u; a < i.accounts.size; ++a) {
                result.emplace_back(i.accounts[a]);
            }
   
            /* Data */
            result.emplace_back(static_cast<std::uint8_t>(i.data.size()));
            result.emplace_back(i.data.begin(), i.data.end());
        }

        /* Address lookup table */
        result.emplace_back(static_cast<std::uint8_t>(transaction.header.address_table_lookup.size()));
        for (const auto& i : transaction.header.address_table_lookup) {

            /* Account key */
            result.emplace_back(i.account_key.begin(), i.account_key.end());

            /* Writable indexes */
            result.emplace_back(static_cast<std::uint8_t>(i.writable_indexes.size()));
            result.emplace_back(i.writable_indexes.begin(), i.writable_indexes.end());

            /* Readonly indexes */
            result.emplace_back(static_cast<std::uint8_t>(i.readonly_indexes.size()));
            result.emplace_back(i.readonly_indexes.begin(), i.readonly_indexes.end());
        }

        return result;
    }

}