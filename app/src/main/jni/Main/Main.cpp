//
// Created by reveny on 21/08/2023.
//

#include "../Include/KittyMemory/MemoryPatch.h"
#include "../Include/ImGui.h"
#include "../Include/RemapTools.h"
#include "../Include/ImGuiSoftKeyboard.h"
#include "../Include/ImGuiSoftKeyboardExample.h"

#include "../Include/Drawing.h"
#include "../Include/Unity.h"

void DrawMenu() {
    ImGui::ShowDemoWindow();
    
    // Demo of soft keyboard integration
    static std::string textInput = "Type here...";
    static std::string multilineInput = "Multiline input...";
    
    ImGui::Text("Soft Keyboard Demo");
    ImGui::Separator();
    
    // Single line input with soft keyboard
    if (ImGui::InputTextWithSoftKeyboard("Text Input", textInput)) {
        // Text changed
    }
    
    // Multiline input with soft keyboard
    if (ImGui::InputTextMultilineWithSoftKeyboard("Multiline Input", multilineInput, ImVec2(300, 100))) {
        // Text changed
    }
    
    // Keyboard control buttons
    if (ImGui::Button("Show Keyboard")) {
        ImGui::ShowSoftKeyboard();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Hide Keyboard")) {
        ImGui::HideSoftKeyboard();
    }
    
    ImGui::SameLine();
    
    ImGui::Text("Keyboard: %s", ImGui::IsSoftKeyboardVisible() ? "Visible" : "Hidden");
    
    // Display current input text
    ImGui::Text("Current Text: %s", textInput.c_str());
    ImGui::Text("Current Multiline: %s", multilineInput.c_str());
    
    ImGui::Separator();
    
    // Add tabs for different examples
    if (ImGui::BeginTabBar("SoftKeyboardExamples")) {
        if (ImGui::BeginTabItem("Basic Demo")) {
            // Basic demo content is already above
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Advanced Examples")) {
            DrawSoftKeyboardExamples();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Custom Input")) {
            DrawCustomInputExamples();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Autocomplete")) {
            DrawAutocompleteExamples();
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
}

void *thread(void *) {
    LOGI(OBFUSCATE("Main Thread Loaded: %d"), gettid());
    initModMenu((void *)DrawMenu);

    //Hooks, Patches and Pointers here
    //Example:
    /*
     * DobbyHook(getAbsoluteAddress("libIl2cpp.so", 0x0), FunctionExample, old_FunctionExample);
     * SetAimRotation = (void (*)(void *, Quaternion)) getAbsoluteAddress("libIl2cpp.so", 0x0);
     */

    LOGI("Main thread done");
    pthread_exit(0);
}

// Call anything from JNI_OnLoad here
extern "C" {
    // JNI Support
    JavaVM *jvm = nullptr;
    JNIEnv *env = nullptr;

    __attribute__((visibility ("default")))
    jint loadJNI(JavaVM *vm) {
        jvm = vm;
        vm->AttachCurrentThread(&env, nullptr);
        LOGI("loadJNI(): Initialized");
        
        // Initialize soft keyboard system
        if (env) {
            // Set JVM reference for soft keyboard
            ImGuiSoftKeyboard::s_jvm = jvm;
            
            // Register JNI methods
            if (ImGuiSoftKeyboard::registerJNIMethods(env)) {
                LOGI("Soft keyboard JNI methods registered successfully");
            } else {
                LOGE("Failed to register soft keyboard JNI methods");
            }
        }

        return JNI_VERSION_1_6;
    }
}

__attribute__((constructor))
void init() {
    LOGI("Loaded Mod Menu");

    // Initialize soft keyboard
    g_softKeyboard = &ImGuiSoftKeyboard::getInstance();
    if (g_softKeyboard->initialize()) {
        LOGI("Soft keyboard initialized successfully");
    } else {
        LOGE("Failed to initialize soft keyboard");
    }

    pthread_t t;
    pthread_create(&t, nullptr, thread, nullptr);

    //Don't leave any traces, remap the loader lib as well
    RemapTools::RemapLibrary("libLoader.so");
}