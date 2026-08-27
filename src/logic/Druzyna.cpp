/**
 * @file Druzyna.cpp
 * @brief Implementacja klasy Druzyna zarzadzajacej skladem zawodnikow i sztabem szkoleniowym.
 *
 * Klasa przechowuje std::unique_ptr do obiektow polimorficznych
 * (zawodnicy, sztab); pamiec zwalniana jest automatycznie.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "Druzyna.h"
#include <iostream>

Druzyna::Druzyna(const std::string& nazwa, const std::string& kraj)
    : m_nazwa(nazwa), m_kraj(kraj) {
    if (nazwa.empty()) throw DrużynaWyjatek("Nazwa druzyny nie moze byc pusta.");
}

std::string Druzyna::getNazwa() const { return m_nazwa; }

std::string Druzyna::getKraj()  const { return m_kraj; }

int Druzyna::getLiczbaZawodnikow() const { return static_cast<int>(m_zawodnicy.size()); }

void Druzyna::dodajZawodnika(std::unique_ptr<Zawodnik> zawodnik) {
    if (!zawodnik) throw DrużynaWyjatek("Proba dodania null-zawodnika");
    m_zawodnicy.push_back(std::move(zawodnik));
}

void Druzyna::dodajCzlonkaSztabu(std::unique_ptr<CzlonekSztabu> czlonek) {
    if (!czlonek) throw DrużynaWyjatek("Proba dodania null-czlonka");
    m_sztab.push_back(std::move(czlonek));
}

void Druzyna::waliduj() const {
    if (static_cast<int>(m_zawodnicy.size()) < MIN_ZAWODNIKOW)
        throw DrużynaWyjatek(m_nazwa + " ma za malo zawodnikow.");
}

Zawodnik* Druzyna::getZawodnik(int index) {
    if (index < 0 || index >= getLiczbaZawodnikow()) throw DrużynaWyjatek("Zly indeks");
    return m_zawodnicy[static_cast<size_t>(index)].get();
}

const Zawodnik* Druzyna::getZawodnik(int index) const {
    if (index < 0 || index >= getLiczbaZawodnikow()) throw DrużynaWyjatek("Zly indeks");
    return m_zawodnicy[static_cast<size_t>(index)].get();
}

int Druzyna::getSilaAtaku() const {
    int suma = 0, cnt = 0;
    for (const auto& z : m_zawodnicy) {
        if (z->getTyp() == "Napastnik" || z->getTyp() == "Pomocnik") {
            suma += z->getOgolnaOcena();
            cnt++;
        }
    }
    return cnt > 0 ? suma / cnt : 50;
}

int Druzyna::getSilaObrony() const {
    int suma = 0, cnt = 0;
    for (const auto& z : m_zawodnicy) {
        if (z->getTyp() == "Bramkarz" || z->getTyp() == "Srodkowy Obronca" ||
            z->getTyp() == "Boczny Obronca") {
            suma += z->getOgolnaOcena();
            cnt++;
        }
    }
    return cnt > 0 ? suma / cnt : 50;
}

Zawodnik* Druzyna::getNajlepszyAtakujacy() {
    Zawodnik* najlepszy = nullptr;
    int maxOcena = -1;
    for (auto& z : m_zawodnicy) {
        if ((z->getTyp() == "Napastnik" || z->getTyp() == "Pomocnik") &&
            z->getOgolnaOcena() > maxOcena && !z->isSuspended()) {
            maxOcena = z->getOgolnaOcena();
            najlepszy = z.get();
        }
    }
    if (!najlepszy && !m_zawodnicy.empty()) najlepszy = m_zawodnicy[0].get();
    return najlepszy;
}

Zawodnik* Druzyna::getBramkarz() {
    for (auto& z : m_zawodnicy) {
        if (z->getTyp() == "Bramkarz") return z.get();
    }
    return nullptr;
}

void Druzyna::wyswietlSklad() const {
    std::cout << "=== " << m_nazwa << " ===\n";
    for (const auto& z : m_zawodnicy) z->wyswietl();
}

void Druzyna::wyswietlSztab() const {
    for (const auto& c : m_sztab) c->wyswietl();
}
