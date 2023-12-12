#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <iosfwd>
#include <fmt/core.h>

struct Directory
{
    std::string path;
    std::string passphrase;
};

struct Config
{
    std::string base_path = "./";
    uint16_t port = 8080;
    std::vector<Directory> dirs;
};

namespace YAML
{
    template<>
    struct convert<Directory>
    {
        static Node encode(const Directory& rhs) { return Node{}; }

        static bool decode(const Node& node, Directory& rhs) {
            if(!node.IsMap())
                return false;

            rhs.path = node["path"].as<std::string>();
            rhs.passphrase = node["passphrase"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<Config>
    {
        static Node encode(const Config& rhs) { return Node{}; }

        static bool decode(const Node& node, Config& rhs) {
            if(!node.IsMap())
                return false;

            if (const auto &port = node["port"]; !port.IsNull())
                rhs.port = port.as<uint16_t>();

            if (const auto &base_path = node["base_path"]; !base_path.IsNull())
                rhs.base_path = base_path.as<std::string>();

            const auto& dirs = node["dirs"];

            if (!dirs.IsSequence())
                return false;

            for (const auto& dir : dirs)
            {
                rhs.dirs.push_back(dir.as<Directory>());
            }

            return true;
        }
    };
}

std::ostream& operator<<(std::ostream& os, const Directory& dir) {
    return os << fmt::format("Dir[path={}, passphrase={}]", dir.path, dir.passphrase);
}