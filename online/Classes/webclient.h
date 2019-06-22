#pragma once
#pragma once
#include "HYSkillButton.h"
#include "DJSkillButton.h"
#include "YSSkillButton.h"
#include <winsock2.h>
#include <process.h>
#include <string>
#include <fstream>
#include <mutex>
#include "DJGameScene.h"
#include "HYGameScene.h"
#include "YSGameScene.h"
#include "GameManager.h" 
#include "BaseMonster.h"
#include "cocos2d.h"
#include <queue>
#include "Crystal.h"
#include "BaseTower.h"

struct TransInfo
{
	int attack;
	int equip;
	float hp;
	int money;
	float force;
	float armor;
	float runspeed;
	float Posx;
	float Posy;
	int direct;
};


class MyClient
{
public:
	~MyClient();
	bool MyProcess();
	bool virtual init();

	static DWORD WINAPI InfoSendThread(LPVOID lpParam);
	static DWORD WINAPI InfoRecvThread(LPVOID lpParam);
	static DWORD WINAPI InfoControlThread(LPVOID lpParam);

	void MyHeroSelect(int type);
	bool GameConnect();
	void EnemyHeroSelect();
	static std::queue<TransInfo> recvQueue;

public:
	SOCKET ClientSock;
	struct sockaddr_in ServerSockAddr;
	//char RecvBuf[1<<10];ÁÄÌì 
	//char SendBuff[1<<10];

private:
	HANDLE hMutex;


};