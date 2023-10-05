#include <cstdio>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <filesystem>
#include "serializer.hpp"

using namespace std;

namespace Serializer
{
    static unordered_map<string, string> s_pathLookup;
    static filesystem::path s_configPath;

    void init(char** argv)
    {
        s_configPath = filesystem::path{argv[0]}.remove_filename() / "porytiles_gui_config.json";

        if (filesystem::exists(s_configPath))
        {
            fstream fileStream{};
            fileStream.open(s_configPath);
            nlohmann::json config{};
            fileStream >> config;
            fileStream.close();

            printf("loading serializer data from disk\n");
            s_pathLookup = config["paths"];
        }
    }

    void shutdown()
    {
        printf("writing serializer data to disk\n");

        nlohmann::json config {};
        config["paths"] = s_pathLookup;

        ofstream fileStream {};
        fileStream.open(s_configPath);
        fileStream << std::setw(4) << config;
        fileStream.close();
    }

    filesystem::path readPath(string id, filesystem::path defaultValue)
    {
        printf("reading path %s\n", id.c_str());

        if (s_pathLookup.contains(id))
        {
            return filesystem::path {s_pathLookup[id]};
        }

        return defaultValue;
    }

    void writePath(string id, filesystem::path path)
    {
        printf("writing path %s (%s)\n", id.c_str(), path.string().c_str());
        s_pathLookup[id] = path.string();
    }
}