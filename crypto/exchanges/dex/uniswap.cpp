#include "../exchanges.hpp"

std::shared_ptr<crypto::exchange::exchange_qoute> crypto::exchange::uniswap::get_qoute(token::token &token, const std::double_t sell_amount, error::error &error, const std::string &chain_id, const bool sell, const std::vector<std::string> &labels) {

      /* Check valid */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      auto dex = token.find_dex("uniswap", chain_id, labels); /* Can use same info */

      /* Check dex */
      if (dex == nullptr) {
            error.t = error::type::unsupported;
            return nullptr;
      }

      /* Input output */
      const auto &input = (sell) ? dex->mint_address : dex->network_address;
      const auto &output = (sell) ? dex->network_address : dex->mint_address;

      /* Buy amount */
      const auto amt = types::biguint(sell_amount, 18u);

      const auto body = R"(
  {"tokenInChainId":1,
  "tokenIn" : ")" + input +
                        R"(", "tokenOutChainId" : 1,
  "tokenOut" : ")" + output +
                        R"(", "amount" : ")" + amt.get_whole_data() + R"(", "sendPortionEnabled" : true,
  "type" : "EXACT_INPUT",
  "intent" : "quote",
  "configs" : [ {"useSyntheticQuotes" : false, "routingType" : "DUTCH_LIMIT"},
  {"protocols" : [ "V2", "V3", "MIXED" ],
  "enableUniversalRouter" : true,
  "routingType" : "CLASSIC",
  "enableFeeOnTransferFeeFetching" : true}]})";

      /* Invoke */
      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://interface.gateway.uniswap.org/v2/quote")));
      session.SetBody(cpr::Body(body));
      const auto response = session.Post();
      std::cout << "INK O " << body << " : " << response.text << std::endl;
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
      result->exchange = supported::uniswap;
      result->qoute = response.text;

      std::cout << "INK O " << response.text << std::endl;
      const auto &qoute = json["quote"];

      // /* Change */
      // bool err = false;
      // result->price_impact = processor::format_str_double(json["priceImpactPct"].GetString(), 0u, err);
      // if (err) {
      //       error = error_type::conversion;
      //       return nullptr;
      // }
      //
      // /* Input */
      // result->input_address = json["inputMint"].GetString();
      // result->input_amount = processor::format_str_double(json["inAmount"].GetString(), 0u, err);
      // if (err) {
      //       error = error_type::conversion;
      //       return nullptr;
      // }
      //
      // /* Output */
      // result->output_address = json["outputMint"].GetString();
      // result->output_amount = processor::format_str_double(json["outAmount"].GetString(), 0u, err);
      // if (err) {
      //       error = error_type::conversion;
      //       return nullptr;
      // }
      //
      // /* Exchange rate */
      // result->exchange_rate = result->input_amount / result->output_amount;

      return result;
}
