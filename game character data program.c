#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct node
{
    char name[16];
    int level;
    int hp;
    struct node* p_next;

} CHARACTER;

typedef struct data
{
    char name[16];
    int level;
    int hp;

} CHARACTER_DATA;

void CharacterAdd(CHARACTER** pp_head, CHARACTER** pp_tail, char name_data[], int level_data, int hp_data)
{
    if (*pp_head != NULL) { // pp_head가 NULL이 아니다 == 리스트가 생성되어 있으니 마지막 노드를 가리키는 pp_tail을 이용함
        (*pp_tail)->p_next = (CHARACTER*)malloc(sizeof(CHARACTER)); // p_next는 CHARACTER * 타입임!
        if ((*pp_tail)->p_next == NULL) {
            printf("메모리 할당 실패!\n\n");
            return;
        }
        (*pp_tail) = (*pp_tail)->p_next;
    }
    else {
        (*pp_head) = (CHARACTER*)malloc(sizeof(CHARACTER));
        (*pp_tail) = (*pp_head);
    }

    strcpy((*pp_tail)->name, name_data);
    (*pp_tail)->level = level_data;
    (*pp_tail)->hp = hp_data;
    (*pp_tail)->p_next = NULL;
}

void SaveCharacter(CHARACTER *pp_head)
{
    FILE* p_file = fopen("CharacterDataINFO.dat", "wb");
    if (p_file == NULL) {
        printf("파일 열기에 실패했습니다.\n\n");
        return;
    }

    CHARACTER* p = pp_head;
    CHARACTER_DATA data;
    while (p != NULL) {
        strcpy(data.name, p->name);
        data.level = p->level;
        data.hp = p->hp;

        if (fwrite(&data, sizeof(CHARACTER_DATA), 1, p_file) != 1) {
            printf("파일 동기화에 실패했습니다.\n");
            return;
        }

        p = p->p_next;
    }
    fclose(p_file);
    return;
}

void LoadCharacter(CHARACTER* *pp_head, CHARACTER* *pp_tail) // 리스트의 헤드포인터와 테일 포인터가 바뀌기 때문에 이중포인터 선언
{
    FILE* p_file = fopen("CharacterDataINFO.dat", "rb");
    if (p_file == NULL) {
        printf("동기화 파일을 찾을 수 없습니다.\n");
        printf("새로운 동기화 파일을 제작합니다.\n");
        p_file = fopen("CharacterDataINFO", "wb");
        if (p_file == NULL) {
            printf("동기화 파일 제작에 실패했습니다.\n");
            return;
        }
        printf("동기화 파일 제작에 성공했습니다.\n\n");
        fclose(p_file);
        return;
    }

    CHARACTER TEMP;
    while (fread(&TEMP, sizeof(CHARACTER_DATA), 1, p_file)) { // CHARACTER_DATA 구조체 만큼 파일 데이터를 읽어와 TEMP 변수에 저장함.
        CharacterAdd(pp_head, pp_tail, TEMP.name, TEMP.level, TEMP.hp); // 파일에서 불러온 데이터들을 바로 CharacterAdd 함수 호출에 넣어버리면서, 바로 연결 리스트를 새로 만듬.
    }
    fclose(p_file);
    printf("파일 동기화에 성공하였습니다.\n\n");
    return;
}

void ShowCharacter(CHARACTER* pp_head)
{
    if (pp_head == NULL) { // pp_head가 NULL이다 즉, 연결 리스트의 헤드포인터가 NULL이기에 리스트가 생성되지 않았음을 의미함. = 캐릭터의 정보가 부족함.
        printf("캐릭터의 정보가 부족합니다.\n");
        printf("캐릭터의 정보가 최소 한명은 필요합니다.\n\n");
        return;
    }

    CHARACTER* p = pp_head;
    printf("=================================\n");
    while (p != NULL) {
        printf("이름 : %s | 레벨 : %d | HP : %d\n", p->name, p->level, p->hp);
        p = p->p_next;
    }
    printf("=================================\n");
    printf("모든 캐릭터 정보를 출력하였습니다.\n\n");
    return;
}

void InputCharacter(char name_data[], int* level_data, int* hp_data) // main 함수에서 선언한 지역변수의 값을 수정하기 위해선, 다름 함수에서는 포인터로 매개변수를 선언하여 주소를 받는다.
{
    printf("캐릭터의 이름을 입력해주세요.\n");
    printf("이름 입력 : ");
    scanf("%s", name_data);
    printf("캐릭터의 레벨을 입력해주세요.\n");
    scanf("%d", level_data); // 포인터 변수로 level_data를 매개변수로 받았기 때문에 &를 제외한다.
    printf("캐릭터의 HP를 입력해주세요.\n");
    scanf("%d", hp_data);
    printf("캐릭터 %s의 데이터를 모두 입력하셨습니다.\n", name_data);
    printf("첫 화면으로 돌아갑니다.\n\n");
    return;
}

void EndProgram(CHARACTER* *pp_head)
{
    printf("프로그램을 종료합니다.\n\n");
    CHARACTER* p;
    while ((*pp_head) != NULL) {
        p = *pp_head;
        *pp_head = (*pp_head)->p_next;
        free(p);
    }
    return;
}

int main()
{
    CHARACTER* p_head = NULL, * p_tail = NULL;
    char name_data[16];
    int level_data = 0, hp_data = 0;
    int num = 0;

    LoadCharacter(&p_head, &p_tail);

    while (1) {
        printf("게임 데이터 관리 프로그램\n");
        printf("[1] : 캐릭터 데이터 추가\n");
        printf("[2] : 캐릭터 데이터 출력\n");
        printf("[3] : 프로그램 종료\n");
        printf("============================\n");
        printf("원하는 기능의 숫자를 입력해 주세요.\n");
        printf("숫자 입력 : ");
        scanf("%d", &num);
        printf("\n");
        if (num == 1) {
            InputCharacter(name_data, &level_data, &hp_data);
            CharacterAdd(&p_head, &p_tail, name_data, level_data, hp_data);
            SaveCharacter(p_head);
            printf("캐릭터 정보 입력을 마치셨습니다.\n");
            printf("파일에 동기화도 성공적입니다.\n\n");
        }
        else if (num == 2) {
            ShowCharacter(p_head);
        }
        else if (num == 3) {
            EndProgram(&p_head);
            p_tail = p_head = NULL;
            break;
        }
        else {
            printf("잘못된 값을 입력하셨습니다. 첫화면으로 돌아갑니다.\n\n");
        }
    }
    return 0;
}