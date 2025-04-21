
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
#include <fcntl.h>

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
        //测试代码
        if (_userdatafile.fail()) {
            std::cerr << "Write operation failed1." << std::endl;
        }
    }
    void writedata(const std::string &_nandp)
    {
        //测试代码
        if (_userdatafile.fail()) {
            std::cerr << "Write operation failed4." << std::endl;
        }
        //测试用代码
        if (!_userdatafile.is_open())
        {
            std::cerr << "用户数据文件打开失败" << std::endl;
            exit(-1);
        }
        
       
        _userdatafile.seekp(std::ios_base::end);
        //测试代码
        if (_userdatafile.fail()) {
            std::cerr << "Write operation failed3." << std::endl;
        }
        std::string nandp= _nandp+'$';
        //测试用代码
        std::cout<<"我进到了writedata中"<<std::endl;
        std::cout<<nandp<<std::endl;
        _userdatafile.write(nandp.c_str(), strlen(nandp.c_str())+1);
        _userdatafile.flush();
        if (_userdatafile.bad() || _userdatafile.fail())
        {
            std::cerr << "用户数据写入错误" << std::endl;
            return;
        }
        
    }

    bool found(const std::string &_nadp)
    {
        
        _userdatafile.seekp(std::ios_base::beg);
        if (_userdatafile.fail()) {
            std::cerr << "Write operation failed10." << std::endl;
        }
        while (!_userdatafile.eof())
        {
            std::string tmp;
            std::getline(_userdatafile, tmp, '$');
            if (_userdatafile.fail()) {
                std::cerr << "Write operation failed11." << std::endl;
            }
            if(tmp == "")
                break;
            if (_nadp.find(tmp) != std::string::npos) // 优化!
                return true;
        }
        std::cerr << "没有找到相关用户" << std::endl;
        //测试代码
        if (_userdatafile.fail()) {
            std::cerr << "Write operation failed9." << std::endl;
        }
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
        for (int i = 0; !_massagetypefile.eof(); i++)
        {
            std::string tmp;
            std::getline(_massagetypefile, tmp, '$');
            if(!(tmp==""))
            {
                _mtmap.insert(std::pair<std::string, int>(tmp, i));
            }
            
        }

        _massagetypefile.close();
    }

    void showmtmap()
    {
        for (auto e : _mtmap)
        {
            std::cout << e.first << "-" << e.second << std::endl;
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
    server(int serverport) : _serverport(serverport),_Is_listen(false)
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
    }

    void deserialize(std::string &massage, std::vector<std::string> *massages)
    {

        size_t front, end, str_len;
        // 测试用代码
        //  int i = 0;
        str_len = strlen(FLAG_END);
        end = 0;
        while (1)
        {
            front = massage.find(FLAG_FRONT, end);
            end = massage.find(FLAG_END, front);
            if (front == std::string::npos || end == std::string::npos)
                break;
            massages->push_back(massage.substr(front + str_len, end - front - str_len));
            // 测试用代码
            //  std::cout<<(*massages)[i];
            //  i++;
        }
    }

    void handle(int mt, const std::string &msg, int clientfd) // 这个clientfd是发消息过来的客户端的fd
    {
        //测试代码
        // std::cout<<"msg:"<<msg<<std::endl;
        switch (mt)
        {
        case 0:
            _register(msg, clientfd);
            break;
        case 1:
            _login(msg, clientfd);
            break;
        case 3:
            _broadcast(msg, clientfd);
            break;
        case 5:
            // std::cout << "发送前 _connect_client" << std::endl;
            _connect_client(clientfd);
            // 测试代码
            break;
        default:
            std::cerr << "未知任务" << std::endl;
            return;
        }
    }

    void _connect_client(int clientfd)
    {
        _send(clientfd, "|$hello client$|");
    }

    void _register(const std::string &_nadp, int clientfd)
    {
        if (!_ud.found(_nadp))
        {
            _ud.writedata(_nadp);
            _send(clientfd, "|$/registerY $|\n");
        }
        else
        {
            _send(clientfd, "|$/registerF $|\n");
        }
    }

    void _login(const std::string &_nadp, int clientfd)
    {
        if (!_ud.found(_nadp))
        {
            _send(clientfd, "|$/loginF $|\n");
            // 测试用代码
            std::cout << "失败" << std::endl;
        }
        else
        {
            _send(clientfd, "|$/loginY $|\n");
            // 测试用代码
            std::cout << "成功" << std::endl;
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
        // 要改动的地方
        if (_Is_listen == false)
        {
            if (listen(_socketfd, 25) == -1)
            {
                std::cerr << "监听失败" << std::endl;
                std::cerr << strerror(errno) << std::endl;
                exit(-1);
            }
            int flags = fcntl(_socketfd, F_GETFL, 0);
            fcntl(_socketfd, F_SETFL, flags | O_NONBLOCK);
            _Is_listen = true;
        }
        // fcntl控制文件描述符设置非阻塞态
        struct sockaddr_in clientaddr;
        socklen_t clientaddr_len = sizeof(struct sockaddr_in);
        //测试代码
        // std::cout<<"准备接受:"<<std::endl;
        int clientfd = accept(_socketfd, (struct sockaddr *)&clientaddr, &clientaddr_len);
        //测试代码
        // std::cout<<"链接到了:"<<clientfd<<std::endl;
        _clientfd.push_back(clientfd);
    }

    void receive()
    {
        char buffer[1024];
        int bufferlen;
        auto cfd = _clientfd;
        // _mt.showmtmap();
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
                    // 测试用代码
                    std::cout << "mt:" << mt << std::endl;
                    if (mt == -1)
                    {
                        std::cerr << "没找到指定的命令\n"
                                  << std::endl;
                        continue;
                    }
                    Task tk(mt, msg);
                    // 测试代码
                    _taskdclientfd.insert(std::pair<int, Task>(client, tk)); // 联系client链接和任务
                }
            }
        }
    }

    void handlep()
    {
        // 测试用代码
        //  std::cout<<"hello"<<std::endl;
        //  sleep(3);
        auto tdcfd = _taskdclientfd; // 拷贝客户端和fd直接的链接
        if (!tdcfd.empty())
        {
            // 测试用代码
            // std::cout << "我进来了" << std::endl;
            for (auto tf : tdcfd)
            {
                // static int i_1231 = 0;
                // 测试用代码
                // std::cout << i_1231 << std::endl;
                handle(tf.second._tasknum, tf.second._taskm, tf.first);
                // sleep(1000000);
            }
            tdcfd.clear();
            _taskdclientfd = tdcfd;
        }
    }

private:
    int _serverport;
    int _socketfd;
    struct sockaddr_in _serveraddr;
    massagetype _mt;
    UserData _ud;

private:
    bool _Is_listen;

private:
    std::vector<int> _clientfd; // 管理已经建立链接的用户

private:
private:
    std::unordered_map<int, Task> _taskdclientfd;
};