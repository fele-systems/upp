#include <server.h>

Server::Server(const Config &config)
    : config(config)
{
}

void Server::register_handlers(crow::SimpleApp &app)
{
    using namespace std::placeholders;

    CROW_ROUTE(app, "/")
        .methods(crow::HTTPMethod::GET)(std::bind(&Server::get_index, this, crow::mustache::context{}));
    CROW_ROUTE(app, "/upload")
        .methods(crow::HTTPMethod::POST)(std::bind(&Server::upload, this, _1));
}

crow::response Server::get_index(crow::mustache::context ctx)
{
    using namespace crow::json;
    crow::response response;
    auto page = crow::mustache::load("index.html");

    wvalue::list dirs;
    for (const auto &dir : config.dirs)
    {
        dirs.push_back(wvalue{{"path", dir.path}, {"passphrase", dir.passphrase}});
    }
    ctx["dirs"] = std::move(dirs);

    response.body = page.render_string(ctx);

    response.add_header("Content-Type", "text/html; charset=utf-8");
    return response;
}

crow::response Server::upload(const crow::request &request)
{
    crow::multipart::message msg{request};

    std::string passphrase;
    std::string directory;

    if (auto itr = msg.part_map.find("passphrase"); itr != msg.part_map.end())
    {
        passphrase = itr->second.body;
    }
    else
    {
        std::cerr << "Could not find passphrase" << std::endl;
        return get_index(crow::mustache::context{{"error", "Could not find passphrase"}});
    }

    if (auto itr = msg.part_map.find("directory"); itr != msg.part_map.end())
    {
        directory = itr->second.body;
    }
    else
    {
        std::cerr << "Could not find directory" << std::endl;
        return get_index(crow::mustache::context{{"error", "Could not find directory"}});
    }

    std::cout << "Passphrase is: " << passphrase << std::endl;
    std::cout << "Directory is: " << directory << std::endl;

    auto config_dir = std::find_if(config.dirs.begin(), config.dirs.end(), [&directory](const auto &dir)
                                   { return dir.path == directory; });

    if (config_dir == config.dirs.end())
    {
        std::cerr << "Could not find directory" << std::endl;
        return get_index(crow::mustache::context{{"error", "Could not find directory"}});
    }

    if (!config_dir->passphrase.empty() && config_dir->passphrase != passphrase)
    {
        std::cerr << "Passphrase is wrong" << std::endl;
        std::cerr << "Actual: " << config_dir->passphrase << std::endl;
        return get_index(crow::mustache::context{{"error", "Passphrase is wrong"}});
    }

    auto fileinput = msg.get_part_by_name("fileInput");
    auto filename = fileinput.get_header_object("Content-Disposition").params.at("filename");

    auto target_file = config_dir->path + filename;

    std::ofstream offload{target_file, std::ios_base::out | std::ios_base::binary};

    if (offload.is_open())
    {
        offload.write(fileinput.body.c_str(), fileinput.body.size());
        offload.flush();
        offload.close();
    }
    else
    {
        std::cout << "Something bad happended. Could not open " << target_file << std::endl;
        return get_index(crow::mustache::context{{"error", "Something bad happended. Could not open " + target_file}});
    }

    std::cout << "Uploaded file: " << target_file << std::endl;

    /*for (const auto& part : msg.parts) {
        for (const auto& header : part.headers) {
            std::cout << header.first << " => " << map_to_string(header.second.params) << std::endl;
        }
    }*/

    return get_index({});
}
