#pragma once
#include "../crypto.hpp"

/*
    All default encodings are in base64
*/

/* Default values */
#define rpc_commitment "confirmed"         /* Default commitment */
#define rpc_encoding "jsonParsed"        /* Default json encoding */
#define rpc_encoding_crypt "base58"       /* Default encoding */
#define rpc_encoding_crypt_nodep "base64" /* Default encoding, not depricated for most rpc functions */
#define rpc_max_blocks_transaction 152u  /* Maximum number of transactions for block */
#define rpc_mincontextslot "0"
#define rpc_maxretries "5"
#define rpc_txlimit "35"

/* 
(Solana) https://solana.com/docs/rpc/http 
(Developer Git) https://github.com/solana-foundation/developer-content/tree/main
*/
namespace crypto::api::solana {

      /* Returns all information associated with the account of provided Pubkey (https://solana.com/docs/rpc/http/getaccountinfo) */
      std::string get_account_info(const std::string &account, crypto::wallet &wallet, crypto::error::error &error);

      /*  Returns the lamport balance of the account of provided Pubkey (https://solana.com/docs/rpc/http/getbalance) */
      std::string get_balance(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns identity and transaction information about a confirmed block in the (https://solana.com/docs/rpc/http/getblock) */
      std::string get_block(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns commitment for particular block (https://solana.com/docs/rpc/http/getblockcommitment) */
      std::string get_block_commitment(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current block height of the node (https://solana.com/docs/rpc/http/getblockheight) */
      std::string get_block_height(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns recent block production information from the current or previous epoch. (https://solana.com/docs/rpc/http/getblockproduction) */
      std::string get_block_production(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns a list of confirmed blocks between two slots (https://solana.com/docs/rpc/http/getblocks) */
      std::string get_blocks(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns a list of confirmed blocks starting at the given slot (https://solana.com/docs/rpc/http/getblockswithlimit) */
      std::string get_blocks_with_limit(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the estimated production time of a block. (https://solana.com/docs/rpc/http/getblocktime) */
      std::string get_block_time(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns information about all the nodes participating in the cluster (https://solana.com/docs/rpc/http/getclusternodes) */
      std::string get_cluster_nodes(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns information about the current epoch (https://solana.com/docs/rpc/http/getepochinfo) */
      std::string get_epoch_info(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the epoch schedule information from this cluster's genesis config (https://solana.com/docs/rpc/http/getepochschedule) */
      std::string get_epoch_schedule(crypto::wallet &wallet, crypto::error::error &error);

      /* Get the fee the network will charge for a particular Message (https://solana.com/docs/rpc/http/getfeeformessage) */
      std::string get_fee_for_message(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the slot of the lowest confirmed block that has not been purged from the (https://solana.com/docs/rpc/http/getfirstavailableblock) */
      std::string get_first_available_block(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the genesis hash (https://solana.com/docs/rpc/http/getgenesishash) */
      std::string get_genesis_hash(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current health of the node. A healthy node is one that is within (https://solana.com/docs/rpc/http/gethealth) */
      std::string get_health(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the highest slot information that the node has snapshots for. (https://solana.com/docs/rpc/http/gethighestsnapshotslot) */
      std::string get_highest_snapshot_slot(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the identity pubkey for the current node (https://solana.com/docs/rpc/http/getidentity) */
      std::string get_identity(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current inflation governor (https://solana.com/docs/rpc/http/getinflationgovernor) */
      std::string get_inflation_governor(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the specific inflation values for the current epoch (https://solana.com/docs/rpc/http/getinflationrate) */
      std::string get_inflation_rate(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the inflation / staking reward for a list of addresses for an epoch (https://solana.com/docs/rpc/http/getinflationreward) */
      std::string get_inflation_reward(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the 20 largest accounts, by lamport balance (results may be cached up to (https://solana.com/docs/rpc/http/getlargestaccounts) */
      std::string get_largest_accounts(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the latest blockhash (https://solana.com/docs/rpc/http/getlatestblockhash) */
      std::string get_latest_blockhash(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the leader schedule for an epoch (https://solana.com/docs/rpc/http/getleaderschedule) */
      std::string get_leader_schedule(crypto::wallet &wallet, crypto::error::error &error);

      /* Get the max slot seen from retransmit stage. (https://solana.com/docs/rpc/http/getmaxretransmitslot) */
      std::string get_max_retransmit_slot(crypto::wallet &wallet, crypto::error::error &error);

      /* Get the max slot seen from after shred insert. (https://solana.com/docs/rpc/http/getmaxshredinsertslot) */
      std::string get_max_shred_insert_slot(crypto::wallet &wallet, crypto::error::error &error);

      /*  Returns minimum balance required to make account rent exempt. (https://solana.com/docs/rpc/http/getminimumbalanceforrentexemption) */
      std::string get_minimum_balance_for_rent_exemption(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the account information for a list of Pubkeys. (https://solana.com/docs/rpc/http/getmultipleaccounts) */
      std::string get_multiple_accounts(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns all accounts owned by the provided program Pubkey (https://solana.com/docs/rpc/http/getprogramaccounts) */
      std::string get_program_accounts(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns a list of recent performance samples, in reverse slot order. Performance (https://solana.com/docs/rpc/http/getrecentperformancesamples) */
      std::string get_recent_performance_samples(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns a list of prioritization fees from recent blocks. (https://solana.com/docs/rpc/http/getrecentprioritizationfees) */
      std::string get_recent_prioritization_fees(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns signatures for confirmed transactions that include the given address in their accountKeys list. Returns signatures backwards in time from the provided signature or most recent confirmed block (https://solana.com/docs/rpc/http/getsignaturesforaddress) */
      std::string get_signatures_for_address(const std::string& address, crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the statuses of a list of signatures. Each signature must be a (https://solana.com/docs/rpc/http/getsignaturestatuses) */
      std::string get_signature_statuses(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the slot that has reached the (https://solana.com/docs/rpc/http/getslot) */
      std::string get_slot(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current slot leader (https://solana.com/docs/rpc/http/getslotleader) */
      std::string get_slot_leader(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the slot leaders for a given slot range (https://solana.com/docs/rpc/http/getslotleaders) */
      std::string get_slot_leaders(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns epoch activation information for a stake account (https://solana.com/docs/rpc/http/getstakeactivation) */
      std::string get_stake_activation(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the stake minimum delegation, in lamports. (https://solana.com/docs/rpc/http/getstakeminimumdelegation) */
      std::string get_stake_minimum_delegation(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns information about the current supply. (https://solana.com/docs/rpc/http/getsupply) */
      std::string get_supply(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the token balance of an SPL Token account. (https://solana.com/docs/rpc/http/gettokenaccountbalance) */
      std::string get_token_account_balance(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns all SPL Token accounts by approved Delegate. (https://solana.com/docs/rpc/http/gettokenaccountsbydelegate) */
      std::string get_token_accounts_by_delegate(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the token balance of an SPL Token account (https://solana.com/docs/rpc/http/gettokenaccountbalance) */
      std::string get_token_accounts_by_owner(const std::string &mint, crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the 20 largest accounts of a particular SPL Token type. (https://solana.com/docs/rpc/http/gettokenlargestaccounts) */
      std::string get_token_largest_accounts(const std::string &mint, crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the total supply of an SPL Token type. (https://solana.com/docs/rpc/http/gettokensupply) */
      std::string get_token_supply(const std::string &mint_pub_key, crypto::wallet &wallet, crypto::error::error &error);

      /*  Returns transaction details for a confirmed transaction (https://solana.com/docs/rpc/http/gettransaction) */
      std::string get_transaction(const std::string &signature, crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current Transaction count from the ledger (https://solana.com/docs/rpc/http/gettransactioncount) */
      std::string get_transaction_count(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the current Solana version running on the node (https://solana.com/docs/rpc/http/getversion) */
      std::string get_version(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the account info and associated stake for all the voting accounts in the (https://solana.com/docs/rpc/http/getvoteaccounts) */
      std::string get_vote_accounts(crypto::wallet &wallet, crypto::error::error &error);

      /* Solana nodes accept HTTP requests using the (https://solana.com/docs/rpc/http/index) */
      std::string index(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns whether a blockhash is still valid or not (https://solana.com/docs/rpc/http/isblockhashvalid) */
      std::string is_blockhash_valid(crypto::wallet &wallet, crypto::error::error &error);

      /* Returns the lowest slot that the node has information about in its ledger. (https://solana.com/docs/rpc/http/minimumledgerslot) */
      std::string minimum_ledger_slot(crypto::wallet &wallet, crypto::error::error &error);

      /* Requests an airdrop of lamports to a Pubkey (https://solana.com/docs/rpc/http/requestairdrop) */
      std::string request_airdrop(crypto::wallet &wallet, crypto::error::error &error);

      /* Submits a signed transaction to the cluster for processing. (https://solana.com/docs/rpc/http/sendtransaction) */
      std::string send_transaction(const std::string& signed_tx, crypto::wallet &wallet, crypto::error::error &error);

      /*  Simulate sending a transaction (https://solana.com/docs/rpc/http/simulatetransaction) */
      std::string simulate_transaction(const std::string& signed_tx, crypto::wallet &wallet, crypto::error::error &error);

} // namespace crypto::api::solana
