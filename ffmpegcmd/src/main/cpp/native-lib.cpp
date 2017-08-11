#include <jni.h>
#include <string>
#include "util/log_print.h"
#include <vector>
using namespace std;

vector<string> splitStr(string str, const char *split);

vector<string> splitStr(string str, const char *split) {
    vector<string> vectors;
    jint start = 0;
    jint end;
    while ((end = str.find(split, start == 0 ? start : start + 1)) != string::npos) {
        vectors.push_back(
                str.substr(start == 0 ? start : start + 1, end - (start == 0 ? start : start + 1)));
        start = end;
    }
    end = strlen(str.c_str());
    vectors.push_back(
            str.substr(start == 0 ? start : start + 1, end - (start == 0 ? start : start + 1)));
    return vectors;
}

extern "C" {
#include "ffmpeg.h"
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_configuration(JNIEnv *env, jclass type) {
    char info[10000] = {0};
    sprintf(info, "%s\n", avcodec_configuration());
    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_urlprotocolinfo(JNIEnv *env, jclass type) {
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
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_avformatinfo(JNIEnv *env, jclass type) {
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
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_avcodecinfo(JNIEnv *env, jclass type) {
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
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_avfilterinfo(JNIEnv *env, jclass type) {
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

JNIEXPORT jint JNICALL
Java_com_sjl_ffmpegcmd_util_FFmpegUtil_runCmd(JNIEnv *env, jclass type, jstring cmd_) {
    const char *cmd = env->GetStringUTFChars(cmd_, 0);
    LOGI("cmd=%s",cmd);
    vector<string> cmdVector = splitStr(cmd," ");
    int len = cmdVector.size();
    char *argv[len];
    for (int i = 0; i < len; i++) {
        argv[i] = (char *) cmdVector[i].c_str();
        LOGI("cmd%d=%s",i,argv[i]);
    }

    env->ReleaseStringUTFChars(cmd_, cmd);
    return run(len,argv);
}

};