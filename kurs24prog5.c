//�����: �������� ����
//��������: ���� ���� �������� ���������� ������� ����������� ������ � ����� ��������������� 

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

// ����
int main() {
    setlocale(LC_CTYPE, "RUS");
    int choice;
    privsv();

    while (1) {
        system("cls");
        printf("����� ���������� � ����!\n\n");
        printf("1. ��������� ������ �� �����\n");
        printf("2. �������� ������ �������\n");
        printf("3. ��������� �������� ������\n");
        printf("4. ������� ��� ������ (c ������� ����������)\n");
        printf("5. ����� �����������\n");
        printf("6. �������� ������ ������ �� ID\n");
        printf("7. ������� ������ �� ID\n");
        printf("8. ������� ��� ������\n");
        printf("9. C������� � ���������\n");
        printf("10.�����\n\n");
        printf("�������� �����: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: {
            system("cls");
            forcase1();
            printf("\n������� ����� ������� ��� �����������...");
            _getch(); // �������� ������� �������
            break;
        }
        case 2: {
            char inputData[100];
            system("cls");
            printf("������� ������ ������ (������ DD.MM.YY_HH:MM:SS_ID): ");
            fgets(inputData, sizeof(inputData), stdin);
            inputData[strcspn(inputData, "\n")] = '\0';
            addCamera(inputData);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 3:
            system("cls");
            generateTestData();
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        case 4:
            system("cls");
            palld();
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        case 5:
            system("cls");
            monitoring();
            break;
        case 6: {
            system("cls");
            printf("������� ID ������ ��� ��������: ");
            int checkId;
            scanf("%d", &checkId);
            checkcamID(checkId);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 7: {
            system("cls");
            printf("������� ID ������ ��� ��������: ");
            int deleteId;
            scanf("%d", &deleteId);
            delcamID(deleteId);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 8:
            system("cls");
            delallc();
            printf("\n������� ����� ������� ��� �����������...");
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
            printf("������������ �����.\n");
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
    }
    return 0;
}

/*
 * ��� ������ Snapshot:
 * ��������� ��� �������� ���������� � ������.
 * �������� ���� � ����� ������ � ������� ������.
 */
typedef struct {
    char date[11];
    char time[9];
} Snapshot;

/*
 * ��� ������ Camera:
 * ��������� ��� �������� ���������� � ������.
 * �������� ���������� ������������� ������, ������ �������, ���������� ������� � ������������ ������.
 */
typedef struct {
    int id;
    Snapshot snapshots[MAX_SNAPSHOTS];
    int snapshotCount;
    char originalData[100];
} Camera;

Camera cameras[MAX_CAMERAS];                   // ������ �����, � ������� �������� ��� ������ � ���
int cameraCount = 0;
char monitoringFile[MAX_FILENAME_LENGTH] = ""; // ��� ����� ��� �����������

/*
 * ������� prData ��������� ������������ ���� � ������� "��.��.��"
 * ������� ��������:
 *   - date: ������, �������������� ���� � ������� "��.��.��"
 * �������� ������:
 *   ������� �������� ��������� ������ �� ��� ����� �����: ����, ����� � ���
 *   ���� ������ �� ������ ��� ���� ���� ��������� �� ��������� ����������� ��������� (1-31),
 *   ��� ����� ��� ��������� (1-12) �� ������� ���������� 0 (������)
 *   � ��������� ������ ������������ 1 (���� ���������)
 * ������������ ��������:
 *   - 1: ���� ���������
 *   - 0: ���� �����������
 */
int prData(const char* date) {
    int day, month, year;
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) != 3) {
        return 0; //������
    }
    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return 0; //�������� ����
    }
    return 1; //���� �����
}

/*
 * ������� prTime ��������� ������������ ������� � ������� "��:��:��"
 * ������� ��������:
 *   - time: ������, �������������� ����� � ������� "��:��:��"
 * �������� ������:
 *   ������� �������� ���������� ����������, �� � ��������� �� �����, ������ ����
 * ������������ ��������:
 *   - 1: ����� ���������
 *   - 0: ����� �����������
 */
int prTime(const char* time) {
    int hour, minute, second;
    if (sscanf(time, "%d:%d:%d", &hour, &minute, &second) != 3) {
        return 0; //������
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; //�������� �����
    }
    return 1; //���������� �����
}

/*
 * ������� addCamera ��������� ����� ������ ��� ����� ������ ��� ������������ ������
 * ������� ��������:
 *   - inputData: ������, ���������� ������ ������ � ������� "DD.MM.YY_HH:MM:SS_ID"
 * �������� ������:
 *   ������� �������� ��������� ������ inputData �� ��� ����������: ����, ����� � ID ������
 *   ���� ������ �� ������������� ���������� �������, �� ��������� ��������� �� ������
 *   ����� ����������� ������������ ���� � ������� � ������� ������� prData � prTime
 *   ���� ������ � ����� ID ��� ����������:
 *     - �����������, ���������� �� ������ � ����� �� ��������. ���� ����������, �� ������� ��������� ����������
 *     - ���� ������ �� ���������� � � ������ ��� ���� ����� ��� ����� �������, �� ������ �����������
 *   ���� ������ � ����� ID �� �������:
 *     - ����������� ����� ������ � ��������� ID � ������ � ������� ���� � �������
 * ������������ ��������:
 *   - ������� �� ���������� ��������. ��� ������ �������������� ������ ������� � ������� ���������
 */
void addCamera(const char* inputData) {
    int id;
    char date[11], time[9];
    if (sscanf(inputData, "%10[^_]_%8[^_]_%d", date, time, &id) != 3) {
        printf("������: ������ ������ ���� � ������� DD.MM.YY_HH:MM:SS_ID.\n");
        return;
    }

    if (!prData(date) || !prTime(time)) {
        printf("������: ������������ ���� ��� �����.\n");
        return;
    }

    // �������� ������������� ������
    for (int i = 0; i < cameraCount; i++) {
        if (cameras[i].id == id) {
            // �������� �� ������� ������
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                if (strcmp(cameras[i].snapshots[j].date, date) == 0 &&
                    strcmp(cameras[i].snapshots[j].time, time) == 0) {
                    // ������ ��� ����������
                    return;
                }
            }

            // ���������� ������ ������
            if (cameras[i].snapshotCount < MAX_SNAPSHOTS) {
                strncpy(cameras[i].snapshots[cameras[i].snapshotCount].date, date, sizeof(date));
                strncpy(cameras[i].snapshots[cameras[i].snapshotCount].time, time, sizeof(time));
                cameras[i].snapshotCount++;
            }
            else {
                printf("������ %d �������� ������������� ���������� �������.\n", id);
            }
            return;
        }
    }

    // ���� ������ �� �������, �������� �����
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
 * ������� generateTestData ���������� �������� ������ ��� �����
 * ������� ��������: ���
 * �������� ������:
 *   1. ������� ����������� � ������������ ���������� ����� (�������), ������� ����� �������������
 *   2. ������������ ��������� ��������� ����� ��� �������� ��������� ��������:
 *      - ID ������ � ��������� �� 1 �� 100
 *      - ���� � �������� ��������� ��������
 *      - ����� � �������� ���������� ��������
 *   3. �������������� ������ � ������� � ������� "DD.MM.YY_HH:MM:SS_ID" ���������� � ������� addCamera ��� ���������� ������ � ������
 *   4. ��������� ������� ��������� ������������� ���������� ���
 * ������������ ��������: ���
 */
void generateTestData() {
    int numLines;
    printf("������� ���������� c���� (������� � ������ �����), ������� ����� �������������: ");
    scanf("%d", &numLines);

    srand(time(NULL));  // ������������� ���������� ��������� �����

    for (int i = 0; i < numLines; i++) {
        int cameraId = rand() % 100 + 1;  // ��������� id ������ �� 1 �� 100
        int day = rand() % 31 + 1;
        int month = rand() % 12 + 1;
        int year = rand() % 100;          //��������� ���� � ��������� 0-99
        int hour = rand() % 24;
        int minute = rand() % 60;
        int second = rand() % 60;

        // ��������� ������ � ������� ��� ������
        char testData[100];
        snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, cameraId);

        addCamera(testData);
    }

    printf("�������� ������ �������������.\n");
}

/*
 * ������� loadDataFromFile ��������� ������ � ������� �� �����
 * ������� ��������:
 *   - filename: ��� �����, �� �������� ����� ��������� ������
 * �������� ������:
 *   1. ��������� ���� � ������ ������
 *   2. ���� ���� �� ������� �������, ��������� ��������� �� ������
 *   3. ��������� ��������� ������ �� ����� � ������ line
 *   4. ������ ������ ���������� � ������� addCamera ��� ���������� ������ � ������
 *   5. ����� �������� ������ ���� �����������
 * ������������ ��������: ���
 */
void loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r"); //�������� ����� � ������ ������
    if (!file) {
        printf("������ ��� �������� ����� %s.\n", filename);
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; //�������� ������� ����� ������
        addCamera(line);
    }
    fclose(file); //�������� �����
    printf("������ �� ����� %s ���������.\n", filename);
}

/*
 * ������� getFileSize ��������� ������ �����
 * ������� ��������:
 *   - filename: ��� ����� ��� ����������� ��� �������
 * �������� ������:
 *   1. ��������� ���� � �������� ������ ��� ��������� �������
 *   2. ���� ���� �� ������� �������, ������������ -1
 *   3. ���������� ��������� � ����� ����� � ���������� ��� ������ � ������� ftell
 *   4. ��������� ���� � ���������� ��� ������
 * ������������ ��������:
 *   - size (long): ������ ����� � ������
 *   - ���� ���� �� ������� �������, ���������� -1
 */
long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb"); //�������� ���� � �������� ������
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file); //��������� ������ � ������� �����
    fclose(file);
    return size;
}

/*
 * ������� reloadFileIfUpdated ���������, ���� �� ��������� � ����� � ������� ��������� ��������
 * ������� ���������:
 *   - filename: ��� �����, ������� ����������� �� ���������
 *   - lastSize: ��������� �� ����������, �������� ������ ����� �� ������ ��������� ��������
 * �������� ������:
 *   1. �������� ������� ������ ����� � ������� ������� getFileSize
 *   2. ���������� ������� ������ � �������� �����, ����������� � lastSize
 *   3. ���� ������ ����� ���������, ��������� �������� lastSize � ������� ��������� � ������� ���������
 *   4. ��������� ����� ������ �� ����� � ������� ������� loadDataFromFile
 * ������������ ��������: ���
 */
void reloadFileIfUpdated(const char* filename, long* lastSize) {
    long currentSize = getFileSize(filename);
    if (currentSize != *lastSize) {
        *lastSize = currentSize;
        printf("���������� ��������� � ����� %s. �������� ����� ������...\n", filename);
        loadDataFromFile(filename);
    }
}

/*
 * ������� generateSingleSnapshot ���������� ���� ������ ������ ��� ������ � ���������� ����������,
 * � ����� ��������� � � �������
 * ������� ���������: ���
 * �������� ������:
 *   1. �������������� ��������� ��������� ����� � ������� ������� srand
 *   2. ���������� ��������� �������� ��� id ������, ���, ������, ����, ����, ������ � �������
 *   3. ��������� ������ testData, ������� ������������ ����� ������ ������ � ������� "��.��.��_��:��:��_ID"
 *   4. ��������� �������������� ������ � ������� ������� addCamera
 *   5. ������� ��������� � ���, ��� ������ ������ ���� ������� �������������
 * ������������ ��������: ���
 */
void generateSingleSnapshot() {
    srand((unsigned int)time(NULL)); // ������������� ���������� ��������� �����
    int id = rand() % 100 + 1;
    int day = rand() % 31 + 1;
    int month = rand() % 12 + 1;
    int year = rand() % 100;
    int hour = rand() % 24;
    int minute = rand() % 60;
    int second = rand() % 60;

    char testData[100];
    snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, id); //������������ ����� ������
    addCamera(testData);
    printf("����� ������ ������ �������������: %s\n", testData);
}

/*
 * ������� sortIDvozr ��������� ���������� �������� ����� �� id � ������������ �������
 * ������� ���������:
 *   - a, b: ��������� �� �������� ������� ����� (���� Camera)
 * �������� ������:
 *   1. ����������� ��������� a � b � ��������� �� ��������� Camera
 *   2. ���������� ������� ����� id ���� �����, ��� ��������� qsort ������������� ������ � ������������ �������
 * ������������ ��������:
 *   - ������� ����� ���������� id ���� �����
 */
int sortIDvozr(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;
    return cameraA->id - cameraB->id;
}

/*
 * ������� sortIDub ��������� ���������� �������� ����� id � ��������� �������
 * ������� ���������:
 *   - a, b: ��������� �� �������� ������� ����� (���� Camera).
 * �������� ������:
 *   ������� �������� ���������� ����������, �� �� ���� ��� ������� ������� B � A
 * ������������ ��������:
 *   - ������� ����� ���������� id ���� �����
 */
int sortIDub(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;
    return cameraB->id - cameraA->id;
}

/*
 * ������� sortDatest ��������� ���������� ������� ����� �� ���� (������� ������)
 * ������� ���������:
 *   - a, b: ��������� �� �������� ������� �����
 * �������� ������:
 *   1. ������� ���������� ��� ������ ������ �� �����
 *   2. ��� ������� ������ ��������� ������ ���� �� ����, ����� � ��� � ������� ������� sscanf
 *   3. ���������� ����, ������ � ���. ���� ��� ��� ��������� ���������, ��������� � ��������� �������
 *   4. ��� ������� ��������� ������ ������� �� ����, ������ � �������, � ��������� ���������
 * ������������ ��������:
 *   - ������������� ��������, ���� ���� ��� ����� ������� ������ ������ ������� (��� ���������� �� �����������)
 *   - ������������� ��������, ���� ���� ��� ����� ������� ������ ����� �������
 *   - 0, ���� ���� � ����� ����������
 */
int sortDatest(const void* a, const void* b) {
    Camera* cameraA = (Camera*)a;
    Camera* cameraB = (Camera*)b;

    // �������� �� ���� ������� ������ � ���������� ��
    for (int i = 0; i < cameraA->snapshotCount; i++) {
        for (int j = 0; j < cameraB->snapshotCount; j++) {
            // �������� ���� �� ���� ����� ���
            int dayA, monthA, yearA;
            int dayB, monthB, yearB;
            sscanf(cameraA->snapshots[i].date, "%d.%d.%d", &dayA, &monthA, &yearA);
            sscanf(cameraB->snapshots[j].date, "%d.%d.%d", &dayB, &monthB, &yearB);

            // ���������� �� ����
            if (yearA != yearB) {
                return yearA - yearB;
            }

            // ���� ���� ����������, ���������� �� ������
            if (monthA != monthB) {
                return monthA - monthB;
            }

            // ���� ������ ����������, ���������� �� ���
            if (dayA != dayB) {
                return dayA - dayB;
            }

            // ���� ��� ����������, ���������� �� �������
            int hourA, minuteA, secondA;
            int hourB, minuteB, secondB;
            sscanf(cameraA->snapshots[i].time, "%d:%d:%d", &hourA, &minuteA, &secondA);
            sscanf(cameraB->snapshots[j].time, "%d:%d:%d", &hourB, &minuteB, &secondB);

            // ���������� �� �������
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
 * ������� sortDaten ��������� ���������� ������� ����� �� ���� (������� �����)
 * ������� ���������:
 *   - a, b: ��������� �� �������� ������� ����� (���� Camera)
 * �������� ������:
 *   ������� �������� ���������� ����������, � ������� ������� �������������� � �������������� ��������
 * ������������ ��������:
 *   - ������������� ��������, ���� ���� ��� ����� ������� ������ ����� ������� (��� ���������� �� ��������)
 *   - ������������� ��������, ���� ���� ��� ����� ������� ������ ������ �������
 *   - 0, ���� ���� � ����� ����������
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
 * ������� palld ������� ������ � ������� � �� ������� � ������������ ����������
 * �������� ������:
 *   1. ��������� ���� �� ������ � �������. ���� ������ ���, ������� ��������� �� ������
 *   2. ���������� ������������ ������� ������� ����������:
 *   3. � ����������� �� ������ ������������ ����������� ��������������� ����������:
 *      - ��� ���������� �� ID ������������ ������� sortIDvozr (�����������) ��� sortIDub (��������)
 *      - ��� ���������� �� ���� ������ ������������ ������� sortDatest (������ �������) � sortDaten (����� �������)
 *   4. ����� ���������� ���������� ������� ��� ������ � ������� � �� ������� � ��������� �������
 *   5. ���� ������������ �������� �������� ������� ����������, ������ ��������� ��� ��������� (��� ���������)
 * ������������ ��������: ���
 */
void palld() {
    if (cameraCount == 0) {
        printf("��� ������ � �������.\n");
        return;
    }

    int sortOption;
    printf("\n�������� ������� ����������:\n");
    printf("1. �� ��������� (��� ����������, ��� ���������)\n");
    printf("2. �� ID (�����������)\n");
    printf("3. �� ID (��������)\n");
    printf("4. �� ���� ������ (������ �������)\n");
    printf("5. �� ���� ������ (����� �������)\n");
    printf("��� �����: ");
    scanf("%d", &sortOption);

    // ������ ��������� ��� ��������� (��� ����������)
    if (sortOption == 1) {
        printf("\n��� ������ (�� ���������, ��� ���������):\n");
        for (int i = 0; i < cameraCount; i++) {
            printf("������ %d:\n", cameras[i].id);
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                printf("����: %s, �����: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
            }
            printf("\n");
        }
        return;
    }

    // ���������� � ����������� �� ������
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
        printf("�������� ����� ����������. ������������ ���������� �� ��������� (��� ����������).\n");
        break;
    }

    // ����� ������ ����� ����������
    printf("\n��� ������:\n");
    for (int i = 0; i < cameraCount; i++) {
        printf("������ %d:\n", cameras[i].id);
        for (int j = 0; j < cameras[i].snapshotCount; j++) {
            printf("����: %s, �����: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * ������� pallda ������� ��� ������ � ������� � �� �������
 * ���� ��� ������ � �������, ��������� ��������� �� ����
 * ������������ ��������: ���
 */
void pallda() {
    if (cameraCount == 0) {
        printf("��� ������ � �������.\n");
        return;
    }
    printf("��� ������:\n");
    for (int i = 0; i < cameraCount; i++) {
        printf("������ %d:\n", cameras[i].id);
        for (int j = 0; j < cameras[i].snapshotCount; j++) {
            printf("����: %s, �����: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * ������� monitoring ��������� ����� �����������, � �������:
 * 1. ����������� ������� ����� ��� ����������� � ����������� ������
 * 2. ������ 5 ������ �����������, ��������� �� ������ �����
 *    - ���� ���� ��� �������, ���������� ������������ ������
 * 3. ������ 20 ������ ������������ ����� ������ (������)
 * 4. ��� ������ �� ������ ����������� ���������� ������ ����� �������
 *
 * ������� ���������� ���, �� ������������ ���������� ����������:
 *  - monitoringFile: ���������� ����������, ���������� ��� ����� ��� �����������
 *  - cameraCount: ���������� ����������, ���������� ���������� �����
 *
 * �������� ������:
 * 1. ������� ���������, ��� �� �������� ���� ��� �����������, � ������� ��������� �� ������, ���� ���� �� ������
 * 2. ���������������� ���������� lastSize ��� ������������ ��������� � �����
 * 3. ����� ����� ����� ��������� � ��������� ��������� � ���, ��� ����� ����������� �������
 * 4. ������ �� ����� ����������� � ��������� �� �����
 * 5. ������� ������ � ����������� ����, � �������:
 *    - ������ ������� ������������� ��� �������� (elapsed5 � elapsed20), ������� ����������� ��������� �������
 *    - ������ 5 ������ ����������� ������ �����:
 *      - ���� ������ ����� ��������� (��� ��������������� � ���������� ����� ������), ���������� ������������ ������ �� �����
 *      - ����� ������������ ������ ��������� ���������� ������ �� �����
 *    - ������ 20 ������ ������������ ����� ������ (����� ������ ������)
 *      - ����� ��������� ������ ����� ��������� � ��������� ���������� ������
 * 6. ���� ������������ �������� ����� �������, ���������� ����� �� ������ ����������� � ������� � ����
 *
 * ������������ ��������: ���
 */
void monitoring() {
    if (strlen(monitoringFile) == 0) {
        printf("���� ��� ����������� �� ��������. ������� ��������� ���� ����� ����.\n");
        printf("������� ����� ������� ��� �������� � ����...\n");
        _getch();
        return;
    }

    long lastSize = getFileSize(monitoringFile);
    system("cls");
    printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");

    // ��������� � ������� ��� ������
    loadDataFromFile(monitoringFile);
    pallda();

    int elapsed5 = 0;   // ������ ��� 5 ������ (�������� ����������)
    int elapsed20 = 0;  // ������ ��� 20 ������ (��������� ����� ������)

    while (1) {
        Sleep(1000);
        elapsed5 += 1;
        elapsed20 += 1;

        if (_kbhit()) {  // �������� ������� �������
            _getch();    // ��������� ������
            system("cls");
            printf("����� �� ������ �����������.\n");
            break;
        }

        if (elapsed5 == 5) {  // ������ 5 ������ ��������� ����
            elapsed5 = 0;
            long currentSize = getFileSize(monitoringFile);

            // ���� ������ ����� ��������� ��� ������������ ������ �����������
            if (currentSize != lastSize) {
                lastSize = currentSize;

                // ��������� ������������� ������ �� �����
                cameraCount = 0;
                loadDataFromFile(monitoringFile);

                // ������� ������� � ����� ���������� ������
                system("cls");
                printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");
                pallda();
            }
        }

        if (elapsed20 == 20) {  // ������ 20 ������ ���������� ����� ������
            elapsed20 = 0;
            generateSingleSnapshot();

            // ����� ��������� ������ ������� ����� � ������� ����������
            system("cls");
            printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");
            pallda();
        }
    }
}


//������� delallc ������������ ��� �������� ���� �����
void delallc() {
    cameraCount = 0;
    printf("��� ������ �������.\n");
}

/*
 * ������� checkcamID ������������ ����� � ����� ������ ��� ������ �� ��������� ID
 * ������� ���������:
 *  - id: ����� ����� (ID ������), ��� ������� ����� ����� � ������� ������
 * �������� ������:
 * 1. ������� ��������� ID ������, ��� ������� ���������� ����� � ������� ������
 * 2. ������� ���� ������ � �������� ID � ������� cameras
 * 3. ���� ������ �������, ��������� ��� ������ (���� � �����) ��� ���� ������
 * 4. ���� ������ � ����� ID �� �������, ��������� ��������� � ���, ��� ������ � ������ ID �����������
 * ������������ ��������: ���
 */
void checkcamID(int id) {
    int found = 0;

    // ����� ������ � �������� id
    for (int i = 0; i < cameraCount; i++) {
        if (cameras[i].id == id) {
            printf("������ ��� ������ %d:\n", id);
            for (int j = 0; j < cameras[i].snapshotCount; j++) {
                printf("����: %s, �����: %s\n", cameras[i].snapshots[j].date, cameras[i].snapshots[j].time);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("������ � ID %d �� �������.\n", id);
    }
}

/*
 * ������� delcamID ������� ������ � �������� ID �� ������ �����
 *������� ���������:
 *  - id: ����� ����� (ID ������), ������� ���������� �������
 * �������� ������:
 * 1. ������� ��������� ID ������, ������� ���������� �������
 * 2. ������������ ����� ������ � �������� ID � ������� cameras
 * 3. ���� ������ �������, �� ����������� ����� ���� ����������� ��������� �������, ����� ������� ������ � �� �������� ���������
 * 4. ����������� ���������� ����� � ������� cameraCount
 * 5. ���� ������ � ��������� ID �� �������, �� ��������� ��������� � ���, ��� ������ �� �������
 * ������������ ��������: ���
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
        // c���� ������ ����� ��������, ����� �� ���� ���������
        for (int i = indexToDelete; i < cameraCount - 1; i++) {
            cameras[i] = cameras[i + 1];
        }

        cameraCount--;
        printf("������ � ID %d ���� �������.\n", id);
    }
    else {
        printf("������ � ID %d �� �������.\n", id);
    }
}

/*
 * ������� rndmpoka ������� ��������� ���������� ����� �� ��������������� ������
 * �������� ������:
 * 1. ������� ������� ������ ����� � ���������� ������� ��� ��������
 * 2. ������������ ������ ������� ���� ��� ��������
 * 3. ���������������� ��������� ��������� ����� � �������������� �������� �������
 * 4. ���������� ��������� ������ � �������� ������� ����
 * 5. ��������� ��������� ��������� ����� ��� ��������
 * ������������ ��������: ���
 */
void rndmpoka() {
    const char* goodbyes[] = {
        "����!", "�� ��������!", "��������!", "�� �������!", "������!",
        "����� �������!", "�����!", "�����!", "�����!", "��������� ����!",
        "����-����!", "������ ����!", "�� �������!",
        "�� ������ �������!", "����� ����!", "����� ��������!", "�� �����!",
        "����� ����������!", "������, ����!", "����� ��� �������!", "�� ���������!",
        "����� ��������!", "����, �� �����!", "����, �����!", "����, �� ������!",
        "�� ����� ������!", "����� ����������!", "���!",
        "�� ������!", "���������!", "������, �� �������!", "����, �� �������!",
        "�������� �����!", "����� �����!", "������ ����!",
        "���� ������!", "�� ������� � �������!", "�� ���������� ���!",
        "�������!", "����� ���� �������!", "��� ����!",
        "�������� ���!", "������, �� �������!", "������!"
    };
    int numGoodbyes = sizeof(goodbyes) / sizeof(goodbyes[0]);  // ����������� ������� �������
    srand(time(NULL));
    int randomIndex = rand() % numGoodbyes;
    printf("%s\n", goodbyes[randomIndex]);
}

/*
 * ������� privsv ������� �������� � ���������
 * �������� ������:
 * 1. ����� ��������� � ������� ������� system("cls")
 * 2. ��������� �������������� ���������, ���������� ���������������� ���������
 * 3. ��������� ������� ������� ��� ����������� ������ ���������
 * ������������ ��������: ���
 */
void privsv() {
    system("cls");
    printf("==============================================================================\n");
    printf("����� ���������� � ��������� ����������� ������ � ������ Camtrol!\n");
    printf("������: 1.0\n");
    printf("�����������: �������� �.�. \n");
    printf("������: ���-241 \n");
    printf("******************************************************************************\n");
    printf("�������� ���������: ������ ��������� � ����� ����� ������,\n");
    printf("������� � �������������� ��������� ������ �� ��������\n");
    printf("������������ � �����. ��� ��������� ��������� ������ ������ ������,\n");
    printf("������ �������������, �������� ����� ����� ��������� � �������� ����������.\n");
    printf("� ������ ����������� ��� ��������� ������ �� �����������\n");
    printf("� �������� ������� �, ��� ���, ���������� ����� ������,\n");
    printf("�������� ������������ �������. � ���� ����� ��� ����� ������� ���\n");
    printf("����� ���-�� ����� ��������� �������, ��� ��� ����������� �������� ������. \n");
    printf("==============================================================================\n");

    printf("\n������� ����� ������� ��� �����������...");
    _getch();
}

void forcase1() {
    printf("������� ��� �����: ");
    fgets(monitoringFile, sizeof(monitoringFile), stdin);
    monitoringFile[strcspn(monitoringFile, "\n")] = '\0';
    loadDataFromFile(monitoringFile);
}