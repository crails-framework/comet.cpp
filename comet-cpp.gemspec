Gem::Specification.new do |s|
  s.name         = 'comet-cpp'
  s.version      = '0.9.0'
  s.date         = '2020-03-05'
  s.summary      = 'Comet.cpp is a JavaScript client framework'
  s.description  = ''
  s.authors      = ["Michael Martin Moro"]
  s.email        = 'michael@unetresgrossebite.com'
  s.files        = `git ls-files -z`.split("\x0").select do |name|
    name.match %r{^(bin|lib|vendor)/}
  end
  s.homepage     = 'https://github.com/crails-framework/comet-cpp'
  s.license      = 'BSD-3-Clause'
  s.require_path = 'lib'
  s.bindir       = 'bin'
  s.executables << 'comet-make' << 'comet-html' << 'comet-new'

  s.add_runtime_dependency 'nokogiri', '~> 1.10.8'
end
