import game_logic

print("| 100 спичек |")
print("1. Описание игры")
print("2. Игра")
print("3. Выход")

while True:
    selected_option = int(input())

    if selected_option == 1:
        game_logic.print_description()
    elif selected_option == 2:
        game_logic.play_game()
    elif selected_option == 3:
        break
    else:
        print("Выберите корректный пункт меню")
