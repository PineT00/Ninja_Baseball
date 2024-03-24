#include "pch.h"
#include "BossTable.h"

std::unordered_map<BossPartsStatus, BossData> BossTable::Undefined = std::unordered_map<BossPartsStatus, BossData>() ;

BossTable::BossTable(Types type)
    : DataTable(type)
{
}

BossPartsStatus GetBossPartsStatusFromString(const std::string& typeStr) {
    if (typeStr == "Wing") return BossPartsStatus::Wing;
    else if (typeStr == "NoWing") return BossPartsStatus::NoWing;
    else if (typeStr == "NoProp") return BossPartsStatus::NoProp;
    else if (typeStr == "OneArm") return BossPartsStatus::OneArm;
    else if (typeStr == "NoArm") return BossPartsStatus::NoArm;

    return BossPartsStatus::UNDEFINED;
}

bool BossTable::Load() {
    Release();

    std::ifstream ifs("tables/BossTable.json");
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file for reading" << std::endl;
        return false;
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (!doc.IsObject()) {
        std::cerr << "Document is not a JSON object" << std::endl;
        return false;
    }

    const rapidjson::Value& boss = doc["boss"];

    for (auto bossIt = boss.MemberBegin(); bossIt != boss.MemberEnd(); ++bossIt) {
        const std::string bossName = bossIt->name.GetString();
        const rapidjson::Value& states = bossIt->value;

        if (!states.IsObject()) continue; // 상태가 객체가 아니면 넘어감

        for (auto it = states.MemberBegin(); it != states.MemberEnd(); ++it) {
            BossPartsStatus id = GetBossPartsStatusFromString(it->name.GetString());

            BossData data;
            data.status = id;

            const rapidjson::Value& files = it->value["files"];
            if (files.IsArray()) {
                for (rapidjson::SizeType i = 0; i < files.Size(); i++) {
                    data.files.push_back(files[i].GetString());
                }
            }

            // bossName을 키로 하고, 그 안의 std::unordered_map에 status와 data를 저장
            table[bossName][id] = data;
        }
    }
    return true;
}


//bool BossTable::Load() {
//    Release();
//
//    std::ifstream ifs("animations/Boss/BossTable.json");
//    if (!ifs.is_open()) {
//        std::cerr << "Failed to open file for reading" << std::endl;
//        return false;
//    }
//
//    rapidjson::IStreamWrapper isw(ifs);
//
//    rapidjson::Document doc;
//    doc.ParseStream(isw);
//    ifs.close(); // std::ifstream은 스코프를 벗어나면 자동으로 닫히지만, 명시적으로 닫는 것이 좋습니다.
//
//    if (!doc.IsObject()) {
//        std::cerr << "Document is not a JSON object" << std::endl;
//        return false;
//    }
//
//    const rapidjson::Value& boss = doc["boss"];
//
//    for (auto bossIt = boss.MemberBegin(); bossIt != boss.MemberEnd(); ++bossIt) {
//        const rapidjson::Value& bossName = (*bossIt).value;
//
//        for (auto it = bossName.MemberBegin(); it != bossName.MemberEnd(); ++it) {
//            BossPartsStatus id = GetBossPartsStatusFromString(it->name.GetString());
//
//            BossData data;
//            data.status = id;
//
//            const rapidjson::Value& files = it->value["files"];
//            if (files.IsArray()) {
//                for (rapidjson::SizeType i = 0; i < files.Size(); i++) {
//                    data.files.push_back(files[i].GetString());
//                }
//            }
//            // Note: 여기서의 table[bossName][data.status] = data; 구문은 잘못된 것 같습니다.
//            // bossName은 Value 객체이고, table의 키로 사용할 수 없습니다. 아마도 수정이 필요할 것입니다.
//        }
//    }
//    return true;
//}

void BossTable::Release()
{
    table.clear();
}

std::unordered_map<BossPartsStatus, BossData>& BossTable::Get(BossType id) 
{
    std::string name;

    switch (id) {
    case BossType::WINDYPLANE:
        name = "WindyPlane";
        break;
    default:
        std::cerr << "Unsupported BossType." << std::endl;
        return Undefined;
    }

    auto findIt = table.find(name);

    if (findIt == table.end()) {
        std::cerr << "No BossData found for the given BossType." << std::endl;
        return Undefined;
    }

    return findIt->second;
}