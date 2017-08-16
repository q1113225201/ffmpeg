package com.sjl.ffmpegcmd;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.sjl.baselib.config.PathConfig;
import com.sjl.baselib.util.FileUtil;
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
                updateTV("开始转换");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String transform = et.getText().toString();
                        String cmd = String.format("ffmpeg -i %s -f %s %s", PathConfig.ORIGN_MP4,transform.substring(transform.lastIndexOf(".")+1),PathConfig.BASE_PATH+transform);
                        FileUtil.deleteFile(PathConfig.BASE_PATH+transform);
                        int result = FFmpegUtil.runCmd(cmd);
                        updateTV(String.format("cmd=%s\nresult=%d",cmd,result));
                    }
                }).start();
            }
        });
        findViewById(R.id.btnGetSound).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                updateTV("开始提取声音");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String transform = "sound.mp3";
                        String cmd = String.format("ffmpeg -i %s -vn -ar 44100 -ac 2 -ab 192 -f mp3 %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform);
                        FileUtil.deleteFile(PathConfig.BASE_PATH+transform);
                        int result = FFmpegUtil.runCmd(cmd);
                        updateTV(String.format("cmd=%s\nresult=%d",cmd,result));
                    }
                }).start();
            }
        });
        findViewById(R.id.btnSplitImage).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                updateTV("视频分解");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String transform = "images/image%d.jpg";
//                        String cmd = String.format("ffmpeg -i %s -vn -ar 44100 -ac 2 -ab 192 -f mp3 %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform);
                        String cmd = String.format("ffmpeg -i %s %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform);
                        FileUtil.deleteFile(PathConfig.BASE_PATH+"images/");
                        FileUtil.createFile(PathConfig.BASE_PATH+"images/file.txt");
                        int result = FFmpegUtil.runCmd(cmd);
                        updateTV(String.format("cmd=%s\nresult=%d",cmd,result));
                    }
                }).start();
            }
        });
        findViewById(R.id.btnCompoundVideo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                updateTV("视频合成");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String transform = "video.avi";
//                        String cmd = String.format("ffmpeg -i %s -vn -ar 44100 -ac 2 -ab 192 -f mp3 %s", PathConfig.ORIGN_MP4,PathConfig.BASE_PATH+transform);
                        String cmd = String.format("ffmpeg -f image2 -i %s %s",PathConfig.BASE_PATH+"images/image%d.jpg", PathConfig.BASE_PATH+transform);
                        FileUtil.deleteFile(PathConfig.BASE_PATH+transform);
                        int result = FFmpegUtil.runCmd(cmd);
                        updateTV(String.format("cmd=%s\nresult=%d",cmd,result));
                    }
                }).start();
            }
        });
    }

    private void updateTV(final String str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvResult.setText(str);
            }
        });
    }


}
