//модуль заметок

using Microsoft.Data.Sqlite;


class NoteManager{
    private readonly NoteDataBase database;
    private Dictionary<int, Note> notes = new();


    public NoteManager(string dbName){
        database = new NoteDataBase(dbName);
        LoadNotes();
    }


    private void LoadNotes(){
        notes = new();

        //считываем все заметки из БД и сохраняем их в индекс
        using var reader = database.Execute("select * from notes", reader: true)!;
        if (reader.HasRows) // если есть данные
        {
            while (reader.Read())   // построчно считываем данные
            {
                int noteId = reader.GetInt32(0);

                var note  = new Note(
                    this,
                    noteId,
                    reader.GetString(1),
                    reader.GetString(2)
                );

                //добавляем объект заметки в общий индекс заметок
                notes[noteId] = note;
            }
        }
    }


    public Note? CreateNote(string title, string text){
        //записываем заметку в БД
        var sqlCmd = "insert into notes (title, text) values (@title,@text)";
        var param = new List<SqliteParameter>
        {
            new("@title", title),
            new("@text", text)
        };
        
        database.Execute(sqlCmd, param);

        //считываем ID новой заметки из БД
        sqlCmd = "select seq from sqlite_sequence where name='notes'";

        using var reader = database.Execute(sqlCmd, reader: true)!;
        if (reader.HasRows)
        {
            reader.Read();
            int noteId = reader.GetInt32(0);

            var note = new Note(
                this,
                noteId,
                title,
                text
            );

            //добавляем объект заметки в общий индекс заметок
            notes[noteId] = note;

            return note;
        }

        return null;
    }


    public bool RemoveNoteByID(int noteID){
        if(notes.ContainsKey(noteID)){

            //удаляем заметку из БД
            var sqlCmd = "delete from notes where id = @noteId";
            var param = new List<SqliteParameter>
            {
                new("@noteId", noteID)
            };

            database.Execute(sqlCmd, param);

            //удаляем объект заметки из индекса
            notes.Remove(noteID);
            return true;
        }

        return false;
    }


    public List<Note> GetAllNotes() {
        return new(notes.Values);
    }


    public Note? GetNoteByID(int noteID){
        if(notes.ContainsKey(noteID)){
            return notes[noteID];
        }

        return null;
    }


    //метод поиска заметки по ключевому слову
    public List<Note> SearchNotes(bool byTitle, string keyword){
        keyword = keyword.ToLower();
        var result = new List<Note>();

        foreach(var note in notes.Values){
            string text;

            if(byTitle){
                text = note.Title;
            }
            else{
                text = note.Text;
            }

            //в случае, если заголовок или текст заметки содержат ключевое слово, добавляем заметку
            if(text.ToLower().Contains(keyword)){
                result.Add(note);
            }
        }

        return result;
    }


    public void Close(){
        database.Close();
    }
}


class Note{
    public readonly string Title;
    public readonly string Text;
    public readonly int NoteId;
    public readonly NoteManager Manager;


    public Note(NoteManager manager, int noteId, string title, string text){
        Manager = manager;
        Title = title;
        Text = text;
        NoteId = noteId;
    }


    public bool Remove(){
        return Manager.RemoveNoteByID(NoteId);
    }
}
