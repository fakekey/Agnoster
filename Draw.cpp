#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>

#include "Draw.h"
#include "SDK.h"
#include "Surface.h"

extern bool g_Initialized;

extern uint64_t ModuleBase;
extern UWorld **GWorldPtr;
extern FNamePool *NamePool;
extern FUObjectArray *GUObjectArray;
struct android_app *g_App = nullptr;

// UClass
extern UClass *Pawn;
extern UClass *GameplayStatics;

// UFunction
extern UFunction *K2_GetActorLocationFn;
extern UFunction *GetAllActorsOfClassFn;
extern UFunction *ProjectWorldLocationToScreenFn;

void Draw::Init() {
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

    if (!ModuleBase)
        return;

    GWorldPtr = (UWorld **)(ModuleBase + Offsets::GWorld);
    NamePool = (FNamePool *)(ModuleBase + Offsets::GNames);
    GUObjectArray = (FUObjectArray *)(ModuleBase + Offsets::GObjects);
    g_App = *(struct android_app **)(ModuleBase + Offsets::AndroidApp);

    // UClass
    Pawn = GUObjectArray->ObjObjects.FindObject<UClass *>("Class Engine.Pawn");
    GameplayStatics = GUObjectArray->ObjObjects.FindObject<UClass *>("Class Engine.GameplayStatics");

    // UFunction
    K2_GetActorLocationFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.Actor.K2_GetActorLocation");
    ProjectWorldLocationToScreenFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.PlayerController.ProjectWorldLocationToScreen");
    GetAllActorsOfClassFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.GameplayStatics.GetAllActorsOfClass");

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    g_Initialized = true;
}

void Draw::MainLoop() {
    ImGuiIO &io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // Do some shits here.
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin("Hello, World!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
    MainThread(ImGui::GetWindowDrawList(), io);
    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern TArray<AActor *> Player;

void Draw::MainThread(ImDrawList *canvas, ImGuiIO &io) {
    // FPS
    ImGui::Text("       Avg: %.1f FPS", io.Framerate);
    ImGui::Text("       Player.Count: %d", Player.Count);
    ImGui::Text("       K2_GetActorLocationFn: %p", K2_GetActorLocationFn);
    ImGui::Text("       ProjectWorldLocationToScreenFn: %p", ProjectWorldLocationToScreenFn);

    // Crosshair
    canvas->AddCircleFilled({io.DisplaySize.x / 2, io.DisplaySize.y / 2}, 3, ImColor{Colors::WHITE});

    // Logic stuffs
    if (K2_GetActorLocationFn == nullptr) {
        K2_GetActorLocationFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.Actor.K2_GetActorLocation");
        return;
    }
    if (ProjectWorldLocationToScreenFn == nullptr) {
        ProjectWorldLocationToScreenFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.PlayerController.ProjectWorldLocationToScreen");
        return;
    }

    uint64_t LastGetArray = 0;

    UWorld *GWorld = GWorldPtr[0];
    if (GWorld) {
        if (GetAllActorsOfClassFn) {
            auto GameState = GWorld->GameState;
            if (GameState) {
                auto pArrCount = (int *)((uint64_t)GameState + 0x318 + 0x8);
                if (pArrCount) {
                    auto ArrCount = *pArrCount;
                    if (ArrCount >= 2) {
                        auto GameInstance = GWorld->OwningGameInstance;
                        if (GameInstance) {
                            auto LocalPlayer = GameInstance->LocalPlayers.Data[0];
                            if (LocalPlayer) {
                                auto Controller = LocalPlayer->PlayerController;
                                if (Controller) {
                                    auto LocalActor = Controller->AcknowledgedPawn;
                                    if (LocalActor) {
                                        auto localPos = LocalActor->K2_GetActorLocation();

                                        if (Surface::mach_absolute_time() > LastGetArray + 100000000ull) {
                                            auto UGStatics = (UGameplayStatics *)GameplayStatics;
                                            UGStatics->GetAllActorsOfClass(GWorld, Pawn, &Player);
                                            LastGetArray = Surface::mach_absolute_time();
                                        }

                                        for (int i = 0; i < Player.Count; i++) {
                                            auto actor = Player.Data[i];
                                            if (actor) {
                                                if (actor == LocalActor)
                                                    continue;

                                                auto pos = actor->K2_GetActorLocation();
                                                int distance = localPos.Distance(pos) / 100;
                                                std::string txt = "[" + std::to_string(distance) + "m]";
                                                FVector2D pos2d;
                                                if (Controller->ProjectWorldLocationToScreen(pos, pos2d, false)) {
                                                    canvas->AddLine({io.DisplaySize.x / 2, 0}, {pos2d.X, pos2d.Y}, ImColor{Colors::GREEN});
                                                    canvas->AddText({pos2d.X, pos2d.Y}, ImColor{Colors::GREEN}, txt.c_str());
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        Player.Count = 0;
                    }
                } else {
                    Player.Count = 0;
                }
            } else {
                Player.Count = 0;
            }
        } else {
            GetAllActorsOfClassFn = GUObjectArray->ObjObjects.FindObject<UFunction *>("Function Engine.GameplayStatics.GetAllActorsOfClass");
            Player.Count = 0;
        }
    } else {
        Player.Count = 0;
    }
}
