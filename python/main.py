"""
Note manager start file
"""

from notes import NotesManager, Note


def create_note_cmd(manager: NotesManager) -> None:
    """Create note"""

    # считываем название заметки
    title = input("Enter the name of the note: ")
    text = str()
    print("Enter the text of the note (ctrl+c to done):")

    # построчно считываем содержание заметки
    while True:
        try:
            text += input('> ') + '\n'

        except KeyboardInterrupt:
            # при ctrl+c прекращаем считывание
            break

    # на основе полученных данных, создаем заметку
    manager.create_note(title, text)


def show_notes(manager: NotesManager) -> None:
    """Show all notes"""

    notes = manager.get_all_notes()
    if len(notes) > 0:
        print('======================')

        # выводим список всех заметок
        for note in notes:
            print(f'{note.note_id}) {note.title}')

        print('======================')

    else:
        print('There is not a single note!')


def search_note_cmd(manager: NotesManager, cmd_segs: list[str]) -> None:
    """Search notes by cmd line"""

    # проводим валидацию введенной пользователем команды
    if len(cmd_segs) == 3:

        if cmd_segs[1] in {'title', 'text'}:

            by_title = cmd_segs[1] == 'title'
            result = manager.search_note(by_title, cmd_segs[2])

            if len(result) > 0:
                for note in result:
                    print(f'{note.note_id}) {note.title}')

            else:
                print('Nothing found')

        else:
            print(
                'The purpose of the search must be title or text, not',
                cmd_segs[1]
            )

    else:
        print('Use: search [title | text] <keyword>')


def get_note(manager: NotesManager, cmd_segs: list[str], show=True) -> Note:
    """Get note by id from cmd line"""

    # проводим валидацию введенной пользователем команды
    if len(cmd_segs) == 2:
        try:
            note_id = int(cmd_segs[1])

        except ValueError:
            print('Note id must be integer')
            return None

        note = manager.get_note_by_id(note_id)

        if note is not None:
            return note

        print('Note not found')

    else:
        if show:
            print('Use: show <note id>')

        else:
            print('Use: remove <note id>')

    return None


def help_cmd():
    """Displaying a list of all commands"""

    print('new - create a new note.')
    print('list - show a list of all notes.')
    print('search - note search. Use: search [title | text] <keyword>')
    print('show - show a note. Use: show <note id>')
    print('remove - remove a note. Use: remove <note id>')
    print('exit - exiting the application')


def main():
    """Main function"""

    manager = NotesManager("notes.sqlite")

    while True:
        try:
            cmd = input('>>> ')

        except KeyboardInterrupt:
            manager.close()
            print('Goodbye!')
            break

        cmd_all_segs = cmd.split(' ', -1)
        cmd_segs = []

        for seg in cmd_all_segs:
            # если сегментов команды больше чем 3,
            # объединяем последние сегменты в третий,
            # чтобы осуществить возможность поиска по ключевой фразе
            if len(cmd_segs) == 3:
                cmd_segs[2] += f" {seg}"

            elif seg != "":
                cmd_segs.append(seg)

        if len(cmd_segs) == 0:
            continue

        # обработка команд
        match cmd_segs[0]:
            case "help":
                help_cmd()

            case "exit":
                manager.close()
                print('Goodbye!')
                break

            case "new":
                create_note_cmd(manager)
                print('Note created!')

            case "list":
                show_notes(manager)

            case "search":
                search_note_cmd(manager, cmd_segs)

            case "show":
                note = get_note(manager, cmd_segs)

                if note:
                    print(note.title)
                    print('================')
                    print(note.text)

            case "remove":
                note = get_note(manager, cmd_segs, False)

                if note:
                    note.remove()
                    print('Note removed!')

            case _:
                print('Unknown command, use the "help"')


if __name__ == '__main__':
    main()
