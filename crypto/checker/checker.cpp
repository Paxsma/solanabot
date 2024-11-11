#include "checker.hpp"

std::string crypto::checker::solsniffer::sniff(const std::string& mint, crypto::error::error& error) {

    std::string result = "";

    cpr::Session session;
    session.SetUrl(cpr::Url(std::string("https://solsniffer.com/api/v1/sniffer/token/" + mint)));

    const auto response = session.Get();
    if (response.status_code == 200) {

        /* Json information and validation */
        rapidjson::Document json;
        json.Parse(response.text.c_str());
        if (json.HasParseError()) {
            error.t = error::type::json_parse;
        }

    }
    result = response.text;
	return result;
}
