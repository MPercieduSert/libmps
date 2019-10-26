/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITYDIVERS
#define ENTITYDIVERS

#include "Arbre.h"

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, Nom et type.
 */
template<class DateTimeAtt, int IDM> class DateTimeNomTypeEntityTemp
        : public EntityAttributsID<Attributs<DateTimeAtt,NomAttribut,TypeAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<DateTimeAtt,NomAttribut,TypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,DateTimeNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   DateTime = PositionEnum<DateTimeAtt,DateTimeNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,DateTimeNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,DateTimeNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<DateTimeAtt,NomAttribut,TypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setDateTime;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(DateTimeNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNomTypeEntityTemp(const QString & nom, idt type = 0, idt id = 0)
        : EAID(id)
    {
        setNom(nom);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    DateTimeNomTypeEntityTemp(const QDateTime & dateTime, const QString & nom, idt type, idt id = 0)
        : DateTimeNomTypeEntityTemp(nom, type, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, int IDM> DateTimeNomTypeEntityTemp<DateTimeAtt, IDM>::~DateTimeNomTypeEntityTemp() {}

template<int IDM> using DateTimeCurrentNomTypeEntity = DateTimeNomTypeEntityTemp<DateTimeCurrentAttribut,IDM>;
template<int IDM> using DateTimeValideNomTypeEntity = DateTimeNomTypeEntityTemp<DateTimeValideAttribut,IDM>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, Nc, Nom et type.
 */
template<class DateTimeAtt, int IDM> class DateTimeNcNomTypeEntityTemp
        : public EntityAttributsID<Attributs<DateTimeAtt,NcNomAttribut,TypeAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<DateTimeAtt,NcNomAttribut,TypeAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,DateTimeNcNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   DateTime = PositionEnum<DateTimeAtt,DateTimeNcNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   Nc = PositionEnum<NcAttribut,DateTimeNcNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,DateTimeNcNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,DateTimeNcNomTypeEntityTemp<DateTimeAtt,IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<DateTimeAtt,NcNomAttribut,TypeAttribut>,IDM>::EntityAttributsID;
    using EAID::setDateTime;
    using EAID::setNom;
    using EAID::setNc;
    using EAID::setType;
    BASE_ENTITY(DateTimeNcNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNcNomTypeEntityTemp(const QString & nom, idt type = 0, idt id = 0)
        : EAID(id)
    {
        setNom(nom);
        setType(type);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNcNomTypeEntityTemp(const QString & nc, const QString & nom, idt type = 0, idt id = 0)
        : DateTimeNcNomTypeEntityTemp(nom, type, id)
        {setNc(nc);}

    //! Constructeur à partir des valeurs attributs.
    DateTimeNcNomTypeEntityTemp(const QDateTime & dateTime, const QString & nc, const QString & nom, idt type, idt id = 0)
        : DateTimeNcNomTypeEntityTemp(nc, nom, type, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, int IDM> DateTimeNcNomTypeEntityTemp<DateTimeAtt, IDM>::~DateTimeNcNomTypeEntityTemp() {}

template<int IDM> using DateTimeCurrentNcNomTypeEntity = DateTimeNcNomTypeEntityTemp<DateTimeCurrentAttribut,IDM>;
template<int IDM> using DateTimeValideNcNomTypeEntity = DateTimeNcNomTypeEntityTemp<DateTimeValideAttribut,IDM>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime et Num.
 */
template<class DateTimeAtt, int IDM> class DateTimeNumEntityTemp : public EntityAttributsID<Attributs<DateTimeAtt,NumAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<DateTimeAtt,NumAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,DateTimeNumEntityTemp<DateTimeAtt,IDM>>::Position,
                   DateTime = PositionEnum<DateTimeAtt,DateTimeNumEntityTemp<DateTimeAtt,IDM>>::Position,
                   Num = PositionEnum<NumAttribut,DateTimeNumEntityTemp<DateTimeAtt,IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};
    using EntityAttributsID<Attributs<DateTimeAtt,NumAttribut>,IDM>::EntityAttributsID;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(DateTimeNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNumEntityTemp(int num, idt id)
        : EAID(id)
        {setNum(num);}

    //! Constructeur à partir des valeurs attributs.
    DateTimeNumEntityTemp(const QDateTime & dateTime, int num, idt id = 0)
        : DateTimeNumEntityTemp(num, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, int IDM> DateTimeNumEntityTemp<DateTimeAtt, IDM>::~DateTimeNumEntityTemp() {}

template<int IDM> using DateTimeCurrentNumEntity = DateTimeNumEntityTemp<DateTimeCurrentAttribut,IDM>;
template<int IDM> using DateTimeValideNumEntity = DateTimeNumEntityTemp<DateTimeValideAttribut,IDM>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime et Num.
 */
template<class DateTimeAtt, class ValeurAtt, class T, int IDM> class DateTimeTypeValeurEntityTemp
        : public EntityAttributsID<Attributs<DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,DateTimeTypeValeurEntityTemp<DateTimeAtt,ValeurAtt,T,IDM>>::Position,
                   DateTime = PositionEnum<DateTimeAtt,DateTimeTypeValeurEntityTemp<DateTimeAtt,ValeurAtt,T,IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,DateTimeTypeValeurEntityTemp<DateTimeAtt,ValeurAtt,T,IDM>>::Position,
                   Valeur = PositionEnum<ValeurAtt,DateTimeTypeValeurEntityTemp<DateTimeAtt,ValeurAtt,T,IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<DateTimeAtt,TypeAttribut,ValeurAtt>,IDM>::EntityAttributsID;
    using EAID::setDateTime;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(DateTimeTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeTypeValeurEntityTemp(idt type, T valeur, idt id = 0)
        : EAID(id)
    {
        setType(type);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    DateTimeTypeValeurEntityTemp(const QDateTime & dateTime, idt type, T valeur, idt id = 0)
        : DateTimeTypeValeurEntityTemp(type, valeur, id)
        {setDateTime(dateTime);}
};

template<class DateTimeAtt, class ValeurAtt, class T, int IDM> DateTimeTypeValeurEntityTemp<DateTimeAtt, ValeurAtt, T, IDM>
                                                                    ::~DateTimeTypeValeurEntityTemp() {}

template<int IDM> using DateTimeCurrentTypeValeurIntEntity = DateTimeTypeValeurEntityTemp<DateTimeCurrentAttribut,ValeurIntAttribut,int,IDM>;
template<int IDM> using DateTimeValideTypeValeurIntEntity = DateTimeTypeValeurEntityTemp<DateTimeValideAttribut,ValeurIntAttribut,int,IDM>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs parent, nc et nom.
 */
template<int IDM> class ArbreSimpleNcNomEntity : public EntityAttributsID<Attributs<ArbreSimpleAttribut,NcAttribut,NomAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<ArbreSimpleAttribut,NcAttribut,NomAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,ArbreSimpleNcNomEntity<IDM>>::Position,
                   Nc = PositionEnum<NcAttribut,ArbreSimpleNcNomEntity<IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,ArbreSimpleNcNomEntity<IDM>>::Position,
                   Parent = PositionEnum<ArbreSimpleAttribut,ArbreSimpleNcNomEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityAttributsID<Attributs<ArbreSimpleAttribut,NcAttribut,NomAttribut>,IDM>::EntityAttributsID;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    BASE_ENTITY(ArbreSimpleNcNomEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ArbreSimpleNcNomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    ArbreSimpleNcNomEntity(const QString & nc, const QString & nom, idt parent = 0, idt id = 0)
        : ArbreSimpleNcNomEntity(nom, id)
    {
        setNc(nc);
        setParent(parent);
    }
};

template<int IDM> ArbreSimpleNcNomEntity<IDM>::~ArbreSimpleNcNomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom et nc.
 */
template<int IDM> class NcNomEntity : public EntityAttributsID<NcNomAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<NcNomAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,NcNomEntity<IDM>>::Position,
                   Nc = PositionEnum<NcAttribut,NcNomEntity<IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,NcNomEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<NcNomAttribut,IDM>::EntityAttributsID;
    using EAID::setNc;
    using EAID::setNom;
    BASE_ENTITY(NcNomEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    NcNomEntity(const QString & nc, const QString & nom, idt id = 0)
        : NcNomEntity(nom, id)
        {setNc(nc);}
};

template<int IDM> NcNomEntity<IDM>::~NcNomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs nc, nom et type.
 */
template<int IDM> class NcNomTypeEntity : public EntityAttributsID<NcNomTypeAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<NcNomTypeAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,NcNomTypeEntity<IDM>>::Position,
                   Nc = PositionEnum<NcAttribut,NcNomTypeEntity<IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,NcNomTypeEntity<IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,NcNomTypeEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<NcNomTypeAttribut,IDM>::EntityAttributsID;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(NcNomTypeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomTypeEntity(const QString & nom)
        {setNom(nom);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomTypeEntity(const QString & nom, idt type)
        : NcNomTypeEntity(nom)
        {setType(type);}

    //! Constructeur à partir des valeurs attributs.
    NcNomTypeEntity(const QString & nc, const QString & nom, idt type, idt id = 0)
        : EAID(id)
    {
        setNc(nc);
        setNom(nom);
        setType(type);
    }
};

template<int IDM> NcNomTypeEntity<IDM>::~NcNomTypeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom.
 */
template<int IDM> class NomEntity : public EntityAttributsID<NomAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<NomAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,NomEntity<IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,NomEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<NomAttribut,IDM>::EntityAttributsID;
    using EAID::setNom;
    BASE_ENTITY(NomEntity)

    //! Constructeur à partir des valeurs attributs.
    NomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}
};

template<int IDM> NomEntity<IDM>::~NomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom et type.
 */
template<int IDM> class NomTypeEntity : public EntityAttributsID<NomTypeAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<NomTypeAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,NomTypeEntity<IDM>>::Position,
                   Nom = PositionEnum<NomAttribut,NomTypeEntity<IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,NomTypeEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<NomTypeAttribut,IDM>::EntityAttributsID;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(NomTypeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NomTypeEntity(const QString & nom)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    NomTypeEntity(const QString & nom, idt type, idt id = 0)
        : EAID(id)
    {
        setNom(nom);
        setType(type);
    }
};

template<int IDM> NomTypeEntity<IDM>::~NomTypeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut num.
 */
template<int IDM> class NumEntity : public EntityAttributsID<NumAttribut,IDM>
{
public:
    using EAID = EntityAttributsID<NumAttribut,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,NumEntity<IDM>>::Position,
                   Num = PositionEnum<NumAttribut,NumEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<NumAttribut,IDM>::EntityAttributsID;
    using EAID::setNum;
    BASE_ENTITY(NumEntity)

    //! Constructeur à partir des valeurs attributs.
    NumEntity(int num, idt id = 0)
        : EAID(id)
        {setNum(num);}
};

template<int IDM> NumEntity<IDM>::~NumEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut Texte et deux DateTime.
 */
template<int IDM> class TexteEntity : public EntityAttributsID<TexteAttributs,IDM>
{
public:
    using EAID = EntityAttributsID<TexteAttributs,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,TexteEntity<IDM>>::Position,
                   Creation = PositionEnum<CreationAttribut,TexteEntity<IDM>>::Position,
                   Modification = PositionEnum<ModificationAttribut,TexteEntity<IDM>>::Position,
                   Texte = PositionEnum<TexteAttribut,TexteEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<TexteAttributs,IDM>::EntityAttributsID;
    using EAID::setCreation;
    using EAID::setModification;
    using EAID::setTexte;
    BASE_ENTITY(TexteEntity)

    //! Constructeur à partir des valeurs attributs.
    TexteEntity(const QString & texte, idt id = 0)
        : EAID(id)
        {setTexte(texte);}

    //! Constructeur à partir des valeurs attributs.
    TexteEntity(const QDateTime & creation, const QDateTime & modification, const QString & texte, idt id = 0)
        : TexteEntity(texte,id)
    {
        setCreation(creation);
        setModification(modification);
    }
};

template<int IDM> TexteEntity<IDM>::~TexteEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant les attributs type et version.
 */
template<int IDM> class TypeVersionEntity : public EntityAttributsID<Attributs<TypeAttribut,VersionAttribut>,IDM>
{
public:
    using EAID = EntityAttributsID<Attributs<TypeAttribut,VersionAttribut>,IDM>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,TypeVersionEntity<IDM>>::Position,
                   Type = PositionEnum<TypeAttribut,TypeVersionEntity<IDM>>::Position,
                   Version = PositionEnum<VersionAttribut,TypeVersionEntity<IDM>>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityAttributsID<Attributs<TypeAttribut,VersionAttribut>,IDM>::EntityAttributsID;
    using EAID::setType;
    using EAID::setVersion;
    BASE_ENTITY(TypeVersionEntity)

    //! Constructeur à partir des valeurs attributs.
    TypeVersionEntity(idt type, int version, idt id = 0)
        : EAID(id)
    {
        setType(type);
        setVersion(version);
    }
};

template<int IDM> TypeVersionEntity<IDM>::~TypeVersionEntity() {}

using VersionBdd = DateTimeCurrentNumEntity<InfoEntity::entityBaseId::VersionBddId>;

#endif // ENTITYDIVERS
