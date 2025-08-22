# Android Soft Keyboard Integration for ImGui

This project provides real Android soft keyboard support for ImGui applications, replacing the need for virtual manual keyboards with the native Android Input Method Framework (IMF).

## Features

- **Real Android Soft Keyboard**: Uses the actual Android system keyboard, not a virtual one
- **Seamless ImGui Integration**: Drop-in replacement for `ImGui::InputText` and `ImGui::InputTextMultiline`
- **Automatic Focus Management**: Automatically shows/hides keyboard when input fields are focused
- **Full Unicode Support**: Supports all languages and input methods available on the device
- **Cursor Position Sync**: Maintains cursor position between ImGui and Android keyboard
- **Input Type Support**: Configurable input types (text, number, email, etc.)
- **Hint Text Support**: Shows placeholder text in the keyboard

## Architecture

The system consists of three main components:

1. **Java Layer** (`SoftKeyboardManager.java`): Manages Android Input Method Framework
2. **JNI Bridge** (`ImGuiKeyboardBridge.java`): Connects Java and C++ layers
3. **C++ Layer** (`ImGuiSoftKeyboard.h/cpp`): Integrates with ImGui

## Usage

### Basic Text Input

Replace your existing `ImGui::InputText` calls with:

```cpp
static std::string text = "Default text";
if (ImGui::InputTextWithSoftKeyboard("Label", text)) {
    // Text changed
}
```

### Multiline Text Input

```cpp
static std::string multilineText = "Multiline text...";
if (ImGui::InputTextMultilineWithSoftKeyboard("Label", multilineText, ImVec2(300, 100))) {
    // Text changed
}
```

### Manual Keyboard Control

```cpp
// Show keyboard manually
ImGui::ShowSoftKeyboard();

// Hide keyboard manually
ImGui::HideSoftKeyboard();

// Check keyboard visibility
bool visible = ImGui::IsSoftKeyboardVisible();
```

### Advanced Configuration

```cpp
// Set input type (text, number, email, etc.)
keyboard->setInputType(android.view.inputmethod.EditorInfo.TYPE_CLASS_TEXT);

// Set hint text
keyboard->setHint("Enter your text here...");

// Set cursor position
keyboard->setCursorPosition(5);
```

## Implementation Details

### Java Side

The `SoftKeyboardManager` creates a hidden `EditText` that receives keyboard input and forwards it to the C++ layer through callbacks.

### JNI Bridge

`ImGuiKeyboardBridge` provides static methods that can be called from C++ to control the keyboard and receive input events.

### C++ Side

`ImGuiSoftKeyboard` integrates with ImGui by:
- Intercepting input field focus events
- Managing text synchronization between ImGui and Android
- Providing custom input functions that use the soft keyboard

## Setup

1. **Include the header**:
```cpp
#include "ImGuiSoftKeyboard.h"
```

2. **Initialize the system**:
```cpp
// In your JNI_OnLoad or initialization function
g_softKeyboard = &ImGuiSoftKeyboard::getInstance();
g_softKeyboard->initialize();
```

3. **Register JNI methods**:
```cpp
// This is done automatically in the provided implementation
ImGuiSoftKeyboard::registerJNIMethods(env);
```

4. **Update each frame**:
```cpp
// In your main loop
g_softKeyboard->update();
```

## Input Types

The system supports various Android input types:

- `EditorInfo.TYPE_CLASS_TEXT`: General text input
- `EditorInfo.TYPE_CLASS_NUMBER`: Numeric input
- `EditorInfo.TYPE_CLASS_PHONE`: Phone number input
- `EditorInfo.TYPE_CLASS_DATETIME`: Date/time input
- `EditorInfo.TYPE_TEXT_VARIATION_EMAIL_ADDRESS`: Email input
- `EditorInfo.TYPE_TEXT_VARIATION_PASSWORD`: Password input

## Event Handling

The system automatically handles:
- Text input changes
- Keyboard show/hide events
- Special key events (Back, Enter, etc.)
- Focus changes

## Performance Considerations

- The system uses efficient JNI calls with minimal overhead
- Text synchronization happens only when needed
- Input buffer is pre-allocated to avoid memory allocations during input

## Troubleshooting

### Keyboard Not Showing
- Ensure the `SoftKeyboardManager` is properly initialized
- Check that the input field is receiving focus events
- Verify JNI methods are registered correctly

### Text Not Syncing
- Check that `update()` is called each frame
- Verify the input field is properly focused
- Ensure JNI bridge is working correctly

### Compilation Errors
- Make sure all source files are included in your build system
- Verify C++17 support is enabled
- Check that JNI headers are available

## Example Integration

See `Main.cpp` for a complete example of how to integrate the soft keyboard with your ImGui application.

## Dependencies

- Android NDK
- ImGui library
- C++17 compiler support
- Android Input Method Framework

## License

This implementation follows the same license as the main project.

## Contributing

Feel free to submit issues and enhancement requests. The system is designed to be extensible for additional input types and features.