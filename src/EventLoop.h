#pragma  once
#include <memory>
#include <atomic>
#include <iostream>
#include <queue> 
#include <thread>
#include <mutex> 
#include <map>
#include "ThreadPool.h"
#include "Channel.h"
#include "Epoll.h"
#include "Connection.h"
#include "Socket.h"
#include "ObjectPool.h"
using namespace std ;

class epOperation ;
class channel ;
class connection ;
class eventLoop ;
class socketFd ;

//事件循环 EventLoop和connection共享channel对象
class eventLoop {   
    //线程号码
    typedef std::vector<vector<pair<int, shared_ptr<channel>>>> channelMap;
    typedef std::function<void(channel* chl, vector<pair<int, shared_ptr<channel>>>&)> callBack ;
public:
    eventLoop() ;
    ~eventLoop() ;
public :
    int wakeup(int fd) ;
    void runThread() ;
    shared_ptr<channel> search(int index, int fd) ;
    int getListenFd() { return servFd ; }
    void loop() ;
    void initObjectPool() ;
    void addConnection(connection* con) ;
    void addClList(int fd, shared_ptr<channel>& channel_) ;
    int fillChannelList(int index, shared_ptr<channel>chl) ;
    shared_ptr<channel>  handleAccept(int index, int listenFd) ;
    int clearCloseChannel(int fd, int index) ;
    void round(shared_ptr<channel>, shared_ptr<epOperation>) ;
    int getNum() ;
    void initEventSet() ;   
    void printClList() ;
    channelMap getMap() { return clList ;}
    shared_ptr<socketFd> getSock() ;
    static mutex mute ;
private :
    callBack readCall ;
    //对象池
    shared_ptr<objectPool<channel>> obp ;
    int objectNum ;
    string ip ;
    string port ;
    shared_ptr<threadPool>pool ;
    int threadNums ;
    //线程
    vector<thread> threads ;
    //线程loop中的重要信息
    int servFd  = -1 ;
    connection* conn ;
    //一个eventLoop一个epoll
    std :: shared_ptr<epOperation> epPtr ;
    //活跃事件列表，二维数组
    vector<vector<shared_ptr<channel>>> activeChannels ;
    map<int, shared_ptr<epOperation>> epSet ;
    //该eventLoop对应的监听套接字封装
    //退出标志
    bool quit ;
    //设置套接字channel键值对
    channelMap clList ;
    int id ;
};

