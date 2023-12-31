# notes-manager

## Описание:

Приложение представляет собой менеджер заметок, где пользователь может создавать, хранить, искать и удалять заметки. Для хранения данных будет использоваться база данных SQLite.


## Основная функциональность:

## Добавление новой заметки:
- Пользователь может добавить новую заметку, указав заголовок и содержание.
- Заметка должна быть сохранена в базе данных.

## Просмотр списка всех заметок:
- Приложение должно отображать список всех заметок с их заголовками.
- Пользователь может выбрать заметку из списка для просмотра подробной информации.

## Поиск заметки:
- Пользователь может ввести ключевое слово или фразу для поиска.
- Приложение должно отобразить список заметок, содержащих данное ключевое слово.

## Удаление заметки:
- Пользователь может выбрать заметку из списка и удалить её.
- Заметка должна быть удалена из базы данных.

## Команды
- new - Создание новой заметки
- list - Вывод списка всех заметок
- search - поиск заметки по слову или фразе
  > Use: search [title | text] <keyword>
- show - вывод содержимого заметки по ее ID
  > Use: show <note id>
- remove - удаление заметки
  > Use: remove <note id>
- exit - выход из приложения
