package com.codingdie.util.media;

import com.alibaba.fastjson.JSONObject;

import java.lang.reflect.Field;

public class MediaUtil {

    static {
        System.setProperty("java.library.path", System.getProperty("java.library.path")
                + ":/usr/local/lib/");
        Field fieldSysPath = null;
        try {
            fieldSysPath = ClassLoader.class.getDeclaredField("sys_paths");
            fieldSysPath.setAccessible(true);
            fieldSysPath.set(null, null);
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        System.loadLibrary("media");
    }
    private static native String getVideoInfo(String filePath);

    public static  VideoInfo parseVideo(String filePath){
        String videoInfo = getVideoInfo(filePath);
        if(videoInfo!=null&&videoInfo.length()>0&&!videoInfo.equals("null")){
            return JSONObject.parseObject(videoInfo,VideoInfo.class);
        }
        return null;
    }

    public static void main(String[] args) {
        System.out.println(JSONObject.toJSONString(parseVideo("/Users/codingdie/Desktop/video/test.mp4")));
    }
}

