//
// Created by codingdie on 2018/5/15.
//

#include <libavformat/avformat.h>
#include "MediaUtil.h"
#include "cJSON.h"

char *getVideoInfo(const char *path) {
    cJSON *info = cJSON_CreateObject();
    av_register_all();

    AVFormatContext *format_ctx = NULL;

    if (avformat_open_input(&format_ctx, path, NULL, NULL) != 0) {
        fprintf(stderr, "Couldn't open input file");
        return NULL;
    }
    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
        printf("av_find_stream_info ERRORn");

        return NULL;
    }
    int audio_stream_index;
    int video_stream_index;

    int i;
    for (i = 0; i < format_ctx->nb_streams; i++) {
        AVStream *stream = format_ctx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            ;
            cJSON_AddNumberToObject(info, "width", stream->codecpar->width);
            cJSON_AddNumberToObject(info, "height", stream->codecpar->height);
            cJSON_AddNumberToObject(info, "length",format_ctx->duration/AV_TIME_BASE);
            cJSON_AddNumberToObject(info, "bps", stream->codecpar->bit_rate);
        }
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
        }
    }

    avformat_close_input(&format_ctx);
    return cJSON_PrintUnformatted(info);
}
