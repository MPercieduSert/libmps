/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef ATTRIBUT_SIMPLE_H
#define ATTRIBUT_SIMPLE_H

#include <array>
#include <cmath>
#include <QColor>
#include <QDate>
#include <QDateTime>
#include <QRegularExpression>
#include <QString>
#include <QVariant>
#include <utility>
#include "info_entity.h"
#include "macro_mps.h"
#include "num_to_texte.h"

/*! \defgroup groupe_attribut_entity Attributs des entités
 *\ingroup groupe_entity
 *\brief Ensemble de classes représentant les attributs des entités de la base de donnée.
 */

//! \ingroup groupe_attribut_entity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    idt id_ ## NOM () const {return id_ ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void set_id_ ## NOM (idt n) {set_id_ ## NUM (n);} \
    /*! \brief Nom de identifiant*/ \
    QString id_ ## NUM ## _nom() const override {return "id_"#NOM;}

//! \ingroup groupe_attribut_entity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE_NEG(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    int id_ ## NOM () const {return id_ ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void set_id_ ## NOM (int n) {set_id_ ## NUM (n);} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void set_id_ ## NOM (idt n) {set_id_ ## NUM (static_cast<int>(n));} \
    /*! \brief Nom de identifiant*/ \
    QString id_ ## NUM ## _nom() const override {return "id_"#NOM;}

/*! \ingroup groupe_attribut_entity
 *\brief Début des macros de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,NOM,Nom) /*! \ingroup groupe_attribut_entity \brief Classe de l'attribut ATTRIBUT.*/ \
    class ATTRIBUT: public MERE { \
    public: \
    using MERE::get; \
    using MERE::set; \
    CONSTR_DEFAUT(ATTRIBUT) \
    CONSTR_AFFECT_DEFAUT(ATTRIBUT) \
    /*! Destructeur.*/ \
    ~ATTRIBUT() override; \
    /*! positions des attributs.*/ \
    enum position {NOM, Nbr_Att}; \
    /*! Nom de l'attribut.*/ \
    static QString Name(post /*pos*/=0) {return #Nom;} \
    /*! Nom de la classe de l'attribut.*/ \
    QString classe_nom() const override {return #ATTRIBUT;} \
    /*! Accesseur de l'attribut nom.*/ \
    ATTRIBUT::att_trans Nom() const {return get();} \
    /*! Mutateur de l'attribut.*/ \
    void set(const ATTRIBUT &ent) {set(ent.get());}\
    /*! Mutateur de l'attribut nom.*/ \
    void set_ ## Nom(ATTRIBUT::att_trans valeur) {set(valeur);} \
    /*! Opérateur égalité */ \
    bool operator ==(const ATTRIBUT &ent) const {return MERE::operator ==(ent);}

/*! \ingroup groupe_attribut_entity
 *\brief Macro de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT(ATTRIBUT,MERE,NOM,Nom) /*! \ingroup groupe_attribut_entity \brief Classe de l'attribut ATTRIBUT.*/ \
    SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,NOM,Nom) \
    /*! Nom de l'attribut.*/ \
    QString attribut_nom() const override {return #Nom;}};

/*! \ingroup groupe_attribut_entity
 *\brief Macro de déclaration d'un attribut de type clé dans les entités.
 */
#define SINGLE_ATTRIBUT_ID(ATTRIBUT,MERE,NUM) /*! \ingroup groupe_attribut_entity \brief Classe de l'attribut ATTRIBUT.*/ \
    SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,Id_ ## NUM,id_ ## NUM) \
    /*! Nom de l'attribut.*/ \
    QString attribut_nom() const override {return id_ ## NUM ## _nom();}\
    /*! Nom de identifiant*/ \
    virtual QString id_ ## NUM ## _nom() const {return "Id"#NUM;}};

namespace mps {
/*! \ingroup groupe_attribut_entity
 *\brief Espace de nom des attributs des entités.
 */
namespace attribut {
/*! \ingroup groupe_attribut_entity
 *\brief Classe signalant la fin d'une branche d'attribut.
 */
class no_attribut;

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs (simple) des entités.
 */
class attribut_entity {
public:
    using att_pre = no_attribut;
    using att_suiv = no_attribut;
    CONSTR_DEFAUT(attribut_entity)

    //! Destructeur.
    virtual ~attribut_entity();

    //! Renvoie une chaine de caractère contenant le nom, la valeur est la validité de l'attribut.
    virtual QString affiche() const;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    virtual QVariant data_v(post /*pos*/) const = 0;

    //! Accesseur de l'attribut pour la base de données.
    virtual QVariant get_to_bdd() const = 0;

    //! Renvoie la validité de la valeur.
    virtual bool is_valid() const
        {return is_valid_attribut();}

    //! Renvoie la validité de la valeur.
    virtual bool is_valid_attribut() const
        {return true;}

    //! Renvoie le nom de l'attribut.
    virtual QString attribut_nom() const = 0;

    //! Renvoie le nom de l'attribut.
    virtual QString classe_nom() const = 0;

    //! Modifie la donnée à partir d'un QVariant.
    virtual void set_data_v(post /*pos*/, const QVariant &value) = 0;

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    virtual QString to_string() const
        {return to_string_attribut();}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    virtual QString to_string_attribut() const = 0;

    //! Renvoie la chaine "valide" si l'attribut est valide et "invalide" sinon.
    QString valid_to_string() const
        {return is_valid_attribut() ? "valide" : "invalide";}

protected:
    //! Renvoie le nom de l'attribut.
    QString multiple_nom(post /*Pos*/) const
        {return attribut_nom();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe patron des attribut avec une transimision par reférence dans les accesseurs et mutateurs.
 */
template<class Var, class Trans> class attribut_entity_temp : public attribut_entity {
public:
    using att_type = Var;
    using att_trans = Trans;
protected:
    att_type m_valeur;     //!< Valeur de l'attribut.

public:
    attribut_entity_temp() {}
    CONSTR_AFFECT_DEFAUT(attribut_entity_temp)

    //! Constructeur, donner la valeur de l'attribut en argument.
    attribut_entity_temp(att_trans valeur)
        : m_valeur(valeur) {}

    //! Destructeur.
    ~attribut_entity_temp() override;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant data_v(post /*pos*/) const override
        {return m_valeur;}

    //! Accesseur de l'attribut.
    att_trans get() const
        {return m_valeur;}

    //! Accesseur de l'attribut pour la base de données.
    QVariant get_to_bdd() const override
        {return m_valeur;}

    //! Mutateur de l'attribut.
    void set(Trans valeur)
        {m_valeur = valeur;}

    //! Modifie la donnée à partir d'un QVariant.
    void set_data_v(post /*pos*/, const QVariant &value) override
        {set(value.value<att_type>());}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString to_string_attribut() const override
        {return QVariant(m_valeur).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const attribut_entity_temp<Var,Trans> &att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (att_trans valeur) const
        {return m_valeur == valeur;}
};

template<class Var, class Trans> attribut_entity_temp<Var,Trans>::~attribut_entity_temp() = default;

// Spécialisation partielle.
template<class T> using attribut_entity_val = attribut_entity_temp<T,T>;
template<class T> using attribut_entity_ref = attribut_entity_temp<T,const T &>;

// Spécialisation des constructeur fixant une valeur par défaut pour les types de base.
template<> inline attribut_entity_val<bool>::attribut_entity_temp() : m_valeur(true) {}
template<> inline attribut_entity_val<double>::attribut_entity_temp() : m_valeur(0) {}
template<> inline attribut_entity_val<float>::attribut_entity_temp() : m_valeur(0) {}
template<> inline attribut_entity_val<int>::attribut_entity_temp() : m_valeur(0) {}
template<> inline attribut_entity_val<idt>::attribut_entity_temp() : m_valeur(0) {}

// Spécilisation du set_datat pour les Variants.
template<> inline void attribut_entity_ref<QVariant>::set_data_v(post /*pos*/, const QVariant &value) {set(value);}

// Alis de spécialiations totales.
using attribut_bool = attribut_entity_val<bool>;
using attribut_double = attribut_entity_val<double>;
using attribut_float = attribut_entity_val<float>;
using attribut_int = attribut_entity_val<int>;
using attribut_unsigned = attribut_entity_val<unsigned>;
using Attribut_id = attribut_entity_val<idt>;

using attribut_date = attribut_entity_ref<QDate>;
using attribut_date_time = attribut_entity_ref<QDateTime>;
using attribut_string = attribut_entity_ref<QString>;
using attribut_variant = attribut_entity_ref<QVariant>;

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type date current.
 */
class attribut_date_current: public attribut_date {
public:
    CONSTR_DEFAUT(attribut_date_current)
    CONSTR_AFFECT_DEFAUT(attribut_date_current)

    //! Destructeur.
    ~attribut_date_current() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant get_to_bdd() const override
        {return QDate::currentDate();}

    //! Accesseur de l'attribut pour la base de données.
    const QDate &get_to_bdd()
        {return m_valeur = QDate::currentDate();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type date pouvant être nulle.
 */
class attribut_date_null : public attribut_date {
public:
    CONSTR_DEFAUT(attribut_date_null)
    CONSTR_AFFECT_DEFAUT(attribut_date_null)

    //! Destructeur.
    ~attribut_date_null() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur.isNull() || m_valeur.isValid();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type date valide.
 */
class attribut_date_valide : public attribut_date {
public:
    CONSTR_DEFAUT(attribut_date_valide)
    CONSTR_AFFECT_DEFAUT(attribut_date_valide)

    //! Destructeur.
    ~attribut_date_valide() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur.isValid();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type datetime current.
 */
class attribut_date_time_current: public attribut_date_time {
public:
    CONSTR_DEFAUT(attribut_date_time_current)
    CONSTR_AFFECT_DEFAUT(attribut_date_time_current)

    //! Destructeur.
    ~attribut_date_time_current() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant get_to_bdd() const override
        {return QDateTime::currentDateTime();}

    //! Accesseur de l'attribut pour la base de données.
    const QDateTime &get_to_bdd()
        {return m_valeur = QDateTime::currentDateTime();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type datetime nulle.
 */
class attribut_date_time_null : public attribut_date_time {
public:
    CONSTR_DEFAUT(attribut_date_time_null)
    CONSTR_AFFECT_DEFAUT(attribut_date_time_null)

    //! Destructeur.
    ~attribut_date_time_null() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur.isNull() || m_valeur.isValid();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type datetime valide.
 */
class attribut_date_time_valide : public attribut_date_time {
public:
    CONSTR_DEFAUT(attribut_date_time_valide)
    CONSTR_AFFECT_DEFAUT(attribut_date_time_valide)

    //! Destructeur.
    ~attribut_date_time_valide() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur.isValid();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type entier positif renvoyant _null pour zéro dans la base de données.
 */
template<class T> class Attribut_zero_null : public attribut_entity_val<T> {
protected:
    using attribut_entity_val<T>::m_valeur;
public:
    enum {Zero = 0};
    CONSTR_DEFAUT(Attribut_zero_null)
    CONSTR_AFFECT_DEFAUT(Attribut_zero_null)

    //! Destructeur.
    ~Attribut_zero_null() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant get_to_bdd() const override
        {return m_valeur == Zero ? QVariant(QVariant::UInt) : m_valeur;}
};

template<class T> Attribut_zero_null<T>::~Attribut_zero_null<T>() = default;

using Attribut_id_null = Attribut_zero_null<idt>;
using attribut_unsigned_null = Attribut_zero_null<unsigned>;

/*! \ingroup groupe_attribut_entity
 *\brief _template des attributs de type entier supérieur ou égal à N.
 */
template<class T, T N> class Attribut_sup : public attribut_entity_val<T> {
protected:
    using attribut_entity_val<T>::m_valeur;
public:
    CONSTR_DEFAUT(Attribut_sup)
    //! Constructeur.
    using attribut_entity_val<T>::attribut_entity_val;
    CONSTR_AFFECT_DEFAUT(Attribut_sup)

    //! Destruteur.
    ~Attribut_sup() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur >= N;}
};

template<class T, T N> Attribut_sup<T,N>::~Attribut_sup() = default;

template<idt N> using Attribut_id_sup = Attribut_sup<idt, N>;
template<int N> using attribut_int_sup = Attribut_sup<int, N>;
template<unsigned N> using attribut_unsigned_sup = Attribut_sup<unsigned, N>;

/*! \ingroup groupe_attribut_entity
 *\brief _template des attributs de type entier inférieur strict à N.
 */
template<class T, T N> class attribut_inf : public attribut_entity_val<T> {
protected:
    using attribut_entity_val<T>::m_valeur;
public:
    CONSTR_DEFAUT(attribut_inf)
    //! Constructeur.
    using attribut_entity_val<T>::attribut_entity_val;
    CONSTR_AFFECT_DEFAUT(attribut_inf)

    //! Destruteur.
    ~attribut_inf() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return m_valeur < N;}
};

template<class T, T N> attribut_inf<T,N>::~attribut_inf() = default;

template<idt N> using attribut_id_inf = attribut_inf<idt, N>;
template<int N> using attribut_int_inf = attribut_inf<int, N>;
template<unsigned N> using attribut_unsigned_inf = attribut_inf<unsigned, N>;

//! Classe mère des attributs de style de numérotation.
using attribut_style_num = attribut_unsigned_inf<divers::num_to_string::Nbr_Style>;

/*! \ingroup groupe_attribut_entity
 *\brief _template des attributs de type entier compris entre M (inclus) et N (exclut).
 */
template<int M, int N> class attribut_encadre : public attribut_int {
public:
    CONSTR_DEFAUT(attribut_encadre)
    CONSTR_AFFECT_DEFAUT(attribut_encadre)

    //! Destruteur.
    ~attribut_encadre() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override
        {return M <= m_valeur &&m_valeur < N;}
};

template<int M, int N> attribut_encadre<M,N>::~attribut_encadre() = default;

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type decimale.
 */
class attribut_decimale : public attribut_int{
public:
    enum {Not_Find = 0,
          Default_Value = 1,
          Nbr_Values = info_entity::decimale::Nbr_Decimales};

    static const std::array<int,Nbr_Values> Decimale;   //!< Liste des inverses des décimales permises.
    static const std::array<int,Nbr_Values> Precision;  //!< Précision d'affichage de chaque valeur.
    //! Constructeur.
    attribut_decimale() : attribut_int(Default_Value) {}
    CONSTR_AFFECT_DEFAUT(attribut_decimale)

    //! Destructeur.
    ~attribut_decimale() override;

    //! Renvoie l'inverse de la valeur de Decimale d'indice indice.
    static double Atome(szt indice)
        {return 1.0 / Decimale.at(indice);}

    //! Renvoie l'inverse de m_valeur.
    double atome() const
        {return 1.0 / m_valeur;}

    //! Renvoie l'indice de valeur dans la list Decimale.
    static szt Indice(int valeur);

    //! Renvoie l'indice de m_valeur dans la liste Decimale.
    szt indice() const
    {return Indice(m_valeur);}

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final override;

    //! Précision en nombre de chiffre à virgule.
    static int Precision_decimale(int valeur);
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type code.
 */
class attribut_code : public attribut_entity {
public:
    using att_type = flag;
    using att_trans = flag;
protected:
    att_type m_valeur;     //!< Valeur de l'attribut.

public:
    using flag_type =flag::flag_type;
    CONSTR_DEFAUT(attribut_code)
    CONSTR_AFFECT_DEFAUT(attribut_code)

    //! Destructeur.
    ~attribut_code() override;

    //! Ajout de valeurs.
    void add(flag_type valeur)
        {m_valeur |= valeur;}

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant data_v(post /*pos*/) const override
        {return m_valeur.value();}

    //! Accesseur de l'attribut.
    flag get() const
        {return m_valeur;}

    //! Retrait de valeur.
    void del(flag_type valeur) {
        if(valeur)
            m_valeur &= ~valeur;
    }

    //! Accesseur de l'attribut pour la base de données.
    QVariant get_to_bdd() const override
        {return m_valeur.value();}

    //! retourne la liste des valeurs contenue
    std::vector<flag_type> list() const {
        std::vector<flag_type> L;
        for(flag_type i = 1; i <= sizeof (flag_type); ++i) {
            flag::flag_type val = 1<<i;
            if(test(val))
                L.push_back(val);
        }
        return L;
    }

    //! Mutateur de l'attribut.
    void set(flag valeur)
        {m_valeur = valeur;}

    //! Modifie la donnée à partir d'un QVariant.
    void set_data_v(post /*pos*/, const QVariant &value) override
        {set(value.toUInt());}

    //! Teste si l'intersection est non vide.
    bool test(flag_type valeur) const
        {return m_valeur.test(valeur);}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString to_string_attribut() const override
        {return QVariant(m_valeur.value()).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const attribut_code &att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (flag valeur) const
        {return m_valeur == valeur;}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type couleur.
 */
class attribut_couleur : public attribut_string {
public:
    CONSTR_DEFAUT(attribut_couleur)
    CONSTR_AFFECT_DEFAUT(attribut_couleur)

    //!Destructeur.
    ~attribut_couleur() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final
        {return QColor::isValidColor(m_valeur);}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type brush.
 */
class attribut_brush : public attribut_unsigned {
public:
    enum {Max = 17};
    CONSTR_DEFAUT(attribut_brush)
    CONSTR_AFFECT_DEFAUT(attribut_brush)

    //!Destructeur.
    ~attribut_brush() override;

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final
        {return m_valeur < Max;}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type QString non vide.
 */
class attribut_string_not_empty : public attribut_string {
public:
    CONSTR_DEFAUT(attribut_string_not_empty)
    CONSTR_AFFECT_DEFAUT(attribut_string_not_empty)

    //!Destructeur.
    ~attribut_string_not_empty();

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final
        {return !m_valeur.isEmpty();}
};

/*! \ingroup groupe_attribut_entity
 *\brief Classe mère des attributs de type référence.
 */
class Attribut_ref : public attribut_string {
public:
    CONSTR_DEFAUT(Attribut_ref)
    CONSTR_AFFECT_DEFAUT(Attribut_ref)

    //! Expression régulière de vérification d'une référence.
    static const QRegularExpression Reg;

    //!Destructeur.
    ~Attribut_ref();

    //! Teste la validité de la valeur.
    bool is_valid_attribut() const final
        {return m_valeur.isNull() || Reg.match(m_valeur).hasMatch();}
};

SINGLE_ATTRIBUT(bordure_attribut,attribut_int,Bordure,bordure)
SINGLE_ATTRIBUT(card_attribut,attribut_int,Card,card)
SINGLE_ATTRIBUT(cible_attribut,attribut_int,Cible,cible)
SINGLE_ATTRIBUT(code_attribut,attribut_code,Code,code)
SINGLE_ATTRIBUT(couleur_attribut,attribut_couleur,Couleur,couleur)
SINGLE_ATTRIBUT(couleur_bordure_attribut,attribut_couleur,Couleur_Bordure,couleur_bordure)
SINGLE_ATTRIBUT(couleur_fond_attribut,attribut_couleur,Couleur_Fond,couleur_fond)
SINGLE_ATTRIBUT(couleur_texte_attribut,attribut_couleur,Couleur_Texte,couleur_texte)
SINGLE_ATTRIBUT(couleur_titre_attribut,attribut_couleur,Couleur_Titre,couleur_titre)
SINGLE_ATTRIBUT(creation_attribut,attribut_date_time_valide,Creation,creation)
SINGLE_ATTRIBUT(date_valide_attribut,attribut_date_valide,Date,date)
SINGLE_ATTRIBUT(date_time_current_attribut,attribut_date_time_current,Date_Time,date_time)
SINGLE_ATTRIBUT(date_time_valide_attribut,attribut_date_time_valide,Date_Time,date_time)
SINGLE_ATTRIBUT(debut_attribut,attribut_date_time_valide,Debut,debut)
SINGLE_ATTRIBUT(decimale_attribut,attribut_decimale,Decimale,decimale)
SINGLE_ATTRIBUT(etat_attribut,attribut_int,Etat,etat)
SINGLE_ATTRIBUT(exact_attribut,attribut_code,Exact,exact)
SINGLE_ATTRIBUT(feuille_attribut,attribut_bool,Feuille,feuille)
SINGLE_ATTRIBUT(fin_attribut,attribut_date_time_valide,Fin,fin)
SINGLE_ATTRIBUT(forme_attribut,attribut_int,Forme,forme)
SINGLE_ATTRIBUT(id_attribut,Attribut_id,Id,id)
SINGLE_ATTRIBUT(id_cible_attribut,Attribut_id_sup<1>,Id_Cible,id_cible)
SINGLE_ATTRIBUT(id_etat_attribut,Attribut_id_sup<1>,Id_Etat,id_etat)
SINGLE_ATTRIBUT(id_prog_attribut,Attribut_id_null,Id_Prog,id_prog)
SINGLE_ATTRIBUT_ID(id_1_attribut,Attribut_id_sup<1>,1)
SINGLE_ATTRIBUT_ID(id_1_neg_attribut,attribut_int,1)
SINGLE_ATTRIBUT_ID(id_1_null_attribut,Attribut_id_null,1)
SINGLE_ATTRIBUT_ID(id_1_zero_attribut,Attribut_id,1)
SINGLE_ATTRIBUT_ID(id_2_attribut,Attribut_id_sup<1>,2)
SINGLE_ATTRIBUT_ID(id_2_neg_attribut,attribut_int,2)
SINGLE_ATTRIBUT_ID(id_2_null_attribut,Attribut_id_null,2)
SINGLE_ATTRIBUT_ID(id_3_attribut,Attribut_id_sup<1>,3)
SINGLE_ATTRIBUT_ID(id_3_null_attribut,Attribut_id_null,3)
SINGLE_ATTRIBUT_ID(id_4_attribut,Attribut_id_sup<1>,4)
SINGLE_ATTRIBUT_ID(id_5_attribut,Attribut_id_sup<1>,5)
SINGLE_ATTRIBUT(nc_attribut,attribut_string_not_empty,Nc,nc)
SINGLE_ATTRIBUT(nom_attribut,attribut_string_not_empty,Nom,nom)
SINGLE_ATTRIBUT(modif_attribut,attribut_code,Modif,modif)
SINGLE_ATTRIBUT(modification_attribut,attribut_date_time_current,Modification,modification)
SINGLE_ATTRIBUT(num_attribut,attribut_int_sup<0>,Num,num)
SINGLE_ATTRIBUT(parent_attribut,Attribut_id_null,Parent,parent)
SINGLE_ATTRIBUT(police_attribut,attribut_string_not_empty,Font,font)
SINGLE_ATTRIBUT(police_texte_attribut,attribut_string_not_empty,Police_Texte,police_texte)
SINGLE_ATTRIBUT(police_titre_attribut,attribut_string_not_empty,Police_Titre,police_titre)
SINGLE_ATTRIBUT(ref_attribut,Attribut_ref,Ref,ref)
SINGLE_ATTRIBUT(saisie_attribut,attribut_code,Saisie,saisie)
SINGLE_ATTRIBUT(style_num_attribut,attribut_style_num,Style_Num,style_num)
SINGLE_ATTRIBUT(texte_attribut,attribut_string,Texte,texte)
SINGLE_ATTRIBUT(texture_attribut,attribut_brush,Texture,texture)
SINGLE_ATTRIBUT(titre_attribut,attribut_string_not_empty,Titre,titre)
SINGLE_ATTRIBUT(total_attribut,attribut_int_sup<1>,Total,total)
SINGLE_ATTRIBUT(tp_val_attribut,attribut_int,Tp_Val,tp_val)
SINGLE_ATTRIBUT(type_attribut,Attribut_id_sup<1>,Type,type)
SINGLE_ATTRIBUT(type_id_attribut,Attribut_id,Type,type)
SINGLE_ATTRIBUT(valeur_double_attribut,attribut_double,Valeur,valeur)
SINGLE_ATTRIBUT(valeur_int_attribut,attribut_int,Valeur,valeur)
SINGLE_ATTRIBUT(valeur_variant_attribut,attribut_variant,Valeur,valeur)
SINGLE_ATTRIBUT(version_attribut,attribut_int_sup<0>,Version,version)
}}
#endif // ATTRIBUT_SIMPLE_H
