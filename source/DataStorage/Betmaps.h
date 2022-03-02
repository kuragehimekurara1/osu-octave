#ifndef _BEATMAPS_H
#define _BEATMAPS_H

#include <fstream>
#include <limits>
#include "map"

#include "defines.h"
#include "GameplayElements/Score.h"

#include "Helpers/JSON.hpp"
using json = nlohmann::json;

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

typedef struct BeatmapEntry {
    std::string Filename;
    std::string BackgroundFilename;
    std::string Title;
    std::string Artist;
    std::string Creator;
    std::string Version;
    std::string AudioFilename;
    std::string BaseDir;
    std::string Checksum;
} BeatmapEntry;

inline void to_json(json& j, const BeatmapEntry h) {
    j = json{{"filename", h.Filename}, {"bg", h.BackgroundFilename}, {"title", h.Title},
             {"artist", h.Artist}, {"creator", h.Creator}, {"version", h.Version},
             {"audiofilename", h.AudioFilename}, {"basedir", h.BaseDir}, {"checksum", h.Checksum}};
}

inline void from_json(const json& j, BeatmapEntry& h) {
    j.at("filename").get_to(h.Filename);
    j.at("bg").get_to(h.BackgroundFilename);
    j.at("title").get_to(h.Title);
    j.at("artist").get_to(h.Artist);
    j.at("creator").get_to(h.Creator);
    j.at("version").get_to(h.Version);
    j.at("audiofilename").get_to(h.AudioFilename);
    j.at("basedir").get_to(h.BaseDir);
    j.at("checksum").get_to(h.Checksum);
}

class Beatmaps
{
public:
    static bool get_beatmap(const std::string& checksum, BeatmapEntry& h) {
        if (beatmaps.find(checksum) != beatmaps.end()) {
            h = beatmaps[checksum].get<BeatmapEntry>();
            return true;
        }
        return false;
    }

    static void set_beatmap(BeatmapEntry& h) {
        beatmaps[h.Checksum] = h;
    }

    static void get_map(std::vector<BeatmapEntry> &v) {
        v.clear();
        for (json::iterator it = beatmaps.begin(); it != beatmaps.end(); ++it) {
            v.emplace_back(it.value());
        }
    }

    static bool is_path_in_state(const char* filename, const char* basedir) {
        std::string s = std::string(basedir) + "/" + std::string(filename);
        if (std::find(state["paths"].begin(), state["paths"].end(), s)!=state["paths"].end()) {
            return true;
        }
        fprintf(stderr, "%s/%s is not in state\n", basedir, filename);
        fprintf(stderr, "%s is not in state\n", s.c_str());
        return false;
    }

    static void add_path_to_state(const char* filename, const char* basedir) {
        std::string s = std::string(basedir) + "/" + std::string(filename);
        state["paths"].emplace_back(s);
    }

    static void clear() {
        beatmaps = {};
        state = {{"paths", json::array()}};
    }

    static void save() {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsIndexFilename);

        std::ofstream o(path);
        o << std::setw(4) << beatmaps << std::endl;

        char path_state[PATH_MAX];
        snprintf(path_state, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsStateFilename);

        std::ofstream p(path_state);
        p << std::setw(4) << state << std::endl;
    }

    static void load() {
        clear();

        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsIndexFilename);

        std::ifstream i(path);
        json j = json::parse(i, nullptr, false);

        if (j.is_discarded()) {
            fprintf(stderr, "Beatmaps index JSON parse error, proceeding with empty.\n");
            return;
        }

        beatmaps = j;

        char path_state[PATH_MAX];
        snprintf(path_state, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsStateFilename);

        std::ifstream u(path_state);
        json k = json::parse(u, nullptr, false);

        if (k.is_discarded()) {
            fprintf(stderr, "Beatmaps state JSON parse error, proceeding with empty.\n");
            return;
        }

        state = k;
    }

private:
    static json beatmaps;
    static json state;
};

#endif //_BEATMAPS_H
