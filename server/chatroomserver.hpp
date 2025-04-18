
// 所有文本处理采用UTF-8编码模式

#pragma once
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <string>
#include <functional>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <mutex>
#include <string.h>
#include <unordered_map>
#include <cerrno>

#define FLAG_END "$|"
#define FLAG_FRONT "|$"

class UserData // 管理所有用户(在线离线都管理)
{
public:
    UserData()
    {
        _userdatafile.open("./Data/UserData.txt", std::ios::in | std::ios::out);
        if (!_userdatafile.is_open())
        {
            std::cerr << "用户数据文件打开失败" << std::endl;
            exit(-1);
        }
    }
    void writedata(const std::string &_nandp)
    {
        _userdatafile.seekp(std::ios_base::end);
        _userdatafile.write(_nandp.c_str(), strlen(_nandp.c_str()));
        if (_userdatafile.bad() || _userdatafile.fail())
        {
            std::cerr << "用户数据写入错误" << std::endl;
            return;
        }
        _userdatafile.flush();
    }

    bool found(const std::string &_nadp)
    {
        _userdatafile.seekp(std::ios_base::beg);
        while (!_userdatafile.eof())
        {
            std::string tmp;
            std::getline(_userdatafile, tmp);
            if (_nadp.find(tmp) != std::string::npos) // 优化!
                return true;
        }
        std::cerr << "没有找到相关用户" << std::endl;
        return false;
    }

    std::string cutusername(const std::string &cstr)
    {
        int firstpos = cstr.find(" ");
        int secondpos = cstr.find(" ", firstpos);
        return cstr.substr(firstpos + 1, secondpos - firstpos);
    }

    ~UserData()
    {
        _userdatafile.close();
    }

public:
    // std::unordered_map<>
    std::fstream _userdatafile;
    std::unordered_map<int, std::string> _cfdandname; // 管理已经登入的用户
};

class massagetype
{
public:
    massagetype()
    {
        _massagetypefile.open("./Data/massagetypefile.txt", std::ios::in);
        if (!_massagetypefile.is_open())
        {
            std::cerr << "文件打开失败" << std::endl;
            close(-1);
        }
        for (int i=0; !_massagetypefile.eof(); i++)
        {
            std::string tmp;
            std::getline(_massagetypefile, tmp,'$');
            _mtmap.insert(std::pair<std::string, int>(tmp, i));
        }

        _massagetypefile.close();
        showmtmap();
        
    }

    void showmtmap ()
    {
        for(auto e : _mtmap)
        {
            std::cout<<e.first<<"-"<<e.second<<std::endl;
        }
    }

    int findmassagetypes(std::string &msg)
    {
        for (auto mt : _mtmap)
        {
            size_t mg = msg.find(mt.first);
            if (mg != std::string::npos)
            {
                return mt.second;
            }
        }
        return -1;
    }

public:
    std::ifstream _massagetypefile;
    std::unordered_map<std::string, int> _mtmap;
};

class Task
{
public:
    Task(int tasknum, std::string &_taskm) : _tasknum(tasknum), _taskm(_taskm)
    {
        ;
    }

    bool operator==(const Task &_other) const
    {
        return (_tasknum == _other._tasknum) && (_other._taskm == _taskm);
    }

public:
    int _tasknum;
    std::string _taskm;
};

class server
{
public:
    server(int serverport) : _serverport(serverport)
    {
        memset(&_serveraddr, 0, sizeof(struct sockaddr_in));
        _socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_socketfd == -1)
        {
            std::cerr << "套接字创建失败" << std::endl;
            exit(-1);
        }
        _serveraddr.sin_family = AF_INET;
        _serveraddr.sin_addr.s_addr = INADDR_ANY;
        _serveraddr.sin_port = htons(_serverport);
        std::cout << _serverport << std::endl;
        if (bind(_socketfd, (const struct sockaddr *)&_serveraddr, sizeof(struct sockaddr_in)) == -1)
        {
            std::cerr << "bind错误" << std::endl;
            std::cout << strerror(errno) << std::endl;
            exit(-1);
        }
        _rcet = new std::thread(&server::receive, this);
        _sendt = new std::thread(&server::handlep, this);
        _connect = new std::thread(&server::connect, this);
    }

    void deserialize(std::string &massage, std::vector<std::string> *massages)
    {

        size_t front, end, str_len;
        str_len = strlen(FLAG_END);
        end = 0;
        while (1)
        {
            front = massage.find(FLAG_FRONT, end);
            end = massage.find(FLAG_END, front);
            if (front == std::string::npos || end == std::string::npos)
                break;
            massages->push_back(massage.substr(front + str_len, end - front - str_len));
        }
    }

    void handle(int mt, const std::string &msg, int clientfd) // 这个clientfd是发消息过来的客户端的fd
    {
        switch (mt)
        {
        case 1:
            _register(msg, clientfd);
            break;
        case 2:
            _login(msg, clientfd);
            break;
        case 4:
            _broadcast(msg, clientfd);
            break;
        case 5:
            _connect_client(clientfd);
        default:
            std::cerr << "未知任务" << std::endl;
            return;
        }
    }

    void _connect_client(int clientfd)
    {
        ;
    }

    void _register(const std::string &_nadp, int clientfd)
    {
        if (!_ud.found(_nadp))
        {
            _ud.writedata(_nadp);
            _send(clientfd, "/registerY \n");
        }
        else
        {
            _send(clientfd, "/registerF \n");
        }
    }

    void _login(const std::string &_nadp, int clientfd)
    {
        if (!_ud.found(_nadp))
        {
            _send(clientfd, "/loginF \n");
        }
        else
        {
            _send(clientfd, "/loginY \n");
            _ud._cfdandname.insert(std::pair<int, std::string>(clientfd, _ud.cutusername(_nadp))); // 这里如果要做成多线程处理的话要加锁(这里是单线程就不加了)
        }
        return;
    }

    void _broadcast(const std::string &msg, int clientfd) // 暂时不支持聊天缓存也就是说新来的人无法看到之前的聊天内容
    {
        auto cfandn = _ud._cfdandname;
        std::string dmsg = msg + "|";
        auto it = cfandn.find(clientfd);
        if (it == cfandn.end())
        {
            std::cerr << "没有找到用户名" << std::endl;
            return;
        }
        dmsg += it->second;

        for (auto fn : cfandn)
        {
            _send(fn.first, dmsg);
        }
    }

    ~server()
    {
        close(_socketfd);
        for (auto i : _clientfd)
        {
            close(i);
        }
    }
    // 仅在类中使用的函数
    void _send(int clientfd, const std::string &content)
    {
        if (send(clientfd, content.c_str(), content.size(), 0) == -1)
        {
            std::cerr << "发送失败" << content << std::endl;
            return;
        }
    }

    void connect()
    {
        if (listen(_socketfd, 25) == -1)
        {
            std::cerr << "监听失败" << std::endl;
            std::cerr << strerror(errno) << std::endl;
            exit(-1);
        }
        while (1)
        {
            struct sockaddr_in clientaddr; // to do
            socklen_t clientaddr_len = sizeof(struct sockaddr_in);
            std::cout << "下一步链接" << std::endl;
            int clientfd = accept(_socketfd, (struct sockaddr *)&clientaddr, &clientaddr_len);
            // std::cout<<"链接到了:"<<clientfd<<"--"<<clientaddr.sin_addr.s_addr<<std::endl;
            _clientfdmutex.lock();
            _clientfd.push_back(clientfd);
            _clientfdmutex.unlock();
        }
    }

    void receive()
    {
        char buffer[1024];
        while (1)
        {
            int bufferlen;
            _clientfdmutex.lock();
            auto cfd = _clientfd;
            _clientfdmutex.unlock();
            for (auto client : cfd)
            {
                std::vector<std::string> massages;
                bufferlen = recv(client, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
                if (bufferlen != -1 && bufferlen != 0)
                {
                    std::string massage(buffer, bufferlen);
                    // 测试用代码
                    std::cout << massage << std::endl;
                    deserialize(massage, &massages);
                    for (auto msg : massages)
                    {
                        // _mt.showmtmap();
                        int mt = _mt.findmassagetypes(msg);
                        if (mt == -1)
                        {
                            std::cerr << "没找到指定的命令\n"
                                      << std::endl;
                            continue;
                        }
                        Task tk(mt, msg);
                        _taskdfdmutex.lock();
                        _taskdclientfd.insert(std::pair<int, Task>(client, tk)); // 联系client链接和任务
                        _taskdfdmutex.unlock();
                    }
                }
            }
        }
    }

    void handlep()
    {
        while (1)
        {
            _taskdfdmutex.lock();
            auto tdcfd = _taskdclientfd; // 拷贝客户端和fd直接的链接
            _taskdfdmutex.unlock();
            if (!tdcfd.empty())
            {
                for (auto tf : tdcfd)
                {
                    handle(tf.second._tasknum, tf.second._taskm, tf.first);
                }
            }
        }
    }

private:
    int _serverport;
    int _socketfd;
    struct sockaddr_in _serveraddr;
    massagetype _mt;
    UserData _ud;

private:
    std::thread *_rcet;
    std::thread *_sendt;
    std::thread *_connect;

private:
    std::vector<int> _clientfd; // 管理已经建立链接的用户

private:
    std::mutex _clientfdmutex;
    std::mutex _taskdfdmutex;

private:
    std::unordered_map<int, Task> _taskdclientfd;
};