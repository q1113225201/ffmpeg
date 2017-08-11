package com.sjl.ffmpeg.util;

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
    //读取基本信息
    public static native String configuration();
    public static native String urlprotocolinfo();
    public static native String avformatinfo();
    public static native String avcodecinfo();
    public static native String avfilterinfo();

    //获取视频信息
    public static native String getVideoInfo(String filename);
    //格式转换
    public static native int transform(String input, String output);
}
