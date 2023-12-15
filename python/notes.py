"""
Notes module
"""

import sqlite3


class Note:
    """Class of note"""

    def __init__(self, manager, note_id: int, title: str, text: str) -> None:
        self.manager = manager
        self.note_id = note_id
        self.title = title
        self.text = text

    def remove(self) -> bool:
        """Remove note from index and DB"""

        return self.manager.remove_note_by_id(self.note_id)


class NotesManager:
    """Notes Manager class"""

    def __init__(self, db_name: str) -> None:
        # подключение к базе данных
        self.database = sqlite3.connect(db_name)
        self.notes = {}

        self.create_tables()
        self.load_notes()

    def create_tables(self) -> None:
        """Create tables for notes"""

        self.database.execute("""CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
            title TEXT NOT NULL,
            text TEXT NOT NULL
        );""")
        self.database.commit()

    def load_notes(self) -> None:
        """Read all notes from DB"""

        self.notes = {}

        # считываем все заметки из БД и сохраняем их в индекс
        cur = self.database.execute("select * from notes")

        for note in cur.fetchall():
            self.notes[note[0]] = Note(self, note[0], note[1], note[2])

    def create_note(self, title: str, text: str) -> Note:
        """Create note by title and text"""

        # записываем заметку в БД
        sql_cmd = "insert into notes (title, text) values (?,?)"
        self.database.execute(sql_cmd, (title, text))
        self.database.commit()

        # считываем ID новой заметки из БД
        sql_cmd = "select seq from sqlite_sequence where name='notes'"
        cur = self.database.execute(sql_cmd)
        note_id = cur.fetchone()[0]

        # добавляем объект заметки в общий индекс заметок
        note = Note(self, note_id, title, text)
        self.notes[note_id] = note

        return note

    def remove_note_by_id(self, note_id: int) -> bool:
        """Remove note from index and DB by note ID"""

        if note_id in self.notes:
            # удаляем заметку из БД
            self.database.execute("delete from notes where id = ?", (note_id,))
            self.database.commit()

            # удаляем объект заметки из индекса
            del self.notes[note_id]

            return True

        return False

    def get_all_notes(self) -> list[Note]:
        """Get all notes from index"""

        return list(self.notes.values())

    def get_note_by_id(self, note_id: int) -> Note:
        """Get note from index by ID"""

        if note_id in self.notes:
            return self.notes[note_id]

        return None

    def search_note(self, by_title: bool, keyword: str) -> list[Note]:
        """Search note by keyword"""

        keyword = keyword.lower()
        result = []

        for note in self.notes.values():
            if by_title:
                text = note.title

            else:
                text = note.text

            # в случае, если заголовок или текст заметки содержат
            # ключевое слово, добавляем заметку
            if keyword in text.lower():
                result.append(note)

        return result

    def close(self):
        """Close DB connection"""

        self.database.close()
