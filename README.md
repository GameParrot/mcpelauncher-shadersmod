# mcpelauncher-shadersmod

This is a mod for mcpelauncher-manifest that allows for RenderDragon shaders.

Usage: 

Put your shaders in a folder called shaders in the data root folder. RenderDragon shaders have an extension of `.material.bin`.  

YSS shaders running with mod:  
![Shaders running](https://user-images.githubusercontent.com/85067619/233049451-6253095e-e5c9-433c-b2f3-5ccad202ecba.png)

## Building
`PATH_TO_NDK="/path/to/ndk"`
- x86

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86 ..`
- x86_64

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 ..`
- armeabi-v7a

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a ..`
- arm64-v8a

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a ..`
