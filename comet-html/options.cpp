#include <boost/program_options.hpp>

using namespace std;

boost::program_options::options_description options_description()
{
  boost::program_options::options_description desc;

  desc.add_options()
    ("help,h", "produce help message")
    ("input,i",  boost::program_options::value<std::string>(), "input folder")
    ("output,o", boost::program_options::value<std::string>(), "output folder")
    ("config,c", boost::program_options::value<std::string>(), "config file")
    ("verbose,v", "enable verbose mode");
  return desc;
}

boost::program_options::variables_map get_options(int argc, char** argv)
{
  boost::program_options::variables_map result;

  boost::program_options::store(
    boost::program_options::parse_command_line(argc, argv, options_description()),
    result
  );
  boost::program_options::notify(result);
  return result;
}
