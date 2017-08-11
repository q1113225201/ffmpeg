package com.sjl.baselib.config;

import android.os.Environment;

/**
 * PathConfig
 *
 * @author SJL
 * @date 2017/8/9
 */

public class PathConfig {
    /**
     * 根目录
     */
    public static String BASE_PATH = Environment.getExternalStorageDirectory() + "/ffmpeg/";
    /**
     * 原始mp4路径
     */
    public static String ORIGN_MP4 = BASE_PATH+"orign.mp4";
}
