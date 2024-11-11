#include "../../crypto.hpp"
#include "../trackers.hpp"
#include <regex>

#pragma region update

/* Token updater */
void crypto::token::token::update_geckoterminal(const std::string &mint, crypto::error::error &error) {

      cpr::Session session;
      cpr::Parameters parameters;
      parameters.Add({"include", "from_token,to_token"});
      parameters.Add({"page[after]", std::to_string(std::time(nullptr))});
      parameters.Add({"pair_id", "2848761"});
      session.SetParameters(parameters);
      session.SetUrl(cpr::Url(std::string("https://app.geckoterminal.com/api/p1/solana/pools/" + mint + "/swaps")));

      return;
}

#pragma endregion

std::string crypto::trackers::geckoterminal::all_coins(crypto::error::error &error) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://api.coingecko.com/api/v3/asset_platforms")));
      const auto response = session.Get();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
      }
      return response.text;
}

std::string crypto::trackers::geckoterminal::all_tokens(crypto::error::error &error) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://tokens.coingecko.com/solana/all.json")));
      const auto response = session.Get();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
      }
      return response.text;
}

#pragma region swaps

std::vector<std::shared_ptr<crypto::trackers::geckoterminal::swap>> form_swaps(const rapidjson::Document &json) {

      std::vector<std::shared_ptr<crypto::trackers::geckoterminal::swap>> result;

      std::unordered_map<std::uint32_t, crypto::trackers::geckoterminal::swap::token> tokens;
      for (auto i = 0u; i < 2u; ++i) {
            crypto::trackers::geckoterminal::swap::token t;

            /* Token data */
            const auto &tk = json["included"][i];
            t.id = std::stoi(tk["id"].GetString());
            t.type = tk["type"].GetString();

            /* Attributes */
            const auto &attributes = tk["attributes"];
            t.name = attributes["name"].GetString();
            t.symbol = attributes["symbol"].GetString();
            t.mint_address = attributes["address"].GetString();

            tokens.insert(std::make_pair(t.id, t));
      }

      const auto &data = json["data"];
      std::printf("AMT %d\n", data.GetArray().Size());
      if (data.GetArray().Size()) {
            std::printf("TS %s\n", data.GetArray()[0]["attributes"]["timestamp"].GetString());
      }
      for (const auto &i : data.GetArray()) {

            const auto s = std::make_shared<crypto::trackers::geckoterminal::swap>();

            /* Not a swap */
            if (std::strcmp(i["type"].GetString(), "swap") || !i.HasMember("attributes")) {
                  continue;
            }

            s->id = i["id"].GetString();

            const auto &attributes = i["attributes"];
            s->timestamp = attributes["timestamp"].GetString();
            s->tx_hash = attributes["tx_hash"].GetString();
            s->tx_from_address = attributes["tx_from_address"].GetString();

            s->from_token_amount = attributes["from_token_amount"].GetString();
            s->price_from = attributes["price_from"].GetString();
            s->price_from_in_currency_token = attributes["price_from_in_currency_token"].GetString();
            s->price_from_in_usd = attributes["price_from_in_usd"].GetString();
            s->from_token_total_in_usd = attributes["from_token_total_in_usd"].GetString();
            s->to_token_amount = attributes["to_token_amount"].GetString();
            s->price_to = attributes["price_to"].GetString();
            s->price_to_in_currency_token = attributes["price_to_in_currency_token"].GetString();
            s->price_to_in_usd = attributes["price_to_in_usd"].GetString();
            s->to_token_total_in_usd = attributes["to_token_total_in_usd"].GetString();

            const auto &relationships = i["relationships"];

            /* From */
            const auto &from = relationships["from_token"]["data"];
            s->from = tokens[std::stoi(from["id"].GetString())];

            /* To */
            const auto &to = relationships["to_token"]["data"];
            s->to = tokens[std::stoi(to["id"].GetString())];

            result.emplace_back(s);
      }

      return result;
}

std::vector<std::shared_ptr<crypto::trackers::geckoterminal::swap>> crypto::trackers::geckoterminal::recent_swaps(const std::string &pool, crypto::error::error &error) {

      std::vector<std::shared_ptr<swap>> result;

      cpr::Session session;
      session.SetParameters(cpr::Parameters{{"include", "from_token,to_token"}, {"page[after]", std::to_string(std::time(nullptr))}});
      session.SetHeader(cpr::Header{{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36"}});
      session.SetUrl(cpr::Url(std::string("https://app.geckoterminal.com/api/p1/solana/pools/" + pool + "/swaps")));

      const auto response = session.Get();
      if (response.status_code == 200) {

            /* Json information and validation */
            rapidjson::Document json;
            json.Parse(response.text.c_str());
            if (json.HasParseError()) {
                  error.t = error::type::json_parse;
                  return result;
            }

            if (!json.HasMember("data") || !json["data"].IsArray()) {
                  error.t = error::type::invalid_mint;
                  return result;
            }

            result = form_swaps(json);
      } else {
            error.t = error::type::unsupported;
      }
      return result;
}

/* Decode url */
std::string decode(const std::string &input) {
      std::stringstream decoded;
      for (auto i = 0u; i < input.size(); ++i) {
            if (input[i] == '%' && i + 2 < input.size() && isxdigit(input[i + 1]) && isxdigit(input[i + 2])) {
                  std::int32_t hex;
                  if (sscanf_s(input.substr(i + 1, 2).c_str(), "%x", &hex) == 1) {
                        decoded << static_cast<std::int8_t>(hex);
                        i += 2;
                  } else {
                        decoded << input[i];
                  }
            } else {
                  decoded << input[i];
            }
      }
      return decoded.str();
}

#pragma endregion