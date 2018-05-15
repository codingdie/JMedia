package com.codingdie.util.media;

public class MediaUtil {

    static {
        System.load("libmedia");
    }
    private static native String getVideoInfo(String filePath);

    public static void main(String[] args) {
        System.out.println(getVideoInfo("/Users/codingdie/Desktop/video/test.mp4"));
    }
}

//com/codingdie/util/Users/codingdie/Documents/code/JMedia/java/out/production/classes/media