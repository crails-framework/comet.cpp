#include "cookies.hpp"
#include "exception.hpp"
#include "object.hpp"
#include "url.hpp"
#include "parse_cookie_values.hpp"
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

using namespace Comet;
using namespace std;

Comet::Cookies cookies;

bool Cookies::has(const string& key)
{
  ensure_cookie_map_is_updated();
  return cookie_store.find(key) != cookie_store.end();
}

template<>
string Cookies::get<string>(const string& key)
{
  ensure_cookie_map_is_updated();
  if (!has(key))
    Comet::raise(std::runtime_error("cookie `" + key + "` does not exist"));
  return cookie_store.at(key);
}

template<>
void Cookies::set<string>(const string& key, const string& val)
{
  ensure_cookie_map_is_updated();
  if (has(key))
    cookie_store.at(key) = val;
  else
    cookie_store.insert(pair<string, string>(key, val));
  set_cookie_string(
    Url::encode(key) + '=' + Url::encode(val)
  );
}

template<>
void Cookies::set<string>(const string& key, const string& val, time_t expires_in)
{
  string time_string;

  ensure_cookie_map_is_updated();
  if (has(key))
    cookie_store.at(key) = val;
  else
    cookie_store.insert(pair<string, string>(key, val));
  if (expires_in > 0)
  {
    ObjectImpl<client::Date> date;
    double expires_time = date->getTime() + expires_in * 1000;

    date->setTime(expires_time);
    time_string = "; expires=" + (string)date.apply("toGMTString");
  }
  set_cookie_string(
    Url::encode(key) + '=' + Url::encode(val) + time_string
  );
}

void Cookies::remove(const string& key)
{
  ensure_cookie_map_is_updated();
  cookie_store.erase(key);
  set_cookie_string(
    Url::encode(key) + "=null; expires=Sat, 05 Nov 2016 00:00:00 GMT"
  );
  last_cookie_string = get_cookie_string();
}

void Cookies::update_cookie_map()
{
  last_cookie_string = get_cookie_string();
  Comet::parse_cookie_values(last_cookie_string,
    [this](const string& key, const string& value) {
    cookie_store.emplace(key, value);
    return true;
  });
}

string Cookies::get_cookie_string() const
{
  return (string)(*client::document.get_cookie());
}

void Cookies::set_cookie_string(const string& str) const
{
  client::document.set_cookie(str.c_str());
}
