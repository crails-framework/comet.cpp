#ifndef  COMET_MVC_MODEL_HPP
# define COMET_MVC_MODEL_HPP

# include "../signal.hpp"
# include "../promise.hpp"
# include "../datatree.hpp"
# include "id_type.hpp"

namespace Comet
{
  namespace Http { class Response; }
  typedef std::function<void (std::shared_ptr<Http::Response>)> HttpResponseCallback;

  class Model : public Listener
  {
  public:
    typedef COMET_MVC_ID_TYPE id_type;

    Signal<void> removed;
    Signal<void> synced;

    virtual id_type     get_id() const { return id; }
    virtual std::string get_url() const = 0;
    virtual std::string get_resource_name() const { return ""; }

    Promise fetch();
    Promise save();
    Promise destroy(HttpResponseCallback callback = HttpResponseCallback());
    virtual void parse(const std::string& str) = 0;

  protected:
    virtual std::string get_payload() = 0;
    virtual std::string get_content_type() const = 0;

    id_type id = COMET_MVC_NULL_ID;
  };

  class JsonModel : public Model
  {
  public:
    virtual void from_json(Data) = 0;
    virtual std::string to_json() const = 0;
    void parse(const std::string& str);
  protected:
    std::string get_payload();
    std::string get_content_type() const;
  };
}

#endif
