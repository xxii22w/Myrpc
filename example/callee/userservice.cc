#include <iostream>
#include <string>
#include "user.pb.h"

using namespace fixbug;

/*
    UserService原来是一个本地服务，提供了两个进程间的本地方法，Login和GFriendLists
    
*/

class UserService : public fixbug::UserServiceRpc // 使用在rpc服务发布端(rpc服务提供者)
{
public:
    bool Login(std::string name,std::string pwd)
    {
        std::cout <<"doing local service : Login" << std::endl;
        std::cout << "name" << name << "pwd:" << pwd << std::endl;
        return true;
    }    

    // 重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    // caller ===> Login(LoginRequest) => muduo => callee
    // callee ===> Login(LoginRequest) => 交到下面重写的这个Login方法上
    virtual void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
		// 框架给业务上报里请求参数LoginRequest,应用获取想要数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name,pwd); // 做本地业务

        // 把响应写入 包括错误码 错误消息 返回值
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_sucess(login_result);

        // 执行回调操作 执行响应对象数据的序列化和网络发送（都是由框架来完成的)
        done->Run();
    }
};

int main(int argc,char** argv)
{
    
    // 调用框架的初始化操作
    MprpcApplication::Init(argc,argv);

    // 把UserService对象发布到rpc节点上
    
    return 0;
}