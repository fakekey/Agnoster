mkdir build
cd build

/Users/nghilq/Library/Android/sdk/cmake/3.22.1/bin/cmake \
    -H/Users/nghilq/Desktop/Agnoster \
    -DCMAKE_SYSTEM_NAME=Android \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_SYSTEM_VERSION=21 \
    -DANDROID_PLATFORM=android-21 \
    -DANDROID_ABI=arm64-v8a \
    -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
    -DANDROID_NDK=/Users/nghilq/Library/Android/sdk/ndk/28.0.13004108 \
    -DCMAKE_ANDROID_NDK=/Users/nghilq/Library/Android/sdk/ndk/28.0.13004108 \
    -DCMAKE_TOOLCHAIN_FILE=/Users/nghilq/Library/Android/sdk/ndk/28.0.13004108/build/cmake/android.toolchain.cmake \
    -DCMAKE_MAKE_PROGRAM=/Users/nghilq/Library/Android/sdk/cmake/3.22.1/bin/ninja \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=/Users/nghilq/Desktop/Agnoster/build \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=/Users/nghilq/Desktop/Agnoster/build \
    -DCMAKE_BUILD_TYPE=Release \
    -B/Users/nghilq/Desktop/Agnoster/build \
    -GNinja \
    ..

/Users/nghilq/Library/Android/sdk/cmake/3.22.1/bin/cmake --build .

cp libagnoster.so /Users/nghilq/Desktop/AgnosterAPK/app/src/main/jniLibs/arm64-v8a
cp libagnoster.so /Users/nghilq/Downloads/Original/com.vng.mf.liteuamo/lib/arm64-v8a
