/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2016
 */
#ifndef ENTITYRELATION
#define ENTITYRELATION

#include "Entity.h"

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant 2 clé.
#define RELATION_ENTITY(ENTITY,TYPE,IDT,ID1,ID2) \
    /*! \ingroup groupeEntity \brief Représentation de l'entité ENTITY.*/ \
    class ENTITY : public Relation##TYPE##Entity<IDT> \
    {public: \
    using Relation##TYPE##Entity<IDT>::Relation##TYPE##Entity; \
    using mere = Relation##TYPE##Entity<IDT>; \
    using mere::operator ==; \
    ENUM_Relation##TYPE(ID1,ID2)\
    BASE_ENTITY(ENTITY) \
    ALIAS_CLE(ID1,1) \
    ALIAS_CLE(ID2,2)};


//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_Relation(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés.
 */
template<int IDM> class RelationEntity : public EntityAttributsID<RelationAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<RelationAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<RelationAttribut,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    BASE_ENTITY(RelationEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationEntity(idt id1, idt id2, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
    }
};

template<int IDM> RelationEntity<IDM>::~RelationEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_RelationExactOneNotNull(ID1,ID2) ENUM_(ID1,ID2)

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés dont une seule est non nulle et une troisième.
 */
template<int IDM> class RelationExactOneNotNullEntity : public EntityAttributsID<RelationExactOneNotNullAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<RelationExactOneNotNullAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<RelationExactOneNotNullAttribut,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    BASE_ENTITY(RelationExactOneNotNullEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationExactOneNotNullEntity(idt id1, idt id2, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
    }
};

template<int IDM> RelationExactOneNotNullEntity<IDM>::~RelationExactOneNotNullEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime.
#define ENUM_RelationDateTime(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et une dateTime.
 */
template<class DateTimeAtt, int IDM> class RelationDateTimeEntity : public EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    BASE_ENTITY(RelationDateTimeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeEntity(idt id1, idt id2, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeEntity(idt id1, idt id2, const QDateTime & dateTime, idt id = 0)
        : RelationDateTimeEntity(id1, id2, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, int IDM> RelationDateTimeEntity<DateTimeAtt, IDM>::~RelationDateTimeEntity() {}

template<int IDM> using RelationDateTimeCurrentEntity = RelationDateTimeEntity<DateTimeCurrentAttribut,IDM>;
#define ENUM_RelationDateTimeCurrent(ID1,ID2) ENUM_RelationDateTime(ID1,ID2)

template<int IDM> using RelationDateTimeValideEntity = RelationDateTimeEntity<DateTimeValideAttribut,IDM>;
#define ENUM_RelationDateTimeValide(ID1,ID2) ENUM_RelationDateTime(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut num.
#define ENUM_RelationNum(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<int IDM> class RelationNumEntity : public EntityAttributsID<Attributs<RelationAttribut,NumAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setNum;
    BASE_ENTITY(RelationNumEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationNumEntity(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }
};

template<int IDM> RelationNumEntity<IDM>::~RelationNumEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut type.
#define ENUM_RelationType(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, Type = mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<int IDM> class RelationTypeEntity : public EntityAttributsID<Attributs<RelationAttribut,TypeAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,TypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,TypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setType;
    BASE_ENTITY(RelationTypeEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationTypeEntity(idt id1, idt id2, idt type = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setType(type);
    }
};

template<int IDM> RelationTypeEntity<IDM>::~RelationTypeEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime et num.
#define ENUM_RelationDateTimeNum(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Num = mere::Num,\
                    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime et un numero.
 */
template<class DateTimeAtt,int IDM> class RelationDateTimeNumEntity :
        public EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(RelationDateTimeNumEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeNumEntity(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumEntity(idt id1, idt id2, const QDateTime & dateTime, int num, idt id = 0)
        : RelationDateTimeNumEntity(id1, id2, num, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt,int IDM> RelationDateTimeNumEntity<DateTimeAtt, IDM>::~RelationDateTimeNumEntity() {}

template<int IDM> using RelationDateTimeCurrentNumEntity = RelationDateTimeNumEntity<DateTimeCurrentAttribut,IDM>;
#define ENUM_RelationDateTimeCurrentNum(ID1,ID2) ENUM_RelationDateTimeNum(ID1,ID2)

template<int IDM> using RelationDateTimeValideNumEntity = RelationDateTimeNumEntity<DateTimeValideAttribut,IDM>;
#define ENUM_RelationDateTimeValideNum(ID1,ID2) ENUM_RelationDateTimeNum(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationDateTimeValeur(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Valeur = mere::Valeur,\
                NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime et un numero.
 */
template<class DateTimeAtt,class ValeurAttribut,class T,int IDM> class RelationDateTimeValeurEntity :
        public EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,ValeurAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,ValeurAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Valeur = PositionEnum<ValeurAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,ValeurAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setValeur;
    BASE_ENTITY(RelationDateTimeValeurEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeValeurEntity(idt id1, idt id2, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeValeurEntity(idt id1, idt id2, T valeur, idt id)
        : RelationDateTimeValeurEntity(id1, id2, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeValeurEntity(idt id1, idt id2, const QDateTime & dateTime, T valeur, idt id = 0)
        : RelationDateTimeValeurEntity(id1, id2, valeur, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt,class ValeurAttribut,class T,int IDM>
    RelationDateTimeValeurEntity<DateTimeAtt, ValeurAttribut, T, IDM>::~RelationDateTimeValeurEntity() {}

template<int IDM> using RelationDateTimeCurrentValeurDoubleEntity
    = RelationDateTimeValeurEntity<DateTimeCurrentAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationDateTimeCurrentValeurDouble(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

template<int IDM> using RelationDateTimeCurrentValeurIntEntity
    = RelationDateTimeValeurEntity<DateTimeCurrentAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationDateTimeCurrentValeurInt(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

template<int IDM> using RelationDateTimeCurrentValeurVariantEntity
    = RelationDateTimeValeurEntity<DateTimeCurrentAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationDateTimeCurrentValeurVariant(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideValeurDoubleEntity
    = RelationDateTimeValeurEntity<DateTimeValideAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationDateTimeValideValeurDouble(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideValeurIntEntity
    = RelationDateTimeValeurEntity<DateTimeValideAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationDateTimeValideValeurInt(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideValeurVariantEntity
    = RelationDateTimeValeurEntity<DateTimeValideAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationDateTimeValideValeurVariant(ID1,ID2) ENUM_RelationDateTimeValeur(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationDateTimeNumValeur(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Num = mere::Num, Valeur = mere::Valeur,\
                NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime, un numero et une valeur.
 */
template<class DateTimeAtt,class ValeurAttribut,class T,int IDM> class RelationDateTimeNumValeurEntity :
        public EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(RelationDateTimeNumValeurEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeNumValeurEntity(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumValeurEntity(idt id1, idt id2, int num, T valeur, idt id = 0)
        : RelationDateTimeNumValeurEntity(id1, id2, num, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumValeurEntity(idt id1, idt id2, const QDateTime & dateTime, int num, T valeur, idt id = 0)
        : RelationDateTimeNumValeurEntity(id1, id2, num, valeur, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt,class ValeurAttribut,class T,int IDM>
    RelationDateTimeNumValeurEntity<DateTimeAtt, ValeurAttribut, T, IDM>::~RelationDateTimeNumValeurEntity() {}

template<int IDM> using RelationDateTimeCurrentNumValeurDoubleEntity
    = RelationDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationDateTimeCurrentNumValeurDouble(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationDateTimeCurrentNumValeurIntEntity
    = RelationDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationDateTimeCurrentNumValeurInt(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationDateTimeCurrentNumValeurVariantEntity
    = RelationDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationDateTimeCurrentNumValeurVariant(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideNumValeurDoubleEntity
    = RelationDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationDateTimeValideNumValeurDouble(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideNumValeurIntEntity
    = RelationDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationDateTimeValideNumValeurInt(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationDateTimeValideNumValeurVariantEntity
    = RelationDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationDateTimeValideNumValeurVariant(ID1,ID2) ENUM_RelationDateTimeNumValeur(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, dateTime, num et Valeur.
#define ENUM_RelationCodeDateTimeNumValeur(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, Code = mere::Code, DateTime = mere::DateTime, Num = mere::Num,\
                    Valeur = mere::Valeur,nNbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un code, une dateTime et un numero.
 */
template<class DateTimeAtt,class ValeurAttribut,class T,int IDM> class RelationCodeDateTimeNumValeurEntity :
            public EntityAttributsID<Attributs<RelationAttribut,CodeAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,CodeAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                    Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                    Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                    Num = PositionEnum<NumAttribut,EAID>::Position,
                    Valeur = PositionEnum<ValeurAttribut,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,CodeAttribut,DateTimeAtt,NumAttribut,ValeurAttribut>,IDM>
                        ::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setCode;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(RelationCodeDateTimeNumValeurEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationCodeDateTimeNumValeurEntity(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeNumValeurEntity(idt id1, idt id2, int num, T valeur, idt id = 0)
        : RelationCodeDateTimeNumValeurEntity(id1, id2, num, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeNumValeurEntity(idt id1, idt id2, int code, const QDateTime & dateTime, int num, T valeur, idt id = 0)
        : RelationCodeDateTimeNumValeurEntity(id1, id2, num, id)
    {
        setCode(code);
        setDateTime(dateTime);
        setValeur(valeur);
    }
};

template<class DateTimeAtt,class ValeurAttribut,class T,int IDM>
        RelationCodeDateTimeNumValeurEntity<DateTimeAtt, ValeurAttribut, T, IDM>::~RelationCodeDateTimeNumValeurEntity() {}

template<int IDM> using RelationCodeDateTimeCurrentNumValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationCodeDateTimeCurrentNumValeurIntEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationCodeDateTimeCurrentNumValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeCurrentAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationCodeDateTimeValideNumValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationCodeDateTimeValideNumValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationCodeDateTimeValideNumValeurIntEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurIntAttribut,int,IDM>;
#define ENUM_RelationCodeDateTimeValideNumValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)

template<int IDM> using RelationCodeDateTimeValideNumValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntity<DateTimeValideAttribut,ValeurVariantAttribut,const QVariant &,IDM>;
    #define ENUM_RelationCodeDateTimeValideNumValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeur(ID1,ID2)
//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationNumTypeValeur(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position {Id, Id1 = mere::Id1, Id2 = mere::Id2, Num = mere::Num, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt,\
            Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un numero, un type et une valeur.
 */
template<class ValeurAttribut,class T,int IDM> class RelationNumTypeValeurEntity :
            public EntityAttributsID<Attributs<RelationAttribut,NumAttribut,TypeAttribut,ValeurAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationAttribut,NumAttribut,TypeAttribut,ValeurAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<RelationAttribut,NumAttribut,TypeAttribut,ValeurAttribut>,IDM>::EntityAttributsID;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setNum;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(RelationNumTypeValeurEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationNumTypeValeurEntity(idt id1, idt id2, int num = 0, idt type =0, idt id = 0)
        : EAID(id)
    {
        setId1(id1);
        setId2(id2);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationNumTypeValeurEntity(idt id1, idt id2, int num, idt type, T valeur, idt id = 0)
        : RelationNumTypeValeurEntity(id1, id2, num, type, id)
    {setValeur(valeur);}
};

template<class ValeurAttribut,class T,int IDM> RelationNumTypeValeurEntity<ValeurAttribut, T, IDM>::~RelationNumTypeValeurEntity() {}

template<int IDM> using RelationNumTypeValeurDoubleEntity = RelationNumTypeValeurEntity<ValeurDoubleAttribut,double,IDM>;
#define ENUM_RelationNumTypeValeurDouble(ID1,ID2) ENUM_RelationNumTypeValeur(ID1,ID2)

template<int IDM> using RelationNumTypeValeurIntEntity = RelationNumTypeValeurEntity<ValeurIntAttribut,int,IDM>;
#define ENUM_RelationNumTypeValeurInt(ID1,ID2) ENUM_RelationNumTypeValeur(ID1,ID2)

template<int IDM> using RelationNumTypeValeurVariantEntity = RelationNumTypeValeurEntity<ValeurVariantAttribut,const QVariant &,IDM>;
#define ENUM_RelationNumTypeValeurVariant(ID1,ID2) ENUM_RelationNumTypeValeur(ID1,ID2)

#endif // ENTITYRELATION
