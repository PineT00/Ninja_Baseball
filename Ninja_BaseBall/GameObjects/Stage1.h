#pragma once
#include "SpriteGo.h"
class Stage1 :
    public SpriteGo
{
public:
	SpriteGo stageStart;
	SpriteGo stageNormal;
	SpriteGo stageBroken1;
	SpriteGo stageBroken2;
	SpriteGo transition;
	SpriteGo stagePlane;
	SpriteGo stageBoss;

	SpriteGo stageBack1;
	SpriteGo stageBack2;
	SpriteGo stageBack3;


	sf::RectangleShape groundBound;
	sf::RectangleShape groundBoundBoss;
	sf::RectangleShape stageBound1_1; //�߱���(�⺻)
	sf::RectangleShape stageBound1_2; //�߱���(������)
	sf::RectangleShape stageBound1_3; //�߱���(����)
	sf::RectangleShape stageBound1_4; //�۷���
	//sf::RectangleShape stageBound1_5; //�μ��� ��������1 //�۷���
	//sf::RectangleShape stageBound1_6; //�μ��� ��������2 //�۷���, �߱��� (�����߰�����)
	sf::RectangleShape stageBound1_7; //����� �������� //��Ʈ
	sf::RectangleShape stageBound1_8; //����� ���� ��������

	bool clearStage1_1 = false;
	bool clearStage1_2 = false;
	bool clearStage1_3 = false;
	bool clearStage1_4 = false;
	bool clearStage1_5 = false; // �μ��� ��������1
	bool clearStage1_6 = false; // �μ��� ��������2
	bool clearStage1_7 = false; // ����� ��������
	bool clearStage1_8 = false; // ����� ���� ��������

private:


public:
	Stage1(const std::string& name = "");
	~Stage1() = default;
	void Init() override;
	void Reset() override;
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

