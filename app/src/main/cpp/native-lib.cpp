#include <jni.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include<Android/log.h>
#include <errno.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , LOG_TAG __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  ,LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  ,LOG_TAG, __VA_ARGS__)

#include "com_lee_delpath_JniTools.h"

#define LOG_TAG "JniTools"

bool is_dir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return false;
}

bool is_special_dir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

bool delete_file(const char *path)
{
    bool isRemoved = false;
    int ret = -1;
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    LOGD("path:%s", path);

    if(is_dir(path)) {
        if ((dir = opendir(path)) == NULL) {
            LOGE("open error :%s", strerror(errno));
            return false;
        }
        while((dir_info = readdir(dir)) != NULL)
        {
            if(is_special_dir(dir_info->d_name)) {
                continue;
            }
            LOGD("file:%s" , dir_info->d_name);
            get_file_path(path, dir_info->d_name, file_path);
            isRemoved = delete_file(file_path);
            if (!isRemoved) {
                LOGD("delete file %s :%s", file_path, strerror(errno));
                break;
            }

        }
        closedir(dir);
        ret =  rmdir(path);
        if (ret != 0) {
            isRemoved = false;
            LOGD("rm dir %s:%s", file_path, strerror(errno));
        }
    } else {
        ret = remove(path);
        LOGD("rm file %s:%s", path, strerror(errno));
        return ret == 0?true:false;
    }

    return isRemoved;
}

JNIEXPORT jstring JNICALL
Java_com_lee_delpath_JniTools_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jboolean JNICALL
Java_com_lee_delpath_JniTools_removePath
(JNIEnv *env, jobject, jstring path) {
    bool isRemoved = false;
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(path, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);

        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    isRemoved = delete_file(rtn);
//    isRemoved = delete_dir(rtn);
    return isRemoved;
}
