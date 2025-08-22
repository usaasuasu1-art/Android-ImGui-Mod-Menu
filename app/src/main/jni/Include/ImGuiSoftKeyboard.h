#pragma once

#include "../ImGui/imgui.h"
#include <string>
#include <vector>
#include <jni.h>

/**
 * ImGui Soft Keyboard Integration for Android
 * Provides real Android soft keyboard support for ImGui text inputs
 */
class ImGuiSoftKeyboard {
public:
    // Singleton instance
    static ImGuiSoftKeyboard& getInstance();
    
    // Initialize the soft keyboard system
    bool initialize();
    
    // Shutdown the soft keyboard system
    void shutdown();
    
    // Check if keyboard is currently visible
    bool isKeyboardVisible() const;
    
    // Show/hide the soft keyboard
    void showKeyboard();
    void hideKeyboard();
    
    // Handle text input from Android keyboard
    void onTextInput(const std::string& text);
    
    // Handle key events from Android keyboard
    void onKeyDown(int keyCode);
    void onKeyUp(int keyCode);
    
    // Handle keyboard show/hide events
    void onKeyboardShow();
    void onKeyboardHide();
    
    // Set input text and cursor position
    void setInputText(const std::string& text);
    std::string getInputText() const;
    void setCursorPosition(int position);
    int getCursorPosition() const;
    
    // Process input for ImGui (call this in your main loop)
    void processInput();
    
    // Custom ImGui::InputText that uses soft keyboard
    bool inputText(const char* label, std::string& text, ImGuiInputTextFlags flags = 0);
    
    // Custom ImGui::InputTextMultiline that uses soft keyboard
    bool inputTextMultiline(const char* label, std::string& text, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0);
    
    // Check if any input field is currently focused
    bool isInputFocused() const;
    
    // Set the currently focused input field
    void setFocusedInput(const char* label);
    
    // Clear focus from all input fields
    void clearFocus();
    
    // Get the label of currently focused input
    const char* getFocusedInputLabel() const;
    
    // Set input type for keyboard (text, number, email, etc.)
    void setInputType(int inputType);
    
    // Set hint text for keyboard
    void setHint(const std::string& hint);
    
    // Update method to be called each frame
    void update();
    
    // JNI methods for Java communication
    static void jniOnTextInput(JNIEnv* env, jclass clazz, jstring text);
    static void jniOnKeyDown(JNIEnv* env, jclass clazz, jint keyCode);
    static void jniOnKeyUp(JNIEnv* env, jclass clazz, jint keyCode);
    static void jniOnKeyboardShow(JNIEnv* env, jclass clazz);
    static void jniOnKeyboardHide(JNIEnv* env, jclass clazz);
    
    // Register JNI methods
    static bool registerJNIMethods(JNIEnv* env);

private:
    ImGuiSoftKeyboard();
    ~ImGuiSoftKeyboard();
    
    // Disable copy constructor and assignment
    ImGuiSoftKeyboard(const ImGuiSoftKeyboard&) = delete;
    ImGuiSoftKeyboard& operator=(const ImGuiSoftKeyboard&) = delete;
    
    // Internal state
    bool m_initialized;
    bool m_keyboardVisible;
    std::string m_currentText;
    int m_cursorPosition;
    const char* m_focusedInputLabel;
    bool m_inputFocused;
    int m_inputType;
    std::string m_hintText;
    
    // Text input buffer for ImGui
    std::vector<char> m_inputBuffer;
    static const size_t MAX_INPUT_BUFFER_SIZE = 1024;
    
    // JNI environment and class references
    static JavaVM* s_jvm;
    static jclass s_bridgeClass;
    static jmethodID s_showKeyboardMethod;
    static jmethodID s_hideKeyboardMethod;
    static jmethodID s_isKeyboardVisibleMethod;
    static jmethodID s_setTextMethod;
    static jmethodID s_getTextMethod;
    static jmethodID s_setCursorPositionMethod;
    static jmethodID s_getCursorPositionMethod;
    static jmethodID s_setInputTypeMethod;
    static jmethodID s_setHintMethod;
    
    // Helper methods
    void updateInputBuffer();
    void syncTextWithJava();
    void syncCursorWithJava();
    void syncInputTypeWithJava();
    void syncHintWithJava();
    
    // JNI helper methods
    JNIEnv* getJNIEnv();
    void callJavaMethod(jmethodID methodID, ...);
    bool callJavaBooleanMethod(jmethodID methodID, ...);
    std::string callJavaStringMethod(jmethodID methodID, ...);
    int callJavaIntMethod(jmethodID methodID, ...);
};

// Global instance
extern ImGuiSoftKeyboard* g_softKeyboard;

// Convenience functions for ImGui integration
namespace ImGui {
    // Custom InputText that uses soft keyboard
    bool InputTextWithSoftKeyboard(const char* label, std::string& text, ImGuiInputTextFlags flags = 0);
    
    // Custom InputTextMultiline that uses soft keyboard
    bool InputTextMultilineWithSoftKeyboard(const char* label, std::string& text, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0);
    
    // Check if soft keyboard is visible
    bool IsSoftKeyboardVisible();
    
    // Show/hide soft keyboard
    void ShowSoftKeyboard();
    void HideSoftKeyboard();
}