package com.sjl.ffmpegcmd.util;

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
    //读取基本信息
    public static native String configuration();
    public static native String urlprotocolinfo();
    public static native String avformatinfo();
    public static native String avcodecinfo();
    public static native String avfilterinfo();

    public static native int runCmd(String cmd);
}
