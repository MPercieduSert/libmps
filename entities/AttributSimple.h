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
//! Macro définisant les alias de l'accesseur et du mutateurs d'une clé numéroté.
#define ALIAS_CLE(NOM,NUM) /*! \brief Alias de l'accesseur de id ## NUM. */ \
    int id ## NOM () const {return id ## NUM ();} \
    /*! \brief Alias du mutateurs de id ## NUM. */ \
    void setId ## NOM (int n) {setId ## NUM (n);}

/*! \ingroup groupeAttributEntity
 * \brief Macro de déclaration d'un attribut dans les entités.
 */
#define SINGLE_ATTRIBUT(ATTRIBUT,MERE,MERETEMPLATE,NOM,nom,type) /*! \ingroup groupeAttributEntity \brief Classe de l'attribut ATTRIBUT.*/ \
    class ATTRIBUT: public MERETEMPLATE { \
    public: \
    using MERETEMPLATE::get; \
    using MERETEMPLATE::set; \
    CONSTR_DEFAUT(ATTRIBUT) \
    CONSTR_AFFECT_DEFAUT(ATTRIBUT) \
    /*! Destructeur.*/ \
    ~ATTRIBUT() override; \
    /*! Positions des attributs.*/ \
    enum Position {NOM, NbrAtt = 1}; \
    /*! Nom de la classe de l'attribut.*/ \
    QString nameAttribut() const override {return #ATTRIBUT;} \
    /*! Nom de l'attribut.*/ \
    static QString nomAttribut(int /*pos*/=0) {return #NOM;} \
    /*! Accesseur de l'attribut nom.*/ \
    type nom() const {return get();} \
    /*! Mutateur de l'attribut.*/ \
    void set(const ATTRIBUT & entity) {set(entity.get());}\
    /*! Mutateur de l'attribut nom.*/ \
    void set ## NOM(type valeur) {set(valeur);} \
    /*! Opérateur égalité */ \
    bool operator ==(const ATTRIBUT & entity) const {return MERETEMPLATE::operator ==(entity);}};

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
template<class T> class AttributEntityRef : public AttributEntity
{
protected:
    T m_valeur;     //!< Valeur de l'attribut.

public:
    using AttType = T;
    CONSTR_AFFECT_DEFAUT(AttributEntityRef)

    //! Constructeur, donner la valeur de l'attribut en argument.
    AttributEntityRef(const T & valeur)
        : m_valeur(valeur)
        {}

    //! Destructeur.
    ~AttributEntityRef() override;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(int /*pos*/) const override
        {return m_valeur;}

    //! Accesseur de l'attribut.
    const T & get() const
        {return m_valeur;}

    //! Accesseur de l'attribut pour la base de données.
    const T & getToBdd() const
        {return m_valeur;}

    //! Mutateur de l'attribut.
    void set(const T & valeur)
        {m_valeur = valeur;}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString toStringAttribut() const override
        {return QVariant(m_valeur).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const AttributEntityRef<T> & att) const
        {return m_valeur == att.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const T & valeur) const
        {return m_valeur == valeur;}
};

template<class T> AttributEntityRef<T>::~AttributEntityRef() = default;

/*! \ingroup groupeAttributEntity
 * \brief Classe patron des attribut avec une transimision par valeur dans les accesseurs et mutateurs.
 */
template<class T> class AttributEntityVal : public AttributEntity
{
protected:
    T m_valeur; //!< Valeur de l'attribut.

public:
    using AttType = T;
    CONSTR_AFFECT_DEFAUT(AttributEntityVal)

    //! Constructeur, donner la valeur de l'attribut en argument.
    AttributEntityVal(T valeur)
        : m_valeur(valeur)
        {}

    //!Destructeur.
    ~AttributEntityVal() override;

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(int /*pos*/) const override
        {return m_valeur;}

    //! Accesseur de l'attribut.
    T get() const
        {return m_valeur;}

    //! Accesseur de l'attribut pour la base de données.
    T getToBdd() const
        {return m_valeur;}

    //! Mutateur de l'attribut.
    void set(T  valeur)
        {m_valeur = valeur;}

    //! Renvoie une chaine de caractère contenant la valeur de l'attribut.
    QString toStringAttribut() const override
        {return QVariant(m_valeur).toString();}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (const AttributEntityVal<T> & entity) const
        {return m_valeur == entity.m_valeur;}

    //! Teste l'égalité entre les deux attributs.
    bool operator == (T valeur) const
        {return m_valeur == valeur;}
};

template<class T> AttributEntityVal<T>::~AttributEntityVal() = default;

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type Bool.
 */
class AttributBool : public AttributEntityVal<bool>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributBool)

    //! Constructeur.
    AttributBool()
        :AttributEntityVal<bool>(true)
        {}

    //! Destructeur.
    ~AttributBool() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toBool());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date pouvant être nulle.
 */
class AttributDateNull : public AttributEntityRef<QDate>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributDateNull)

    //! Constructeur.
    AttributDateNull()
        :AttributEntityRef<QDate>(QDate())
        {}

    //! Destructeur.
    ~AttributDateNull() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const override
        {return m_valeur.isNull() || m_valeur.isValid();}

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toDate());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type date valide.
 */
class AttributDateValide : public AttributDateNull
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
 * \brief Classe mère des attributs de type date pouvant être nulle.
 */
class AttributDateTime : public AttributEntityRef<QDateTime>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributDateTime)

    //! Constructeur.
    AttributDateTime()
        :AttributEntityRef<QDateTime>(QDateTime())
        {}

    //! Destructeur.
    ~AttributDateTime() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toDateTime());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type datetime valide.
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

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type int.
 */
class AttributInt : public AttributEntityVal<int>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributInt)

    //! Constructeur.
    AttributInt()
        : AttributEntityVal<int>(0)
    {}

    //!Destructeur.
    ~AttributInt() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toInt());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type decimale.
 */
class AttributDecimale : public AttributEntityVal<int>
{
public:
    enum {NotFind = -1,
         NbrValues = 16};

    static const std::array<int, NbrValues> Decimale;   //! Liste des inverses des décimales permises.

    CONSTR_AFFECT_DEFAUT(AttributDecimale)

    //! Constructeur.
    AttributDecimale()
        :AttributEntityVal<int>(1)
        {}

    //! Destructeur.
    ~AttributDecimale() override;

    //! Renvoie l'inverse de la valeur de Decimale d'indice indice.
    static double atome(int indice)
        {return 1.0 / Decimale.at(indice);}

    //! Renvoie l'inverse de m_valeur.
    double atome() const
        {return 1.0 / m_valeur;}

    //! Renvoie l'indice de valeur dans la list Decimale.
    static int indice(int valeur)
    {
        int i = 0;
        while(i != NbrValues && Decimale[i] != valeur)
            ++i;
        if(i != NbrValues)
            return i;
        else
            return NotFind;
    }

    //! Renvoie l'indice de m_valeur dans la list Decimale.
    int indice() const
    {return indice(m_valeur);}

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return std::find(Decimale.begin(),Decimale.end(),m_valeur) != Decimale.end();}

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toInt());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type entier renvoyant Null pour zéro dans la base de données.
 */
class AttributIntNull : public AttributInt
{
public:
    CONSTR_DEFAUT(AttributIntNull)
    CONSTR_AFFECT_DEFAUT(AttributIntNull)

    //!Destructeur.
    ~AttributIntNull() override;

    //! Accesseur de l'attribut pour la base de données.
    QVariant getToBdd() const
        {return m_valeur == 0 ? QVariant(QVariant::Int) : m_valeur;}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type entier positif renvoyant Null pour zéro dans la base de données.
 */
class AttributIntNullPositif : public AttributIntNull
{
public:
    CONSTR_DEFAUT(AttributIntNullPositif)
    CONSTR_AFFECT_DEFAUT(AttributIntNullPositif)

    //! Destructeur.
    ~AttributIntNullPositif() override;

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur >= 0;}

};

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier supérieur ou égal à N.
 */
template<int N> class AttributIntSup : public AttributInt
{
public:
    CONSTR_DEFAUT(AttributIntSup)
    CONSTR_AFFECT_DEFAUT(AttributIntSup)

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur >= N;}
};

/*! \ingroup groupeAttributEntity
 * \brief Template des attributs de type entier inférieur ou égal à N.
 */
template<int N> class AttributIntInf : public AttributInt
{
public:
    CONSTR_DEFAUT(AttributIntInf)
    CONSTR_AFFECT_DEFAUT(AttributIntInf)

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
        {return m_valeur <= N;}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type code.
 */
class AttributCode : public AttributEntityVal<int>
{
public:
    //! Constructeur.
    AttributCode()
        : AttributEntityVal<int>(1)
    {}
    CONSTR_AFFECT_DEFAUT(AttributCode)

    //! Destructeur.
    ~AttributCode() override;

    //! Ajoute une valeur.
    void add(int valeur)
    {
        if(m_valeur%valeur)
            m_valeur*=valeur;
    }

    //! Retire une valeur.
    void del(int valeur)
    {
        if(in(valeur))
            m_valeur/=valeur;
    }

    //! Teste si la valeur est contenue.
    bool in(int valeur) const
        {return m_valeur%valeur == 0;}

    //! Teste la validité de la valeur.
    bool isValidAttribut() const final override
    {   if(m_valeur <= 0)
            return false;
        if(m_valeur == 1)
            return true;
        auto i = 2;
        auto r = sqrt(m_valeur);
        while (i <= r and m_valeur%(i*i))
            i++;
        return i > r;
    }

    QList<int> list() const
    {
        QList<int> L;
        auto r = sqrt(m_valeur);
        for(auto i = 2; i<= r; ++i)
            if(in(i))
                L.append(i);
        return L;
    }

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toInt());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type double.
 */
class AttributDouble : public AttributEntityVal<double>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributDouble)

    //! Constructeur.
    AttributDouble()
        : AttributEntityVal<double>(0)
        {}

    //! Destructeur.
    ~AttributDouble() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toDouble());}
};

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type QString pouvant être vide.
 */
class AttributString : public AttributEntityRef<QString>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributString)

    //! Constructeur.
    AttributString()
        :AttributEntityRef<QString>(QString())
        {}

    //!Destructeur.
    ~AttributString() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value.toString());}
};

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

/*! \ingroup groupeAttributEntity
 * \brief Classe mère des attributs de type QVariant pouvant être vide.
 */
class AttributVariant : public AttributEntityRef<QVariant>
{
public:
    CONSTR_AFFECT_DEFAUT(AttributVariant)

    //! Constructeur.
    AttributVariant()
        :AttributEntityRef<QVariant>(QVariant())
        {}

    //!Destructeur.
    ~AttributVariant() override;

    //! Modifie la donnée à partir d'un QVariant.
    void setDataP(const QVariant & value, int /*pos*/) override
        {set(value);}
};

SINGLE_ATTRIBUT(AlphaAttribut,AttributIntSup,AttributIntSup<0>,Alpha,alpha,int)
SINGLE_ATTRIBUT(CardAttribut,AttributIntSup,AttributIntSup<-1>,Card,card,int)
SINGLE_ATTRIBUT(CibleAttribut,AttributIntSup,AttributIntSup<0>,Cible,cible,int)
SINGLE_ATTRIBUT(CodeAttribut,AttributCode,AttributCode,Code,code,int)
SINGLE_ATTRIBUT(CreationAttribut,AttributDateTimeValide,AttributDateTimeValide,Creation,creation,QDateTime)
SINGLE_ATTRIBUT(DateValideAttribut,AttributDateValide,AttributDateValide,Date,date,QDate)
SINGLE_ATTRIBUT(DateTimeCurrentAttribut,AttributDateTimeCurrent,AttributDateTimeCurrent,DateTime,dateTime,QDateTime)
SINGLE_ATTRIBUT(DateTimeValideAttribut,AttributDateTimeValide,AttributDateTimeValide,DateTime,dateTime,QDateTime)
SINGLE_ATTRIBUT(DecimaleAttribut,AttributDecimale,AttributDecimale,Decimale,decimale,int)
SINGLE_ATTRIBUT(EtatAttribut,AttributInt,AttributInt,Etat,etat,int)
SINGLE_ATTRIBUT(ExactAttribut,AttributBool,AttributBool,Exact,exact,bool)
SINGLE_ATTRIBUT(IdAttribut,AttributIntSup,AttributIntSup<0>,Id,id,int)
SINGLE_ATTRIBUT(IdCibleAttribut,AttributIntSup,AttributIntSup<0>,IdCible,idCible,int)
SINGLE_ATTRIBUT(IdEtatAttribut,AttributIntSup,AttributIntSup<0>,IdEtat,idEtat,int)
SINGLE_ATTRIBUT(IdProgAttribut,AttributIntNullPositif,AttributIntNullPositif,IdProg,idProg,int)
SINGLE_ATTRIBUT(Id1Attribut,AttributIntSup,AttributIntSup<1>,Id1,id1,int)
SINGLE_ATTRIBUT(Id1NegAttribut,AttributInt,AttributInt,Id1,id1,int)
SINGLE_ATTRIBUT(Id1NullAttribut,AttributIntNullPositif,AttributIntNullPositif,Id1,id1,int)
SINGLE_ATTRIBUT(Id2Attribut,AttributIntSup,AttributIntSup<1>,Id2,id2,int)
SINGLE_ATTRIBUT(Id2NullAttribut,AttributIntNullPositif,AttributIntNullPositif,Id2,id2,int)
SINGLE_ATTRIBUT(Id3Attribut,AttributIntSup,AttributIntSup<1>,Id3,id3,int)
SINGLE_ATTRIBUT(NcAttribut,AttributStringNotEmpty,AttributStringNotEmpty,Nc,nc,QString)
SINGLE_ATTRIBUT(NomAttribut,AttributStringNotEmpty,AttributStringNotEmpty,Nom,nom,QString)
SINGLE_ATTRIBUT(ModifAttribut,AttributBool,AttributBool,Modif,modif,bool)
SINGLE_ATTRIBUT(ModificationAttribut,AttributDateTimeCurrent,AttributDateTimeCurrent,Modification,modification,QDateTime)
SINGLE_ATTRIBUT(NumAttribut,AttributIntSup,AttributIntSup<0>,Num,num,int)
SINGLE_ATTRIBUT(ParentAttribut,AttributIntNullPositif,AttributIntNullPositif,Parent,parent,int)
SINGLE_ATTRIBUT(SaisieAttribut,AttributBool,AttributBool,Saisie,saisie,bool)
SINGLE_ATTRIBUT(TexteAttribut,AttributString,AttributString,Texte,texte,QString)
SINGLE_ATTRIBUT(TotalAttribut,AttributIntSup,AttributIntSup<0>,Total,total,int)
SINGLE_ATTRIBUT(TpValAttribut,AttributIntSup,AttributIntSup<0>,TpVal,tpVal,int)
SINGLE_ATTRIBUT(TypeAttribut,AttributIntSup,AttributIntSup<0>,Type,type,int)
SINGLE_ATTRIBUT(ValeurDoubleAttribut,AttributDouble,AttributDouble,Valeur,valeur,double)
SINGLE_ATTRIBUT(ValeurIntAttribut,AttributInt,AttributInt,Valeur,valeur,int)
SINGLE_ATTRIBUT(ValeurVariantAttribut,AttributVariant,AttributVariant,Valeur,valeur,QVariant)
SINGLE_ATTRIBUT(VersionAttribut,AttributIntSup,AttributIntSup<0>,Version,version,int)
#endif // ATTRIBUTSIMPLE_H
