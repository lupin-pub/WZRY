#ifndef _HYSKILLBUTTON_H_
#define _HYSKILLBUTTON_H_

#include "cocos2d.h" 
USING_NS_CC;

class HYSkillButton :public Layer
{
public:


	virtual bool init();
	CREATE_FUNC(HYSkillButton);

	//���徫��֡
	virtual void onEnter();

	//���ܰ�ť 
	MenuItemImage * m_normalskill;
	MenuItemImage * m_SkillOne;
	MenuItemImage * m_SkillTwo;
	MenuItemImage * m_SkillThree;
	//����
	Menu * m_skills;

	//��������� ���ܰ�ť
	//YSSkillButton* YSSkillButton::createSkillButton();
	//bool SkillButtoninit();

	// ���ܰ�ť����ص� 
	void norskillClickCallBack(Ref* obj);
	void skillOneClickCallBack(Ref* obj);
	void skillTwoClickCallBack(Ref* obj);
	void skillThreeClickCallBack(Ref* obj);
	static void HYNorSkill();
	static void HYSkillOne();
	static void HYSkillTwo();
	static void HYSkillThree();

	// ������ȴ��ϻص�
	void norSkillCoolDownCallBack(Node *node);
	void skillOneCoolDownCallBack(Node* node);
	void skillTwoCoolDownCallBack(Node* node);
	void skillThreeCoolDownCallBack(Node* node);


	//������ 
	ProgressTimer * progressTimerNor;
	ProgressTimer * progressTimerOne;
	ProgressTimer * progressTimerTwo;
	ProgressTimer * progressTimerThree;


	//����������ͼƬ
	//Sprite * spProOne;
	//Sprite * spProTwo;
	//Sprite * spProThr;

	//�ȼ��ĸ���
	void updateLevel();
	//��ɱ�����ĸ���
	void updateKilled();

	//�������Ե�����
	virtual int getAttact();
	virtual int getMonsterExp();
	virtual int getTowerExpOnce();
	virtual int getTowerExpDeath();

	virtual void setAttact(int temp);

private:
	static Scene * gameScene;

	//����һ�μ��ܸ���һ�θı������
	int addarmor = 0;
	int addRunSpeed = 0;
	//�˺��Է���ֵ
	int addAttact = 0;
	int addMonsterExp = 30;
	int addTowerExpOnce = 10;
	int addTowerExpDeath = 100;

	//����Ӣ���������
	void changeAttributes();
};

#endif