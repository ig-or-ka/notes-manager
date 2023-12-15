//модуль базы данных

using Microsoft.Data.Sqlite;

class NoteDataBase
{
    readonly SqliteConnection database;

    public NoteDataBase(string dbName){
        database = new SqliteConnection($"Data Source={dbName}");
        database.Open();
        CreateTables();
    }

    void CreateTables(){
        //создание таблицы заметок
        var sqlCmd = "CREATE TABLE IF NOT EXISTS notes (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," +
            "title TEXT NOT NULL," +
            "text TEXT NOT NULL);";

        Execute(sqlCmd);
    }

    public SqliteDataReader? Execute(string sqlCmd, List<SqliteParameter>? param=null, bool reader=false){
        var command = database.CreateCommand();
        command.CommandText = sqlCmd;

        if(param != null){
            //добавление параметров запроса
            command.Parameters.AddRange(param);
        }

        if(reader){
            return command.ExecuteReader();
        }
        else{
            command.ExecuteNonQuery();
        }        

        return null;
    }

    public void Close(){
        database.Close();
    }
}
