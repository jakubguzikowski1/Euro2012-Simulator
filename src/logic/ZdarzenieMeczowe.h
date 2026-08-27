/**
 * @file ZdarzenieMeczowe.h
 * @brief Drzewo dziedziczenia nr 2 – Zdarzenia Meczowe.
 *
 * Abstrakcyjna klasa bazowa ZdarzenieMeczowe oraz 5 klas pochodnych:
 * Strzal, Faul, Kartonik, RzutRozny, Zmiana.
 *
 * Metoda wykonaj() jest czysta wirtualna – polimorficzne wywolanie
 * w petli meczowej Mecz::symuluj() przetwarza kazde zdarzenie inaczej.
 *
 * UWAGA: Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Exceptions.h"
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Enum efektu zdarzenia (uzywany przez Mecz do aktualizacji stanu)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Efekt zdarzenia meczowego na stan meczu.
 */
enum class EfektZdarzenia {
    BRAK,              ///< Brak wplywu na wynik.
    GOL_GOSPODARZ,     ///< Bramka dla druzyny domowej.
    GOL_GOSC,          ///< Bramka dla druzyny gostki.
    ZOLTA_KARTKA,      ///< Zolta kartka.
    CZERWONA_KARTKA,   ///< Czerwona kartka + wykluczenie.
    ZMIANA_ZAWODNIKA   ///< Wejscie rezerwowego.
};

// ─────────────────────────────────────────────────────────────────────────────
// Klasa bazowa (abstrakcyjna)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Abstrakcyjna klasa bazowa dla zdarzen meczowych.
 *
 * Kazde zdarzenie zawiera minute meczu, nazwe druzyny i nazwe zawodnika.
 * Efekt zdarzenia jest ustawiany podczas wywolania wykonaj().
 */
class ZdarzenieMeczowe {
public:
    /**
     * @brief Konstruktor zdarzenia.
     * @param minuta    Minuta meczu, w ktorej nastapilo zdarzenie (1-120).
     * @param druzyna   Nazwa druzyny zwiazanej ze zdarzeniem.
     * @param zawodnik  Pelne nazwisko zawodnika (lub "").
     * @throws ZdarzenieWyjatek Jesli minuta poza zakresem.
     */
    ZdarzenieMeczowe(int minuta, const std::string& druzyna,
                     const std::string& zawodnik = "");

    virtual ~ZdarzenieMeczowe() = default;

    int         getMinuta()    const;
    std::string getDruzyna()   const;
    std::string getZawodnik()  const;
    EfektZdarzenia getEfekt()  const;

    /**
     * @brief Wykonuje zdarzenie i ustawia jego efekt.
     *
     * Czysta wirtualna – kazda klasa pochodna realizuje wlasna logike
     * (np. Strzal losuje czy padla bramka, Kartonik ustawia typ kartki).
     *
     * @return Tekstowy opis przebiegu zdarzenia.
     */
    virtual std::string wykonaj() = 0;

    /**
     * @brief Zwraca krotki opis zdarzenia (bez wywolania logiki).
     * @return Nazwa typu zdarzenia.
     */
    virtual std::string getTypZdarzenia() const = 0;

    /** @brief Wyswietla zdarzenie na stdout (uzywane przez warstwe konsolowa). */
    void wyswietl() const;

protected:
    int            m_minuta;    ///< Minuta meczu.
    std::string    m_druzyna;   ///< Druzyna zwiazana ze zdarzeniem.
    std::string    m_zawodnik;  ///< Zawodnik (moze byc pusty).
    EfektZdarzenia m_efekt;     ///< Efekt ustawiany przez wykonaj().
    std::string    m_opis;      ///< Opis ustalony po wywolaniu wykonaj().
};

// ─────────────────────────────────────────────────────────────────────────────
// Klasy pochodne (5 klas)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Zdarzenie: strzal na bramke.
 *
 * Wykonaj() losuje wynik na podstawie sily strzelca i bramkarza.
 * Jesli padnie bramka, ustawia odpowiedni EfektZdarzenia.
 */
class Strzal : public ZdarzenieMeczowe {
public:
    /**
     * @brief Konstruktor strzalu.
     * @param minuta        Minuta meczu.
     * @param druzyna       Druzyna atakujaca.
     * @param strzelec      Imie i nazwisko strzelca.
     * @param ocenaStrzalu  Ocena ofensywna strzelca (1-99).
     * @param ocenaBramkarza Ocena defensywna bramkarza (1-99).
     * @param czyGospodarz  True jesli strzela druzyna domowa.
     */
    Strzal(int minuta, const std::string& druzyna, const std::string& strzelec,
           int ocenaStrzalu, int ocenaBramkarza, bool czyGospodarz);

    std::string wykonaj() override;
    std::string getTypZdarzenia() const override { return "Strzal"; }

    bool czyGol() const { return m_czyGol; }

private:
    int  m_ocenaStrzalu;    ///< Sila ataku strzelca.
    int  m_ocenaBramkarza;  ///< Sila obrony bramkarza.
    bool m_czyGospodarz;    ///< Czy strzela gospodarz.
    bool m_czyGol;          ///< Wynik – ustalany w wykonaj().
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Zdarzenie: faul.
 *
 * Rejestruje faul jednego zawodnika na drugim.
 */
class Faul : public ZdarzenieMeczowe {
public:
    /**
     * @param minuta        Minuta meczu.
     * @param druzyna       Druzyna popelniajaca faul.
     * @param sprawca       Zawodnik popelniajacy faul.
     * @param poszkodowany  Zawodnik poszkodowany.
     * @param ciezkosc      Ciezkosc faulu (1=lekki, 2=sredni, 3=ciezki).
     */
    Faul(int minuta, const std::string& druzyna, const std::string& sprawca,
         const std::string& poszkodowany, int ciezkosc);

    std::string wykonaj() override;
    std::string getTypZdarzenia() const override { return "Faul"; }

    int         getCiezkosc()      const { return m_ciezkosc; }
    std::string getPoszkodowany()  const { return m_poszkodowany; }

private:
    std::string m_poszkodowany;  ///< Poszkodowany zawodnik.
    int         m_ciezkosc;      ///< Ciezkosc faulu (1-3).
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Zdarzenie: kartka (zolta lub czerwona).
 */
class Kartonik : public ZdarzenieMeczowe {
public:
    /** @brief Rodzaj kartki. */
    enum class Kolor { ZOLTY, CZERWONY };

    /**
     * @param minuta    Minuta meczu.
     * @param druzyna   Druzyna karanego zawodnika.
     * @param zawodnik  Ukarany zawodnik.
     * @param kolor     Rodzaj kartki.
     */
    Kartonik(int minuta, const std::string& druzyna,
             const std::string& zawodnik, Kolor kolor);

    std::string wykonaj() override;
    std::string getTypZdarzenia() const override { return "Kartonik"; }

    Kolor getKolor() const { return m_kolor; }

private:
    Kolor m_kolor; ///< Rodzaj kartki.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Zdarzenie: rzut rozny.
 *
 * Rzut rozny moze zakonczyc sie golem lub splajcowaniem pilki.
 */
class RzutRozny : public ZdarzenieMeczowe {
public:
    /**
     * @param minuta         Minuta meczu.
     * @param druzyna        Druzyna wykonujaca rzut rozny.
     * @param zawodnik       Zawodnik wykonujacy rzut.
     * @param czyGospodarz   Czy jest to druzyna domowa.
     * @param ocenaWykonania Ocena techniczna zawodnika (1-99).
     */
    RzutRozny(int minuta, const std::string& druzyna, const std::string& zawodnik,
              bool czyGospodarz, int ocenaWykonania);

    std::string wykonaj() override;
    std::string getTypZdarzenia() const override { return "Rzut rozny"; }

    bool czyGol() const { return m_czyGol; }

private:
    bool m_czyGospodarz;     ///< Czy wykonuje gospodarz.
    int  m_ocenaWykonania;   ///< Ocena techniki (wplywa na szanse gola).
    bool m_czyGol;           ///< Wynik rzutu – ustalany w wykonaj().
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Zdarzenie: zmiana zawodnika.
 */
class Zmiana : public ZdarzenieMeczowe {
public:
    /**
     * @param minuta        Minuta meczu.
     * @param druzyna       Druzyna dokonujaca zmiany.
     * @param wychodzacy    Wychodzacy zawodnik.
     * @param wchodzacy     Wchodzacy zawodnik.
     */
    Zmiana(int minuta, const std::string& druzyna,
           const std::string& wychodzacy, const std::string& wchodzacy);

    std::string wykonaj() override;
    std::string getTypZdarzenia() const override { return "Zmiana"; }

    std::string getWychodzacy() const { return m_wychodzacy; }
    std::string getWchodzacy()  const { return m_wchodzacy; }

private:
    std::string m_wychodzacy; ///< Wychodzacy zawodnik.
    std::string m_wchodzacy;  ///< Wchodzacy zawodnik.
};
