#include "ImGuiSoftKeyboard.h"
#include <string>

/**
 * Example usage of ImGui Soft Keyboard integration
 * This file demonstrates various input types and configurations
 */
void DrawSoftKeyboardExamples() {
    // Static variables for different input types
    static std::string textInput = "Text input example";
    static std::string multilineInput = "Multiline input example\nSecond line\nThird line";
    static std::string numberInput = "12345";
    static std::string emailInput = "user@example.com";
    static std::string passwordInput = "password123";
    static std::string phoneInput = "+1234567890";
    
    ImGui::Begin("Soft Keyboard Examples");
    
    ImGui::Text("This demonstrates various input types with Android soft keyboard");
    ImGui::Separator();
    
    // Basic text input
    ImGui::Text("Basic Text Input:");
    if (ImGui::InputTextWithSoftKeyboard("Text", textInput)) {
        // Text changed
    }
    ImGui::Text("Current value: %s", textInput.c_str());
    
    ImGui::Spacing();
    
    // Multiline input
    ImGui::Text("Multiline Input:");
    if (ImGui::InputTextMultilineWithSoftKeyboard("Multiline", multilineInput, ImVec2(300, 100))) {
        // Text changed
    }
    ImGui::Text("Current value: %s", multilineInput.c_str());
    
    ImGui::Spacing();
    
    // Number input
    ImGui::Text("Number Input:");
    if (ImGui::InputTextWithSoftKeyboard("Number", numberInput)) {
        // Text changed
    }
    ImGui::Text("Current value: %s", numberInput.c_str());
    
    ImGui::Spacing();
    
    // Email input
    ImGui::Text("Email Input:");
    if (ImGui::InputTextWithSoftKeyboard("Email", emailInput)) {
        // Text changed
    }
    ImGui::Text("Current value: %s", emailInput.c_str());
    
    ImGui::Spacing();
    
    // Password input
    ImGui::Text("Password Input:");
    if (ImGui::InputTextWithSoftKeyboard("Password", passwordInput)) {
        // Text changed
    }
    ImGui::Text("Current value: %s", passwordInput.c_str());
    
    ImGui::Spacing();
    
    // Phone input
    ImGui::Text("Phone Input:");
    if (ImGui::InputTextWithSoftKeyboard("Phone", phoneInput)) {
        // Text changed
    }
    ImGui::Text("Current value: %s", phoneInput.c_str());
    
    ImGui::Separator();
    
    // Keyboard control buttons
    ImGui::Text("Keyboard Control:");
    
    if (ImGui::Button("Show Keyboard")) {
        ImGui::ShowSoftKeyboard();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Hide Keyboard")) {
        ImGui::HideSoftKeyboard();
    }
    
    ImGui::SameLine();
    
    ImGui::Text("Status: %s", ImGui::IsSoftKeyboardVisible() ? "Visible" : "Hidden");
    
    ImGui::Separator();
    
    // Advanced configuration examples
    ImGui::Text("Advanced Configuration:");
    
    if (ImGui::Button("Set Text Input Type")) {
        if (g_softKeyboard) {
            // Set to general text input
            g_softKeyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_CLASS_TEXT);
            g_softKeyboard->setHint("Enter text here...");
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Set Number Input Type")) {
        if (g_softKeyboard) {
            // Set to numeric input
            g_softKeyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_CLASS_NUMBER);
            g_softKeyboard->setHint("Enter numbers only...");
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Set Email Input Type")) {
        if (g_softKeyboard) {
            // Set to email input
            g_softKeyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
            g_softKeyboard->setHint("Enter email address...");
        }
    }
    
    ImGui::Spacing();
    
    if (ImGui::Button("Set Password Input Type")) {
        if (g_softKeyboard) {
            // Set to password input
            g_softKeyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_TEXT_VARIATION_PASSWORD);
            g_softKeyboard->setHint("Enter password...");
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Set Phone Input Type")) {
        if (g_softKeyboard) {
            // Set to phone input
            g_softKeyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_CLASS_PHONE);
            g_softKeyboard->setHint("Enter phone number...");
        }
    }
    
    ImGui::Separator();
    
    // Input validation examples
    ImGui::Text("Input Validation Examples:");
    
    // Email validation
    static bool emailValid = false;
    if (!emailInput.empty()) {
        size_t atPos = emailInput.find('@');
        size_t dotPos = emailInput.find('.', atPos);
        emailValid = (atPos != std::string::npos && dotPos != std::string::npos && dotPos > atPos + 1);
    }
    
    ImGui::Text("Email validation: %s", emailValid ? "Valid" : "Invalid");
    
    // Number validation
    static bool numberValid = false;
    if (!numberInput.empty()) {
        numberValid = (numberInput.find_first_not_of("0123456789") == std::string::npos);
    }
    
    ImGui::Text("Number validation: %s", numberValid ? "Valid" : "Invalid");
    
    // Phone validation
    static bool phoneValid = false;
    if (!phoneInput.empty()) {
        std::string digitsOnly = phoneInput;
        digitsOnly.erase(std::remove_if(digitsOnly.begin(), digitsOnly.end(), 
                                      [](char c) { return !std::isdigit(c); }), digitsOnly.end());
        phoneValid = (digitsOnly.length() >= 10);
    }
    
    ImGui::Text("Phone validation: %s", phoneValid ? "Valid" : "Invalid");
    
    ImGui::Separator();
    
    // Performance information
    ImGui::Text("Performance Information:");
    ImGui::Text("Input focused: %s", g_softKeyboard && g_softKeyboard->isInputFocused() ? "Yes" : "No");
    
    if (g_softKeyboard && g_softKeyboard->isInputFocused()) {
        ImGui::Text("Focused input: %s", g_softKeyboard->getFocusedInputLabel() ? g_softKeyboard->getFocusedInputLabel() : "None");
        ImGui::Text("Cursor position: %d", g_softKeyboard->getCursorPosition());
    }
    
    ImGui::End();
}

/**
 * Example of custom input validation and formatting
 */
void DrawCustomInputExamples() {
    static std::string customInput = "";
    static std::string formattedInput = "";
    
    ImGui::Begin("Custom Input Examples");
    
    ImGui::Text("Custom input with validation and formatting");
    ImGui::Separator();
    
    // Custom input with real-time formatting
    if (ImGui::InputTextWithSoftKeyboard("Custom Input", customInput)) {
        // Apply custom formatting
        formattedInput = customInput;
        
        // Example: Convert to uppercase
        std::transform(formattedInput.begin(), formattedInput.end(), formattedInput.begin(), ::toupper);
        
        // Example: Remove special characters
        formattedInput.erase(std::remove_if(formattedInput.begin(), formattedInput.end(), 
                                          [](char c) { return !std::isalnum(c) && c != ' '; }), formattedInput.end());
    }
    
    ImGui::Text("Original: %s", customInput.c_str());
    ImGui::Text("Formatted: %s", formattedInput.c_str());
    
    ImGui::Separator();
    
    // Input with character limit
    static std::string limitedInput = "";
    static const int maxLength = 50;
    
    if (ImGui::InputTextWithSoftKeyboard("Limited Input (50 chars)", limitedInput)) {
        if (limitedInput.length() > maxLength) {
            limitedInput = limitedInput.substr(0, maxLength);
        }
    }
    
    ImGui::Text("Length: %zu/%d", limitedInput.length(), maxLength);
    
    // Progress bar for character limit
    float progress = (float)limitedInput.length() / maxLength;
    ImGui::ProgressBar(progress, ImVec2(-1, 0), "");
    
    ImGui::End();
}

/**
 * Example of input with suggestions/autocomplete
 */
void DrawAutocompleteExamples() {
    static std::string searchInput = "";
    static std::vector<std::string> suggestions = {
        "Android", "ImGui", "Soft Keyboard", "JNI", "C++", "Java", "OpenGL", "NDK"
    };
    static std::vector<std::string> filteredSuggestions;
    
    ImGui::Begin("Autocomplete Examples");
    
    ImGui::Text("Input with autocomplete suggestions");
    ImGui::Separator();
    
    // Search input
    if (ImGui::InputTextWithSoftKeyboard("Search", searchInput)) {
        // Filter suggestions based on input
        filteredSuggestions.clear();
        if (!searchInput.empty()) {
            for (const auto& suggestion : suggestions) {
                if (suggestion.find(searchInput) != std::string::npos) {
                    filteredSuggestions.push_back(suggestion);
                }
            }
        }
    }
    
    // Show suggestions
    if (!searchInput.empty() && !filteredSuggestions.empty()) {
        ImGui::Text("Suggestions:");
        for (const auto& suggestion : filteredSuggestions) {
            if (ImGui::Selectable(suggestion.c_str())) {
                searchInput = suggestion;
                filteredSuggestions.clear();
            }
        }
    }
    
    ImGui::Separator();
    
    // Add new suggestions
    static std::string newSuggestion = "";
    if (ImGui::InputTextWithSoftKeyboard("Add Suggestion", newSuggestion)) {
        // Handle new suggestion input
    }
    
    if (ImGui::Button("Add to Suggestions") && !newSuggestion.empty()) {
        suggestions.push_back(newSuggestion);
        newSuggestion.clear();
    }
    
    ImGui::End();
}