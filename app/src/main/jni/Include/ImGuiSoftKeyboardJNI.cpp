#include "ImGuiSoftKeyboard.h"
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "ImGuiSoftKeyboardJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// JNI method implementations for ImGuiKeyboardBridge
extern "C" {

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeOnTextInput(JNIEnv* env, jclass clazz, jstring text) {
    ImGuiSoftKeyboard::jniOnTextInput(env, clazz, text);
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeOnKeyDown(JNIEnv* env, jclass clazz, jint keyCode) {
    ImGuiSoftKeyboard::jniOnKeyDown(env, clazz, keyCode);
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeOnKeyUp(JNIEnv* env, jclass clazz, jint keyCode) {
    ImGuiSoftKeyboard::jniOnKeyUp(env, clazz, keyCode);
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeOnKeyboardShow(JNIEnv* env, jclass clazz) {
    ImGuiSoftKeyboard::jniOnKeyboardShow(env, clazz);
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeOnKeyboardHide(JNIEnv* env, jclass clazz) {
    ImGuiSoftKeyboard::jniOnKeyboardHide(env, clazz);
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeSetInputFocus(JNIEnv* env, jclass clazz, jboolean focused) {
    if (g_softKeyboard) {
        if (focused) {
            // Focus is handled by ImGui input system
            LOGD("Input focus set from Java");
        } else {
            g_softKeyboard->clearFocus();
        }
    }
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeSetInputText(JNIEnv* env, jclass clazz, jstring text) {
    if (g_softKeyboard) {
        const char* ctext = env->GetStringUTFChars(text, nullptr);
        g_softKeyboard->setInputText(std::string(ctext));
        env->ReleaseStringUTFChars(text, ctext);
    }
}

JNIEXPORT jstring JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeGetInputText(JNIEnv* env, jclass clazz) {
    if (g_softKeyboard) {
        std::string text = g_softKeyboard->getInputText();
        return env->NewStringUTF(text.c_str());
    }
    return env->NewStringUTF("");
}

JNIEXPORT void JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeSetCursorPosition(JNIEnv* env, jclass clazz, jint position) {
    if (g_softKeyboard) {
        g_softKeyboard->setCursorPosition(position);
    }
}

JNIEXPORT jint JNICALL
Java_com_reveny_imgui_mod_ImGuiKeyboardBridge_nativeGetCursorPosition(JNIEnv* env, jclass clazz) {
    if (g_softKeyboard) {
        return g_softKeyboard->getCursorPosition();
    }
    return 0;
}

} // extern "C"