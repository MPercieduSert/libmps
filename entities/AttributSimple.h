/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef ATTRIBUTSIMPLE_H
#define ATTRIBUTSIMPLE_H

#include <array>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QVariant>
#include <utility>
#include <cmath>
#include "infoEntity.h"
#include "macrolibmps.h"

/*! \defgroup groupeAttributEntity Attributs des entités
 * \ingroup groupeEntity
 * \brief Ensemble de classes représentant les attributs des entités de la base de donnée.
 */

//! \ingroup groupeAttributEntity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    idt id ## NOM () const {return id ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (idt n) {setId ## NUM (n);} \
    /*! Nom de identifiant*/ \
    QString nameId ## NUM() const override {return "Id"#NOM;}

//! \ingroup groupeAttributEntity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE_NEG(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    int id ## NOM () const {return id ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (int n) {setId ## NUM (n);} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (szt n) {setId ## NUM (static_cast<int>(n));} \
    /*! Nom de identifiant*/ \
    QString nameId ## NUM() const override {return "Id"#NOM;}

/*! \ingroup groupeAttributEntity
 * \brief Début des macros de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,NOM,nom) /*! \ingroup groupeAttributEntity \brief Classe de l'attribut ATTRIBUT.*/ \
    class ATTRIBUT: public MERE { \
    public: \
    using MERE::get; \
    using MERE::set; \
    CONSTR_DEFAUT(ATTRIBUT) \
    CONSTR_AFFECT_DEFAUT(ATTRIBUT) \
    /*! Destructeur.*/ \
    ~ATTRIBUT() override; \
    /*! Positions des attributs.*/ \
    enum Position {NOM, NbrAtt}; \
    /*! Nom de l'attribut.*/ \
    static QString NameAttribut(szt /*pos*/=0) {return #NOM;} \
    /*! Nom de la classe de l'attribut.*/ \
    QString nameClasseAttribut() const override {return #ATTRIBUT;} \
    /*! Accesseur de l'attribut nom.*/ \
    ATTRIBUT::AttTrans nom() const {return get();} \
    /*! Mutateur de l'attribut.*/ \
    void set(const ATTRIBUT & entity) {set(entity.get());}\
    /*! Mutateur de l'attribut nom.*/ \
    void set ## NOM(ATTRIBUT::AttTrans valeur) {set(valeur);} \
    /*! Opérateur égalité */ \
    bool operator ==(const ATTRIBUT & entity) const {return MERE::operator ==(entity);}

/*! \ingroup groupeAttributEntity
 * \brief Macro de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT(ATTRIBUT,MERE,NOM,nom) /*! \ingroup groupeAttributEntity \brief Classe de l'attribut ATTRIBUT.*/ \
    SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,NOM,nom) \
    /*! Nom de l'attribut.*/ \
    QString nameAtt() const override {return #NOM;}};

/*! \ingroup groupeAttributEntity
 * \brief Macro de déclaration d'un attribut de type clé dans les entités.
 */
#define SINGLE_ATTRIBUT_ID(ATTRIBUT,MERE,NUM) /*! \ingroup groupeAttributEntity \brief Classe de l'attribut ATTRIBUT.*/ \
    SINGLE_ATTRIBUT_DEBUT(ATTRIBUT,MERE,Id ## NUM,id ## NUM) \
    /*! Nom de l'attribut.*/ \
    QString nameAtt() const override {return nameId ## NUM();}\
    /*! Nom de identifiant*/ \
    virtual QString nameId ## NUM() const {return "Id"#NUM;}};

/*! \ingroup groupeAttributEntity
 * \brief Espace de nom des attributs des entités.
 */
namespace attributMPS {

using namespace typeMPS;

/*! \ingroup groupeAttributEntity
 * \brief Classe signalant la fin d'une branche d'attribut.
 */
class NoAttribut;

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs (simple) des entités.
 */
class AttributEntity {
public:
    using AttPre = NoAttribut;
    using AttSuiv = NoAttribut;
    CONSTR_DEFAUT(AttributEntity)

    //! Destructeur.
    virtual ~AttributEntity();

    //! Renvoie une chaine de caractère contenant le nom, la valeur est la validité de l'attribut.
    virtual QString affiche() const;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    virtual QVariant dataP(szt /*pos*/) const = 0;

    //! Accesseur de l'attribut pour la base de données.
    virtual QVariant getToBdd() const = 0;

    //! Renvoie la validité de la valeur.
    virtual bool isValid() const
        {return isValidAttribut();}

    //! Renvoie la validité de la valeur.
    virtual bool isValidAttribut() const
        {return true;}

    //! Renvoie le nom de l'attribut.
    virtual QString nameAtt() const = 0;

    //! Renvoie le nom de l'attribut.
    QString attributName(szt /*Pos*/) const
        {return nameAtt();}

    //! Renvoie le nom de l'attribut.
    virtual QString nameClasseAttribut() const = 0;

    //! Modifie la donnée à partir d'un QVariant.
    virtual void setDataP(szt /*pos*/, const QVariant & value) = 0;

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    virtual QString toString() const
        {return toStringAttribut();}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    virtual QString toStringAttribut() const = 0;

    //! Renvoie la chaine "valide" si l'attribut est valide et "invalide" sinon.
    QString validToString() const
        {return isValidAttribut() ? "valide" : "invalide";}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe patron des attribut avec une transimision par reférence dans les accesseurs et mutateurs.
 */
template<class Var, class Trans> class AttributEntityTemp : public AttributEntity {
public:
    using AttType = Var;
    using AttTrans = Trans;
protected:
    AttType m_valeur;     //!< Valeur de l'attribut.

public:
    AttributEntityTemp() {}
    CONSTR_AFFECT_DEFAUT(AttributEntityTemp)

    //! Constructeur, donner la valeur de l'attribut en argument.
    AttributEntityTemp(AttTrans valeur)
        : m_valeur(valeur) {}

    //! Destructeur.
    ~AttributEntityTemp() override;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(szt /*pos*/) const override
        {return m_valeur;}

    //! Accesseur de l'attribut.
    AttTrans get() const
        {return m_valeur;}

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const override
        {return m_valeur;}

    //! Mutateur de l'attribut.
    void set(Trans valeur)
        {m_valeur = valeur;}

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(szt /*pos*/, const QVariant & value) override
        {set(value.value<AttType>());}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString toStringAttribut() const override
        {return QVariant(m_valeur).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const AttributEntityTemp<Var,Trans> & att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (AttTrans valeur) const
        {return m_valeur == valeur;}
};

template<class Var, class Trans> AttributEntityTemp<Var,Trans>::~AttributEntityTemp() = default;

// Spécialisation partielle.
template<class T> using AttributEntityVal = AttributEntityTemp<T,T>;
template<class T> using AttributEntityRef = AttributEntityTemp<T,const T &>;

// Spécialisation des constructeur fixant une valeur par défaut pour les types de base.
template<> inline AttributEntityVal<bool>::AttributEntityTemp() : m_valeur(true) {}
template<> inline AttributEntityVal<double>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<float>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<int>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<idt>::AttributEntityTemp() : m_valeur(0) {}

// Spécilisation du setDatat pour les Variants.
template<> inline void AttributEntityRef<QVariant>::setDataP(szt /*pos*/, const QVariant & value) {set(value);}

// Alis de spécialiations totales.
using AttributBool = AttributEntityVal<bool>;
using AttributDouble = AttributEntityVal<double>;
using AttributFloat = AttributEntityVal<float>;
using AttributInt = AttributEntityVal<int>;
using AttributUnsigned = AttributEntityVal<unsigned>;
using AttributId = AttributEntityVal<idt>;

using AttributDate = AttributEntityRef<QDate>;
using AttributDateTime = AttributEntityRef<QDateTime>;
using AttributString = AttributEntityRef<QString>;
using AttributVariant = AttributEntityRef<QVariant>;

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date current.
 */
class AttributDateCurrent: public AttributDate {
public:
    CONSTR_DEFAUT(AttributDateCurrent)
    CONSTR_AFFECT_DEFAUT(AttributDateCurrent)

    //! Destructeur.
    ~AttributDateCurrent() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const override
        {return QDate::currentDate();}

    //! Accesseur de l'attribut pour la base de données.
    const QDate & getToBdd()
        {return m_valeur = QDate::currentDate();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date pouvant être nulle.
 */
class AttributDateNull : public AttributDate {
public:
    CONSTR_DEFAUT(AttributDateNull)
    CONSTR_AFFECT_DEFAUT(AttributDateNull)

    //! Destructeur.
    ~AttributDateNull() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur.isNull() || m_valeur.isValid();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date valide.
 */
class AttributDateValide : public AttributDate {
public:
    CONSTR_DEFAUT(AttributDateValide)
    CONSTR_AFFECT_DEFAUT(AttributDateValide)

    //! Destructeur.
    ~AttributDateValide() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur.isValid();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime current.
 */
class AttributDateTimeCurrent: public AttributDateTime {
public:
    CONSTR_DEFAUT(AttributDateTimeCurrent)
    CONSTR_AFFECT_DEFAUT(AttributDateTimeCurrent)

    //! Destructeur.
    ~AttributDateTimeCurrent() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const override
        {return QDateTime::currentDateTime();}

    //! Accesseur de l'attribut pour la base de données.
    const QDateTime & getToBdd()
        {return m_valeur = QDateTime::currentDateTime();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime nulle.
 */
class AttributDateTimeNull : public AttributDateTime {
public:
    CONSTR_DEFAUT(AttributDateTimeNull)
    CONSTR_AFFECT_DEFAUT(AttributDateTimeNull)

    //! Destructeur.
    ~AttributDateTimeNull() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur.isNull() || m_valeur.isValid();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime valide.
 */
class AttributDateTimeValide : public AttributDateTime {
public:
    CONSTR_DEFAUT(AttributDateTimeValide)
    CONSTR_AFFECT_DEFAUT(AttributDateTimeValide)

    //! Destructeur.
    ~AttributDateTimeValide() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur.isValid();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type entier positif renvoyant Null pour zéro dans la base de données.
 */
template<class T> class AttributZeroNull : public AttributEntityVal<T> {
protected:
    using AttributEntityVal<T>::m_valeur;
public:
    enum {Zero = 0};
    CONSTR_DEFAUT(AttributZeroNull)
    CONSTR_AFFECT_DEFAUT(AttributZeroNull)

    //! Destructeur.
    ~AttributZeroNull() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const override
        {return m_valeur == Zero ? QVariant(QVariant::UInt) : m_valeur;}
};

template<class T> AttributZeroNull<T>::~AttributZeroNull<T>() = default;

using AttributIdNull = AttributZeroNull<idt>;
using AttributUnsignedNull = AttributZeroNull<unsigned>;

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier supérieur ou égal à N.
 */
template<class T, int N> class AttributSup : public AttributEntityVal<T> {
protected:
    using AttributEntityVal<T>::m_valeur;
public:
    CONSTR_DEFAUT(AttributSup)
    //! Constructeur.
    using AttributEntityVal<T>::AttributEntityVal;
    CONSTR_AFFECT_DEFAUT(AttributSup)

    //! Destruteur.
    ~AttributSup() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur >= N;}
};

template<class T, int N> AttributSup<T,N>::~AttributSup() = default;

template<int N> using AttributIdSup = AttributSup<idt, N>;
template<int N> using AttributIntSup = AttributSup<int, N>;
template<int N> using AttributUnsignedSup = AttributSup<unsigned, N>;

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier inférieur ou égal à N.
 */
template<int N> class AttributIntInf : public AttributInt {
public:
    CONSTR_DEFAUT(AttributIntInf)
    CONSTR_AFFECT_DEFAUT(AttributIntInf)

    //! Destruteur.
    ~AttributIntInf() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur <= N;}
};

template<int N> AttributIntInf<N>::~AttributIntInf() = default;

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier compris entre M (inclus) et N (exclut).
 */
template<int M, int N> class AttributEncadre : public AttributInt {
public:
    CONSTR_DEFAUT(AttributEncadre)
    CONSTR_AFFECT_DEFAUT(AttributEncadre)

    //! Destruteur.
    ~AttributEncadre() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return M <= m_valeur && m_valeur < N;}
};

template<int M, int N> AttributEncadre<M,N>::~AttributEncadre() = default;

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type decimale.
 */
class AttributDecimale : public AttributEncadre<0,infoEntity::decimale::NbrDecimales> {
public:
    enum {NotFind = -1,
         NbrValues = infoEntity::decimale::NbrDecimales};

    static const std::array<int, NbrValues> Decimale;   //! Liste des inverses des décimales permises.
    CONSTR_DEFAUT(AttributDecimale)
    CONSTR_AFFECT_DEFAUT(AttributDecimale)

    //! Destructeur.
    ~AttributDecimale() override;

    //! Renvoie l'inverse de la valeur de Decimale d'indice indice.
    static double atome(int indice)
        {return 1.0 / Decimale.at(static_cast<uint>(indice));}

    //! Renvoie l'inverse de m_valeur.
    double atome() const
        {return 1.0 / m_valeur;}

    //! Renvoie l'indice de valeur dans la list Decimale.
    static int indice(int valeur) {
        auto i = 0;
        while(i != NbrValues && Decimale[static_cast<uint>(i)] != valeur)
            ++i;
        return i;
    }

    //! Renvoie l'indice de m_valeur dans la list Decimale.
    int indice() const
    {return indice(m_valeur);}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type code.
 */
class AttributCode : public AttributEntity {
public:
    using AttType = flag;
    using AttTrans = flag;
protected:
    AttType m_valeur;     //!< Valeur de l'attribut.

public:
    using flag_type =flag::flag_type;
    CONSTR_DEFAUT(AttributCode)
    CONSTR_AFFECT_DEFAUT(AttributCode)

    //! Destructeur.
    ~AttributCode() override;

    //! Ajout de valeurs.
    void add(flag_type valeur)
        {m_valeur |= valeur;}

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(szt /*pos*/) const override
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
    QVariant getToBdd() const override
        {return m_valeur.value();}

    //! retourne la liste des valeurs contenue
    std::vector<unsigned> list() const {
        std::vector<unsigned> L;
        for(unsigned i = 1; i <= sizeof (unsigned); ++i) {
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
    void setDataP(szt /*pos*/, const QVariant & value) override
        {set(value.toUInt());}

    //! Teste si l'intersection est non vide.
    bool test(flag_type valeur) const
        {return m_valeur.test(valeur);}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString toStringAttribut() const override
        {return QVariant(m_valeur.value()).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const AttributCode & att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (flag valeur) const
        {return m_valeur == valeur;}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type QString non vide.
 */
class AttributStringNotEmpty : public AttributString {
public:
    CONSTR_DEFAUT(AttributStringNotEmpty)
    CONSTR_AFFECT_DEFAUT(AttributStringNotEmpty)

    //!Destructeur.
    ~AttributStringNotEmpty();

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final
        {return !m_valeur.isEmpty();}
};

SINGLE_ATTRIBUT(AlphaAttribut,AttributInt,Alpha,alpha)
SINGLE_ATTRIBUT(CardAttribut,AttributInt,Card,card)
SINGLE_ATTRIBUT(CibleAttribut,AttributInt,Cible,cible)
SINGLE_ATTRIBUT(CodeAttribut,AttributCode,Code,code)
SINGLE_ATTRIBUT(CreationAttribut,AttributDateTimeValide,Creation,creation)
SINGLE_ATTRIBUT(DateValideAttribut,AttributDateValide,Date,date)
SINGLE_ATTRIBUT(DateTimeCurrentAttribut,AttributDateTimeCurrent,DateTime,dateTime)
SINGLE_ATTRIBUT(DateTimeValideAttribut,AttributDateTimeValide,DateTime,dateTime)
SINGLE_ATTRIBUT(DecimaleAttribut,AttributDecimale,Decimale,decimale)
SINGLE_ATTRIBUT(EtatAttribut,AttributInt,Etat,etat)
SINGLE_ATTRIBUT(ExactAttribut,AttributCode,Exact,exact)
SINGLE_ATTRIBUT(FeuilleAttribut,AttributBool,Feuille,feuille)
SINGLE_ATTRIBUT(IdAttribut,AttributId,Id,id)
SINGLE_ATTRIBUT(IdCibleAttribut,AttributIdSup<1>,IdCible,idCible)
SINGLE_ATTRIBUT(IdEtatAttribut,AttributIdSup<1>,IdEtat,idEtat)
SINGLE_ATTRIBUT(IdProgAttribut,AttributIdNull,IdProg,idProg)
SINGLE_ATTRIBUT_ID(Id1Attribut,AttributIdSup<1>,1)
SINGLE_ATTRIBUT_ID(Id1NegAttribut,AttributInt,1)
SINGLE_ATTRIBUT_ID(Id1NullAttribut,AttributIdNull,1)
SINGLE_ATTRIBUT_ID(Id2Attribut,AttributIdSup<1>,2)
SINGLE_ATTRIBUT_ID(Id2NegAttribut,AttributInt,2)
SINGLE_ATTRIBUT_ID(Id2NullAttribut,AttributIdNull,2)
SINGLE_ATTRIBUT_ID(Id3Attribut,AttributIdSup<1>,3)
SINGLE_ATTRIBUT_ID(Id3NullAttribut,AttributIdNull,3)
SINGLE_ATTRIBUT_ID(Id4Attribut,AttributIdSup<1>,4)
SINGLE_ATTRIBUT_ID(Id5Attribut,AttributIdSup<1>,5)
SINGLE_ATTRIBUT(NcAttribut,AttributStringNotEmpty,Nc,nc)
SINGLE_ATTRIBUT(NomAttribut,AttributStringNotEmpty,Nom,nom)
SINGLE_ATTRIBUT(ModifAttribut,AttributCode,Modif,modif)
SINGLE_ATTRIBUT(ModificationAttribut,AttributDateTimeCurrent,Modification,modification)
SINGLE_ATTRIBUT(NumAttribut,AttributIntSup<0>,Num,num)
SINGLE_ATTRIBUT(ParentAttribut,AttributIdNull,Parent,parent)
SINGLE_ATTRIBUT(SaisieAttribut,AttributCode,Saisie,saisie)
SINGLE_ATTRIBUT(TexteAttribut,AttributString,Texte,texte)
SINGLE_ATTRIBUT(TotalAttribut,AttributInt,Total,total)
SINGLE_ATTRIBUT(TpValAttribut,AttributInt,TpVal,tpVal)
SINGLE_ATTRIBUT(TypeAttribut,AttributIdSup<1>,Type,type)
SINGLE_ATTRIBUT(ValeurDoubleAttribut,AttributDouble,Valeur,valeur)
SINGLE_ATTRIBUT(ValeurIntAttribut,AttributInt,Valeur,valeur)
SINGLE_ATTRIBUT(ValeurVariantAttribut,AttributVariant,Valeur,valeur)
SINGLE_ATTRIBUT(VersionAttribut,AttributIntSup<0>,Version,version)
}

#endif // ATTRIBUTSIMPLE_H
