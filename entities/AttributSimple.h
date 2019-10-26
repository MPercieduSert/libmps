/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef ATTRIBUTSIMPLE_H
#define ATTRIBUTSIMPLE_H

#include <array>
#include <QDate>
//#include <QMessageBox>
#include <QString>
#include <QVariant>
#include <utility>
#include <cmath>
#include "macrolibmps.h"



/*! \defgroup groupeAttributEntity Attributs des entités
 * \ingroup groupeEntity
 * \brief Ensemble de classes représentant les attributs des entités de la base de donnée.
 */

//! \ingroup groupeAttributEntity
//! Type des attributs
using idt = unsigned;
using codeType = unsigned;

//! \ingroup groupeAttributEntity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    idt id ## NOM () const {return id ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (idt n) {setId ## NUM (n);}

//! \ingroup groupeAttributEntity
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE_NEG(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    int id ## NOM () const {return id ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (int n) {setId ## NUM (n);}

/*! \ingroup groupeAttributEntity
 * \brief Macro de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT(ATTRIBUT,MERE,NOM,nom) /*! \ingroup groupeAttributEntity \brief Classe de l'attribut ATTRIBUT.*/ \
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
    /*! Nom de la classe de l'attribut.*/ \
    QString nameAttribut() const override {return #ATTRIBUT;} \
    /*! Nom de l'attribut.*/ \
    static QString nomAttribut(int /*pos*/=0) {return #NOM;} \
    /*! Accesseur de l'attribut nom.*/ \
    ATTRIBUT::AttType nom() const {return get();} \
    /*! Mutateur de l'attribut.*/ \
    void set(const ATTRIBUT & entity) {set(entity.get());}\
    /*! Mutateur de l'attribut nom.*/ \
    void set ## NOM(ATTRIBUT::AttType valeur) {set(valeur);} \
    /*! Opérateur égalité */ \
    bool operator ==(const ATTRIBUT & entity) const {return MERE::operator ==(entity);}};

/*! \ingroup groupeAttributEntity
 * \brief Classe signalant la fin d'une branche d'attribut.
 */
class NoAttribut;

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs (simple) des entités.
 */
class AttributEntity
{
public:
    using AttPre = NoAttribut;
    using AttSuiv = NoAttribut;
    CONSTR_DEFAUT(AttributEntity)

    //! Destructeur.
    virtual ~AttributEntity();

    //! Renvoie une chaine de caractère contenant le nom, la valeur est la validité de l'attribut.
    virtual QString affiche() const;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    virtual QVariant dataP(int /*pos*/) const = 0;

    //! Renvoie la validité de la valeur.
    virtual bool isValid() const
        {return isValidAttribut();}

    //! Renvoie la validité de la valeur.
    virtual bool isValidAttribut() const
        {return true;}

    //! Renvoie le nom de l'attribut.
    virtual QString nameAttribut() const = 0;

    //! Modifie la donnée à partir d'un QVariant.
    virtual void setDataP(const QVariant & value, int /*pos*/) = 0;

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
template<class Var, class Trans> class AttributEntityTemp : public AttributEntity
{
public:
    using AttType = Var;

protected:
    AttType m_valeur;     //!< Valeur de l'attribut.

public:
    AttributEntityTemp() {}
    CONSTR_AFFECT_DEFAUT(AttributEntityTemp)

    //! Constructeur, donner la valeur de l'attribut en argument.
    AttributEntityTemp(Trans valeur)
        : m_valeur(valeur)
        {}

    //! Destructeur.
    ~AttributEntityTemp() override;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(int /*pos*/) const override
        {return m_valeur;}

    //! Accesseur de l'attribut.
    Trans get() const
        {return m_valeur;}

    //! Accesseur de l'attribut pour la base de données.
    Trans getToBdd() const
        {return m_valeur;}

    //! Mutateur de l'attribut.
    void set(Trans valeur)
        {m_valeur = valeur;}

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.value<AttType>());}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString toStringAttribut() const override
        {return QVariant(m_valeur).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const AttributEntityTemp<Var,Trans> & att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (Trans valeur) const
        {return m_valeur == valeur;}
};

template<class Var, class Trans> AttributEntityTemp<Var,Trans>::~AttributEntityTemp() = default;

///*! \ingroup groupeAttributEntity
// * \brief Classe patron des attribut avec une transimision par valeur dans les accesseurs et mutateurs.
// */
//template<class T> class AttributEntityVal : public AttributEntity
//{
//protected:
//    T m_valeur; //!< Valeur de l'attribut.

//public:
//    using AttType = T;
//    CONSTR_AFFECT_DEFAUT(AttributEntityVal)

//    //! Constructeur, donner la valeur de l'attribut en argument.
//    AttributEntityVal(T valeur)
//        : m_valeur(valeur)
//        {}

//    //!Destructeur.
//    ~AttributEntityVal() override;

//    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
//    QVariant dataP(int /*pos*/) const override
//        {return m_valeur;}

//    //! Accesseur de l'attribut.
//    T get() const
//        {return m_valeur;}

//    //! Accesseur de l'attribut pour la base de données.
//    T getToBdd() const
//        {return m_valeur;}

//    //! Mutateur de l'attribut.
//    void set(T  valeur)
//        {m_valeur = valeur;}

//    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
//    QString toStringAttribut() const override
//        {return QVariant(m_valeur).toString();}

//    //! Teste l'égalité entre les deux attributs.
//    bool operator == (const AttributEntityVal<T> & entity) const
//        {return m_valeur == entity.m_valeur;}

//    //! Teste l'égalité entre les deux attributs.
//    bool operator == (T valeur) const
//        {return m_valeur == valeur;}
//};

//template<class T> AttributEntityVal<T>::~AttributEntityVal() = default;

template<class T> using AttributEntityVal = AttributEntityTemp<T,T>;
template<class T> using AttributEntityRef = AttributEntityTemp<T,const T &>;

template<> inline AttributEntityVal<bool>::AttributEntityTemp() : m_valeur(true) {}
template<> inline AttributEntityVal<double>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<float>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<int>::AttributEntityTemp() : m_valeur(0) {}
template<> inline AttributEntityVal<idt>::AttributEntityTemp() : m_valeur(0) {}
template<> inline void AttributEntityRef<QVariant>::setDataP(const QVariant & value, int /*pos*/) {set(value);}

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


///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type Bool.
// */
//class AttributBool : public AttributEntityVal<bool>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributBool)

//    //! Constructeur.
//    AttributBool()
//        :AttributEntityVal<bool>(true)
//        {}

//    //! Destructeur.
//    ~AttributBool() override;
//};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type date.
// */
//class AttributDate : public AttributEntityRef<QDate>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributDate)

//    //! Constructeur.
//    AttributDate()
//        :AttributEntityRef<QDate>(QDate())
//        {}

//    //! Destructeur.
//    ~AttributDate() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, int /*pos*/) override
//        {set(value.toDate());}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date pouvant être nulle.
 */
class AttributDateNull : public AttributDate
{
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
class AttributDateValide : public AttributDate
{
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
 * \brief Classe mère des attributs de type date current.
 */
class AttributDateCurrent: public AttributDate
{
public:
    CONSTR_DEFAUT(AttributDateCurrent)
    CONSTR_AFFECT_DEFAUT(AttributDateCurrent)

    //! Destructeur.
    ~AttributDateCurrent() override;

    //! Accesseur de l'attribut pour la base de données.
    QDate getToBdd() const
        {return QDate::currentDate();}

    //! Accesseur de l'attribut pour la base de données.
    const QDate & getToBdd()
        {return m_valeur = QDate::currentDate();}
};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type date pouvant être nulle.
// */
//class AttributDateTime : public AttributEntityRef<QDateTime>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributDateTime)

//    //! Constructeur.
//    AttributDateTime()
//        :AttributEntityRef<QDateTime>(QDateTime())
//        {}

//    //! Destructeur.
//    ~AttributDateTime() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, int /*pos*/) override
//        {set(value.toDateTime());}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime current.
 */
class AttributDateTimeCurrent: public AttributDateTime
{
public:
    CONSTR_DEFAUT(AttributDateTimeCurrent)
    CONSTR_AFFECT_DEFAUT(AttributDateTimeCurrent)

    //! Destructeur.
    ~AttributDateTimeCurrent() override;

    //! Accesseur de l'attribut pour la base de données.
    QDateTime getToBdd() const
        {return QDateTime::currentDateTime();}

    //! Accesseur de l'attribut pour la base de données.
    const QDateTime & getToBdd()
        {return m_valeur = QDateTime::currentDateTime();}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime nulle.
 */
class AttributDateTimeNull : public AttributDateTime
{
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
class AttributDateTimeValide : public AttributDateTime
{
public:
    CONSTR_DEFAUT(AttributDateTimeValide)
    CONSTR_AFFECT_DEFAUT(AttributDateTimeValide)

    //! Destructeur.
    ~AttributDateTimeValide() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur.isValid();}
};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type int.
// */
//class AttributInt : public AttributEntityVal<int>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributInt)

//    //! Constructeur.
//    AttributInt(int n = 0)
//        : AttributEntityVal<int>(n)
//    {}

//    //!Destructeur.
//    ~AttributInt() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, int /*pos*/) override
//        {set(value.toInt());}
//};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type unsigned.
// */
//class AttributUnsigned : public AttributEntityVal<unsigned>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributUnsigned)

//    //! Constructeur.
//    AttributUnsigned(unsigned n = 0)
//        : AttributEntityVal<unsigned>(n)
//    {}

//    //!Destructeur.
//    ~AttributUnsigned() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, unsigned /*pos*/) override
//        {set(value.toUInt());}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type decimale.
 */
class AttributDecimale : public AttributInt
{
public:
    enum {NotFind = -1,
         NbrValues = 16};

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
    static int indice(int valeur)
    {
        auto i = 0;
        while(i != NbrValues && Decimale[static_cast<uint>(i)] != valeur)
            ++i;
        return i;
    }

    //! Renvoie l'indice de m_valeur dans la list Decimale.
    int indice() const
    {return indice(m_valeur);}

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return std::find(Decimale.begin(),Decimale.end(),m_valeur) != Decimale.end();}
};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type entier renvoyant Null pour zéro dans la base de données.
// */
//class AttributIntNull : public AttributInt
//{
//public:
//    CONSTR_DEFAUT(AttributIntNull)
//    CONSTR_AFFECT_DEFAUT(AttributIntNull)

//    //!Destructeur.
//    ~AttributIntNull() override;

//    //! Accesseur de l'attribut pour la base de données.
//    QVariant getToBdd() const
//        {return m_valeur == 0 ? QVariant(QVariant::Int) : m_valeur;}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type entier positif renvoyant Null pour zéro dans la base de données.
 */
template<class T> class AttributZeroNull : public AttributEntityVal<T>
{
protected:
    using AttributEntityVal<T>::m_valeur;
public:
    enum {Zero = 0};
    CONSTR_DEFAUT(AttributZeroNull)
    CONSTR_AFFECT_DEFAUT(AttributZeroNull)

    //! Destructeur.
    ~AttributZeroNull() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const
        {return m_valeur == Zero ? QVariant(QVariant::UInt) : m_valeur;}

//    //! Teste la validité de la valeur.
//    bool isValidAttribut() const final override
//        {return m_valeur >= 0;}
};

template<class T> AttributZeroNull<T>::~AttributZeroNull<T>() = default;

using AttributIdNull = AttributZeroNull<idt>;
using AttributUnsignedNull = AttributZeroNull<unsigned>;

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier supérieur ou égal à N.
 */
template<class T, int N> class AttributSup : public AttributEntityVal<T>
{
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
template<int N> class AttributIntInf : public AttributInt
{
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
 * \brief Classe mère des attributs de type code.
 */
class AttributCode : public AttributEntityVal<codeType>
{
public:
    CONSTR_DEFAUT(AttributCode)
    CONSTR_AFFECT_DEFAUT(AttributCode)

    //! Destructeur.
    ~AttributCode() override;

    //! Ajoute une valeur.
    void add(unsigned valeur)
        {m_valeur|=valeur;}

    //! Retire une valeur.
    void del(unsigned valeur)
        {m_valeur&=~valeur;}

    //! Teste si la valeur est contenue.
    bool in(unsigned valeur) const
        {return !(m_valeur & valeur);}

    QList<int> list() const
    {
        QList<int> L;
        for(unsigned i = 0, j = 1; i < sizeof (unsigned) - 1; ++i, j *= 2)
            if(in(j))
                L.append(static_cast<int>(i));
        return L;
    }
};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type entier intNull renvoyant Null pour -1 dans la base de données.
// */
//class AttributNumNull : public AttributIntSup<-1>
//{
//public:
//    //! Constructeur
//    AttributNumNull()
//        : AttributIntSup(-1)
//    {}
//    CONSTR_AFFECT_DEFAUT(AttributNumNull)

//    //! Destructeur.
//    ~AttributNumNull() override;

//    //! Accesseur de l'attribut pour la base de données.
//    QVariant getToBdd() const
//        {return m_valeur == -1 ? QVariant(QVariant::Int) : m_valeur;}

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, unsigned /*pos*/) override
//        {set(value.isNull() ? -1 : value.toInt());}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type double.
 */
//class AttributDouble : public AttributEntityVal<double>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributDouble)

//    //! Constructeur.
//    AttributDouble()
//        : AttributEntityVal<double>(0)
//        {}

//    //! Destructeur.
//    ~AttributDouble() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, unsigned /*pos*/) override
//        {set(value.toDouble());}
//};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type QString pouvant être vide.
// */
//class AttributString : public AttributEntityRef<QString>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributString)

//    //! Constructeur.
//    AttributString()
//        :AttributEntityRef<QString>(QString())
//        {}

//    //!Destructeur.
//    ~AttributString() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, unsigned /*pos*/) override
//        {set(value.toString());}
//};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type QString non vide.
 */
class AttributStringNotEmpty : public AttributString
{
public:
    CONSTR_DEFAUT(AttributStringNotEmpty)
    CONSTR_AFFECT_DEFAUT(AttributStringNotEmpty)

    //!Destructeur.
    ~AttributStringNotEmpty();

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final
        {return !m_valeur.isEmpty();}
};

///*! \ingroup groupeAttributEntity
// * \brief Classe mère des attributs de type QVariant pouvant être vide.
// */
//class AttributVariant : public AttributEntityRef<QVariant>
//{
//public:
//    CONSTR_AFFECT_DEFAUT(AttributVariant)

//    //! Constructeur.
//    AttributVariant()
//        :AttributEntityRef<QVariant>(QVariant())
//        {}

//    //!Destructeur.
//    ~AttributVariant() override;

//    //! Modifie la donnée à partir d'un QVariant.
//    void setDataP(const QVariant & value, unsigned /*pos*/) override
//        {set(value);}
//};

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
SINGLE_ATTRIBUT(IdAttribut,AttributId,Id,id)
SINGLE_ATTRIBUT(IdCibleAttribut,AttributIdSup<1>,IdCible,idCible)
SINGLE_ATTRIBUT(IdEtatAttribut,AttributIdSup<1>,IdEtat,idEtat)
SINGLE_ATTRIBUT(IdProgAttribut,AttributIdNull,IdProg,idProg)
SINGLE_ATTRIBUT(Id1Attribut,AttributIdSup<1>,Id1,id1)
SINGLE_ATTRIBUT(Id1NegAttribut,AttributInt,Id1,id1)
SINGLE_ATTRIBUT(Id1NullAttribut,AttributIdNull,Id1,id1)
SINGLE_ATTRIBUT(Id2Attribut,AttributIdSup<1>,Id2,id2)
SINGLE_ATTRIBUT(Id2NullAttribut,AttributIdNull,Id2,id2)
SINGLE_ATTRIBUT(Id3Attribut,AttributIdSup<1>,Id3,id3)
SINGLE_ATTRIBUT(Id3NullAttribut,AttributIdNull,Id3,id3)
SINGLE_ATTRIBUT(Id4Attribut,AttributIdSup<1>,Id4,id4)
SINGLE_ATTRIBUT(Id5Attribut,AttributIdSup<1>,Id5,id5)
SINGLE_ATTRIBUT(NcAttribut,AttributStringNotEmpty,Nc,nc)
SINGLE_ATTRIBUT(NomAttribut,AttributStringNotEmpty,Nom,nom)
SINGLE_ATTRIBUT(ModifAttribut,AttributCode,Modif,modif)
SINGLE_ATTRIBUT(ModificationAttribut,AttributDateTimeCurrent,Modification,modification)
SINGLE_ATTRIBUT(NumAttribut,AttributIntSup<0>,Num,num)
//SINGLE_ATTRIBUT(NumNullAttribut,AttributUnsignedNull,Num,num)
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
#endif // ATTRIBUTSIMPLE_H
