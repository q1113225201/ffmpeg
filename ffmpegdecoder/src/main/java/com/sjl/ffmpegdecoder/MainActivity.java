package com.sjl.ffmpegdecoder;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;

import com.sjl.baselib.config.PathConfig;
import com.sjl.baselib.util.FileUtil;
import com.sjl.baselib.util.PermisstionUtil;
import com.sjl.baselib.util.ToastUtil;

import java.io.IOException;

public class MainActivity extends Activity {
    private Context context;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
    }

    private void initView() {
        context = this;
        PermisstionUtil.requestPermissions(context, new PermisstionUtil.OnPermissionResult() {
            @Override
            public void granted(int requestCode) {
                if (!FileUtil.isFileExist(PathConfig.ORIGN_MP4)) {
                    try {
                        FileUtil.copyAssetsFile(context, "orign.mp4", PathConfig.ORIGN_MP4);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            @Override
            public void denied(int requestCode) {
                ToastUtil.showToast(context, "读写权限被禁止");
            }
        }, "正在请求文件读写权限", 100, PermisstionUtil.STORAGE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermisstionUtil.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
}
