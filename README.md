Comet.cpp
======
Comet.cpp is an MVC front-end development library targeted for the web. It's been inspired by Backbone.js and Aurelia, and provides utilities to interact with HTML5 apis, as well as a minimalist framework for MVC development, and an HTML templating engine that generates C++ from HTML, allowing you to easily generate and interact with DOM elements.

Comet.cpp leverages [Cheerp](https://www.leaningtech.com/pages/cheerp.html)'s C++ to JavaScript transpiler to compile your C++ application into a native web application. Cheerp is a transpiler that features numerous custom optimisation steps to maximise performance and minimise size of the compiled JavaScript output. It's also best suited for Comet since it allows you to directly interact with the native JavaScript APIs, as well as debugging your code directly from your browser's debugger.

Install Comet.cpp
======
Before using comet, you will need to install Cheerp's transpiler, by following Cheerp's [Getting started](https://docs.leaningtech.com/cheerp/Getting-started) guide.

Comet also uses the `build2` build toolchain to make installation as straightforward as possible. Follow the instrucations at [Build2's homepage](https://build2.org/install.xhtml) to install it.

Next, run the following commands to install comet:

```
bpkg create -d comet-gcc cc config.cxx=c++
cd comet-gcc
bpkg add https://pkg.cppget.org/1/beta
bpkg add "https://github.com/crails-framework/comet.cpp.git#master"
bpkg fetch

bpkg build comet
bpkg install comet --all --recursive config.install.sudo=sudo config.install.root=/usr/local
```

This will install comet along with all its dependencies. You're good to go !

## You already have boost installed on your system
Assuming you already have boost installed on your system, you may replace `bpkg build comet` with the following:

```
bpkg build comet '?sys:libboost-program-options' '?sys:libboost-json'
```

Which will build comet and install it using the boost libraries provided by your system.

Creating your first Comet.cpp application
======
## Setting up a new comet application
Install the `comet-cpp` gem using the following command: `gem install comet-cpp`.

Then, create a new directory for your project, and run the following command: `comet-new -n hello-world`.

The `comet-new` command will generate the basic file structure for your new project. Let's take a look at what's been created:

```
CMakeLists.txt		# The CMakeLists for your project.

Gemfile			# Handles the ruby development dependencies for your project, such
			# as comet or guard.

Guardfile		# Configures guard to watch your files and generate the code for
			# your html template and/or compile your code.

app/			# Your code's directory
app/main.cpp		# The entry point of your application. By default, it initializes the Application class.
app/routes.cpp		# This is where you'll register your applications routes.
app/application.hpp	# The main class of your application.
app/controllers		# Your controller's directory
app/models		# Your model's directory
app/collections		# Your collection's directory
app/views/              # Your html templates and custom elements code directory
app/views/layouts/      # Your layout html templates' directory
comet-elements/		# This is where the code generated for your html templates will be persisted and compiled from.
public/			# This is the directory that will contain your static files and compiled assets.
public/index.html       # A default index.html file generated to launch your Comet app from
```

## Creating a layout and a view
Comet's view system consists of layouts and views. Layout are container views that are by default directly attached to the page's body. Views are similar to HTML5 custom elements and can be attached to a layout, or instantiated directly from other elements.

Let's make our first layout view. Create the file `app/views/layouts/application.html` and fill it as following:

````
<template>
  <body>
    <h1>Hello comet !</h1>
    <slot name="content">
      <!-- The content slot in a layout will be replaced by the current view once it's been initialized -->
    </slot>
  </body>
</template>
````

And now, we'll add our first view by creating the file `app/views/homepage.html` with the following content:

````
<template>
  <body>
    This is the content for my view.
  </body>
</template>
````

## Adding your first controller
Now that we've added views, we need to render those. Views and layouts are rendered from controllers. Let's create our first one at `app/controllers/application.hpp` with the following code:

````
#ifndef  APPLICATION_CONTROLLER_HPP
# define APPLICATION_CONTROLLER_HPP

# include <comet/mvc/controller.hpp>
// The following inclusions are for your html view and layout templates.
# include "html/views/layouts/application.hpp"
# include "html/views/homepage.hpp"

class ApplicationController : public Comet::Controller
{
public:
  ApplicationController(const Comet::Params& p) : Comet::Controller(p)
  {
  }

  void home()
  {
    auto view = std::make_shared<HtmlTemplate::Homepage>();

    use_layout<HtmlTemplate::Application>()
      .render(view);
  }
};

#endif
````

We have added a controller with a `home` action. To trigger that action, we must implement a route in `app/routes.cpp` as following:

```
#include <comet/router.hpp>
#include "controllers/application.hpp"

using namespace Comet;

void Router::initialize()
{
  match_route("/?", ApplicationController, home);
}
```

The `match_route` macro takes three parameters: a regular expression string to match the browser's location hash, the controller's class name, and the controller's method name.

## Compiling your code
Use the `comet-html` command to compile your html template into C++.

Then, use the `comet-make` command to generate your application's javascript.

## Launching the web server
You can use the `comet-web` command to start a simple web server and test your application.
