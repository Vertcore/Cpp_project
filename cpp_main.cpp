#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

/**
* @author Miłosz Gilga
* @date 2020-2021
* @section Description
* Projekt "Plan". Podstawy Programowania Komputerów.
* Informatyka, Wydział Elektryczny Politechniki Śląskiej.
* @copyright Copyright (c) 2021 by Miłosz Gilga.
*/

/**
* @brief Typ wyliczeniowy enum, przypisujący konkrentemu dniu tygodnia odpowiednią wartość int.
*/
enum DaysEnum {
    pn = 1,
    wt = 2,
    sr = 3,
    cz = 4,
    pt = 5,
    sb = 6,
    nd = 7
};

/**
* @brief Struktura główna, przechowująca zmienne z jednej linii razem ze zmienną Enum.
*/
struct Line {
    string hour;
    DaysEnum day;
    string group;
    string surname;
    string subject;
};

/**
* @brief Struktura przechowująca zmienne niezbędne do walidacji danych w pliku.
*/
struct Valid {
    int firstHourHint;
    int firstHourMint;
    int secondHourHint;
    int secondHourMint;
    int firstHourAllInt;
    int secondHourAllInt;
    string firstHourH;
    string firstHourM;
    string firstHourAll;
    string secondHourH;
    string secondHourM;
    string secondHourAll;
};

void readLine(ifstream& file, Line& line);
void readLineTab(ifstream& file, Line* lineTab, const int numOfLines);
int hourStringToInt(string& hour);
void globalSort(Line* lineTab, const int numOfLines);
void infoPrintInCmd(int i, int check, string& filePath);
void stringToIntValid(Valid& valid);
void lineCount(string& filePath, int& numOfLines);
int globalCheck(string& filePath, const int numOfLines, Valid& valid);
int runInCmd(int argc, char* argv[], string& filePath);
void genrFiles(Line* lineTab, int& numOfLines);

/**
* @brief Funkcja główna main sterująca programem i regulujące uruchamianie poszczególnych funkcji.
*
* @param argc - liczba argumentów podanych podczas wywoływania programu
* @param argv - tablica argumentów argc
*/
int main(int argc, char* argv[]) {
    /** Funkcja ustawiająca Polskie znaków w konsoli */
    setlocale(LC_CTYPE, "Polish");
    ifstream file;
    string filePath;
    Valid valid;
    Line line;
    /** Funkcja pozwalająca uruchamiać program z konsoli wiersza poleceń */
    if (runInCmd(argc, argv, filePath)) {
        return 0;
    }

    int numOfLines = 0;
    lineCount(filePath, numOfLines);

    Line* lineTab = new Line[numOfLines];
    /** Funkcja walidująca dane w pliku txt */
    if (globalCheck(filePath, numOfLines, valid)) {
        return 0;
    }
    /** Ponowne otwarcie pliku txt i podstawowa walidacja */
    file.open(filePath.c_str());
    if (file.fail()) {
        int check = 0, i = 0;
        infoPrintInCmd(i, check, filePath);
        file.close();
        return 1;
    }

    readLineTab(file, lineTab, numOfLines);
    globalSort(lineTab, numOfLines);
    genrFiles(lineTab, numOfLines);

    /** Końcowe czyszczenie pamięci (usuwanie tablicy i zamknięcie pliku) */
    delete[] lineTab;
    file.close();
    return 0;
}

/**
* @brief Funkcja wczytująca dane z jednej linii pliku txt
* 
* @note Głównym zadaniem funkcji jest wczytanie pojedyńczej linii i zapisania wartości do odpowiednich zmiennych.
* Funkcja nie sprawdza poprawności wczytywania danych.
* Dodatkowo funkcja przypisuje konretnemu dniu tygodnia wartość enum.
*
* @param file - odwołanie do pliku txt
* @param line - odwołanie się do struktury Line
*/
void readLine(ifstream& file, Line& line) {
    string dayWeek;
    /** Wczytywanie danych z pojedyńczej linii do zmiennych struktury */
    file >> line.hour >> dayWeek >> line.group >> line.surname >> line.subject;
    /** Konwersja dni tygodnia (string) i przypisywanie wartość enum zmiennej day ze struktury */
    if (dayWeek == "pn") line.day = pn;
    else if (dayWeek == "wt") line.day = wt;
    else if (dayWeek == "sr") line.day = sr;
    else if (dayWeek == "cz") line.day = cz;
    else if (dayWeek == "pt") line.day = pt;
    else if (dayWeek == "sb") line.day = sb;
    else if (dayWeek == "nd") line.day = nd;
}

/**
* @brief Funkcja wczytująca dane z wszystkich wierszy pliku txt
*
* Funkcja nie sprawdza poprawności wczytywania danych.
*
* @param lineTab - odwołanie do tablicy dynamicznej przez wskaźnik
* @param numOfLines - ilość wierszy w pliku txt w postaci wartości liczbowej
*/
void readLineTab(ifstream& file, Line* lineTab, const int numOfLines) {
    for (int i = 0; i < numOfLines; i++) {
        readLine(file, lineTab[i]);
    }
}

/**
* @brief Funkcja konwertująca godzinę rozpoczęcia na typ int
*
* Funkcja nie sprawdza poprawności wczytywania danych.
*
* @param hour - zmienna struktury określająca godzinę przekazywaną z funkcji globalSort
* @return hourIntAfter - zwrócenie godziny rozpoczęcia zapisanej w postaci liczby całkowitej
*/
int hourStringToInt(string& hour) {
    /** Wycięcie godziny i konkatenacja do jednej zmiennej */
    string hourCut = hour.substr(0, 2) + hour.substr(3, 5);
    /** Konwersja godziny w postaci string na wartośc typu int */
    stringstream hourInt(hourCut);
    int hourIntAfter;
    hourInt >> hourIntAfter;
    return hourIntAfter;
}

/**
* @brief Funkcja sortująca typu BubbleSort.
*
* Funkcja zapożyczona ze strony https://en.wikipedia.org/wiki/Bubble_sort,
* której kod został dostosowany do programu.
*
* @note Funkcja sortuje wg hierarchii: nazwisko, dzień tygodnia, godzina rozpoczęcia.
*
* @param lineTab - przekazanie tablicy dynamicznej z liniami pliku txt
* @param numOfLines - liczba linii w pliku txt
*/
void globalSort(Line* lineTab, const int numOfLines) {
    Line structL;
    for (int i = 0; i < numOfLines - 1; i++) {
        for (int j = 0; j < numOfLines - 1 - i; j++) {
            if ((lineTab[j].surname > lineTab[j + 1].surname) || (lineTab[j].surname == lineTab[j + 1].surname)
                && (lineTab[j].day > lineTab[j + 1].day) || (lineTab[j].day == lineTab[j + 1].day)
                && (hourStringToInt(lineTab[j].hour)) > (hourStringToInt(lineTab[j + 1].hour))) {
                structL = lineTab[j + 1];
                lineTab[j + 1] = lineTab[j];
                lineTab[j] = structL;
            }
        }
    }
}

/**
* @brief Funkcja drukująca na ekran odpowiednie komunikaty w konsoli.
* @note Funkcja jest ściśle połączona z funkcją globalCheck i runAsCmd.
*
* @param i - iterator pętli z funkcji sprawdzającej globalCheck
* @param check - wartość typu intiger odpowiadająca za wybór wypisania odpowiedniego błędu
* @param filePath - ścieżka dostępu do pliku
*/
void infoPrintInCmd(int i, int check, string& filePath) {
    if (check == 0) {
        cerr << endl;
        cerr << "  ================= ERROR! ================" << endl;
        cerr << "  Nie udało się otworzyć pliku: " << filePath << endl;
        cerr << "  Plik jest uszkodzony lub nie istnieje. " << endl;
        cerr << "  =========================================" << endl;
    }
    else if (check == 1) {
        cerr << endl;
        cerr << "  ========================= ERROR! ==========================" << endl;
        cerr << "  Błąd wartości godzinowych w " << i + 1 << " wierszu!" << endl;
        cerr << "  Wartość godziny wykracza poza skalę!" << endl;
        cerr << "  ===========================================================" << endl;
    }
    else if (check == 2) {
        cerr << endl;
        cerr << "  ========================= ERROR! ==========================" << endl;
        cerr << "  Błąd wartości godzinowych w " << i + 1 << " wierszu!" << endl;
        cerr << "  Godzina rozpoczęcia jest późniejsza od godziny zakończenia!" << endl;
        cerr << "  ===========================================================" << endl;
    }
    else if (check == 3) {
        cerr << endl;
        cerr << "  ========================= ERROR! ==========================" << endl;
        cerr << "  Błąd w nazwie grupy w " << i + 1 << " wierszu!" << endl;
        cerr << "  Podana nazwa grupy nie istnieje!" << endl;
        cerr << "  ===========================================================" << endl;
    }
    else if (check == 4) {
        cerr << endl;
        cerr << "  ========================= ERROR! ==========================" << endl;
        cerr << "  Błąd w nazwach dni tygodnia w " << i + 1 << " wierszu!" << endl;
        cerr << "  Podana nazwa dnia tygodnia nie istnieje!" << endl;
        cerr << "  ===========================================================" << endl;
    }
    else if (check == 5) {
        cerr << endl;
        cerr << "  ================ ERROR! =================" << endl;
        cerr << "  Została podana zła wartość przełącznika." << endl;
        cerr << "  Prawidłowa wartość przełącznika to '-i'" << endl;
        cerr << "  =========================================" << endl;
    }
    else if (check == 6) {
        cout << endl;
        cout << "  ========================= INSTRUKCJA URUCHOMIENIA PROGRAMU ===========================" << endl;
        cout << "  Aby uruchomić program należy w poszczególnych krokach:" << endl;
        cout << "  1. Wpisać do konsoli przełącznik '-i'" << endl;
        cout << "  2. Po przełączniku umieścić spację a po niej nazwę pliku (w tym przypadku 'plik.txt')" << endl;
        cout << "  ======================================================================================" << endl;
    }
}

/**
* @brief Funkcja konwertująca string na int.
*
* @param valid - odniesienie do struktury ze zmiennymi niezbędnymi do walidacji
*/
void stringToIntValid(Valid& valid) {
    stringstream firstHourHcon(valid.firstHourH);
    firstHourHcon >> valid.firstHourHint;
    stringstream firstHourMcon(valid.firstHourM);
    firstHourMcon >> valid.firstHourMint;
    stringstream secondHourHcon(valid.secondHourH);
    secondHourHcon >> valid.secondHourHint;
    stringstream secondHourMcon(valid.secondHourM);
    secondHourMcon >> valid.secondHourMint;
    stringstream firstHourAcon(valid.firstHourAll);
    firstHourAcon >> valid.firstHourAllInt;
    stringstream secondHourAcon(valid.secondHourAll);
    secondHourAcon >> valid.secondHourAllInt;
}

/**
* @brief Funkcja zliczająca ilość wierszy.
*
* Funkcja posiada podstawową walidację odczytu pliku. 
* Jeśli plik nie istnieje, wyprowadza informację o błędzie.
*
* @param filePath - ścieżka do pliku txt
* @param numOfLines - ilość wierszy w pliku txt w postaci wartości liczbowej
*/
void lineCount(string& filePath, int& numOfLines) {
    ifstream file;
    file.open(filePath.c_str()); {
        if (file.fail()) {
            int check = 0, i = 0;
            infoPrintInCmd(i, check, filePath);
            file.close();
        }
        else {
            string line;
            while (getline(file, line)) {
                numOfLines++;
            }
            file.close();
        }
    }
}

/**
* @brief Funkcja walidująca dane z pliku txt.
*
* @note Funkcja rozpoczyna od walidacji ogólnej pliku.
* Następnie sprawdza godzinę, wzgędem tego, czy wartość minut i/lub godzin nie została przekroczona.
* Poźniej upewnia się, czy godzina zakończenia nie jest mniejsza od godziny rozpoczęcia.
* Następnie sprawdza, czy dzień tygodnia jest wpisany prawidłowo.
* Po czym waliduję nazwę grupy, jeżeli w pliku przypadkowo znalazła się literówka.
* Jeśli nie wykryto żadnych błędów, funkcja wyświetla w konsoli komunikat o poprawnej walidacji pliku.
*
* @param filePath - ścieżka do pliku txt
* @param numOfLines - ilość wierszy w pliku txt w postaci wartości liczbowej
* @param valid - odwołanie do struktury Valid
*
* @return - dla wartości 0, program wykonuje się dalej; dla wartości != 0 program wyświetla komunikat o błędzie i kończy działanie
*/
int globalCheck(string& filePath, const int numOfLines, Valid& valid) {
    ifstream file;
    file.open(filePath.c_str());
    if (file.fail()) {
        file.close();
        return 1;
    }
    else {
        for (int i = 0; i < numOfLines; i++) {
            string line;
            int check1 = 1, check2 = 2, check3 = 3, check4 = 4, check5 = 5;
            getline(file, line);
            /** Wycięcie i wpisanie do zmiennych wartości */
            valid.firstHourH = line.substr(0, 2);
            valid.firstHourM = line.substr(3, 2);
            valid.secondHourH = line.substr(6, 2);
            valid.secondHourM = line.substr(9, 2);
            string weekDay = line.substr(12, 2);
            string groupIndex = line.substr(15, 2);
            valid.firstHourAll = line.substr(0, 2) + line.substr(3, 2);
            valid.secondHourAll = line.substr(6, 2) + line.substr(9, 2);
            /** Zamiana godzin string na typ int */
            stringToIntValid(valid);
            /** Walidacja zakresu godziny i minuty */
            if ((valid.firstHourHint > 24 || valid.firstHourMint >= 60) 
                || (valid.secondHourHint > 24 || valid.secondHourMint >= 60)) {
                infoPrintInCmd(i, check1, filePath);
                return 2;
            }
            /** Walidacja zakresu między godziną rozpoczęcia a godziną zakończenia */
            else if (valid.secondHourAllInt - valid.firstHourAllInt <= 0) {
                infoPrintInCmd(i, check2, filePath);
                return 3;
            }
            /** Walidacja nazwy grupy */
            else if (groupIndex != "gr") {
                infoPrintInCmd(i, check3, filePath);
                return 4;
            }
            /** Walidacja dnia tygodnia */
            else if ((weekDay != "pn") && (weekDay != "wt") && (weekDay != "sr") &&
                (weekDay != "cz") && (weekDay != "pt") && (weekDay != "sb") && (weekDay != "nd")) {
                infoPrintInCmd(i, check4, filePath);
                return 5;
            }
        }
        cout << endl;
        cout << "  ==================== SUKCES! =====================" << endl;
        cout << "  Walidacja pliku wejściowego przebiegła pomyślnie." << endl;
        cout << "  Wygenerowane pliki znajdziesz w folderze projektu." << endl;
        cout << "  ==================================================" << endl;
        file.close();
        return 0;
    }
}

/**
* @brief Funkcja umożliwiająca uruchomienie programu z wiersza poleceń.
*
* @note Jeśli zostanie podanych zbyt mało argumentów lub ich brak, program wypisze instrukcję, jak go uruchomić.
* Jeśli argumenty zostaną podane prawidłowo, ale wartość przełącznika będzie inna od tej zadeklarowanej,
* program wypiszę instrukcję z prawidłową wartością przełącznika.
*
* @param argc - liczba argumentów podanych podczas wywoływania programu
* @param argv - tablica argumentów argc
* @param filePath - ścieżka dostępu do pliku
* @return - funkcja zwraca wartość 0, gdy argumenty zostaną podane prawidłowo.
* Dla wartości != 0 program wyświetla komunikat o błędzie i kończy działanie.
*/
int runInCmd(int argc, char* argv[], string& filePath) {
    if (argc == 3) {
        if (!string(argv[1]).compare("-i")) {
            filePath = argv[2];
            return 0;
        }
        else {
            int check = 5, i = 0;
            infoPrintInCmd(i, check, filePath);
            return 1;
        }
    }
    else {
        int check = 6, i = 0;
        infoPrintInCmd(i, check, filePath);
        return 2;
    }
}

/**
* @brief Funkcja generująca i zapełniająca końcowe pliki danymi
*
* @note Funkcja odpowiada za generowanie i wpisywanie danych do końcowego
* pliku txt. Przed wpisaniem zmiennej day ze struktury, konwertowana jest ona z enum
* na string i w tej postaci umieszczana jest w pliku końcowym.
*
* @param lineTab - tablica dynamiczna struktury
* @param numOfLines - liczba linii w pliku txt
*/
void genrFiles(Line* lineTab, int& numOfLines) {
    string surname = " ";
    ofstream outFiles;
    for (const Line* oneLine = lineTab; oneLine < lineTab + numOfLines; oneLine++) {
        /** Tworzenie pliku txt z nazwiskiem prowadzącego */
        if ((*oneLine).surname != surname) {
            if (outFiles.is_open()) {
                outFiles.close();
            }
            outFiles.open((*oneLine).surname + string(".txt"));
            surname = (*oneLine).surname;
        }
        /** Zamiana Enum na string (dni tygodnia) */
        string dayString;
        if ((*oneLine).day == 1) dayString = "pn";
        else if ((*oneLine).day == 2) dayString = "wt";
        else if ((*oneLine).day == 3) dayString = "sr";
        else if ((*oneLine).day == 4) dayString = "cz";
        else if ((*oneLine).day == 5) dayString = "pt";
        else if ((*oneLine).day == 6) dayString = "sb";
        else if ((*oneLine).day == 7) dayString = "nd";
        outFiles << (*oneLine).hour << " " << dayString << " " << (*oneLine).group << " " << (*oneLine).subject << endl;
    }
    outFiles.close();
}
