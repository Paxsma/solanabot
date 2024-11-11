#include "rpc.hpp"
#include "crypto/base58.hpp"
#include "json_rpc.hpp"

std::string crypto::api::solana::get_account_info(const std::string &account, crypto::wallet &wallet, crypto::error::error &error) {
    
      const auto json = payload("getAccountInfo",
          payload_v(account) payload_sep payload_contain(payload_cobject("encoding", rpc_encoding_crypt_nodep)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_balance(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("getBalance",
          payload_v(wallet.get_private_key_str()) payload_sep payload_contain(payload_cobject("commitment", rpc_commitment) payload_sep payload_ciobject("minContextSlot", rpc_mincontextslot)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_block(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_block_commitment(crypto::wallet &wallet, crypto::error::error &error) {


      return "";
}

std::string crypto::api::solana::get_block_height(crypto::wallet &wallet, crypto::error::error &error) {
      
      const auto json = cpayload("getBlockHeight",
          payload_contain(payload_cobject("encoding", rpc_encoding_crypt_nodep)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_block_production(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_blocks(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_blocks_with_limit(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_block_time(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_cluster_nodes(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_epoch_info(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_epoch_schedule(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_fee_for_message(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_first_available_block(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload_method("getFirstAvailableBlock");
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_genesis_hash(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload_method("getGenesisHash");
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_health(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload_method("getHealth");
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_highest_snapshot_slot(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload_method("getHighestSnapshotSlot");
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_identity(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_inflation_governor(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_inflation_rate(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_inflation_reward(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_largest_accounts(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_latest_blockhash(crypto::wallet &wallet, crypto::error::error &error) {
   
      const auto json = cpayload("getLatestBlockhash",
         payload_contain(payload_cobject("commitment", rpc_commitment) payload_sep payload_ciobject("minContextSlot", rpc_mincontextslot)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_leader_schedule(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_max_retransmit_slot(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_max_shred_insert_slot(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_minimum_balance_for_rent_exemption(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_multiple_accounts(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_program_accounts(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_recent_performance_samples(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_recent_prioritization_fees(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}
std::string crypto::api::solana::get_signatures_for_address(const std::string& address, crypto::wallet &wallet, crypto::error::error &error) {
    
    const auto json = payload("getSignaturesForAddress",
        payload_v(address) payload_sep payload_contain(payload_cobject("encoding", rpc_encoding) payload_sep payload_ciobject("limit", rpc_txlimit)));
    return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_signature_statuses(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_slot(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_slot_leader(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_slot_leaders(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_stake_activation(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_stake_minimum_delegation(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_supply(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_token_account_balance(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_token_accounts_by_delegate(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_token_accounts_by_owner(const std::string &mint, crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("getTokenAccountsByOwner",
          payload_v(wallet.get_public_key_str()) payload_sep payload_contain(payload_object("mint", mint) payload_sep payload_cobject("encoding", rpc_encoding)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_token_largest_accounts(const std::string &mint, crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("getTokenLargestAccounts",
          payload_v(mint) payload_sep payload_contain(payload_cobject("commitment", rpc_commitment)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_token_supply(const std::string &mint_pub_key, crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("getTokenSupply",
          payload_v(mint_pub_key) payload_sep payload_contain(payload_cobject("commitment", rpc_commitment)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_transaction(const std::string &signature, crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("getTransaction", 
          payload_v(signature) payload_sep payload_contain(payload_cobject("commitment", rpc_commitment) payload_sep payload_cobject("encoding", rpc_encoding) payload_sep payload_ciobject("maxSupportedTransactionVersion", "0")));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_transaction_count(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::get_version(crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload_method("getVersion");
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::get_vote_accounts(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::index(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::is_blockhash_valid(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::minimum_ledger_slot(crypto::wallet &wallet, crypto::error::error &error) {

      return "";
}

std::string crypto::api::solana::request_airdrop(crypto::wallet &wallet, crypto::error::error &error) {


      return "";
}

std::string crypto::api::solana::send_transaction(const std::string& signed_tx, crypto::wallet &wallet, crypto::error::error &error) {

    const auto json = payload("sendTransaction", 
        payload_v(signed_tx) payload_sep payload_contain(payload_ciobject("maxRetries", rpc_maxretries) payload_sep payload_cobject("encoding", rpc_encoding_crypt_nodep)));
    return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}

std::string crypto::api::solana::simulate_transaction(const std::string& signed_tx, crypto::wallet &wallet, crypto::error::error &error) {

      const auto json = payload("simulateTransaction", 
          payload_v(signed_tx) payload_sep payload_contain(payload_cobject("commitment", rpc_commitment) payload_sep payload_cobject("encoding", rpc_encoding_crypt_nodep)));
      return rpc::v2::call<rpc::v2::protocol::post>(wallet.get_end_point(), error, json);
}