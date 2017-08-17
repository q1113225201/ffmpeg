package com.sjl.ffmpegdecoder.util;

/**
 * FFmpegUtil
 *
 * @author SJL
 * @date 2017/8/11
 */

public class FFmpegUtil {
    static {
        System.loadLibrary("native-lib");
    }
    public static native int decoder(Object object,String input,String outputYUV,String outputH264);
    public static native void msgFromJNI(Object object);
}
