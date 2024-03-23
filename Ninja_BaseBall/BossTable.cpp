#include "pch.h"
#include "BossTable.h"
#include "rapidcsv.h"

BossData BossTable::Undefined = BossData();

BossTable::BossTable(Types type)
    : DataTable(type)
{
}

bool BossTable::Load()
{
    Release();

    rapidcsv::Document doc("tables/BossTable.csv", rapidcsv::LabelParams());
    for (int i = 0; i < doc.GetRowCount(); ++i)
    {
        auto row = doc.GetRow<std::string>(i);
        BossType id = (BossType)std::stoi(row[0]);

        if (table.find(id) != table.end())
        {
            std::cout << "Boss ID Duplicated" << std::endl;
            return false;
        }

        table[id] = BossData();
        table[id].id = id;
        table[id].nameId = row[1];
        table[id].animationChaseClipId = row[2];
        table[id].animationPunchOneClipId = row[3];
        table[id].animationPunchTwoClipId = row[4];
        table[id].animationWindAttackClipId = row[5];
        table[id].animationGunAttackClipId = row[6];
        table[id].animationCryingClipId = row[7];
		table[id].animationDeadClipId = row[8];
    }
    return true;
}

void BossTable::Release()
{
    table.clear();
}

const BossData BossTable::Get(BossType id)
{
    auto find = table.find(id);

    if (find == table.end())
    {
        std::cout << "Can't find boss id";

        return Undefined;
    }

    return find->second;
}