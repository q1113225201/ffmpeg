package com.sjl.ffmpeg.util.ffmpeg;

/**
 * FFmpegUtil
 *
 * @author SJL
 * @date 2017/8/8
 */

public class FFmpegUtil {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String configuration();
    public static native String urlprotocolinfo();
    public static native String avformatinfo();
    public static native String avcodecinfo();
    public static native String avfilterinfo();
    public static native int decode(String input, String output);
}