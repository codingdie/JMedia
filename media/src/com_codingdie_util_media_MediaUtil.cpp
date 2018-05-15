#include "com_codingdie_util_media_MediaUtil.h"

#ifdef __cplusplus
extern "C" {
#include "MediaUtil.h"

#endif
/*
 * Class:     com_codingdie_util_media_MediaUtil
 * Method:    getVideoInfo
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_codingdie_util_media_MediaUtil_getVideoInfo
        (JNIEnv *env, jclass jclass1, jstring jfilePath) {
    jboolean no = 0;
    const char *filePath = env->GetStringUTFChars(jfilePath, &no);
    return env->NewStringUTF(getVideoInfo(filePath));
}

#ifdef __cplusplus
}
#endif