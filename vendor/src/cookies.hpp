#ifndef  CRAILS_FRONT_COOKIES_HPP
# define CRAILS_FRONT_COOKIES_HPP

# include "from_string.hpp"
# include <string>
# include <map>
# include <ctime>

namespace Comet
{
  class Cookies
  {
    typedef std::map<std::string, std::string> CookieMap;
  public:
    bool        has(const std::string& key);
    void        remove(const std::string& key);

    template<typename TYPE>
    TYPE get(const std::string& key)
    {
      std::string val = get<std::string>(key);

      return Comet::from_string<TYPE>(val);
    }

    template<typename TYPE>
    void set(const std::string& key, const TYPE& val)
    {
      set(key, std::to_string(val), 0);
    }

    template<typename TYPE>
    void set(const std::string& key, const TYPE& val, std::time_t expires_in)
    {
      set(key, std::to_string(val), expires_in);
    }

  private:
    std::string get_cookie_string() const;
    void        set_cookie_string(const std::string&) const;

    inline void ensure_cookie_map_is_updated()
    {
      if (last_cookie_string != get_cookie_string())
        update_cookie_map();
    }

    void update_cookie_map();

    CookieMap   cookie_store;
    std::string last_cookie_string;
  };

  template<>
  std::string Cookies::get<std::string>(const std::string& key);
  template<>
  void Cookies::set(const std::string& key, const std::string& val, std::time_t);
}

#endif
