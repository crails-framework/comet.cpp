#ifndef  COMET_MVC_COLLECTION_HPP
# define COMET_MVC_COLLECTION_HPP

# include "../signal.hpp"
# include "../http.hpp"
# include "../datatree.hpp"
# include "model.hpp"
# include <map>
# include <list>

namespace Comet
{
  template<typename MODEL>
  class Collection : public Listener
  {
    typedef std::shared_ptr<MODEL> ModelPtr;
    typedef std::map<typename MODEL::id_type, ModelPtr> Map;
  public:
    typedef MODEL Model;

    Signal<MODEL&> added;
    Signal<MODEL&> removed;
    Signal<MODEL&> changed;
    Signal<void>   synced;

    virtual std::string get_url() const = 0;
    virtual std::string get_mimetype() const = 0;

    void add(ModelPtr model)
    {
      auto it = models.at(model->get_id());

      if (it == models.end())
      {
        models.emplace(model->get_id(), model);
        added.trigger(*model);
      }
    }

    void remove(ModelPtr model)
    {
      auto it = models.at(model->get_id());

      if (it != models.end())
      {
        models.erase(it);
        removed.trigger(*model);
      }
    }

    void each(std::function<void (ModelPtr)> callback)
    {
      for (auto item : models)
        callback(item.second);
    }

    std::list<ModelPtr> where(std::function<bool (ModelPtr)> selector) const
    {
      std::list<ModelPtr> results;

      for (auto item : models)
      {
        if (selector(item.second))
          results.push_back(item.second);
      }
      return results;
    }

    ModelPtr find(std::function<bool (ModelPtr)> selector) const
    {
      for (auto item : models)
      {
        if (selector(item.second))
          return item.second;
      }
      return nullptr;
    }

    ModelPtr get(typename MODEL::id_type id)
    {
      auto it = models.find(id);

      if (it != models.end())
        return it->second;
      return nullptr;
    }

    Promise fetch()
    {
      auto request = Http::Request::get(get_url());
        
      request->set_headers({{"Accept", get_mimetype()}});
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

    unsigned int count() const
    {
      return models.size();
    }

  protected:
    virtual void parse(const std::string& str) = 0;

    Map models;
  };

  template<typename MODEL>
  class JsonCollection : public Collection<MODEL>
  {
  public:
    virtual std::string get_mimetype() const
    {
      return "application/json";
    }

    virtual std::string get_resource_name() const
    {
      return "";
    }

  protected:
    virtual void parse(const std::string& str)
    {
      DataTree   payload;
      Data       models_json;
      const auto resource_name = get_resource_name();

      payload.from_json(str);
      if (resource_name.length() == 0)
        models_json = payload.as_data();
      else
        models_json = payload[resource_name];
      models_json.each([this](Data model_json) -> bool
      {
        auto ptr = std::make_shared<MODEL>();
        ptr->from_json(model_json);
        Collection<MODEL>::models.emplace(ptr->get_id(), ptr);
        return true;
      });
    }
  };
}

#endif
