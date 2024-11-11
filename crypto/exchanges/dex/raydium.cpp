#include "../exchanges.hpp"

std::string crypto::exchange::raydium::V1::all_tokens(error::error &error) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://api.raydium.io/v2/main/pairs")));
      const auto response = session.Get();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
      }
      return response.text;
}

std::shared_ptr<crypto::exchange::exchange_qoute> crypto::exchange::raydium::V1::get_qoute(token::token &token, const std::double_t sell_amount, std::double_t slipage, error::error &error, const std::string &chain_id, const bool sell, const std::vector<std::string> &labels) {

      /* Check valid */
      if (!token.valid_token()) {
            error.t = error::type::invalid_token;
            return nullptr;
      }

      auto dex = token.find_dex("raydium", chain_id, labels); /* Can use same info */

      /* Check dex */
      if (dex == nullptr && (dex = token.find_dex("orca", chain_id, labels)) == nullptr && (dex = token.find_dex("jupiter", chain_id, labels)) == nullptr) {
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
      session.SetUrl(cpr::Url(std::string("https://transaction.raydium.io/v1/compute/swap-base-in")));
      session.SetParameters(cpr::Parameters{
          {"inputMint", input},
          {"outputMint", output},
          {"amount", amt.get_whole_data()},
          {"slippageBps", slip.get_whole_data()},
          {"txVersion", "V0"}});
      session.SetHeader(cpr::Header{{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36"}, {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"}});
      const auto response = session.Get();

      /* Parse JSON */
      rapidjson::Document json;
      std::cout << "SS " << response.text.c_str() << std::endl;
      json.Parse(response.text.c_str());
      if (json.HasParseError()) {
            error.t = error::type::json_parse;
            return nullptr;
      }

      if (response.status_code != 200 || !json["success"].GetBool()) {

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

      const auto& data = json["data"];

      /* Change */
      result->price_impact = types::biguint(data["priceImpactPct"].GetString());

      /* Input */
      result->input_address = data["inputMint"].GetString();
      result->input_amount = types::biguint(data["inAmount"].GetString());

      /* Output */
      result->output_address = data["outputMint"].GetString();
      result->output_amount = types::biguint(data["outAmount"].GetString());

      result->finalize();
      return result;
}
