#pragma once
#include "DataTable.h"

struct BossData
{
	BossType id;
	std::string nameId;
	std::string animationChaseClipId;
	std::string animationPunchOneClipId;
	std::string animationPunchTwoClipId;
	std::string animationWindAttackClipId;
	std::string animationGunAttackClipId;
	std::string animationCryingClipId;
	std::string animationDeadClipId;
};

class BossTable : public DataTable
{
public:
	static BossData Undefined;

protected:
	BossTable(const BossTable&) = delete;
	BossTable(BossTable&&) = delete;
	BossTable& operator=(const BossTable&) = delete;
	BossTable& operator=(BossTable&&) = delete;

	std::unordered_map <BossType, BossData> table;

public:
	BossTable(Types type);
	~BossTable() override = default;

	bool Load() override;
	void Release() override;

	const BossData Get(BossType id);
};