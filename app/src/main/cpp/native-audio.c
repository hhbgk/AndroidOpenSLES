/*
 *  COPYRIGHT NOTICE
 *  Copyright (C) 2017, hhbgk
 *
 *  @license under the Apache License, Version 2.0
 *
 *  @file    native-audio.c
 *
 *  @author  hhbgk
 *  @date    2018/08/13
 */
#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include "opensl_io.h"

#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "opensl",__VA_ARGS__)

#define SAMPLERATE 8000
#define CHANNELS 1
#define PERIOD_TIME 20//1//40 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS
#define FILE_PATH "/sdcard/audio.pcm"

static volatile int g_loop_exit = 0;

/*
 * Class:     com_hhbgk_example_opensles_OpenSlWrapper
 * Method:    nativeStartCapture
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_hhbgk_example_opensles_OpenSlWrapper_nativeStartCapture(JNIEnv *env, jobject thiz)
{
/*
    FILE * fp = fopen(FILE_PATH, "wb");
    if( fp == NULL ) {
        LOG("cannot open file (%s)\n", FILE_PATH);
        return -1;
    }

    OPENSL_STREAM* stream = android_OpenAudioDevice(SAMPLERATE, CHANNELS, CHANNELS, FRAME_SIZE);
    if (stream == NULL) {
        fclose(fp);
        LOG("failed to open audio device ! \n");
        return JNI_FALSE;
    }

    int samples;
    short buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    while (!g_loop_exit) {
        samples = android_AudioIn(stream, buffer, BUFFER_SIZE);
        if (samples < 0) {
            LOG("android_AudioIn failed !\n");
            break;
        }
        if (fwrite((unsigned char *)buffer, samples*sizeof(short), 1, fp) != 1) {
            LOG("failed to save captured data !\n ");
            break;
        }
        LOG("capture %d samples !\n", samples);
    }

    android_CloseAudioDevice(stream);
    fclose(fp);
*/
    LOG("nativeStartCapture completed !");

    return JNI_TRUE;
}

/*
 * Class:     com_hhbgk_example_opensles_OpenSlWrapper
 * Method:    nativeStopCapture
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_hhbgk_example_opensles_OpenSlWrapper_nativeStopCapture(JNIEnv *env, jobject thiz)
{
    g_loop_exit = 1;
    return JNI_TRUE;
}

/*
 * Class:     com_hhbgk_example_opensles_OpenSlWrapper
 * Method:    nativeStartPlayback
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_hhbgk_example_opensles_OpenSlWrapper_nativeStartPlayback(JNIEnv *env, jobject thiz, jbyteArray jdata)
{
    if (jdata == NULL)
        return JNI_FALSE;
    unsigned char *data = (unsigned char *) (*env)->GetByteArrayElements(env, jdata, NULL);
    if (data == NULL)
        return JNI_FALSE;
    int data_len = (*env)->GetArrayLength(env, jdata);

    int min_audio_buffer_size = android_GetAudioMinBufferSize(SAMPLERATE, CHANNELS, 16);
    OPENSL_STREAM* stream = android_OpenAudioDevice(SAMPLERATE, CHANNELS, CHANNELS, FRAME_SIZE);
    if (stream == NULL) {
        (*env)->ReleaseByteArrayElements(env, jdata, (jbyte *) data, NULL);
        LOG("failed to open audio device ! \n");
        return JNI_FALSE;
    }
    LOG("data_len=%d, min_audio_buffer_size=%d, BUFFER_SIZE=%d", data_len, min_audio_buffer_size, BUFFER_SIZE);
    int samples;
    short buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    int remaining = data_len;
    int len = 0;
    while (!g_loop_exit && remaining > 0)
    {
        /*if (fread((unsigned char *)buffer, BUFFER_SIZE*2, 1, fp) != 1) {
            LOG("failed to read data \n ");
            break;
        }*/
        if (remaining >= (BUFFER_SIZE * 2)) {
            memcpy(buffer, data + len, BUFFER_SIZE * 2);
            len = len + BUFFER_SIZE * 2;
            remaining = remaining - (BUFFER_SIZE * 2);
        } else if (remaining > 0) {
            memcpy(buffer, data + len, (size_t) remaining);
            len = len + remaining;
            remaining = 0;
        } else {
            break;
        }
        samples = android_AudioOut(stream, buffer, BUFFER_SIZE);
        if (samples < 0) {
            LOG("android_AudioOut failed !\n");
        }
        LOG("playback %d samples !\n", samples);
    }

    android_CloseAudioDevice(stream);
    (*env)->ReleaseByteArrayElements(env, jdata, (jbyte *) data, NULL);
    LOG("nativeStartPlayback completed !");

    return JNI_TRUE;
}

/*
 * Class:     com_hhbgk_example_opensles_OpenSlWrapper
 * Method:    nativeStopPlayback
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_hhbgk_example_opensles_OpenSlWrapper_nativeStopPlayback(JNIEnv *env, jobject thiz)
{
    g_loop_exit = 1;
    return JNI_TRUE;
}