#ifndef  MY_APPLICATION_HPP
# define MY_APPLICATION_HPP

# include <comet/router.hpp>

class Application
{
  friend void webMain();
  Application() {}
  Application(const Application&)    = delete;
  void operator=(const Application&) = delete;

public:
  static Application& get()
  {
    static Application instance;
    return instance;
  }

  static void start()
  {
    get();
  }

  Comet::Router router;
};

#endif
