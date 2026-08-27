/**
 * @file ZdarzenieMeczowe.cpp
 * @brief Implementacja hierarchii klas zdarzen meczowych.
 *
 * Plik zawiera logike wykonywania (metoda wirtualna wykonaj) roznych zdarzen
 * zglaszanych podczas trwania meczu. To w tym miejscu obliczane sa
 * szanse na strzelenie gola (uwzgledniajace statystyki zawodnikow) czy
 * skutki otrzymania czerwonej kartki.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "ZdarzenieMeczowe.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
// ZdarzenieMeczowe (klasa bazowa)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy bazowe wlasciwosci zdarzenia.
 * @param minuta Minuta, w ktorej doszlo do zdarzenia (1-120).
 * @param druzyna Nazwa druzyny bioracej udzial w zdarzeniu.
 * @param zawodnik Nazwisko glownego aktora zdarzenia.
 * @throws ZdarzenieWyjatek Jesli minuta meczu jest mniejsza niz 1 lub wieksza niz 120.
 */
ZdarzenieMeczowe::ZdarzenieMeczowe(int minuta, const std::string& druzyna,
                                   const std::string& zawodnik)
    : m_minuta(minuta), m_druzyna(druzyna), m_zawodnik(zawodnik),
    m_efekt(EfektZdarzenia::BRAK)
{
    if (minuta < 1 || minuta > 120)
        throw ZdarzenieWyjatek("Minuta meczu musi byc w zakresie 1-120. Otrzymano: " +
                               std::to_string(minuta));
}

/** @brief Zwraca minute zdarzenia. */
int            ZdarzenieMeczowe::getMinuta()   const { return m_minuta; }

/** @brief Zwraca nazwe druzyny powiazanej ze zdarzeniem. */
std::string    ZdarzenieMeczowe::getDruzyna()  const { return m_druzyna; }

/** @brief Zwraca nazwisko glownego zawodnika bioracego udzial w akcji. */
std::string    ZdarzenieMeczowe::getZawodnik() const { return m_zawodnik; }

/** @brief Zwraca enum EfektZdarzenia, informujacy silnik np. o zdobyciu gola. */
EfektZdarzenia ZdarzenieMeczowe::getEfekt()    const { return m_efekt; }

/**
 * @brief Metoda wypisujaca sformatowany tekst zdarzenia do konsoli.
 * Wykorzystuje polimorficzna metode getTypZdarzenia() i wewnetrzny opis m_opis.
 */
void ZdarzenieMeczowe::wyswietl() const {
    std::cout << "  " << std::setw(3) << m_minuta << "' ["
              << std::left << std::setw(12) << getTypZdarzenia() << "] "
              << m_opis << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Strzal
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor dla zdarzenia Strzal.
 * @param minuta Minuta meczu.
 * @param druzyna Druzyna atakujaca.
 * @param strzelec Zawodnik oddajacy strzal.
 * @param ocenaStrzalu OVR strzelajacego (wplywa na szanse gola).
 * @param ocenaBramkarza OVR broniacego (wplywa na obrone).
 * @param czyGospodarz Czy atakuje gospodarz (dla prawidlowego przypisania punktu).
 */
Strzal::Strzal(int minuta, const std::string& druzyna, const std::string& strzelec,
               int ocenaStrzalu, int ocenaBramkarza, bool czyGospodarz)
    : ZdarzenieMeczowe(minuta, druzyna, strzelec),
    m_ocenaStrzalu(ocenaStrzalu), m_ocenaBramkarza(ocenaBramkarza),
    m_czyGospodarz(czyGospodarz), m_czyGol(false)
{}

/**
 * @brief Kalkuluje wynik strzalu opierajac sie o roznice ocen i czynnik losowy.
 * @return Tekstowy opis akcji ("GOL!" lub pudlo).
 */
std::string Strzal::wykonaj() {
    // Prawdopodobienstwo gola: atak vs obrona + losowos (20-50%)
    int prog = m_ocenaStrzalu - m_ocenaBramkarza + 50; // bazowo 50
    prog = std::max(10, std::min(80, prog));            // zakres 10-80
    m_czyGol = (std::rand() % 100) < prog / 4;         // ~12-20% szans per strzal

    if (m_czyGol) {
        m_efekt = m_czyGospodarz ? EfektZdarzenia::GOL_GOSPODARZ
                                 : EfektZdarzenia::GOL_GOSC;
        m_opis = "GOL! " + m_zawodnik + " (" + m_druzyna + ") trafia do siatki!";
    } else {
        m_efekt = EfektZdarzenia::BRAK;
        int los = std::rand() % 3;
        if (los == 0)      m_opis = m_zawodnik + " strzela obok!";
        else if (los == 1) m_opis = m_zawodnik + " strzela w poprzeczke!";
        else               m_opis = m_zawodnik + " – bramkarz interweniuje!";
    }
    return m_opis;
}

// ─────────────────────────────────────────────────────────────────────────────
// Faul
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor zdarzenia Faul.
 * @param minuta Minuta meczu.
 * @param druzyna Druzyna faulujaca.
 * @param sprawca Nazwisko faulujacego zawodnika.
 * @param poszkodowany Nazwisko faulowanego gracza z druzyny przeciwnej.
 * @param ciezkosc Skala faulu od 1 (lekki) do 3 (ciezki).
 */
Faul::Faul(int minuta, const std::string& druzyna, const std::string& sprawca,
           const std::string& poszkodowany, int ciezkosc)
    : ZdarzenieMeczowe(minuta, druzyna, sprawca),
    m_poszkodowany(poszkodowany), m_ciezkosc(ciezkosc)
{}

/** @brief Generuje tekstowy opis faulu zalezny od jego ciezkosci. */
std::string Faul::wykonaj() {
    std::string opisCiezkosci;
    switch (m_ciezkosc) {
    case 1:  opisCiezkosci = "lekki";  break;
    case 2:  opisCiezkosci = "sredni"; break;
    default: opisCiezkosci = "ciezki"; break;
    }
    m_efekt = EfektZdarzenia::BRAK;
    m_opis  = m_zawodnik + " fauluje " + m_poszkodowany +
             " (" + opisCiezkosci + " faul)";
    return m_opis;
}

// ─────────────────────────────────────────────────────────────────────────────
// Kartonik
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor zdarzenia przyznania kartki (zolta lub czerwona).
 * @param minuta Minuta meczu.
 * @param druzyna Druzyna ukaranego zawodnika.
 * @param zawodnik Nazwisko ukaranego gracza.
 * @param kolor Enum: ZOLTY lub CZERWONY.
 */
Kartonik::Kartonik(int minuta, const std::string& druzyna,
                   const std::string& zawodnik, Kolor kolor)
    : ZdarzenieMeczowe(minuta, druzyna, zawodnik), m_kolor(kolor)
{}

/**
 * @brief Przypisuje odpowiedni EfektZdarzenia (zolty/czerwony)
 * i buduje opis sytuacyjny.
 */
std::string Kartonik::wykonaj() {
    if (m_kolor == Kolor::CZERWONY) {
        m_efekt = EfektZdarzenia::CZERWONA_KARTKA;
        m_opis  = m_zawodnik + " otrzymuje CZERWONA kartke! Wykluczenie z gry.";
    } else {
        m_efekt = EfektZdarzenia::ZOLTA_KARTKA;
        m_opis  = m_zawodnik + " otrzymuje zolta kartke.";
    }
    return m_opis;
}

// ─────────────────────────────────────────────────────────────────────────────
// RzutRozny
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor dla stalego fragmentu gry: Rzut Rozny.
 * @param minuta Minuta spotkania.
 * @param druzyna Druzyna wykonujaca rzut.
 * @param zawodnik Nazwisko wykonawcy.
 * @param czyGospodarz Flaga informujaca czy atakuje gospodarz.
 * @param ocenaWykonania OVR wykonujacego rzut zawodnika.
 */
RzutRozny::RzutRozny(int minuta, const std::string& druzyna,
                     const std::string& zawodnik, bool czyGospodarz, int ocenaWykonania)
    : ZdarzenieMeczowe(minuta, druzyna, zawodnik),
    m_czyGospodarz(czyGospodarz), m_ocenaWykonania(ocenaWykonania), m_czyGol(false)
{}

/**
 * @brief Algorytm przeliczajacy prawdopodobienstwo zdobycia bezposredniego gola z rzutu roznego.
 * @return Wynik sytuacji w postaci stringa ("BRAMKA!" lub zablokowanie).
 */
std::string RzutRozny::wykonaj() {
    // Rzut rozny to ok. 5% szans na gola (wazony ocena wykonania)
    int prog = m_ocenaWykonania / 10; // 1-10% zalezne od oceny
    m_czyGol = (std::rand() % 100) < prog;

    if (m_czyGol) {
        m_efekt = m_czyGospodarz ? EfektZdarzenia::GOL_GOSPODARZ
                                 : EfektZdarzenia::GOL_GOSC;
        m_opis  = "BRAMKA Z RZUTU ROZNEGO! " + m_zawodnik + " (" + m_druzyna + ")";
    } else {
        m_efekt = EfektZdarzenia::BRAK;
        m_opis  = "Rzut rozny " + m_zawodnik + " – pilka zablokowana.";
    }
    return m_opis;
}

// ─────────────────────────────────────────────────────────────────────────────
// Zmiana
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor zdarzenia technicznego: Zmiana zawodnikow.
 * @param minuta Minuta spotkania.
 * @param druzyna Druzyna przeprowadzajaca zmiane.
 * @param wychodzacy Nazwisko gracza opuszczajacego boisko.
 * @param wchodzacy Nazwisko zawodnika rezerwowego.
 */
Zmiana::Zmiana(int minuta, const std::string& druzyna,
               const std::string& wychodzacy, const std::string& wchodzacy)
    : ZdarzenieMeczowe(minuta, druzyna, wychodzacy),
    m_wychodzacy(wychodzacy), m_wchodzacy(wchodzacy)
{}

/** @brief Rejestruje zmiane zawodnikow. Zwraca sformatowany tekst wchodzacy/wychodzacy. */
std::string Zmiana::wykonaj() {
    m_efekt = EfektZdarzenia::ZMIANA_ZAWODNIKA;
    m_opis  = "Zmiana: " + m_wychodzacy + " -> " + m_wchodzacy +
             " (" + m_druzyna + ")";
    return m_opis;
}