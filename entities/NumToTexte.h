/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef NUMTOTEXTE_H
#define NUMTOTEXTE_H

#include <map>
#include <QRegularExpressionValidator>
#include <QString>
#include <QVariant>

/*! \defgroup groupeDivers Divers
 * \brief Ensemble de classes diverses.
 */

/*! \ingroup groupeDivers
 * \brief Espace de nom de classes diverses.
 */
namespace diversMPS  {
/*! \ingroup groupeDivers
 * \brief Classe de conversion entre chaine de caractères et entiers.
 */
class NumToTexte {
protected:
    unsigned m_style;                           //!< Identifiant du style d'affichage des nombres.
    int (*m_toNum)(const QString &);            //!< Conversion d'une chaine de caractères en entier en fonction du style.
    QString (*m_toString)(int);                 //!< Converstion d'un entier (début à 0) en QString avec le style définie.
    QRegularExpressionValidator m_valide;       //!< Motif de validité pour le style courant.
    enum {BaseLettre = 26,
          Rho = 17,
          Sigma = Rho + 1,
          BaseGrec = 24,
          DebutGrecMajuscule = 0x391,
          DebutGrecMinuscule = 0x3B1,
          DebutMajuscule = 0x041,
          DebutMinuscule = 0x061,

    };
public:
    //! Type d'affichage des attributs alpha.
    enum style {
        Arabe,
        ArabeSuivant,
        GrecMajuscule,
        GrecMinuscule,
        Majuscule,
        Minuscule,
        Romain,
        RomainSuivant,
        NbrStyle,
    };


    const static std::vector<std::pair<int,QString>> m_romain;    //!< map de converions nombre <-> lettre des chifres romains.

    //! Constructeur.
    NumToTexte(unsigned style = Arabe)
        {setStyle(style);}

    //! Constructeur par copie.
    NumToTexte(const NumToTexte & other)
        {setStyle(other.m_style);}

    //! Destructeur.
    ~NumToTexte() = default;

    //! Décomposition de l'entier.
    template<int Base, int Debut> static QString decomposition(int num);

    //! Décomposition de l'entier.
    template<int Debut> static QString decompositionGrec(int num);

    //! Écriture avec une lettre grec majuscule.
    static QString grecMajuscule(int num)
        {return decompositionGrec<DebutGrecMajuscule>(num);}

    //! Écriture avec une lettre grec majuscule.
    static int grecMajusculeToNum(const QString & str)
        {return recompositionGrec<DebutGrecMajuscule>(str);}

    //! Écriture avec une lettre grec minuscule.
    static QString grecMinuscule(int num)
        {return decompositionGrec<DebutGrecMinuscule>(num);}

    //! Écriture avec une lettre grec majuscule.
    static int grecMinusculeToNum(const QString & str)
        {return recompositionGrec<DebutGrecMinuscule>(str);}

    //! Écriture avec une lettre majuscule.
    static QString majuscule(int num)
        {return decomposition<BaseLettre,DebutMajuscule>(num);}

    //! Écriture avec une lettre majuscule.
    static int majusculeToNum(const QString & str)
        {return recomposition<BaseLettre,DebutMajuscule>(str);}

    //! Écriture avec une lettre minuscule.
    static QString minuscule(int num)
        {return decomposition<BaseLettre,DebutMinuscule>(num);}

    //! Écriture avec une lettre majuscule.
    static int minusculeToNum(const QString & str)
        {return recomposition<BaseLettre,DebutMinuscule>(str);}

    int num(const QString & str) const
        {return m_toNum(str);}

    //! Convertion en un uint
    operator uint() const noexcept
        {return m_style;}

    //! Opérateur de copie.
    NumToTexte & operator =(const NumToTexte & other) noexcept {
        setStyle(other.m_style);
        return *this;
    }

    //! Opérateur d'affectation à partir d'un uint.
    NumToTexte & operator =(unsigned style) noexcept {
        setStyle(style);
        return *this;
    }

    //! Test d'égalité.
    bool operator == (const NumToTexte & other) const noexcept
        {return m_style == other.m_style;}

    //! Recomposition de l'entier.
    template<int Base, int Debut> static int recomposition(const QString & str);

    //! Recomposition de l'entier.
    template<int Debut> static int recompositionGrec(const QString & str);

    //! Écriture en chiffre romain.
    static QString romain(int num);

    //! Entier coorespondant à un chiffre romain.
    static int romainToNum(const QString & str);

    //! Mutateur du style.
    void setStyle(unsigned style);

    //! Accesseur du style.
    unsigned style() const noexcept
        {return m_style;}

    //! Converstion d'un entier (début à 0) en QString avec le style définie.
    QString texte(int num) const
        {return m_toString(num);}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString & input, int & pos) const
        {return m_valide.validate(input,pos);}
};
/////////////////////////////////////// Définition //////////////////////
template<int Base, int Debut> QString NumToTexte::decomposition(int num) {
    if(num < 0)
        return QString();
    if(num < Base)
        return QChar(Debut + num);
    QString str;
    auto reste = num % Base;
    num -= reste;
    num /= Base;
    while(num) {
        str.prepend(QChar(Debut + reste));
        reste = num % Base;
        num -= reste;
        num /= Base;
    }
    str.prepend(QChar(Debut + reste - 1));
    return str;
}
template<int Debut> QString NumToTexte::decompositionGrec(int num) {
    if(num < 0)
        return QString();
    if(num < Rho)
        return QChar(Debut + num);
    if(num < BaseGrec)
        return QChar(Debut + num + 1);
    QString str;
    auto reste = num % BaseGrec;
    num -= reste;
    num /= BaseGrec;
    while(num) {
        if(reste < Rho)
            str.prepend(QChar(Debut + reste));
        else
            str.prepend(QChar(Debut + reste + 1));
        reste = num % BaseGrec;
        num -= reste;
        num /= BaseGrec;
    }
    if(reste < Rho)
        str.prepend(QChar(Debut + reste - 1));
    else
        str.prepend(QChar(Debut + reste));
    return str;
}
template<int Base, int Debut> int NumToTexte::recomposition(const QString & str) {
    if(str.isEmpty())
        return 0;
    if(str.count() == 1)
        return str.back().unicode() - Debut;
    auto iter = str.cbegin();
    int num = iter->unicode() - Debut + 1;
    ++iter;
    while (iter != str.cend()) {
        num *= Base;
        num += iter->unicode() - Debut;
        ++iter;
    }
    return num;
}

template<int Debut> int NumToTexte::recompositionGrec(const QString & str) {
    if(str.isEmpty())
        return 0;
    if(str.count() == 1) {
        int num = str.back().unicode() - Debut;
        if(num >= Sigma)
            --num;
        return num;
    }
    auto iter = str.cbegin();
    int num = iter->unicode() - Debut;
    if(num < Sigma)
        ++num;
    ++iter;
    while (iter != str.cend()) {
        num *= BaseGrec;
        int code = iter->unicode() - Debut;
        num += code;
        if(code >= Sigma)
            --num;
        ++iter;
    }
    return num;
}
}
#endif // NUMTOTEXTE_H
