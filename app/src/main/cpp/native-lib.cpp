#include <jni.h>
#include <string>
#include "util/log_print.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_configuration(JNIEnv *env, jclass type) {
    char info[10000] = {0};
    sprintf(info, "%s\n", avcodec_configuration());
    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_urlprotocolinfo(JNIEnv *env, jclass type) {
    char info[40000] = {0};
    av_register_all();

    struct URLProtocol *pup = NULL;
    //Input
    struct URLProtocol **p_temp = &pup;
    avio_enum_protocols((void **) p_temp, 0);
    while ((*p_temp) != NULL) {
        sprintf(info, "%s[Input][%10s]\n", info, avio_enum_protocols((void **) p_temp, 0));
    }
    pup = NULL;
    //Output
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        sprintf(info, "%s[Output][%10s]\n", info, avio_enum_protocols((void **) p_temp, 1));
    }

    //LOGE("%s", info);
    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_avformatinfo(JNIEnv *env, jclass type) {
    char info[40000] = {0};

    av_register_all();

    AVInputFormat *if_temp = av_iformat_next(NULL);
    AVOutputFormat *of_temp = av_oformat_next(NULL);
    //Input
    while (if_temp != NULL) {
        sprintf(info, "%s[Input][%10s]\n", info, if_temp->name);
        if_temp = if_temp->next;
    }
    //Output
    while (of_temp != NULL) {
        sprintf(info, "%s[Output][%10s]\n", info, of_temp->name);
        of_temp = of_temp->next;
    }
    //LOGE("%s", info);
    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_avcodecinfo(JNIEnv *env, jclass type) {
    char info[40000] = {0};

    av_register_all();

    AVCodec *c_temp = av_codec_next(NULL);

    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%s[Decode]", info);
        } else {
            sprintf(info, "%s[Encode]", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);


        c_temp = c_temp->next;
    }
    //LOGE("%s", info);

    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_avfilterinfo(JNIEnv *env, jclass type) {
    char info[40000] = {0};
    avfilter_register_all();
    AVFilter *f_temp = (AVFilter *) avfilter_next(NULL);
    while (f_temp != NULL) {
        sprintf(info, "%s[%10s]\n", info, f_temp->name);
        f_temp = f_temp->next;
    }
    //LOGE("%s", info);

    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_getVideoInfo(JNIEnv *env, jclass type,
                                                        jstring filename_) {
    const char *filename = env->GetStringUTFChars(filename_, 0);

    av_register_all();
    AVFormatContext *avFormatContext = avformat_alloc_context();
    //Input
    //打开媒体文件
    if (avformat_open_input(&avFormatContext, filename, 0, 0) < 0) {
        LOGE("无法打开输入文件");
        return 0;
    }
    //获取视频信息
    if (avformat_find_stream_info(avFormatContext, 0) < 0) {
        LOGE("无法检索输入流信息");
        return 0;
    }
    int videoIndex = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if(avFormatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoIndex = i;
            break;
        }
    }
    AVStream *avStream = avFormatContext->streams[videoIndex];
    LOGI("duration=%d",avStream->info->codec_info_duration);

    env->ReleaseStringUTFChars(filename_, filename);

    return env->NewStringUTF("");
}

JNIEXPORT jint JNICALL
Java_com_sjl_ffmpeg_util_FFmpegUtil_transform(JNIEnv *env, jclass type, jstring input_,
                                                  jstring output_) {
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *output = env->GetStringUTFChars(output_, 0);
    AVOutputFormat *outputFormat = NULL;
    //Input AVFormatContext
    AVFormatContext *iformatContext = NULL;
    //Output AVFormatContext
    AVFormatContext *oformatContext = NULL;
    av_register_all();
    //Input
    //打开媒体文件
    if (avformat_open_input(&iformatContext, input, 0, 0) < 0) {
        LOGE("无法打开输入文件");
        return 0;
    }
    //获取视频信息
    if (avformat_find_stream_info(iformatContext, 0) < 0) {
        LOGE("无法检索输入流信息");
        return 0;
    }
    av_dump_format(iformatContext, 0, input, 0);
    //初始化输出视频码流的AVFormatContext
    avformat_alloc_output_context2(&oformatContext, NULL, NULL, output);
    if (!oformatContext) {
        LOGE("无法创建输出上下文");
        return 0;
    }
    outputFormat = oformatContext->oformat;
    for (int i = 0; i < iformatContext->nb_streams; i++) {
        AVStream *inStream = iformatContext->streams[i];
        AVStream *outStream = avformat_new_stream(oformatContext, inStream->codec->codec);
        if (!outStream) {
            LOGE("分配输出流错误");
            return 0;
        }
        //关键：赋值AVCodecContext
        if (avcodec_copy_context(outStream->codec, inStream->codec) < 0) {
//            avcodec_parameters_from_context()
            LOGE("复制错误");
            return 0;
        }
        outStream->codec->codec_tag = 0;
        if (oformatContext->oformat->flags & AVFMT_GLOBALHEADER) {
            outStream->codec->flags != CODEC_FLAG_GLOBAL_HEADER;
        }
    }
    //Output
    av_dump_format(oformatContext, 0, output, 1);
    //打开输出文件
    if (!(oformatContext->flags & AVFMT_NOFILE)) {
        if (avio_open(&oformatContext->pb, output, AVIO_FLAG_WRITE) < 0) {
            LOGE("无法打开输出文件");
            return 0;
        }
    }
    //写文件头
    if(avformat_write_header(oformatContext,NULL)<0){
        LOGE("打开输出文件错误");
        return 0;
    }
    int frameIndex = 0;
    while (1) {
        AVStream *inStream;
        AVStream *outStream;
        AVPacket pkt;
        if (av_read_frame(iformatContext, &pkt)<0) {
            break;
        }
        inStream = iformatContext->streams[pkt.stream_index];
        outStream = oformatContext->streams[pkt.stream_index];
        //转换 PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, inStream->time_base, outStream->time_base,
                                   (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, inStream->time_base, outStream->time_base,
                                   (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, inStream->time_base, outStream->time_base);
        pkt.pos = -1;
        //将AVPacket（存储视频压缩码流数据）写入文件
        if (av_interleaved_write_frame(oformatContext, &pkt) < 0) {
            LOGE("合并AVPacket错误");
            break;
        }
        LOGI("写入第%d帧", frameIndex);
        av_free_packet(&pkt);
        frameIndex++;
    }
    //写文件尾
    av_write_trailer(oformatContext);

    avformat_close_input(&iformatContext);
    //关闭输出流
    if (oformatContext && !(outputFormat->flags & AVFMT_NOFILE)) {
        avio_close(oformatContext->pb);
    }
    avformat_free_context(oformatContext);

    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);

    return 1;
}
};