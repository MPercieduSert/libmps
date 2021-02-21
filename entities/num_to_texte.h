/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef NUM_TO_TEXTE_H
#define NUM_TO_TEXTE_H

#include <map>
#include <QRegularExpressionValidator>
#include <QString>
#include <QVariant>
#include "type_mps.h"

/*! \defgroup groupeDivers Divers
 *\brief Ensemble de classes diverses.
 */

/*! \ingroup groupeDivers
 *\brief Espace de nom de classes diverses.
 */
namespace mps {
namespace divers  {
/*! \ingroup groupeDivers
 *\brief Classe de conversion entre chaine de caractères et entiers.
 */
class num_to_string {
protected:
    enumt m_style;                          //!< Identifiant du style d'affichage des nombres.
    int (*m_to_num)(const QString &);       //!< Conversion d'une chaine de caractères en entier en fonction du style.
    QString (*m_to_string)(int);            //!< Converstion d'un entier (début à 0) en QString avec le style définie.
    QRegularExpressionValidator m_valide;   //!< Motif de validité pour le style courant.
    enum {Rho = 17,
          Sigma = Rho + 1,
          Grec_Base = 24,
          Lettre_Base = 26,
          Grec_Majuscule_Debut = 0x391,
          Grec_Minuscule_Debut = 0x3B1,
          Lettre_Majuscule_Debut = 0x041,
          Lettre_Minuscule_Debut = 0x061
    };
public:
    //! Type d'affichage des attributs alpha.
    enum style {
        Arabe_Style,
        Arabe_Suivant_Style,
        Grec_Majuscule_Style,
        Grec_Minuscule_Style,
        Majuscule_Style,
        Minuscule_Style,
        Romain_Style,
        Romain_Suivant_Style,
        Nbr_Style
    };


    const static std::vector<std::pair<int,QString>> m_Romain;    //!< map de converions nombre <-> lettre des chifres romains.

    //! Constructeur.
    num_to_string(enumt style = Arabe_Style)
        {set_style(style);}

    //! Constructeur par copie.
    num_to_string(const num_to_string &other)
        {set_style(other.m_style);}

    //! Destructeur.
    ~num_to_string() = default;

    //! Décomposition de l'entier.
    template<int Base, int Debut> static QString Decomposition(int num);

    //! Décomposition de l'entier.
    template<int Debut> static QString Decomposition_grec(int num);

    //! Écriture avec une lettre grec majuscule.
    static QString Grec_majuscule(int num)
        {return Decomposition_grec<Grec_Majuscule_Debut>(num);}

    //! Écriture avec une lettre grec majuscule.
    static int Grec_majuscule_to_num(const QString &str)
        {return Recomposition_grec<Grec_Majuscule_Debut>(str);}

    //! Écriture avec une lettre grec minuscule.
    static QString Grec_minuscule(int num)
        {return Decomposition_grec<Grec_Minuscule_Debut>(num);}

    //! Écriture avec une lettre grec majuscule.
    static int Grec_minuscule_to_num(const QString &str)
        {return Recomposition_grec<Grec_Minuscule_Debut>(str);}

    //! Écriture avec une lettre majuscule.
    static QString Majuscule(int num)
        {return Decomposition<Lettre_Base,Lettre_Majuscule_Debut>(num);}

    //! Écriture avec une lettre majuscule.
    static int Majuscule_to_num(const QString &str)
        {return Recomposition<Lettre_Base,Lettre_Majuscule_Debut>(str);}

    //! Écriture avec une lettre minuscule.
    static QString Minuscule(int num)
        {return Decomposition<Lettre_Base,Lettre_Minuscule_Debut>(num);}

    //! Écriture avec une lettre majuscule.
    static int Minuscule_to_num(const QString &str)
        {return Recomposition<Lettre_Base,Lettre_Minuscule_Debut>(str);}

    int num(const QString &str) const
        {return m_to_num(str);}

    //! Convertion en un enumt
    operator enumt() const noexcept
        {return m_style;}

    //! Opérateur de copie.
    num_to_string &operator =(const num_to_string &other) noexcept {
        set_style(other.m_style);
        return *this;
    }

    //! Opérateur d'affectation à partir d'un enumt.
    num_to_string &operator =(enumt style) noexcept {
        set_style(style);
        return *this;
    }

    //! Test d'égalité.
    bool operator == (const num_to_string &other) const noexcept
        {return m_style == other.m_style;}

    //! Recomposition de l'entier.
    template<int Base, int Debut> static int Recomposition(const QString &str);

    //! Recomposition de l'entier.
    template<int Debut> static int Recomposition_grec(const QString &str);

    //! Écriture en chiffre romain.
    static QString Romain(int num);

    //! Entier coorespondant à un chiffre romain.
    static int Romain_to_num(const QString &str);

    //! Mutateur du style.
    void set_style(enumt style);

    //! Accesseur du style.
    enumt style() const noexcept
        {return m_style;}

    //! Converstion d'un entier (début à 0) en QString avec le style définie.
    QString texte(int num) const
        {return m_to_string(num);}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString &input, int &pos) const
        {return m_valide.validate(input,pos);}
};
/////////////////////////////////////// Définition //////////////////////
template<int Base, int Debut> QString num_to_string::Decomposition(int num) {
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
template<int Debut> QString num_to_string::Decomposition_grec(int num) {
    if(num < 0)
        return QString();
    if(num < Rho)
        return QChar(Debut + num);
    if(num < Grec_Base)
        return QChar(Debut + num + 1);
    QString str;
    auto reste = num % Grec_Base;
    num -= reste;
    num /= Grec_Base;
    while(num) {
        if(reste < Rho)
            str.prepend(QChar(Debut + reste));
        else
            str.prepend(QChar(Debut + reste + 1));
        reste = num % Grec_Base;
        num -= reste;
        num /= Grec_Base;
    }
    if(reste < Rho)
        str.prepend(QChar(Debut + reste - 1));
    else
        str.prepend(QChar(Debut + reste));
    return str;
}
template<int Base, int Debut> int num_to_string::Recomposition(const QString &str) {
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

template<int Debut> int num_to_string::Recomposition_grec(const QString &str) {
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
        num *= Grec_Base;
        int code = iter->unicode() - Debut;
        num += code;
        if(code >= Sigma)
            --num;
        ++iter;
    }
    return num;
}
}}
#endif // NUM_TO_TEXTE_H
