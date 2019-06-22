#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <random>
#include <cstring>
#pragma comment(lib,"ws2_32.lib")
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "webclient.h"
//����һЩͷ�ļ�Ҫ���� ����..

#define PORTS 1236  //�˿ں�~~~�����ڶ�~~~ 
USING_NS_CC;

using std::queue;
using std::fstream;
using std::string;
using std::endl;

std::mutex g_lock;

std::queue<TransInfo> MyClient::recvQueue;
//currHp MONEY force armor runSpeed //���

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
bool MyClient::init()
{
	std::fstream outfile;
	outfile.open("d:\\glorylog.txt");

	WSADATA MyWSAData;
	if (WSAStartup(MAKEWORD(2, 2), &MyWSAData) != 0)
	{
		return false;
	}

	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_port = htons(PORTS);
	ServerSockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.101");

	if ((ClientSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	unsigned long ul = 1;
	int ret = 0;//ioctlsocket(ClientSock, FIONBIO, (unsigned long *)&ul);
	/*if (ret == SOCKET_ERROR)
	{
		outfile << "the setting of non-blocks failed!";
		closesocket(ClientSock);
		WSACleanup();
		return false;
	}
	else
	{
		outfile << "the setting of non-blocks succeed" << endl;
	}*/

	if (connect(ClientSock, (LPSOCKADDR)&ServerSockAddr, sizeof(ServerSockAddr)) < 0)
	{
		fd_set wfd;
		struct timeval SuspendTime;

		FD_ZERO(&wfd);
		FD_SET(ClientSock, &wfd);
		SuspendTime.tv_sec = 0.1;
		SuspendTime.tv_usec = 0;

		int sel = select(ClientSock, NULL, &wfd, NULL, &SuspendTime);
		if (sel <= 0)
		{
			outfile << "connection fail" << endl;
			closesocket(ClientSock);
			WSACleanup();
			return false;
		}
	}
	outfile << "the client has connected successfully!" << endl;
	outfile.close();
	return true;
}


DWORD WINAPI MyClient::InfoSendThread(LPVOID lpParam)
{
	int times = 0;
	MyClient *myclient = static_cast<MyClient*>(lpParam);
	const int PACKAGE = 53;//���� ����δ�� 
	char SendContent[1 << 10];
	TransInfo* reserve = nullptr;
	TransInfo* thesend;
	auto gm = GameManager::getInstance();
	Scene* RunGameScene = Director::getInstance()->getRunningScene();
	BaseMonster* myplayer = nullptr;
	if (gm->MyType == 0)
	{
		Houyi* myplayer = dynamic_cast<Houyi*>((static_cast<HYGameScene*>(RunGameScene))->hero);///////////�趨���� 
	}
	if (gm->MyType == 1)
	{
		Daji* myplayer = dynamic_cast<Daji*>((static_cast<DJGameScene*>(RunGameScene))->hero);///////////�趨���� 
	}
	if (gm->MyType == 2)
	{
		Yase* myplayer = dynamic_cast<Yase*>((static_cast<YSGameScene*>(RunGameScene))->hero);///////////�趨����  
	}
	//	static int times = 0;
	while (true)
	{
		fd_set wfd;
		FD_ZERO(&wfd);
		FD_SET(myclient->ClientSock, &wfd);
		int sel = select(0, NULL, &wfd, NULL, NULL);
		if (sel > 0)
		{
			if (FD_ISSET(myclient->ClientSock, &wfd))
			{
				thesend = new TransInfo[1];
				while (true)
				{
					if (gm->MyType != 10)
						break;
				}
				if (myplayer == nullptr)
				{
					break;
				}
				Crystal* mycrystal = static_cast<Crystal*>(RunGameScene->getChildByTag(808)->getChildByTag(6061)); ///////////////pyҪ����һ��tag 
				if (mycrystal->getCurrHp() <= 0) //ˮ����ûѪ�� 
				{
					send(myclient->ClientSock, "!Gameover_youwin!", strlen("!Gameover_youwin!") + sizeof(char), 0);
					break;
				}
				thesend->attack = myplayer->getIsAttacking();      /////////////attack���ܻ��кܶ� Ҫ��isAttacking�ĳ�int���ͣ���Ӧ��1234�����û�й�����Ϊ0
				thesend->equip = myplayer->getIsequip();      //////////////�������װ��1-39 û���� ����0  ����Ҫ��basemonster.h���һ��int���͵�isequip
				Vec2 pos = myplayer->getPosition();
				if (reserve->Posx != pos.x || reserve->Posy != pos.y || thesend->attack || thesend->equip) //////////////���Ӹ�py 
				{
					thesend->Posx = pos.x;//currHp MONEY force armor runSpeed //���
					thesend->Posy = pos.y;
					thesend->hp = myplayer->getCurrHp();
					thesend->money = myplayer->getMoney();
					thesend->force = myplayer->getForce();
					thesend->armor = myplayer->getArmor();
					thesend->runspeed = myplayer->getRunSpeed();
					if (0 < myplayer->getState() < 5)
						thesend->direct = myplayer->getState(); //enum����ת��Ϊint���� 
					if (reserve != nullptr)
					{
						if (!memcmp(reserve, thesend, sizeof(reserve)) && !thesend->attack && !thesend->equip)  //���û���Ͳ��� 
						{
							times++;
							if (times < 200)
								continue;
							else
								times = 0;
						}
						else
							memcpy(reserve, thesend, sizeof(reserve));
					}
					else
					{
						reserve = new TransInfo[1];
						memcpy(reserve, thesend, sizeof(reserve));
					}
					ZeroMemory(SendContent, sizeof(SendContent));//currHp MONEY force armor runSpeed //���
					sprintf(SendContent, "%f %d %f %f %f %d %d %d %f %f", thesend->hp, thesend->money, thesend->force, thesend->armor, thesend->runspeed, thesend->equip, thesend->attack, thesend->direct, pos.x, pos.y);

					int byte = 0;
					while (byte < PACKAGE)
					{
						int m = send(myclient->ClientSock, SendContent, strlen(SendContent) + sizeof(char), 0);
						byte += m;
					}
				}
				delete[]thesend;
			}
		}
	}
	if (reserve != nullptr)
	{
		delete[]reserve;
	}
	return 1;
}


DWORD WINAPI MyClient::InfoRecvThread(LPVOID lpParam)
{
	MyClient *myclient = static_cast<MyClient*>(lpParam);
	const int PACKAGE = 53;
	char RecvContent[PACKAGE * 100];

	while (true)
	{
		fd_set rfd;
		FD_ZERO(&rfd);
		FD_SET(myclient->ClientSock, &rfd);
		int sel = select(0, &rfd, NULL, NULL, NULL);
		if (sel > 0)
		{
			if (FD_ISSET(myclient->ClientSock, &rfd))
			{
				ZeroMemory(RecvContent, sizeof(RecvContent));

				int direct2;
				int attack2 = 0;

				int byte2 = 0;
				int n;
				while (byte2 < PACKAGE)
				{
					n = recv(myclient->ClientSock, RecvContent, PACKAGE * 100, 0);
					if (n == SOCKET_ERROR)
						continue;
					byte2 += n;
				}
				RecvContent[PACKAGE] = '\0';

				if (RecvContent[0] == '!')
				{
					break;
				}

				for (int i = 0; i < (byte2 / PACKAGE); i++)
				{
					TransInfo temp;//currHp MONEY force armor runSpeed //���
					sscanf(RecvContent + i * PACKAGE, "%f %d %f %f %f %d %d %d %d %f %f", &temp.hp, &temp.money, &temp.force, &temp.armor, &temp.runspeed, &temp.equip, &temp.attack, &temp.direct, &temp.Posx, &temp.Posy);

					g_lock.lock();
					recvQueue.push(temp);
					g_lock.unlock();
				}
			}
		}
	}
}

//attackҪ��Ϊ0 1 �Ҳ��ܼ�⵽�ı�
DWORD WINAPI MyClient::InfoControlThread(LPVOID lpParam)
{
	MyClient *myclient = (MyClient*)lpParam;
	auto gm = GameManager::getInstance();
	Scene* RunGameScene = Director::getInstance()->getRunningScene();
	BaseMonster* enemy;
	if (gm->EnemyType == 0)
		enemy = dynamic_cast<Houyi*>(static_cast<HYGameScene*>(RunGameScene)->getChildByName("enemy"));/////////
	if (gm->EnemyType == 1)
		enemy = dynamic_cast<Daji*>(static_cast<DJGameScene*>(RunGameScene)->getChildByName("enemy"));
	if (gm->EnemyType == 2)
		enemy = dynamic_cast<Yase*>(static_cast<YSGameScene*>(RunGameScene)->getChildByName("enemy"));

	while (true)
	{
		TransInfo temp;
		g_lock.lock();
		if (!recvQueue.empty())
		{
			temp = recvQueue.front();
			recvQueue.pop();
			g_lock.unlock();
		}
		else
		{
			g_lock.unlock();
			continue;
		}
		Vec2 Pos(temp.Posx, temp.Posy);
		if (temp.attack)
		{
			if (temp.attack == 1)
			{
				if (gm->MyType == 0)
				{
					HYSkillButton::HYSkillOne();
				}
				if (gm->MyType == 1)
				{
					DJSkillButton::DJSkillOne();
				}
				if (gm->MyType == 2)
				{
					HYSkillButton::HYSkillOne();
				}
			}
			if (temp.attack == 2)
			{
				if (gm->MyType == 0)
				{
					HYSkillButton::HYSkillTwo();
				}
				if (gm->MyType == 1)
				{
					DJSkillButton::DJSkillTwo();
				}
				if (gm->MyType == 2)
				{
					HYSkillButton::HYSkillTwo();
				}
			}
			if (temp.attack == 3)
			{
				if (gm->MyType == 0)
				{
					HYSkillButton::HYSkillThree();
				}
				if (gm->MyType == 1)
				{
					DJSkillButton::DJSkillThree();
				}
				if (gm->MyType == 2)
				{
					HYSkillButton::HYSkillThree();
				}
			}
			if (temp.attack == 4)
			{
				if (gm->MyType == 0)
				{
					HYSkillButton::HYNorSkill();
				}
				if (gm->MyType == 1)
				{
					DJSkillButton::DJNorSkill();
				}
				if (gm->MyType == 2)
				{
					HYSkillButton::HYNorSkill();
				}
			}
		}

		if (temp.Posx > 0 && temp.Posy > 0)
		{
			enemy->setPosition(1024 - temp.Posx, 576 - temp.Posy);
		}
		else
			continue;//currHp MONEY force armor runSpeed //���
		enemy->setCurrHp(temp.hp);
		enemy->setMoney(temp.money);
		enemy->setForce(temp.force);
		enemy->setArmor(temp.armor);
		enemy->setRunSpeed(temp.runspeed);
	}
	return 1;
}


bool MyClient::MyProcess()
{

	HANDLE SendThread, RecvThread, ControlThread;
	SendThread = CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(MyClient::InfoSendThread), static_cast<LPVOID>(this), 0, NULL);
	RecvThread = CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(MyClient::InfoRecvThread), static_cast<LPVOID>(this), 0, NULL);
	ControlThread = CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(MyClient::InfoControlThread), static_cast<LPVOID>(this), 0, NULL); //��ȷ������ 

	CloseHandle(SendThread);
	CloseHandle(RecvThread);
	CloseHandle(ControlThread);

	return true;
}

bool MyClient::GameConnect()
{
			char buf[23];
			ZeroMemory(buf, sizeof(buf));
			int m = 0, n = 0;
			int x = n;
			while (m < 22)
			{
				n = recv(ClientSock, buf + x, strlen(buf) + sizeof(char), 0);
				x += n;
				if (n == SOCKET_ERROR)
					continue;
				m += n;
			}
			buf[22] = '\0';

			char szMsg[256] = { 0 };
			sprintf(szMsg, "�յ� %u ��Ϣ:%s��\r\n", ClientSock, buf);
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;

			if (buf[0] == '#')
				return true;
			else
				return false;
}

		void MyClient::MyHeroSelect(int type)
		{
			char buf[2];
			ZeroMemory(buf, sizeof(buf));
			if (type == 1)
				buf[0] = '1';
			if (type == 2)
				buf[0] = '2';
			if (type == 3)
				buf[0] = '3';
			buf[1] = '\0';
			int nLen = strlen(buf) + sizeof(char);
			int nRet = send(ClientSock, buf, nLen, 0);

			char szMsg[256] = { 0 };
			sprintf(szMsg, "�� %u ������Ϣ:%s��\r\n", ClientSock, buf);
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;
		}


		void MyClient::EnemyHeroSelect()
		{
			auto gm = GameManager::getInstance();
			char buf[2];
			ZeroMemory(buf, sizeof(buf));
			int m = 0, n = 0;
			int x = 0;
			while (m < 2)
			{
				n = recv(ClientSock, buf + x, strlen(buf) + sizeof(char), 0);
				x += n;
				if (n == SOCKET_ERROR)
					continue;
				m += n;
			}
			buf[1] = '\0';

			char szMsg[256] = { 0 };
			sprintf(szMsg, "�յ� %u ��Ϣ:%s��\r\n", ClientSock, buf);
			wchar_t *pwszMsg = AnsiToUnicode(szMsg);
			OutputDebugString(pwszMsg);
			delete[] pwszMsg;

			if (n == SOCKET_ERROR)
				int dsfdf = 0;
			if (buf != nullptr)
			{
				if (buf[0] == '1')
					gm->EnemyType = 1;
				if (buf[0] == '2')
					gm->EnemyType = 2;
				if (buf[0] == '3')
					gm->EnemyType = 3;
			}
		}


		MyClient::~MyClient()
		{
			closesocket(ClientSock);
			WSACleanup();
		}

		