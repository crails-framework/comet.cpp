#ifndef  CRAILS_FRONT_ROUTER_HPP
# define CRAILS_FRONT_ROUTER_HPP

# include "router_base.hpp"
# include "signal.hpp"
# include <cheerp/client.h>
# include <map>

namespace Comet
{
  typedef std::map<std::string, std::string> Params;

  class Router : public RouterBase<Params, std::function<void (const Params&)> >
  {
  public:
    Router();

    Signal<const std::string&> on_route_executed;
    Signal<const std::string&> on_route_not_found;

    bool navigate(const std::string& path, bool trigger = true);

    void initialize();
    std::string get_current_path() const;

  private:
    void on_hash_changed();

    std::string last_hash;
  };
}

#endif
