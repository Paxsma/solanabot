#include "../exchanges.hpp"

crypto::exchange::trades::view crypto::exchange::mexc::trading::get_trades(const tradable trade, const trades::resolution res, error::error &error, time_t start, time_t end) {

      trades::view result;

      std::string coin = "";
      switch (trade) {
            case tradable::bitcoin:
            default: {
                  coin = "BTC_USDT";
                  break;
            }
      }

      std::string time = "";
      switch (res) {
            case trades::resolution::minute_1:
            default: {
                  time = "Min1";
                  break;
            }
      }

     // auto time = std::time(nullptr);
      const auto url = cpr::Url(std::string("https://futures.mexc.com/api/v1/contract/kline/" + coin));

      for (auto i = 0u; i < INT_MAX; ++i) {

            cpr::Session session;
           // session.SetUrl(url);
           // session.SetParameters(cpr::Parameters {
           // {"interval", time},
           // {"end", static_cast<std::uint64_t>(time -= 1000u)},
           // {"start", static_cast<std::uint64_t>(time)}});

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
