//Автор: Ястребов Глеб
//Описание: Этот файл содержит реализацию системы мониторинга данных с камер видеонаблюдения 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <conio.h> 

#define MAX_CAMERAS 10
#define MAX_SNAPSHOTS 10
#define MAX_FILENAME_LENGTH 100 

int prData(const char* date);
int prTime(const char* time);
void addCamera(const char* inputData);
void generateTestData();
void loadDataFromFile(const char* filename);
long getFileSize(const char* filename);
void reloadFileIfUpdated(const char* filename, long* lastSize);
void generateSingleSnapshot();
int sortIDvozr(const void* a, const void* b);
int sortIDub(const void* a, const void* b);
int sortDatest(const void* a, const void* b);
int sortDaten(const void* a, const void* b);
void palld();
void pallda();
void monitoring();
void delallc();
void checkcamID(int id);
void delcamID(int id);
void rndmpoka();
void privsv();
void forcase1();

// меню
int main() {
    setlocale(LC_CTYPE, "RUS");
    int choice;
    privsv();

    while (1) {
        system("cls");
        printf("Добро пожаловать в меню!\n\n");
        printf("1. Загрузить данные из файла\n");
        printf("2. Добавить камеру вручную\n");
        printf("3. Генерация тестовых данных\n");
        printf("4. Вывести все данные (c выбором сортировки)\n");
        printf("5. Режим мониторинга\n");
        printf("6. Проверка данных камеры по ID\n");
        printf("7. Удалить камеру по ID\n");
        printf("8. Удалить все камеры\n");
        printf("9. Cведения о программе\n");
        printf("10.Выход\n\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: {
            system("cls");
            forcase1();
            printf("\nНажмите любую клавишу для продолжения...");
            _getch(); // ожидание нажатия клавиши
            break;
        }
        case 2: {
            char inputData[100];
            system("cls");
            printf("Введите данные камеры (формат DD.MM.YY_HH:MM:SS_ID): ");
            fgets(inputData, sizeof(inputData), stdin);
            inputData[strcspn(inputData, "\n")] = '\0';
            addCamera(inputData);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 3:
            system("cls");
            generateTestData();
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 4:
            system("cls");
            palld();
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 5:
            system("cls");
            monitoring();
            break;
        case 6: {
            system("cls");
            printf("Введите ID камеры для проверки: ");
            int checkId;
            scanf("%d", &checkId);
            checkcamID(checkId);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 7: {
            system("cls");
            printf("Введите ID камеры для удаления: ");
            int deleteId;
            scanf("%d", &deleteId);
            delcamID(deleteId);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 8:
            system("cls");
            delallc();
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 9:
            privsv();
            break;
        case 10:
            system("cls");
            rndmpoka();
            return 0;
        default:
            system("cls");
            printf("Некорректный выбор.\n");
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
    }
    return 0;
}

/*
 * Тип данных Snapshot:
 * Структура для хранения информации о снимке.
 * Содержит дату и время снимка в формате строки.
 */
typedef struct {
    char date[11];
    char time[9];
} Snapshot;

/*
 * Тип данных Camera:
 * Структура для хранения информации о камере.
 * Содержит уникальный идентификатор камеры, массив снимков, количество снимков и оригинальные данные.
 */
typedef struct {
    int id;
    Snapshot snapshots[MAX_SNAPSHOTS];
    int snapshotCount;
    char originalData[100];
} Camera;

Camera cameras[MAX_CAMERAS];                   // массив камер, в котором хранятся все данные о них
int cameraCount = 0;
char monitoringFile[MAX_FILENAME_LENGTH] = ""; // имя файла для мониторинга

/*
 * Функция prData проверяет корректность даты в формате "дд.мм.гг"
 * Входной параметр:
 *   - date: строка, представляющая дату в формате "дд.мм.гг"
 * Алгоритм работы:
 *   Функция пытается разобрать строку на три целых числа: день, месяц и год
 *   Если разбор не удался или если день находится за пределами допустимого диапазона (1-31),
 *   или месяц вне диапазона (1-12) то функция возвращает 0 (ошибка)
 *   В противном случае возвращается 1 (дата корректна)
 * Возвращаемое значение:
 *   - 1: дата корректна
 *   - 0: дата некорректна
 */
int prData(const char* date) {
    int day, month, year;
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) != 3) {
        return 0; //ошибка
    }
    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return 0; //неверная дата
    }
    return 1; //дата верна
}

/*
 * Функция prTime проверяет корректность времени в формате "чч:мм:сс"
 * Входной параметр:
 *   - time: строка, представляющая время в формате "чч:мм:сс"
 * Алгоритм работы:
 *   Функция работает аналогично предыдущей, но с поправкой на время, вместо даты
 * Возвращаемое значение:
 *   - 1: время корректно
 *   - 0: время некорректно
 */
int prTime(const char* time) {
    int hour, minute, second;
    if (sscanf(time, "%d:%d:%d", &hour, &minute, &second) != 3) {
        return 0; //ошибка
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; //неверное время
    }
    return 1; //корректное время
}

/*
 * Функция addCamera добавляет новую камеру или новый снимок для существующей камеры
 * Входной параметр:
 *   - inputData: строка, содержащая данные камеры в формате "DD.MM.YY_HH:MM:SS_ID"
 * Алгоритм работы:
 *   Функция пытается разобрать строку inputData на три компонента: дату, время и ID камеры
 *   Если данные не соответствуют ожидаемому формату, то выводится сообщение об ошибке
 *   Затем проверяется корректность даты и времени с помощью функций prData и prTime
 *   Если камера с таким ID уже существует:
 *     - Проверяется, существует ли снимок с таким же временем. Если существует, то функция завершает выполнение
 *     - Если снимок не существует и у камеры еще есть место для новых снимков, то снимок добавляется
 *   Если камера с таким ID не найдена:
 *     - Добавляется новая камера с указанным ID и снимок с данными даты и времени
 * Возвращаемое значение:
 *   - Функция не возвращает значений. Все ошибки обрабатываются внутри функции с выводом сообщений
 */
void addCamera(const char* inputData) {
    int id;
    char date[11], time[9];
    if (sscanf(inputData, "%10[^_]_%8[^_]_%d", date, time, &id) != 3) {
        printf("Ошибка: данные должны быть в формате DD.MM.YY_HH:MM:SS_ID.\n");
        return;
    }

    if (!prData(date) || !prTime(time)) {
        printf("Ошибка: некорректные дата или время.\n");
        return;
    }

    // проверка существования камеры
    for (int i = 0; i < cameraCount; i++) {
        if (cameras[i].id == id) {
            // проверка на наличие снимка
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                if (strcmp(cameras[i].snapshots[j].date, date) == 0 &&
                    strcmp(cameras[i].snapshots[j].time, time) == 0) {
                    // снимок уже существует
                    return;
                }
            }

            // добавление нового снимка
            if (cameras[i].snapshotCount < MAX_SNAPSHOTS) {
                strncpy(cameras[i].snapshots[cameras[i].snapshotCount].date, date, sizeof(date));
                strncpy(cameras[i].snapshots[cameras[i].snapshotCount].time, time, sizeof(time));
                cameras[i].snapshotCount++;
            }
            else {
                printf("Камера %d достигла максимального количества снимков.\n", id);
            }
            return;
        }
    }

    // если камера не найдена, добавляю новую
    if (cameraCount < MAX_CAMERAS) {
        cameras[cameraCount].id = id;
        strncpy(cameras[cameraCount].snapshots[0].date, date, sizeof(date));
        strncpy(cameras[cameraCount].snapshots[0].time, time, sizeof(time));
        cameras[cameraCount].snapshotCount = 1;
        strncpy(cameras[cameraCount].originalData, inputData, sizeof(cameras[cameraCount].originalData));
        cameraCount++;
    }
}

/*
 * Функция generateTestData генерирует тестовые данные для камер
 * Входной параметр: нет
 * Алгоритм работы:
 *   1. Функция запрашивает у пользователя количество строк (снимков), которые нужно сгенерировать
 *   2. Используется генератор случайных чисел для создания случайных значений:
 *      - ID камеры в диапазоне от 1 до 100
 *      - Дата в пределах возможных значений
 *      - Время в пределах допустимых значений
 *   3. Сформированная строка с данными в формате "DD.MM.YY_HH:MM:SS_ID" передается в функцию addCamera для добавления данных о камере
 *   4. Повторяет процесс указанное пользователем количество раз
 * Возвращаемое значение: нет
 */
void generateTestData() {
    int numLines;
    printf("Введите количество cтрок (снимков с разных камер), которые нужно сгенерировать: ");
    scanf("%d", &numLines);

    srand(time(NULL));  // инициализация генератора случайных чисел

    for (int i = 0; i < numLines; i++) {
        int cameraId = rand() % 100 + 1;  // генерация id камеры от 1 до 100
        int day = rand() % 31 + 1;
        int month = rand() % 12 + 1;
        int year = rand() % 100;          //генерация года в диапазоне 0-99
        int hour = rand() % 24;
        int minute = rand() % 60;
        int second = rand() % 60;

        // формируем строку с данными для камеры
        char testData[100];
        snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, cameraId);

        addCamera(testData);
    }

    printf("Тестовые данные сгенерированы.\n");
}

/*
 * Функция loadDataFromFile загружает данные о камерах из файла
 * Входной параметр:
 *   - filename: имя файла, из которого нужно загрузить данные
 * Алгоритм работы:
 *   1. Открывает файл в режиме чтения
 *   2. Если файл не удалось открыть, выводится сообщение об ошибке
 *   3. Построчно считывает данные из файла в массив line
 *   4. Каждая строка передается в функцию addCamera для добавления данных о камере
 *   5. После загрузки данных файл закрывается
 * Возвращаемое значение: нет
 */
void loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r"); //открытие файла в режиме чтения
    if (!file) {
        printf("Ошибка при открытии файла %s.\n", filename);
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; //удаление символа новой строки
        addCamera(line);
    }
    fclose(file); //закрытие файла
    printf("Данные из файла %s загружены.\n", filename);
}

/*
 * Функция getFileSize проверяет размер файла
 * Входной параметр:
 *   - filename: имя файла для определения его размера
 * Алгоритм работы:
 *   1. Открывает файл в бинарном режиме для получения размера
 *   2. Если файл не удалось открыть, возвращается -1
 *   3. Перемещает указатель в конец файла и определяет его размер с помощью ftell
 *   4. Закрывает файл и возвращает его размер
 * Возвращаемое значение:
 *   - size (long): размер файла в байтах
 *   - Если файл не удалось открыть, возвращает -1
 */
long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb"); //открытие файл в бинарном режиме
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file); //получение данных о размере файла
    fclose(file);
    return size;
}

/*
 * Функция reloadFileIfUpdated проверяет, были ли изменения в файле с момента последней проверки
 * Входные параметры:
 *   - filename: имя файла, который проверяется на изменения
 *   - lastSize: указатель на переменную, хранящую размер файла на момент последней проверки
 * Алгоритм работы:
 *   1. Получает текущий размер файла с помощью функции getFileSize
 *   2. Сравнивает текущий размер с размером файла, сохраненным в lastSize
 *   3. Если размер файла изменился, обновляет значение lastSize и выводит сообщение о наличии изменений
 *   4. Загружает новые данные из файла с помощью функции loadDataFromFile
 * Возвращаемое значение: нет
 */
void reloadFileIfUpdated(const char* filename, long* lastSize) {
    long currentSize = getFileSize(filename);
    if (currentSize != *lastSize) {
        *lastSize = currentSize;
        printf("Обнаружены изменения в файле %s. Загрузка новых данных...\n", filename);
        loadDataFromFile(filename);
    }
}

/*
 * Функция generateSingleSnapshot генерирует одну строку данных для камеры с случайными значениями,
 * а затем добавляет её в систему
 * Входные параметры: нет
 * Алгоритм работы:
 *   1. Инициализирует генератор случайных чисел с помощью функции srand
 *   2. Генерирует случайные значения для id камеры, дня, месяца, года, часа, минуты и секунды
 *   3. Формирует строку testData, которая представляет собой данные камеры в формате "ДД.ММ.ГГ_ЧЧ:ММ:СС_ID"
 *   4. Добавляет сформированные данные с помощью функции addCamera
 *   5. Выводит сообщение о том, что строка данных была успешно сгенерирована
 * Возвращаемое значение: нет
 */
void generateSingleSnapshot() {
    srand((unsigned int)time(NULL)); // инициализация генератора случайных чисел
    int id = rand() % 100 + 1;
    int day = rand() % 31 + 1;
    int month = rand() % 12 + 1;
    int year = rand() % 100;
    int hour = rand() % 24;
    int minute = rand() % 60;
    int second = rand() % 60;

    char testData[100];
    snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, id); //формирование новой строки
    addCamera(testData);
    printf("Новая строка данных сгенерирована: %s\n", testData);
}

/*
 * Функция sortIDvozr выполняет сортировку массивов камер по id в возрастающем порядке
 * Входные параметры:
 *   - a, b: указатели на элементы массива камер (типа Camera)
 * Алгоритм работы:
 *   1. Преобразует указатели a и b в указатели на структуры Camera
 *   2. Возвращает разницу между id двух камер, что позволяет qsort отсортировать массив в возрастающем порядке
 * Возвращаемое значение:
 *   - Разница между значениями id двух камер
 */
int sortIDvozr(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;
    return cameraA->id - cameraB->id;
}

/*
 * Функция sortIDub выполняет сортировку массивов камер id в убывающем порядке
 * Входные параметры:
 *   - a, b: указатели на элементы массива камер (типа Camera).
 * Алгоритм работы:
 *   Функция работает аналогично предыдущей, но на этот раз берется разница B и A
 * Возвращаемое значение:
 *   - Разница между значениями id двух камер
 */
int sortIDub(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;
    return cameraB->id - cameraA->id;
}

/*
 * Функция sortDatest выполняет сортировку снимков камер по дате (сначала старые)
 * Входные параметры:
 *   - a, b: указатели на элементы массива камер
 * Алгоритм работы:
 *   1. Функция перебирает все снимки каждой из камер
 *   2. Для каждого снимка разбивает строку даты на день, месяц и год с помощью функции sscanf
 *   3. Сравнивает годы, месяцы и дни. Если все эти параметры совпадают, переходит к сравнению времени
 *   4. Для времени разбивает строку времени на часы, минуты и секунды, и выполняет сравнение
 * Возвращаемое значение:
 *   - Отрицательное значение, если дата или время первого снимка раньше второго (для сортировки по возрастанию)
 *   - Положительное значение, если дата или время первого снимка позже второго
 *   - 0, если даты и время одинаковые
 */
int sortDatest(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;

    // проходим по всем снимкам камеры и сравниваем их
    for (int i = 0; i < cameraA->snapshotCount; i++) {
        for (int j = 0; j < cameraB->snapshotCount; j++) {
            // разбиваю дату на день месяц год
            int dayA, monthA, yearA;
            int dayB, monthB, yearB;
            sscanf(cameraA->snapshots[i].date, "%d.%d.%d", &dayA, &monthA, &yearA);
            sscanf(cameraB->snapshots[j].date, "%d.%d.%d", &dayB, &monthB, &yearB);

            // сравниваем по году
            if (yearA != yearB) {
                return yearA - yearB;
            }

            // если годы одинаковые, сравниваем по месяцу
            if (monthA != monthB) {
                return monthA - monthB;
            }

            // если месяцы одинаковые, сравниваем по дню
            if (dayA != dayB) {
                return dayA - dayB;
            }

            // если дни одинаковые, сравниваем по времени
            int hourA, minuteA, secondA;
            int hourB, minuteB, secondB;
            sscanf(cameraA->snapshots[i].time, "%d:%d:%d", &hourA, &minuteA, &secondA);
            sscanf(cameraB->snapshots[j].time, "%d:%d:%d", &hourB, &minuteB, &secondB);

            // сравниваем по времени
            if (hourA != hourB) {
                return hourA - hourB;
            }
            if (minuteA != minuteB) {
                return minuteA - minuteB;
            }
            return secondA - secondB;
        }
    }

    return 0;
}

/*
 * Функция sortDaten выполняет сортировку снимков камер по дате (сначала новые)
 * Входные параметры:
 *   - a, b: указатели на элементы массива камер (типа Camera)
 * Алгоритм работы:
 *   Функция работает аналогично предыдущей, с заменой местами положительного и отрицательного значения
 * Возвращаемое значение:
 *   - Положительное значение, если дата или время первого снимка позже второго (для сортировки по убыванию)
 *   - Отрицательное значение, если дата или время первого снимка раньше второго
 *   - 0, если даты и время одинаковые
 */
int sortDaten(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;

    for (int i = 0; i < cameraA->snapshotCount; i++) {
        for (int j = 0; j < cameraB->snapshotCount; j++) {
            int dayA, monthA, yearA;
            int dayB, monthB, yearB;
            sscanf(cameraA->snapshots[i].date, "%d.%d.%d", &dayA, &monthA, &yearA);
            sscanf(cameraB->snapshots[j].date, "%d.%d.%d", &dayB, &monthB, &yearB);

            if (yearA != yearB) {
                return yearB - yearA;
            }

            if (monthA != monthB) {
                return monthB - monthA;
            }

            if (dayA != dayB) {
                return dayB - dayA;
            }

            int hourA, minuteA, secondA;
            int hourB, minuteB, secondB;
            sscanf(cameraA->snapshots[i].time, "%d:%d:%d", &hourA, &minuteA, &secondA);
            sscanf(cameraB->snapshots[j].time, "%d:%d:%d", &hourB, &minuteB, &secondB);

            if (hourA != hourB) {
                return hourB - hourA;
            }
            if (minuteA != minuteB) {
                return minuteB - minuteA;
            }
            return secondB - secondA;
        }
    }

    return 0;
}

/*
 * Функция palld выводит данные о камерах и их снимках с возможностью сортировки
 * Алгоритм работы:
 *   1. Проверяет есть ли данные о камерах. Если данных нет, выводит сообщение об ошибке
 *   2. Предлагает пользователю выбрать порядок сортировки:
 *   3. В зависимости от выбора пользователя выполняется соответствующая сортировка:
 *      - Для сортировки по ID используется функции sortIDvozr (возрастание) или sortIDub (убывание)
 *      - Для сортировки по дате снимка используются функции sortDatest (старые сначала) и sortDaten (новые сначала)
 *   4. После выполнения сортировки выводит все данные о камерах и их снимках в выбранном порядке
 *   5. Если пользователь выбирает неверный вариант сортировки, данные выводятся без изменений (как загружено)
 * Возвращаемое значение: нет
 */
void palld() {
    if (cameraCount == 0) {
        printf("Нет данных о камерах.\n");
        return;
    }

    int sortOption;
    printf("\nВыберите порядок сортировки:\n");
    printf("1. По умолчанию (без сортировки, как загружено)\n");
    printf("2. По ID (возрастание)\n");
    printf("3. По ID (убывание)\n");
    printf("4. По дате снимка (старые сначала)\n");
    printf("5. По дате снимка (новые сначала)\n");
    printf("Ваш выбор: ");
    scanf("%d", &sortOption);

    // данные выводятся как загружены (без сортировки)
    if (sortOption == 1) {
        printf("\nВсе данные (по умолчанию, как загружено):\n");
        for (int i = 0; i < cameraCount; i++) {
            printf("Камера %d:\n", cameras[i].id);
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                printf("Дата: %s, Время: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
            }
            printf("\n");
        }
        return;
    }

    // сортировка в зависимости от выбора
    switch (sortOption) {
    case 2:
        qsort(cameras, cameraCount, sizeof(Camera), sortIDvozr);
        break;
    case 3:
        qsort(cameras, cameraCount, sizeof(Camera), sortIDub);
        break;
    case 4:
        qsort(cameras, cameraCount, sizeof(Camera), sortDatest);
        break;
    case 5:
        qsort(cameras, cameraCount, sizeof(Camera), sortDaten);
        break;
    default:
        printf("Неверный выбор сортировки. Используется сортировка по умолчанию (без сортировки).\n");
        break;
    }

    // вывод данных после сортировки
    printf("\nВсе данные:\n");
    for (int i = 0; i < cameraCount; i++) {
        printf("Камера %d:\n", cameras[i].id);
        for (int j = 0; j < cameras[i].snapshotCount; j++) {
            printf("Дата: %s, Время: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * Функция pallda выводит все данные о камерах и их снимках
 * Если нет данных о камерах, выводится сообщение об этом
 * Возвращаемое значение: нет
 */
void pallda() {
    if (cameraCount == 0) {
        printf("Нет данных о камерах.\n");
        return;
    }
    printf("Все данные:\n");
    for (int i = 0; i < cameraCount; i++) {
        printf("Камера %d:\n", cameras[i].id);
        for (int j = 0; j < cameras[i].snapshotCount; j++) {
            printf("Дата: %s, Время: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * Функция monitoring реализует режим мониторинга, в котором:
 * 1. Проверяется наличие файла для мониторинга и загружаются данные
 * 2. Каждые 5 секунд проверяется, изменился ли размер файла
 *    - Если файл был изменен, происходит перезагрузка данных
 * 3. Каждые 20 секунд генерируется новая запись (снимок)
 * 4. Для выхода из режима мониторинга достаточно нажать любую клавишу
 *
 * Входных параметров нет, но используются глобальные переменные:
 *  - monitoringFile: глобальная переменная, содержащая имя файла для мониторинга
 *  - cameraCount: глобальная переменная, содержащая количество камер
 *
 * Алгоритм работы:
 * 1. Функция проверяет, был ли загружен файл для мониторинга, и выводит сообщение об ошибке, если файл не выбран
 * 2. Инициализируется переменная lastSize для отслеживания изменений в файле
 * 3. После этого экран очищается и выводится сообщение о том, что режим мониторинга активен
 * 4. Данные из файла загружаются и выводятся на экран
 * 5. Функция входит в бесконечный цикл, в котором:
 *    - Каждую секунду увеличиваются два счетчика (elapsed5 и elapsed20), которые отслеживают интервалы времени
 *    - Каждый 5 секунд проверяется размер файла:
 *      - Если размер файла изменился (что свидетельствует о добавлении новых данных), происходит перезагрузка данных из файла
 *      - После перезагрузки данных выводятся актуальные данные на экран
 *    - Каждые 20 секунд генерируется новый снимок (новая строка данных)
 *      - После генерации данных экран очищается и выводятся актуальные данные
 * 6. Если пользователь нажимает любую клавишу, происходит выход из режима мониторинга и возврат в меню
 *
 * Возвращаемое значение: нет
 */
void monitoring() {
    if (strlen(monitoringFile) == 0) {
        printf("Файл для мониторинга не загружен. Сначала загрузите файл через меню.\n");
        printf("Нажмите любую клавишу для возврата в меню...\n");
        _getch();
        return;
    }

    long lastSize = getFileSize(monitoringFile);
    system("cls");
    printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");

    // загружаем и выводим все данные
    loadDataFromFile(monitoringFile);
    pallda();

    int elapsed5 = 0;   // таймер для 5 секунд (проверка обновлений)
    int elapsed20 = 0;  // таймер для 20 секунд (генерация новой строки)

    while (1) {
        Sleep(1000);
        elapsed5 += 1;
        elapsed20 += 1;

        if (_kbhit()) {  // проверка нажатия клавиши
            _getch();    // считываем символ
            system("cls");
            printf("Выход из режима мониторинга.\n");
            break;
        }

        if (elapsed5 == 5) {  // каждые 5 секунд проверяем файл
            elapsed5 = 0;
            long currentSize = getFileSize(monitoringFile);

            // если размер файла изменился или перезагрузка данных обязательна
            if (currentSize != lastSize) {
                lastSize = currentSize;

                // полностью перезагружаем данные из файла
                cameraCount = 0;
                loadDataFromFile(monitoringFile);

                // очистка консоли и вывод актуальных данных
                system("cls");
                printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");
                pallda();
            }
        }

        if (elapsed20 == 20) {  // каждые 20 секунд генерируем новую запись
            elapsed20 = 0;
            generateSingleSnapshot();

            // после генерации данных очищаем экран и выводим актуальное
            system("cls");
            printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");
            pallda();
        }
    }
}


//Функция delallc используется для удаления всех камер
void delallc() {
    cameraCount = 0;
    printf("Все камеры удалены.\n");
}

/*
 * Функция checkcamID осуществляет поиск и вывод данных для камеры по заданному ID
 * Входные параметры:
 *  - id: целое число (ID камеры), для которой нужно найти и вывести данные
 * Алгоритм работы:
 * 1. Функция принимает ID камеры, для которой необходимо найти и вывести данные
 * 2. Функция ищет камеру с заданным ID в массиве cameras
 * 3. Если камера найдена, выводятся все снимки (дату и время) для этой камеры
 * 4. Если камера с таким ID не найдена, выводится сообщение о том, что камера с данным ID отсутствует
 * Возвращаемое значение: нет
 */
void checkcamID(int id) {
    int found = 0;

    // поиск камеры с заданным id
    for (int i = 0; i < cameraCount; i++) {
        if (cameras[i].id == id) {
            printf("Данные для камеры %d:\n", id);
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                printf("Дата: %s, Время: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Камера с ID %d не найдена.\n", id);
    }
}

/*
 * Функция delcamID удаляет камеру с заданным ID из списка камер
 *Входные параметры:
 *  - id: целое число (ID камеры), которую необходимо удалить
 * Алгоритм работы:
 * 1. Функция принимает ID камеры, которую необходимо удалить
 * 2. Производится поиск камеры с заданным ID в массиве cameras
 * 3. Если камера найдена, то выполняется сдвиг всех последующих элементов массива, чтобы удалить камеру и не оставить пропусков
 * 4. Уменьшается количество камер в массиве cameraCount
 * 5. Если камера с указанным ID не найдена, то выводится сообщение о том, что камера не найдена
 * Возвращаемое значение: нет
 */
void delcamID(int id) {
    int found = 0;
    int indexToDelete = -1;

    for (int i = 0; i < cameraCount; i++) {
        if (cameras[i].id == id) {
            found = 1;
            indexToDelete = i;
            break;
        }
    }

    if (found) {
        // cдвиг камеры после удаления, чтобы не было пропусков
        for (int i = indexToDelete; i < cameraCount - 1; i++) {
            cameras[i] = cameras[i + 1];
        }

        cameraCount--;
        printf("Камера с ID %d была удалена.\n", id);
    }
    else {
        printf("Камера с ID %d не найдена.\n", id);
    }
}

/*
 * Функция rndmpoka выводит случайную прощальную фразу из подготовленного списка
 * Алгоритм работы:
 * 1. Функция создает массив строк с различными фразами для прощания
 * 2. Определяется размер массива фраз для прощания
 * 3. Инициализируется генератор случайных чисел с использованием текущего времени
 * 4. Выбирается случайный индекс в пределах массива фраз
 * 5. Выводится выбранная случайная фраза для прощания
 * Возвращаемое значение: нет
 */
void rndmpoka() {
    const char* goodbyes[] = {
        "Пока!", "До свидания!", "Увидимся!", "До встречи!", "Прощай!",
        "Всего доброго!", "Бывай!", "Пойду!", "Удачи!", "Спокойной ночи!",
        "Пока-пока!", "Береги себя!", "До скорого!",
        "До скорой встречи!", "Удачи тебе!", "Всего хорошего!", "На связи!",
        "Всего наилучшего!", "Прощай, друг!", "Пусть вам повезет!", "Не прощаемся!",
        "Скоро увидимся!", "Пока, до связи!", "Пока, удачи!", "Пока, до завтра!",
        "До новых встреч!", "Всего наилучшего!", "Чао!",
        "До завтра!", "Счастливо!", "Прощай, до встречи!", "Пока, до скорого!",
        "Увидимся позже!", "Желаю удачи!", "Береги себя!",
        "Будь здоров!", "До встречи в будущем!", "До следующего дня!",
        "Держись!", "Пусть тебе повезет!", "Жду тебя!",
        "Удачного дня!", "Прощай, до скорого!", "Гудбай!"
    };
    int numGoodbyes = sizeof(goodbyes) / sizeof(goodbyes[0]);  // определение размера массива
    srand(time(NULL));
    int randomIndex = rand() % numGoodbyes;
    printf("%s\n", goodbyes[randomIndex]);
}

/*
 * Функция privsv выводит сведения о программе
 * Алгоритм работы:
 * 1. Экран очтщается с помощью команды system("cls")
 * 2. Выводится приветственное сообщение, писывается функциональность программы
 * 3. Ожидается нажатие клавиши для продолжения работы программы
 * Возвращаемое значение: нет
 */
void privsv() {
    system("cls");
    printf("==============================================================================\n");
    printf("Добро пожаловать в программу мониторинга данных с камеры Camtrol!\n");
    printf("Версия: 1.0\n");
    printf("Разработчик: Ястребов Г.В. \n");
    printf("Группа: бИЦ-241 \n");
    printf("******************************************************************************\n");
    printf("Описание программы: Данная программа — умный страж данных,\n");
    printf("который с неослабевающим вниманием следит за снимками\n");
    printf("поступающими с камер. Она тщательно проверяет каждую строку данных,\n");
    printf("словно исследователь, стараясь найти любые изменения и обновить информацию.\n");
    printf("В режиме мониторинга она неустанно следит за изменениями\n");
    printf("в реальном времени и, как маг, генерирует новые записи,\n");
    printf("наполняя пространство данными. И если вдруг вам нужно удалить или\n");
    printf("найти что-то среди множества снимков, она без промедлений выполнит задачу. \n");
    printf("==============================================================================\n");

    printf("\nНажмите любую клавишу для продолжения...");
    _getch();
}

void forcase1() {
    printf("Введите имя файла: ");
    fgets(monitoringFile, sizeof(monitoringFile), stdin);
    monitoringFile[strcspn(monitoringFile, "\n")] = '\0';
    loadDataFromFile(monitoringFile);
}