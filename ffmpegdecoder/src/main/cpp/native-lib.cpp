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

jint runCmd(const char *cmd);
jint runCmd(const char *cmd) {
    LOGI("cmd=%s", cmd);
    vector<string> cmdVector = splitStr(cmd, " ");
    int len = cmdVector.size();
    char *argv[len];
    for (int i = 0; i < len; i++) {
        argv[i] = (char *) cmdVector[i].c_str();
        LOGI("cmd%d=%s", i, argv[i]);
    }
    return run(len, argv);
}

};