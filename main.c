//==============================================================================
// Created by Patrik Fusek 
// testovany so suborom "konferencny_zoznam.txt"
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>
#include <ctype.h>
#include <string.h>

#pragma warning(disable:6011)

// pocet riadkov v zazname;
enum { R8 = 8 };

enum Riadok_e {
    ePrezenter = 0,
    eRodneCislo = 1,
    eKodPprezentacnejMiestnosti = 2,
    eNazovPrispevku = 3,
    eMenaAutorov = 4,
    eTypPrezentovania = 5,
    eCasPrezentovania = 6,
    eDatum = 7
};

const char* TextPred[] =
{
   "Prezenter",
   "Rodne cislo",
   "Kod pprezentacnej miestnosti",
   "Nazov prispevku",
   "Mena autorov",
   "Typ prezentovania",
   "Cas prezentovania",
   "Datum"
};

typedef struct _Node_t {
    int zaznamId;
    char* zaznamItems[R8];
    struct _Node_t* next;
}Node;

void** Alokuj(int pocetZaznamov);
void AlokujPamet(char** pRetazec, int* velkost, int pocetRiadkovZaznamu);
void AlokujPametForIntArray(int** pInt, int velkost, int pocetRiadkovZaznamu);
void DealokujPamet(char** pRetazec, int pocetPrezenterov);
void Dealokuj(void** pTable, int pocetZaznamov);
void PrintZaznamy(void** pTable, int pocetZaznamov);
FILE* FileOpen(FILE* zoznamF, void** pTable, int pocetZaznamov);
void DajObsahZaznamov(FILE* zoznamF, void** pTable, int pocetZaznamov);

//==============================================================================
// Daj, vytvor novy Node. 
//==============================================================================
Node* DajNovyNode(int zaznamId, char* zaznamItems[R8])
{
    Node* node = malloc(sizeof(Node));
    int riadkyZoznamuMaxSize[] = { 50 + 1, 10 + 1, 3 + 1, 150 + 1, 200 + 1, 2 + 1, 4 + 1, 8 + 1 };
    node->zaznamId = zaznamId;
    int i;
    for (i = 0; i < R8; i++)
    {
        node->zaznamItems[i] = malloc(sizeof(char) * riadkyZoznamuMaxSize[i]);
        if (zaznamItems != NULL)
        {
            strcpy(node->zaznamItems[i], zaznamItems[i]);
        }
    }
    node->next = NULL;
    return node;
}

//==============================================================================
// Uvolni pamet pre jeden Node. 
//==============================================================================
DealokateNode(Node* node)
{
    size_t i;
    for (i = 0; i < R8; i++)
    {
        free(node->zaznamItems[i]);
    }
    free(node);
}

//==============================================================================
// Uvolni pamet pre vsetky nody v zozname, zacni od head nodu
//==============================================================================
DealokateNodes(Node* head)
{
    Node* temp;
    while (head)
    {
        temp = head->next;
        DealokateNode(head);
        head = temp;
    }
}

//==============================================================================
//
//==============================================================================
char* DajZaznam(void** pTable, int cisloZaznamu)
{
    return ((char*)(pTable[cisloZaznamu]));
}

//==============================================================================
//
//==============================================================================
char* DajRiadokZaznamu(void** pTable, int cisloZaznamu, int cisloRiadku)
{
    return ((char**)(pTable[cisloZaznamu]))[cisloRiadku];
}

//==============================================================================
//
//==============================================================================
int DajPocetZaznamov(FILE* zoznamF)
{
    int chCount = 0;
    int pocetZaznamov = 0;
    char ch;
    int  pocetCharNovyRiadok = 0;
    rewind(zoznamF);
    while (EOF != (ch = fgetc(zoznamF)))
    {
        chCount++;
        //if (ch == '$' || ch == '\n' || ch == '\r')
        if (ch == '$' || ch == '\n')
        {
            pocetCharNovyRiadok++;
            if (pocetCharNovyRiadok == 4)
            {
                pocetCharNovyRiadok = 0;
                pocetZaznamov++;
            }
        }
        else
        {
            pocetCharNovyRiadok = 0;
        }
    }
    if (ch == EOF)
    {
        //pocetZaznamov++;
    }
    rewind(zoznamF);
    return pocetZaznamov;
}

//==============================================================================
//
//==============================================================================
int KontrolaKoduMiestnosti(char* buffer, int zaznam)
{
    if (strlen(buffer) == 3 && isupper(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2]))
    {
        return 1;
    }
    else
    {
        if (zaznam == -1)
        {
            printf("Nekorektne zadany vstup v prikazeN: kod miestnosti %s\n", buffer);
        }
        else if (zaznam == -2)
        {

        }
        else
        {
            printf("Nekorektne zadany vstup v zazname %d: kod miestnosti %s\n", zaznam + 1, buffer);
        }
        return 0;
    }
}

//==============================================================================
//
//==============================================================================
int KontrolaCasuPrezentovania(char* buffer, int zaznam)
{
    int  isValidFlag = 1;
    char aHour[3];
    char aMinute[3];

    aHour[2] = 0;
    aMinute[2] = 0;
    memcpy(aHour, buffer, 2);
    memcpy(aMinute, buffer + 2, 2);

    if (isdigit(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2]) && isdigit(buffer[3]) && strlen(buffer) == 4)
    {
        int hour = atoi(aHour);
        int minute = atoi(aMinute);

        if ((hour < 0 || hour > 23) || (minute < 0 || minute > 59))
        {
            isValidFlag = 0;
        }
    }
    else
    {
        isValidFlag = 0;
    }

    if (isValidFlag == 0 && zaznam > -2)
    {
        printf("Nekorektne zadany vstup v zazname %d: Cas Prezentovania %s\n", zaznam + 1, buffer);

    }
    return isValidFlag;
}

//==============================================================================
//
//==============================================================================
int KontrolaDatumu(char* buffer, int zaznam)
{
    if (strlen(buffer) != 8 && zaznam > -2)
    {
        if (zaznam == -1)
        {
            printf("Nekorektne zadany vstup v prikazeN: Datum %s\n", buffer);
        }
        else
        {
            printf("Nekorektne zadany vstup v zazname %d: Datum %s\n", zaznam, buffer);
        }
        return 0;
    }

    char aYear[5];
    char aMonth[3];
    char aDay[3];

    aYear[4] = 0;
    aMonth[2] = 0;
    aDay[2] = 0;

    memcpy(aYear, buffer, 4);
    memcpy(aMonth, buffer + 4, 2);
    memcpy(aDay, buffer + 6, 2);

    int year = atoi(aYear);
    int month = atoi(aMonth);
    int day = atoi(aDay);

    int pocetDnivMesiaci[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    if ((year <= 0 || year > 9999) || (month <= 0 || month > 12) || (day <= 0 || day > pocetDnivMesiaci[month - 1]))
    {
        if (zaznam == -1)
        {
            printf("Nekorektne zadany vstup v prikazeN: Datum %s\n", buffer);
        }
        else if (zaznam == -2) {}
        else
        {
            printf("Nekorektne zadany vstup v zazname %d: Datum %s\n", zaznam + 1, buffer);
        }
        return 0;
    }
    return 1;
}

//==============================================================================
//
//==============================================================================
long KontrolaRodnehoCisla(char* buffer, int zaznam)
{
    int maxRC = 10;

    long long num = 0;
    size_t i;
    for (i = 0; i < strlen(buffer); i++)
    {
        num = num * 10 + ((long long)buffer[i] - '0');
    }
    if (strlen(buffer) == maxRC && num % 11 == 0)
    {
        return 1;
    }
    else
    {
        printf("Nekorektne zadany vstup v zazname %d: rodne cislo %s\n", zaznam + 1, buffer);
        return 0;
    }
}

//==============================================================================
//
//==============================================================================
int KontrolaTypuPrezentovania(char* buffer, int zaznam)
{
    if (strcmp(buffer, "PP") != 0)
        if (strcmp(buffer, "PD") != 0)
            if (strcmp(buffer, "UP") != 0)
                if (strcmp(buffer, "UD") != 0)
                {
                    if (zaznam > -2)
                    {
                        printf("Nekorektne zadany vstup v zazname %d: Typ Prezentovania %s\n", zaznam + 1, buffer);
                    }
                    return 0;
                }
    return 1;
}


//==============================================================================
//
//==============================================================================
void DajObsahZaznamov(FILE* zoznamF, void** pTable, int pocetZaznamov)
{
    int pocetCharNovyRiadok = 0;
    int vysledok = 1;
    char ch = 0;
    char buffer[200];

    char* prefix = "";
    char last = '\n';
    char* p = NULL;
    int i = 0;
    buffer[0] = 0;
    if (pocetZaznamov == 0)
    {
        pocetZaznamov = DajPocetZaznamov(zoznamF);
    }

    int pocetCharDolar = 0;
    do
    {
        ch = fgetc(zoznamF);
        //if (ch == '$' || ch == '\n' || ch == '\r') 
        if (ch == '$' || ch == '\n')
        {
            pocetCharNovyRiadok++;
        }
    } while (pocetCharNovyRiadok < 4);
    pocetCharNovyRiadok = 0;

    for (; ch != EOF && i < pocetZaznamov;)
    {
        int bufIdx = 0;
        int j;
        for (j = 0; j < R8; )
        {
            ch = fgetc(zoznamF);
            if (ch == '$') pocetCharDolar++;
            //if (ch == '\n' || ch == '\r')
            if (ch == '\n')
            {
                pocetCharNovyRiadok++;
                if (pocetCharNovyRiadok == 1)
                {
                    switch (j)
                    {
                    case 0: prefix = "Prezenter";
                        break;
                    case 1: prefix = "rodne cislo";
                        vysledok = KontrolaRodnehoCisla(buffer, i);
                        break;
                    case 2: prefix = "kod prezentacnej miestnosti";
                        vysledok = KontrolaKoduMiestnosti(buffer, i);
                        break;
                    case 3: prefix = "nazov prispevku";
                        break;
                    case 4: prefix = "mena autorov";
                        break;
                    case 5: prefix = "typ prezentovania";
                        vysledok = KontrolaTypuPrezentovania(buffer, i);
                        break;
                    case 6: prefix = "cas prezentovania";
                        vysledok = KontrolaCasuPrezentovania(buffer, i);
                        break;
                    case 7: prefix = "datum";
                        vysledok = KontrolaDatumu(buffer, i);
                        break;
                    default:
                        break;
                    }
                    //if (vysledok == 1) 
                    //{
                    if (pTable != NULL)
                    {
                        strcpy(DajRiadokZaznamu(pTable, i, j), buffer);
                    }
                    else
                    {
                        printf("%s: %s\n", prefix, buffer);
                    }
                    //}
                    vysledok = 1;
                    bufIdx = 0;
                    buffer[bufIdx] = 0;
                    j++;
                }
                if (pocetCharDolar >= 3)
                {
                    pocetCharDolar = 0;
                    if (pTable == 0)
                    {
                        printf("\n");
                    }

                    i++;
                    if (i >= pocetZaznamov)
                    {
                        break;
                    }
                }

            }
            else if (ch == EOF)
            {
                if (pTable != NULL)
                {
                    strcpy(DajRiadokZaznamu(pTable, i, j), buffer);
                }
                else
                {
                    printf("%s %s\n", "datum:", buffer);
                }
                break;
            }
            else if (ch == '$')
            {
            }
            else
            {
                pocetCharNovyRiadok = 0;
                buffer[bufIdx] = ch;
                if (buffer[bufIdx] == '#')
                {
                    buffer[bufIdx] = ' ';
                }
                bufIdx++;
                buffer[bufIdx] = 0;
            }
        }
    }
    rewind(zoznamF);
}

//==============================================================================
//
//==============================================================================
FILE* FileOpen(FILE* zoznamF, void** pTable, int pocetZaznamov)
{
    if (zoznamF == NULL)
    {
        zoznamF = fopen("konferencny_zoznam.txt", "rb");
        if (zoznamF == NULL)
        {
            //printf("Neotvoreny subor\n");
        }
    }
    else
    {
        //printf("Subor uz bol otvoreny\n");
    }
    return zoznamF;
}

//==============================================================================
// p Acâ‚¬â€o je prAfÂ­kaz na pridanie zAfÂ!znamu do spAfÂ!janAfÂ©ho zoznamu zAfÂ!znamov
//==============================================================================
Node* prikazN(Node* topNode, FILE* zoznamF)
{
    Node** nodes = NULL;
    void** pTable = NULL;
    int pocetZaznamov = 0;

    zoznamF = FileOpen(zoznamF, pTable, pocetZaznamov);

    if (zoznamF != NULL)
    {
        pocetZaznamov = DajPocetZaznamov(zoznamF);
    }
    else
    {
        //printf("@Neotvoreny subor\n");
    }

    if (pTable != 0)
    {
        Dealokuj(pTable, pocetZaznamov);
        pTable = 0;
    }

    if (topNode != NULL)
    {
        DealokateNodes(topNode);
    }
    if (zoznamF != NULL)
    {
        pTable = Alokuj(pocetZaznamov);
        DajObsahZaznamov(zoznamF, pTable, pocetZaznamov);
    }

    if (pocetZaznamov > 0)
    {
        printf("Nacitalo sa %d zaznamov\r\n", pocetZaznamov);
        if (nodes == NULL)
        {
            nodes = malloc(sizeof(Node) * (pocetZaznamov));
        }
        int i;
        for (i = 0; i < pocetZaznamov; i++)
        {
            nodes[i] = DajNovyNode(i, (char**)DajZaznam(pTable, i));
        }

        for (i = 0; (i < pocetZaznamov - 1); i++)
        {
            nodes[i]->next = nodes[i + 1];
        }

        topNode = nodes[0];
    }
    else
    {
        printf("Zaznamy neboli nacitane!\r\n");
    }

    if (pTable != 0) //NULL
    {
        Dealokuj(pTable, pocetZaznamov);
        pTable = 0; //NULL
    }
    return topNode;
}

//==============================================================================
// v Acâ‚¬â€o je prAfÂ­kaz na vAfÂ1pis celAfÂ©ho spAfÂ!janAfÂ©ho zoznamu zAfÂ!znamov
//==============================================================================
void prikazV(Node* firstNode)
{
    Node* node = firstNode;
    int i = 1;
    while (node != NULL)
    {
        //printf("%d.%d\r\n", i++, node->zaznamId);
        printf("%d\r\n", i++);
        printf("%s: %s\n\r", TextPred[ePrezenter], node->zaznamItems[ePrezenter]);
        printf("%s: %s\n\r", TextPred[eRodneCislo], node->zaznamItems[eRodneCislo]);
        printf("%s: %s\n\r", TextPred[eKodPprezentacnejMiestnosti], node->zaznamItems[eKodPprezentacnejMiestnosti]);
        printf("%s: %s\n\r", TextPred[eNazovPrispevku], node->zaznamItems[eNazovPrispevku]);
        printf("%s: %s\n\r", TextPred[eMenaAutorov], node->zaznamItems[eMenaAutorov]);
        printf("%s: %s\n\r", TextPred[eTypPrezentovania], node->zaznamItems[eTypPrezentovania]);
        printf("%s: %s\n\r", TextPred[eCasPrezentovania], node->zaznamItems[eCasPrezentovania]);
        printf("%s: %s\n\r", TextPred[eDatum], node->zaznamItems[eDatum]);
        node = node->next;
    }
}

//==============================================================================
// p Acâ‚¬â€o je prAfÂ­kaz na pridanie zAfÂ!znamu do spAfÂ!janAfÂ©ho zoznamu zAfÂ!znamov.
//==============================================================================
Node* prikazP(Node* topNode)
{
    int findFlag = 0;
    int zaznamId;

    Node* novyNode = DajNovyNode(0, NULL);

    int r = 0;
    do
    {
        //printf("@Zadaj cislo:");
        zaznamId = 0;
        r = scanf("%d", &zaznamId);
    } while ((r == 0 || zaznamId < 1) && zaznamId != -1);

    if (zaznamId < 0)
    {
        // rychly debug, bez dlheho vypisovania
        novyNode->zaznamId = 3;
        zaznamId = 3;
        char* pZaznam1[R8] =
        {
        "Patrik Fusek",
        "0206263432",
        "A21",
        "Metody inspirovane prirodou(2)",
        "Jana Oriesokova#Jan Oriesok#Jozko Mrkvicka",
        "PD",
        "1520",
        "20200405"
        };
        size_t i;
        for (i = 0; i < R8; i++)
        {
            strcpy(novyNode->zaznamItems[i], pZaznam1[i]);
        }
    }
    else
    {
        novyNode->zaznamId = zaznamId;
        size_t i;
        for (i = 0; i < R8; i++)
        {
            r = scanf("%s", novyNode->zaznamItems[i]);
        }
    }

    //for (size_t i = 0; i < R8; i++)
    //{
    //   printf("%s\r\n", novyNode->zaznamItems[i]);
    //}

    Node* node = topNode;
    int pozicia = 1;
    while (node->next != NULL)
    {
        if (pozicia == zaznamId - 1 || zaznamId == 1)
        {
            break;
        }
        pozicia++;
        node = node->next;
    }

    if (node->next != NULL)
    {
        if (pozicia == 1)
        {
            novyNode->next = node;
            topNode = novyNode;
        }
        else
        {
            // vloz novy node medzi dva existujuce
            novyNode->next = node->next;
            node->next = novyNode;
        }
    }
    else
    {
        // pridaj novy node na koniec
        node->next = novyNode;
    }
    return topNode;
}

//==============================================================================
// funkcia pre prikaz z
//==============================================================================
Node* VymazZaznamyPodlaRodnehoCisla(Node* head, char* rodneCislo)
{
    Node* novyHead = head;
    Node* current = head, * previous = head;

    while (current != NULL)
    {
        int r = strcmp(current->zaznamItems[eRodneCislo], rodneCislo);
        if (r == 0)
        {
            if (current == head)
            {
                head = head->next;
                novyHead = head;
                printf("%s\n", current->zaznamItems[ePrezenter]);
                DealokateNode(current);
                current = head;

            }
            else
            {
                Node* tmp = current;
                previous->next = current->next;
                current = current->next;
                printf("%s\n", tmp->zaznamItems[ePrezenter]);
                DealokateNode(tmp);
            }

        }
        else
        {
            previous = current;
            current = current->next;
        }

    }
    return novyHead;
}

//==============================================================================
// z Acâ‚¬â€o je prAfÂ­kaz na zmazanie zAfÂ!znamov podA„Â3a rodneho cAfÂ­sla
//==============================================================================
Node* prikazZ(Node* node0)
{
    Node* novyHead = node0;
    char rodneCislo[20];
    int vstup = scanf("%s", rodneCislo);

    if (vstup > 0 && rodneCislo[0] != 'x')
    {
        novyHead = VymazZaznamyPodlaRodnehoCisla(node0, rodneCislo);
    }
    else
    {
        char* pRodneCislo = "9403133410"; // "0206263432"
        printf("\r\n%s\r\n", pRodneCislo);
        novyHead = VymazZaznamyPodlaRodnehoCisla(node0, pRodneCislo);
    }
    return novyHead;
}

//==============================================================================
//
//==============================================================================
void prikazH(Node* head)
{
    Node* node = head;
    char zadanaPrezentacnaMiestnost[5];
    scanf("%s", zadanaPrezentacnaMiestnost);
    
    int i = 1;
    while (node != NULL)
    {
        int r = strcmp(zadanaPrezentacnaMiestnost, node->zaznamItems[eKodPprezentacnejMiestnosti]);
        if (r == 0)
        {
            //printf("%d.\r\n", node->zaznamId);
            printf("%d\r\n", i);
            printf("%s: %s\n\r", TextPred[ePrezenter], node->zaznamItems[ePrezenter]);
            printf("%s: %s\n\r", TextPred[eRodneCislo], node->zaznamItems[eRodneCislo]);
            printf("%s: %s\n\r", TextPred[eKodPprezentacnejMiestnosti], node->zaznamItems[eKodPprezentacnejMiestnosti]);
            printf("%s: %s\n\r", TextPred[eNazovPrispevku], node->zaznamItems[eNazovPrispevku]);
            printf("%s: %s\n\r", TextPred[eMenaAutorov], node->zaznamItems[eMenaAutorov]);
            printf("%s: %s\n\r", TextPred[eTypPrezentovania], node->zaznamItems[eTypPrezentovania]);
            printf("%s: %s\n\r", TextPred[eCasPrezentovania], node->zaznamItems[eCasPrezentovania]);
            printf("%s: %s\n\r", TextPred[eDatum], node->zaznamItems[eDatum]);
            i++;
        }
        node = node->next;
    }
    if (i == 0)
    {
        printf("Pre prezentacnu miestnost %s nie su prezentujuce osoby\r\n", zadanaPrezentacnaMiestnost);
    }
}

//==============================================================================
//
//==============================================================================
Node* ZmenZaznamyPodlaRodnehoCisla(Node* head, char* rodneCislo, char* novyTypPrezentovania)
{
    Node* node = head;
    while (node != NULL)
    {
        int r = strcmp(node->zaznamItems[eRodneCislo], rodneCislo);
        if (r == 0)
        {
            strcpy(node->zaznamItems[eTypPrezentovania], novyTypPrezentovania);
            printf("Prezenter Prezenter s menom %s bude prezentovat prispevok %s:%s\r\n",
                node->zaznamItems[ePrezenter],
                node->zaznamItems[eNazovPrispevku],
                node->zaznamItems[eTypPrezentovania]
            );
        }
        node = node->next;
    }
    return head;
}

//==============================================================================
//
//==============================================================================
int KontrolaZadanehoTypuPrezentovania(char* typPrezentovania)
{
    if (strcmp(typPrezentovania, "PP") != 0)
        if (strcmp(typPrezentovania, "PD") != 0)
            if (strcmp(typPrezentovania, "UP") != 0)
                if (strcmp(typPrezentovania, "UD") != 0)
                {
                    // Nekorektne zadany udaj
                    return 1;
                }
    // Korektne zadany udaj
    return 0;
}

//==============================================================================
//
//==============================================================================
long KontrolaZadanehoRodnehoCisla(char* rodneCislo)
{
    int maxRC = 10;

    long long num = 0;
    size_t i;
    for (i = 0; i < strlen(rodneCislo); i++)
    {
        num = num * 10 + ((long long)rodneCislo[i] - '0');
    }
    if (strlen(rodneCislo) == maxRC && num % 11 == 0)
    {
        return 0;
    }
    else
    {
        //Nekorektne zadany vstup v rodneho cisla
        return 1;
    }
}
//==============================================================================
// a Acâ‚¬â€o je prAfÂ­kaz na aktualizAfÂ!ciu (zmenu) typu prezentovania prispevku
// vstup - 9403133410 PD=novy typ prezentovania
//==============================================================================
void prikazA(Node* head)
{
    char rodneCislo[20];
    char typPrezentovania[10];

    int vstup = 0;
    int rc = 1;
    while (rc > 0)
    {
        vstup = scanf("%s %s", rodneCislo, typPrezentovania);
        if (vstup > 0 && rodneCislo[0] != 'x')
        {
            rc = KontrolaZadanehoRodnehoCisla(rodneCislo);
            rc |= KontrolaZadanehoTypuPrezentovania(typPrezentovania);
            if (rc > 0)
            {
                printf("\r\nZadane udaje nie su korektne, zadaj novy retazec : ");
                continue;
            }
            ZmenZaznamyPodlaRodnehoCisla(head, rodneCislo, typPrezentovania);
            // hotovo
            rc = 0;
            break; // pre istou :-)
        }
        else
        {
            char* pRodneCislo = "9403133410"; // "0206263432"
            char* pTypPrezentovania = "PD";
            printf("%s %s\r\n", pRodneCislo, pTypPrezentovania);
            ZmenZaznamyPodlaRodnehoCisla(head, pRodneCislo, pTypPrezentovania);
            rc = 0;
            break; // pre istotu
        }
    }
}

//==============================================================================
// funkcia na prehodenie dvoch zAfÂ!znamov v spAfÂ!janom zozname
//==============================================================================
Node* PrehodNody(Node* head, int c1, int c2)
{
    Node* novyHead = head;
    Node* node1 = NULL;
    Node* node2 = NULL;
    Node* prev1, * prev2, * temp;
    int i;

    // najvzdialenejsi node od head nodu
    int max = (c1 > c2) ? c1 : c2;

    // zistuje sa pocet zaznamov
    int pocetZaznamov = 0;
    Node* node = head;
    while (node != NULL)
    {
        pocetZaznamov++;
        node = node->next;
    }

    // Overenie ci c1 a c2 je v rozsahu 1 az pocet zaznamov
    if ((c1 <= 0 || c1 > pocetZaznamov) || (c2 <= 0 || c2 > pocetZaznamov))
    {
        return novyHead; // rovnaky s head
    }

    // ak su rovnake nie je co zamienat
    if (c1 == c2)
    {
        return novyHead; // rovnaky s head
    }

    // nastavenie pomocnych premennych
    i = 1;
    temp = head;
    prev1 = NULL;
    prev2 = NULL;

    // hladam nody ktore sa maju prehodit
    while (temp != NULL && (i <= max))
    {
        if (i == c1 - 1)
            prev1 = temp;
        if (i == c1)
            node1 = temp;

        if (i == c2 - 1)
            prev2 = temp;
        if (i == c2)
            node2 = temp;

        temp = temp->next;
        i++;
    }

    // Ak sa take nody nasli
    if (node1 != NULL && node2 != NULL)
    {
        // prepojenie node1 s node2
        if (prev1 != NULL)
            prev1->next = node2;

        // prepojenia node2 s node1
        if (prev2 != NULL)
            prev2->next = node1;

        // vymena node1 a node2 cez ich 
        // node next polozky
        temp = node1->next;
        node1->next = node2->next;
        node2->next = temp;

        // Nastavenie head nodu ak bol zahrnuty v prehodeni
        if (prev1 == NULL)
            head = node2;
        else if (prev2 == NULL)
            head = node1;
        // hotovo
        novyHead = head;
    }
    return novyHead;
}

//==============================================================================
// r - prAfÂ­kaz na prehodenie dvoch zAfÂ!znamov v spAfÂ!janom zozname
//==============================================================================
Node* prikazR(Node* head)
{
    Node* novyHead = head;
    int c1 = 0;
    int r = 0;
    r = scanf("%d", &c1);
    int c2 = 0;
    r = scanf("%d", &c2);
    novyHead = PrehodNody(head, c1, c2);
    return novyHead;
}
//==============================================================================
// q - prikaz na vypisanie spajaneho zoznamu struktur
//==============================================================================

void prikazQ(Node* head)
{
    while (head != NULL)
    {
   
        printf("%s: %s\n\r", TextPred[ePrezenter], head->zaznamItems[ePrezenter]);
        printf("%s: %s\n\r", TextPred[eRodneCislo], head->zaznamItems[eRodneCislo]);
        printf("%s: %s\n\r", TextPred[eKodPprezentacnejMiestnosti], head->zaznamItems[eKodPprezentacnejMiestnosti]);
        printf("%s: %s\n\r", TextPred[eNazovPrispevku], head->zaznamItems[eNazovPrispevku]);
        printf("%s: %s\n\r", TextPred[eMenaAutorov], head->zaznamItems[eMenaAutorov]);
        printf("%s: %s\n\r", TextPred[eTypPrezentovania], head->zaznamItems[eTypPrezentovania]);
        printf("%s: %s\n\r", TextPred[eCasPrezentovania], head->zaznamItems[eCasPrezentovania]);
        printf("%s: %s\n\n\r", TextPred[eDatum], head->zaznamItems[eDatum]);
    head = head->next;
    }
}

//==============================================================================
// 
//==============================================================================

Node* funkcia(Node* head, unsigned int k)
{
    Node* node = head;
    int i = 0;
    while (node != NULL)
    {
        node = node->next;
        i++;
    }
    node = head;
    i -= k;
    for (int j = 0; j < i; j++)
    {
        node = node->next;
    }
    printf("%s: %s\n\r", TextPred[ePrezenter], node->zaznamItems[ePrezenter]);
    printf("%s: %s\n\r", TextPred[eRodneCislo], node->zaznamItems[eRodneCislo]);
    printf("%s: %s\n\r", TextPred[eKodPprezentacnejMiestnosti], node->zaznamItems[eKodPprezentacnejMiestnosti]);
    printf("%s: %s\n\r", TextPred[eNazovPrispevku], node->zaznamItems[eNazovPrispevku]);
    printf("%s: %s\n\r", TextPred[eMenaAutorov], node->zaznamItems[eMenaAutorov]);
    printf("%s: %s\n\r", TextPred[eTypPrezentovania], node->zaznamItems[eTypPrezentovania]);
    printf("%s: %s\n\r", TextPred[eCasPrezentovania], node->zaznamItems[eCasPrezentovania]);
    printf("%s: %s\n\n\r", TextPred[eDatum], node->zaznamItems[eDatum]);
    return node;
}

//==============================================================================
// w - prikaz na vymenu/otocenie celeho spajaneho zoznamu struktur
//==============================================================================

Node* prikazW(Node* head, int k)
{
    
    Node* nodeHead = head;
    Node* previous;
    Node* current;

    for (int i = 0; i < k; i++)
    {
        nodeHead = nodeHead->next;
    }
    if (nodeHead != NULL)
    {
        previous = nodeHead;
        current = nodeHead->next;
        nodeHead = nodeHead->next;

        previous->next = NULL; 

        while (nodeHead != NULL)
        {

            nodeHead = nodeHead->next;
            current->next = previous;
            previous = current;

            current = nodeHead;
        }

        nodeHead = previous; 
        //printf("SUCCESSFULLY REVERSED LIST\n");
    }
    return nodeHead;
}

//==============================================================================
// main function
//==============================================================================
int main()
{
    Node* topNode = NULL;
    FILE* zoznamF = NULL;
    int k = 4;
    char prikazy;
    int pracuj = 1;
    

    while (pracuj > 0)
    {
        if (scanf("%c", &prikazy) != 0)
        {
            switch (prikazy)
            {
                //-----------------------------------------------------------------------------
            case 'n': // n Acâ‚¬â€o je prAfÂ­kaz na naA„Â?AfÂ­tanie zAfÂ!znamov zoznamu AfÂoA„Â?astnAfÂ­kov konferencie zo
                      // sAfÂoboru konferencny_zoznam.txt do spAfÂ!janAfÂ©ho zoznamu A…Â!truktAfÂor
                topNode = prikazN(topNode, zoznamF);
                //c(topNode, 4);
                break;
                //-----------------------------------------------------------------------------
            case 'v': // v Acâ‚¬â€o je prAfÂ­kaz na vAfÂ1pis celAfÂ©ho spAfÂ!janAfÂ©ho zoznamu zAfÂ!znamov
                prikazV(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'r': // r Acâ‚¬â€o je prAfÂ­kaz na prehodenie dvoch zAfÂ!znamov v spAfÂ!janom zozname
                topNode = prikazR(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'z': // z Acâ‚¬â€o je prAfÂ­kaz na zmazanie zAfÂ!znamov podA„Â3a rodneho cAfÂ­sla
                topNode = prikazZ(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'h': // h - umoA…Â3nAfÂ­ pouA…Â3AfÂ­vateA„Â3ovi vyhA„Â3adaA…ÂY a vypAfÂ­saA…ÂY vA…Â!etky poloA…Â3ky zAfÂ!znamu podA„Â3a kAfÂ3du prezentaA„Â?nej miestnosti
                prikazH(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'a': // a Acâ‚¬â€o je prAfÂ­kaz na aktualizAfÂ!ciu (zmenu) typu prezentovania prispevku
                prikazA(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'p': // p Acâ‚¬â€o je prAfÂ­kaz na pridanie zAfÂ!znamu do spAfÂ!janAfÂ©ho zoznamu zAfÂ!znamov.
                topNode = prikazP(topNode);
                break;
                //-----------------------------------------------------------------------------
            case 'w':
                topNode = prikazW(topNode, k);
                break;
            case 'q':
                prikazQ(topNode);
                break;
            case 'k': // k Acâ‚¬â€o je prAfÂ­kaz na ukonA„Â?enie programu. Ak spAfÂ!janAfÂ1 zoznam existoval, treba korektne uvoA„Â3niA…ÂY alokovanAfÂo pamAfÂ¤A…ÂY.P
                pracuj = 0;
                break;
                //-----------------------------------------------------------------------------
            default:
                break;
            }
        }
    }

    // po stlaceni k

    if (zoznamF != NULL)
    {
        fclose(zoznamF);
    }

    DealokateNodes(topNode);

    //printf("Koniec Programu.\n");
    return 0;
}

//==============================================================================
// alokacia pre nacitanie suboru
//==============================================================================
void** Alokuj(int pocetZaznamov)
{
    int pocetRiadkovZaznamu = R8; // 0-7

    void** pTable;
    pTable = malloc(sizeof(char**) * pocetZaznamov);

    int riadkyZoznamuMaxSize[] = { 50 + 1, 10 + 1, 3 + 1, 150 + 1, 200 + 1, 2 + 1, 4 + 1, 8 + 1 };
    int i;
    for (i = 0; i < pocetZaznamov; i++)
    {
        char** pZaznam;
        pZaznam = malloc(sizeof(char*) * pocetRiadkovZaznamu);
        AlokujPamet(pZaznam, riadkyZoznamuMaxSize, pocetRiadkovZaznamu);
        pTable[i] = pZaznam;
    }
    return pTable;
}

//==============================================================================
// dealokacia po nacitani suboru a vytvoreni dynamickeho zoznamu
//==============================================================================
void Dealokuj(void** pTable, int pocetZaznamov)
{
    if (pTable != 0)
    {
        int i;
        for (i = 0; i < pocetZaznamov; i++)
        {
            DealokujPamet(/*zaznam*/ pTable[i], R8);
        }
        free(pTable);
    }
}

//==============================================================================
//
//==============================================================================
void AlokujPamet(char** pRetazec, int* velkost, int pocetRiadkovZaznamu)
{
    int i;
    for (i = 0; i < pocetRiadkovZaznamu; i++)
    {
        pRetazec[i] = malloc(sizeof(char) * velkost[i]);
    }
}

//==============================================================================
//
//==============================================================================
void DealokujPamet(char** pRetazec, int pocetRiadkov)
{
    int i;
    for (i = 0; i < pocetRiadkov; i++)
    {
        if (pRetazec != 0)
        {
            free(pRetazec[i]);
        }
    }
}

//=============================================================================.=
// end of file
//=============================================================================.=
