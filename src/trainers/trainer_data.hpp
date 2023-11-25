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

struct NameDataList
{
    std::vector<std::string> names{};
    std::vector<std::string> defines{};

    [[nodiscard]]
    size_t size() const
    {
        assert(names.size() == defines.size());
        return names.size();
    }
};

struct PokeemeraldData
{
    NameDataList moves{};
    NameDataList species{};
    NameDataList abilities{};
    NameDataList natures{};
    NameDataList items{};
};

static PokeemeraldData s_data{};

enum class Gender
{
    Male,
    Female,
};

struct TrainerMonProperty
{
    virtual ~TrainerMonProperty() = default;

    virtual std::string compileLine() = 0;
    virtual void draw() = 0;
    virtual std::string getName() = 0;
};

struct NicknameProperty final : TrainerMonProperty
{
    std::string nickname;

    std::string compileLine() override
    {
        return std::format(".nickname = COMPOUND_STRING(\"{}\")", nickname);
    }

    void draw() override
    {
        ImGui::InputText("##Nickname", &nickname);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(nickname));
    }

    std::string getName() override
    {
        return "Nickname";
    }
};

struct EvProperty final : TrainerMonProperty
{
    int ev[6]{};

    std::string compileLine() override
    {
        return std::format(".ev = TRAINER_PARTY_EVS({}, {}, {}, {}, {}, {})", ev[0], ev[1], ev[2], ev[3], ev[4], ev[5]);
    }

    void draw() override
    {
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 6);

        for (int i = 0; i < 6; i++)
        {
            ImGui::PushID(i);
            ImGui::DragInt("##EV", &ev[i], 1, 0, 252);

            if (i < 5)
                ImGui::SameLine();

            ImGui::PopID();
        }
        ImGui::PopItemWidth();
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(ev));
    }

    std::string getName() override
    {
        return "EVs";
    }
};

struct IvProperty final : TrainerMonProperty
{
    int iv[6]{};

    std::string compileLine() override
    {
        return std::format(".iv = TRAINER_PARTY_IVS({}, {}, {}, {}, {}, {})", iv[0], iv[1], iv[2], iv[3], iv[4], iv[5]);
    }

    void draw() override
    {
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 6);

        for (int i = 0; i < 6; i++)
        {
            ImGui::PushID(i);
            ImGui::DragInt("##IV", &iv[i], 1, 0, 252);

            if (i < 5)
                ImGui::SameLine();

            ImGui::PopID();
        }
        ImGui::PopItemWidth();
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(iv));
    }

    std::string getName() override
    {
        return "IVs";
    }
};

struct FriendshipProperty final : TrainerMonProperty
{
    int friendship{};

    std::string compileLine() override
    {
        return std::format(".friendship = {}", friendship);
    }

    void draw() override
    {
        ImGui::DragInt("##Friendship", &friendship, 1, 0, 255);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(friendship));
    }

    std::string getName() override
    {
        return "Friendship";
    }
};

struct ShinyProperty final : TrainerMonProperty
{
    bool isShiny {};

    std::string compileLine() override
    {
        return std::format(".isShiny = {}", isShiny ? "TRUE" : "FALSE");
    }

    void draw() override
    {
        ImGui::Checkbox("##Is Shiny", &isShiny);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(isShiny));
    }

    std::string getName() override
    {
        return "Is Shiny";
    }
};

struct GenderProperty final : TrainerMonProperty
{
    Gender gender {};

    std::string compileLine() override
    {
        return std::format(".gender = TRAINER_MON_{}", gender == Gender::Female ? "FEMALE" : "MALE");
    }

    void draw() override
    {
        if (ImGui::RadioButton("Male", gender == Gender::Male)) gender = Gender::Male;
        ImGui::SameLine();
        if (ImGui::RadioButton("Female", gender == Gender::Female)) gender = Gender::Female;
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(gender));
    }

    std::string getName() override
    {
        return "Gender";
    }
};

struct MovesProperty final : TrainerMonProperty
{
    MovesProperty() :
        m_moveCombo1(s_data.moves.names),
        m_moveCombo2(s_data.moves.names),
        m_moveCombo3(s_data.moves.names),
        m_moveCombo4(s_data.moves.names)
    {
    }

    std::string compileLine() override
    {
        return std::format(".moves = {{{}, {}, {}, {}}}",
                           s_data.moves.defines[m_selectedMove1Index],
                           s_data.moves.defines[m_selectedMove2Index],
                           s_data.moves.defines[m_selectedMove3Index],
                           s_data.moves.defines[m_selectedMove4Index]
        );
    }

    void draw() override
    {
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4);
        m_moveCombo1.draw("##Move 1", m_selectedMove1Index);
        ImGui::SameLine();
        m_moveCombo2.draw("##Move 2", m_selectedMove2Index);
        ImGui::SameLine();
        m_moveCombo3.draw("##Move 3", m_selectedMove3Index);
        ImGui::SameLine();
        m_moveCombo4.draw("##Move 4", m_selectedMove4Index);
        ImGui::PopItemWidth();
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CUSTOM_NAME("moveIndex_1", m_selectedMove1Index),
            CUSTOM_NAME("moveIndex_2", m_selectedMove2Index),
            CUSTOM_NAME("moveIndex_3", m_selectedMove3Index),
            CUSTOM_NAME("moveIndex_4", m_selectedMove4Index)
        );
    }

    std::string getName() override
    {
        return "Moves";
    }

private:
    ImGuiUtils::FilteredCombo m_moveCombo1;
    ImGuiUtils::FilteredCombo m_moveCombo2;
    ImGuiUtils::FilteredCombo m_moveCombo3;
    ImGuiUtils::FilteredCombo m_moveCombo4;
    int m_selectedMove1Index{};
    int m_selectedMove2Index{};
    int m_selectedMove3Index{};
    int m_selectedMove4Index{};
};

struct AbilityProperty final : TrainerMonProperty
{
    AbilityProperty() : m_combo{s_data.abilities.names}
    {
    }

    std::string compileLine() override
    {
        return std::format(".ability = {}", s_data.abilities.defines[m_selectedIndex]);
    }

    void draw() override
    {
        m_combo.draw("##Ability", m_selectedIndex);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(CUSTOM_NAME("abilityIndex", m_selectedIndex));
    }

    std::string getName() override
    {
        return "Ability";
    }

private:
    ImGuiUtils::FilteredCombo m_combo;
    int m_selectedIndex{};
};

struct NatureProperty final : TrainerMonProperty
{
    NatureProperty() : m_combo{s_data.natures.defines}
    {
    }

    std::string compileLine() override
    {
        return std::format(".nature = TRAINER_PARTY_NATURE({})", s_data.natures.defines[m_selectedIndex]);
    }

    void draw() override
    {
        m_combo.draw("##Nature", m_selectedIndex);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(CUSTOM_NAME("natureIndex", m_selectedIndex));
    }

    std::string getName() override
    {
        return "Nature";
    }

private:
    ImGuiUtils::FilteredCombo m_combo;
    int m_selectedIndex{};
};

struct ItemProperty final : TrainerMonProperty
{
    ItemProperty() : m_combo{s_data.items.names}
    {
    }

    std::string compileLine() override
    {
        return std::format(".heldItem = {}", s_data.items.defines[m_selectedIndex]);
    }

    void draw() override
    {
        m_combo.draw("##Item", m_selectedIndex);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(CUSTOM_NAME("itemIndex", m_selectedIndex));
    }

    std::string getName() override
    {
        return "Held Item";
    }

private:
    ImGuiUtils::FilteredCombo m_combo;
    int m_selectedIndex{};
};

struct BallProperty final : TrainerMonProperty
{
    BallProperty() : m_combo{m_ballData.names}
    {
        ImGuiTextFilter filter{"BALL"};
        filter.Build();

        for (size_t i = 0; i < s_data.items.size(); i++)
        {
            if (s_data.items.defines[i] == "FIRST_BALL")
                break;

            if (filter.PassFilter(s_data.items.defines[i].c_str()))
            {
                m_ballData.names.push_back(s_data.items.names[i]);
                m_ballData.defines.push_back(s_data.items.defines[i]);
            }
        }
    }

    std::string compileLine() override
    {
        return std::format(".ball = {}", m_ballData.defines[m_selectedIndex]);
    }

    void draw() override
    {
        m_combo.draw("##Ball", m_selectedIndex);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(AUTO_NAME(m_selectedIndex));
    }

    std::string getName() override
    {
        return "Ball";
    }

private:
    ImGuiUtils::FilteredCombo m_combo;
    NameDataList m_ballData{};
    int m_selectedIndex{};
};

template <typename T>
struct Toggleable
{
    bool isActive;
    T value;

    bool drawBegin()
    {
        ImGui::Checkbox("##CHECKBOX", &isActive);
        ImGui::SameLine();

        if (!isActive)
            ImGui::BeginDisabled();

        return isActive;
    }

    void drawEnd()
    {
        if (!isActive)
            ImGui::EndDisabled();
    }
};

class TrainerMonData
{
public:
    std::vector<Toggleable<TrainerMonProperty&>> properties;

    TrainerMonData() : m_speciesCombo{s_data.species.defines}
    {
    }

    std::string generateData()
    {
        std::string result{"    {\n"};
        result += std::format("        .species = {},\n        .lvl = {}",
                              s_data.species.defines[m_selectedSpeciesIndex], m_level);

        for (auto property : properties)
        {
            if (property.isActive)
                result += std::format(",\n        {}", property.value.compileLine());
        }

        result += "\n    }";
        return result;
    }

    void draw()
    {
        m_speciesCombo.draw("##Species", m_selectedSpeciesIndex);
        ImGui::Text("Level"); ImGui::SameLine();
        ImGui::InputInt("##Level", &m_level);
        m_level = std::clamp(m_level, 0, 100);

        for (size_t i = 0; i < properties.size(); i++)
        {
            ImGui::PushID(static_cast<int>(i));
            bool shouldDraw = properties[i].drawBegin();
            ImGui::Text("%s", properties[i].value.getName().c_str());

            if (shouldDraw)
            {
                ImGui::SameLine();
                properties[i].value.draw();
            }

            properties[i].drawEnd();
            ImGui::PopID();
        }
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CUSTOM_NAME("speciesIndex", m_selectedSpeciesIndex),
            CUSTOM_NAME("level", m_level),
            AUTO_NAME(properties)
        );
    }

private:
    ImGuiUtils::FilteredCombo m_speciesCombo;
    int m_selectedSpeciesIndex{};
    int m_level{1};
};

inline void initializeEmeraldExpansionProperties(TrainerMonData& data)
{
    data.properties.clear();
    data.properties.emplace_back(false, *new NicknameProperty);
    data.properties.emplace_back(false, *new AbilityProperty);
    data.properties.emplace_back(false, *new ItemProperty);
    data.properties.emplace_back(false, *new MovesProperty);
    data.properties.emplace_back(false, *new NatureProperty);
    data.properties.emplace_back(false, *new EvProperty);
    data.properties.emplace_back(false, *new IvProperty);
    data.properties.emplace_back(false, *new ShinyProperty);
    data.properties.emplace_back(false, *new BallProperty);
    data.properties.emplace_back(false, *new GenderProperty);
    data.properties.emplace_back(false, *new FriendshipProperty);
}

inline NameDataList parseNameData(const std::filesystem::path& nameFilePath, const std::string& arrayName)
{
    NameDataList result{};
    std::ifstream stream{nameFilePath};

    TextParser::tryFind(stream, arrayName);
    TextParser::tryFind(stream, '{');

    while (TextParser::tryFind(stream, '[', '}'))
    {
        stream.get(); // skip the '['
        NameData data{};
        result.defines.push_back(TextParser::readToExclusive(stream, ']'));
        TextParser::tryFind(stream, '"');
        stream.get(); // skip the '"'
        result.names.push_back(TextParser::readToExclusive(stream, '"'));
    }

    return result;
}

inline NameDataList parseItemData(const std::filesystem::path& projectFilePath)
{
    NameDataList result{};
    std::ifstream stream{projectFilePath / "include/constants/items.h"};

    TextParser::tryFind(stream, "#define ITEM_NONE 0");

    while (TextParser::tryFind(stream, '#'))
    {
        TextParser::tryFind(stream, ' ');
        stream.get(); // skip the ' '
        std::string value = TextParser::readToExclusive(stream, ' ');

        if (value == "ITEMS_COUNT")
            break;

        // todo: find proper item name?
        result.defines.push_back(value);
        result.names.push_back(value);
    }

    return result;
}

inline NameDataList parseNatureData(const std::filesystem::path& projectFilePath)
{
    NameDataList result{};
    std::ifstream stream{projectFilePath / "src/data/text/nature_names.h"};

    // Pass 1: get all the name pointers
    std::unordered_map<std::string, std::string> namePointerLookup{};

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
        std::string define = TextParser::readToExclusive(stream, ']');
        TextParser::tryFind(stream, 's');
        std::string pointerId = TextParser::readToExclusive(stream, ',');

        // Try to resolve the pointer reference.
        if (namePointerLookup.contains(pointerId))
        {
            result.names.push_back(namePointerLookup[pointerId]);
            result.defines.push_back(define);
        }
    }

    return result;
}



inline void drawMonData(TrainerMonData& data)
{
    ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.25f);
    static bool isInitialized {false};
    static ImGuiUtils::FilteredCombo* trainerItemCombo1 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo2 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo3 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo4 {};

    if (ImGui::Button("load data"))
    {
        isInitialized = true;
        std::filesystem::path projectRoot{R"(\\wsl.localhost\Debian\home\poetahto\projects\islandgame2)"};

        s_data.moves = parseNameData(projectRoot / "src/data/text/move_names.h", "gMoveNames");
        s_data.species = parseNameData(projectRoot / "src/data/text/species_names.h", "gSpeciesNames");
        s_data.abilities = parseNameData(projectRoot / "src/data/text/abilities.h", "gAbilityNames");
        s_data.natures = parseNatureData(projectRoot);
        s_data.items = parseItemData(projectRoot);

        trainerItemCombo1 = new ImGuiUtils::FilteredCombo {s_data.items.names};
        trainerItemCombo2 = new ImGuiUtils::FilteredCombo {s_data.items.names};
        trainerItemCombo3 = new ImGuiUtils::FilteredCombo {s_data.items.names};
        trainerItemCombo4 = new ImGuiUtils::FilteredCombo {s_data.items.names};

        initializeEmeraldExpansionProperties(data);
    }

    if (isInitialized)
    {
        if (ImGui::Button("generate command"))
            SDL_SetClipboardText(data.generateData().c_str());

        ImGui::SeparatorText("Trainer");
        static int s_selectedItem {};
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4);

        trainerItemCombo1->draw("##item1", s_selectedItem); ImGui::SameLine();
        trainerItemCombo2->draw("##item2", s_selectedItem); ImGui::SameLine();
        trainerItemCombo3->draw("##item3", s_selectedItem); ImGui::SameLine();
        trainerItemCombo4->draw("##item4", s_selectedItem);

        ImGui::PopItemWidth();
        static int s_trainerClass {};
        ImGui::Combo("Trainer Class", &s_trainerClass, "TRAINER_CLASS_PKMN_TRAINER_1\0TRAINER\0COOLGUY");
        static int s_encounterMusic {};
        ImGui::Combo("Encounter Music", &s_encounterMusic, "TRAINER_ENCOUNTER_MUSIC_HIKER\0HIKER");
        static int s_picture {};
        ImGui::Combo("Picture", &s_picture, "TRAINER_PIC_COOLTRAINER_M");
        static std::string s_name{};
        ImGui::InputText("Name", &s_name);
        static bool s_doubleBattle{};
        ImGui::Checkbox("Double battle", &s_doubleBattle);

        ImGui::SeparatorText("Flags");
        static bool s_flag {};
        ImGui::Checkbox("Check Bad Move", &s_flag);
        // todo: dynamically get all flags

        static int s_partyIndex{};
        static const char* s_partyMembers[] { "Bulbasaur", "Venusaur", "Charizard", "Volcarona"};
        ImGui::SeparatorText("Trainer Party");
        ImGui::SetNextItemWidth(150);
        ImGui::ListBox("##Party", &s_partyIndex, s_partyMembers, IM_ARRAYSIZE(s_partyMembers)); // todo: add members into list
        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("Mon Editor", ImVec2(0, 350), true);
        data.draw();
        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::PopItemWidth();
    }
}
