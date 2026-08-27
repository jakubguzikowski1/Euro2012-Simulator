/**
 * @file CzlonekSztabu.h
 * @brief Drzewo dziedziczenia nr 3 – Sztab Szkoleniowy.
 *
 * Abstrakcyjna klasa bazowa CzlonekSztabu oraz 5 klas pochodnych:
 * TrenerGlowny, Asystent, Lekarz, Fizjoterapeuta, Skaut.
 *
 * UWAGA: Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Exceptions.h"
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Klasa bazowa (abstrakcyjna)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Abstrakcyjna klasa bazowa dla czlonkow sztabu szkoleniowego.
 *
 * Hermetyzuje dane osobowe i doswiadczenie. Definiuje czyste
 * metody wirtualne: wykonajAkcje() i getRola().
 */
class CzlonekSztabu {
public:
    /**
     * @brief Konstruktor czlonka sztabu.
     * @param imie          Imie.
     * @param nazwisko      Nazwisko.
     * @param wiek          Wiek (18-80).
     * @param doswiadczenie Lata doswiadczenia (0-50).
     * @throws ZawodnikWyjatek Jesli dane sa nieprawidlowe.
     */
    CzlonekSztabu(const std::string& imie, const std::string& nazwisko,
                  int wiek, int doswiadczenie);

    virtual ~CzlonekSztabu() = default;

    std::string getImie()           const;
    std::string getNazwisko()       const;
    std::string getPelneNazwisko()  const;
    int         getWiek()           const;
    int         getDoswiadczenie()  const;

    /**
     * @brief Wykonuje akcje charakterystyczna dla danej roli w sztabie.
     * @return Tekstowy opis akcji.
     */
    virtual std::string wykonajAkcje() const = 0;

    /**
     * @brief Zwraca nazwe roli w sztabie (np. "Trener Glowny").
     * @return Nazwa roli.
     */
    virtual std::string getRola() const = 0;

    /** @brief Wyswietla informacje o czlonku sztabu. */
    void wyswietl() const;

protected:
    std::string m_imie;          ///< Imie.
    std::string m_nazwisko;      ///< Nazwisko.
    int         m_wiek;          ///< Wiek.
    int         m_doswiadczenie; ///< Lata doswiadczenia.
};

// ─────────────────────────────────────────────────────────────────────────────
// Klasy pochodne (5 klas)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Trener glowny druzyny.
 *
 * Odpowiada za formacje taktyczna i strategie gry.
 */
class TrenerGlowny : public CzlonekSztabu {
public:
    /**
     * @param imie       Imie.
     * @param nazwisko   Nazwisko.
     * @param wiek       Wiek.
     * @param dosw       Lata doswiadczenia.
     * @param formacja   Formacja taktyczna (np. "4-4-2").
     * @param strategia  Styl gry (np. "Pressing", "Kontra").
     */
    TrenerGlowny(const std::string& imie, const std::string& nazwisko,
                 int wiek, int dosw,
                 const std::string& formacja, const std::string& strategia);

    std::string wykonajAkcje() const override;
    std::string getRola()      const override { return "Trener Glowny"; }

    std::string getFormacja()  const { return m_formacja; }
    std::string getStrategia() const { return m_strategia; }

private:
    std::string m_formacja;   ///< Formacja taktyczna.
    std::string m_strategia;  ///< Styl gry.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Asystent trenera.
 *
 * Wspomaga trenera glownego w analizie i szkoleniu.
 */
class Asystent : public CzlonekSztabu {
public:
    /**
     * @param imie            Imie.
     * @param nazwisko        Nazwisko.
     * @param wiek            Wiek.
     * @param dosw            Lata doswiadczenia.
     * @param specjalizacja   Obszar specjalizacji (np. "Obrona", "Stale fragmenty").
     */
    Asystent(const std::string& imie, const std::string& nazwisko,
             int wiek, int dosw, const std::string& specjalizacja);

    std::string wykonajAkcje() const override;
    std::string getRola()      const override { return "Asystent Trenera"; }

    std::string getSpecjalizacja() const { return m_specjalizacja; }

private:
    std::string m_specjalizacja; ///< Obszar specjalizacji.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Lekarz druzyny.
 *
 * Odpowiada za diagnostyke medyczna i leczenie kontuzji.
 */
class Lekarz : public CzlonekSztabu {
public:
    /**
     * @param imie              Imie.
     * @param nazwisko          Nazwisko.
     * @param wiek              Wiek.
     * @param dosw              Lata doswiadczenia.
     * @param specjalizacjaMed  Specjalizacja medyczna (np. "Ortopedia").
     */
    Lekarz(const std::string& imie, const std::string& nazwisko,
           int wiek, int dosw, const std::string& specjalizacjaMed);

    std::string wykonajAkcje() const override;
    std::string getRola()      const override { return "Lekarz"; }

    std::string getSpecjalizacjaMed() const { return m_specjalizacjaMed; }

private:
    std::string m_specjalizacjaMed; ///< Specjalizacja medyczna.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Fizjoterapeuta druzyny.
 *
 * Odpowiada za rehabilitacje, stretching i prewencje kontuzji.
 */
class Fizjoterapeuta : public CzlonekSztabu {
public:
    /**
     * @param imie        Imie.
     * @param nazwisko    Nazwisko.
     * @param wiek        Wiek.
     * @param dosw        Lata doswiadczenia.
     * @param certyfikat  Posiadany certyfikat (np. "UEFA Medical").
     */
    Fizjoterapeuta(const std::string& imie, const std::string& nazwisko,
                   int wiek, int dosw, const std::string& certyfikat);

    std::string wykonajAkcje() const override;
    std::string getRola()      const override { return "Fizjoterapeuta"; }

    std::string getCertyfikat() const { return m_certyfikat; }

private:
    std::string m_certyfikat; ///< Certyfikat zawodowy.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Skaut druzyny.
 *
 * Obserwuje i analizuje potencjalnych zawodnikow i rywali.
 */
class Skaut : public CzlonekSztabu {
public:
    /**
     * @param imie    Imie.
     * @param nazwisko Nazwisko.
     * @param wiek    Wiek.
     * @param dosw    Lata doswiadczenia.
     * @param region  Region zwiadowania (np. "Europa Wschodnia").
     */
    Skaut(const std::string& imie, const std::string& nazwisko,
          int wiek, int dosw, const std::string& region);

    std::string wykonajAkcje() const override;
    std::string getRola()      const override { return "Skaut"; }

    std::string getRegion() const { return m_region; }

private:
    std::string m_region; ///< Region zwiadowania.
};
