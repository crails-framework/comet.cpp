#include "model.hpp"
#include "../http.hpp"

using namespace Comet;
using namespace std;

Comet::Promise Model::fetch()
{
  auto request = Http::Request::get(get_url());

  request->set_headers({{"Accept", get_content_type()}});
  return request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      if (response->has_body())
        parse(response->get_response_text());
      synced.trigger();
    }
  });
}

Comet::Promise Model::save()
{
  auto request = Http::Request::make(get_id() == COMET_MVC_NULL_ID ? "post" : "put", get_url());
  
  request->set_headers({
    {"Content-Type", get_content_type()},
    {"Accept",       get_content_type()}
  });
  request->set_body(get_payload());
  return request->send().then([this, request]()
  {
    auto response = request->get_response();
    
    if (response->ok())
    {
      if (response->has_body())
        parse(response->get_response_text());
      synced.trigger();
    }
  });
}

Comet::Promise Model::destroy(Comet::HttpResponseCallback callback)
{
  auto request = Http::Request::_delete(get_url());

  request->set_headers({{"Accept", get_content_type()}});
  return request->send().then([this, request, callback]()
  {
    auto response = request->get_response();

    if (response->ok())
      removed.trigger();
    if (callback)
      callback(response);
  });
}

std::string JsonModel::get_payload()
{
  if (get_resource_name().length() > 0)
  {
    std::stringstream stream;
    stream << '{' << '"' << get_resource_name() << '"' << ':' << to_json() << '}';

    return stream.str();
  }
  return to_json();
}

void JsonModel::parse(const std::string& str)
{
  DataTree data;

  data.from_json(str);
  if (get_resource_name().length() > 0 && data[get_resource_name()].exists())
    from_json(data[get_resource_name()]);
  else
    from_json(data);
}

std::string JsonModel::get_content_type() const
{
  return "application/json";
}
