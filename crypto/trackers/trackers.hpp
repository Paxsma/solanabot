#pragma once
#include <string>

namespace crypto::trackers {

      namespace dexscreener {

            /* Gets mint addresses when searched (https://api.dexscreener.com/latest/dex/search/?q=:q) */
            std::vector<std::string> search_mint(const std::vector<std::string> &search_terms, crypto::error::error &error);
     
      } // namespace dexscreener

      namespace geckoterminal {

            /* Get all avaliable coins (https://api.coingecko.com/api/v3/asset_platforms) */
            std::string all_coins(crypto::error::error &error);

            /* Gets all avaliable tokens (https://tokens.coingecko.com/{ID}/all.json) */
            std::string all_tokens(crypto::error::error &error);

            struct swap {

                  std::string id = "";

                  std::string timestamp = "";
                  std::string tx_hash = "";
                  std::string tx_from_address = "";
                  types::biguint from_token_amount = types::biguint("0");
                  types::biguint price_from = types::biguint("0");
                  types::biguint price_from_in_currency_token = types::biguint("0");
                  types::biguint price_from_in_usd = types::biguint("0");
                  types::biguint from_token_total_in_usd = types::biguint("0");
                  types::biguint to_token_amount = types::biguint("0");
                  types::biguint price_to = types::biguint("0");
                  types::biguint price_to_in_currency_token = types::biguint("0");
                  types::biguint price_to_in_usd = types::biguint("0");
                  types::biguint to_token_total_in_usd = types::biguint("0");

                  struct token {
                        std::uint32_t id = 0u;
                        std::string type = "";
                        std::string name = "";
                        std::string symbol = "";
                        std::string mint_address = "";
                  };
                  token from;
                  token to;
            };

            /* Gets most recent swaps with a token */
            std::vector<std::shared_ptr<swap>> recent_swaps(const std::string &pool, crypto::error::error &error);
            
      } // namespace geckoterminal

} // namespace crypto::trackers