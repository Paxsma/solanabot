#pragma once
#include "../crypto.hpp"


/* Custom compile time JSON for speed */
#define payload_sep ","
#define payload_contain(value) "{" value "}"
#define payload_object(key, value) "\"" key "\":\"" + value + "\""
#define payload_iobject(key, value) "\"" key "\":" + value +
#define payload_cobject(key, value) "\"" key "\":\"" value "\""
#define payload_ciobject(key, value) "\"" key "\":" value 
#define payload_v(value) "\"" + value + "\""
#define payload_iv(value) + value +
#define payload(method, values) (std::string ("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"" method "\",\"params\":[") + (values "]}"))
#define cpayload(method, values) std::string ("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"" method "\",\"params\":[" values "]}")
#define payload_method(method) std::string ("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"" method "\"}")

namespace rpc::v2 {

      enum class protocol : std::uint8_t {
            get,
            post,
            option
      };
      template <const protocol p>
      std::string call(const std::string &endpoint, crypto::error::error &error, const std::string& json) {

            /* Create */
            cpr::Session session;
            session.SetUrl(cpr::Url(endpoint));
            session.SetHeader(cpr::Header{{"Content-Type", "application/json"}});
            session.SetBody(cpr::Body{json});

            /* Call */
            cpr::Response response;
            switch (p) {
                  case protocol::option: {
                        response = session.Options();
                        break;
                  }
                  case protocol::post: {
                        response = session.Post();
                        break;
                  }
                  case protocol::get:
                  default: {
                        response = session.Get();
                        break;
                  }
            }

            /* Error? */
            if (response.status_code != 200) {

                  error.t = crypto::error::type::invalid_status;

                  rapidjson::Document json;
                  json.Parse(response.text.c_str());
                  if (!json.HasParseError() && json.HasMember("error") && json["error"].HasMember("message")) {
                        error.what_str = json["error"]["message"].GetString();
                  }
            }

            return response.text;
      }

} // namespace rpc::v2