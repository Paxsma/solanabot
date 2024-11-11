#pragma once
#include "../crypto.hpp"

namespace crypto::exchange {

     namespace trades {

            enum class resolution : std::uint16_t {
                 minute_1 = 1u,
                 minute_15 = 15u,
                 minute_30 = 30u,
                 hour_1 = 60u,
                 hour_2 = 120u,
                 hour_4 = 240u,
                 hour_6 = 360u,
                 hour_12 = 720u
           };

            struct candle {
                  time_t ts;
                  std::double_t candle_upper = 0;
                  std::double_t candle_lower = 0;
                  std::double_t high = 0;
                  std::double_t low = 0;
            };

            struct view {
                  std::vector<std::shared_ptr<candle>> candles;
                  resolution res = resolution::minute_1;
            };

      } // namespace trades

#pragma region DEX

      /* (DEX) https://station.jup.ag/api-v6 */
      namespace jupiter::V6 {

            /* Gets qoute, returns JSON (https://station.jup.ag/api-v6/get-quote) returns qoute. */
            std::shared_ptr<exchange_qoute> get_qoute(token::token &token, const std::double_t sell_amount, std::double_t slipage, error::error &error, const std::string &chain_id /* Chain ID */, const bool sell = false /* True for sell, false for buy. */, const std::vector<std::string> &labels = std::vector<std::string>() /* Labels */);

            /* Swap tokens (https://station.jup.ag/api-v6/post-swap) returns JSON string. */
            std::shared_ptr<exchange_swap> swap(const std::shared_ptr<exchange::exchange_qoute> &qoute, wallet &wall, error::error &error);

            /* Returns JSON of all tradable tokens (https://station.jup.ag/api-v6/get-tokens) */
            std::string all_tokens(error::error &error);

            /* See if token is tradable (https://station.jup.ag/api-v6/get-tokens) Returns boolean. */
            bool tradable(token::token &token, error::error &error, const std::string &chain_id /* Chain ID */, const std::vector<std::string> &labels = std::vector<std::string>() /* Labels */);

            namespace trading {

                enum class tradable : std::uint8_t {
                    bitcoin,
                    ethereum,
                    solana
                };

                trades::view get_trades(const tradable trade, const trades::resolution res, error::error &error, time_t start = 0, time_t end = 0);
            }
      } // namespace jupiter::V6

      /* (DEX) https://docs.kyberswap.com/kyberswap-solutions/kyberswap-aggregator/developer-guides/ */
      namespace kyberswap::V1 {

            /* Gets qoute, returns JSON (https://docs.kyberswap.com/kyberswap-solutions/kyberswap-aggregator/developer-guides/execute-a-swap-with-the-aggregator-api) returns qoute. */
            std::shared_ptr<exchange_qoute> get_qoute(token::token &token, const std::double_t sell_amount, error::error &error, const std::string &chain_id /* Chain ID */, const bool save_gas = false /* Save on gas(true), Max return(false)*/, const bool sell = false /* True for sell, false for buy. */, const std::vector<std::string> &labels = std::vector<std::string>() /* Labels */);

            /* Swap tokens (https://station.jup.ag/api-v6/post-swap) returns JSON string. */
            std::shared_ptr<exchange_swap> swap(const std::shared_ptr<exchange::exchange_qoute> &qoute, wallet &wall, error::error &error);

            /* See if token is tradable (https://station.jup.ag/api-v6/get-tokens) Returns boolean. */
            bool tradable(token::token &token, error::error &error, const std::string &chain_id /* Chain ID */, const std::vector<std::string> &labels = std::vector<std::string>() /* Labels */);

      } // namespace kyberswap::V1

      /* UNFINISHED */
      namespace uniswap {

            /* Gets qoute, returns json (https://docs.uniswap.org/sdk/v3/guides/swaps/quoting) returns qoute.*/
            std::shared_ptr<exchange_qoute> get_qoute(token::token &token, const std::double_t sell_amount, error::error &error, const std::string &chain_id, const bool sell, const std::vector<std::string> &labels = std::vector<std::string>() /* Labels */);

            /* Gets all tokens supported by uniswap (https://tokens.coingecko.com/uniswap/all.json) */
            // std::string all_tokens(error::error &error);

      } // namespace uniswap

      namespace sushiswap {

            /* Gets all tokens, Descriptive gets (https://api.sushi.com/price/v1/1) else gets (https://tokens.sushi.com/v0) */
            std::string all_tokens(error::error &error, const bool descriptive);

      } // namespace sushiswap

      namespace paraswap {

      }

      namespace raydium::V1 {

            /* Gets all tokens (https://api.raydium.io/v2/main/pairs) */
            std::string all_tokens(error::error &error);

            /* Gets qoute, returns JSON (https://transaction.raydium.io/v1/compute/swap-base-in) returns qoute. */
            std::shared_ptr<crypto::exchange::exchange_qoute> get_qoute(token::token &token, const std::double_t sell_amount, std::double_t slipage, error::error &error, const std::string &chain_id, const bool sell = false, const std::vector<std::string> &labels = std::vector<std::string>());

      } // namespace raydium::V2

      namespace photon {



      }

      namespace pump_fun {


      }

#pragma endregion

      #pragma region cex 
      namespace mexc {

          namespace trading {

                  enum class tradable : std::uint8_t {
                        bitcoin,
                        ethereum,
                        solana
                  };

                  trades::view get_trades(const tradable trade, const trades::resolution res, error::error &error, time_t start = 0, time_t end = 0);
          }

      }
      #pragma endregion
} // namespace crypto::exchange