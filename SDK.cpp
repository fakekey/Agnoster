#include "SDK.h"

extern FNamePool *NamePool;

std::string FNameEntry::ToString() {
    if (Header.bIsWide) {
        return {};
    }
    return {AnsiName, Header.Len};
}

FNameEntry *FName::GetDisplayNameEntry() const {
    return &NamePool->Resolve(ComparisonIndex);
}

std::string FName::GetName() {
    auto entry = GetDisplayNameEntry();
    auto name = entry->ToString();
    if (Number > 0) {
        name += '_' + std::to_string(Number);
    }
    auto pos = name.rfind('/');
    if (pos != std::string::npos) {
        name = name.substr(pos + 1);
    }
    return name;
}

std::string UObject::GetName() {
    return NamePrivate.GetName();
}

std::string UObject::GetFullName() {
    std::string name;
    for (auto outer = OuterPrivate; outer; outer = outer->OuterPrivate) {
        name = outer->GetName() + "." + name;
    }
    name = ClassPrivate->GetName() + " " + name + GetName();
    return name;
}

FVector FVector::operator/(const FVector &v) const {
    FVector result(this->X / v.X, this->Y / v.Y, this->Z / v.Z);
    return result;
}

FVector FVector::operator/(float mod) const {
    FVector result(this->X / mod, this->Y / mod, this->Z / mod);
    return result;
}

FVector &FVector::operator+=(const FVector &v) {
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}

FVector &FVector::operator+=(float fl) {
    this->X += fl;
    this->Y += fl;
    this->Z += fl;
    return *this;
}

FVector &FVector::operator-=(const FVector &v) {
    this->X -= v.X;
    this->Y -= v.Y;
    this->Z -= v.Z;
    return *this;
}

FVector &FVector::operator-=(float fl) {
    this->X -= fl;
    this->Y -= fl;
    this->Z -= fl;
    return *this;
}

float FVector::Distance(const FVector &to) const {
    return sqrtf(powf(to.X - X, 2) + powf(to.Y - Y, 2) + powf(to.Z - Z, 2));
}

bool FVector::IsValid() const {
    return this->X != 0 && this->Y != 0;
}

FVector FVector::operator-(const FVector &v) const {
    FVector result(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
    return result;
}

float FVector::Dot(FVector const &other) const {
    return this->X * other.X + this->Y * other.Y + this->Z * other.Z;
}

void UObject::ProcessEvent(struct UFunction *func, void *params) {
    reinterpret_cast<ProcessEventFn>(VTable[70])(this, func, params);
}

extern UFunction *K2_GetActorLocationFn;
FVector AActor::K2_GetActorLocation() {
    struct {
        FVector Return;
    } params;

    ProcessEvent(K2_GetActorLocationFn, &params);
    return params.Return;
}

bool UObject::IsA(UClass *cmp) const {
    for (auto super = ClassPrivate; super; super = static_cast<UClass *>(super->SuperField)) {
        if (super == cmp)
            return true;
    }

    return false;
}

extern UFunction *GetAllActorsOfClassFn;
void UGameplayStatics::GetAllActorsOfClass(UObject *WorldContextObject, UClass *ActorClass, TArray<AActor *> *OutActors) {
    auto fn = GetAllActorsOfClassFn;
    UGameplayStatics_GetAllActorsOfClass_Params params{};
    params.WorldContextObject = WorldContextObject;
    params.ActorClass = ActorClass;

    auto flags = fn->FunctionFlags;
    ProcessEvent(fn, &params);
    fn->FunctionFlags = flags;

    if (OutActors != nullptr)
        *OutActors = params.OutActors;
}

extern UFunction *ProjectWorldLocationToScreenFn;
bool APlayerController::ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D &ScreenLocation, bool bPlayerViewportRelative) {
    struct {
        FVector WorldLocation;
        FVector2D ScreenLocation;
        bool bPlayerViewportRelative;
        bool Return;
    } params;

    params.WorldLocation = WorldLocation;
    params.ScreenLocation = ScreenLocation;
    params.bPlayerViewportRelative = bPlayerViewportRelative;
    ProcessEvent(ProjectWorldLocationToScreenFn, &params);
    ScreenLocation = params.ScreenLocation;
    return params.Return;
}
