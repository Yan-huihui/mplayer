# mplayer简介
MPlayer被评为Linux下的最佳媒体播放工具，并且成功地被移植到Windows下。它能播放几乎所有流行的音频和视频格式，与其他播放器相比，资源占用非常少，不需要任何系统解码器就可以播放各种媒体格式的文件，对于常MPEG/XviD/DivX格式的文件支持尤其好，不仅拖动播放速度快得不可思议，而且播放破损文件时的效果也好得出奇，在低配置的机器上使用的优势更加明显，被美誉为“绿色播放器”。


相比其他软件，MPlayer可以称得上是绿色软件，它本身携带了多种类型的解码器，不需要再安装xvid、ffdshow、ac3 filter、ogg、vobsub等等那些所谓看DVDrip必备利器。不仅如此，它还拥有广泛的“群众基础”（输出设备）的支持，支持HTTP、MMS或者RTSP/RTP协议播放网络上的文件，优点简直是数不胜数。当然它最可爱的地方还是简单易用。


由于需要在终端输入命令进行操作文件， 难免有些许不便。该项目将基于mplayer，自动获取固定目录下音视频文件列表，进而对音视频文件进行一系列操作

# 注意事项
该项目在Ubuntu 18.04 64位下编译运行
需要安装mplayer环境

通过调用mplayer，自动识别/home/linux/Music目录下的音视频文件，加载到播放列表中。    
（如果目录下没有音视频文件会报错退出程序，没有指定目录也会报错）

通过程序可以执行以下操作：

1.查看播放列表   
2.开始/暂停（在歌曲播放结束，自动切换下一首）   
3.停止   
4.上一曲   
5.下一曲   
6.倍速播放（1倍速、2倍速、4倍速）   
7.定位   
8.播放方式（列表播放、单曲循环、随机播放）   
9.退出   
