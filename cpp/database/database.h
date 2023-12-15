#pragma once
//модуль базы данных

#include "sqlite3.h"
#include <string>
#include <vector>
#include <optional>


class NoteDataBaseException
{
public: 
    explicit NoteDataBaseException(const std::string& message): message{message}{}
    std::string getMessage() const {return message;}
private:
    std::string message;
};


class NoteDataBase{
public:
    explicit NoteDataBase(const std::string& db_name);
    NoteDataBase(const NoteDataBase&) = delete;
    NoteDataBase(NoteDataBase&& moved) = delete;
    NoteDataBase& operator=(const NoteDataBase&) = delete;
    NoteDataBase& operator=(NoteDataBase&& moved) = delete;
    ~NoteDataBase();
    void execute( //метод исполнения SQL команд
        const std::string& sql_cmd, //SQL команда
        std::optional<const std::vector<std::string>*> params = std::nullopt, //параметры команды
        std::optional<std::vector<std::vector<std::string>>*> result = std::nullopt //контейнер для результата выполнения команды
    );
    void close();

private:
    sqlite3* database;
    void create_tables();
    bool closed = false;
};
