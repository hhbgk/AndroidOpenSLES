package com.hhbgk.example.opensles;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends Activity implements View.OnClickListener {
    private OpenSlWrapper mOpenSlWrapper;
    private Button mStartBtn, mStopBtn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mOpenSlWrapper = new OpenSlWrapper();
        mStartBtn = findViewById(R.id.btn_start);
        mStartBtn.setOnClickListener(this);
        mStopBtn = findViewById(R.id.btn_stop);
        mStopBtn.setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mOpenSlWrapper != null) {
            mOpenSlWrapper.stopPlayback();
        }
    }

    @Override
    public void onClick(View view) {
        if (view == mStartBtn) {
            mStartBtn.setEnabled(false);
            new Thread(new Runnable() {
                @Override
                public void run() {
                    playAudio();
                }
            }).start();
        } else if (view == mStopBtn) {
            mStartBtn.setEnabled(true);
            mOpenSlWrapper.stopPlayback();
        }
    }

    private void playAudio() {
        InputStream inputStream = getResources().openRawResource(R.raw.audio);
        byte[] buffer = null;
        try {
            buffer = new byte[inputStream.available()];
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (buffer != null) {
            try {
                if (inputStream.read(buffer) > 0) {
                    mOpenSlWrapper.startPlayback(buffer);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
