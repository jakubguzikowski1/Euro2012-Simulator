/**
 * @file Mecz.cpp
 * @brief Implementacja klasy Mecz – petla meczowa i symulacja.
 *
 * Glowna metoda symuluj() iteruje przez 90 minut meczu, dla kazdej
 * minuty losuje typ zdarzenia, tworzy polimorficzny obiekt pochodny
 * klasy ZdarzenieMeczowe (dynamicznie przez operator new),
 * wywoluje wykonaj() i przetwarza efekt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "Mecz.h"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>

// ─────────────────────────────────────────────────────────────────────────────
// Konstruktor i Destruktor
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy mecz miedzy dwiema druzynami.
 *
 * Przed utworzeniem meczu przeprowadzana jest walidacja obu druzyn
 * (czy posiadaja wymagana minimalna liczbe zawodnikow).
 *
 * @param gospodarz Referencja do druzyny grajacej jako gospodarz.
 * @param gosc Referencja do druzyny grajacej jako gosc.
 * @throws MeczWyjatek Jesli ktora z druzyn nie przejdzie walidacji.
 */
Mecz::Mecz(Druzyna& gospodarz, Druzyna& gosc)
    : m_gospodarz(gospodarz), m_gosc(gosc),
    m_bramkiGospodarza(0), m_bramkiGoscia(0), m_rozegrany(false)
{
    // Walidacja druzyn przed meczem
    try { m_gospodarz.waliduj(); }
    catch (const DrużynaWyjatek& e) {
        throw MeczWyjatek(std::string("Blad druzyny domowej: ") + e.what());
    }
    try { m_gosc.waliduj(); }
    catch (const DrużynaWyjatek& e) {
        throw MeczWyjatek(std::string("Blad druzyny goscinnej: ") + e.what());
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Gettery i wlasciwosci meczu
// ─────────────────────────────────────────────────────────────────────────────

/** @brief Zwraca liczbe bramek strzelonych przez gospodarza. */
int      Mecz::getBramkiGospodarza() const { return m_bramkiGospodarza; }

/** @brief Zwraca liczbe bramek strzelonych przez goscia. */
int      Mecz::getBramkiGoscia()     const { return m_bramkiGoscia; }

/** @brief Sprawdza, czy mecz zostal juz zasymulowany. */
bool     Mecz::czyRozegrany()        const { return m_rozegrany; }

/** @brief Sprawdza, czy mecz zakonczyl sie remisem. */
bool     Mecz::czyRemis()            const { return m_rozegrany && (m_bramkiGospodarza == m_bramkiGoscia); }

/** @brief Zwraca stala referencje do druzyny gospodarzy. */
const Druzyna& Mecz::getGospodarz()  const { return m_gospodarz; }

/** @brief Zwraca stala referencje do druzyny gosci. */
const Druzyna& Mecz::getGosc()       const { return m_gosc; }

/** @brief Zwraca liste wszystkich zdarzen wygenerowanych podczas meczu. */
const std::vector<std::unique_ptr<ZdarzenieMeczowe>>& Mecz::getZdarzenia() const {
    return m_zdarzenia;
}

/**
 * @brief Zwraca wskaznik na druzyne wygrana.
 * @return Wskaznik na zwyciezce lub nullptr w przypadku remisu / braku rozegrania.
 */
Druzyna* Mecz::getZwyciezca() {
    if (!m_rozegrany || czyRemis()) return nullptr;
    return (m_bramkiGospodarza > m_bramkiGoscia) ? &m_gospodarz : &m_gosc;
}

// ─────────────────────────────────────────────────────────────────────────────
// Petla meczowa (glowna metoda)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Symuluje przebieg 90-minutowego spotkania krok po kroku.
 *
 * Co minute losowana jest szansa na wystapienie zdarzenia. Prawdopodobienstwo
 * ataku danej druzyny zalezy od jej sily ofensywnej. Nastepnie wybierany jest
 * typ zdarzenia (Strzal, Faul, Rozny, Kartonik, Zmiana) i tworzony jest
 * odpowiedni polimorficzny obiekt. Metoda aktualizuje rowniez kondycje zawodnikow.
 *
 * @throws MeczWyjatek Jesli mecz zostal juz wczesniej rozegrany.
 */
void Mecz::symuluj() {
    if (m_rozegrany)
        throw MeczWyjatek("Mecz " + m_gospodarz.getNazwa() + " vs " +
                          m_gosc.getNazwa() + " zostal juz rozegrany.");

    int zmGospodarz = 0, zmGosc = 0;  // max 3 zmiany na druzyne

    // Sila druzyn wplywa na possesje i liczbe wydarzen ofensywnych
    int silaAtGos = m_gospodarz.getSilaAtaku();
    int silaAtGsc = m_gosc.getSilaAtaku();

    for (int minuta = 1; minuta <= 90; minuta++) {
        // ~35% szans na zdarzenie w danej minucie
        if (std::rand() % 100 >= 35) continue;

        // Kto atakuje? Wazony losowy wybor (lepsza druzyna atakuje czesciej)
        int progGospodarz = 50 + (silaAtGos - silaAtGsc) / 4;
        progGospodarz = std::max(20, std::min(80, progGospodarz));
        bool czyGospodarz = (std::rand() % 100) < progGospodarz;

        // Losowanie typu zdarzenia
        int los = std::rand() % 100;

        if (los < 38) {
            // 38% – Strzal
            generujStrzal(minuta, czyGospodarz);
        } else if (los < 60) {
            // 22% – Faul
            generujFaul(minuta, !czyGospodarz);
        } else if (los < 78) {
            // 18% – Rzut rozny
            generujRzutRozny(minuta, czyGospodarz);
        } else if (los < 90) {
            // 12% – Kartonik
            generujKartonik(minuta, !czyGospodarz);
        } else {
            // 10% – Zmiana (od 55 minuty)
            if (minuta >= 55)
                generujZmiane(minuta, czyGospodarz, zmGospodarz, zmGosc);
        }

        // Co minute każdy zawodnik traci 1 punkt kondycji
        for (int i = 0; i < m_gospodarz.getLiczbaZawodnikow(); i++)
            m_gospodarz.getZawodnik(i)->zmniejszKondycje(1);
        for (int i = 0; i < m_gosc.getLiczbaZawodnikow(); i++)
            m_gosc.getZawodnik(i)->zmniejszKondycje(1);
    }

    m_rozegrany = true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Metody pomocnicze i generatory zdarzen
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Losuje dowolnego zawodnika z podanej druzyny.
 * @param druzyna Druzyna, z ktorej wybieramy zawodnika.
 * @return Zlaczone imie i nazwisko wylosowanego zawodnika.
 */
std::string Mecz::losujZawodnika(const Druzyna& druzyna) {
    int n = druzyna.getLiczbaZawodnikow();
    if (n == 0) return "Nieznany";
    int idx = std::rand() % n;
    return druzyna.getZawodnik(idx)->getPelneNazwisko();
}

/**
 * @brief Przetwarza skutek wygenerowanego zdarzenia.
 * Funkcja inkrementuje wynik punktowy na podstawie zwroconego efektu.
 * @param efekt Enum okreslajacy rodzaj efektu (np. gol dla gospodarzy).
 */
void Mecz::przetworzEfekt(EfektZdarzenia efekt) {
    switch (efekt) {
    case EfektZdarzenia::GOL_GOSPODARZ:   m_bramkiGospodarza++; break;
    case EfektZdarzenia::GOL_GOSC:        m_bramkiGoscia++;     break;
    default: break;
    }
}

/**
 * @brief Tworzy i wykonuje obiekt zdarzenia Strzal.
 * @param minuta Minuta spotkania.
 * @param czyGospodarz Flaga okreslajaca, czy akcje przeprowadza gospodarz.
 */
void Mecz::generujStrzal(int minuta, bool czyGospodarz) {
    Druzyna& atakujaca  = czyGospodarz ? m_gospodarz : m_gosc;
    Druzyna& broniaca   = czyGospodarz ? m_gosc      : m_gospodarz;

    Zawodnik* strzelec  = atakujaca.getNajlepszyAtakujacy();
    Zawodnik* bramkarz  = broniaca.getBramkarz();

    int ocenaStrzalu   = strzelec ? strzelec->getOgolnaOcena() : 60;
    int ocenaBramkarza = bramkarz ? bramkarz->getOgolnaOcena() : 60;
    std::string nazwaStrzelca = strzelec ? strzelec->getPelneNazwisko() : losujZawodnika(atakujaca);

    auto zdarzenie = std::make_unique<Strzal>(
        minuta, atakujaca.getNazwa(), nazwaStrzelca,
        ocenaStrzalu, ocenaBramkarza, czyGospodarz);

    zdarzenie->wykonaj();  // polimorficzne wywolanie

    if (zdarzenie->czyGol() && strzelec)
        strzelec->dodajGola();

    przetworzEfekt(zdarzenie->getEfekt());
    m_zdarzenia.push_back(std::move(zdarzenie));
}

/**
 * @brief Tworzy i wykonuje obiekt zdarzenia Faul.
 * @param minuta Minuta spotkania.
 * @param czyGospodarz Flaga okreslajaca, czy faul popelnia gospodarz.
 */
void Mecz::generujFaul(int minuta, bool czyGospodarz) {
    Druzyna& sprawca_dr  = czyGospodarz ? m_gospodarz : m_gosc;
    Druzyna& ofiaraDr    = czyGospodarz ? m_gosc      : m_gospodarz;

    std::string sprawca     = losujZawodnika(sprawca_dr);
    std::string poszkodowany = losujZawodnika(ofiaraDr);
    int ciezkosc = 1 + std::rand() % 3;

    auto zdarzenie = std::make_unique<Faul>(
        minuta, sprawca_dr.getNazwa(), sprawca, poszkodowany, ciezkosc);

    zdarzenie->wykonaj();  // polimorficzne wywolanie
    przetworzEfekt(zdarzenie->getEfekt());
    m_zdarzenia.push_back(std::move(zdarzenie));
}

/**
 * @brief Tworzy i wykonuje obiekt zdarzenia Kartonik (zolta/czerwona kartka).
 * @param minuta Minuta spotkania.
 * @param czyGospodarz Flaga okreslajaca druzyne ukarana kartka.
 */
void Mecz::generujKartonik(int minuta, bool czyGospodarz) {
    Druzyna& druzyna = czyGospodarz ? m_gospodarz : m_gosc;
    std::string zawodnik = losujZawodnika(druzyna);

    // 10% szans na czerwona kartke
    Kartonik::Kolor kolor = (std::rand() % 10 == 0)
                                ? Kartonik::Kolor::CZERWONY
                                : Kartonik::Kolor::ZOLTY;

    auto zdarzenie = std::make_unique<Kartonik>(
        minuta, druzyna.getNazwa(), zawodnik, kolor);

    zdarzenie->wykonaj();  // polimorficzne wywolanie
    przetworzEfekt(zdarzenie->getEfekt());
    m_zdarzenia.push_back(std::move(zdarzenie));
}

/**
 * @brief Tworzy i wykonuje obiekt zdarzenia RzutRozny.
 * @param minuta Minuta spotkania.
 * @param czyGospodarz Flaga okreslajaca druzyne wykonujaca rzut rozny.
 */
void Mecz::generujRzutRozny(int minuta, bool czyGospodarz) {
    Druzyna& druzyna = czyGospodarz ? m_gospodarz : m_gosc;
    Zawodnik* wykonawca = druzyna.getNajlepszyAtakujacy();
    std::string nazwaWyk = wykonawca ? wykonawca->getPelneNazwisko() : losujZawodnika(druzyna);
    int ocena = wykonawca ? wykonawca->getOgolnaOcena() : 60;

    auto zdarzenie = std::make_unique<RzutRozny>(
        minuta, druzyna.getNazwa(), nazwaWyk, czyGospodarz, ocena);

    zdarzenie->wykonaj();  // polimorficzne wywolanie

    if (zdarzenie->czyGol() && wykonawca)
        wykonawca->dodajGola();

    przetworzEfekt(zdarzenie->getEfekt());
    m_zdarzenia.push_back(std::move(zdarzenie));
}

/**
 * @brief Tworzy i wykonuje obiekt zdarzenia Zmiana.
 * Sprawdza dostepnosc limitu zmian i podmienia zawodnikow na boisku.
 * @param minuta Minuta spotkania.
 * @param czyGospodarz Flaga okreslajaca druzyne dokonujaca zmiany.
 * @param zmGospodarz Referencja do licznika zmian gospodarzy.
 * @param zmGosc Referencja do licznika zmian gosci.
 */
void Mecz::generujZmiane(int minuta, bool czyGospodarz, int& zmGospodarz, int& zmGosc) {
    int& licznikZmian = czyGospodarz ? zmGospodarz : zmGosc;
    if (licznikZmian >= 3) return;

    Druzyna& druzyna = czyGospodarz ? m_gospodarz : m_gosc;
    int n = druzyna.getLiczbaZawodnikow();
    if (n < 2) return;

    std::string wychodzacy = druzyna.getZawodnik(std::rand() % n)->getPelneNazwisko();
    std::string wchodzacy  = druzyna.getZawodnik(std::rand() % n)->getPelneNazwisko();

    auto zdarzenie = std::make_unique<Zmiana>(
        minuta, druzyna.getNazwa(), wychodzacy, wchodzacy);

    zdarzenie->wykonaj();  // polimorficzne wywolanie
    przetworzEfekt(zdarzenie->getEfekt());
    m_zdarzenia.push_back(std::move(zdarzenie));
    licznikZmian++;
}

/**
 * @brief Wypisuje sformatowany raport meczowy w konsoli.
 * Prezentuje koncowy wynik punktowy oraz liste wszystkich
 * zdarzen meczowych, delegujac wypisywanie do metody wyswietl()
 * kazdego polimorficznego obiektu ZdarzenieMeczowe.
 */
void Mecz::wyswietlRaport() const {
    std::cout << "\n  " << std::left << std::setw(18) << m_gospodarz.getNazwa()
    << " " << m_bramkiGospodarza << " : " << m_bramkiGoscia
    << " " << m_gosc.getNazwa() << "\n";

    if (!m_zdarzenia.empty()) {
        std::cout << "  ─────────────────────────────\n";
        for (const auto& zd : m_zdarzenia)
            zd->wyswietl();  // polimorficznie: wyswietl() ZdarzeniaMeczowego
    }
}