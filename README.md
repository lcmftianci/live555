# live555 使用live555实现的rtsp server与 rtsp client
live555 库编译, 库封装，打开rtsp视频流

# 使用方式
* 配置ffmpeg路径和opencv路径 分别用于解码和播放视频
    打开Live555.sln 配置库路径
    编译RtspServer，然后将Titanic.mkv拷贝到RtspServer.exe执行程序目录下启动RtspServer.exe
```   
    cd x64\Debug
    RtspServer
```

* 编译RtspClient,运行
```
    cd x64\Debug
    RtspClient rtsp://127.0.0.1/Titanic.mkv
```
