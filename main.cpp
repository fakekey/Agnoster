#include <EGL/egl.h>

#include "Draw.h"
#include "Killer.h"
#include "Predef.h"
#include "Surface.h"

JavaVM *g_vm = nullptr;
JNIEnv *g_env = nullptr;

int RegisterFunctions(JNIEnv *env) {
    // first for Killer
    JNINativeMethod f_methods[] = {
        {"StartKiller", "(Landroid/content/Context;)V", (void *)Killer::StartKiller},
        {"ProcInvoke", "(Ljava/lang/reflect/Method;[Ljava/lang/Object;)Ljava/lang/Object;", (void *)Killer::ProcInvoke},
    };

    jclass f_clazz = env->FindClass("com/nghilq/agnoster/Agnoster");
    if (!f_clazz)
        return -1;

    if (env->RegisterNatives(f_clazz, f_methods, sizeof(f_methods) / sizeof(f_methods[0])) != 0)
        return -1;

    // second for Surface
    JNINativeMethod s_methods[] = {
        {"Init", "(Landroid/view/Surface;)V", (void *)Surface::Init},
        {"Initialized", "()Z", (void *)Surface::Initialized},
        {"SurfaceChanged", "(Ljavax/microedition/khronos/opengles/GL10;II)V", (void *)Surface::SurfaceChanged},
        {"Tick", "(Lcom/nghilq/agnoster/AgnosterSurface;)V", (void *)Surface::Tick},
        {"Shutdown", "()V", (void *)Surface::Shutdown},
    };

    jclass s_clazz = env->FindClass("com/nghilq/agnoster/AgnosterSurface");
    if (!s_clazz)
        return -1;

    if (env->RegisterNatives(s_clazz, s_methods, sizeof(s_methods) / sizeof(s_methods[0])) != 0)
        return -1;

    return 0;
}

// HOOK_DEF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
//     orig_Input(thiz, ex_ab, ex_ac);
//     if (g_Initialized)
//         ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
//     return;
// }

extern bool g_Initialized;
HOOK_DEF(EGLBoolean, eglSwapBuffers, EGLDisplay dpy, EGLSurface surface) {
    if (!g_Initialized)
        Draw::Init();
    else
        Draw::MainLoop();

    return orig_eglSwapBuffers(dpy, surface);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_vm = vm;

    if (vm->GetEnv(reinterpret_cast<void **>(&g_env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    if (RegisterFunctions(g_env) != 0) {
        return -1;
    }

    // Hook libinput.so for interacting with imgui
    // void *linput = xdl_open("libinput.so", 0);
    // if (linput) {
    //     void *symbolPointer = (void *)xdl_sym(linput, "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE", nullptr);
    //     if (symbolPointer)
    //         MUFF(symbolPointer, hook_Input, orig_Input);
    // }
    // xdl_close(linput);

    void *lEGL = xdl_open("libEGL.so", 0);
    if (lEGL) {
        void *symbolPointer = (void *)xdl_sym(lEGL, "eglSwapBuffers", nullptr);
        if (symbolPointer)
            MUFF(symbolPointer, hook_eglSwapBuffers, orig_eglSwapBuffers);
    }
    xdl_close(lEGL);

    return JNI_VERSION_1_6;
}
