package com.sjl.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.sjl.ffmpeg.util.FFmpegUtil;

public class FFmpegBaseActivity extends Activity {
    private TextView tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg_base);

        initView();
    }

    private void initView() {
        tv = findViewById(R.id.tv);
        findViewById(R.id.btnConfiguration).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(FFmpegUtil.configuration());
            }
        });
        findViewById(R.id.btnUrlProtocolInfo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(FFmpegUtil.urlprotocolinfo());
            }
        });
        findViewById(R.id.btnAvformatInfo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(FFmpegUtil.avformatinfo());
            }
        });
        findViewById(R.id.btnAvcodecInfo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(FFmpegUtil.avcodecinfo());
            }
        });
        findViewById(R.id.btnAvfilterInfo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(FFmpegUtil.avfilterinfo());
            }
        });
    }
}
