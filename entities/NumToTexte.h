/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef NUMTOTEXTE_H
#define NUMTOTEXTE_H

#include <map>
#include <QString>
#include <QVariant>

namespace diversMPS  {

class NumToTexte {
protected:
    uint m_style;        //!< Identifiant du style d'affichage des nombres.
    QString (*m_toString)(int);    //!< Converstion d'un entier (début à 0) en QString avec le style définie.
public:
    //! Type d'affichage des attributs alpha.
    enum style {
        Arabe,
        GrecMajuscule,
        GrecMinuscule,
        Majuscule,
        Minuscule,
        Romain,
        NbrStyle,
    };

    const static std::map<int,QString> m_romain;    //!< map de converions nombre <-> lettre des chifres romains.

    //! Constructeur.
    NumToTexte(uint style = Arabe)
        {setStyle(style);}

    //! Constructeur par copie.
    NumToTexte(const NumToTexte & other)
        {setStyle(other.m_style);}

    //! Destructeur.
    ~NumToTexte() = default;

    //! Écriture avec des chiffres arabes.
    static QString arabe(int num)
        {return QString::number(num + 1);}

    //! Écriture avec une lettre grec majuscule.
    static QString grecMajuscule(int num) {
        auto i = 0x391;
        i += num;
        return QChar(i);
    }

    //! Écriture avec une lettre majuscule.
    static QString majuscule(int num) {
        auto i = 0x41;
        i += num;
        return QChar(i);
    }

    //! Écriture avec une lettre minuscule.
    static QString minuscule(int num) {
        auto i = 0x61;
        i += num;
        return QChar(i);
    }

    //! Écriture avec une lettre grec minuscule.
    static QString grecMinuscule(int num) {
        auto i = 0x3B1;
        i += num;
        return QChar(i);
    }

    //! Convertion en un uint
    operator uint() const
        {return m_style;}

    //! Convertion en un uint
    operator QVariant() const
        {return m_style;}

    //! Opérateur de copie.
    NumToTexte & operator =(const NumToTexte & other) {
        setStyle(other.m_style);
        return *this;
    }

    //! Opérateur d'affectation à partir d'un uint.
    NumToTexte & operator =(uint style) {
        setStyle(style);
        return *this;
    }

    //! Test d'égalité.
    bool operator == (const NumToTexte & other) const
        {return m_style == other.m_style;}

    //! Écriture en chiffre romain.
    static QString romain(int num);

    //! Mutateur du style.
    void setStyle(uint style);

    //! Converstion d'un entier (début à 0) en QString avec le style définie.
    QString texte(int num) const
        {return m_toString(num);}
};
}
Q_DECLARE_METATYPE(diversMPS::NumToTexte)
#endif // NUMTOTEXTE_H
