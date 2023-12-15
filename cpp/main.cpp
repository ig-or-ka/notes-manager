#include <iostream>
#include <sstream>
#include <csignal>
#include "notes.h"
using namespace std;


string DB_NAME = "notes.sqlite";

void create_note_cmd(NoteManager& note_manager){
    char buf[256];

    //считываем название заметки
    string title;
    cout << "Enter the name of the note: ";
    cin.getline(buf, sizeof(buf));
    title = buf;

    //построчно считываем содержание заметки
    string text_line;
    string text;
    cout << "Enter the text of the note (enter 'end' to done):" << endl;
    while (true)
    {
        cout << "> ";
        cin.getline(buf, sizeof(buf));
        text_line = buf;

        //если было введено ключевое слово end, прекращаем считывание
        if(text_line == "end" || text_line == "'end'"){
            break;
        }
        text += text_line;
        text += '\n';

    }

    //на основе полученных данных, создаем заметку
    note_manager.create_note(title, text);

    cout << "Note created!" << endl;
}


const void show_notes(const NoteManager& note_manager){
    vector<Note*> notes;
    note_manager.get_all_notes(notes);

    if(notes.size() > 0){
        cout << "======================" << endl;

        //выводим список всех заметок
        for(auto note : notes){
            cout << note->get_note_id() << ") " << note->get_title() << endl;
        }

        cout << "======================" << endl;
    }

    else{
        cout << "There is not a single note!" << endl;
    }
}


//метод получения объекта заметки по ее ID
optional<Note*> get_note(const NoteManager& note_manager, const string& id_seg, bool show=true){
    //проводим валидацию введенной пользователем команды
    if(id_seg.length() > 0){
        int note_id;

        try{
            note_id = stoi(id_seg);
        }
        catch(invalid_argument){
            cout << "Note id must be integer" << endl;
            return nullptr;
        }

        auto note = note_manager.get_note_by_id(note_id);
        if(note){
            return note;
        }

        else{
            cout << "Note not found" << endl;
        }
    }

    else{
        if(show){
            cout << "Use: show <note id>" << endl;
        }
        else{
            cout << "Use: remove <note id>" << endl;
        }        
    }

    return nullopt;
}


//метод обработки команды поиска заметки
void search_note_cmd(NoteManager& note_manager, const string& purpose, const string& keyword){
    //проводим валидацию введенной пользователем команды
    if(purpose.length() > 0 && keyword.length() > 0){
        if(purpose == "title" || purpose == "text"){
            vector<Note*> result;

            bool by_title = false;
            if(purpose == "title"){
                by_title = true;
            }

            note_manager.search_note(by_title, keyword, result);

            if(result.size() > 0){
                for(auto note : result){
                    cout << note->get_note_id() << ") " << note->get_title() << endl;
                }
            }

            else{
                cout << "Nothing found" << endl;
            }
        }

        else{
            cout << "The purpose of the search must be title or text, not " << purpose << endl;
        }
    }

    else{
        cout << "Use: search [title | text] <keyword>" << endl;
    }
}


//обработчик сигнала ctrl+c
void ctrl_c_signal(int signal){
    cout << "\nGoodbye" << endl;
    exit(0);
}


int main(){
    std::signal(SIGINT, ctrl_c_signal); //регистрация обработчика сигнала
    NoteManager note_manager(DB_NAME);

    while(true){
        if(cin.rdstate() != 3)
            cout << ">>> ";

        char buf[256];
        cin.getline(buf, sizeof(buf));

        if (buf[0] == 0)
            continue;

        stringstream sst;
        sst << buf;

        string cmd_segs[3];
        sst >> cmd_segs[0] >> cmd_segs[1];
        
        if(cmd_segs[1].length() > 0){
            sst.get();
            getline(sst, cmd_segs[2]);
        }

        //обработка команд
        if(cmd_segs[0] == "new"){
            create_note_cmd(note_manager);
        }

        else if(cmd_segs[0] == "exit"){
            cout << "Goodbye" << endl;
            break;
        }

        else if(cmd_segs[0] == "list"){
            show_notes(note_manager);
        }

        else if(cmd_segs[0] == "show"){
            auto note = get_note(note_manager, cmd_segs[1]);

            if(note){
                cout << (**note).get_title() << endl;
                cout << "================" << endl;
                cout << (**note).get_text() << endl;
            }
        }

        else if(cmd_segs[0] == "remove"){
            auto note = get_note(note_manager, cmd_segs[1], false);

            if(note){
                (**note).remove();
                cout << "Note removed!" << endl;
            }
        }

        else if(cmd_segs[0] == "search"){
            search_note_cmd(note_manager, cmd_segs[1], cmd_segs[2]);
        }

        else if(cmd_segs[0] == "help"){
            cout << "new - create a new note." << endl;
            cout << "list - show a list of all notes." << endl;
            cout << "search - note search. Use: search [title | text] <keyword>" << endl;
            cout << "show - show a note. Use: show <note id>" << endl;
            cout << "remove - remove a note. Use: remove <note id>" << endl;
            cout << "exit - exiting the application" << endl;
        }

        else{
            cout << "Unknown command, use the \"help\"" << endl;
        }
    }
    return 0;
}
