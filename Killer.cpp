#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "Killer.h"
#include "Predef.h"

std::vector<std::vector<uint8_t>> apk_signatures{{0x30, 0x82, 0x03, 0x3D, 0x30, 0x82, 0x02, 0x25, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x04, 0x79, 0x88, 0x84, 0x58, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30, 0x4E, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x63, 0x6E, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x02, 0x67, 0x64, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x02, 0x73, 0x7A, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x02, 0x6D, 0x66, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x13, 0x02, 0x6D, 0x66, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x02, 0x6D, 0x66, 0x30, 0x20, 0x17, 0x0D, 0x32, 0x30, 0x30, 0x38, 0x32, 0x36, 0x31, 0x30, 0x32, 0x30, 0x33, 0x39, 0x5A, 0x18, 0x0F, 0x32, 0x32, 0x39, 0x34, 0x30, 0x36, 0x31, 0x31, 0x31, 0x30, 0x32, 0x30, 0x33, 0x39, 0x5A, 0x30, 0x4E, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x63, 0x6E, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x02, 0x67, 0x64, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x02, 0x73, 0x7A, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x02, 0x6D, 0x66, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x13, 0x02, 0x6D, 0x66, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x02, 0x6D, 0x66, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xB9, 0x86, 0xAA, 0xD7, 0x8A, 0x7C, 0x88, 0xD9, 0x6F, 0x4B, 0x24, 0xBC, 0xC7, 0x6E, 0x3F, 0xAE, 0x32, 0x3D, 0x36, 0x6F, 0x1E, 0xD1, 0xEC, 0xFB, 0x6E, 0xA4, 0xA1, 0x6E, 0x95, 0x28, 0x0F, 0x51, 0x41, 0x1A, 0xEB, 0x51, 0x52, 0x56, 0x7C, 0x16, 0x01, 0x4A, 0xB3, 0xEF, 0x6B, 0x8E, 0xEB, 0xAD, 0xB2, 0x24, 0x24, 0x7B, 0xA7, 0xD2, 0x5F, 0xCD, 0x13, 0x8A, 0x0E, 0x74, 0x6A, 0x7E, 0x02, 0x5E, 0x5D, 0x29, 0xAA, 0x05, 0x66, 0x9C, 0x46, 0xAB, 0xA2, 0x83, 0x6E, 0x2E, 0x53, 0x6C, 0x82, 0x3A, 0x23, 0xBE, 0x08, 0xEE, 0xDC, 0x67, 0xD7, 0xAF, 0xAE, 0x4C, 0x4E, 0x1F, 0x01, 0x54, 0x45, 0x66, 0x6A, 0xD8, 0x03, 0xA2, 0xAB, 0x93, 0xF0, 0x1C, 0xE1, 0xC2, 0x76, 0xFA, 0x90, 0x86, 0x46, 0xF1, 0x12, 0x22, 0x7F, 0xD4, 0x2F, 0x2C, 0xFD, 0x68, 0xC9, 0x7E, 0x82, 0x6D, 0xEE, 0xB8, 0x76, 0x1F, 0x2F, 0x2A, 0x23, 0xBD, 0x6B, 0x39, 0x91, 0xD6, 0x91, 0xD0, 0x78, 0x0A, 0x80, 0x55, 0xB2, 0x98, 0x8B, 0x58, 0xCE, 0x7E, 0x2E, 0xCF, 0x8B, 0x91, 0x73, 0x3A, 0x2C, 0xC7, 0xB0, 0xC1, 0x70, 0xFD, 0x15, 0x1A, 0xF2, 0xDA, 0xAB, 0x0E, 0xEE, 0x2B, 0x17, 0x06, 0xEB, 0x27, 0x30, 0xED, 0x9B, 0x0B, 0x8E, 0xD9, 0x0E, 0x6B, 0x54, 0xC5, 0xAE, 0xAD, 0xFB, 0x0E, 0xA2, 0x4D, 0x68, 0x82, 0x4D, 0xBB, 0x45, 0x99, 0xFD, 0x19, 0xD3, 0xDE, 0x36, 0x88, 0xA3, 0x28, 0xCC, 0x02, 0x64, 0x0F, 0x7C, 0x32, 0x1F, 0x68, 0xC2, 0x6B, 0x2A, 0x6C, 0x9C, 0x37, 0x03, 0x16, 0x04, 0xF4, 0xF0, 0x1D, 0x97, 0xDB, 0x93, 0xE0, 0xD4, 0xC7, 0xD7, 0xF0, 0x32, 0x1F, 0x17, 0x78, 0x95, 0xAE, 0x7C, 0xFE, 0x87, 0xBC, 0xC6, 0x1A, 0xEA, 0xAB, 0xC3, 0x50, 0xEB, 0x3E, 0xF3, 0xE7, 0x2E, 0xE8, 0xE1, 0xE1, 0xDD, 0xE1, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x21, 0x30, 0x1F, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0xF5, 0xEC, 0xEB, 0x06, 0x3B, 0xCE, 0x03, 0x00, 0x87, 0x66, 0x29, 0x57, 0x50, 0xA2, 0x78, 0x85, 0xD0, 0xAC, 0xB9, 0x84, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x13, 0x0B, 0xBE, 0x32, 0x74, 0xE9, 0x22, 0x78, 0xB4, 0xEE, 0xF6, 0x0D, 0xC1, 0x1F, 0x41, 0x8B, 0x97, 0x77, 0xB1, 0x19, 0x93, 0x34, 0x1D, 0xE8, 0x74, 0xDB, 0x53, 0x51, 0xA8, 0x5E, 0x1A, 0x44, 0x75, 0xC0, 0xC0, 0xED, 0x0B, 0x6B, 0xB7, 0x46, 0x54, 0x06, 0x16, 0x03, 0x6D, 0xF2, 0xE5, 0x29, 0x3D, 0xBF, 0xD1, 0xD7, 0x3D, 0xD1, 0x1E, 0x22, 0xC1, 0x00, 0x4A, 0xA0, 0xE4, 0x82, 0x7A, 0xA7, 0x26, 0x91, 0x39, 0x56, 0xBA, 0x88, 0xD0, 0x5A, 0x82, 0xE6, 0xC1, 0xBB, 0xC6, 0xE9, 0x0D, 0x15, 0xF3, 0x5A, 0x0B, 0x2B, 0x73, 0x8C, 0x34, 0x33, 0xAD, 0xDD, 0xB5, 0xB1, 0xAB, 0xE8, 0x7F, 0x5E, 0xB8, 0xF4, 0xEB, 0xB6, 0x47, 0x97, 0x4E, 0xA3, 0xFA, 0x4E, 0x7B, 0xAE, 0x60, 0xD4, 0xA5, 0x08, 0x77, 0xB7, 0x6F, 0x36, 0x91, 0xE9, 0x57, 0x6B, 0xFE, 0x79, 0x74, 0x51, 0xEA, 0x05, 0x30, 0x3D, 0x5B, 0x32, 0x42, 0xB0, 0xD0, 0x27, 0xAC, 0xE2, 0x3F, 0xF8, 0xCF, 0x32, 0xBE, 0x49, 0xFC, 0xBE, 0x56, 0x45, 0x6F, 0x97, 0xA7, 0x12, 0x69, 0x9E, 0xA5, 0x63, 0xBE, 0x79, 0xB0, 0x44, 0x8E, 0x8D, 0x56, 0x3C, 0xFF, 0x19, 0xC5, 0x93, 0x21, 0xF9, 0x16, 0x56, 0xA8, 0x15, 0x52, 0xB5, 0x39, 0xC1, 0x9D, 0x0A, 0x79, 0xE5, 0x7B, 0x3E, 0x0C, 0x8D, 0x43, 0x85, 0x58, 0xDE, 0x51, 0x66, 0x09, 0x1F, 0x50, 0x0A, 0x52, 0xC8, 0x51, 0xA4, 0x38, 0x4D, 0x1A, 0xF2, 0x7B, 0xDE, 0x66, 0xE2, 0x3E, 0x8F, 0x5C, 0x68, 0xAE, 0xE5, 0x76, 0xC6, 0xF2, 0x31, 0x35, 0x81, 0xDC, 0x7A, 0x6A, 0x9E, 0xF5, 0xC3, 0xD9, 0x3A, 0x56, 0x53, 0xEE, 0x01, 0xF0, 0x8F, 0x81, 0x60, 0x0A, 0x67, 0xC5, 0x42, 0x4B, 0x38, 0xD2, 0xF0, 0x10, 0xDB, 0x38, 0xC6, 0xE7, 0xAB, 0xDC, 0x78, 0xE8, 0xC4, 0x19, 0x89, 0x93, 0x05}};

std::string g_pkgName;
jstring g_apkPath;
jobject g_proxy, g_pkgMgr;
AAssetManager *g_assetManager;

void Killer::StartKiller(JNIEnv *env, jclass clazz, jobject context) {
    env->PushLocalFrame(64); // We call this so that we don't need to manually delete the local refs

    g_env = env;
    g_assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(context, env->GetMethodID(env->FindClass("android/content/Context"), "getAssets", "()Landroid/content/res/AssetManager;")));

    std::string apkPkg = getPackageName(context);
    g_pkgName = apkPkg;

    auto procName = getProcessName();
    LOGI("-------- Killing %s", procName);

    char apkDir[512];
    sprintf(apkDir, "/data/data/%s/cache", apkPkg.c_str());
    mkdir(apkDir, 0777);

    std::string apkPath = "/data/data/";
    apkPath += apkPkg;
    apkPath += "/cache/";
    apkPath += apk_fake_name;

    if (access(apkPath.c_str(), F_OK) == -1) {
        extractAsset(apk_asset_path, apkPath);
    }

    g_apkPath = (jstring)env->NewGlobalRef(g_env->NewStringUTF(apkPath.c_str()));

    Class activityThreadClass("android.app.ActivityThread");
    auto sCurrentActivityThreadField = activityThreadClass.getField("sCurrentActivityThread");
    auto sCurrentActivityThread = sCurrentActivityThreadField.get(NULL);

    auto mBoundApplicationField = activityThreadClass.getField("mBoundApplication");
    patch_AppBindData(mBoundApplicationField.get(sCurrentActivityThread));

    auto mInitialApplicationField = activityThreadClass.getField("mInitialApplication");
    patch_Application(mInitialApplicationField.get(sCurrentActivityThread));

    auto mAllApplicationsField = activityThreadClass.getField("mAllApplications");
    auto mAllApplications = mAllApplicationsField.get(sCurrentActivityThread);
    ArrayList list(mAllApplications);
    for (int i = 0; i < list.size(); i++) {
        auto application = list.get(i);
        patch_Application(application);
        list.set(i, application);
    }
    mAllApplicationsField.set(sCurrentActivityThread, list.getObj());

    auto mPackagesField = activityThreadClass.getField("mPackages");
    auto mPackages = mPackagesField.get(sCurrentActivityThread);
    ArrayMap mPackagesMap(mPackages);
    for (int i = 0; i < mPackagesMap.size(); i++) {
        WeakReference loadedApk(mPackagesMap.valueAt(i));
        patch_LoadedApk(loadedApk.get());
        mPackagesMap.setValueAt(i, WeakReference::Create(loadedApk.get()));
    }
    mPackagesField.set(sCurrentActivityThread, mPackagesMap.getObj());

    auto mResourcePackagesField = activityThreadClass.getField("mResourcePackages");
    auto mResourcePackages = mResourcePackagesField.get(sCurrentActivityThread);
    ArrayMap mResourcePackagesMap(mResourcePackages);
    for (int i = 0; i < mResourcePackagesMap.size(); i++) {
        WeakReference loadedApk(mResourcePackagesMap.valueAt(i));
        patch_LoadedApk(loadedApk.get());
        mResourcePackagesMap.setValueAt(i, WeakReference::Create(loadedApk.get()));
    }
    mResourcePackagesField.set(sCurrentActivityThread, mResourcePackagesMap.getObj());

    // patch_ContextImpl(getApplicationContext(context));
    patch_PackageManager(context);

    env->PopLocalFrame(0);
}

jobject Killer::ProcInvoke(JNIEnv *env, jclass clazz, jobject method, jobjectArray args) {
    env->PushLocalFrame(64);

    auto Integer_intValue = [&](jobject param) -> int {
        auto integerClass = env->FindClass("java/lang/Integer");
        auto intValueMethod = env->GetMethodID(integerClass, "intValue", "()I");
        return env->CallIntMethod(param, intValueMethod);
    };

    Method mMethod(env, method);
    const char *mName = mMethod.getName();
    auto mResult = mMethod.invoke(g_pkgMgr, args);

    if (!strcmp(mName, "getPackageInfo")) {
        const jobject packageInfo = mResult;
        if (packageInfo) {
            const char *packageName = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(args, 0), 0);
            int flags = Integer_intValue(env->GetObjectArrayElement(args, 1));
            if (!strcmp(packageName, g_pkgName.c_str())) {
                if ((flags & 0x40) != 0) {
                    Class packageInfoClass(env, "android.content.pm.PackageInfo");
                    auto applicationInfoField = packageInfoClass.getField("applicationInfo");
                    auto applicationInfo = applicationInfoField.get(packageInfo);
                    if (applicationInfo) {
                        Class applicationInfoClass(env, "android.content.pm.ApplicationInfo");
                        auto sourceDirField = applicationInfoClass.getField("sourceDir");
                        auto publicSourceDirField = applicationInfoClass.getField("publicSourceDir");

                        sourceDirField.set(applicationInfo, g_apkPath);
                        publicSourceDirField.set(applicationInfo, g_apkPath);
                    }
                    applicationInfoField.set(packageInfo, applicationInfo);
                    auto signaturesField = packageInfoClass.getField("signatures");

                    auto signatureClass = env->FindClass("android/content/pm/Signature");
                    auto signatureConstructor = env->GetMethodID(signatureClass, "<init>", "([B)V");
                    auto signatureArray = env->NewObjectArray(apk_signatures.size(), signatureClass, NULL);
                    for (int i = 0; i < apk_signatures.size(); i++) {
                        auto signature = env->NewByteArray(apk_signatures[i].size());
                        env->SetByteArrayRegion(signature, 0, apk_signatures[i].size(), (jbyte *)apk_signatures[i].data());
                        env->SetObjectArrayElement(signatureArray, i, env->NewObject(signatureClass, signatureConstructor, signature));
                    }
                    signaturesField.set(packageInfo, signatureArray);
                } else if ((flags & 0x8000000) != 0) {
                    Class packageInfoClass(env, "android.content.pm.PackageInfo");
                    auto applicationInfoField = packageInfoClass.getField("applicationInfo");
                    auto applicationInfo = applicationInfoField.get(packageInfo);
                    if (applicationInfo) {
                        Class applicationInfoClass(env, "android.content.pm.ApplicationInfo");
                        auto sourceDirField = applicationInfoClass.getField("sourceDir");
                        auto publicSourceDirField = applicationInfoClass.getField("publicSourceDir");

                        sourceDirField.set(applicationInfo, g_apkPath);
                        publicSourceDirField.set(applicationInfo, g_apkPath);
                    }
                    applicationInfoField.set(packageInfo, applicationInfo);

                    auto signingInfoField = packageInfoClass.getField("signingInfo");
                    auto signingInfo = signingInfoField.get(packageInfo);

                    Class signingInfoClass(env, "android.content.pm.SigningInfo");
                    auto mSigningDetailsField = signingInfoClass.getField("mSigningDetails");
                    auto mSigningDetails = mSigningDetailsField.get(signingInfo);

                    Class signingDetailsClass(env, "android.content.pm.PackageParser$SigningDetails");
                    auto signaturesField = signingDetailsClass.getField("signatures");
                    auto pastSigningCertificatesField = signingDetailsClass.getField("pastSigningCertificates");

                    auto signatureClass = env->FindClass("android/content/pm/Signature");
                    auto signatureConstructor = env->GetMethodID(signatureClass, "<init>", "([B)V");
                    auto signatureArray = env->NewObjectArray(apk_signatures.size(), signatureClass, NULL);
                    for (int i = 0; i < apk_signatures.size(); i++) {
                        auto signature = env->NewByteArray(apk_signatures[i].size());
                        env->SetByteArrayRegion(signature, 0, apk_signatures[i].size(), (jbyte *)apk_signatures[i].data());
                        env->SetObjectArrayElement(signatureArray, i, env->NewObject(signatureClass, signatureConstructor, signature));
                    }
                    signaturesField.set(mSigningDetails, signatureArray);
                    pastSigningCertificatesField.set(mSigningDetails, signatureArray);
                } else {
                    Class packageInfoClass(env, "android.content.pm.PackageInfo");
                    auto applicationInfoField = packageInfoClass.getField("applicationInfo");
                    auto applicationInfo = applicationInfoField.get(packageInfo);
                    if (applicationInfo) {
                        Class applicationInfoClass(env, "android.content.pm.ApplicationInfo");
                        auto sourceDirField = applicationInfoClass.getField("sourceDir");
                        auto publicSourceDirField = applicationInfoClass.getField("publicSourceDir");

                        sourceDirField.set(applicationInfo, g_apkPath);
                        publicSourceDirField.set(applicationInfo, g_apkPath);
                    }
                    applicationInfoField.set(packageInfo, applicationInfo);
                }
            }
        }
    } else if (!strcmp(mName, "getApplicationInfo")) {
        const char *packageName = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(args, 0), 0);
        if (!strcmp(packageName, g_pkgName.c_str())) {
            auto applicationInfo = mResult;
            if (applicationInfo) {
                Class applicationInfoClass(env, "android.content.pm.ApplicationInfo");

                auto sourceDirField = applicationInfoClass.getField("sourceDir");
                auto publicSourceDirField = applicationInfoClass.getField("publicSourceDir");

                sourceDirField.set(applicationInfo, g_apkPath);
                publicSourceDirField.set(applicationInfo, g_apkPath);
            }
        }
    } else if (!strcmp(mName, "getInstallerPackageName")) {
        const char *packageName = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(args, 0), 0);
        if (!strcmp(packageName, g_pkgName.c_str())) {
            mResult = env->NewStringUTF("com.android.vending");
        }
    }

    if (mResult) {
        mResult = env->NewGlobalRef(mResult);
    }
    return env->PopLocalFrame(mResult); // make sure all local refs are deleted except for the result
}

const char *Killer::getProcessName() {
    FILE *f = fopen("/proc/self/cmdline", "rb");
    if (f) {
        char *buf = new char[64];
        fread(buf, sizeof(char), 64, f);
        fclose(f);
        return buf;
    }
    return 0;
}

std::string Killer::getPackageName(jobject obj) {
    auto contextClass = g_env->FindClass("android/content/Context");
    auto getPackageNameMethod = g_env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
    return g_env->GetStringUTFChars((jstring)g_env->CallObjectMethod(obj, getPackageNameMethod), 0);
}

jobject Killer::getApplicationContext(jobject obj) {
    auto contextClass = g_env->FindClass("android/content/Context");
    auto getApplicationContextMethod = g_env->GetMethodID(contextClass, "getApplicationContext", "()Landroid/content/Context;");
    return g_env->CallObjectMethod(obj, getApplicationContextMethod);
}

jobject Killer::getPackageManager(jobject obj) {
    auto contextClass = g_env->FindClass("android/content/Context");
    auto getPackageManagerMethod = g_env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    return g_env->CallObjectMethod(obj, getPackageManagerMethod);
}

void Killer::patch_ApplicationInfo(jobject obj) {
    if (obj) {
        LOGI("-------- Patching ApplicationInfo - %p", obj);
        Class applicationInfoClass("android.content.pm.ApplicationInfo");

        auto sourceDirField = applicationInfoClass.getField("sourceDir");
        auto publicSourceDirField = applicationInfoClass.getField("publicSourceDir");

        sourceDirField.set(obj, g_apkPath);
        publicSourceDirField.set(obj, g_apkPath);
    }
}

void Killer::patch_LoadedApk(jobject obj) {
    if (obj) {
        LOGI("-------- Patching LoadedApk - %p", obj);
        Class loadedApkClass("android.app.LoadedApk");

        auto mApplicationInfoField = loadedApkClass.getField("mApplicationInfo");
        patch_ApplicationInfo(mApplicationInfoField.get(obj));

        auto mAppDirField = loadedApkClass.getField("mAppDir");
        auto mResDirField = loadedApkClass.getField("mResDir");

        mAppDirField.set(obj, g_apkPath);
        mResDirField.set(obj, g_apkPath);
    }
}

void Killer::patch_AppBindData(jobject obj) {
    if (obj) {
        LOGI("-------- Patching AppBindData - %p", obj);
        Class appBindDataClass("android.app.ActivityThread$AppBindData");

        auto infoField = appBindDataClass.getField("info");
        patch_LoadedApk(infoField.get(obj));

        auto appInfoField = appBindDataClass.getField("appInfo");
        patch_ApplicationInfo(appInfoField.get(obj));
    }
}

void Killer::patch_ContextImpl(jobject obj) {
    if (obj) {
        LOGI("-------- Patching ContextImpl - %p", obj);
        Class contextImplClass("android.app.ContextImpl");

        auto mPackageInfoField = contextImplClass.getField("mPackageInfo");
        patch_LoadedApk(mPackageInfoField.get(obj));

        auto mPackageManagerField = contextImplClass.getField("mPackageManager");
        mPackageManagerField.set(obj, g_proxy);
    }
}

void Killer::patch_Application(jobject obj) {
    if (obj) {
        LOGI("-------- Patching Application - %p", obj);
        Class applicationClass("android.app.Application");

        auto mLoadedApkField = applicationClass.getField("mLoadedApk");
        patch_LoadedApk(mLoadedApkField.get(obj));

        // patch_ContextImpl(getApplicationContext(obj));
    }
}

void Killer::extractAsset(std::string assetName, std::string extractPath) {
    LOGI("-------- Extracting %s to %s", assetName.c_str(), extractPath.c_str());
    AAssetManager *assetManager = g_assetManager;
    AAsset *asset = AAssetManager_open(assetManager, assetName.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        return;
    }

    int fd = open(extractPath.c_str(), O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        AAsset_close(asset);
        return;
    }

    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    int bytesRead;
    while ((bytesRead = AAsset_read(asset, buffer, BUFFER_SIZE)) > 0) {
        int bytesWritten = write(fd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            AAsset_close(asset);
            close(fd);
            return;
        }
    }

    AAsset_close(asset);
    close(fd);
}

void Killer::patch_PackageManager(jobject obj) {
    if (!obj)
        return;

    Class activityThreadClass("android.app.ActivityThread");
    auto sCurrentActivityThreadField = activityThreadClass.getField("sCurrentActivityThread");
    auto sCurrentActivityThread = sCurrentActivityThreadField.get(NULL);

    auto sPackageManagerField = activityThreadClass.getField("sPackageManager");
    g_pkgMgr = g_env->NewGlobalRef(sPackageManagerField.get(NULL));

    Class iPackageManagerClass("android.content.pm.IPackageManager");

    auto classClass = g_env->FindClass("java/lang/Class");
    auto getClassLoaderMethod = g_env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");

    auto classLoader = g_env->CallObjectMethod(iPackageManagerClass.getClass(), getClassLoaderMethod);
    auto classArray = g_env->NewObjectArray(1, classClass, NULL);
    g_env->SetObjectArrayElement(classArray, 0, iPackageManagerClass.getClass());

    auto apkKillerClass = g_env->FindClass("com/nghilq/agnoster/Agnoster");
    auto myInvocationHandlerField = g_env->GetStaticFieldID(apkKillerClass, "myInvocationHandler", "Ljava/lang/reflect/InvocationHandler;");
    auto myInvocationHandler = g_env->GetStaticObjectField(apkKillerClass, myInvocationHandlerField);

    auto proxyClass = g_env->FindClass("java/lang/reflect/Proxy");
    auto newProxyInstanceMethod = g_env->GetStaticMethodID(proxyClass, "newProxyInstance", "(Ljava/lang/ClassLoader;[Ljava/lang/Class;Ljava/lang/reflect/InvocationHandler;)Ljava/lang/Object;");
    g_proxy = g_env->NewGlobalRef(g_env->CallStaticObjectMethod(proxyClass, newProxyInstanceMethod, classLoader, classArray, myInvocationHandler));

    sPackageManagerField.set(sCurrentActivityThread, g_proxy);

    auto pm = getPackageManager(obj);
    Class applicationPackageManagerClass("android.app.ApplicationPackageManager");
    auto mPMField = applicationPackageManagerClass.getField("mPM");
    mPMField.set(pm, g_proxy);
}
