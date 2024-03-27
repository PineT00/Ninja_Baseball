#pragma once
#include "DataTable.h"
#include "WindyPlane.h" // WindyPlane의 정의를 포함해야 합니다.
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"
#include <rapidjson/istreamwrapper.h>

struct BossData
{
    BossPartsStatus status;
    std::vector<std::string> files;
    BossData() {}
    BossData(BossPartsStatus status, std::vector<std::string> files)
        : status(status), files(std::move(files)) {}
};


class BossTable : public DataTable
{
public:
    static std::unordered_map<BossPartsStatus, BossData> Undefined;

protected:
    BossTable(const BossTable&) = delete;
    BossTable(BossTable&&) = delete;
    BossTable& operator=(const BossTable&) = delete;
    BossTable& operator=(BossTable&&) = delete;

    std::unordered_map<std::string, std::unordered_map<BossPartsStatus, BossData>> table;

public:
    BossTable(Types type);
    ~BossTable() override = default;

    bool Load() override;
    void Release() override;

    std::unordered_map<BossPartsStatus, BossData>& Get(BossType id);
};
