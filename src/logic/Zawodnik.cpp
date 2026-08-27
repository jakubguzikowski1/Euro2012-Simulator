/**
 * @file Zawodnik.cpp
 * @brief Implementacja hierarchii klas zawodnikow.
 *
 * Plik zawiera definicje metod klasy bazowej Zawodnik, obejmujace
 * zarzadzanie podstawowymi statystykami (gole, kartki, kondycja).
 * Zawiera rowniez implementacje klas pochodnych, z ktorych kazda
 * ma swoj unikalny sposob wyliczania ogolnej oceny przydatnosci (OVR).
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "Zawodnik.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// Zawodnik (klasa bazowa)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Metoda pomocnicza walidujaca zakres statystyki pilkarskiej.
 * @param wartosc Wartosc sprawdzanej statystyki.
 * @param nazwa Nazwa statystyki (np. "szybkosc") uzywana w komunikacie o bledzie.
 * @throws ZawodnikWyjatek Jesli wartosc nie miesci sie w przedziale 1-99.
 */
void Zawodnik::walidujStatystyke(int wartosc, const std::string& nazwa) {
    if (wartosc < 1 || wartosc > 99)
        throw ZawodnikWyjatek("Statystyka '" + nazwa + "' musi byc w zakresie 1-99. Otrzymano: " + std::to_string(wartosc));
}

/**
 * @brief Konstruktor inicjalizujacy podstawowe statystyki zawodnika.
 * Domyslnie kondycja ustawiana jest na 100%, a konta goli i kartek sa zerowane.
 * @param imie Imie zawodnika.
 * @param nazwisko Nazwisko zawodnika.
 * @param numer Numer na koszulce (1-99).
 * @param wiek Wiek zawodnika.
 * @param szybkosc Statystyka szybkosci (1-99).
 * @param sila Statystyka sily fizycznej (1-99).
 * @throws ZawodnikWyjatek Jesli szybkosc lub sila sa poza zakresem 1-99.
 */
Zawodnik::Zawodnik(const std::string& imie, const std::string& nazwisko,
                   int numer, int wiek, int szybkosc, int sila)
    : m_imie(imie), m_nazwisko(nazwisko), m_numer(numer), m_wiek(wiek),
    m_kondycja(100), m_gole(0), m_zolteKartki(0), m_zawieszony(false)
{
    walidujStatystyke(szybkosc, "szybkosc");
    walidujStatystyke(sila,     "sila");
    m_szybkosc = szybkosc;
    m_sila     = sila;
}

/** @brief Zwraca imie zawodnika. */
std::string Zawodnik::getImie()           const { return m_imie; }

/** @brief Zwraca nazwisko zawodnika. */
std::string Zawodnik::getNazwisko()       const { return m_nazwisko; }

/** @brief Zwraca polaczone imie i nazwisko jako jeden string. */
std::string Zawodnik::getPelneNazwisko()  const { return m_imie + " " + m_nazwisko; }

/** @brief Zwraca numer zawodnika na koszulce. */
int         Zawodnik::getNumer()          const { return m_numer; }

/** @brief Zwraca wiek zawodnika. */
int         Zawodnik::getWiek()           const { return m_wiek; }

/** @brief Zwraca statystyke szybkosci. */
int         Zawodnik::getSzybkosc()       const { return m_szybkosc; }

/** @brief Zwraca statystyke sily. */
int         Zawodnik::getSila()           const { return m_sila; }

/** @brief Zwraca obecny poziom kondycji zawodnika (max 100). */
int         Zawodnik::getKondycja()       const { return m_kondycja; }

/** @brief Zwraca ilosc goli strzelonych w aktualnym turnieju. */
int         Zawodnik::getGole()           const { return m_gole; }

/** @brief Zwraca sume zoltych kartek. */
int         Zawodnik::getZolteKartki()    const { return m_zolteKartki; }

/** @brief Sprawdza, czy zawodnik ma czerwona kartke lub limit zoltych. */
bool        Zawodnik::isSuspended()       const { return m_zawieszony; }

/** @brief Inkrementuje licznik goli strzelonych przez zawodnika. */
void Zawodnik::dodajGola()             { m_gole++; }

/**
 * @brief Odejmuje punkty kondycji (zapobiegajac spadkowi ponizej zera).
 * @param o Wartosc, o jaka zmniejszona zostanie kondycja.
 */
void Zawodnik::zmniejszKondycje(int o) {
    m_kondycja = std::max(0, m_kondycja - o);
}

/**
 * @brief Dodaje zolta kartke.
 * Jesli zawodnik otrzyma dwie zolte kartki, zostaje automatycznie zawieszony.
 */
void Zawodnik::dodajZoltaKartke() {
    m_zolteKartki++;
    if (m_zolteKartki >= 2) m_zawieszony = true;
}

/** @brief Dodaje bezposrednia czerwona kartke (natychmiastowe zawieszenie). */
void Zawodnik::dodajCzerwonaKartke() {
    m_zawieszony = true;
}

/**
 * @brief Wypisuje w konsoli informacje o zawodniku (numer, imie, pozycja, OVR).
 * Wykorzystuje metody wirtualne getTyp() i getOgolnaOcena().
 */
void Zawodnik::wyswietl() const {
    std::cout << "  [" << std::setw(2) << m_numer << "] "
              << std::left << std::setw(22) << getPelneNazwisko()
              << std::setw(18) << getTyp()
              << " OVR:" << getOgolnaOcena() << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Bramkarz
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor specyficzny dla Bramkarza.
 * @param imie Imie bramkarza.
 * @param nazwisko Nazwisko bramkarza.
 * @param numer Numer koszulki.
 * @param wiek Wiek.
 * @param szybkosc Ogolna szybkosc.
 * @param sila Ogolna sila.
 * @param refleks Zdolnosc szybkiej reakcji na strzal (1-99).
 * @param graNaWyjsciach Skutecznosc podczas wychodzenia z pola karnego (1-99).
 */
Bramkarz::Bramkarz(const std::string& imie, const std::string& nazwisko,
                   int numer, int wiek, int szybkosc, int sila,
                   int refleks, int graNaWyjsciach)
    : Zawodnik(imie, nazwisko, numer, wiek, szybkosc, sila)
{
    walidujStatystyke(refleks,        "refleks");
    walidujStatystyke(graNaWyjsciach, "graNaWyjsciach");
    m_refleks        = refleks;
    m_graNaWyjsciach = graNaWyjsciach;
}

/** @brief Generuje napis symulujacy wirtualna akcje dla Bramkarza. */
std::string Bramkarz::wykonajAkcje() const {
    return getPelneNazwisko() + " wykonuje interwencje! (refleks: " +
           std::to_string(m_refleks) + ")";
}

/**
 * @brief Oblicza ogolna ocene (OVR) wazna dla Bramkarza.
 * Refleks: 60%, Gra na wyjsciach: 25%, Szybkosc: 15%.
 */
int Bramkarz::getOgolnaOcena() const {
    return (m_refleks * 60 + m_graNaWyjsciach * 25 + m_szybkosc * 15) / 100;
}

// ─────────────────────────────────────────────────────────────────────────────
// SrodkowyObronca
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor specyficzny dla Srodkowego Obroncy.
 * @param odbior Umiejetnosc zabrania pilki bez faulu (1-99).
 * @param graGlowa Skutecznosc pojedynkow powietrznych (1-99).
 */
SrodkowyObronca::SrodkowyObronca(const std::string& imie, const std::string& nazwisko,
                                 int numer, int wiek, int szybkosc, int sila,
                                 int odbior, int graGlowa)
    : Zawodnik(imie, nazwisko, numer, wiek, szybkosc, sila)
{
    walidujStatystyke(odbior,   "odbior");
    walidujStatystyke(graGlowa, "graGlowa");
    m_odbior   = odbior;
    m_graGlowa = graGlowa;
}

/** @brief Generuje napis symulujacy wirtualna akcje dla Srodkowego Obroncy. */
std::string SrodkowyObronca::wykonajAkcje() const {
    return getPelneNazwisko() + " przecina podanie! (odbior: " +
           std::to_string(m_odbior) + ")";
}

/**
 * @brief Oblicza ogolna ocene (OVR) wazna dla Srodkowego Obroncy.
 * Odbior: 40%, Gra glowa: 35%, Sila: 25%.
 */
int SrodkowyObronca::getOgolnaOcena() const {
    return (m_odbior * 40 + m_graGlowa * 35 + m_sila * 25) / 100;
}

// ─────────────────────────────────────────────────────────────────────────────
// BocznyObronca
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor specyficzny dla Bocznego Obroncy.
 * @param drybling Zdolnosc omijania przeciwnikow z pilka (1-99).
 * @param wytrzymalosc Zdolnosc ciaglego biegania wzdluz linii (1-99).
 */
BocznyObronca::BocznyObronca(const std::string& imie, const std::string& nazwisko,
                             int numer, int wiek, int szybkosc, int sila,
                             int drybling, int wytrzymalosc)
    : Zawodnik(imie, nazwisko, numer, wiek, szybkosc, sila)
{
    walidujStatystyke(drybling,      "drybling");
    walidujStatystyke(wytrzymalosc,  "wytrzymalosc");
    m_drybling      = drybling;
    m_wytrzymalosc  = wytrzymalosc;
}

/** @brief Generuje napis symulujacy wirtualna akcje dla Bocznego Obroncy. */
std::string BocznyObronca::wykonajAkcje() const {
    return getPelneNazwisko() + " wychodzi na skrzydlo! (drybling: " +
           std::to_string(m_drybling) + ")";
}

/**
 * @brief Oblicza ogolna ocene (OVR) wazna dla Bocznego Obroncy.
 * Szybkosc: 35%, Drybling: 35%, Wytrzymalosc: 30%.
 */
int BocznyObronca::getOgolnaOcena() const {
    return (m_szybkosc * 35 + m_drybling * 35 + m_wytrzymalosc * 30) / 100;
}

// ─────────────────────────────────────────────────────────────────────────────
// Pomocnik
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor specyficzny dla Pomocnika.
 * @param podanie Zdolnosc precyzyjnego transportowania pilki (1-99).
 * @param wizja Czytanie gry i przewidywanie ruchu formacji (1-99).
 */
Pomocnik::Pomocnik(const std::string& imie, const std::string& nazwisko,
                   int numer, int wiek, int szybkosc, int sila,
                   int podanie, int wizja)
    : Zawodnik(imie, nazwisko, numer, wiek, szybkosc, sila)
{
    walidujStatystyke(podanie, "podanie");
    walidujStatystyke(wizja,   "wizja");
    m_podanie = podanie;
    m_wizja   = wizja;
}

/** @brief Generuje napis symulujacy wirtualna akcje dla Pomocnika. */
std::string Pomocnik::wykonajAkcje() const {
    return getPelneNazwisko() + " rozgrywa pilke! (podanie: " +
           std::to_string(m_podanie) + ", wizja: " + std::to_string(m_wizja) + ")";
}

/**
 * @brief Oblicza ogolna ocene (OVR) wazna dla Pomocnika.
 * Podanie: 45%, Wizja: 35%, Szybkosc: 20%.
 */
int Pomocnik::getOgolnaOcena() const {
    return (m_podanie * 45 + m_wizja * 35 + m_szybkosc * 20) / 100;
}

// ─────────────────────────────────────────────────────────────────────────────
// Napastnik
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor specyficzny dla Napastnika.
 * @param strzelanie Zdolnosc i celnosc oddawania strzalow do bramki (1-99).
 * @param ustawienie Umiejetnosc znalezienia luki w obronie przeciwnika (1-99).
 */
Napastnik::Napastnik(const std::string& imie, const std::string& nazwisko,
                     int numer, int wiek, int szybkosc, int sila,
                     int strzelanie, int ustawienie)
    : Zawodnik(imie, nazwisko, numer, wiek, szybkosc, sila)
{
    walidujStatystyke(strzelanie,  "strzelanie");
    walidujStatystyke(ustawienie,  "ustawienie");
    m_strzelanie = strzelanie;
    m_ustawienie = ustawienie;
}

/** @brief Generuje napis symulujacy wirtualna akcje oddawania strzalu przez Napastnika. */
std::string Napastnik::wykonajAkcje() const {
    return getPelneNazwisko() + " oddaje strzal! (strzelanie: " +
           std::to_string(m_strzelanie) + ")";
}

/**
 * @brief Oblicza ogolna ocene (OVR) wazna dla Napastnika.
 * Strzelanie: 55%, Ustawienie: 30%, Szybkosc: 15%.
 */
int Napastnik::getOgolnaOcena() const {
    return (m_strzelanie * 55 + m_ustawienie * 30 + m_szybkosc * 15) / 100;
}