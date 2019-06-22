#include <winsock2.h>
#include <process.h>               //C�е��߳� 
#include <string>
#include <random>
#include <array>
#pragma comment(lib,"ws2_32.lib")

struct  WZClientInformation           //Client �����Ϣ
{
	SOCKET ClientSock;       //�׽���

	sockaddr_in Client;      //Clinet IP

	//sockaddr_in Client;      //Clinet IP port
	int ID;                   //Server�����ID����
	DWORD RecvThreadID;        //Server������յ��̺߳�
	DWORD SendThreadID;			//Server���䷢�͵��̺߳�
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

		//MAX_NUM = 2000  //����������� 
		MAX_NUM = 2  //����������� 

		//MAX_NUM = 4  //�����������  

	};
public:
	GameServer();
	~GameServer();

	int ProcessGameServer();    //�̴߳���

	static int SendMessageToOneClient(int ID, const std::string  str);  //��ĳһ��Client������Ϣ

	int CheckSocket();     //��⵱ǰ���õ�ID��
	static void CleanSocket(int ID); //���ID�ŵ��׽���
	static void SendMessageToAllClient(const std::string  str, int ID = -1); //������Client������Ϣ
		//Socket ���
public:
	static DWORD WINAPI ListenThread(void *data); //�����߳�

private:
	//>>>>>> > WebGameLogic
	SOCKET ListenSocket;       //�ȴ��������ݵ�socket
	static WZClientInformation AcceptSocket[MAX_NUM];  //Client�������Ϣ
	sockaddr_in Server;        //�󶨵�ַ

	//���������ݵĴ���
public:
	static std::vector <PlayerInfo> allPlayerInfo;
};

