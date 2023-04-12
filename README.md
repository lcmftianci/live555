# live555 使用live555实现的rtsp server与 rtsp client
live555 库编译, 库封装，打开rtsp视频流

## 使用方式
* 配置ffmpeg路径和opencv路径 分别用于解码和播放视频,如果不需要解码播放视频可以不同配置ffmpeg/opencv, RtspClient就是只拉流不解码的例子
    </br>打开Live555.sln 配置库路径
    </br>编译RtspServer，然后将Titanic.mkv拷贝到RtspServer.exe执行程序目录下启动RtspServer.exe
```   
    cd x64\Debug
    RtspServer
```

* 编译RtspClient,运行,只接受流
```
    cd x64\Debug
    RtspClient rtsp://127.0.0.1/Titanic.mkv
```

* 编译RtspPlayer,运行,支持解码播放
```
    cd x64\Debug
    RtspPlayer rtsp://127.0.0.1/Titanic.mkv
```

## cmake编译方式 
```
    cmake -Bbuild_ninja .
    cmake --build build_ninja
```