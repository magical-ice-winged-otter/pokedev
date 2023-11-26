#ifndef POKEDEV_TRAINER_EDITOR_HPP
#define POKEDEV_TRAINER_EDITOR_HPP

#include "game_data.hpp"
#include "mons/mon_data.hpp"
#include "mons/mon_editor.hpp"

struct TrainerData {
    size_t encounterMusicIndex {};
    size_t classIndex {};
    size_t pictureIndex {};
    size_t itemIndex[4] {};
    std::string name {};
    bool doubleBattle {};
    MonData partyMembers[6] {};
    int partyMemberCount {1};
    std::vector<size_t> aiFlagIndices {};
};

class TrainerEditor {
public:
    void init(GameData& gameData);
    void setDataToEdit(TrainerData* data);
    void draw();

private:
    GameData* m_gameData {};
    MonEditor m_monEditor {};
    TrainerData* m_dataToEdit {};
    size_t m_editedPartyMemberIndex {};
};


#endif //POKEDEV_TRAINER_EDITOR_HPP
