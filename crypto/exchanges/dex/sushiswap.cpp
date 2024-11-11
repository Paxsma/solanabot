#include "../exchanges.hpp"

std::string crypto::exchange::sushiswap::all_tokens(error::error &error, const bool descriptive) {

      cpr::Session session;
      session.SetUrl(cpr::Url(std::string((descriptive) ? "https://api.sushi.com/price/v1/1" : "https://tokens.sushi.com/v0")));
      const auto response = session.Get();

      if (response.status_code != 200) {
            error.t = error::type::invalid_status;
      }
      return response.text;
}
