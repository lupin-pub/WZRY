#ifndef _HYSKILLBUTTON_H_
#define _HYSKILLBUTTON_H_

#include "cocos2d.h" 
USING_NS_CC;

class HYSkillButton :public Layer
{
public:


	virtual bool init();
	CREATE_FUNC(HYSkillButton);

	//缓冲精灵帧
	virtual void onEnter();

	//技能按钮 
	MenuItemImage * m_normalskill;
	MenuItemImage * m_SkillOne;
	MenuItemImage * m_SkillTwo;
	MenuItemImage * m_SkillThree;
	//技能
	Menu * m_skills;

	//创建并添加 技能按钮
	//YSSkillButton* YSSkillButton::createSkillButton();
	//bool SkillButtoninit();

	// 技能按钮点击回调 
	void norskillClickCallBack(Ref* obj);
	void skillOneClickCallBack(Ref* obj);
	void skillTwoClickCallBack(Ref* obj);
	void skillThreeClickCallBack(Ref* obj);
	static void HYNorSkill();
	static void HYSkillOne();
	static void HYSkillTwo();
	static void HYSkillThree();

	// 技能冷却完毕回调
	void norSkillCoolDownCallBack(Node *node);
	void skillOneCoolDownCallBack(Node* node);
	void skillTwoCoolDownCallBack(Node* node);
	void skillThreeCoolDownCallBack(Node* node);


	//进度条 
	ProgressTimer * progressTimerNor;
	ProgressTimer * progressTimerOne;
	ProgressTimer * progressTimerTwo;
	ProgressTimer * progressTimerThree;


	//进度条精灵图片
	//Sprite * spProOne;
	//Sprite * spProTwo;
	//Sprite * spProThr;

	//等级的更新
	void updateLevel();
	//被杀次数的更新
	void updateKilled();

	//技能属性的设置
	virtual int getAttact();
	virtual int getMonsterExp();
	virtual int getTowerExpOnce();
	virtual int getTowerExpDeath();

	virtual void setAttact(int temp);

private:
	static Scene * gameScene;

	//调用一次技能更新一次改变的属性
	int addarmor = 0;
	int addRunSpeed = 0;
	//伤害对方的值
	int addAttact = 0;
	int addMonsterExp = 30;
	int addTowerExpOnce = 10;
	int addTowerExpDeath = 100;

	//更改英雄类的属性
	void changeAttributes();
};

#endif