package com.reveny.imgui.mod;

import android.app.Activity;
import android.content.Context;
import android.inputmethodservice.InputMethodService;
import android.os.Build;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * Manages Android soft keyboard integration with ImGui
 * This class provides a bridge between Android's Input Method Framework and ImGui
 */
public class SoftKeyboardManager {
    private static final String TAG = "SoftKeyboardManager";
    
    private Activity mActivity;
    private InputMethodManager mInputMethodManager;
    private EditText mHiddenEditText;
    private FrameLayout mKeyboardContainer;
    private boolean mIsKeyboardVisible = false;
    private boolean mIsInitialized = false;
    
    // Callback interface for keyboard events
    public interface KeyboardCallback {
        void onTextInput(String text);
        void onKeyDown(int keyCode);
        void onKeyUp(int keyCode);
        void onKeyboardShow();
        void onKeyboardHide();
    }
    
    private KeyboardCallback mCallback;
    
    public SoftKeyboardManager(Activity activity) {
        mActivity = activity;
        mInputMethodManager = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
        initializeKeyboard();
    }
    
    /**
     * Initialize the hidden EditText and keyboard container
     */
    private void initializeKeyboard() {
        if (mIsInitialized) return;
        
        // Create a hidden EditText that will receive keyboard input
        mHiddenEditText = new EditText(mActivity);
        mHiddenEditText.setVisibility(View.INVISIBLE);
        mHiddenEditText.setFocusable(true);
        mHiddenEditText.setFocusableInTouchMode(true);
        mHiddenEditText.setInputType(EditorInfo.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_VARIATION_NORMAL);
        
        // Set up text change listener
        mHiddenEditText.addTextChangedListener(new android.text.TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
            
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {}
            
            @Override
            public void afterTextChanged(android.text.Editable s) {
                if (mCallback != null) {
                    mCallback.onTextInput(s.toString());
                }
            }
        });
        
        // Set up key listener for special keys
        mHiddenEditText.setOnKeyListener((v, keyCode, event) -> {
            if (mCallback != null) {
                if (event.getAction() == android.view.KeyEvent.ACTION_DOWN) {
                    mCallback.onKeyDown(keyCode);
                } else if (event.getAction() == android.view.KeyEvent.ACTION_UP) {
                    mCallback.onKeyUp(keyCode);
                }
            }
            return false; // Let the system handle the key event
        });
        
        // Create container for the hidden EditText
        mKeyboardContainer = new FrameLayout(mActivity);
        mKeyboardContainer.addView(mHiddenEditText);
        mKeyboardContainer.setVisibility(View.INVISIBLE);
        
        // Add to activity's root view
        View rootView = mActivity.findViewById(android.R.id.content);
        if (rootView instanceof FrameLayout) {
            ((FrameLayout) rootView).addView(mKeyboardContainer);
        }
        
        mIsInitialized = true;
    }
    
    /**
     * Set the callback for keyboard events
     */
    public void setKeyboardCallback(KeyboardCallback callback) {
        mCallback = callback;
    }
    
    /**
     * Show the soft keyboard
     */
    public void showKeyboard() {
        if (!mIsInitialized) return;
        
        mHiddenEditText.requestFocus();
        mInputMethodManager.showSoftInput(mHiddenEditText, InputMethodManager.SHOW_IMPLICIT);
        mIsKeyboardVisible = true;
        
        if (mCallback != null) {
            mCallback.onKeyboardShow();
        }
    }
    
    /**
     * Hide the soft keyboard
     */
    public void hideKeyboard() {
        if (!mIsInitialized) return;
        
        mHiddenEditText.clearFocus();
        mInputMethodManager.hideSoftInputFromWindow(mHiddenEditText.getWindowToken(), 0);
        mIsKeyboardVisible = false;
        
        if (mCallback != null) {
            mCallback.onKeyboardHide();
        }
    }
    
    /**
     * Check if keyboard is currently visible
     */
    public boolean isKeyboardVisible() {
        return mIsKeyboardVisible;
    }
    
    /**
     * Set input type for the keyboard
     */
    public void setInputType(int inputType) {
        if (mHiddenEditText != null) {
            mHiddenEditText.setInputType(inputType);
        }
    }
    
    /**
     * Set hint text for the keyboard
     */
    public void setHint(String hint) {
        if (mHiddenEditText != null) {
            mHiddenEditText.setHint(hint);
        }
    }
    
    /**
     * Clear the text input
     */
    public void clearText() {
        if (mHiddenEditText != null) {
            mHiddenEditText.setText("");
        }
    }
    
    /**
     * Get current text from keyboard
     */
    public String getText() {
        return mHiddenEditText != null ? mHiddenEditText.getText().toString() : "";
    }
    
    /**
     * Set text to keyboard input
     */
    public void setText(String text) {
        if (mHiddenEditText != null) {
            mHiddenEditText.setText(text);
        }
    }
    
    /**
     * Set cursor position
     */
    public void setCursorPosition(int position) {
        if (mHiddenEditText != null) {
            mHiddenEditText.setSelection(position);
        }
    }
    
    /**
     * Get cursor position
     */
    public int getCursorPosition() {
        return mHiddenEditText != null ? mHiddenEditText.getSelectionStart() : 0;
    }
    
    /**
     * Clean up resources
     */
    public void destroy() {
        if (mKeyboardContainer != null && mKeyboardContainer.getParent() != null) {
            ((FrameLayout) mKeyboardContainer.getParent()).removeView(mKeyboardContainer);
        }
        mIsInitialized = false;
    }
}