#!/bin/bash

EXE="./build/hashbench"
CONFIG_DIR="./configs"

if [ ! -f "$EXE" ]; then
    echo "Ошибка: Исполняемый файл $EXE не найден. Сначала скомпилируйте проект."
    exit 1
fi

if [ ! -d "$CONFIG_DIR" ]; then
    echo "Ошибка: Папка $CONFIG_DIR не найдена."
    exit 1
fi

echo "--- Запуск ---"

for config in "$CONFIG_DIR"/*.yaml; do
    echo ""
    echo ">>> Обработка конфига: $config"
    
    $EXE "$config"
    
    if [ $? -eq 0 ]; then
        echo ">>> Готово."
    else
        echo ">>> Произошла ошибка при выполнении $config"
    fi
done

echo ""
echo "--- Все тесты завершены ---"