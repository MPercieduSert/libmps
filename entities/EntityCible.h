/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITYCIBLE_H
#define ENTITYCIBLE_H

#include "Arbre.h"

//! \ingroup groupeMacroEntity
//! Macro implémentant le debut d'un classe ayant une clé.
#define ID1_ENTITY_DEBUT(ENTITY,TYPE,IDT,ID1) \
    /*! \ingroup groupeEntity \brief Représentation de l'entité ENTITY.*/ \
    class ENTITY : public TYPE##Entity<IDT> \
{public: \
    using TYPE##Entity<IDT>::TYPE##Entity; \
    using mere = TYPE##Entity<IDT>; \
    using mere::operator ==; \
    ENUM_##TYPE(ID1) \
    BASE_ENTITY(ENTITY)

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant une clé positive.
#define ID1_ENTITY(ENTITY,TYPE,IDT,ID1) ID1_ENTITY_DEBUT(ENTITY,TYPE,IDT,ID1) ALIAS_CLE(ID1,1)};

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant une clé négative.
#define ID1_ENTITY_NEG(ENTITY,TYPE,IDT,ID1) ID1_ENTITY_DEBUT(ENTITY,TYPE,IDT,ID1) ALIAS_CLE_NEG(ID1,1)};

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_IdNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1,  Nc = mere::Nc, Nom = mere::Nom, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<class Id1Att, int IDM> class IdNcNomEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,NcNomAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,NcNomAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,NcNomAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;

    BASE_ENTITY(IdNcNomEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomEntityTemp(Id1Type id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdNcNomEntityTemp(Id1Type id1, const QString & nc, const QString & nom, idt id = 0)
        : IdNcNomEntityTemp(nom, id)
    {
        setId1(id1);
        setNc(nc);
    }
};

template<class Id1Att, int IDM> IdNcNomEntityTemp<Id1Att, IDM>::~IdNcNomEntityTemp() {}

template<int IDM> using IdNcNomEntity = IdNcNomEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdNcNom(ID1) ENUM_IdNcNomTemp(ID1)

template<int IDM> using IdNullNcNomEntity = IdNcNomEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullNcNom(ID1) ENUM_IdNcNomTemp(ID1)

template<int IDM> using IdNegNcNomEntity = IdNcNomEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegNcNom(ID1) ENUM_IdNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_IdNcNomTypeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1,  Nc = mere::Nc, Nom = mere::Nom, Type = mere::Type, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<class Id1Att, int IDM> class IdNcNomTypeEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,NcNomTypeAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,NcNomTypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,NcNomTypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setType;

    BASE_ENTITY(IdNcNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomTypeEntityTemp(Id1Type id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNcNomTypeEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdNcNomTypeEntityTemp(Id1Type id1, const QString & nc, const QString & nom, idt type, idt id = 0)
        : IdNcNomTypeEntityTemp(nom, id)
    {
        setId1(id1);
        setNc(nc);
        setType(type);
    }
};

template<class Id1Att, int IDM> IdNcNomTypeEntityTemp<Id1Att, IDM>::~IdNcNomTypeEntityTemp() {}

template<int IDM> using IdNcNomTypeEntity = IdNcNomTypeEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

template<int IDM> using IdNullNcNomTypeEntity = IdNcNomTypeEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

template<int IDM> using IdNegNcNomTypeEntity = IdNcNomTypeEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegNcNomType(ID1) ENUM_IdNcNomTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un type et une version.
#define ENUM_IdTypeVersionTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Version = mere::Version, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<class Id1Att, int IDM> class IdTypeVersionEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,TypeAttribut,VersionAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,TypeAttribut,VersionAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Version = PositionEnum<VersionAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,TypeAttribut,VersionAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setType;
    using EAID::setVersion;

    BASE_ENTITY(IdTypeVersionEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeVersionEntityTemp(Id1Type id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeVersionEntityTemp(Id1Type id1, idt type, idt id)
        : EAID(id1, id)
        {setType(type);}

    //! Constructeur à partir des valeurs attributs.
    IdTypeVersionEntityTemp(Id1Type id1, idt type, int version, idt id = 0)
        : IdTypeVersionEntityTemp(id1, type, id)
        {setVersion(version);}
};

template<class Id1Att, int IDM> IdTypeVersionEntityTemp<Id1Att, IDM>::~IdTypeVersionEntityTemp() {}

template<int IDM> using IdTypeVersionEntity = IdTypeVersionEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

template<int IDM> using IdNullTypeVersionEntity = IdTypeVersionEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

template<int IDM> using IdNegTypeVersionEntity = IdTypeVersionEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegTypeVersion(ID1) ENUM_IdTypeVersionTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un nom cours et un nom.
#define ENUM_IdArbreSimpleNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Parent = mere::Parent, Nc = mere::Nc, Nom = mere::Nom,\
    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Ordre = mere::Ordre};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, nom cours, nom et parent.
 */
template<class Id1Att, int IDM> class IdArbreSimpleNcNomEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,NcNomAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,NcNomAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ArbreSimpleAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,NcNomAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    BASE_ENTITY(IdArbreSimpleNcNomEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleNcNomEntityTemp(Id1Type id1, idt id)
        : EAID(id)
        {setId1(id1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdArbreSimpleNcNomEntityTemp(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    IdArbreSimpleNcNomEntityTemp(Id1Type id1, const QString & nc, const QString & nom, idt parent, idt id = 0)
        : IdArbreSimpleNcNomEntityTemp(nom, id)
    {
        setId1(id1);
        setNc(nc);
        setParent(parent);
    }
};

template<class Id1Att, int IDM> IdArbreSimpleNcNomEntityTemp<Id1Att, IDM>::~IdArbreSimpleNcNomEntityTemp() {}

template<int IDM> using IdArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

template<int IDM> using IdNullArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

template<int IDM> using IdNegArbreSimpleNcNomEntity = IdArbreSimpleNcNomEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegArbreSimpleNcNom(ID1) ENUM_IdArbreSimpleNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un idProg, un nom cours et un nom.
#define ENUM_IdArbreSimpleIdProgNcNomTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Parent = mere::Parent, IdProg = mere::IdProg, Nc = mere::Nc,\
    Nom = mere::Nom, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Ordre = mere::Ordre};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, parent, idProg, nom cours et nom.
 */
template<class Id1Att, int IDM> class IdArbreSimpleIdProgNcNomEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,IdProgAttribut,NcNomAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,IdProgAttribut,NcNomAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdProg = PositionEnum<IdProgAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ArbreSimpleAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityAttributsID<Attributs<Id1Att,ArbreSimpleAttribut,IdProgAttribut,NcNomAttribut>,IDM>::EntityAttributsID;
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
    IdArbreSimpleIdProgNcNomEntityTemp(Id1Type id1, const QString & nc, const QString & nom,
                                       idt parent, idt idProg = 0, idt id = 0)
        : IdArbreSimpleIdProgNcNomEntityTemp(id, nom)
    {
        setId1(id1);
        setIdProg(idProg);
        setNc(nc);
        setParent(parent);
    }
};

template<class Id1Att, int IDM> IdArbreSimpleIdProgNcNomEntityTemp<Id1Att, IDM>::~IdArbreSimpleIdProgNcNomEntityTemp() {}

template<int IDM> using IdArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

template<int IDM> using IdNullArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

template<int IDM> using IdNegArbreSimpleIdProgNcNomEntity = IdArbreSimpleIdProgNcNomEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegArbreSimpleIdProgNcNom(ID1) ENUM_IdArbreSimpleIdProgNcNomTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé étrangère, une creation, une modification et un numéro.
#define ENUM_IdCreationModifNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Creation = mere::Creation, Modification = mere::Modification,\
    Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, creation, modification, et num.
 */
template<class Id1Att, int IDM> class IdCreationModifNumEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,CreationAttribut,ModificationAttribut,NumAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CreationAttribut,ModificationAttribut,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Creation = PositionEnum<CreationAttribut,EAID>::Position,
                   Modification = PositionEnum<ModificationAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CreationAttribut,ModificationAttribut,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setCreation;
    using EAID::setModification;
    using EAID::setNum;
    BASE_ENTITY(IdCreationModifNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdCreationModifNumEntityTemp(Id1Type id1, int num, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    IdCreationModifNumEntityTemp(Id1Type id1, const QDateTime & creation, const QDateTime & modif, int num, idt id = 0)
        : IdCreationModifNumEntityTemp(id1,num,id)
    {
        setCreation(creation);
        setModification(modif);
    }
};

template<class Id1Att, int IDM> IdCreationModifNumEntityTemp<Id1Att, IDM>::~IdCreationModifNumEntityTemp() {}

template<int IDM> using IdCreationModifNumEntity = IdCreationModifNumEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

template<int IDM> using IdNullCreationModifNumEntity = IdCreationModifNumEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

template<int IDM> using IdNegCreationModifNumEntity = IdCreationModifNumEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegCreationModifNum(ID1) ENUM_IdCreationModifNumTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_IdDateTimeTypeValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, DateTime = mere::Datetime, Type = mere::Type,\
    Valeur::mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime et Num.
 */
template<class DateTimeAtt, class ValeurAtt, class Id1Att, class T, int IDM> class IdDateTimeTypeValeurEntityTemp
        : public EntityAttributsID<Attributs<Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setDateTime;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(IdDateTimeTypeValeurEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdDateTimeTypeValeurEntityTemp(Id1Type id1, idt type = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    IdDateTimeTypeValeurEntityTemp(Id1Type id1, const QDateTime & dateTime, idt type, T valeur, idt id = 0)
        : IdDateTimeTypeValeurEntityTemp(id1,type,id)
    {
        setDateTime(dateTime);
        setValeur(valeur);
    }
};

template<class Id1Att, class DateTimeAtt, class ValeurAtt, class T, int IDM>
IdDateTimeTypeValeurEntityTemp<Id1Att,DateTimeAtt, ValeurAtt, T, IDM>::~IdDateTimeTypeValeurEntityTemp() {}

template<int IDM> using IdDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_IdDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_IdDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_IdDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_IdDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_IdDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_IdDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_IdNullDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_IdNullDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_IdNullDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_IdNullDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_IdNullDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNullDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_IdNullDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeCurrentTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_IdNegDateTimeCurrentTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeCurrentTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_IdNegDateTimeCurrentTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeCurrentTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_IdNegDateTimeCurrentTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeValideTypeValeurDoubleEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_IdNegDateTimeValideTypeValeurDouble(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeValideTypeValeurIntEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_IdNegDateTimeValideTypeValeurInt(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

template<int IDM> using IdNegDateTimeValideTypeValeurVariantEntity
= IdDateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_IdNegDateTimeValideTypeValeurVariant(ID1) ENUM_IdDateTimeTypeValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_CibleTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, NbrAtt = mere::NbrAtt,\
    Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<class Id1Att, int IDM> class CibleEntityTemp : public EntityAttributsID<Attributs<Id1Att,CibleAttributs>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttributs>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttributs>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    BASE_ENTITY(CibleEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    CibleEntityTemp(Id1Type id1, idt idCible, int cible, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
    }
};

template<class Id1Att, int IDM> CibleEntityTemp<Id1Att, IDM>::~CibleEntityTemp() {}

template<int IDM> using CibleEntity = CibleEntityTemp<Id1Attribut,IDM>;
#define ENUM_Cible(ID1) ENUM_CibleTemp(ID1)

template<int IDM> using CibleNullEntity = CibleEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_CibleNull(ID1) ENUM_CibleTemp(ID1)

template<int IDM> using CibleNegEntity = CibleEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_CibleNeg(ID1) ENUM_CibleTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, un numéro et un type.
#define ENUM_CibleNumTypeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, Num = mere::Num,\
    Type = mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, Num et Type.
 */
template<class Id1Att, int IDM> class CibleNumTypeEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,CibleAttributs,NumAttribut,TypeAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttributs,NumAttribut,TypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttributs,NumAttribut,TypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    using EAID::setType;
    BASE_ENTITY(CibleNumTypeEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    CibleNumTypeEntityTemp(idt idCible, int cible, int num, idt type)
    {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleNumTypeEntityTemp(Id1Type id1, idt idCible, int cible, int num, idt type, idt id = 0)
        : CibleNumTypeEntityTemp(idCible, cible, num, type)
    {
        setId(id);
        setId1(id1);
    }
};

template<class Id1Att, int IDM> CibleNumTypeEntityTemp<Id1Att, IDM>::~CibleNumTypeEntityTemp() {}

template<int IDM> using CibleNumTypeEntity = CibleNumTypeEntityTemp<Id1Attribut,IDM>;
#define ENUM_CibleNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

template<int IDM> using CibleNullNumTypeEntity = CibleNumTypeEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_CibleNullNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

template<int IDM> using CibleNegNumTypeEntity = CibleNumTypeEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_CibleNegNumType(ID1) ENUM_CibleNumTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_CibleNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut IdCible, cible, num et valeur.
 */
template<class AttributValeur, class Id1Att, class T, int IDM> class CibleNumValeurEntityTemp :
        public EntityAttributsID<Attributs<Id1Att, CibleAttributs, NumAttribut, AttributValeur>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att, CibleAttributs, NumAttribut, AttributValeur>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<AttributValeur,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att, CibleAttributs, NumAttribut, AttributValeur>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(CibleNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleNumValeurEntityTemp(Id1Type id1, idt idCible, int cible, int num = 0)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleNumValeurEntityTemp(Id1Type id1, idt idCible, int cible, int num, T valeur, idt id = 0)
        : CibleNumValeurEntityTemp(id1,idCible,cible,num)
    {
        setId(id);
        setValeur(valeur);
    }
};

template<class AttributValeur, class Id1Att, class T, int IDM>
    CibleNumValeurEntityTemp<AttributValeur, Id1Att, T, IDM>::~CibleNumValeurEntityTemp() {}

template<int IDM> using CibleNumValeurDoubleEntity
= CibleNumValeurEntityTemp<ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_CibleNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNumValeurIntEntity
= CibleNumValeurEntityTemp<ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_CibleNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNumValeurVariantEntity
= CibleNumValeurEntityTemp<ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_CibleNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNullNumValeurDoubleEntity
= CibleNumValeurEntityTemp<ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_CibleNullNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNullNumValeurIntEntity
= CibleNumValeurEntityTemp<ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_CibleNullNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNullNumValeurVariantEntity
= CibleNumValeurEntityTemp<ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_CibleNullNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNegNumValeurDoubleEntity
= CibleNumValeurEntityTemp<ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_CibleNegNumValeurDouble(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNegNumValeurIntEntity
= CibleNumValeurEntityTemp<ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_CibleNegNumValeurInt(ID1) ENUM_CibleNumValeurTemp(ID1)

template<int IDM> using CibleNegNumValeurVariantEntity
= CibleNumValeurEntityTemp<ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_CibleNegNumValeurVariant(ID1) ENUM_CibleNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible et une DateTime.
#define ENUM_CibleDateTimeTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    DateTime = mere::DateTime, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible et DateTime.
 */
template<class DateTimeAtt, class Id1Att, int IDM> class CibleDateTimeEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,CibleAttributs,DateTimeAtt>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttributs,DateTimeAtt>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttributs,DateTimeAtt>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    BASE_ENTITY(CibleDateTimeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeEntityTemp(Id1Type id1, idt idCible, int cible, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeEntityTemp(Id1Type id1, idt idCible, int cible, const QDateTime & dateTime, idt id = 0)
        : CibleDateTimeEntityTemp(id1,idCible,cible,id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class Id1Att, int IDM> CibleDateTimeEntityTemp<DateTimeAtt, Id1Att, IDM>
    ::~CibleDateTimeEntityTemp() {}

template<int IDM> using CibleDateTimeCurrentEntity = CibleDateTimeEntityTemp<DateTimeCurrentAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<int IDM> using CibleDateTimeValideEntity = CibleDateTimeEntityTemp<DateTimeValideAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentEntity = CibleDateTimeEntityTemp<DateTimeCurrentAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<int IDM> using CibleNullDateTimeValideEntity = CibleDateTimeEntityTemp<DateTimeValideAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentEntity = CibleDateTimeEntityTemp<DateTimeCurrentAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeCurrent(ID1) ENUM_CibleDateTimeTemp(ID1)

template<int IDM> using CibleNegDateTimeValideEntity = CibleDateTimeEntityTemp<DateTimeValideAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeValide(ID1) ENUM_CibleDateTimeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num et une DateTime.
#define ENUM_CibleDateTimeNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, \
    DateTime = mere::DateTime, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, DateTime et Num.
 */
template<class DateTimeAtt, class Id1Att, int IDM> class CibleDateTimeNumEntityTemp :
        public EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(CibleDateTimeNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumEntityTemp(Id1Type id1, idt idCible, int cible, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumEntityTemp(Id1Type id1, idt idCible, int cible, const QDateTime & dateTime, int num, idt id = 0)
        : CibleDateTimeNumEntityTemp(id1,idCible,cible,num,id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class Id1Att, int IDM> CibleDateTimeNumEntityTemp<DateTimeAtt, Id1Att, IDM>::~CibleDateTimeNumEntityTemp() {}

template<int IDM> using CibleDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<DateTimeCurrentAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<int IDM> using CibleDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<DateTimeValideAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<DateTimeCurrentAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<int IDM> using CibleNullDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<DateTimeValideAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentNumEntity = CibleDateTimeNumEntityTemp<DateTimeCurrentAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeCurrentNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

template<int IDM> using CibleNegDateTimeValideNumEntity = CibleDateTimeNumEntityTemp<DateTimeValideAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeValideNum(ID1) ENUM_CibleDateTimeNumTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num, type et une DateTime.
#define ENUM_CibleDateTimeNumTypeTemp(ID1) /*! \brief Positions des attributs */ \
        enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible, \
        DateTime = mere::DateTime, Num = mere::Num, Type= mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible, Cible, DateTime et Num.
 */
template<class DateTimeAtt, class Id1Att, int IDM> class CibleDateTimeNumTypeEntityTemp :
            public EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                       Id1 = PositionEnum<Id1Att,EAID>::Position,
                       IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                       Cible = PositionEnum<CibleAttribut,EAID>::Position,
                       DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                       Num = PositionEnum<NumAttribut,EAID>::Position,
                       Type = PositionEnum<TypeAttribut,EAID>::Position,
                       NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setType;
    BASE_ENTITY(CibleDateTimeNumTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumTypeEntityTemp(Id1Type id1, idt idCible, int cible, int num = 0, idt type = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumTypeEntityTemp(Id1Type id1, idt idCible, int cible, const QDateTime & dateTime,
                                   int num, idt type, idt id = 0)
        : CibleDateTimeNumTypeEntityTemp(id1,idCible,cible,num,type,id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class Id1Att, int IDM> CibleDateTimeNumTypeEntityTemp<DateTimeAtt, Id1Att, IDM>
    ::~CibleDateTimeNumTypeEntityTemp() {}

template<int IDM> using CibleDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeCurrentAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeCurrentNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<int IDM> using CibleDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeValideAttribut,Id1Attribut,IDM>;
#define ENUM_CibleDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeCurrentAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeCurrentTypeNum(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<int IDM> using CibleNullDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeValideAttribut,Id1NullAttribut,IDM>;
#define ENUM_CibleNullDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeCurrentAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeCurrentTypeNum(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

template<int IDM> using CibleNegDateTimeValideNumTypeEntity = CibleDateTimeNumTypeEntityTemp<DateTimeValideAttribut,Id1NegAttribut,IDM>;
#define ENUM_CibleNegDateTimeValideNumType(ID1) ENUM_CibleDateTimeNumTypeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_CibleDateTimeNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, Cible = mere::Cible,\
    DateTime = mere::DateTime, Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut une clé, IdCible, cible, datetime, num et valeur.
 */
template<class DateTimeAtt, class AttributValeur, class Id1Att, class T, int IDM> class CibleDateTimeNumValeurEntityTemp :
        public EntityAttributsID<Attributs<Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, AttributValeur>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, AttributValeur>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<AttributValeur,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att, CibleAttributs, DateTimeAtt, NumAttribut, AttributValeur>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(CibleDateTimeNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleDateTimeNumValeurEntityTemp(Id1Type id1, idt idCible, int cible, int num = 0)
    {
        setId1(id1);
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumValeurEntityTemp(Id1Type id1, idt idCible, int cible, int num, T valeur, idt id = 0)
        : CibleDateTimeNumValeurEntityTemp(id1,idCible,cible,num)
    {
        setId(id);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleDateTimeNumValeurEntityTemp(Id1Type id1, idt idCible, int cible, const QDateTime & dateTime, int num, T valeur, idt id = 0)
        : CibleDateTimeNumValeurEntityTemp(id1,idCible,cible,num,valeur,id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class AttributValeur, class Id1Att, class T, int IDM>
    CibleDateTimeNumValeurEntityTemp<DateTimeAtt, AttributValeur, Id1Att, T, IDM>::~CibleDateTimeNumValeurEntityTemp() {}

template<int IDM> using CibleDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_CibleDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_CibleDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_CibleDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_CibleDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_CibleDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_CibleDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_CibleNullDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_CibleNullDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_CibleNullDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_CibleNullDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_CibleNullDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNullDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_CibleNullDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_CibleNegDateTimeCurrentNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_CibleNegDateTimeCurrentNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeCurrentNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_CibleNegDateTimeCurrentNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeValideNumValeurDoubleEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_CibleNegDateTimeValideNumValeurDouble(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeValideNumValeurIntEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_CibleNegDateTimeValideNumValeurInt(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

template<int IDM> using CibleNegDateTimeValideNumValeurVariantEntity
= CibleDateTimeNumValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_CibleNegDateTimeValideNumValeurVariant(ID1) ENUM_CibleDateTimeNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, num et Valeur.
#define ENUM_IdNumValeurTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Num = mere::Num, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, num et valeur.
 */
template<class AttributValeur, class Id1Att, class T,  int IDM> class IdNumValeurEntityTemp :
            public EntityAttributsID<Attributs<Id1Att,NumAttribut,AttributValeur>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,NumAttribut,AttributValeur>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<AttributValeur,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,NumAttribut,AttributValeur>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setNum;
    BASE_ENTITY(IdNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdNumValeurEntityTemp(Id1Type id1, int num)
    {
        setId1(id1);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    IdNumValeurEntityTemp(Id1Type id1, int num, T valeur, idt id = 0)
        : IdNumValeurEntityTemp(id1,num)
    {
        setId(id);
        setValeur(valeur);
    }
};

template<class AttributValeur, class Id1Att, class T,  int IDM> IdNumValeurEntityTemp<AttributValeur, Id1Att, T, IDM>
    ::~IdNumValeurEntityTemp() {}

template<int IDM> using IdNumValeurDoubleEntity = IdNumValeurEntityTemp<ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_IdNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNumValeurIntEntity = IdNumValeurEntityTemp<ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_IdNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNumValeurVariantEntity = IdNumValeurEntityTemp<ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_IdNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNullNumValeurDoubleEntity = IdNumValeurEntityTemp<ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_IdNullNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNullNumValeurIntEntity = IdNumValeurEntityTemp<ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_IdNullNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNullNumValeurVariantEntity = IdNumValeurEntityTemp<ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_IdNullNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNegNumValeurDoubleEntity = IdNumValeurEntityTemp<ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_IdNegNumValeurDouble(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNegNumValeurIntEntity = IdNumValeurEntityTemp<ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_IdNegNumValeurInt(ID1) ENUM_IdNumValeurTemp(ID1)

template<int IDM> using IdNegNumValeurVariantEntity = IdNumValeurEntityTemp<ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_IdNegNumValeurVariant(ID1) ENUM_IdNumValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, type et Valeur.
#define ENUM_IdTypeValeurTemp(ID1) /*! \brief Positions des attributs */ \
enum Position {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, type et valeur.
 */
template<class AttributValeur, class Id1Att, class T, int IDM> class IdTypeValeurEntityTemp :
            public EntityAttributsID<Attributs<Id1Att,TypeAttribut,AttributValeur>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,TypeAttribut,AttributValeur>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<AttributValeur,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,TypeAttribut,AttributValeur>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setId1;
    using EAID::setType;
    BASE_ENTITY(IdTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    IdTypeValeurEntityTemp(Id1Type id1, idt type)
    {
        setId1(id1);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    IdTypeValeurEntityTemp(Id1Type id1, idt type, T valeur, idt id = 0)
        : IdTypeValeurEntityTemp(id1, type)
    {
        setId(id);
        setValeur(valeur);
    }
};

template<class AttributValeur, class Id1Att, class T, int IDM> IdTypeValeurEntityTemp<AttributValeur, Id1Att, T, IDM>
    ::~IdTypeValeurEntityTemp() {}

template<int IDM> using IdTypeValeurDoubleEntity = IdTypeValeurEntityTemp<ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_IdTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdTypeValeurIntEntity = IdTypeValeurEntityTemp<ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_IdTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdTypeValeurVariantEntity = IdTypeValeurEntityTemp<ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_IdTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNullTypeValeurDoubleEntity = IdTypeValeurEntityTemp<ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_IdNullTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNullTypeValeurIntEntity = IdTypeValeurEntityTemp<ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_IdNullTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNullTypeValeurVariantEntity = IdTypeValeurEntityTemp<ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_IdNullTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNegTypeValeurDoubleEntity = IdTypeValeurEntityTemp<ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_IdNegTypeValeurDouble(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNegTypeValeurIntEntity = IdTypeValeurEntityTemp<ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_IdNegTypeValeurInt(ID1) ENUM_IdTypeValeurTemp(ID1)

template<int IDM> using IdNegTypeValeurVariantEntity = IdTypeValeurEntityTemp<ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_IdNegTypeValeurVariant(ID1) ENUM_IdTypeValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, num, type, valeur et version.
#define ENUM_IdNumTypeVersionValeurTemp(ID1) /*! \brief Positions des attributs */ \
enum Position {Id = mere::Id, Id1 = mere::Id1, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé, num, type, version et valeur.
 */
template<class AttributValeur, class Id1Att, class T, int IDM> class IdNumTypeVersionValeurEntityTemp :
                public EntityAttributsID<Attributs<Id1Att,NumAttribut,TypeAttribut,VersionAttribut,AttributValeur>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,NumAttribut,TypeAttribut,VersionAttribut,AttributValeur>,IDM>;
        //! Positions des attributs.
        enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                       Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                       Num = PositionEnum<NumAttribut,EAID>::Position,
                       Type = PositionEnum<TypeAttribut,EAID>::Position,
                       Valeur = PositionEnum<AttributValeur,EAID>::Position,
                       Version = PositionEnum<VersionAttribut,EAID>::Position,
                       NbrAtt = EAID::NbrAtt};

        using EntityAttributsID<Attributs<Id1Att,NumAttribut,TypeAttribut,VersionAttribut,AttributValeur>,IDM>::EntityAttributsID;
        using EAID::setId;
        using EAID::setId1;
        using EAID::setNum;
        using EAID::setType;
        using EAID::setVersion;
        BASE_ENTITY(IdNumTypeVersionValeurEntityTemp)

        //! Constructeur à partir d'un jeux de valeurs attributs unique.
        IdNumTypeVersionValeurEntityTemp(Id1Type id1, int num, idt id =0)
            : EAID(id)
        {
            setId1(id1);
            setNum(num);
        }

        //! Constructeur à partir des valeurs attributs.
        IdNumTypeVersionValeurEntityTemp(Id1Type id1, int num, idt type, T valeur, int version, idt id = 0)
            : IdNumTypeVersionValeurEntityTemp(id1, num, id)
        {
            setType(type);
            setValeur(valeur);
            setVersion(version);
        }
    };

template<class AttributValeur, class Id1Att, class T, int IDM> IdNumTypeVersionValeurEntityTemp<AttributValeur, Id1Att, T, IDM>
    ::~IdNumTypeVersionValeurEntityTemp() {}

template<int IDM> using IdNumTypeVersionValeurDoubleEntity = IdNumTypeVersionValeurEntityTemp<ValeurDoubleAttribut,Id1Attribut,double,IDM>;
#define ENUM_IdNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<ValeurIntAttribut,Id1Attribut,int,IDM>;
#define ENUM_IdNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<ValeurVariantAttribut,Id1Attribut,const QVariant &,IDM>;
#define ENUM_IdNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNullNumTypeVersionValeurDoubleEntity
    = IdNumTypeVersionValeurEntityTemp<ValeurDoubleAttribut,Id1NullAttribut,double,IDM>;
#define ENUM_IdNullNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNullNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<ValeurIntAttribut,Id1NullAttribut,int,IDM>;
#define ENUM_IdNullNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNullNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<ValeurVariantAttribut,Id1NullAttribut,const QVariant &,IDM>;
#define ENUM_IdNullNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNegNumTypeVersionValeurDoubleEntity
    = IdNumTypeVersionValeurEntityTemp<ValeurDoubleAttribut,Id1NegAttribut,double,IDM>;
#define ENUM_IdNegNumTypeVersionValeurDouble(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNegNumTypeVersionValeurIntEntity = IdNumTypeVersionValeurEntityTemp<ValeurIntAttribut,Id1NegAttribut,int,IDM>;
#define ENUM_IdNegNumTypeVersionValeurInt(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

template<int IDM> using IdNegNumTypeVersionValeurVariantEntity
    = IdNumTypeVersionValeurEntityTemp<ValeurVariantAttribut,Id1NegAttribut,const QVariant &,IDM>;
#define ENUM_IdNegNumTypeVersionValeurVariant(ID1) ENUM_IdNumTypeVersionValeurTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une cible et un code.
#define ENUM_IdCibleCodeTemp(ID1) /*! \brief Positions des attributs */ \
enum Position {Id = mere::Id, Id1 = mere::Id1, Cible = mere::Cible, Code = mere::Code, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<class Id1Att, int IDM> class IdCibleCodeEntityTemp : public EntityAttributsID<Attributs<Id1Att,CibleAttribut,CodeAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttribut,CodeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                    Id1 = PositionEnum<Id1Att,EAID>::Position,
                    Cible = PositionEnum<CibleAttribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttribut,CodeAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setCible;
    using EAID::setCode;
    BASE_ENTITY(IdCibleCodeEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdCibleCodeEntityTemp(Id1Type id1, int cible, codeType code = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setCible(cible);
        setCode(code);
    }
};

template<class Id1Att, int IDM> IdCibleCodeEntityTemp<Id1Att, IDM>::~IdCibleCodeEntityTemp() {}

template<int IDM> using IdCibleCodeEntity = IdCibleCodeEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

template<int IDM> using IdNullCibleCodeEntity = IdCibleCodeEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

template<int IDM> using IdNegCibleCodeEntity = IdCibleCodeEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegCibleCode(ID1) ENUM_IdCibleCodeTemp(ID1)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une cible et un num.
#define ENUM_IdCibleNumTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, Cible = mere::Cible, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut clé étrangère, idCible et Cible.
 */
template<class Id1Att, int IDM> class IdCibleNumEntityTemp : public EntityAttributsID<Attributs<Id1Att,CibleAttribut,NumAttribut>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleAttribut,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleAttribut,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setCible;
    using EAID::setNum;
    BASE_ENTITY(IdCibleNumEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    IdCibleNumEntityTemp(Id1Type id1, int cible, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setCible(cible);
        setNum(num);
    }
};

template<class Id1Att, int IDM> IdCibleNumEntityTemp<Id1Att, IDM>::~IdCibleNumEntityTemp() {}

template<int IDM> using IdCibleNumEntity = IdCibleNumEntityTemp<Id1Attribut,IDM>;
#define ENUM_IdCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

template<int IDM> using IdNullCibleNumEntity = IdCibleNumEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_IdNullCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

template<int IDM> using IdNegCibleNumEntity = IdCibleNumEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_IdNegCibleNum(ID1) ENUM_IdCibleNumTemp(ID1)

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, cible et code.
 */
template<int IDM> class CibleSimpleCodeEntity : public EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,CodeAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,CodeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                    IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                    Cible = PositionEnum<CibleAttribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,CodeAttribut>,IDM>::EntityAttributsID;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setCode;
    BASE_ENTITY(CibleSimpleCodeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleCodeEntity(idt idCible, int cible)
    {
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleCodeEntity(idt idCible, int cible, codeType code, idt id = 0)
        : EAID(id)
    {
        setIdCible(idCible);
        setCible(cible);
        setCode(code);
    }
};

template<int IDM> CibleSimpleCodeEntity<IDM>::~CibleSimpleCodeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, cible et num.
 */
template<int IDM> class CibleSimpleNumEntity : public EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,NumAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setNum;
    BASE_ENTITY(CibleSimpleNumEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleNumEntity(idt idCible, int cible)
    {
        setIdCible(idCible);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleNumEntity(idt idCible, int cible, int num, idt id = 0)
        : EAID(id)
    {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }
};

template<int IDM> CibleSimpleNumEntity<IDM>::~CibleSimpleNumEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut idCible, idEtat, cible, etat, num et dateTime.
 */
template<int IDM> class HistoriqueEntity :
        public EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,IdEtatAttribut,EtatAttribut,NumAttribut,DateTimeValideAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,IdEtatAttribut,
        EtatAttribut,NumAttribut,DateTimeValideAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   IdEtat = PositionEnum<IdEtatAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut,EAID>::Position,
                   Etat = PositionEnum<EtatAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<IdCibleAttribut,CibleAttribut,IdEtatAttribut,
    EtatAttribut,NumAttribut,DateTimeValideAttribut>,IDM>::EntityAttributsID;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setIdEtat;
    using EAID::setEtat;
    using EAID::setNum;
    using EAID::setDateTime;
    BASE_ENTITY(HistoriqueEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    HistoriqueEntity(idt idCible, int cible, int num = 0, idt id = 0)
        :EAID(id)
    {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    HistoriqueEntity(idt idCible, idt idEtat, int cible, const QDateTime & date, int etat, int num = 0, idt id = 0)
        :HistoriqueEntity(idCible,cible,num,id)
    {
        setIdEtat(idEtat);
        setEtat(etat);
        setDateTime(date);
    }
};

template<int IDM> HistoriqueEntity<IDM>::~HistoriqueEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut IdCible, cible, datetime, num, type et valeur.
 */
template<class DateTimeAtt, class AttributValeur, class T, int IDM> class CibleSimpleDateTimeNumTypeValeurEntityTemp :
            public EntityAttributsID<Attributs<CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, AttributValeur>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, AttributValeur>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<AttributValeur,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<CibleAttributs, DateTimeAtt, NumAttribut, TypeAttribut, AttributValeur>,IDM>::EntityAttributsID;
    using EAID::setId;
    using EAID::setIdCible;
    using EAID::setCible;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(CibleSimpleDateTimeNumTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, int num = 0, idt type =0)
    {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, int num, idt type, T valeur, idt id = 0)
        : CibleSimpleDateTimeNumTypeValeurEntityTemp(idCible,cible,num,type)
    {
        setId(id);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    CibleSimpleDateTimeNumTypeValeurEntityTemp(idt idCible, int cible, const QDateTime & dateTime,
                                               int num, idt type, T valeur, idt id = 0)
        : CibleSimpleDateTimeNumTypeValeurEntityTemp(idCible,cible,num,type,valeur,id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class AttributValeur, class T, int IDM>
        CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeAtt, AttributValeur, T, IDM>::~CibleSimpleDateTimeNumTypeValeurEntityTemp() {}

template<int IDM> using CibleSimpleDateTimeCurrentNumTypeValeurDoubleEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurDoubleAttribut,double,IDM>;

template<int IDM> using CibleSimpleDateTimeCurrentNumTypeValeurIntEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,int,IDM>;

template<int IDM> using CibleSimpleDateTimeCurrentNumTypeValeurVariantEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurVariantAttribut,const QVariant &,IDM>;

template<int IDM> using CibleSimpleDateTimeValideNumTypeValeurDoubleEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeValideAttribut,ValeurDoubleAttribut,double,IDM>;

template<int IDM> using CibleSimpleDateTimeValideNumTypeValeurIntEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,int,IDM>;

template<int IDM> using CibleSimpleDateTimeValideNumTypeValeurVariantEntity
    = CibleSimpleDateTimeNumTypeValeurEntityTemp<DateTimeValideAttribut,ValeurVariantAttribut,const QVariant &,IDM>;

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une DateTime et Valeur.
#define ENUM_UtilisationTemp(ID1) /*! \brief Positions des attributs */ \
    enum Position {Id = mere::Id, Id1 = mere::Id1, IdCible = mere::IdCible, IdEtat = mere::IdEtat, Cible = mere::Cible,\
        DateTime = mere::DateTime, Etat = mere::Etat, Num = mere::Num,  NbrAtt = mere::NbrAtt, Id ## ID1 = Id1};

/*! \ingroup groupeBaseEntity
 * \brief Représentation de l'entité Utilisation.
 */
template<class Id1Att, int IDM> class UtilisationEntityTemp :
            public EntityAttributsID<Attributs<Id1Att,CibleDateTimeNumAttribut,EtatAttributs>,IDM>
{
public:
    using Id1Type = typename Id1Att::AttType;
    using EAID = EntityAttributsID<Attributs<Id1Att,CibleDateTimeNumAttribut,EtatAttributs>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   IdEtat = PositionEnum<IdEtatAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut,EAID>::Position,
                   Etat = PositionEnum<EtatAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<Id1Att,CibleDateTimeNumAttribut,EtatAttributs>,IDM>::EntityAttributsID;
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
    UtilisationEntityTemp(idt idCible, int cible, int num = 0)
    {
        setIdCible(idCible);
        setCible(cible);
        setNum(num);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    UtilisationEntityTemp(idt idCible, idt idEtat, int cible, int etat, int num = 0, idt id = 0)
        : UtilisationEntityTemp(idCible, cible, num)
    {
        setId(id);
        setIdEtat(idEtat);
        setEtat(etat);
    }

    //! Constructeur à partir des valeurs attributs.
    UtilisationEntityTemp(Id1Type id1, idt idCible, idt idEtat, int cible,
                          const QDateTime & dateTime, int etat, int num, idt id = 0)
        : UtilisationEntityTemp(idCible,cible,idEtat,etat,num,id)
    {
        setId1(id1);
        setDateTime(dateTime);
    }
};

template<class Id1Att, int IDM> UtilisationEntityTemp<Id1Att,IDM>::~UtilisationEntityTemp() {}

template<int IDM> using UtilisationEntity = UtilisationEntityTemp<Id1Attribut,IDM>;
#define ENUM_Utilisation(ID1) ENUM_UtilisationTemp(ID1)

template<int IDM> using UtilisationNullEntity = UtilisationEntityTemp<Id1NullAttribut,IDM>;
#define ENUM_UtilisationNull(ID1) ENUM_UtilisationTemp(ID1)

template<int IDM> using UtilisationNegEntity = UtilisationEntityTemp<Id1NegAttribut,IDM>;
#define ENUM_UtilisationNeg(ID1) ENUM_UtilisationTemp(ID1)
#endif // ENTITYCIBLE_H
