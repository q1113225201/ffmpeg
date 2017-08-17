package com.sjl.ffmpegdecoder;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.view.View;
import android.widget.TextView;

import com.sjl.baselib.config.PathConfig;
import com.sjl.baselib.util.FileUtil;
import com.sjl.baselib.util.PermisstionUtil;
import com.sjl.baselib.util.ToastUtil;
import com.sjl.ffmpegdecoder.util.FFmpegUtil;

import java.io.IOException;

public class MainActivity extends Activity {
    private Context context;
    private TextView tv;
    Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            tv.setText(msg.obj.toString());
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
    }

    private void initView() {
        context = this;
        tv = findViewById(R.id.tv);
        findViewById(R.id.btnToYUVH264).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FileUtil.deleteFile(PathConfig.BASE_PATH + "result.yuv");
                FileUtil.deleteFile(PathConfig.BASE_PATH + "result.h264");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        int result = FFmpegUtil.decoder(MainActivity.this,PathConfig.ORIGN_MP4, PathConfig.BASE_PATH + "result.yuv", PathConfig.BASE_PATH + "result.h264");
                        sendMessage("result:" + result);
//                        FFmpegUtil.msgFromJNI(MainActivity.this);
                    }
                }).start();

            }
        });
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

    public void sendMessage(String message){
        Message msg = new Message();
        msg.obj = message;
        handler.sendMessage(msg);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermisstionUtil.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
}
