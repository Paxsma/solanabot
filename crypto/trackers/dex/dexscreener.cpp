#include "../../crypto.hpp"
#include "../trackers.hpp"

#pragma region update

/* Token updater */
void crypto::token::token::update_dexscreener(const std::string &mint, crypto::error::error &error) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://api.dexscreener.com/latest/dex/tokens/" + mint)));

      const auto response = session.Get();
      if (response.status_code == 200) {

            /* Json information and validation */
            rapidjson::Document json;
            json.Parse(response.text.c_str());
            if (json.HasParseError()) {
                  error.t = error::type::json_parse;
                  return;
            }
            if (std::strcmp(json["schemaVersion"].GetString(), "1.0.0")) {
                  error.t = error::type::json_outdated;
                  return;
            }
            const auto &pairs = json["pairs"];
            if (!pairs.IsArray() || pairs.Empty()) {
                  error.t = error::type::invalid_mint;
                  return;
            }

            for (const auto &data : pairs.GetArray()) {

                  const auto dex_id = data["dexId"].GetString();

                  /* Labels */
                  std::vector<std::string> labels;
                  if (data.HasMember("labels")) {
                        for (const auto &l : data["labels"].GetArray()) {
                              labels.emplace_back(l.GetString());
                        }
                  }

                  const auto find = this->find_dex(dex_id, labels);
                  const auto dex = (find == nullptr) ? std::make_shared<token_tracking>() : find;

                  /* Update metadata */
                  this->name = data["baseToken"]["name"].GetString();
                  this->symbol = data["baseToken"]["symbol"].GetString();
                  this->set_metadata = true;

                  if (find == nullptr) {
                        dex->dex_id = dex_id;
                        dex->chain_id = data["chainId"].GetString();
                        dex->pair_address = data["pairAddress"].GetString();
                        dex->url = data["url"].GetString();
                        dex->network_address = data["quoteToken"]["address"].GetString();
                        dex->network_name = data["baseToken"]["name"].GetString();
                        dex->network_symbol = data["baseToken"]["symbol"].GetString();
                        dex->mint_address = data["baseToken"]["address"].GetString();
                        dex->creation = std::to_string(data["pairCreatedAt"].GetUint64());
                        dex->labels = labels;
                        dex->tracker = trackers::dexscreener;
                  }

                  /* Update pricing */
                  dex->price_native = types::biguint(data["priceNative"].GetString());
                  dex->price_usd = types::biguint(data["priceUsd"].GetString());

                  /* Qoute */
                  const auto &qoute = data["quoteToken"];
                  dex->qoute_address = qoute["address"].GetString();
                  dex->qoute_name = qoute["name"].GetString();
                  dex->qoute_symbol = qoute["symbol"].GetString();

                  /* Liquid */
                  const auto &liquid = data["liquidity"];
                  dex->liquid_usd = types::biguint(liquid["usd"].GetDouble());
                  dex->liquid_token_amount = types::biguint(liquid["base"].GetDouble());
                  dex->liquid_pooled = types::biguint(liquid["quote"].GetDouble());

                  /* Price */
                  const auto &price_change = data["priceChange"];
                  dex->price_change_minute_5 = price_change["m5"].GetDouble();
                  dex->price_change_hour_1 = price_change["h1"].GetDouble();
                  dex->price_change_hour_6 = price_change["h6"].GetDouble();
                  dex->price_change_hour_24 = price_change["h24"].GetDouble();

                  /* Volume */
                  const auto &price_volume = data["volume"];
                  dex->volume_minute_5 = price_volume["m5"].GetDouble();
                  dex->volume_hour_1 = price_volume["h1"].GetDouble();
                  dex->volume_hour_6 = price_volume["h6"].GetDouble();
                  dex->volume_hour_24 = price_volume["h24"].GetDouble();

                  /* Buys/Sells */
                  const auto &txns = data["txns"];
                  dex->txns_minute_5_buys = txns["m5"]["buys"].GetDouble();
                  dex->txns_minute_5_sells = txns["m5"]["sells"].GetDouble();
                  dex->txns_hour_1_buys = txns["h1"]["buys"].GetDouble();
                  dex->txns_hour_1_sells = txns["h1"]["sells"].GetDouble();
                  dex->txns_hour_6_buys = txns["h6"]["buys"].GetDouble();
                  dex->txns_hour_6_sells = txns["h6"]["sells"].GetDouble();
                  dex->txns_hour_24_buys = txns["h24"]["buys"].GetDouble();
                  dex->txns_hour_24_sells = txns["h24"]["sells"].GetDouble();

                  dex->est_supply = dex->fdv / dex->price_usd;

                  /* Info */
                  if (data.HasMember("info")) {

                        const auto &info = data["info"];

                        /* Image */
                        if (info.HasMember("imageUrl")) {
                              dex->info_image = info["imageUrl"].GetString();
                        }
                        /* Websites */
                        if (info.HasMember("websites")) {
                              const auto &websites = info["websites"];
                              for (const auto &m : websites.GetArray()) {
                                    dex->websites.insert(std::make_pair(m["label"].GetString(), m["url"].GetString()));
                              }
                        }
                        /* Socials */
                        if (info.HasMember("socials")) {
                              const auto &socials = info["socials"];
                              for (const auto &m : socials.GetArray()) {
                                    dex->socials.insert(std::make_pair(m["type"].GetString(), m["url"].GetString()));
                              }
                        }
                  }

                  /* Append to dexes */
                  if (find == nullptr) {
                        if (this->tracked.find(dex_id) != this->tracked.end()) {
                              this->tracked[dex_id].emplace_back(dex);
                        } else {
                              this->tracked.insert(std::make_pair(dex_id, std::vector<std::shared_ptr<token_tracking>>{dex}));
                        }
                  }
            }

      } else {
            error.t = error::type::unsupported;
      }
      return;
}

#pragma endregion

std::vector<std::string> crypto::trackers::dexscreener::search_mint(const std::vector<std::string> &search_terms, crypto::error::error &error) {

      std::vector<std::string> result;

      /* Compile */
      cpr::Session session;
      session.SetUrl(cpr::Url(std::string("https://api.dexscreener.com/latest/dex/search/?q=" + search_terms.front())));

      /* Call */
      const auto response = session.Get();
      if (response.status_code == 200) {

            /* Validate */
            rapidjson::Document json;
            json.Parse(response.text.c_str());
            if (json.HasParseError()) {
                  error.t = error::type::json_parse;
                  return result;
            }

            /* Version */
            if (std::strcmp(json["schemaVersion"].GetString(), "1.0.0")) {
                  error.t = error::type::json_outdated;
                  return result;
            }

            /* No pairs */
            const auto &pairs = json["pairs"];
            if (!pairs.IsArray() || pairs.Empty()) {
                  error.t = error::type::invalid_search;
                  return result;
            }

            /* Search */
            for (const auto &i : pairs.GetArray()) {
                  result.emplace_back(i["baseToken"]["address"].GetString());
            }
      }

      /* Remove duplicates */
      std::sort(result.begin(), result.end());
      result.erase(std::unique(result.begin(), result.end()), result.end());

      return result;
}