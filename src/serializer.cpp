#include <cstdio>
#include "serializer.hpp"

using namespace std;

namespace Serializer
{
    void init()
    {
        printf("loading serializer data from disk\n");
    }

    void shutdown()
    {
        printf("writing serializer data to disk\n");
    }

    filesystem::path readPath(string id)
    {
        printf("reading path %s\n", id.c_str());
        return filesystem::path {};
    }

    void writePath(string id, filesystem::path path)
    {
        printf("writing path %s (%s)\n", id.c_str(), path.string().c_str());
    }
}