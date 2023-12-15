NoteManager manager = new("notes.sqlite");


//обработчик события ctrl+c
void Exit(object? _, ConsoleCancelEventArgs? e){
    manager.Close();
    Console.WriteLine("Goodbye!");
}


//регистрация обработчика события CTRL+C
Console.CancelKeyPress += new ConsoleCancelEventHandler(Exit);


void CreateNote(){
    //считываем название заметки
    Console.Write("Enter the name of the note: ");
    var title = Console.ReadLine()!;
    var text = "";

    //построчно считываем содержание заметки
    Console.WriteLine("Enter the text of the note (enter 'end' to done): ");

    while(true){
        Console.Write("> ");
        var text_readed = Console.ReadLine();

        //при вводе end прекращаем считывание
        if(text_readed == "end" || text_readed == "'end'"){
            break;
        }

        text += text_readed + '\n';
    }

    //на основе полученных данных, создаем заметку
    manager.CreateNote(title, text);
}


void ShowNotes(){
    var notes = manager.GetAllNotes();

    if(notes.Count > 0){
        Console.WriteLine("======================");

        //выводим список всех заметок
        foreach(var note in notes){
            Console.WriteLine($"{note.NoteId}) {note.Title}");
        }

        Console.WriteLine("======================");
    }

    else{
        Console.WriteLine("There is not a single note!");
    }
}


//функция получения объекта заметки по ее ID
Note? GetNote(List<string> CmdSegs, bool show=true){
    //проводим валидацию введенной пользователем команды
    if(CmdSegs.Count == 2){
        if(int.TryParse(CmdSegs[1], out var noteId)){
            var note = manager.GetNoteByID(noteId);

            if(note != null){
                return note;
            }

            else{
                Console.WriteLine("Note not found");
            }
        }

        else{
            Console.WriteLine("Note id must be integer");
        }
    }

    else{
        if(show){
            Console.WriteLine("Use: show <note id>");
        }
        else{
            Console.WriteLine("Use: remove <note id>");
        }
    }

    return null;
}


//функция обработки команды поиска заметки
void SearchNoteCmd(List<string> cmdSegs){
    //проводим валидацию введенной пользователем команды
    if(cmdSegs.Count == 3){
        if(cmdSegs[1] == "title" || cmdSegs[1] == "text"){
            var byTitle = cmdSegs[1] == "title";
            var result = manager.SearchNotes(byTitle, cmdSegs[2]);

            if(result.Count > 0){
                foreach(var note in result){
                    Console.WriteLine($"{note.NoteId}) {note.Title}");
                }
            }
            
            else{
                Console.WriteLine("Nothing found");
            }
        }

        else{
            Console.WriteLine($"The purpose of the search must be title or text, not {cmdSegs[1]}");
        }
    }

    else{
        Console.WriteLine("Use: search [title | text] <keyword>");
    }
}


while (true){
    Console.Write(">>> ");
    var cmd = Console.ReadLine();
    if(cmd == null)
        break;

    var cmdAllSegs = cmd!.Split(' ');
    var cmdSegs = new List<string>();
    
    foreach(var seg in cmdAllSegs){
        //если сегментов команды больше чем 3, объединяем последние сегменты в третий,
        //чтобы осуществить возможность поиска по ключевой фразе
        if(cmdSegs.Count == 3){
            cmdSegs[2] += $" {seg}";
        }
        else if (seg != ""){
            cmdSegs.Add(seg);
        }
    }

    //обработка команд
    switch(cmdSegs[0]){
        case "help":
            Console.WriteLine("new - create a new note.");
            Console.WriteLine("list - show a list of all notes.");
            Console.WriteLine("search - note search. Use: search [title | text] <keyword>");
            Console.WriteLine("show - show a note. Use: show <note id>");
            Console.WriteLine("remove - remove a note. Use: remove <note id>");
            Console.WriteLine("exit - exiting the application");
            break;

        case "exit":
            Exit(null,null);
            return;

        case "new":
            CreateNote();
            Console.WriteLine("Note created!");
            break;

        case "list":
            ShowNotes();
            break;

        case "show":
        {
            var note = GetNote(cmdSegs);

            if(note != null){
                Console.WriteLine(note.Title);
                Console.WriteLine("================");
                Console.WriteLine(note.Text);
            }
            break;
        }

        case "remove":
        {
            var note = GetNote(cmdSegs, false);

            if(note != null){
                note.Remove();
                Console.WriteLine("Note removed!");
            }
            break;
        }

        case "search":
            SearchNoteCmd(cmdSegs);
            break;

        default:
            Console.WriteLine("Unknown command, use the \"help\"");
            break;
    }
}
