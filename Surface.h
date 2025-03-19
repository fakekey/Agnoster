#pragma once

#ifndef AGNOSTER_SURFACE_H
#define AGNOSTER_SURFACE_H

#include <imgui.h>
#include <jni.h>

namespace Colors {
    constexpr ImVec4 BLACK = ImVec4(0.06f, 0.06f, 0.06f, 1.f);
    constexpr ImVec4 WHITE = ImVec4(1.f, 1.f, 1.f, 1.f);
    constexpr ImVec4 RED = ImVec4(1.f, 0.f, 0.f, 1.f);
    constexpr ImVec4 DARK_RED = ImVec4(0.6f, 0.f, 0.f, 1.f);
    constexpr ImVec4 GREEN = ImVec4(0.f, 1.f, 0.f, 1.f);
    constexpr ImVec4 DARK_GREEN = ImVec4(0.f, 0.6f, 0.f, 1.f);
    constexpr ImVec4 YELLOW = ImVec4(1.f, 1.f, 0.f, 1.f);
    constexpr ImVec4 DARK_YELLOW = ImVec4(0.6f, 0.6f, 0.f, 1.f);
    constexpr ImVec4 CYAN = ImVec4(0.f, 1.f, 1.f, 1.f);
    constexpr ImVec4 PURPLE = ImVec4(1.f, 0.f, 1.f, 1.f);
    constexpr ImVec4 GRAY = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
    constexpr ImVec4 ORANGE = ImVec4(1.f, 0.54f, 0.f, 1.f);
    constexpr ImVec4 BLUE = ImVec4(0.f, 0.f, 1.f, 1.f);
    constexpr ImVec4 BROWN = ImVec4(0.54f, 0.27f, 0.06f, 1.f);
} // namespace Colors

namespace Surface {
    void Init(JNIEnv *env, jclass clazz, jobject surface);
    void SurfaceChanged(JNIEnv *env, jclass clazz, jobject gl, jint width, jint height);
    void Tick(JNIEnv *env, jclass clazz, jobject thiz);
    void Shutdown(JNIEnv *env, jclass clazz);
    jboolean Initialized(JNIEnv *env, jclass clazz);
    void MainThread(ImDrawList *canvas, ImGuiIO &io);
    uint64_t mach_absolute_time();
} // namespace Surface

#endif
