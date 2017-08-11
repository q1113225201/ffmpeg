package com.sjl.ffmpegcmd;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.sjl.baselib.config.PathConfig;
import com.sjl.ffmpegcmd.util.FFmpegUtil;

public class FFmpegCmdActivity extends Activity {
    private EditText et;
    private TextView tvResult;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg_cmd);

        initView();
    }

    private void initView() {
        et=findViewById(R.id.et);
        tvResult=findViewById(R.id.tvResult);
        findViewById(R.id.btnTransform).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tvResult.setText("开始转换");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String transform = et.getText().toString();
                        final String cmd = String.format("ffmpeg -i %s -f %s %s", PathConfig.ORIGN_MP4,transform.substring(transform.lastIndexOf(".")+1),PathConfig.BASE_PATH+transform);
                        final int result = FFmpegUtil.runCmd(cmd);
//                        final int result = FFmpegUtil.runCmd(String.format("ffmpeg -i %s -vcodec copy -an -f m4v %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform));
//                        final int result = FFmpegUtil.runCmd(String.format("ffmpeg -i %s -vframes 30 -y -f gif %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform));
//                        final int result = FFmpegUtil.runCmd(String.format("-i %s -f avi sdcard/output.avi", PathConfig.ORIGN_MP4));
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                tvResult.setText(String.format("cmd=%s\nresult=%d",cmd,result));
                            }
                        });
                    }
                }).start();
            }
        });

    }
}
