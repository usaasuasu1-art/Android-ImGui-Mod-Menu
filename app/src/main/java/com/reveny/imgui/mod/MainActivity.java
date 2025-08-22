package com.reveny.imgui.mod;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.reveny.imgui.mod.test.myGlSurfaceView;
import com.reveny.imgui.mod.SoftKeyboardManager;
import com.reveny.imgui.mod.ImGuiKeyboardBridge;

public class MainActivity extends Activity {

    private SoftKeyboardManager mSoftKeyboardManager;
    private ImGuiKeyboardBridge mKeyboardBridge;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize soft keyboard manager
        mSoftKeyboardManager = new SoftKeyboardManager(this);
        mKeyboardBridge = ImGuiKeyboardBridge.getInstance();
        mKeyboardBridge.initialize(mSoftKeyboardManager);

        //Taken from https://github.com/JimSeker/opengl/blob/master/OpenGL30Cube/app/src/main/java/edu/cs4730/opengl30cube/MainActivity.java
        if (detectOpenGLES30()) {
            //so we know it a opengl 3.0 and use our extended GLsurfaceview.
            setContentView(new myGlSurfaceView(this));
        } else {
            // This is where you could create an OpenGL ES 2.0 and/or 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2.
            Log.e("OpenGL", "OpenGL ES 3.0 not supported on device.");
            finish();
        }

        System.loadLibrary("Loader");
    }

    private boolean detectOpenGLES30() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x30000);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN);
        }
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mKeyboardBridge != null) {
            mKeyboardBridge.destroy();
        }
    }
}