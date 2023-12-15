#pragma once
//модуль заметок

#include <string>
#include <map>
#include <vector>
#include "database.h"


class Note;

class NoteManager{
private:
    std::map<int, Note*> notes;
    NoteDataBase* database = nullptr;

    void load_notes();

public:
    explicit NoteManager(const std::string& db_name);
    NoteManager(const NoteManager&) = delete;
    NoteManager(NoteManager&& moved) = delete;
    NoteManager& operator=(const NoteManager&) = delete;
    NoteManager& operator=(NoteManager&& moved) = delete;
    ~NoteManager();

    Note* create_note(const std::string& title, const std::string& text);
    bool remove_note_by_id(int note_id);
    void get_all_notes(std::vector<Note*>& notes) const;
    std::optional<Note*> get_note_by_id(int note_id) const;
    void search_note(bool by_title, const std::string& keyword, std::vector<Note*>& notes) const;
};


class Note{
private:
    std::string title;
    std::string text;
    int note_id;  
    NoteManager* note_manager = nullptr;

public:
    Note(NoteManager* note_manager, int note_id, const std::string& title, const std::string& text);
    bool remove();

    const int get_note_id() const { return note_id; }
    const std::string& get_title() const { return title; }
    const std::string& get_text() const { return text; }
};
