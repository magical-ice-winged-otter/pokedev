#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>
#include "imgui_utils.hpp"
#include "serializer.hpp"
#include "text_parser.hpp"

struct NameData
{
    std::string name;
    std::string define;
};

enum class Gender
{
    Male,
    Female,
};

struct TrainerMonProperty
{
    std::string compileLine();
};

struct TrainerMonData
{
    int level;
    NameData species;
    std::string nickname;
    int ev[6];
    int iv[6];
    int friendship;
    bool isShiny;
    Gender gender;
    NameData moves[4];
    NameData ability;
    NameData nature;
    NameData heldItem;
    NameData ball;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            AUTO_NAME(nickname),
            AUTO_NAME(ev),
            AUTO_NAME(iv),
            CUSTOM_NAME("moveOne", moves[0]),
            CUSTOM_NAME("moveTwo", moves[1]),
            CUSTOM_NAME("moveThree", moves[2]),
            CUSTOM_NAME("moveFour", moves[3]),
            AUTO_NAME(species),
            AUTO_NAME(heldItem),
            AUTO_NAME(ability),
            AUTO_NAME(level),
            AUTO_NAME(ball),
            AUTO_NAME(friendship),
            AUTO_NAME(nature),
            AUTO_NAME(gender),
            AUTO_NAME(isShiny)
        );
    }
};

inline std::vector<NameData> parseNameData(const std::filesystem::path& nameFilePath, const std::string& arrayName)
{
    std::vector<NameData> result{};
    std::ifstream stream{nameFilePath};

    TextParser::tryFind(stream, arrayName);
    TextParser::tryFind(stream, '{');

    while (TextParser::tryFind(stream, '[', '}'))
    {
        stream.get(); // skip the '['
        NameData data{};
        data.define = TextParser::readToExclusive(stream, ']');
        TextParser::tryFind(stream, '"');
        stream.get(); // skip the '"'
        data.name = TextParser::readToExclusive(stream, '"');
        result.push_back(data);
    }

    return result;
}

inline std::vector<NameData> parseItemData(const std::filesystem::path& projectFilePath)
{
    std::vector<NameData> result{};
    std::ifstream stream{projectFilePath / "include/constants/items.h"};

    TextParser::tryFind(stream, "#define ITEM_NONE 0");

    while (TextParser::tryFind(stream, '#'))
    {
        NameData data {};
        TextParser::tryFind(stream, ' ');
        stream.get(); // skip the ' '
        std::string value = TextParser::readToExclusive(stream, ' ');

        if (value == "ITEMS_COUNT")
            break;

        data.define = value;
        data.name = value;
        result.push_back(data);
    }

    return result;
}

inline std::vector<NameData> parseNatureData(const std::filesystem::path& projectFilePath)
{
    std::vector<NameData> result{};
    std::ifstream stream{projectFilePath / "src/data/text/nature_names.h"};

    // Pass 1: get all of the name pointers
    std::unordered_map<std::string, std::string> namePointerLookup {};

    while (TextParser::tryFind(stream, "static const u8", '{'))
    {
        stream.ignore(16); // skip the "static const u8 " part
        std::string pointerId = TextParser::readToExclusive(stream, '[');
        TextParser::tryFind(stream, '"');
        stream.get(); // ignore the '"'
        std::string pointerValue = TextParser::readToExclusive(stream, '"');
        namePointerLookup.emplace(pointerId, pointerValue);
    }

    // Pass 2: get all the names
    stream.clear();
    stream.seekg(0);
    TextParser::tryFind(stream, "gNatureNamePointers");
    TextParser::tryFind(stream, '{');

    while (TextParser::tryFind(stream, '[', '}'))
    {
        stream.get(); // skip the '['
        NameData data {};
        data.define = TextParser::readToExclusive(stream, ']');
        TextParser::tryFind(stream, 's');
        std::string pointerId = TextParser::readToExclusive(stream, ',');

        // Try to resolve the pointer reference.
        if (namePointerLookup.contains(pointerId))
        {
            data.name = namePointerLookup[pointerId];
            result.push_back(data);
        }
    }

    return result;
}

inline std::vector<std::string>* createNameArray(const std::vector<NameData>& nameData)
{
    std::vector<std::string>* result = new std::vector<std::string>(nameData.size());

    for (size_t i = 0; i < nameData.size(); i++)
        (*result)[i] = nameData[i].name;

    return result;
}

static std::vector<NameData> s_moveData{};
static ImGuiUtils::FilteredCombo* s_moveCombo1{};
static ImGuiUtils::FilteredCombo* s_moveCombo2{};
static ImGuiUtils::FilteredCombo* s_moveCombo3{};
static ImGuiUtils::FilteredCombo* s_moveCombo4{};
static int s_selectedMove1Index{};
static int s_selectedMove2Index{};
static int s_selectedMove3Index{};
static int s_selectedMove4Index{};

static std::vector<NameData> s_speciesData{};
static ImGuiUtils::FilteredCombo* s_speciesCombo{};
static int s_selectedSpeciesIndex{};

static std::vector<NameData> s_abilityData{};
static ImGuiUtils::FilteredCombo* s_abilityCombo{};
static int s_selectedAbilityIndex{};

static std::vector<NameData> s_natureData{};
static ImGuiUtils::FilteredCombo* s_natureCombo{};
static int s_selectedNatureIndex{};

static std::vector<NameData> s_itemData{};
static ImGuiUtils::FilteredCombo* s_itemCombo{};
static int s_selectedItemIndex{};
static ImGuiUtils::FilteredCombo* s_ballCombo{};
static int s_selectedBallIndex{};

inline void drawMonData(TrainerMonData& data)
{
    if (ImGui::Button("load data"))
    {
        std::filesystem::path projectRoot{R"(\\wsl.localhost\Ubuntu\home\poetahto\projects\decomps\islandgame2)"};

        // Parse move data
        s_moveData = parseNameData(projectRoot / "src/data/text/move_names.h", "gMoveNames");
        std::vector<std::string>* moveNames = createNameArray(s_moveData);
        s_moveCombo1 = new ImGuiUtils::FilteredCombo{*moveNames};
        s_moveCombo2 = new ImGuiUtils::FilteredCombo{*moveNames};
        s_moveCombo3 = new ImGuiUtils::FilteredCombo{*moveNames};
        s_moveCombo4 = new ImGuiUtils::FilteredCombo{*moveNames};

        // parse species data
        s_speciesData = parseNameData(projectRoot / "src/data/text/species_names.h", "gSpeciesNames");
        s_speciesCombo = new ImGuiUtils::FilteredCombo{*createNameArray(s_speciesData)};

        // parse ability data
        s_abilityData = parseNameData(projectRoot / "src/data/text/abilities.h", "gAbilityNames");
        s_abilityCombo = new ImGuiUtils::FilteredCombo{*createNameArray(s_abilityData)};

        // parse nature data
        s_natureData = parseNatureData(projectRoot);
        s_natureCombo = new ImGuiUtils::FilteredCombo{*createNameArray(s_natureData)};

        // parse item data
        s_itemData = parseItemData(projectRoot);
        s_itemCombo = new ImGuiUtils::FilteredCombo{*createNameArray(s_itemData)};

        // parse the ball data (its a subset of the items)
        auto ballData = new std::vector<std::string>{};
        ImGuiTextFilter filter {"BALL"};
        filter.Build();

        for (const NameData& itemData : s_itemData)
        {
            if (itemData.define == "FIRST_BALL")
                break;

            if (filter.PassFilter(itemData.define.c_str()))
                ballData->push_back(itemData.name);
        }

        s_ballCombo = new ImGuiUtils::FilteredCombo{*ballData};
    }

    ImGui::InputText("Nickname", &data.nickname);
    ImGui::DragInt("Friendship", &data.friendship, 1, 0, 255);
    ImGui::InputInt("Level", &data.level);
    data.level = std::min(data.level, 100);
    ImGui::Checkbox("Is Shiny", &data.isShiny);

    if (ImGui::RadioButton("Male", data.gender == Gender::Male)) data.gender = Gender::Male;
    ImGui::SameLine();
    if (ImGui::RadioButton("Female", data.gender == Gender::Female)) data.gender = Gender::Female;

    float spacing = 50;
    for (int i = 0; i < 6; i++)
    {
        ImGui::PushID(i);
        ImGui::SetNextItemWidth(spacing);
        ImGui::DragInt("##EV", &data.ev[i], 1, 0, 252);
        ImGui::SameLine();
        ImGui::PopID();
    }
    ImGui::Text("EV");

    for (int i = 0; i < 6; i++)
    {
        ImGui::PushID(i);
        ImGui::SetNextItemWidth(spacing);
        ImGui::DragInt("##IV", &data.iv[i], 1, 0, 31);
        ImGui::SameLine();
        ImGui::PopID();
    }
    ImGui::Text("IV");

    if (s_moveCombo1 != nullptr && s_moveCombo1->draw("Move 1", s_selectedMove1Index))
        data.moves[0] = s_moveData[s_selectedMove1Index];

    if (s_moveCombo2 != nullptr && s_moveCombo2->draw("Move 2", s_selectedMove2Index))
        data.moves[1] = s_moveData[s_selectedMove2Index];

    if (s_moveCombo3 != nullptr && s_moveCombo3->draw("Move 3", s_selectedMove3Index))
        data.moves[2] = s_moveData[s_selectedMove3Index];

    if (s_moveCombo4 != nullptr && s_moveCombo4->draw("Move 4", s_selectedMove4Index))
        data.moves[3] = s_moveData[s_selectedMove4Index];

    if (s_speciesCombo != nullptr && s_speciesCombo->draw("Species", s_selectedSpeciesIndex))
        data.species = s_speciesData[s_selectedSpeciesIndex];

    if (s_abilityCombo != nullptr && s_abilityCombo->draw("Ability", s_selectedAbilityIndex))
        data.ability = s_abilityData[s_selectedAbilityIndex];

    if (s_natureCombo != nullptr && s_natureCombo->draw("Nature", s_selectedNatureIndex))
        data.nature = s_natureData[s_selectedNatureIndex];

    if (s_itemCombo != nullptr && s_itemCombo->draw("Item", s_selectedItemIndex))
        data.heldItem = s_itemData[s_selectedItemIndex];

    if (s_ballCombo != nullptr && s_ballCombo->draw("Ball", s_selectedBallIndex))
        data.ball = s_itemData[s_selectedBallIndex];
}
