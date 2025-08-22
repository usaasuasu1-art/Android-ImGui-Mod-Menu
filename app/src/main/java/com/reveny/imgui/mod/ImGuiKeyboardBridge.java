package com.reveny.imgui.mod;

import android.util.Log;

/**
 * JNI Bridge for ImGui keyboard integration
 * This class provides native methods to communicate between Java and C++
 */
public class ImGuiKeyboardBridge {
    private static final String TAG = "ImGuiKeyboardBridge";
    
    // Native method declarations
    public static native void nativeOnTextInput(String text);
    public static native void nativeOnKeyDown(int keyCode);
    public static native void nativeOnKeyUp(int keyCode);
    public static native void nativeOnKeyboardShow();
    public static native void nativeOnKeyboardHide();
    public static native void nativeSetInputFocus(boolean focused);
    public static native void nativeSetInputText(String text);
    public static native String nativeGetInputText();
    public static native void nativeSetCursorPosition(int position);
    public static native int nativeGetCursorPosition();
    
    // Static instance for singleton pattern
    private static ImGuiKeyboardBridge sInstance;
    private SoftKeyboardManager mKeyboardManager;
    
    // Callback implementation for SoftKeyboardManager
    private final SoftKeyboardManager.KeyboardCallback mKeyboardCallback = new SoftKeyboardManager.KeyboardCallback() {
        @Override
        public void onTextInput(String text) {
            Log.d(TAG, "Text input: " + text);
            nativeOnTextInput(text);
        }
        
        @Override
        public void onKeyDown(int keyCode) {
            Log.d(TAG, "Key down: " + keyCode);
            nativeOnKeyDown(keyCode);
        }
        
        @Override
        public void onKeyUp(int keyCode) {
            Log.d(TAG, "Key up: " + keyCode);
            nativeOnKeyUp(keyCode);
        }
        
        @Override
        public void onKeyboardShow() {
            Log.d(TAG, "Keyboard shown");
            nativeOnKeyboardShow();
        }
        
        @Override
        public void onKeyboardHide() {
            Log.d(TAG, "Keyboard hidden");
            nativeOnKeyboardHide();
        }
    };
    
    private ImGuiKeyboardBridge() {}
    
    /**
     * Get singleton instance
     */
    public static ImGuiKeyboardBridge getInstance() {
        if (sInstance == null) {
            sInstance = new ImGuiKeyboardBridge();
        }
        return sInstance;
    }
    
    /**
     * Initialize the bridge with SoftKeyboardManager
     */
    public void initialize(SoftKeyboardManager keyboardManager) {
        mKeyboardManager = keyboardManager;
        if (mKeyboardManager != null) {
            mKeyboardManager.setKeyboardCallback(mKeyboardCallback);
        }
    }
    
    /**
     * Show the soft keyboard
     */
    public void showKeyboard() {
        if (mKeyboardManager != null) {
            mKeyboardManager.showKeyboard();
        }
    }
    
    /**
     * Hide the soft keyboard
     */
    public void hideKeyboard() {
        if (mKeyboardManager != null) {
            mKeyboardManager.hideKeyboard();
        }
    }
    
    /**
     * Check if keyboard is visible
     */
    public boolean isKeyboardVisible() {
        return mKeyboardManager != null && mKeyboardManager.isKeyboardVisible();
    }
    
    /**
     * Set input type for keyboard
     */
    public void setInputType(int inputType) {
        if (mKeyboardManager != null) {
            mKeyboardManager.setInputType(inputType);
        }
    }
    
    /**
     * Set hint text
     */
    public void setHint(String hint) {
        if (mKeyboardManager != null) {
            mKeyboardManager.setHint(hint);
        }
    }
    
    /**
     * Set text to keyboard input
     */
    public void setText(String text) {
        if (mKeyboardManager != null) {
            mKeyboardManager.setText(text);
        }
    }
    
    /**
     * Get text from keyboard input
     */
    public String getText() {
        return mKeyboardManager != null ? mKeyboardManager.getText() : "";
    }
    
    /**
     * Set cursor position
     */
    public void setCursorPosition(int position) {
        if (mKeyboardManager != null) {
            mKeyboardManager.setCursorPosition(position);
        }
    }
    
    /**
     * Get cursor position
     */
    public int getCursorPosition() {
        return mKeyboardManager != null ? mKeyboardManager.getCursorPosition() : 0;
    }
    
    /**
     * Clean up resources
     */
    public void destroy() {
        if (mKeyboardManager != null) {
            mKeyboardManager.destroy();
            mKeyboardManager = null;
        }
    }
    
    // Static methods for easy access from native code
    public static void showKeyboardStatic() {
        if (sInstance != null) {
            sInstance.showKeyboard();
        }
    }
    
    public static void hideKeyboardStatic() {
        if (sInstance != null) {
            sInstance.hideKeyboard();
        }
    }
    
    public static boolean isKeyboardVisibleStatic() {
        return sInstance != null && sInstance.isKeyboardVisible();
    }
    
    public static void setTextStatic(String text) {
        if (sInstance != null) {
            sInstance.setText(text);
        }
    }
    
    public static String getTextStatic() {
        return sInstance != null ? sInstance.getText() : "";
    }
    
    public static void setCursorPositionStatic(int position) {
        if (sInstance != null) {
            sInstance.setCursorPosition(position);
        }
    }
    
    public static int getCursorPositionStatic() {
        return sInstance != null ? sInstance.getCursorPosition() : 0;
    }
}