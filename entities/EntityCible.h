/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITYCIBLE_H
#define ENTITYCIBLE_H

#include "EntityRelation.h"

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant une clé positive.
#define ID1_ENTITY(ENTITY,TYPE,IDT,ID1) ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2) ENUM_##TYPE(ID1) ALIAS_CLE(ID1,1)};

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant une clé négative.
#define ID1_ENTITY_NEG(ENTITY,TYPE,IDT,ID1) ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2) ENUM_##TYPE(ID1) ALIAS_CLE_NEG(ID1,1)};

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_IdNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1,  Nc = mere::Nc, Nom = mere::Nom, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

namespace entityBaseMPS {

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<szt IDM , class Id1Att> class IdNcNomEntityTemp :
        public EntityIDs<IDM,Id1Att,NcNomAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,NcNomAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,NcNomAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;

    BASE_ENTITY(IdNcNomEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomEntityTemp(Id1Trans id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdNcNomEntityTemp(Id1Trans id1, const QString & nc, const QString & nom, idt id = 0)
        : IdNcNomEntityTemp(nom, id) {
        setId1(id1);
        setNc(nc);
    }
};

template<szt IDM , class Id1Att> IdNcNomEntityTemp<IDM,Id1Att>::~IdNcNomEntityTemp() {}

template<szt IDM > using IdNcNomEntity = IdNcNomEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdNcNom(ID1) ENUM_IdNcNomTemp(ID1)

template<szt IDM > using IdNullNcNomEntity = IdNcNomEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullNcNom(ID1) ENUM_IdNcNomTemp(ID1)

template<szt IDM > using IdNegNcNomEntity = IdNcNomEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegNcNom(ID1) ENUM_IdNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_IdNcNomTypeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1,  Nc = mere::Nc, Nom = mere::Nom, Type = mere::Type, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<szt IDM , class Id1Att> class IdNcNomTypeEntityTemp :
        public EntityIDs<IDM,Id1Att,NcNomTypeAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,NcNomTypeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,NcNomTypeAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setType;

    BASE_ENTITY(IdNcNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomTypeEntityTemp(Id1Trans id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomTypeEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdNcNomTypeEntityTemp(Id1Trans id1, const QString & nc, const QString & nom, idt type, idt id = 0)
        : IdNcNomTypeEntityTemp(nom, id) {
        setId1(id1);
        setNc(nc);
        setType(type);
    }
};

template<szt IDM , class Id1Att> IdNcNomTypeEntityTemp<IDM,Id1Att>::~IdNcNomTypeEntityTemp() {}

template<szt IDM > using IdNcNomTypeEntity = IdNcNomTypeEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

template<szt IDM > using IdNullNcNomTypeEntity = IdNcNomTypeEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

template<szt IDM > using IdNegNcNomTypeEntity = IdNcNomTypeEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un type et une version.
#define ENUM_IdTypeVersionTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Version = mere::Version, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<szt IDM , class Id1Att> class IdTypeVersionEntityTemp :
        public EntityIDs<IDM,Id1Att,TypeAttribut,VersionAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,TypeAttribut,VersionAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Version = PositionEnum<VersionAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,TypeAttribut,VersionAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setType;
    using EAID::setVersion;

    BASE_ENTITY(IdTypeVersionEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeVersionEntityTemp(Id1Trans id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeVersionEntityTemp(Id1Trans id1, idt type, idt id)
        : EAID(id1, id)
        {setType(type);}

    //! Constructeur à partir des valeurs attributs.
    IdTypeVersionEntityTemp(Id1Trans id1, idt type, int version, idt id = 0)
        : IdTypeVersionEntityTemp(id1, type, id)
        {setVersion(version);}
};

template<szt IDM , class Id1Att> IdTypeVersionEntityTemp<IDM,Id1Att>::~IdTypeVersionEntityTemp() {}

template<szt IDM > using IdTypeVersionEntity = IdTypeVersionEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

template<szt IDM > using IdNullTypeVersionEntity = IdTypeVersionEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

template<szt IDM > using IdNegTypeVersionEntity = IdTypeVersionEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un nom cours et un nom.
#define ENUM_IdArbreSimpleNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Parent = mere::Parent, Nc = mere::Nc, Nom = mere::Nom,\
    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Ordre = mere::Ordre};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, nom cours, nom et parent.
 */
template<szt IDM , class Id1Att> class IdArbreSimpleNcNomEntityTemp :
        public EntityIDs<IDM,Id1Att,ParentAttribut,NcNomAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,ParentAttribut,NcNomAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityIDs<IDM,Id1Att,ParentAttribut,NcNomAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    BASE_ENTITY(IdArbreSimpleNcNomEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleNcNomEntityTemp(Id1Trans id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleNcNomEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdArbreSimpleNcNomEntityTemp(Id1Trans id1, const QString & nc, const QString & nom, idt parent = 0, idt id = 0)
        : IdArbreSimpleNcNomEntityTemp(nom, id) {
        setId1(id1);
        setNc(nc);
        setParent(parent);
    }
};

template<szt IDM , class Id1Att> IdArbreSimpleNcNomEntityTemp<IDM,Id1Att>::~IdArbreSimpleNcNomEntityTemp() {}

template<szt IDM > using IdArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

template<szt IDM > using IdNullArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

template<szt IDM > using IdNegArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un idProg, un nom cours et un nom.
#define ENUM_IdArbreSimpleIdProgNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Parent = mere::Parent, IdProg = mere::IdProg, Nc = mere::Nc,\
    Nom = mere::Nom, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Ordre = mere::Ordre};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, parent, idProg, nom cours et nom.
 */
template<szt IDM , class Id1Att> class IdArbreSimpleIdProgNcNomEntityTemp :
        public EntityIDs<IDM,Id1Att,ParentAttribut,IdProgAttribut,NcNomAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,ParentAttribut,IdProgAttribut,NcNomAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdProg = PositionEnum<IdProgAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityIDs<IDM,Id1Att,ParentAttribut,IdProgAttribut,NcNomAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setIdProg;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    BASE_ENTITY(IdArbreSimpleIdProgNcNomEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleIdProgNcNomEntityTemp(idt idProg, idt id)
        : EAID(id)
        {setIdProg(idProg);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleIdProgNcNomEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdArbreSimpleIdProgNcNomEntityTemp(Id1Trans id1, const QString & nc, const QString & nom,
                                       idt parent = 0, idt idProg = 0, idt id = 0)
        : IdArbreSimpleIdProgNcNomEntityTemp(id, nom) {
        setId1(id1);
        setIdProg(idProg);
        setNc(nc);
        setParent(parent);
    }
};

template<szt IDM , class Id1Att> IdArbreSimpleIdProgNcNomEntityTemp<IDM,Id1Att>::~IdArbreSimpleIdProgNcNomEntityTemp() {}

template<szt IDM > using IdArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

template<szt IDM > using IdNullArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

template<szt IDM > using IdNegArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, une creation, une modification et un numéro.
#define ENUM_IdCreationModifNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Creation = mere::Creation, Modification = mere::Modification,\
    Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, creation, modification, et num.
 */
template<szt IDM , class Id1Att> class IdCreationModifNumEntityTemp :
        public EntityIDs<IDM,Id1Att,CreationAttribut,ModificationAttribut,NumAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CreationAttribut,ModificationAttribut,NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Creation = PositionEnum<CreationAttribut,EAID>::Position,
                   Modification = PositionEnum<ModificationAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CreationAttribut,ModificationAttribut,NumAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setCreation;
    using EAID::setModification;
    using EAID::setNum;
    BASE_ENTITY(IdCreationModifNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdCreationModifNumEntityTemp(Id1Trans id1, int num, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    IdCreationModifNumEntityTemp(Id1Trans id1, const QDateTime & creation, const QDateTime & modif, int num, idt id = 0)
        : IdCreationModifNumEntityTemp(id1,num,id) {
        setCreation(creation);
        setModification(modif);
    }
};

template<szt IDM , class Id1Att> IdCreationModifNumEntityTemp<IDM,Id1Att>::~IdCreationModifNumEntityTemp() {}

template<szt IDM > using IdCreationModifNumEntity = IdCreationModifNumEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

template<szt IDM > using IdNullCreationModifNumEntity = IdCreationModifNumEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

template<szt IDM > using IdNegCreationModifNumEntity = IdCreationModifNumEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_IdDateTimeTypeValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, DateTime = mere::Datetime, Type = mere::Type,\
    Valeur::mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime et Num.
 */
template<szt IDM , class DateTimeAtt, class ValeurAtt, class Id1Att> class IdDateTimeTypeValeurEntityTemp
        : public EntityIDs<IDM,Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setDateTime;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(IdDateTimeTypeValeurEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdDateTimeTypeValeurEntityTemp(Id1Trans id1, idt type = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    IdDateTimeTypeValeurEntityTemp(Id1Trans id1, const QDateTime & dateTime, idt type, ValTrans valeur, idt id = 0)
        : IdDateTimeTypeValeurEntityTemp(id1,type,id) {
        setDateTime(dateTime);
        setValeur(valeur);
    }
};

template<szt IDM , class Id1Att, class DateTimeAtt, class ValeurAtt>
IdDateTimeTypeValeurEntityTemp<IDM,Id1Att,DateTimeAtt, ValeurAtt>::~IdDateTimeTypeValeurEntityTemp() {}

template<szt IDM > using IdDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_IdDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_IdDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_IdDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_IdDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_IdDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_IdDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNullDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_IdNullDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<szt IDM > using IdNegDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_IdNegDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_CibleTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<szt IDM , class Id1Att> class CibleEntityTemp : public EntityIDs<IDM,Id1Att,CibleAttributs> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttributs>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttributs>::EntityIDs;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    BASE_ENTITY(CibleEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    CibleEntityTemp(Id1Trans id1, idt idCible, int cible, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
    }
};

template<szt IDM , class Id1Att> CibleEntityTemp<IDM,Id1Att>::~CibleEntityTemp() {}

template<szt IDM > using CibleEntity = CibleEntityTemp<IDM,Id1Attribut>;
#define ENUM_Cible(ID1) ENUM_CibleTemp(ID1)

template<szt IDM > using CibleNullEntity = CibleEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_CibleNull(ID1) ENUM_CibleTemp(ID1)

template<szt IDM > using CibleNegEntity = CibleEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_CibleNeg(ID1) ENUM_CibleTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, un numéro et un type.
#define ENUM_CibleNumTypeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, Num = mere::Num,\
    Type = mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, Num et Type.
 */
template<szt IDM , class Id1Att> class CibleNumTypeEntityTemp :
        public EntityIDs<IDM,Id1Att,CibleAttributs,NumAttribut,TypeAttribut>
{
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttributs,NumAttribut,TypeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttributs,NumAttribut,TypeAttribut>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    using EAID::setType;
    BASE_ENTITY(CibleNumTypeEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    CibleNumTypeEntityTemp(idt idCible, int cible, int num, idt type) {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleNumTypeEntityTemp(Id1Trans id1, idt idCible, int cible, int num, idt type, idt id = 0)
        : CibleNumTypeEntityTemp(idCible, cible, num, type) {
        setId(id);
        setId1(id1);
    }
};

template<szt IDM , class Id1Att> CibleNumTypeEntityTemp<IDM,Id1Att>::~CibleNumTypeEntityTemp() {}

template<szt IDM > using CibleNumTypeEntity = CibleNumTypeEntityTemp<IDM,Id1Attribut>;
#define ENUM_CibleNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

template<szt IDM > using CibleNullNumTypeEntity = CibleNumTypeEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_CibleNullNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

template<szt IDM > using CibleNegNumTypeEntity = CibleNumTypeEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_CibleNegNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_CibleNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut IdCible, cible, num et valeur.
 */
template<szt IDM , class ValeurAtt, class Id1Att> class CibleNumValeurEntityTemp :
        public EntityIDs<IDM,Id1Att, CibleAttributs, NumAttribut, ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att, CibleAttributs, NumAttribut, ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att, CibleAttributs, NumAttribut, ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(CibleNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleNumValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num = 0) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleNumValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num, ValTrans valeur, idt id = 0)
        : CibleNumValeurEntityTemp(id1,idCible,cible,num) {
        setId(id);
        setValeur(valeur);
    }
};

template<szt IDM , class ValeurAtt, class Id1Att>
    CibleNumValeurEntityTemp<IDM,ValeurAtt, Id1Att>::~CibleNumValeurEntityTemp() {}

template<szt IDM > using CibleNumValeurDoubleEntity
= CibleNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_CibleNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNumValeurIntEntity
= CibleNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1Attribut>;
#define ENUM_CibleNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNumValeurVariantEntity
= CibleNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_CibleNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNullNumValeurDoubleEntity
= CibleNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_CibleNullNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNullNumValeurIntEntity
= CibleNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_CibleNullNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNullNumValeurVariantEntity
= CibleNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_CibleNullNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNegNumValeurDoubleEntity
= CibleNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_CibleNegNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNegNumValeurIntEntity
= CibleNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_CibleNegNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<szt IDM > using CibleNegNumValeurVariantEntity
= CibleNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_CibleNegNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible et une DateTime.
#define ENUM_CibleDateTimeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    DateTime = mere::DateTime, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible et DateTime.
 */
template<szt IDM , class DateTimeAtt, class Id1Att> class CibleDateTimeEntityTemp :
        public EntityIDs<IDM,Id1Att,CibleAttributs,DateTimeAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttributs,DateTimeAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttributs,DateTimeAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    BASE_ENTITY(CibleDateTimeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeEntityTemp(Id1Trans id1, idt idCible, int cible, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeEntityTemp(Id1Trans id1, idt idCible, int cible, const QDateTime & dateTime, idt id = 0)
        : CibleDateTimeEntityTemp(id1,idCible,cible,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class Id1Att> CibleDateTimeEntityTemp<IDM,DateTimeAtt, Id1Att>
    ::~CibleDateTimeEntityTemp() {}

template<szt IDM > using CibleDateTimeCurrentEntity = CibleDateTimeEntityTemp<IDM,DateTimeCurrentAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<szt IDM > using CibleDateTimeValideEntity = CibleDateTimeEntityTemp<IDM,DateTimeValideAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentEntity = CibleDateTimeEntityTemp<IDM,DateTimeCurrentAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideEntity = CibleDateTimeEntityTemp<IDM,DateTimeValideAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentEntity = CibleDateTimeEntityTemp<IDM,DateTimeCurrentAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideEntity = CibleDateTimeEntityTemp<IDM,DateTimeValideAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num et une DateTime.
#define ENUM_CibleDateTimeNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, \
    DateTime = mere::DateTime, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, DateTime et Num.
 */
template<szt IDM , class DateTimeAtt, class Id1Att> class CibleDateTimeNumEntityTemp :
        public EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(CibleDateTimeNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumEntityTemp(Id1Trans id1, idt idCible, int cible, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumEntityTemp(Id1Trans id1, idt idCible, int cible, const QDateTime & dateTime, int num, idt id = 0)
        : CibleDateTimeNumEntityTemp(id1,idCible,cible,num,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class Id1Att> CibleDateTimeNumEntityTemp<IDM,DateTimeAtt, Id1Att>::~CibleDateTimeNumEntityTemp() {}

template<szt IDM > using CibleDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeCurrentAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeValideAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeCurrentAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeValideAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeCurrentAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<IDM,DateTimeValideAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num, type et une DateTime.
#define ENUM_CibleDateTimeNumTypeTemp(ID1) /*! \brief Positions des attributs */ \
        enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, \
        DateTime = mere::DateTime, Num = mere::Num, Type= mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, DateTime et Num.
 */
template<szt IDM , class DateTimeAtt, class Id1Att> class CibleDateTimeNumTypeEntityTemp :
            public EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                       Id1 = PositionEnum<Id1Att,EAID>::Position,
                       IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                       Cible = PositionEnum<CibleAttribut,EAID>::Position,
                       DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                       Num = PositionEnum<NumAttribut,EAID>::Position,
                       Type = PositionEnum<TypeAttribut,EAID>::Position,
                       NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setType;
    BASE_ENTITY(CibleDateTimeNumTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumTypeEntityTemp(Id1Trans id1, idt idCible, int cible, int num = 0, idt type = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumTypeEntityTemp(Id1Trans id1, idt idCible, int cible, const QDateTime & dateTime,
                                   int num, idt type, idt id = 0)
        : CibleDateTimeNumTypeEntityTemp(id1,idCible,cible,num,type,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class Id1Att> CibleDateTimeNumTypeEntityTemp<IDM,DateTimeAtt, Id1Att>
    ::~CibleDateTimeNumTypeEntityTemp() {}

template<szt IDM > using CibleDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeCurrentAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeValideAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeCurrentAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentTypeNum(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeValideAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeCurrentAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentTypeNum(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<IDM,DateTimeValideAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_CibleDateTimeNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    DateTime = mere::DateTime, Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut une clé, IdCible, cible, datetime, num et valeur.
 */
template<szt IDM , class DateTimeAtt, class ValeurAtt, class Id1Att> class CibleDateTimeNumValeurEntityTemp :
        public EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValeurTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(CibleDateTimeNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num = 0) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num, ValeurTrans valeur, idt id = 0)
        : CibleDateTimeNumValeurEntityTemp(id1,idCible,cible,num) {
        setId(id);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumValeurEntityTemp(Id1Trans id1, idt idCible, int cible, const QDateTime & dateTime, int num, ValeurTrans valeur, idt id = 0)
        : CibleDateTimeNumValeurEntityTemp(id1,idCible,cible,num,valeur,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class ValeurAtt, class Id1Att>
    CibleDateTimeNumValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt, Id1Att>::~CibleDateTimeNumValeurEntityTemp() {}

template<szt IDM > using CibleDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime, un typeValeur et Valeur.
#define ENUM_CibleDateTimeNumTpValValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    DateTime = mere::DateTime, Num = mere::Num, TypeVal = mere::TypeVal, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut une clé, IdCible, cible, datetime, num, typeVal et valeur.
 */
template<szt IDM , class DateTimeAtt, class ValeurAtt, class Id1Att> class CibleDateTimeNumTpValValeurEntityTemp :
        public EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, TpValAttribut, ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValeurTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, TpValAttribut, ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   TpVal = PositionEnum<TpValAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, TpValAttribut, ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setTpVal;
    using EAID::setValeur;
    BASE_ENTITY(CibleDateTimeNumTpValValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumTpValValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num = 0) {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumTpValValeurEntityTemp(Id1Trans id1, idt idCible, int cible, int num, int tpVal, ValeurTrans valeur, idt id = 0)
        : CibleDateTimeNumTpValValeurEntityTemp(id1,idCible,cible,num) {
        setId(id);
        setTpVal(tpVal);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumTpValValeurEntityTemp(Id1Trans id1, idt idCible, int cible, const QDateTime & dateTime,
                                          int num, int tpVal, ValeurTrans valeur, idt id = 0)
        : CibleDateTimeNumTpValValeurEntityTemp(id1,idCible,cible,num,tpVal,valeur,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class ValeurAtt, class Id1Att>
    CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt, Id1Att>::~CibleDateTimeNumTpValValeurEntityTemp() {}

template<szt IDM > using CibleDateTimeCurrentNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleDateTimeCurrentNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleDateTimeCurrentNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeCurrentNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleDateTimeValideNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_CibleDateTimeValideNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeCurrentNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeCurrentNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNullDateTimeValideNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_CibleNullDateTimeValideNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeCurrentNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeCurrentNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumTpValValeurDoubleEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumTpValValeurDouble(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumTpValValeurIntEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumTpValValeurInt(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

template<szt IDM > using CibleNegDateTimeValideNumTpValValeurVariantEntity
= CibleDateTimeNumTpValValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_CibleNegDateTimeValideNumTpValValeurVariant(ID1) ENUM_CibleDateTimeNumTpValValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, num et Valeur.
#define ENUM_IdNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, num et valeur.
 */
template<szt IDM , class ValeurAtt, class Id1Att> class IdNumValeurEntityTemp :
            public EntityIDs<IDM,Id1Att,NumAttribut,ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,NumAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,NumAttribut,ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setNum;
    BASE_ENTITY(IdNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNumValeurEntityTemp(Id1Trans id1, int num) {
        setId1(id1);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    IdNumValeurEntityTemp(Id1Trans id1, int num, ValTrans valeur, idt id = 0)
        : IdNumValeurEntityTemp(id1,num) {
        setId(id);
        setValeur(valeur);
    }
};

template<szt IDM , class ValeurAtt, class Id1Att> IdNumValeurEntityTemp<IDM,ValeurAtt, Id1Att>
    ::~IdNumValeurEntityTemp() {}

template<szt IDM > using IdNumValeurDoubleEntity = IdNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_IdNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNumValeurIntEntity = IdNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1Attribut>;
#define ENUM_IdNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNumValeurVariantEntity = IdNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_IdNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNullNumValeurDoubleEntity = IdNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNullNumValeurIntEntity = IdNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNullNumValeurVariantEntity = IdNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNegNumValeurDoubleEntity = IdNumValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNegNumValeurIntEntity = IdNumValeurEntityTemp<IDM,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<szt IDM > using IdNegNumValeurVariantEntity = IdNumValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, type et Valeur.
#define ENUM_IdTypeValeurTemp(ID1) /*! \brief Positions des attributs */ \
enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, type et valeur.
 */
template<szt IDM , class ValeurAtt, class Id1Att> class IdTypeValeurEntityTemp :
            public EntityIDs<IDM,Id1Att,TypeAttribut,ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,TypeAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,TypeAttribut,ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setType;
    BASE_ENTITY(IdTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeValeurEntityTemp(Id1Trans id1, idt type) {
        setId1(id1);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    IdTypeValeurEntityTemp(Id1Trans id1, idt type, ValTrans valeur, idt id = 0)
        : IdTypeValeurEntityTemp(id1, type) {
        setId(id);
        setValeur(valeur);
    }
};

template<szt IDM , class ValeurAtt, class Id1Att> IdTypeValeurEntityTemp<IDM,ValeurAtt, Id1Att>
    ::~IdTypeValeurEntityTemp() {}

template<szt IDM > using IdTypeValeurDoubleEntity = IdTypeValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_IdTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdTypeValeurIntEntity = IdTypeValeurEntityTemp<IDM,ValeurIntAttribut,Id1Attribut>;
#define ENUM_IdTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdTypeValeurVariantEntity = IdTypeValeurEntityTemp<IDM,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_IdTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNullTypeValeurDoubleEntity = IdTypeValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_IdNullTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNullTypeValeurIntEntity = IdTypeValeurEntityTemp<IDM,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_IdNullTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNullTypeValeurVariantEntity = IdTypeValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_IdNullTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNegTypeValeurDoubleEntity = IdTypeValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_IdNegTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNegTypeValeurIntEntity = IdTypeValeurEntityTemp<IDM,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_IdNegTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<szt IDM > using IdNegTypeValeurVariantEntity = IdTypeValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_IdNegTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, num, type, valeur et version.
#define ENUM_IdNumTypeVersionValeurTemp(ID1) /*! \brief Positions des attributs */ \
enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, num, type, version et valeur.
 */
template<szt IDM , class ValeurAtt, class Id1Att> class IdNumTypeVersionValeurEntityTemp :
                public EntityIDs<IDM,Id1Att,NumAttribut,TypeAttribut,VersionAttribut,ValeurAtt> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,NumAttribut,TypeAttribut,VersionAttribut,ValeurAtt>;
        //! Positions des attributs.
        enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                       Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                       Num = PositionEnum<NumAttribut,EAID>::Position,
                       Type = PositionEnum<TypeAttribut,EAID>::Position,
                       Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                       Version = PositionEnum<VersionAttribut,EAID>::Position,
                       NbrAtt = EAID::NbrAtt};

        using EntityIDs<IDM,Id1Att,NumAttribut,TypeAttribut,VersionAttribut,ValeurAtt>::EntityIDs;
        using EAID::setId;
        using EAID::setId1;
        using EAID::setNum;
        using EAID::setType;
        using EAID::setVersion;
        BASE_ENTITY(IdNumTypeVersionValeurEntityTemp)

        //! Constructeur à partir d'un jeux de valeurs attributs unique.
        IdNumTypeVersionValeurEntityTemp(Id1Trans id1, int num, idt id =0)
            : EAID(id) {
            setId1(id1);
            setNum(num);
        }

        //! Constructeur à partir des valeurs attributs.
        IdNumTypeVersionValeurEntityTemp(Id1Trans id1, int num, idt type, ValTrans valeur, int version, idt id = 0)
            : IdNumTypeVersionValeurEntityTemp(id1, num, id) {
            setType(type);
            setValeur(valeur);
            setVersion(version);
        }
    };

template<szt IDM , class ValeurAtt, class Id1Att> IdNumTypeVersionValeurEntityTemp<IDM,ValeurAtt, Id1Att>
    ::~IdNumTypeVersionValeurEntityTemp() {}

template<szt IDM > using IdNumTypeVersionValeurDoubleEntity = IdNumTypeVersionValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1Attribut>;
#define ENUM_IdNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<IDM,ValeurIntAttribut,Id1Attribut>;
#define ENUM_IdNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<IDM,ValeurVariantAttribut,Id1Attribut>;
#define ENUM_IdNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNullNumTypeVersionValeurDoubleEntity
    = IdNumTypeVersionValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNullNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<IDM,ValeurIntAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNullNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NullAttribut>;
#define ENUM_IdNullNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNegNumTypeVersionValeurDoubleEntity
    = IdNumTypeVersionValeurEntityTemp<IDM,ValeurDoubleAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNegNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<IDM,ValeurIntAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<szt IDM > using IdNegNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<IDM,ValeurVariantAttribut,Id1NegAttribut>;
#define ENUM_IdNegNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une cible et un code.
#define ENUM_IdCibleCodeTemp(ID1) /*! \brief Positions des attributs */ \
enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Cible = mere::Cible, Code = mere::Code, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<szt IDM , class Id1Att> class IdCibleCodeEntityTemp : public EntityIDs<IDM,Id1Att,CibleAttribut,CodeAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttribut,CodeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                    Id1 = PositionEnum<Id1Att,EAID>::Position,
                    Cible = PositionEnum<CibleAttribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttribut,CodeAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setCible;
    using EAID::setCode;
    BASE_ENTITY(IdCibleCodeEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdCibleCodeEntityTemp(Id1Trans id1, int cible, flag code = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setCible(cible);
        setCode(code);
    }
};

template<szt IDM , class Id1Att> IdCibleCodeEntityTemp<IDM,Id1Att>::~IdCibleCodeEntityTemp() {}

template<szt IDM > using IdCibleCodeEntity = IdCibleCodeEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

template<szt IDM > using IdNullCibleCodeEntity = IdCibleCodeEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

template<szt IDM > using IdNegCibleCodeEntity = IdCibleCodeEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une cible et un num.
#define ENUM_IdCibleNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, Cible = mere::Cible, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<szt IDM , class Id1Att> class IdCibleNumEntityTemp : public EntityIDs<IDM,Id1Att,CibleAttribut,NumAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleAttribut,NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleAttribut,NumAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setCible;
    using EAID::setNum;
    BASE_ENTITY(IdCibleNumEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdCibleNumEntityTemp(Id1Trans id1, int cible, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setCible(cible);
        setNum(num);
    }
};

template<szt IDM , class Id1Att> IdCibleNumEntityTemp<IDM,Id1Att>::~IdCibleNumEntityTemp() {}

template<szt IDM > using IdCibleNumEntity = IdCibleNumEntityTemp<IDM,Id1Attribut>;
#define ENUM_IdCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

template<szt IDM > using IdNullCibleNumEntity = IdCibleNumEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_IdNullCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

template<szt IDM > using IdNegCibleNumEntity = IdCibleNumEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_IdNegCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, cible et code.
 */
template<szt IDM > class CibleSimpleCodeEntity : public EntityIDs<IDM,IdCibleAttribut,CibleAttribut,CodeAttribut> {
public:
    using EAID = EntityIDs<IDM,IdCibleAttribut,CibleAttribut,CodeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                    IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                    Cible = PositionEnum<CibleAttribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,IdCibleAttribut,CibleAttribut,CodeAttribut>::EntityIDs;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setCode;
    BASE_ENTITY(CibleSimpleCodeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleCodeEntity(idt idCible, int cible) {
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleCodeEntity(idt idCible, int cible, flag code, idt id = 0)
        : EAID(id) {
        setIdCible(idCible);
        setCible(cible);
        setCode(code);
    }
};

template<szt IDM > CibleSimpleCodeEntity<IDM>::~CibleSimpleCodeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, cible et num.
 */
template<szt IDM > class CibleSimpleNumEntity : public EntityIDs<IDM,IdCibleAttribut,CibleAttribut,NumAttribut> {
public:
    using EAID = EntityIDs<IDM,IdCibleAttribut,CibleAttribut,NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,IdCibleAttribut,CibleAttribut,NumAttribut>::EntityIDs;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    BASE_ENTITY(CibleSimpleNumEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleNumEntity(idt idCible, int cible) {
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleNumEntity(idt idCible, int cible, int num, idt id = 0)
        : EAID(id) {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }
};

template<szt IDM > CibleSimpleNumEntity<IDM>::~CibleSimpleNumEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, idEtat, cible, etat, num et dateTime.
 */
template<szt IDM > class HistoriqueEntity :
        public EntityIDs<IDM,IdCibleAttribut,CibleAttribut,IdEtatAttribut,EtatAttribut,NumAttribut,DateTimeValideAttribut> {
public:
    using EAID = EntityIDs<IDM,IdCibleAttribut,CibleAttribut,IdEtatAttribut,
        EtatAttribut,NumAttribut,DateTimeValideAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   IdEtat = PositionEnum<IdEtatAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut,EAID>::Position,
                   Etat = PositionEnum<EtatAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,IdCibleAttribut,CibleAttribut,IdEtatAttribut,EtatAttribut,NumAttribut,DateTimeValideAttribut>::EntityIDs;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setIdEtat;
    using EAID::setEtat;
    using EAID::setNum;
    using EAID::setDateTime;
    BASE_ENTITY(HistoriqueEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    HistoriqueEntity(idt idCible, int cible, int num = 0, idt id = 0)
        :EAID(id) {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    HistoriqueEntity(idt idCible, idt idEtat, int cible, const QDateTime & date, int etat, int num = 0, idt id = 0)
        :HistoriqueEntity(idCible,cible,num,id) {
        setIdEtat(idEtat);
        setEtat(etat);
        setDateTime(date);
    }
};

template<szt IDM > HistoriqueEntity<IDM>::~HistoriqueEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut IdCible, cible, type et valeur.
 */
template<szt IDM , class ValeurAtt> class CibleSimpleTypeValeurEntityTemp :
                public EntityIDs<IDM,CibleAttributs, TypeAttribut, ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,CibleAttributs, TypeAttribut, ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,CibleAttributs, TypeAttribut, ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(CibleSimpleTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleTypeValeurEntityTemp(idt idCible, int cible, idt type = 0) {
        setIdCible(idCible);
        setCible(cible);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleTypeValeurEntityTemp(idt idCible, int cible, idt type, ValTrans valeur, idt id = 0)
        : CibleSimpleTypeValeurEntityTemp(idCible,cible,type,valeur)
        {setId(id);}
};

template<szt IDM , class ValeurAtt> CibleSimpleTypeValeurEntityTemp<IDM, ValeurAtt>::~CibleSimpleTypeValeurEntityTemp() {}

template<szt IDM > using CibleSimpleTypeValeurDoubleEntity = CibleSimpleTypeValeurEntityTemp<IDM,ValeurDoubleAttribut>;
template<szt IDM > using CibleSimpleTypeValeurIntEntity = CibleSimpleTypeValeurEntityTemp<IDM,ValeurIntAttribut>;
template<szt IDM > using CibleSimpleTypeValeurVariantEntity = CibleSimpleTypeValeurEntityTemp<IDM,ValeurVariantAttribut>;


/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut IdCible, cible, datetime, num, type et valeur.
 */
template<szt IDM , class DateTimeAtt, class ValeurAtt> class CibleSimpleDateTimeNumTypeValeurEntityTemp :
            public EntityIDs<IDM,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, ValeurAtt>::EntityIDs;
    using EAID::setId;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(CibleSimpleDateTimeNumTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, int num = 0, idt type =0) {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, int num, idt type, ValTrans valeur, idt id = 0)
        : CibleSimpleDateTimeNumTypeValeurEntityTemp(idCible,cible,num,type) {
        setId(id);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, const QDateTime & dateTime,
                                               int num, idt type, ValTrans valeur, idt id = 0)
        : CibleSimpleDateTimeNumTypeValeurEntityTemp(idCible,cible,num,type,valeur,id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt, class ValeurAtt>
        CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt>::~CibleSimpleDateTimeNumTypeValeurEntityTemp() {}

template<szt IDM > using CibleSimpleDateTimeCurrentNumTypeValeurDoubleEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;

template<szt IDM > using CibleSimpleDateTimeCurrentNumTypeValeurIntEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;

template<szt IDM > using CibleSimpleDateTimeCurrentNumTypeValeurVariantEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;

template<szt IDM > using CibleSimpleDateTimeValideNumTypeValeurDoubleEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;

template<szt IDM > using CibleSimpleDateTimeValideNumTypeValeurIntEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;

template<szt IDM > using CibleSimpleDateTimeValideNumTypeValeurVariantEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_UtilisationTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position:szt {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, IdEtat = mere::IdEtat, Cible = mere::Cible,\
        DateTime = mere::DateTime, Etat = mere::Etat, Num = mere::Num,  NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Représentation de l'entité Utilisation.
 */
template<szt IDM , class Id1Att> class UtilisationEntityTemp :
            public EntityIDs<IDM,Id1Att,CibleDateTimeNumAttribut,EtatAttributs> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,CibleDateTimeNumAttribut,EtatAttributs>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   IdEtat = PositionEnum<IdEtatAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut,EAID>::Position,
                   Etat = PositionEnum<EtatAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,CibleDateTimeNumAttribut,EtatAttributs>::EntityIDs;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setIdEtat;
    using EAID::setEtat;
    using EAID::setNum;
    using EAID::setDateTime;
    BASE_ENTITY(UtilisationEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    UtilisationEntityTemp(idt idCible, int cible, int num = 0) {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    UtilisationEntityTemp(idt idCible, idt idEtat, int cible, int etat, int num = 0, idt id = 0)
        : UtilisationEntityTemp(idCible, cible, num) {
        setId(id);
        setIdEtat(idEtat);
        setEtat(etat);
    }

    //! Constructeur à partir des valeurs attributs.
    UtilisationEntityTemp(Id1Trans id1, idt idCible, idt idEtat, int cible,
                          const QDateTime & dateTime, int etat, int num, idt id = 0)
        : UtilisationEntityTemp(idCible,cible,idEtat,etat,num,id) {
        setId1(id1);
        setDateTime(dateTime);
    }
};

template<szt IDM , class Id1Att> UtilisationEntityTemp<IDM,Id1Att>::~UtilisationEntityTemp() {}

template<szt IDM > using UtilisationEntity = UtilisationEntityTemp<IDM,Id1Attribut>;
#define ENUM_Utilisation(ID1) ENUM_UtilisationTemp(ID1)

template<szt IDM > using UtilisationNullEntity = UtilisationEntityTemp<IDM,Id1NullAttribut>;
#define ENUM_UtilisationNull(ID1) ENUM_UtilisationTemp(ID1)

template<szt IDM > using UtilisationNegEntity = UtilisationEntityTemp<IDM,Id1NegAttribut>;
#define ENUM_UtilisationNeg(ID1) ENUM_UtilisationTemp(ID1)
}
#endif // ENTITYCIBLE_H
