#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WZserver.h"
#include <iostream>
#include <vector>
#include <fstream>

using std::cout;
using std::string;

volatile static int count = 0;
static int playerNum = 0;     //玩家总数
std::vector <PlayerInfo> GameServer::allPlayerInfo;    //？？？？
WZClientInformation GameServer::AcceptSocket[GameServer::MAX_NUM];  //？？？
int con = 0;

wchar_t *AnsiToUnicode(const char *szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}
/*
名称：构造函数
描述：用于Socket初始化
*/

GameServer::GameServer()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)   //判断一下有没有这个版本 (makeword是将两个byte型合并成一个word型，一个在高8位(b)，一个在低8位(a) )
	{
		cout << "无法加载套接字协议栈.\n";
		fflush(stdout);
		return;
	}

	//设置监听套接字
	ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPv4 STREAM TCP

	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "套接字初始化错误，错误号：" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	int port = 1236;  //待修改

	//设置服务器的信息
	Server.sin_family = PF_INET;
	Server.sin_port = htons(port); //htons是将整型变量从主机字节顺序转变成网络字节顺序
	Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);     //INADDR_ANY就是inet_addr("0.0.0.0")

	int errornum;
	//绑定
	bind(ListenSocket, (LPSOCKADDR)&Server, sizeof(Server));    //类型强制转换,LPSOCKADDR为结构指针
	while ((errornum = WSAGetLastError()) == 10048)    //10048错误 端口被占用
	{
		port++;
		Server.sin_port = htons(port);
		int retval = bind(ListenSocket, (LPSOCKADDR)&Server, sizeof(Server));
		cout << "绑定错误，错误号10048\n";
		fflush(stdout);

	}
	cout << "服务器的IP为:" << Server.sin_addr.S_un.S_addr
		<< " 端口号为: " << port << "\n";
	fflush(stdout);
	//监听   有listen就有accept
		//if (listen(ListenSocket, 5) == SOCKET_ERROR)

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)     //connect往listen队列填装连接，每一个accept操作都会从listen队列取连接转成socket，等待这个socket close掉之后再继续从listen队列中取连接，而listen队列长度就是listen函数的第二个参数--backlog
	{
		cout << "监听出错，错误号：" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	//将所有信息初始化
	for (int i = 0; i < MAX_NUM; i++)
		AcceptSocket[i].ClientSock = INVALID_SOCKET;

	cout << "网络初始化成功\n";
	fflush(stdout);
}


/*
名称：析构函数
作用：用于资源释放
*/
GameServer::~GameServer()
{
	if (ListenSocket != INVALID_SOCKET)
		closesocket(ListenSocket);
	cout << "服务器关闭\n";
	fflush(stdout);
	WSACleanup();
	return;
}


/*
名称：检测ID
描述：用于检测当前没有用的ID号
*/
int GameServer::CheckSocket()
{
	for (int i = 0; i < MAX_NUM; i++)
	{
		if (AcceptSocket[i].ClientSock == INVALID_SOCKET)
			return i;
	}
	return -1;
}

/*
名称：线程处理
描述：为每一个新玩家创建一个接受线程
	  如果人数达到上限，就拒绝
*/
int GameServer::ProcessGameServer()
{


	while (true)
	{
		int index = CheckSocket();
		sockaddr_in Client;

		int ClntLen = sizeof(Client);

		if (index != -1) //玩家未满
		{
			//对应前面的listen，这里是对应操作accept
			cout << "等待Client连接...\n";
			fflush(stdout);
			AcceptSocket[index].ClientSock = accept(
				ListenSocket,
				(struct sockaddr*)&AcceptSocket[index].Client,
				&ClntLen);
			//cout << "#1 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			AcceptSocket[index].ID = index;       //记录这个Client的ID，以后要寻找它
			AcceptSocket[index].Active = false;
			char szMsg[256] = { 0 };
			sprintf(szMsg, "收到 %u 接入。\r\n", AcceptSocket[index].ClientSock);
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;

			if (AcceptSocket[index].ClientSock == INVALID_SOCKET)
			{
				cout << "连接错误\n";
				fflush(stdout);
				break;
			}
			//cout << "#2 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			cout << "连接成功\n";
			fflush(stdout);
			//至此client与server连接成功,欢呼
			AcceptSocket[index].ID = index;
			//传输ID，其他的不用
			std::string msg = std::to_string(AcceptSocket[index].ID);    //将数值转化为字符串。返回对应的字符串。
			//SendMessageToOneClient(AcceptSocket[index].ID, msg);
			cout << "#ID :" << msg << "\n";
			cout << "连接成功\n";
			fflush(stdout);
			playerNum++;
			
			//至此client与server连接成功
			if (((index == 1)&&(con == 0))||((index == 0) && (con == 0)))
			{
				//创建接受者线程 
				int ThreadID;     //线程ID
				//把刚刚连接成功的Client建立一个新的线程
				ThreadID = (int)CreateThread(NULL, 0,
					(LPTHREAD_START_ROUTINE)(GameServer::ListenThread), //线程点函数
					(LPVOID)&AcceptSocket[index], 0,              //参数
					&AcceptSocket[index].RecvThreadID          //系统分配的ID
				);

				if (!ThreadID)
				{
					cout << "创建线程错误\n";
					fflush(stdout);
					ExitThread(AcceptSocket[index].RecvThreadID);
				}

				//至此，新的线程创建成功，可以传输数据了

				cout << "新玩家" << index << "的接受线程创建成功\n";
				fflush(stdout);
				//cout << "#3 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
				struct PlayerInfo temp;
				temp.clientInfo = { NULL,AcceptSocket[index].Client,AcceptSocket[index].ID,0,0,0 };
				temp.nickname = std::to_string(AcceptSocket[index].Client.sin_addr.S_un.S_addr) + "_" +
					std::to_string(AcceptSocket[index].Client.sin_port) + "_" + std::to_string(AcceptSocket[index].ID);      //？？？
				allPlayerInfo.push_back(temp);

			}
			if (index == 1)        //当两个client均连接时向客户端发一个消息，游戏开始
			{
				if (con == 0)
				{
					std::string check_both_connected = "#Connect Successfully";
					SendMessageToOneClient(AcceptSocket[0].ID, check_both_connected);
					SendMessageToOneClient(AcceptSocket[1].ID, check_both_connected); \
						con = 1;
				}
			}
			cout << "新玩家加入，IP地址为：" << inet_ntoa(AcceptSocket[index].Client.sin_addr)
				<< "  端口号为：" << ntohs(AcceptSocket[index].Client.sin_port) << "\n";
			fflush(stdout);
		}


		else   //玩家已满
		{
			SOCKET Accept = accept(ListenSocket,
				(struct sockaddr*)&Client, &ClntLen);

			if (Accept == INVALID_SOCKET)
			{
				cout << "连接错误\n";
				fflush(stdout);
				break;
			}


			cout << "非法请求的玩家的IP的地址为:" << inet_ntoa(Client.sin_addr)
				<< " 端口号为 :" << ntohs(Client.sin_port) << "\n";
			fflush(stdout);
			send(Accept, "当前用户已满", strlen("当前用户已满") + sizeof(char), 0);

			closesocket(Accept);

			cout << "非法连接玩家已断开\n";
			fflush(stdout);
			break;
		}
	}
	return 0;
}

/*
名称：接受线程
描述：Select模式
	首先判断该线程是否可读，如果可读就读取其上的信息
*/
DWORD WINAPI GameServer::ListenThread(void *data) //传进来具体哪个AcceptSocket[xx]的地址
{
	WZClientInformation *GameSocket = (WZClientInformation *)data;

	while (true)
	{
		cout << "DEBUG:新的收发循环\n";
		//接收命令 

		char recvBuf[28000];
		//string recvBuf;
		
		//fd_set Read;//基于select模式对IO进行管理  
		cout << "DEBUG:1\n";
		//FD_ZERO(&Read);    //初始化为0  清空fdset与所有文件句柄的联系
		//FD_SET(GameSocket->ClientSock, &Read); //将ClientSock加入队列
		cout << "DEBUG:2\n";
		//we only care reads
		//int sel = select(0, &Read, NULL, NULL, NULL);
		cout << "DEBUG:select之后\n";
		ZeroMemory(recvBuf, sizeof(recvBuf));
		//if (FD_ISSET(GameSocket->ClientSock, &Read))     //??
		//{
			//接受客户端的数据
			int result = recv(GameSocket->ClientSock, recvBuf, sizeof(recvBuf), 0);
			cout << "recv 返回值: " << result << "\n";
			fflush(stdout);
			if (result > 0 && result < 28000)
			{
				recvBuf[result] = '\0';
				cout << "玩家" << GameSocket->ID << "发送了消息:"
					<< recvBuf << "\n";
				fflush(stdout);
			}
			if (result == SOCKET_ERROR)
			{
				//CleanSocket(GameSocket->ID);
				break;
			}

			char szMsg[256] = { 0 };
			sprintf(szMsg, "收到 %u 消息:%s。\r\n", GameSocket->ClientSock, recvBuf );
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;
			//}

		//发送命令 
		char sendBuf[28000];
		//fd_set write;//基于select模式对IO进行管理  
		//FD_ZERO(&write);    //初始化为0
		//FD_SET(GameSocket->ClientSock, &write); //将ClientSock加入队列
		//we only care read
		//select(0, NULL, &write, NULL, NULL);
		ZeroMemory(sendBuf, sizeof(sendBuf));
		//if (FD_ISSET(GameSocket->ClientSock, &write))       //检查fdset联系的文件句柄fd是否可读写， > 0表示可读写。
		//{
			//接受客户端的数据
			memcpy(sendBuf, recvBuf,500);
			//cout << "Just Before Enter TOALL func, AcceptSocket[ID].ClientSock: " << GameSocket->ClientSock << "\n";
			SendMessageToAllClient(sendBuf, GameSocket->ID);
			//SendMessageToOneClient(GameSocket->ID, sendBuf);
		//}

	}
	return 1;
}

/*
名称：发送信息给一个client
描述：发送信息给一个client
*/
int GameServer::SendMessageToOneClient(int ID, const string  str)
{
	if (ID < 0)
		return 0;

	int oneSend = send(AcceptSocket[ID].ClientSock,
		str.c_str(), str.length() + sizeof(char), 0);    //通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
	cout << "向玩家" << ID << "发送了" << oneSend << "个bits的消息\n";
	fflush(stdout);
	//ZeroMemory(sendBuf, sizeof(sendBuf));
	if (oneSend == SOCKET_ERROR)
	{
		cout << "向玩家" << ID << "发送消息失败\n";
		fflush(stdout);
		return 0;
	}
	char szMsg[256] = { 0 };
	sprintf(szMsg, "向 %u 发送消息:%s。\r\n", AcceptSocket[ID].ClientSock, str.c_str());
	wchar_t *pwszMsg = AnsiToUnicode(szMsg);
	OutputDebugString(pwszMsg);
	delete[] pwszMsg;

	return 1;
}

/*
名称：全发送
描述：给所有Client发送信息
*/
void GameServer::SendMessageToAllClient(const string  str, int ID)
{

	bool flag = true;
	for (int i = 0; i < MAX_NUM; i++)
	{
		//cout << "Just In TOALL func, AcceptSocket[ID].ClientSock :" << AcceptSocket[i].ClientSock << "\n";
		if (ID != i)
		{
			if (AcceptSocket[i].ClientSock != INVALID_SOCKET &&             //返回INVALID_SOCKET错误
				!SendMessageToOneClient(i, str))
				flag = false;
		}
	}
	if (flag)
		cout << "服务器向全体(除了玩家" << ID << ")发送了消息:"
		<< str << "\n";
	else
		cout << "发送存在问题\n";
	fflush(stdout);

}


/*
名称：清理Socket
描述：清理退出游戏的线程
*/
void GameServer::CleanSocket(int ID)
{
	if (ID < 0)
		return;

	char send[20];
	sprintf(send, "#DD i%d*", ID);
	SendMessageToAllClient(send, ID);


	cout << " 玩家" << ID << "退出游戏\n";
	fflush(stdout);

	AcceptSocket[ID].Active = false;
	closesocket(AcceptSocket[ID].ClientSock);
	AcceptSocket[ID].ClientSock = INVALID_SOCKET;


	cout << "正在关闭其接受线程:" << AcceptSocket[ID].RecvThreadID << "\n";
	fflush(stdout);
	ExitThread(AcceptSocket[ID].RecvThreadID);



	cout << "关闭成功!\n";
	fflush(stdout);
}
