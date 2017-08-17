#include <jni.h>
#include <string>
#include "util/log_print.h"

using namespace std;
void sendMessage(JNIEnv *env, jobject obj, const char *msg);

void sendMessage(JNIEnv *env, jobject obj, const char *msg) {
    jclass clazz = env->FindClass("com/sjl/ffmpegdecoder/MainActivity");
    if (clazz == 0) {
        LOGE("没有找到类");
        return;
    }
    jmethodID method = env->GetMethodID(clazz, "sendMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("没有找到方法");
        return;
    }
    jstring message = env->NewStringUTF(msg);
    env->CallVoidMethod(obj, method, message);
    env->ReleaseStringUTFChars(message, env->GetStringUTFChars(message,NULL));
}

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

JNIEXPORT jint JNICALL
Java_com_sjl_ffmpegdecoder_util_FFmpegUtil_decoder(JNIEnv *env, jclass type, jobject object,
                                                   jstring input_, jstring outputYUV_,
                                                   jstring outputH264_) {
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *outputYUV = env->GetStringUTFChars(outputYUV_, 0);
    const char *outputH264 = env->GetStringUTFChars(outputH264_, 0);

    av_register_all();
    avformat_network_init();
    AVFormatContext *formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, input, NULL, NULL) != 0) {
        LOGE("无法打开文件");
        return 0;
    }
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOGE("无法找到流信息");
        return 0;
    }
    int videoIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if (videoIndex == -1) {
        LOGE("无法找到视频流");
        return 0;
    }
    AVCodecContext *codecContext = formatContext->streams[videoIndex]->codec;
    AVCodec *codec = avcodec_find_decoder(codecContext->codec_id);
    if (codec == NULL) {
        LOGE("无法找到编码器");
        return 0;
    }
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        LOGE("无法打开编码器");
        return 0;
    }
    LOGI("format = %s\ncodec = %s\nresolution = %d*%d",
         formatContext->iformat->name,
         codec->name,
         codecContext->width, codecContext->height);

    AVFrame *frame = av_frame_alloc();
    AVFrame *frameYUV = av_frame_alloc();
    uint8_t *outBuffer = (uint8_t *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height,
                                     1));
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, outBuffer, AV_PIX_FMT_YUV420P,
                         codecContext->width, codecContext->height, 1);
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));

    SwsContext *swsContext = sws_getContext(codecContext->width, codecContext->height,
                                            codecContext->pix_fmt,
                                            codecContext->width, codecContext->height,
                                            AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    int count = 0;
    int getPicture;
    int ySize;
    char msg[1000] = {0};
    FILE *fileH264 = fopen(outputH264, "wb+");
    FILE *fileYUV = fopen(outputYUV, "wb+");
    //循环读每一帧压缩数据
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoIndex) {
            //把h264数据写入文件
            fwrite(packet->data, 1, packet->size, fileH264);
            avcodec_send_packet(codecContext, packet);
            getPicture = avcodec_receive_frame(codecContext, frame);
            if (getPicture == 0) {
                sws_scale(swsContext, (const uint8_t *const *) frame->data, frame->linesize, 0,
                          codecContext->height,
                          frameYUV->data, frameYUV->linesize);
                ySize = codecContext->width * codecContext->height;
                fwrite(frameYUV->data[0], 1, ySize, fileYUV);//Y
                fwrite(frameYUV->data[1], 1, ySize / 4, fileYUV);//U
                fwrite(frameYUV->data[2], 1, ySize / 4, fileYUV);//V
                LOGI("第%d帧解析完成", count);
                sprintf(msg, "第%d帧解析完成", count);
                sendMessage(env, object, msg);
                count++;
            }
        }
        av_packet_unref(packet);
    }
    //av_read_frame()循环退出时，解码器中可能还包含剩余的几帧数据
    //通过flush_decoder将几帧数据输出
    while (1) {
        avcodec_send_packet(codecContext, packet);
        getPicture = avcodec_receive_frame(codecContext, frame);
        if (getPicture == AVERROR_EOF) {
            LOGI("全部flush完成");
            break;
        }
        if (getPicture != 0) {
            LOGI("异常错误");
            break;
        }
        sws_scale(swsContext, (const uint8_t *const *) frame->data, frame->linesize, 0,
                  codecContext->height,
                  frameYUV->data, frameYUV->linesize);
        ySize = codecContext->width * codecContext->height;
        fwrite(frameYUV->data[0], 1, ySize, fileYUV);//Y
        fwrite(frameYUV->data[1], 1, ySize / 4, fileYUV);//U
        fwrite(frameYUV->data[2], 1, ySize / 4, fileYUV);//V
        LOGI("第%d帧解析完成flush", count);
        count++;
    }
    sws_freeContext(swsContext);
    //关闭文件释放内存
    fclose(fileH264);
    fclose(fileYUV);

    av_frame_free(&frameYUV);
    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(outputYUV_, outputYUV);
    env->ReleaseStringUTFChars(outputH264_, outputH264);
    return 1;
}
JNIEXPORT void JNICALL
Java_com_sjl_ffmpegdecoder_util_FFmpegUtil_msgFromJNI(JNIEnv *env, jclass type, jobject object) {
    sendMessage(env, object, "from jni");
}
};