package com.reactnativerandomvaluesjsihelper;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

@ReactModule(name = RandomValuesJsiHelperModule.NAME)
public class RandomValuesJsiHelperModule extends ReactContextBaseJavaModule {
    public static final String NAME = "RandomValuesJsiHelper";

    public RandomValuesJsiHelperModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    @NonNull
    public String getName() {
        return NAME;
    }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      Log.i(NAME, "Loading C++ library...");
      System.loadLibrary("reactnativerandomvaluesjsihelper");
      JavaScriptContextHolder jsContext = getReactApplicationContext().getJavaScriptContextHolder();
      Log.i(NAME, "Installing JSI Bindings...");
      nativeInstall(jsContext.get(), this);
      return true;
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
      return false;
    }
  }

  public byte[] getRandomBytes(int byteLength){
    byte[] data = new byte[byteLength];
    SecureRandom random = new SecureRandom();

    random.nextBytes(data);

    return data;
  }

  public static native void nativeInstall(long jsiPointer, RandomValuesJsiHelperModule instance);
}
