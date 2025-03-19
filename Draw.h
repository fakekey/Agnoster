#pragma once

#ifndef AGNOSTER_DRAW_H
#define AGNOSTER_DRAW_H

#include <android_native_app_glue.h>
#include <imgui.h>
#include <jni.h>
#include <xdl.h>

namespace Draw {
    void Init();
    void MainLoop();
    void MainThread(ImDrawList *canvas, ImGuiIO &io);
} // namespace Draw

#endif
