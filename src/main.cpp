#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h> 
#include <android/log.h>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <unistd.h>

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);
void (*mcpelauncher_log)(int level, const char* tag, const char* text);

std::string dataDir = "/data/data/com.mojang.minecraftpe";
std::vector<std::string> shadersList;

int countCharacterOccurrences(const std::string& str, char character) {
    int count = 0;
    for (char c : str) {
        if (c == character) {
            count++;
        }
    }
    return count;
}

std::string assetsToRoot;

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    std::string assetsDir;

    //Below is a hack to get the real path to the assets folder and game data folder
    char originalDir[PATH_MAX];
    getcwd(originalDir, sizeof(originalDir));
    chdir(dataDir.c_str());
    char currentDir[PATH_MAX];
    getcwd(currentDir, sizeof(currentDir));
    chdir(".");
    char currentDir2[PATH_MAX];
    getcwd(currentDir2, sizeof(currentDir2));
    chdir(originalDir);
    dataDir = currentDir; // gets the data dir using chdir which uses iorewrite 
    assetsDir = currentDir2; // gets the assets dir using chdir which uses iorewrite
    chdir("/");

    int occurrences = countCharacterOccurrences(assetsDir, '/');

    for(int i = 0; i<occurrences; i++) {
        assetsToRoot+="../"; // hack to get back to the root directory using the assets folder as the start due to how the asset makager works
    }

    auto h = dlopen("libmcpelauncher_mod.so", 0);

    DIR *dr;
    struct dirent *en;
    dr = opendir((dataDir + "/shaders").c_str()); //open all directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            if (strstr(en->d_name, ".material.bin")) {
                shadersList.push_back(std::string(en->d_name));
            }
        }
        closedir(dr); //close all directory
    }

    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");
    mcpelauncher_log = (decltype(mcpelauncher_log)) dlsym(h, "mcpelauncher_log");

    mcpelauncher_preinithook("AAssetManager_open", (void*)+[](AAssetManager *mgr, const char *filename, int mode) -> AAsset* {
        if ((strstr(filename, ".material.bin"))) {
            std::string fName = std::string(filename).substr(std::string(filename).find_last_of("/") + 1);
            if (std::find(shadersList.begin(), shadersList.end(), fName) != shadersList.end()) {
                __android_log_print(ANDROID_LOG_VERBOSE, "ShadersMod", "Patched shader %s via AAssetManager", fName.c_str());
                return AAssetManager_open(mgr, (assetsToRoot + dataDir + "/shaders/" + fName).c_str(), mode); // uses custom asset path like /path/to/assets/../../../ to get to root
            } else {
                return AAssetManager_open(mgr, filename, mode);
            }
        } else {
            return AAssetManager_open(mgr, filename, mode);
        }
    }, nullptr);

    //dlclose(h);
}

extern "C" __attribute__ ((visibility ("default"))) void mod_init() {
}
