#include "../exchanges.hpp"
#include <cryptopp/base64.h>

std::shared_ptr<crypto::exchange::exchange_qoute> crypto::exchange::kyberswap::V1::get_qoute(token::token &token, const std::double_t sell_amount, error::error &error, const std::string &chain_id, const bool save_gas, const bool sell, const std::vector<std::string> &labels) {

      /* Check valid */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      auto dex = token.find_dex("uniswap", chain_id, labels); /* Can use same info */

      /* Check dex */
      if (dex == nullptr && (dex = token.find_dex("kyberswap", chain_id, labels)) == nullptr) {
            error.t = error::type::unsupported;
            return nullptr;
      }

      /* Input output */
      const auto &input = (sell) ? dex->mint_address : dex->network_address;
      const auto &output = (sell) ? dex->network_address : dex->mint_address;

      /* Buy amount */
      const auto amt = types::biguint(sell_amount, 18u);

      /* Invoke */
      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://aggregator-api.kyberswap.com/linea/api/v1/routes")));
      session.SetParameters(cpr::Parameters{
          {"tokenIn", input},
          {"tokenOut", output},
          {"amountIn", amt.get_whole_data()},
          {"saveGas", ((save_gas) ? "true" : "false")},
          {"gasInclude", "true"}});
      const auto response = session.Get();

      /* Parse JSON */
      rapidjson::Document json;
      json.Parse(response.text.c_str());
      if (json.HasParseError()) {
            error.t = error::type::json_parse;
            return nullptr;
      }

      if (response.status_code != 200) {
            error.t = error::type::unkown;
            return nullptr;
      }

      const auto result = std::make_shared<exchange::exchange_qoute>();
      result->exchange = supported::kyberswap;
      result->qoute = response.text;

      const auto &data = json["data"]["routeSummary"];

      /* Input */
      result->input_address = data["tokenIn"].GetString();
      result->input_amount = types::biguint(data["amountIn"].GetString());

      /* Output */
      result->output_address = data["tokenOut"].GetString();
      result->output_amount = types::biguint(data["amountOut"].GetString());

      /* Gas */
      result->swap_fee = types::biguint(data["gas"].GetString()) + data["extraFee"]["feeAmount"].GetString();

      result->finalize();

      return result;
}

std::shared_ptr<crypto::exchange::exchange_swap> crypto::exchange::kyberswap::V1::swap(const std::shared_ptr<exchange::exchange_qoute> &qoute, wallet &wall, error::error &error) {

      /* Invalid qoute */
      if (qoute->qoute.empty()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      /* Check qoute */
      if (qoute->exchange != exchange::supported::kyberswap) {
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

      std::string decoded;
      CryptoPP::StringSource(result->transaction, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)));
      result->transaction_decoded = std::vector<uint8_t>(decoded.begin(), decoded.end());

      return result;
}

bool crypto::exchange::kyberswap::V1::tradable(token::token &token, error::error &error, const std::string &chain_id, const std::vector<std::string> &labels) {

      /* Invalid qoute */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return false;
      }

      /* Invoke */
      //  const auto all = all_tokens(error);
      //  if (error == error_type::invalid_status) {
      //        return false;
      //  }
      //
      //  /* Parse */
      //  rapidjson::Document json;
      //  json.Parse(all.c_str());
      //  if (json.HasParseError()) {
      //        error = error_type::json_parse;
      //        return false;
      //  }
      //
      //  const auto dex = token.find_dex("raydium", chain_id, labels);
      //
      //  /* Search */
      //  for (const auto &t : json.GetArray()) {
      //        if (t.GetString() == dex->mint_address) {
      //              return true;
      //        }
      //  }
      //
      //  return false;
}