#include <winsock2.h>
#include <process.h>               //C中的线程 
#include <string>
#include <random>
#include <array>
#pragma comment(lib,"ws2_32.lib")

struct  WZClientInformation           //Client 相关信息
{
	SOCKET ClientSock;       //套接字

	sockaddr_in Client;      //Clinet IP

	//sockaddr_in Client;      //Clinet IP port
	int ID;                   //Server分配的ID号码
	DWORD RecvThreadID;        //Server分配接收的线程号
	DWORD SendThreadID;			//Server分配发送的线程号
	bool Active;
};

struct PlayerInfo {
	std::string nickname;
	struct  WZClientInformation clientInfo;
	int order;
};


class GameServer
{
protected:
	enum {

		//MAX_NUM = 2000  //最大上限人数 
		MAX_NUM = 2  //最大上限人数 

		//MAX_NUM = 4  //最大上限人数  

	};
public:
	GameServer();
	~GameServer();

	int ProcessGameServer();    //线程处理

	static int SendMessageToOneClient(int ID, const std::string  str);  //向某一个Client发送信息

	int CheckSocket();     //检测当前可用的ID号
	static void CleanSocket(int ID); //清空ID号的套接字
	static void SendMessageToAllClient(const std::string  str, int ID = -1); //向所有Client发送信息
		//Socket 相关
public:
	static DWORD WINAPI ListenThread(void *data); //接受线程

private:
	//>>>>>> > WebGameLogic
	SOCKET ListenSocket;       //等待接受数据的socket
	static WZClientInformation AcceptSocket[MAX_NUM];  //Client的相关信息
	sockaddr_in Server;        //绑定地址

	//对网络数据的处理
public:
	static std::vector <PlayerInfo> allPlayerInfo;
};

