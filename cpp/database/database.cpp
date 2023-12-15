#include <iostream>
#include "database.h"

using namespace std;


NoteDataBase::NoteDataBase(const std::string& db_name){
    int exit_code = sqlite3_open(db_name.c_str(), &database); //открываем подключение к БД

    //в случае ошибки, прерываем исполнение программы
    if (exit_code) {
        string msg = "Error open DB: ";
        msg += sqlite3_errmsg(database);

        throw NoteDataBaseException(msg);
    }

    create_tables(); //создание таблицы заметок
}


NoteDataBase::~NoteDataBase(){
    close();
}


void execute_exception(sqlite3_stmt* stmt, sqlite3* database){
    string msg = "Error execute: ";
    msg += sqlite3_errmsg(database);

    //освобождаем ресурсы
    sqlite3_finalize(stmt);
    
    throw NoteDataBaseException(msg);
}


void NoteDataBase::execute(const string& sql_cmd, optional<const vector<string>*> params, optional<vector<vector<string>>*> result){
    sqlite3_stmt* stmt = 0;
    
    int result_code = sqlite3_prepare_v2(database, sql_cmd.c_str(), -1, &stmt, 0);

    if(result_code == SQLITE_OK){
        if(params){
            //добавляем параметры к запросу
            for(int i = 0; i < (**params).size(); i++){
                sqlite3_bind_text(stmt, i+1, (**params)[i].c_str(), -1, SQLITE_STATIC);
            }
        }            

        if(result){
            //считываем построчно результаты выполнения запроса
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                vector<string> row;

                for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) { 
                    auto val = sqlite3_column_text(stmt, colIndex);
                    row.push_back((const char*)val);
                }

                (**result).push_back(row);
            }
        }

        else{
            //если запрос не требует считывания результата, делаем проверку на ошибку
            result_code = sqlite3_step(stmt);

            if(result_code != SQLITE_DONE){
                execute_exception(stmt, database);
            }
        }
    }

    else{
        execute_exception(stmt, database);
    }

    //освобождаем ресурсы
    sqlite3_finalize(stmt);
}


void NoteDataBase::close(){
    if(!closed){
        sqlite3_close(database);
        closed = true;
    }    
}


void NoteDataBase::create_tables(){
    string sql_cmd = "CREATE TABLE IF NOT EXISTS notes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "title TEXT NOT NULL,"
        "text TEXT NOT NULL);";

    char* messaggeError; 
    int exit_code = sqlite3_exec(database, sql_cmd.c_str(), NULL, 0, &messaggeError);

    if (exit_code != SQLITE_OK){
        //в случае ошибки создания таблицы, освобождаем ресурсы и выдаем исключение
        string msg = "Error Create Table: ";
        msg += messaggeError;

        sqlite3_free(messaggeError);
        sqlite3_close(database);

        throw NoteDataBaseException(msg);
    }
}
