/**
 * @file CzlonekSztabu.cpp
 * @brief Implementacja hierarchii klas sztabu szkoleniowego.
 *
 * Plik zawiera implementacje metod klasy bazowej CzlonekSztabu oraz
 * wszystkich klas po niej dziedziczacych, definiujac ich unikalne zachowania (polimorfizm).
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "CzlonekSztabu.h"
#include <iostream>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// CzlonekSztabu (klasa bazowa)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy wspolne atrybuty dla wszystkich czlonkow sztabu.
 * @param imie Imie czlonka sztabu.
 * @param nazwisko Nazwisko czlonka sztabu.
 * @param wiek Wiek w latach.
 * @param doswiadczenie Lata doswiadczenia w zawodzie.
 * @throws ZawodnikWyjatek Jesli imie lub nazwisko sa puste, albo wiek jest poza zakresem 18-80.
 */
CzlonekSztabu::CzlonekSztabu(const std::string& imie, const std::string& nazwisko,
                             int wiek, int doswiadczenie)
    : m_imie(imie), m_nazwisko(nazwisko), m_wiek(wiek), m_doswiadczenie(doswiadczenie)
{
    if (imie.empty() || nazwisko.empty())
        throw ZawodnikWyjatek("Imie i nazwisko czlonka sztabu nie moga byc puste.");
    if (wiek < 18 || wiek > 80)
        throw ZawodnikWyjatek("Wiek czlonka sztabu musi byc w zakresie 18-80.");
}

/** @brief Zwraca imie czlonka sztabu. */
std::string CzlonekSztabu::getImie()          const { return m_imie; }

/** @brief Zwraca nazwisko czlonka sztabu. */
std::string CzlonekSztabu::getNazwisko()      const { return m_nazwisko; }

/** @brief Zwraca zlaczone imie i nazwisko. */
std::string CzlonekSztabu::getPelneNazwisko() const { return m_imie + " " + m_nazwisko; }

/** @brief Zwraca wiek czlonka sztabu. */
int         CzlonekSztabu::getWiek()          const { return m_wiek; }

/** @brief Zwraca lata doswiadczenia. */
int         CzlonekSztabu::getDoswiadczenie() const { return m_doswiadczenie; }

/**
 * @brief Wyswietla w konsoli podstawowe dane czlonka sztabu.
 * Formatuje wyjscie, dodajac role (pobierana polimorficznie).
 */
void CzlonekSztabu::wyswietl() const {
    std::cout << "  [" << getRola() << "] " << getPelneNazwisko()
    << " (wiek: " << m_wiek << ", dosw.: " << m_doswiadczenie << " lat)\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// TrenerGlowny
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy glownego trenera.
 * @param imie Imie trenera.
 * @param nazwisko Nazwisko trenera.
 * @param wiek Wiek trenera.
 * @param dosw Lata doswiadczenia.
 * @param formacja Preferowana formacja (np. "4-4-2").
 * @param strategia Preferowana strategia (np. "Ofensywa").
 */
TrenerGlowny::TrenerGlowny(const std::string& imie, const std::string& nazwisko,
                           int wiek, int dosw,
                           const std::string& formacja, const std::string& strategia)
    : CzlonekSztabu(imie, nazwisko, wiek, dosw),
    m_formacja(formacja), m_strategia(strategia)
{}

/**
 * @brief Wykonuje akcje charakterystyczna dla trenera glownego.
 * @return Opis ustawiania formacji i strategii w postaci stringa.
 */
std::string TrenerGlowny::wykonajAkcje() const {
    return getPelneNazwisko() + " ustawia druzyno w formacji " +
           m_formacja + " ze strategia: " + m_strategia;
}

// ─────────────────────────────────────────────────────────────────────────────
// Asystent
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy asystenta trenera.
 * @param imie Imie asystenta.
 * @param nazwisko Nazwisko asystenta.
 * @param wiek Wiek asystenta.
 * @param dosw Lata doswiadczenia.
 * @param specjalizacja Obszar specjalizacji (np. "Trening bramkarzy").
 */
Asystent::Asystent(const std::string& imie, const std::string& nazwisko,
                   int wiek, int dosw, const std::string& specjalizacja)
    : CzlonekSztabu(imie, nazwisko, wiek, dosw), m_specjalizacja(specjalizacja)
{}

/**
 * @brief Wykonuje akcje charakterystyczna dla asystenta.
 * @return Opis analizy ustawienia w oparciu o specjalizacje.
 */
std::string Asystent::wykonajAkcje() const {
    return getPelneNazwisko() + " analizuje ustawienie druzyny (spec.: " +
           m_specjalizacja + ")";
}

// ─────────────────────────────────────────────────────────────────────────────
// Lekarz
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy lekarza druzyny.
 * @param imie Imie lekarza.
 * @param nazwisko Nazwisko lekarza.
 * @param wiek Wiek lekarza.
 * @param dosw Lata doswiadczenia.
 * @param specjalizacjaMed Specjalizacja medyczna (np. "Ortopeda").
 */
Lekarz::Lekarz(const std::string& imie, const std::string& nazwisko,
               int wiek, int dosw, const std::string& specjalizacjaMed)
    : CzlonekSztabu(imie, nazwisko, wiek, dosw), m_specjalizacjaMed(specjalizacjaMed)
{}

/**
 * @brief Wykonuje akcje charakterystyczna dla lekarza.
 * @return Opis interwencji medycznej wobec kontuzjowanego zawodnika.
 */
std::string Lekarz::wykonajAkcje() const {
    return getPelneNazwisko() + " bada kontuzjowanego zawodnika (" +
           m_specjalizacjaMed + ")";
}

// ─────────────────────────────────────────────────────────────────────────────
// Fizjoterapeuta
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy fizjoterapeute.
 * @param imie Imie fizjoterapeuty.
 * @param nazwisko Nazwisko fizjoterapeuty.
 * @param wiek Wiek fizjoterapeuty.
 * @param dosw Lata doswiadczenia.
 * @param certyfikat Rodzaj posiadanego certyfikatu.
 */
Fizjoterapeuta::Fizjoterapeuta(const std::string& imie, const std::string& nazwisko,
                               int wiek, int dosw, const std::string& certyfikat)
    : CzlonekSztabu(imie, nazwisko, wiek, dosw), m_certyfikat(certyfikat)
{}

/**
 * @brief Wykonuje akcje charakterystyczna dla fizjoterapeuty.
 * @return Opis przeprowadzania stretchingu lub rozgrzewki.
 */
std::string Fizjoterapeuta::wykonajAkcje() const {
    return getPelneNazwisko() + " przeprowadza stretching przed meczem (" +
           m_certyfikat + ")";
}

// ─────────────────────────────────────────────────────────────────────────────
// Skaut
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy skauta.
 * @param imie Imie skauta.
 * @param nazwisko Nazwisko skauta.
 * @param wiek Wiek skauta.
 * @param dosw Lata doswiadczenia.
 * @param region Region odpowiedzialnosci skauta (np. "Europa Wschodnia").
 */
Skaut::Skaut(const std::string& imie, const std::string& nazwisko,
             int wiek, int dosw, const std::string& region)
    : CzlonekSztabu(imie, nazwisko, wiek, dosw), m_region(region)
{}

/**
 * @brief Wykonuje akcje charakterystyczna dla skauta.
 * @return Opis obserwacji zawodnikow w wyznaczonym regionie.
 */
std::string Skaut::wykonajAkcje() const {
    return getPelneNazwisko() + " obserwuje zawodnikow w regionie: " + m_region;
}