#include <imgui.h>
#include <imgui_stdlib.h>
#include <regex>
#include <string>
#include "imgui_utils.hpp"
#include "serializer.hpp"
#include "text_parser.hpp"

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
    std::vector<std::string> moves{};
    std::vector<std::string> species{};
    std::vector<std::string> abilities{};
    std::vector<std::string> natures{};
    std::vector<std::string> items{};
    std::vector<std::string> trainerClasses{};
    std::vector<std::string> trainerEncounterMusic{};
    std::vector<std::string> trainerPictures{};
    std::vector<std::string> trainerAiFlags{};
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
            m_moveCombo1(s_data.moves),
            m_moveCombo2(s_data.moves),
            m_moveCombo3(s_data.moves),
            m_moveCombo4(s_data.moves)
    {
    }

    std::string compileLine() override
    {
        return std::format(".moves = {{{}, {}, {}, {}}}",
                           s_data.moves[m_selectedMove1Index],
                           s_data.moves[m_selectedMove2Index],
                           s_data.moves[m_selectedMove3Index],
                           s_data.moves[m_selectedMove4Index]
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
    AbilityProperty() : m_combo{s_data.abilities}
    {
    }

    std::string compileLine() override
    {
        return std::format(".ability = {}", s_data.abilities[m_selectedIndex]);
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
    NatureProperty() : m_combo{s_data.natures}
    {
    }

    std::string compileLine() override
    {
        return std::format(".nature = TRAINER_PARTY_NATURE({})", s_data.natures[m_selectedIndex]);
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
    ItemProperty() : m_combo{s_data.items}
    {
    }

    std::string compileLine() override
    {
        return std::format(".heldItem = {}", s_data.items[m_selectedIndex]);
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

        for (const std::string& item : s_data.items)
        {
            if (filter.PassFilter(item.c_str()))
            {
                m_ballData.names.push_back(item);
                m_ballData.defines.push_back(item);
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

    TrainerMonData() : m_speciesCombo{s_data.species}
    {
    }

    std::string generateData()
    {
        std::string result{"    {\n"};
        result += std::format("        .species = {},\n        .lvl = {}",
                              s_data.species[m_selectedSpeciesIndex], m_level);

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

inline std::vector<std::string> parseDefines(const std::filesystem::path& filePath, const char* regex)
{
    std::ifstream stream {filePath};
    std::string line {};
    std::vector<std::string> results {};
    std::smatch match {};

    while (std::getline(stream, line))
    {
        bool hasDefine = line.find("#define") != std::string::npos;

        if (hasDefine && std::regex_search(line, match, std::regex(regex)))
        {
            size_t start = line.find("#define") + 8;
            size_t end = line.find(' ', start);
            results.emplace_back(line.substr(start, end - start));
        }
    }

    return results;
}

inline void drawMonData(TrainerMonData& data)
{
    ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.25f);
    static bool isInitialized {false};
    static ImGuiUtils::FilteredCombo* trainerItemCombo1 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo2 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo3 {};
    static ImGuiUtils::FilteredCombo* trainerItemCombo4 {};
    static ImGuiUtils::FilteredCombo* trainerClass {};
    static ImGuiUtils::FilteredCombo* trainerEncounterMusic {};
    static ImGuiUtils::FilteredCombo* trainerPicture {};
    static bool* trainerAiFlags {};

    if (ImGui::Button("load data"))
    {
        isInitialized = true;
        std::filesystem::path projectRoot{R"(\\wsl.localhost\Debian\home\poetahto\projects\islandgame2)"};

        s_data.abilities = parseDefines(projectRoot / "include/constants/abilities.h", ".*#define ABILITY_");
        s_data.species = parseDefines(projectRoot / "include/constants/species.h", ".*#define SPECIES_");
        s_data.moves = parseDefines(projectRoot / "include/constants/moves.h", ".*#define MOVE_");
        s_data.items = parseDefines(projectRoot / "include/constants/items.h", ".*#define ITEM_");
        s_data.natures = parseDefines(projectRoot / "include/constants/pokemon.h", ".*#define NATURE_");
        s_data.trainerClasses = parseDefines(projectRoot / "include/constants/trainers.h", ".*#define TRAINER_CLASS_");
        s_data.trainerEncounterMusic = parseDefines(projectRoot / "include/constants/trainers.h", ".*#define TRAINER_ENCOUNTER_MUSIC_");
        s_data.trainerPictures = parseDefines(projectRoot / "include/constants/trainers.h", ".*#define TRAINER_PIC_");
        s_data.trainerAiFlags = parseDefines(projectRoot / "include/constants/battle_ai.h", ".*#define AI_FLAG_.*\\(.*\\)");

        trainerItemCombo1 = new ImGuiUtils::FilteredCombo {s_data.items};
        trainerItemCombo2 = new ImGuiUtils::FilteredCombo {s_data.items};
        trainerItemCombo3 = new ImGuiUtils::FilteredCombo {s_data.items};
        trainerItemCombo4 = new ImGuiUtils::FilteredCombo {s_data.items};
        trainerClass = new ImGuiUtils::FilteredCombo {s_data.trainerClasses};
        trainerEncounterMusic = new ImGuiUtils::FilteredCombo {s_data.trainerEncounterMusic};
        trainerPicture = new ImGuiUtils::FilteredCombo {s_data.trainerPictures};
        trainerAiFlags = new bool[s_data.trainerAiFlags.size()] {false};

        initializeEmeraldExpansionProperties(data);
    }

    if (isInitialized)
    {
        if (ImGui::Button("generate command"))
            SDL_SetClipboardText(data.generateData().c_str());

        ImGui::SeparatorText("Trainer");
        static int s_selectedItem1 {};
        static int s_selectedItem2 {};
        static int s_selectedItem3 {};
        static int s_selectedItem4 {};
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4);

        trainerItemCombo1->draw("##item1", s_selectedItem1); ImGui::SameLine();
        trainerItemCombo2->draw("##item2", s_selectedItem2); ImGui::SameLine();
        trainerItemCombo3->draw("##item3", s_selectedItem3); ImGui::SameLine();
        trainerItemCombo4->draw("##item4", s_selectedItem4);

        ImGui::PopItemWidth();
        static int trainerClassIndex {};
        trainerClass->draw("Trainer Class", trainerClassIndex);
        static int trainerEncounterMusicIndex {};
        trainerEncounterMusic->draw("Encounter Music", trainerEncounterMusicIndex);
        static int trainerPictureIndex {};
        trainerPicture->draw("Picture", trainerPictureIndex);
        static std::string s_name{};
        ImGui::InputText("Name", &s_name);
        static bool s_doubleBattle{};
        ImGui::Checkbox("Double battle", &s_doubleBattle);

        if (ImGui::CollapsingHeader("Flags"))
        {
            for (int i = 0; i < s_data.trainerAiFlags.size(); ++i)
                ImGui::Checkbox(s_data.trainerAiFlags[i].c_str(), &trainerAiFlags[i]);
        }

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
