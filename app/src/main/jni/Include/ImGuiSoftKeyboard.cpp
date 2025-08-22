#include "ImGuiSoftKeyboard.h"
#include <android/log.h>
#include <jni.h>
#include <cstdarg>
#include <cstring>

// Android logging
#define LOG_TAG "ImGuiSoftKeyboard"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Static member initialization
JavaVM* ImGuiSoftKeyboard::s_jvm = nullptr;
jclass ImGuiSoftKeyboard::s_bridgeClass = nullptr;
jmethodID ImGuiSoftKeyboard::s_showKeyboardMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_hideKeyboardMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_isKeyboardVisibleMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_setTextMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_getTextMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_setCursorPositionMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_getCursorPositionMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_setInputTypeMethod = nullptr;
jmethodID ImGuiSoftKeyboard::s_setHintMethod = nullptr;

// Global instance
ImGuiSoftKeyboard* g_softKeyboard = nullptr;

// Constructor
ImGuiSoftKeyboard::ImGuiSoftKeyboard()
    : m_initialized(false)
    , m_keyboardVisible(false)
    , m_cursorPosition(0)
    , m_focusedInputLabel(nullptr)
    , m_inputFocused(false)
    , m_inputType(0)
{
    m_inputBuffer.resize(MAX_INPUT_BUFFER_SIZE);
    m_inputBuffer[0] = '\0';
}

// Destructor
ImGuiSoftKeyboard::~ImGuiSoftKeyboard() {
    shutdown();
}

// Get singleton instance
ImGuiSoftKeyboard& ImGuiSoftKeyboard::getInstance() {
    static ImGuiSoftKeyboard instance;
    return instance;
}

// Initialize the soft keyboard system
bool ImGuiSoftKeyboard::initialize() {
    if (m_initialized) return true;
    
    LOGI("Initializing ImGui Soft Keyboard");
    
    // Initialize input buffer
    m_inputBuffer.resize(MAX_INPUT_BUFFER_SIZE);
    m_inputBuffer[0] = '\0';
    
    m_initialized = true;
    LOGI("ImGui Soft Keyboard initialized successfully");
    
    return true;
}

// Shutdown the soft keyboard system
void ImGuiSoftKeyboard::shutdown() {
    if (!m_initialized) return;
    
    LOGI("Shutting down ImGui Soft Keyboard");
    
    // Clear focus
    clearFocus();
    
    // Hide keyboard if visible
    if (m_keyboardVisible) {
        hideKeyboard();
    }
    
    m_initialized = false;
    LOGI("ImGui Soft Keyboard shutdown complete");
}

// Check if keyboard is currently visible
bool ImGuiSoftKeyboard::isKeyboardVisible() const {
    return m_keyboardVisible;
}

// Show the soft keyboard
void ImGuiSoftKeyboard::showKeyboard() {
    if (!m_initialized) return;
    
    LOGD("Showing soft keyboard");
    
    if (s_showKeyboardMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            env->CallStaticVoidMethod(s_bridgeClass, s_showKeyboardMethod);
            m_keyboardVisible = true;
        }
    }
}

// Hide the soft keyboard
void ImGuiSoftKeyboard::hideKeyboard() {
    if (!m_initialized) return;
    
    LOGD("Hiding soft keyboard");
    
    if (s_hideKeyboardMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            env->CallStaticVoidMethod(s_bridgeClass, s_hideKeyboardMethod);
            m_keyboardVisible = false;
        }
    }
}

// Handle text input from Android keyboard
void ImGuiSoftKeyboard::onTextInput(const std::string& text) {
    if (!m_initialized) return;
    
    LOGD("Text input received: %s", text.c_str());
    
    // Update current text
    m_currentText = text;
    
    // Update input buffer for ImGui
    updateInputBuffer();
    
    // Sync with Java side
    syncTextWithJava();
}

// Handle key events from Android keyboard
void ImGuiSoftKeyboard::onKeyDown(int keyCode) {
    if (!m_initialized) return;
    
    LOGD("Key down: %d", keyCode);
    
    // Handle special keys
    switch (keyCode) {
        case 4: // KEYCODE_BACK
            if (m_keyboardVisible) {
                hideKeyboard();
                clearFocus();
            }
            break;
        case 66: // KEYCODE_ENTER
            if (m_inputFocused) {
                // Handle enter key in input field
                LOGD("Enter key pressed in input field");
            }
            break;
        default:
            break;
    }
}

// Handle key events from Android keyboard
void ImGuiSoftKeyboard::onKeyUp(int keyCode) {
    if (!m_initialized) return;
    
    LOGD("Key up: %d", keyCode);
}

// Handle keyboard show events
void ImGuiSoftKeyboard::onKeyboardShow() {
    if (!m_initialized) return;
    
    LOGD("Keyboard shown");
    m_keyboardVisible = true;
}

// Handle keyboard hide events
void ImGuiSoftKeyboard::onKeyboardHide() {
    if (!m_initialized) return;
    
    LOGD("Keyboard hidden");
    m_keyboardVisible = false;
    clearFocus();
}

// Set input text
void ImGuiSoftKeyboard::setInputText(const std::string& text) {
    m_currentText = text;
    updateInputBuffer();
    syncTextWithJava();
}

// Get input text
std::string ImGuiSoftKeyboard::getInputText() const {
    return m_currentText;
}

// Set cursor position
void ImGuiSoftKeyboard::setCursorPosition(int position) {
    m_cursorPosition = position;
    syncCursorWithJava();
}

// Get cursor position
int ImGuiSoftKeyboard::getCursorPosition() const {
    return m_cursorPosition;
}

// Process input for ImGui
void ImGuiSoftKeyboard::processInput() {
    if (!m_initialized) return;
    
    // Process any pending input events
    // This is called from the main ImGui loop
}

// Custom ImGui::InputText that uses soft keyboard
bool ImGuiSoftKeyboard::inputText(const char* label, std::string& text, ImGuiInputTextFlags flags) {
    if (!m_initialized) return false;
    
    // Check if this input field is focused
    bool isFocused = (m_focusedInputLabel == label);
    
    // Use ImGui's InputText for display
    bool changed = ImGui::InputText(label, m_inputBuffer.data(), m_inputBuffer.size(), flags);
    
    // Check if this input field was clicked/focused
    if (ImGui::IsItemClicked() || ImGui::IsItemFocused()) {
        if (!isFocused) {
            setFocusedInput(label);
            setInputText(text);
            showKeyboard();
        }
    }
    
    // Update text from buffer if changed
    if (changed) {
        text = std::string(m_inputBuffer.data());
        setInputText(text);
    }
    
    // Update buffer from current text
    if (isFocused) {
        strncpy(m_inputBuffer.data(), m_currentText.c_str(), m_inputBuffer.size() - 1);
        m_inputBuffer[m_inputBuffer.size() - 1] = '\0';
    }
    
    return changed;
}

// Custom ImGui::InputTextMultiline that uses soft keyboard
bool ImGuiSoftKeyboard::inputTextMultiline(const char* label, std::string& text, const ImVec2& size, ImGuiInputTextFlags flags) {
    if (!m_initialized) return false;
    
    // Check if this input field is focused
    bool isFocused = (m_focusedInputLabel == label);
    
    // Use ImGui's InputTextMultiline for display
    bool changed = ImGui::InputTextMultiline(label, m_inputBuffer.data(), m_inputBuffer.size(), size, flags);
    
    // Check if this input field was clicked/focused
    if (ImGui::IsItemClicked() || ImGui::IsItemFocused()) {
        if (!isFocused) {
            setFocusedInput(label);
            setInputText(text);
            showKeyboard();
        }
    }
    
    // Update text from buffer if changed
    if (changed) {
        text = std::string(m_inputBuffer.data());
        setInputText(text);
    }
    
    // Update buffer from current text
    if (isFocused) {
        strncpy(m_inputBuffer.data(), m_currentText.c_str(), m_inputBuffer.size() - 1);
        m_inputBuffer[m_inputBuffer.size() - 1] = '\0';
    }
    
    return changed;
}

// Check if any input field is currently focused
bool ImGuiSoftKeyboard::isInputFocused() const {
    return m_inputFocused;
}

// Set the currently focused input field
void ImGuiSoftKeyboard::setFocusedInput(const char* label) {
    m_focusedInputLabel = label;
    m_inputFocused = true;
    
    LOGD("Input field focused: %s", label ? label : "null");
}

// Clear focus from all input fields
void ImGuiSoftKeyboard::clearFocus() {
    m_focusedInputLabel = nullptr;
    m_inputFocused = false;
    
    LOGD("Input focus cleared");
}

// Get the label of currently focused input
const char* ImGuiSoftKeyboard::getFocusedInputLabel() const {
    return m_focusedInputLabel;
}

// Set input type for keyboard
void ImGuiSoftKeyboard::setInputType(int inputType) {
    m_inputType = inputType;
    syncInputTypeWithJava();
}

// Set hint text for keyboard
void ImGuiSoftKeyboard::setHint(const std::string& hint) {
    m_hintText = hint;
    syncHintWithJava();
}

// Update method to be called each frame
void ImGuiSoftKeyboard::update() {
    if (!m_initialized) return;
    
    // Sync with Java side if needed
    if (m_inputFocused) {
        syncTextWithJava();
        syncCursorWithJava();
    }
}

// Update input buffer
void ImGuiSoftKeyboard::updateInputBuffer() {
    strncpy(m_inputBuffer.data(), m_currentText.c_str(), m_inputBuffer.size() - 1);
    m_inputBuffer[m_inputBuffer.size() - 1] = '\0';
}

// Sync text with Java side
void ImGuiSoftKeyboard::syncTextWithJava() {
    if (s_setTextMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            jstring jtext = env->NewStringUTF(m_currentText.c_str());
            env->CallStaticVoidMethod(s_bridgeClass, s_setTextMethod, jtext);
            env->DeleteLocalRef(jtext);
        }
    }
}

// Sync cursor with Java side
void ImGuiSoftKeyboard::syncCursorWithJava() {
    if (s_setCursorPositionMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            env->CallStaticVoidMethod(s_bridgeClass, s_setCursorPositionMethod, m_cursorPosition);
        }
    }
}

// Sync input type with Java side
void ImGuiSoftKeyboard::syncInputTypeWithJava() {
    if (s_setInputTypeMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            env->CallStaticVoidMethod(s_bridgeClass, s_setInputTypeMethod, m_inputType);
        }
    }
}

// Sync hint with Java side
void ImGuiSoftKeyboard::syncHintWithJava() {
    if (s_setHintMethod) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            jstring jhint = env->NewStringUTF(m_hintText.c_str());
            env->CallStaticVoidMethod(s_bridgeClass, s_setHintMethod, jhint);
            env->DeleteLocalRef(jhint);
        }
    }
}

// Get JNI environment
JNIEnv* ImGuiSoftKeyboard::getJNIEnv() {
    if (!s_jvm) return nullptr;
    
    JNIEnv* env = nullptr;
    jint result = s_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    if (result == JNI_EDETACHED) {
        result = s_jvm->AttachCurrentThread(&env, nullptr);
        if (result != JNI_OK) {
            LOGE("Failed to attach current thread to JVM");
            return nullptr;
        }
    }
    
    return env;
}

// JNI method implementations
void ImGuiSoftKeyboard::jniOnTextInput(JNIEnv* env, jclass clazz, jstring text) {
    const char* ctext = env->GetStringUTFChars(text, nullptr);
    if (g_softKeyboard) {
        g_softKeyboard->onTextInput(std::string(ctext));
    }
    env->ReleaseStringUTFChars(text, ctext);
}

void ImGuiSoftKeyboard::jniOnKeyDown(JNIEnv* env, jclass clazz, jint keyCode) {
    if (g_softKeyboard) {
        g_softKeyboard->onKeyDown(keyCode);
    }
}

void ImGuiSoftKeyboard::jniOnKeyUp(JNIEnv* env, jclass clazz, jint keyCode) {
    if (g_softKeyboard) {
        g_softKeyboard->onKeyUp(keyCode);
    }
}

void ImGuiSoftKeyboard::jniOnKeyboardShow(JNIEnv* env, jclass clazz) {
    if (g_softKeyboard) {
        g_softKeyboard->onKeyboardShow();
    }
}

void ImGuiSoftKeyboard::jniOnKeyboardHide(JNIEnv* env, jclass clazz) {
    if (g_softKeyboard) {
        g_softKeyboard->onKeyboardHide();
    }
}

// Register JNI methods
bool ImGuiSoftKeyboard::registerJNIMethods(JNIEnv* env) {
    // Get the ImGuiKeyboardBridge class
    jclass localClass = env->FindClass("com/reveny/imgui/mod/ImGuiKeyboardBridge");
    if (!localClass) {
        LOGE("Failed to find ImGuiKeyboardBridge class");
        return false;
    }
    
    // Create global reference
    s_bridgeClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);
    
    if (!s_bridgeClass) {
        LOGE("Failed to create global reference to ImGuiKeyboardBridge class");
        return false;
    }
    
    // Get method IDs
    s_showKeyboardMethod = env->GetStaticMethodID(s_bridgeClass, "showKeyboardStatic", "()V");
    s_hideKeyboardMethod = env->GetStaticMethodID(s_bridgeClass, "hideKeyboardStatic", "()V");
    s_isKeyboardVisibleMethod = env->GetStaticMethodID(s_bridgeClass, "isKeyboardVisibleStatic", "()Z");
    s_setTextMethod = env->GetStaticMethodID(s_bridgeClass, "setTextStatic", "(Ljava/lang/String;)V");
    s_getTextMethod = env->GetStaticMethodID(s_bridgeClass, "getTextStatic", "()Ljava/lang/String;");
    s_setCursorPositionMethod = env->GetStaticMethodID(s_bridgeClass, "setCursorPositionStatic", "(I)V");
    s_getCursorPositionMethod = env->GetStaticMethodID(s_bridgeClass, "getCursorPositionStatic", "()I");
    s_setInputTypeMethod = env->GetStaticMethodID(s_bridgeClass, "setInputType", "(I)V");
    s_setHintMethod = env->GetStaticMethodID(s_bridgeClass, "setHint", "(Ljava/lang/String;)V");
    
    // Check if all methods were found
    if (!s_showKeyboardMethod || !s_hideKeyboardMethod || !s_isKeyboardVisibleMethod ||
        !s_setTextMethod || !s_getTextMethod || !s_setCursorPositionMethod ||
        !s_getCursorPositionMethod || !s_setInputTypeMethod || !s_setHintMethod) {
        LOGE("Failed to find some JNI methods");
        return false;
    }
    
    LOGI("JNI methods registered successfully");
    return true;
}

// Convenience functions for ImGui integration
namespace ImGui {
    bool InputTextWithSoftKeyboard(const char* label, std::string& text, ImGuiInputTextFlags flags) {
        if (g_softKeyboard) {
            return g_softKeyboard->inputText(label, text, flags);
        }
        return false;
    }
    
    bool InputTextMultilineWithSoftKeyboard(const char* label, std::string& text, const ImVec2& size, ImGuiInputTextFlags flags) {
        if (g_softKeyboard) {
            return g_softKeyboard->inputTextMultiline(label, text, size, flags);
        }
        return false;
    }
    
    bool IsSoftKeyboardVisible() {
        return g_softKeyboard ? g_softKeyboard->isKeyboardVisible() : false;
    }
    
    void ShowSoftKeyboard() {
        if (g_softKeyboard) {
            g_softKeyboard->showKeyboard();
        }
    }
    
    void HideSoftKeyboard() {
        if (g_softKeyboard) {
            g_softKeyboard->hideKeyboard();
        }
    }
}