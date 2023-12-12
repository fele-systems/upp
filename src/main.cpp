#include <crow.h>
#include <crow/http_response.h>
#include <type_traits>
#include <fmt/core.h>
#include <yaml-cpp/yaml.h>
#include <config.h>

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
    

    CROW_ROUTE(app, "/")
    ([&](){
        crow::response response;
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx;
        {
            wvalue::list dirs;
            for (const auto& dir : config.dirs)
            {
                dirs.push_back(wvalue{ {"path",dir.path}, {"passphrase",dir.passphrase} });
            }
            ctx["dirs"] = std::move(dirs);
        }
        response.body = page.render_string(ctx);
        response.add_header("Content-Type", "text/html; charset=utf-8");
        return response;
    });

    CROW_ROUTE(app, "/upload")
            .methods(crow::HTTPMethod::POST)
    ([](const crow::request& request) {
        crow::multipart::message msg{request};

        auto passphrase = msg.get_part_by_name("passphrase");

        std::cout << "Passphrase is: " << passphrase.body << std::endl;
        
        auto fileinput = msg.get_part_by_name("fileInput");
        auto filename = fileinput.get_header_object("Content-Disposition").params.at("filename");

        std::ofstream offload { fmt::format("/tmp/{}", filename), std::ios_base::out | std::ios_base::binary };

        if (offload.is_open())
        {
            offload.write(fileinput.body.c_str(), fileinput.body.size());
            offload.flush();
            offload.close();
        }
        else
        {
            std::cout << "Something bad happended" << std::endl;
        }


        std::cout << "Uploaded file: " << filename << std::endl;

        /*for (const auto& part : msg.parts) {
            for (const auto& header : part.headers) {
                std::cout << header.first << " => " << map_to_string(header.second.params) << std::endl;
            }
        }*/

        crow::response response{};
        response.redirect("/static/index.html");
        return response;
    });

    app.port(config.port).multithreaded().run();

    return 0;
}