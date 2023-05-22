#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h> 
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <vector>

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);
void (*mcpelauncher_log)(int level, const char* tag, const char* text);

std::string dataDir = "/data/data/com.mojang.minecraftpe";
std::vector<std::string> shadersList;

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
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
                char pt1[256] = "Patched shader ";
                strcat(pt1, fName.c_str());
                strcat(pt1, " via AAssetManager");
                mcpelauncher_log(0, "ShadersMod", pt1);
                return AAssetManager_open(mgr, (dataDir + "/shaders/" + fName).c_str(), mode);
            } else {
                return AAssetManager_open(mgr, filename, mode);
            }
        } else {
            return AAssetManager_open(mgr, filename, mode);
        }


    }, nullptr);

    mcpelauncher_preinithook("fopen", (void*)+[](const char *filename, const char *mode) -> FILE* {
        if ((strstr(filename, ".material.bin"))) {
            std::string fName = std::string(filename).substr(std::string(filename).find_last_of("/") + 1);
            if (std::find(shadersList.begin(), shadersList.end(), fName) != shadersList.end()) {
                char pt1[256] = "Patched shader ";
                strcat(pt1, fName.c_str());
                strcat(pt1, " via fopen");
                mcpelauncher_log(0, "ShadersMod", pt1);
                return fopen((dataDir + "/shaders/" + fName).c_str(), mode);
            } else {
                return fopen(filename, mode);
            }
        } else {
            return fopen(filename, mode);
        }
    }, nullptr);

    //dlclose(h);
}

extern "C" __attribute__ ((visibility ("default"))) void mod_init() {
}
