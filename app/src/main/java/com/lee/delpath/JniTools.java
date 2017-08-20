package com.lee.delpath;

/**
 * Created by DeepL on 2017/8/20 0020.
 */

public class JniTools {
    public native String stringFromJNI();
    public native boolean removePath(String path);
    static {
        System.loadLibrary("native-lib");
    }

}
