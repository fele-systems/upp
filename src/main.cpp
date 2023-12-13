#include <crow.h>
#include <crow/http_response.h>
#include <type_traits>
#include <fmt/core.h>
#include <yaml-cpp/yaml.h>
#include <config.h>
#include <server.h>

template<typename K, typename V>
std::string map_to_string(const std::unordered_map<K,V> &m) {
    std::string output = "";
    std::string convrt = "";
    std::string result = "";

	for (auto it = m.cbegin(); it != m.cend(); it++) {
	
        if constexpr (std::is_same<V, std::string>::value) {
		    convrt = it->second;
        } else {
            convrt = std::to_string(it->second);
        }
		output += (it->first) + ":" + (convrt) + ", ";
	}
	
	result = output.substr(0, output.size() - 2 );
	
  return result;
}

int _main()
{
    auto config = YAML::LoadFile("upp.yml")["server"].as<Config>();

    std::cout << "Port: " << config.port << std::endl;

    for (const auto& dir : config.dirs)
    {
        std::cout << dir << std::endl;
    }
/*
    if (!config["server"] || !config["server"]["dirs"])
    {
        std::cerr << "Missing server or dirs object in upp.yaml";
        return 1;
    }

    for (const auto& dir : config["server"]["dirs"])
    {
        std::cout << "path: " << dir["path"].as<std::string>() << std::endl;
        std::cout << "passphrase: " << dir["passphrase"].as<std::string>() << std::endl;
    }*/
    return 0;
}



int main()
{
    using namespace crow::json;

    crow::SimpleApp app;

    auto config = YAML::LoadFile("upp.yml")["server"].as<Config>();
    Server server {config};

    server.register_handlers(app);


    app.port(config.port).multithreaded().run();

    return 0;
}