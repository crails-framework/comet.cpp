#ifndef  MY_APPLICATION_HPP
# define MY_APPLICATION_HPP

# include <comet/router.hpp>

class Application
{
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
    auto& app = get();

    app.router.start();
  }

  Comet::Router router;
};

#endif
