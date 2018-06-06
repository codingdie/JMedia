//
// Created by codingdie on 2018/5/15.
//

#include <libavformat/avformat.h>
#include <png.h>
#include <jpeglib.h>
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
            video_stream_index = i;;
            cJSON_AddNumberToObject(info, "width", stream->codecpar->width);
            cJSON_AddNumberToObject(info, "height", stream->codecpar->height);
            cJSON_AddNumberToObject(info, "length", format_ctx->duration / AV_TIME_BASE);
            cJSON_AddNumberToObject(info, "bps", stream->codecpar->bit_rate);
        }
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
        }
    }

    avformat_close_input(&format_ctx);
    return cJSON_PrintUnformatted(info);
}


#define PNG_BYTES_TO_CHECK 4

#define HAVE_ALPHA 1


#define NO_ALPHA 0
enum {
    JPEG_COMPRESS_ERROR = 1, JPEG_DECOMPRESS_ERROR = 2
};

/* 自定义jpeg图像压缩/解压缩过程中错误退出函数 */

int check_if_png(const char *file_name) {
    FILE *fp;
    unsigned char buf[PNG_BYTES_TO_CHECK];
    if ((fp = fopen(file_name, "rb")) == NULL)
        return 0;
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
        return 0;
    fclose(fp);
    return png_check_sig(buf, PNG_BYTES_TO_CHECK); //0错误 非0正确

}

void jpeg_custom_default(j_common_ptr cinfo) {
    longjmp(cinfo->client_data, 1);
}

char *getImageInfo(const char *path) {

    cJSON *info = cJSON_CreateObject();

    if (check_if_png(path)) {
        FILE *fp = fopen(path, "rb");
        png_structp png_ptr;
        png_infop info_ptr;
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        info_ptr = png_create_info_struct(png_ptr);
        rewind(fp);
        png_init_io(png_ptr, fp); //文件指针赋值
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
        cJSON_AddNumberToObject(info, "width", png_get_image_width(png_ptr, info_ptr));
        cJSON_AddNumberToObject(info, "height", png_get_image_width(png_ptr, info_ptr));
        cJSON_AddNumberToObject(info, "channels", png_get_channels(png_ptr, info_ptr));
        cJSON_AddNumberToObject(info, "depth", png_get_bit_depth(png_ptr, info_ptr));
        fclose(fp);
    } else {
        FILE *fp;
        if ((fp = fopen(path, "rb")) == NULL) {
            return 0;
        }
        struct jpeg_error_mgr jerr;
        struct jpeg_decompress_struct cinfo;
        jpeg_create_decompress(&cinfo);
        cinfo.err = jpeg_std_error(&jerr);
        jerr.error_exit = jpeg_custom_default;
        jmp_buf jmpbuffer;
        cinfo.client_data = jmpbuffer;
        jpeg_stdio_src(&cinfo, fp);
        setjmp(cinfo.client_data);
        if(jerr.msg_code==0){
            jpeg_read_header(&cinfo, TRUE);
            cJSON_AddNumberToObject(info, "width", cinfo.image_width);
            cJSON_AddNumberToObject(info, "height", cinfo.image_height);
            cJSON_AddNumberToObject(info, "channels", cinfo.num_components);
            cJSON_AddNumberToObject(info, "depth", cinfo.block_size);
        }
        fclose(fp);
    }
    return cJSON_PrintUnformatted(info);
}
