#include "../exchanges.hpp"
#include "../../rpc/crypto/base64.hpp"

std::shared_ptr<crypto::exchange::exchange_qoute> crypto::exchange::jupiter::V6::get_qoute(token::token &token, const std::double_t sell_amount, std::double_t slipage, error::error &error, const std::string &chain_id, const bool sell, const std::vector<std::string> &labels) {

      /* Check valid */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      auto dex = token.find_dex("orca", chain_id, labels); /* Can use same info */

      /* Check dex */
      if (dex == nullptr && (dex = token.find_dex("jupiter", chain_id, labels)) == nullptr && (dex = token.find_dex("raydium", chain_id, labels)) == nullptr) {
            error.t = error::type::unsupported;
            return nullptr;
      }

      /* Input output */
      const auto &input = (sell) ? dex->mint_address : dex->network_address;
      const auto &output = (sell) ? dex->network_address : dex->mint_address;

      /* Buy amount */
      const auto amt = types::biguint(sell_amount, 9u);

      /* Slipage */
      const auto slip = types::biguint(slipage, 1u);

      /* Invoke */
      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://quote-api.jup.ag/v6/quote")));
      session.SetParameters(cpr::Parameters{
          {"inputMint", input},
          {"outputMint", output},
          {"amount", amt.get_whole_data()},
          {"slippageBps", slip.get_whole_data()}});
      const auto response = session.Get();
  
      /* Parse JSON */
      rapidjson::Document json;
      json.Parse(response.text.c_str());
      if (json.HasParseError()) {
            error.t = error::type::json_parse;
            return nullptr;
      }

      if (response.status_code != 200) {

            error.t = error::type::invalid_status;
            if (json.HasMember("error")) {
                  error.what_str = json["error"].GetString();
            }
            return nullptr;
      }

      const auto result = std::make_shared<exchange::exchange_qoute>();
      result->exchange = supported::jupiter;
      result->qoute = response.text;
      result->slipage = slip;

      /* Change */
      result->price_impact = types::biguint(json["priceImpactPct"].GetString());

      /* Input */
      result->input_address = json["inputMint"].GetString();
      result->input_amount = types::biguint(json["inAmount"].GetString());

      /* Output */
      result->output_address = json["outputMint"].GetString();
      result->output_amount = types::biguint(json["outAmount"].GetString());

      result->finalize();
      return result;
}

std::shared_ptr<crypto::exchange::exchange_swap> crypto::exchange::jupiter::V6::swap(const std::shared_ptr<exchange::exchange_qoute> &qoute, wallet &wall, error::error &error) {

      /* Invalid qoute */
      if (qoute->qoute.empty()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      /* Check qoute */
      if (qoute->exchange != supported::jupiter) {
            error.t = error::type::unsupported;
            return nullptr;
      }

      /* Invoke */
      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://quote-api.jup.ag/v6/swap")));
      session.SetHeader(cpr::Header{
          {"Content-Type", "application/json"},
          {"Accept", "application/json"}});
      std::string payload = "{\"wrapAndUnwrapSol\": true, \"prioritizationFeeLamports\": 1, \"asLegacyTransaction\": false, \"dynamicComputeUnitLimit\": true, \"userPublicKey\": \"" + wall.get_private_key_str() + "\", \"quoteResponse\": " + qoute->qoute + "}";
      session.SetBody(cpr::Body{payload});
      const auto response = session.Post();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
            return nullptr;
      }

      /* Parse */
      rapidjson::Document json;
      json.Parse(response.text.c_str());
      if (json.HasParseError()) {
            error.t = error::type::json_parse;
            return nullptr;
      }

      /* Retn */
      const auto result = std::make_shared<exchange::exchange_swap>();
      result->swap = response.text;
      result->transaction = json["swapTransaction"].GetString();

      const auto decoded = base64_decode(result->transaction);
      std::cout << "NEPTUNIA " << decoded.size() << std::endl;
      result->transaction_decoded = std::vector<std::uint8_t>(decoded.begin(), decoded.end());

      return result;
}

std::string crypto::exchange::jupiter::V6::all_tokens(error::error &error) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://quote-api.jup.ag/v6/tokens")));
      const auto response = session.Get();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
      }
      return response.text;
}

bool crypto::exchange::jupiter::V6::tradable(token::token &token, error::error &error, const std::string &chain_id, const std::vector<std::string> &labels) {

      /* Invalid qoute */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return false;
      }

      /* Invoke */
      const auto all = all_tokens(error);
      if (error.err()) {
            return false;
      }

      /* Parse */
      rapidjson::Document json;
      json.Parse(all.c_str());
      if (json.HasParseError()) {
            error.t = error::type::json_parse;
            return false;
      }

      /* Check dex */
      auto dex = token.find_dex("orca", chain_id, labels);
      if (dex == nullptr && (dex = token.find_dex("jupiter", chain_id, labels)) == nullptr && (dex = token.find_dex("raydium", chain_id, labels)) == nullptr) {
            error.t = error::type::unsupported;
            return false;
      }

      /* Search */
      for (const auto &t : json.GetArray()) {
            if (t.GetString() == dex->mint_address) {
                  return true;
            }
      }

      return false;
}

crypto::exchange::trades::view crypto::exchange::jupiter::V6::trading::get_trades(const tradable trade, const trades::resolution res, error::error &error, time_t start, time_t end) {

    trades::view result;

    std::string coin = "";
    switch (trade) {
        case tradable::ethereum:  {
              coin = "Crypto.ETH/USD";
              break;
        }
        case tradable::solana: {
              coin = "Crypto.SOL/USD";
              break;
        }
        case tradable::bitcoin:
        default: {
              coin = "Crypto.BTC/USD";
              break;
        }
    }

    const auto url = cpr::Url(std::string("https://benchmarks.pyth.network/v1/shims/tradingview/history"));
    time_t time = std::time(nullptr);

    for (auto i = 0u; i < INT_MAX; ++i) {

          cpr::Session session;
          session.SetUrl(url);
         //// session.SetParameters(cpr::Parameters {
           //   {"symbol", coin},
           //   {"from", static_cast<std::uint64_t>(time -= 1000u)},
           //   {"to", static_cast<std::uint64_t>(time)},
           //   {"resolution", static_cast<std::uint16_t>(res)}});

          const auto response = session.Get();
          if (response.status_code != 200) {
                error.t = error::type::invalid_status;
                error.what_str = response.text;
                break;
          }

          const auto json = nlohmann::json(response.text);
          if (json["s"] != "ok") {
                  error.t = error::type::invalid_status;
                  error.what_str = response.text;
                  break;
          }

          for (auto i = 0u; i < json["t"].size(); ++i) {

              auto ptr = std::make_shared<trades::candle>();
              ptr->ts = json["t"][i];
              
          }
    }

    return result;
}