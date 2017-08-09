package com.sjl.ffmpeg;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.sjl.ffmpeg.config.PathConfig;
import com.sjl.ffmpeg.util.FileUtil;
import com.sjl.ffmpeg.util.PermisstionUtil;
import com.sjl.ffmpeg.util.ToastUtil;
import com.sjl.ffmpeg.util.ffmpeg.FFmpegUtil;

import java.io.IOException;

public class DecodeActivity extends Activity {
    private Context context;
    private EditText etOutput;
    private TextView tvState;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode);

        initView();
    }

    private void initView() {
        context = this;
        etOutput = findViewById(R.id.etOutput);
        tvState = findViewById(R.id.tvState);
        findViewById(R.id.btnTransform).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
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
                        transform(PathConfig.ORIGN_MP4, PathConfig.BASE_PATH + etOutput.getText().toString());
                    }

                    @Override
                    public void denied(int requestCode) {
                        ToastUtil.showToast(context, "读写权限被禁止");
                    }
                }, "正在请求文件读写权限", 100, PermisstionUtil.STORAGE);
            }
        });
    }

    /**
     * 格式转换
     * @param orign 原始文件
     * @param dest  目标文件
     */
    private void transform(String orign, String dest) {
        FileUtil.deleteFile(dest);
        tvState.setText("result="+FFmpegUtil.decode(orign,dest));
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermisstionUtil.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
}
