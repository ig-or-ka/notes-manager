#include "notes.h"
#include <vector>
#include <iostream>
#include <cctype>
#include <algorithm>

using namespace std;


//функция преобразования строки к нижнему регистру
string str_lower(const string& str){
    string res;

    for (int i = 0; i < str.length(); i++) {
        res += (char)tolower(str[i]);
    }

    return res;
}


//NoteManager
NoteManager::NoteManager(const std::string& db_name){
    database = new NoteDataBase(db_name);
    load_notes();
}


NoteManager::~NoteManager(){
    delete database;

    for(auto note : notes){
        delete note.second;
    }
}


Note* NoteManager::create_note(const std::string& title, const std::string& text){
    //записываем заметку в БД
    auto sql_cmd = "insert into notes (title, text) values (?,?)";
    vector<string> params{
        title, text
    };

    database->execute(sql_cmd, &params);

    //считываем ID новой заметки из БД
    sql_cmd = "select seq from sqlite_sequence where name='notes'";
    vector<vector<string>> result;
    database->execute(sql_cmd, nullopt, &result);

    int note_id = stoi(result[0][0]);
    //добавляем объект заметки в общий индекс заметок
    notes[note_id] = new Note(this, note_id, title, text);

    return notes[note_id];
}


bool NoteManager::remove_note_by_id(int note_id){
    if(notes.count(note_id)){
        //удаляем заметку из БД
        auto sql_cmd = "delete from notes where id = ?";
        vector<string> params{
            to_string(note_id)
        };
        database->execute(sql_cmd, &params);

        //удалем объект заметки
        delete notes[note_id];
        //удаляем ссылку на объект заметки из индекса
        notes.erase(note_id);

        return true;
    }

    return false;
}


void NoteManager::load_notes(){
    notes.clear();

    //считываем все заметки из БД и сохраняем их в индекс
    auto sql_cmd = "select * from notes";
    vector<vector<string>> result;
    database->execute(sql_cmd, nullopt, &result);

    for(auto row : result){
        notes[stoi(row[0])] = new Note(
            this,
            stoi(row[0]),
            row[1],
            row[2]
        );
    }
}


optional<Note*> NoteManager::get_note_by_id(int note_id) const {
    if(notes.count(note_id)){
        return notes.at(note_id);
    }

    return nullopt;
}


void NoteManager::get_all_notes(std::vector<Note*>& notes_result) const{
    notes_result.clear();

    transform(
        notes.begin(), 
        notes.end(), 
        std::back_inserter(notes_result),
        [](const auto &kv){ return kv.second;}
    );
}


void NoteManager::search_note(bool by_title, const std::string& keyword, std::vector<Note*>& result) const{
    //приводим ключевые слова к нижнему регистру
    string key = str_lower(keyword);

    result.clear();

    for(auto note : notes){
        string text;

        if(by_title){
            text = str_lower(note.second->get_title());
        }

        else{
            text = str_lower(note.second->get_text());
        }

        //в случае, если заголовок или текст заметки не содержат ключевое слово, пропускаем заметку
        if(text.find(key) == string::npos){
            continue;
        }
        
        result.push_back(note.second);
    }
}


//Note
Note::Note(NoteManager* note_manager, int note_id, const string& title, const string& text) 
    : title(title), text(text), note_id(note_id), note_manager(note_manager){}


bool Note::remove(){
    if(note_manager){
        return note_manager->remove_note_by_id(note_id);
    }

    return false;
}
