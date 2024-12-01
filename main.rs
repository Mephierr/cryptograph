use std::io::{self, Write};

struct UI;

impl UI {
    // Отображаем меню
    fn display_menu(&self, options: &[&str]) {
        println!("Выберите опцию:");
        for (i, option) in options.iter().enumerate() {
            println!("{}. {}", i + 1, option);
        }
        println!("0. Выход");
    }

    // Получаем ввод от пользователя
    fn get_user_input(&self, max_option: usize) -> usize {
        loop {
            print!("Введите номер опции: ");
            io::stdout().flush().unwrap(); // Обязательно очищаем буфер
            let mut input = String::new();
            io::stdin().read_line(&mut input).unwrap();

            match input.trim().parse::<usize>() {
                Ok(choice) if choice <= max_option => return choice,
                _ => println!("Неверный ввод. Пожалуйста, попробуйте снова."),
            }
        }
    }

    // Вывод сообщения
    fn show_message(&self, message: &str) {
        println!("{}", message);
    }
}

fn main() {
    let ui = UI;
    let options = [
        "Симметричное шифрование", 
        "Асимметричное шифрование", 
        "Потоковое шифрование", 
        "Блочное шифрование", 
        "Гомоморфное шифрование", 
        "Квантовое шифрование"
    ];

    loop {
        ui.display_menu(&options);
        let choice = ui.get_user_input(options.len());

        if choice == 0 {
            ui.show_message("Выход из программы.");
            break;
        }

        ui.show_message(&format!("Вы выбрали: {}", options[choice - 1]));
    }
}