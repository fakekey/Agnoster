#include <GLES3/gl3.h>
#include <android/native_window_jni.h>
#include <android_native_app_glue.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>
#include <linux/time.h>
#include <thread>
#include <xdl.h>

#include "SDK.h"
#include "Surface.h"

ANativeWindow *g_NativeWindow = nullptr;
bool g_Initialized = false;
int glWidth = 0;
int glHeight = 0;

uint64_t ModuleBase = 0;
UWorld **GWorldPtr = nullptr;
FNamePool *NamePool = nullptr;
FUObjectArray *GUObjectArray = nullptr;

// UClass
UClass *Pawn = nullptr;
UClass *SGPlayerState = nullptr;
UClass *BP_UamCharacter_C = nullptr;
UClass *BP_UamAICharacter_C = nullptr;
UClass *GameplayStatics = nullptr;

// UFunction
UFunction *K2_GetActorLocationFn = nullptr;
UFunction *ProjectWorldLocationToScreenFn = nullptr;
UFunction *GetAllActorsOfClassFn = nullptr;

// uint64_t LastTryFindUClass = 0;

// if (BP_UamCharacter_C == nullptr && mach_absolute_time() > LastTryFindUClass + 15000000000ull) {
//     if (BP_UamAICharacter_C == nullptr) {
//         BP_UamAICharacter_C = GUObjectArray->ObjObjects.FindObject<UClass *>("BlueprintGeneratedClass BP_UamAICharacter.BP_UamAICharacter_C");
//     }
//     if (BP_UamAICharacter_C != nullptr) {
//         BP_UamCharacter_C = GUObjectArray->ObjObjects.FindObject<UClass *>("BlueprintGeneratedClass BP_UamCharacter.BP_UamCharacter_C");
//     }
//     LastTryFindUClass = mach_absolute_time();
// }

uint64_t Surface::mach_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000ull + now.tv_nsec;
}

void Surface::Init(JNIEnv *env, jclass clazz, jobject surface) {
    if (g_Initialized)
        return;

    // Find libUE4.so base address
    if (ModuleBase == 0) {
        void *lUE4 = xdl_open("libUE4.so", 0);
        if (lUE4) {
            void *symbolPointer = (void *)xdl_sym(lUE4, "ANativeActivity_onCreate", nullptr);
            if (symbolPointer)
                ModuleBase = (uint64_t)symbolPointer - Offsets::EntryPoint;
        }
        xdl_close(lUE4);
    }

    GWorldPtr = (UWorld **)(ModuleBase + Offsets::GWorld);
    NamePool = (FNamePool *)(ModuleBase + Offsets::GNames);
    GUObjectArray = (FUObjectArray *)(ModuleBase + Offsets::GObjects);

    // UClass
    Pawn = GUObjectArray->ObjObjects.FindObject<UClass *>("Class Engine.Pawn");
    SGPlayerState = GUObjectArray->ObjObjects.FindObject<UClass *>("Class SGFramework.SGPlayerState");
    GameplayStatics = GUObjectArray->ObjObjects.FindObject<UClass *>("Class Engine.GameplayStatics");

    // UFunction
    K2_GetActorLocationFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.Actor.K2_GetActorLocation");
    ProjectWorldLocationToScreenFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.PlayerController.ProjectWorldLocationToScreen");
    GetAllActorsOfClassFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.GameplayStatics.GetAllActorsOfClass");

    g_NativeWindow = ANativeWindow_fromSurface(env, surface);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_NativeWindow);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    g_Initialized = true;
}

void Surface::SurfaceChanged(JNIEnv *env, jclass clazz, jobject gl, jint width, jint height) {
    // get some info to setup menu
    glWidth = width;
    glHeight = height;
    glViewport(0, 0, width, height);

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = {(float)width, (float)height};
}

void Surface::Tick(JNIEnv *env, jclass clazz, jobject thiz) {
    ImGuiIO &io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // Overlay Window
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin("Hello, World!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
    MainThread(ImGui::GetWindowDrawList(), io);
    ImGui::End();

    // Rendering
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void Surface::Shutdown(JNIEnv *env, jclass clazz) {
    if (!g_Initialized)
        return;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    ANativeWindow_release(g_NativeWindow);

    g_Initialized = false;
}

jboolean Surface::Initialized(JNIEnv *env, jclass clazz) {
    return g_Initialized;
}

TArray<AActor *> Player{};

void Surface::MainThread(ImDrawList *canvas, ImGuiIO &io) {
    // FPS
    ImGui::Text("       Avg: %.1f FPS", io.Framerate);
    ImGui::Text("       GWorld: %p", GWorldPtr[0]);
    ImGui::Text("       GameplayStatics: %p", GameplayStatics);
    ImGui::Text("       GetAllActorsOfClassFn: %p", GetAllActorsOfClassFn);

    // Crosshair
    canvas->AddCircleFilled({io.DisplaySize.x / 2, io.DisplaySize.y / 2}, 3, ImColor{Colors::WHITE});

    // Logic stuffs
    UWorld *GWorld = GWorldPtr[0];
    if (GWorld) {
        auto UGStatics = (UGameplayStatics *)GameplayStatics;
        UGStatics->GetAllActorsOfClass(GWorld, Pawn, &Player);
        ImGui::Text("       Player.Count: %d", Player.Count);
    }
}
