#pragma once

#ifndef AGNOSTER_PREDEF_H
#define AGNOSTER_PREDEF_H

#include <android/log.h>
#include <dobby.h>
#include <imgui.h>
#include <imgui_impl_android.h>
#include <xdl.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "agnoster", __VA_ARGS__)

#define HOOK_DEF(ret, func, ...)     \
    ret (*orig_##func)(__VA_ARGS__); \
    ret hook_##func(__VA_ARGS__)

#define MUFF(target, ptr, orig) DobbyHook((void *)target, (void *)ptr, (void **)&orig)

#endif
