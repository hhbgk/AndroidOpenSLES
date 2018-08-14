package com.hhbgk.example.opensles;

/**
 * Description:
 * Author:created by bob on 18-8-13.
 */
public final class OpenSlWrapper {
    private native boolean nativeStartCapture();

    private native boolean nativeStopCapture();

    private native boolean nativeStartPlayback(byte[] data);

    private native boolean nativeStopPlayback();

    static {
        System.loadLibrary("native-lib");
    }

    public boolean startPlayback(byte[] data) {
        return nativeStartPlayback(data);
    }

    public boolean stopPlayback() {
        return nativeStopPlayback();
    }
}
