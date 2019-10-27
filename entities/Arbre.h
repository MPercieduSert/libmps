/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef ARBRE_H
#define ARBRE_H

#include "Entity.h"

SINGLE_ATTRIBUT(FeuilleAttribut,AttributBool,Feuille,feuille)

/*! \ingroup groupeEntity
 * \brief Classe mère des entités de type arbre simple.
 */

class ArbreSimpleAttribut : public ParentAttribut
{
public:
    enum {Ordre = Entity::Id};
    CONSTR_DEFAUT(ArbreSimpleAttribut)
    CONSTR_AFFECT_DEFAUT(ArbreSimpleAttribut)

    //! Destructeur.
    ~ArbreSimpleAttribut() override;
};

/*! \ingroup groupeEntity
 * \brief Classe mère des entités de type arbre.
 */

class ArbreAttribut : public Attributs<ParentAttribut,FeuilleAttribut,NumAttribut>
{
public:
    enum {Ordre = PositionEnum<NumAttribut,ArbreAttribut>::Position};
    //using Attributs<ArbreSimpleAttribut,FeuilleAttribut,NumAttribut>::Attributs;
    CONSTR_DEFAUT(ArbreAttribut)
    CONSTR_AFFECT_DEFAUT(ArbreAttribut)

    //! Destructeur.
    ~ArbreAttribut() override;
};

class Arbre : public EntityAttributsID<ArbreAttribut,InfoEntity::ArbreId>
{
public:
    using EAID = EntityAttributsID<ArbreAttribut,InfoEntity::ArbreId>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Feuille = PositionEnum<FeuilleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<ArbreAttribut,InfoEntity::ArbreId>::EntityAttributsID;
    BASE_ENTITY(Arbre)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Arbre(int num, idt parent, idt id = 0)
        : EAID(id)
    {
        setNum(num);
        setParent(parent);
    }

    //! Constructeur à partir des valeurs attributs.
    Arbre(bool feuille, int num, idt parent, idt id = 0)
        : Arbre(num,parent,id)
        {setFeuille(feuille);}
};

#endif // ARBREENTITY_H
