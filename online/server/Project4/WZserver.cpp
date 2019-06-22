#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WZserver.h"
#include <iostream>
#include <vector>
#include <fstream>

using std::cout;
using std::string;

volatile static int count = 0;
static int playerNum = 0;     //�������
std::vector <PlayerInfo> GameServer::allPlayerInfo;    //��������
WZClientInformation GameServer::AcceptSocket[GameServer::MAX_NUM];  //������
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
���ƣ����캯��
����������Socket��ʼ��
*/

GameServer::GameServer()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)   //�ж�һ����û������汾 (makeword�ǽ�����byte�ͺϲ���һ��word�ͣ�һ���ڸ�8λ(b)��һ���ڵ�8λ(a) )
	{
		cout << "�޷������׽���Э��ջ.\n";
		fflush(stdout);
		return;
	}

	//���ü����׽���
	ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPv4 STREAM TCP

	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "�׽��ֳ�ʼ�����󣬴���ţ�" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	int port = 1236;  //���޸�

	//���÷���������Ϣ
	Server.sin_family = PF_INET;
	Server.sin_port = htons(port); //htons�ǽ����ͱ����������ֽ�˳��ת��������ֽ�˳��
	Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);     //INADDR_ANY����inet_addr("0.0.0.0")

	int errornum;
	//��
	bind(ListenSocket, (LPSOCKADDR)&Server, sizeof(Server));    //����ǿ��ת��,LPSOCKADDRΪ�ṹָ��
	while ((errornum = WSAGetLastError()) == 10048)    //10048���� �˿ڱ�ռ��
	{
		port++;
		Server.sin_port = htons(port);
		int retval = bind(ListenSocket, (LPSOCKADDR)&Server, sizeof(Server));
		cout << "�󶨴��󣬴����10048\n";
		fflush(stdout);

	}
	cout << "��������IPΪ:" << Server.sin_addr.S_un.S_addr
		<< " �˿ں�Ϊ: " << port << "\n";
	fflush(stdout);
	//����   ��listen����accept
		//if (listen(ListenSocket, 5) == SOCKET_ERROR)

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)     //connect��listen������װ���ӣ�ÿһ��accept���������listen����ȡ����ת��socket���ȴ����socket close��֮���ټ�����listen������ȡ���ӣ���listen���г��Ⱦ���listen�����ĵڶ�������--backlog
	{
		cout << "������������ţ�" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	//��������Ϣ��ʼ��
	for (int i = 0; i < MAX_NUM; i++)
		AcceptSocket[i].ClientSock = INVALID_SOCKET;

	cout << "�����ʼ���ɹ�\n";
	fflush(stdout);
}


/*
���ƣ���������
���ã�������Դ�ͷ�
*/
GameServer::~GameServer()
{
	if (ListenSocket != INVALID_SOCKET)
		closesocket(ListenSocket);
	cout << "�������ر�\n";
	fflush(stdout);
	WSACleanup();
	return;
}


/*
���ƣ����ID
���������ڼ�⵱ǰû���õ�ID��
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
���ƣ��̴߳���
������Ϊÿһ������Ҵ���һ�������߳�
	  ��������ﵽ���ޣ��;ܾ�
*/
int GameServer::ProcessGameServer()
{


	while (true)
	{
		int index = CheckSocket();
		sockaddr_in Client;

		int ClntLen = sizeof(Client);

		if (index != -1) //���δ��
		{
			//��Ӧǰ���listen�������Ƕ�Ӧ����accept
			cout << "�ȴ�Client����...\n";
			fflush(stdout);
			AcceptSocket[index].ClientSock = accept(
				ListenSocket,
				(struct sockaddr*)&AcceptSocket[index].Client,
				&ClntLen);
			//cout << "#1 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			AcceptSocket[index].ID = index;       //��¼���Client��ID���Ժ�ҪѰ����
			AcceptSocket[index].Active = false;
			char szMsg[256] = { 0 };
			sprintf(szMsg, "�յ� %u ���롣\r\n", AcceptSocket[index].ClientSock);
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;

			if (AcceptSocket[index].ClientSock == INVALID_SOCKET)
			{
				cout << "���Ӵ���\n";
				fflush(stdout);
				break;
			}
			//cout << "#2 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			cout << "���ӳɹ�\n";
			fflush(stdout);
			//����client��server���ӳɹ�,����
			AcceptSocket[index].ID = index;
			//����ID�������Ĳ���
			std::string msg = std::to_string(AcceptSocket[index].ID);    //����ֵת��Ϊ�ַ��������ض�Ӧ���ַ�����
			//SendMessageToOneClient(AcceptSocket[index].ID, msg);
			cout << "#ID :" << msg << "\n";
			cout << "���ӳɹ�\n";
			fflush(stdout);
			playerNum++;
			
			//����client��server���ӳɹ�
			if (((index == 1)&&(con == 0))||((index == 0) && (con == 0)))
			{
				//�����������߳� 
				int ThreadID;     //�߳�ID
				//�Ѹո����ӳɹ���Client����һ���µ��߳�
				ThreadID = (int)CreateThread(NULL, 0,
					(LPTHREAD_START_ROUTINE)(GameServer::ListenThread), //�̵߳㺯��
					(LPVOID)&AcceptSocket[index], 0,              //����
					&AcceptSocket[index].RecvThreadID          //ϵͳ�����ID
				);

				if (!ThreadID)
				{
					cout << "�����̴߳���\n";
					fflush(stdout);
					ExitThread(AcceptSocket[index].RecvThreadID);
				}

				//���ˣ��µ��̴߳����ɹ������Դ���������

				cout << "�����" << index << "�Ľ����̴߳����ɹ�\n";
				fflush(stdout);
				//cout << "#3 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
				struct PlayerInfo temp;
				temp.clientInfo = { NULL,AcceptSocket[index].Client,AcceptSocket[index].ID,0,0,0 };
				temp.nickname = std::to_string(AcceptSocket[index].Client.sin_addr.S_un.S_addr) + "_" +
					std::to_string(AcceptSocket[index].Client.sin_port) + "_" + std::to_string(AcceptSocket[index].ID);      //������
				allPlayerInfo.push_back(temp);

			}
			if (index == 1)        //������client������ʱ��ͻ��˷�һ����Ϣ����Ϸ��ʼ
			{
				if (con == 0)
				{
					std::string check_both_connected = "#Connect Successfully";
					SendMessageToOneClient(AcceptSocket[0].ID, check_both_connected);
					SendMessageToOneClient(AcceptSocket[1].ID, check_both_connected); \
						con = 1;
				}
			}
			cout << "����Ҽ��룬IP��ַΪ��" << inet_ntoa(AcceptSocket[index].Client.sin_addr)
				<< "  �˿ں�Ϊ��" << ntohs(AcceptSocket[index].Client.sin_port) << "\n";
			fflush(stdout);
		}


		else   //�������
		{
			SOCKET Accept = accept(ListenSocket,
				(struct sockaddr*)&Client, &ClntLen);

			if (Accept == INVALID_SOCKET)
			{
				cout << "���Ӵ���\n";
				fflush(stdout);
				break;
			}


			cout << "�Ƿ��������ҵ�IP�ĵ�ַΪ:" << inet_ntoa(Client.sin_addr)
				<< " �˿ں�Ϊ :" << ntohs(Client.sin_port) << "\n";
			fflush(stdout);
			send(Accept, "��ǰ�û�����", strlen("��ǰ�û�����") + sizeof(char), 0);

			closesocket(Accept);

			cout << "�Ƿ���������ѶϿ�\n";
			fflush(stdout);
			break;
		}
	}
	return 0;
}

/*
���ƣ������߳�
������Selectģʽ
	�����жϸ��߳��Ƿ�ɶ�������ɶ��Ͷ�ȡ���ϵ���Ϣ
*/
DWORD WINAPI GameServer::ListenThread(void *data) //�����������ĸ�AcceptSocket[xx]�ĵ�ַ
{
	WZClientInformation *GameSocket = (WZClientInformation *)data;

	while (true)
	{
		cout << "DEBUG:�µ��շ�ѭ��\n";
		//�������� 

		char recvBuf[28000];
		//string recvBuf;
		
		//fd_set Read;//����selectģʽ��IO���й���  
		cout << "DEBUG:1\n";
		//FD_ZERO(&Read);    //��ʼ��Ϊ0  ���fdset�������ļ��������ϵ
		//FD_SET(GameSocket->ClientSock, &Read); //��ClientSock�������
		cout << "DEBUG:2\n";
		//we only care reads
		//int sel = select(0, &Read, NULL, NULL, NULL);
		cout << "DEBUG:select֮��\n";
		ZeroMemory(recvBuf, sizeof(recvBuf));
		//if (FD_ISSET(GameSocket->ClientSock, &Read))     //??
		//{
			//���ܿͻ��˵�����
			int result = recv(GameSocket->ClientSock, recvBuf, sizeof(recvBuf), 0);
			cout << "recv ����ֵ: " << result << "\n";
			fflush(stdout);
			if (result > 0 && result < 28000)
			{
				recvBuf[result] = '\0';
				cout << "���" << GameSocket->ID << "��������Ϣ:"
					<< recvBuf << "\n";
				fflush(stdout);
			}
			if (result == SOCKET_ERROR)
			{
				//CleanSocket(GameSocket->ID);
				break;
			}

			char szMsg[256] = { 0 };
			sprintf(szMsg, "�յ� %u ��Ϣ:%s��\r\n", GameSocket->ClientSock, recvBuf );
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;
			//}

		//�������� 
		char sendBuf[28000];
		//fd_set write;//����selectģʽ��IO���й���  
		//FD_ZERO(&write);    //��ʼ��Ϊ0
		//FD_SET(GameSocket->ClientSock, &write); //��ClientSock�������
		//we only care read
		//select(0, NULL, &write, NULL, NULL);
		ZeroMemory(sendBuf, sizeof(sendBuf));
		//if (FD_ISSET(GameSocket->ClientSock, &write))       //���fdset��ϵ���ļ����fd�Ƿ�ɶ�д�� > 0��ʾ�ɶ�д��
		//{
			//���ܿͻ��˵�����
			memcpy(sendBuf, recvBuf,500);
			//cout << "Just Before Enter TOALL func, AcceptSocket[ID].ClientSock: " << GameSocket->ClientSock << "\n";
			SendMessageToAllClient(sendBuf, GameSocket->ID);
			//SendMessageToOneClient(GameSocket->ID, sendBuf);
		//}

	}
	return 1;
}

/*
���ƣ�������Ϣ��һ��client
������������Ϣ��һ��client
*/
int GameServer::SendMessageToOneClient(int ID, const string  str)
{
	if (ID < 0)
		return 0;

	int oneSend = send(AcceptSocket[ID].ClientSock,
		str.c_str(), str.length() + sizeof(char), 0);    //ͨ��string�����ĳ�Ա����c_str()��string ����ת����c�е��ַ�����ʽ��
	cout << "�����" << ID << "������" << oneSend << "��bits����Ϣ\n";
	fflush(stdout);
	//ZeroMemory(sendBuf, sizeof(sendBuf));
	if (oneSend == SOCKET_ERROR)
	{
		cout << "�����" << ID << "������Ϣʧ��\n";
		fflush(stdout);
		return 0;
	}
	char szMsg[256] = { 0 };
	sprintf(szMsg, "�� %u ������Ϣ:%s��\r\n", AcceptSocket[ID].ClientSock, str.c_str());
	wchar_t *pwszMsg = AnsiToUnicode(szMsg);
	OutputDebugString(pwszMsg);
	delete[] pwszMsg;

	return 1;
}

/*
���ƣ�ȫ����
������������Client������Ϣ
*/
void GameServer::SendMessageToAllClient(const string  str, int ID)
{

	bool flag = true;
	for (int i = 0; i < MAX_NUM; i++)
	{
		//cout << "Just In TOALL func, AcceptSocket[ID].ClientSock :" << AcceptSocket[i].ClientSock << "\n";
		if (ID != i)
		{
			if (AcceptSocket[i].ClientSock != INVALID_SOCKET &&             //����INVALID_SOCKET����
				!SendMessageToOneClient(i, str))
				flag = false;
		}
	}
	if (flag)
		cout << "��������ȫ��(�������" << ID << ")��������Ϣ:"
		<< str << "\n";
	else
		cout << "���ʹ�������\n";
	fflush(stdout);

}


/*
���ƣ�����Socket
�����������˳���Ϸ���߳�
*/
void GameServer::CleanSocket(int ID)
{
	if (ID < 0)
		return;

	char send[20];
	sprintf(send, "#DD i%d*", ID);
	SendMessageToAllClient(send, ID);


	cout << " ���" << ID << "�˳���Ϸ\n";
	fflush(stdout);

	AcceptSocket[ID].Active = false;
	closesocket(AcceptSocket[ID].ClientSock);
	AcceptSocket[ID].ClientSock = INVALID_SOCKET;


	cout << "���ڹر�������߳�:" << AcceptSocket[ID].RecvThreadID << "\n";
	fflush(stdout);
	ExitThread(AcceptSocket[ID].RecvThreadID);



	cout << "�رճɹ�!\n";
	fflush(stdout);
}
