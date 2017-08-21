# wifi-detect-csource
some net options
方法一般无非
1.驱动实现
2.用户态pcap抓包
3.修改hostapd

-----
具体的协议解析示例prot

------
步骤
0.调通pcap抓包
1.了解网络字节序，比如ip地址，端口，mask，mac等等都是吗，都怎样求得。
2.数据报和数据帧的关系，有无mac的封装。
3.对自定义的数据报的解析
4.对数据帧的解析
5.对网上http://cuiqingcai.com/1918.html里面所有知识点的掌握，包括很多，如协议之类

-------
对WiFi帧的解析
1.先将Probe Request 外部设备请求AP的帧进行解析
