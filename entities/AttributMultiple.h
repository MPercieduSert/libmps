/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef ATTRIBUTMULTIPLE_H
#define ATTRIBUTMULTIPLE_H

#include "AttributSimple.h"

/*! \defgroup groupeEntity Entités
 * \brief Ensemble de classes représentant les entités de la base de donnée.
 */

/*! \ingroup groupeEntity
 * \brief Espace de nom des entités.
 */
namespace entityMPS {

namespace amps = attributMPS;

using namespace typeMPS;
using NoAttribut = amps::NoAttribut;

/*! \ingroup groupeEntity
 * \brief Classe template des attributs multiple.
 */
template<class... Attribut> class Attributs;

template<class Attribut> class Attributs<Attribut> : public Attribut {
public:
    CONSTR_DEFAUT(Attributs)
    CONSTR_AFFECT_DEFAUT(Attributs)
};

template <class AttributPremier, class... AttributSuivant> class Attributs<AttributPremier, AttributSuivant...>
        : public AttributPremier, public Attributs<AttributSuivant...> {
public:
    //! Nombre d'attribut de l'entité.
    enum {NbrAtt = AttributPremier::NbrAtt + Attributs<AttributSuivant...>::NbrAtt};
    //!Filliation des attributs
    using AttPre = AttributPremier;
    using AttSuiv = Attributs<AttributSuivant...>;
    CONSTR_DEFAUT(Attributs)
    CONSTR_AFFECT_DEFAUT(Attributs)

    //! Renvoie une chaine de caractère contenant les noms, les validités et les valeurs des attributs.
    QString affiche() const override
        {return AttributPremier::affiche().append("\n").append(Attributs<AttributSuivant...>::affiche());}

    //! Retourne un QVariant contenant la donnée souhaitée ou un QVariant nulle si num est invalide.
    QVariant data(szt num) const
        {return num < NbrAtt ? dataP(num) : QVariant();}

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return AttributPremier::isValid() && Attributs<AttributSuivant...>::isValid();}

    //! Retourne le nom de l'attribut donnée souhaitée, pos doit être valide.
    QString attributName(szt pos) const
        {return pos < AttributPremier::NbrAtt ? AttributPremier::attributName(pos)
                                              : Attributs<AttributSuivant...>::attributName(pos - AttributPremier::NbrAtt);}

    //! Retourne le nom de l'attribut donnée souhaitée, pos doit être valide.
    static QString NameAttribut(szt pos)
        {return pos < AttributPremier::NbrAtt ? AttributPremier::NameAttribut(pos)
                                              : Attributs<AttributSuivant...>::NameAttribut(pos - AttributPremier::NbrAtt);}

    //! Renvoie une chaine de caractère contenant les valeurs de l'attribut.
    QString toString() const override
        {return AttributPremier::toString().append(",").append(Attributs<AttributSuivant...>::toString());}

    //! Test d'égalité entre cette entité et celle transmise en argument.
    bool operator ==(const Attributs<AttributPremier, AttributSuivant...> & entity) const
        {return AttributPremier::operator ==(entity) && Attributs<AttributSuivant...>::operator ==(entity);}

protected:
    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(szt pos) const override
        {return pos < AttributPremier::NbrAtt ? AttributPremier::dataP(pos)
                                              : Attributs<AttributSuivant...>::dataP(pos - AttributPremier::NbrAtt);}

    //! Remplace les attributs de l'entité par celle de l'entité transmise.
    void set(const  Attributs<AttributPremier, AttributSuivant...> & entity) {
        AttributPremier::set(entity);
        Attributs<AttributSuivant...>::set(entity);
    }

    //! Modifie la donnée à partir d'un QVariant, pos doit être valide.
   void setDataP(szt pos, const QVariant & value) override
        {return pos < AttributPremier::NbrAtt ? AttributPremier::setDataP(pos, value)
                                              : Attributs<AttributSuivant...>::setDataP(pos - AttributPremier::NbrAtt, value);}
};

/*! \ingroup groupeEntity
 * \brief Structure template de calcul des positions des attributs multiple.
 */
template<class AttFound, class Att, class AttPre, class AttSuiv, szt Pos> struct PosisionEnumTemp {
    enum {Position = PosisionEnumTemp<AttFound,AttPre,typename AttPre::AttPre,typename AttPre::AttSuiv,Pos>::Position
          + PosisionEnumTemp<AttFound,AttSuiv,typename AttSuiv::AttPre,typename AttSuiv::AttSuiv,AttPre::NbrAtt + Pos>::Position};
};

template<class AttFound, class Att, szt Pos> struct PosisionEnumTemp<AttFound,Att,NoAttribut,NoAttribut,Pos>
    {enum {Position = 0};};

template<class AttFound, szt Pos> struct PosisionEnumTemp<AttFound,AttFound,NoAttribut,NoAttribut,Pos>
    {enum {Position = Pos};};

template<class AttFound, szt Pos> struct PosisionEnumTemp<AttFound,Attributs<AttFound>,NoAttribut,NoAttribut,Pos>
    {enum {Position = Pos};};

template<class AttFound, class Ent> using PositionEnum = PosisionEnumTemp<AttFound,Ent,typename Ent::AttPre,typename Ent::AttSuiv,0>;
}

namespace attributMPS {
namespace emps = entityMPS;

// Attributs Multiples Prédéfinies
using CibleAttributs = emps::Attributs<IdCibleAttribut, CibleAttribut>;
using CibleDateTimeAttribut = emps::Attributs<CibleAttributs, DateTimeValideAttribut>;
using CibleDateTimeNumAttribut = emps::Attributs<CibleDateTimeAttribut, NumAttribut>;
using CreaModifAttribut = emps::Attributs<CreationAttribut,ModificationAttribut>;
using DebutFinAttribut = emps::Attributs<DebutAttribut,FinAttribut>;
using EtatAttributs = emps::Attributs<IdEtatAttribut, EtatAttribut>;
template<class ValeurAttribut> using CibleDateTimeNumValeurAttribut = emps::Attributs<CibleDateTimeNumAttribut, ValeurAttribut>;
using IdNumAttribut = emps::Attributs<Id1Attribut, NumAttribut>;
using IdProgNomAttribut = emps::Attributs<IdProgAttribut,NomAttribut>;
template<class ValeurAttribut> using IdNumValeurAttribut = emps::Attributs<IdNumAttribut, ValeurAttribut>;
using NomTypeAttribut = emps::Attributs<NomAttribut, TypeAttribut>;
using NcNomAttribut = emps::Attributs<NcAttribut, NomAttribut>;
using NcNomTypeAttribut = emps::Attributs<NcNomAttribut, TypeAttribut>;
using RelationAttribut = emps::Attributs<Id1Attribut, Id2Attribut>;
using RelationDateTimeAttribut = emps::Attributs<RelationAttribut, DateTimeValideAttribut>;
using RelationNumAttribut = emps::Attributs<RelationAttribut, NumAttribut>;
using TexteAttributs = emps::Attributs<CreaModifAttribut,TexteAttribut>;

/*! \ingroup groupeEntity
 * \brief Attributs des entités de type arbre.
 */
class ArbreAttribut : public emps::Attributs<ParentAttribut,FeuilleAttribut,NumAttribut> {
public:
    enum {Ordre = emps::PositionEnum<NumAttribut,ArbreAttribut>::Position};
    //using Attributs<ArbreSimpleAttribut,FeuilleAttribut,NumAttribut>::Attributs;
    CONSTR_DEFAUT(ArbreAttribut)
    CONSTR_AFFECT_DEFAUT(ArbreAttribut)

    //! Destructeur.
    //~ArbreAttribut() override;
};

/*! \ingroup groupeAttributEntity
 * \brief Attributs contenant deux identifiants dont exactement un est non nul pour être valide.
 */
class RelationExactOneNotNullAttribut : public emps::Attributs<Id1NullAttribut,Id2NullAttribut> {
public:
    CONSTR_DEFAUT(RelationExactOneNotNullAttribut)
    CONSTR_AFFECT_DEFAUT(RelationExactOneNotNullAttribut)

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return (Id1NullAttribut::isValidAttribut() && id2() == 0) || (id1() == 0 && Id2NullAttribut::isValidAttribut());}
};

/*! \ingroup groupeAttributEntity
 * \brief Attributs contenant trois identifiants dont exactement un est non nul pour être valide.
 */
class RelationTroisExactOneNotNullAttribut : public emps::Attributs<Id1NullAttribut,Id2NullAttribut,Id3NullAttribut> {
public:
    CONSTR_DEFAUT(RelationTroisExactOneNotNullAttribut)
    CONSTR_AFFECT_DEFAUT(RelationTroisExactOneNotNullAttribut)

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return (Id1NullAttribut::isValidAttribut() && id2() == 0 && id3() == 0)
                || (Id2NullAttribut::isValidAttribut() && id1() == 0 && id3() == 0)
                || (Id3NullAttribut::isValidAttribut() && id1() == 0 && id2() == 0);}
};
}
#endif // ENTITYATTRIBUT_H
