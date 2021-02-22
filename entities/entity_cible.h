/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITY_CIBLE_H
#define ENTITY_CIBLE_H

#include "entity_relation.h"

//! \ingroup groupe_macro_entity
//! Macro implémentant une classe ayant une clé positive.
#define ID1_ENTITY(ENTITY,TYPE,IDT,ID1,id1) ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT) ENUM_##TYPE(ID1) ALIAS_CLE(id1,1)};

//! \ingroup groupe_macro_entity
//! Macro implémentant une classe ayant une clé négative.
#define ID1_ENTITY_NEG(ENTITY,TYPE,IDT,ID1,id1) ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT) ENUM_##TYPE(ID1) ALIAS_CLE_NEG(id1,1)};

namespace mps {
namespace entities_base {
//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_id_nc_nom_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1,  Nc = mere::Nc, Nom = mere::Nom, Nbr_Att = mere::Nbr_Att,\
    Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<entidt IDM , class Id_1_Att> class id_nc_nom_entity_temp :
        public entity_ids<IDM,Id_1_Att,nc_nom_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,nc_nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,nc_nom_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_nc;
    using eaid::set_nom;

    BASE_ENTITY(id_nc_nom_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_nc_nom_entity_temp(Id_1_Trans id_1, idt id)
        : eaid(id)
        {set_id_1(id_1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_nc_nom_entity_temp(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    id_nc_nom_entity_temp(Id_1_Trans id_1, const QString &nc, const QString &nom, idt id = 0)
        : id_nc_nom_entity_temp(nom, id) {
        set_id_1(id_1);
        set_nc(nc);
    }
};

template<entidt IDM , class Id_1_Att> id_nc_nom_entity_temp<IDM,Id_1_Att>::~id_nc_nom_entity_temp() {}

template<entidt IDM > using id_nc_nom_entity = id_nc_nom_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_nc_nom(ID1) ENUM_id_nc_nom_temp(ID1)

template<entidt IDM > using id_null_nc_nom_entity = id_nc_nom_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_nc_nom(ID1) ENUM_id_nc_nom_temp(ID1)

template<entidt IDM > using id_neg_nc_nom_entity = id_nc_nom_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_nc_nom(ID1) ENUM_id_nc_nom_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, un nom cours, un nom et un type.
#define ENUM_id_nc_nom_type_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1,  Nc = mere::Nc, Nom = mere::Nom, Type = mere::Type, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<entidt IDM , class Id_1_Att> class id_nc_nom_type_entity_temp :
        public entity_ids<IDM,Id_1_Att,nc_nom_type_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,nc_nom_type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,nc_nom_type_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_type;

    BASE_ENTITY(id_nc_nom_type_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_nc_nom_type_entity_temp(Id_1_Trans id_1, idt id)
        : eaid(id)
        {set_id_1(id_1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_nc_nom_type_entity_temp(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    id_nc_nom_type_entity_temp(Id_1_Trans id_1, const QString &nc, const QString &nom, idt type, idt id = 0)
        : id_nc_nom_type_entity_temp(nom, id) {
        set_id_1(id_1);
        set_nc(nc);
        set_type(type);
    }
};

template<entidt IDM , class Id_1_Att> id_nc_nom_type_entity_temp<IDM,Id_1_Att>::~id_nc_nom_type_entity_temp() {}

template<entidt IDM > using id_nc_nom_type_entity = id_nc_nom_type_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_nc_nom_type(ID1) ENUM_id_nc_nom_type_temp(ID1)

template<entidt IDM > using id_null_nc_nom_type_entity = id_nc_nom_type_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_nc_nom_type(ID1) ENUM_id_nc_nom_type_temp(ID1)

template<entidt IDM > using id_neg_nc_nom_type_entity = id_nc_nom_type_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_nc_nom_type(ID1) ENUM_id_nc_nom_type_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, un type et une version.
#define ENUM_id_type_version_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Type = mere::Type, Version = mere::Version, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, nom cours, nom et type.
 */
template<entidt IDM , class Id_1_Att> class id_type_version_entity_temp :
        public entity_ids<IDM,Id_1_Att,type_attribut,version_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,type_attribut,version_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Version = position_enum<version_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,type_attribut,version_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_type;
    using eaid::set_version;

    BASE_ENTITY(id_type_version_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_type_version_entity_temp(Id_1_Trans id_1, idt id)
        : eaid(id)
        {set_id_1(id_1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_type_version_entity_temp(Id_1_Trans id_1, idt type, idt id)
        : eaid(id_1, id)
        {set_type(type);}

    //! Constructeur à partir des valeurs attributs.
    id_type_version_entity_temp(Id_1_Trans id_1, idt type, int version, idt id = 0)
        : id_type_version_entity_temp(id_1, type, id)
        {set_version(version);}
};

template<entidt IDM , class Id_1_Att> id_type_version_entity_temp<IDM,Id_1_Att>::~id_type_version_entity_temp() {}

template<entidt IDM > using id_type_version_entity = id_type_version_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_type_version(ID1) ENUM_id_type_version_temp(ID1)

template<entidt IDM > using id_null_type_version_entity = id_type_version_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_type_version(ID1) ENUM_id_type_version_temp(ID1)

template<entidt IDM > using id_neg_type_version_entity = id_type_version_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_type_version(ID1) ENUM_id_type_version_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un nom cours et un nom.
#define ENUM_id_arbre_simple_nc_nom_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Parent = mere::Parent, Nc = mere::Nc, Nom = mere::Nom, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Ordre = mere::Ordre};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, nom cours, nom et parent.
 */
template<entidt IDM , class Id_1_Att> class id_arbre_simple_nc_nom_entity_temp :
        public entity_ids<IDM,Id_1_Att,parent_attribut,nc_nom_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,parent_attribut,nc_nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Parent = position_enum<parent_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Ordre = Nom};

    using entity_ids<IDM,Id_1_Att,parent_attribut,nc_nom_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_parent;
    BASE_ENTITY(id_arbre_simple_nc_nom_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_arbre_simple_nc_nom_entity_temp(Id_1_Trans id_1, idt id)
        : eaid(id)
        {set_id_1(id_1);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_arbre_simple_nc_nom_entity_temp(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    id_arbre_simple_nc_nom_entity_temp(Id_1_Trans id_1, const QString &nc, const QString &nom, idt parent = 0, idt id = 0)
        : id_arbre_simple_nc_nom_entity_temp(nom, id) {
        set_id_1(id_1);
        set_nc(nc);
        set_parent(parent);
    }
};

template<entidt IDM , class Id_1_Att> id_arbre_simple_nc_nom_entity_temp<IDM,Id_1_Att>::~id_arbre_simple_nc_nom_entity_temp() {}

template<entidt IDM > using id_arbre_simple_nc_nom_entity = id_arbre_simple_nc_nom_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_arbre_simple_nc_nom(ID1) ENUM_id_arbre_simple_nc_nom_temp(ID1)

template<entidt IDM > using id_null_arbre_simple_nc_nom_entity = id_arbre_simple_nc_nom_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_arbre_simple_nc_nom(ID1) ENUM_id_arbre_simple_nc_nom_temp(ID1)

template<entidt IDM > using id_neg_arbre_simple_nc_nom_entity = id_arbre_simple_nc_nom_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_arbre_simple_nc_nom(ID1) ENUM_id_arbre_simple_nc_nom_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, un parent, un Id_Prog, un nom cours et un nom.
#define ENUM_id_arbre_simple_id_prog_nc_nom_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Parent = mere::Parent, Id_Prog = mere::Id_Prog, Nc = mere::Nc,\
    Nom = mere::Nom, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Ordre = mere::Ordre};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, parent, Id_Prog, nom cours et nom.
 */
template<entidt IDM , class Id_1_Att> class id_arbre_simple_id_prog_nc_nom_entity_temp :
        public entity_ids<IDM,Id_1_Att,parent_attribut,id_prog_attribut,nc_nom_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,parent_attribut,id_prog_attribut,nc_nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Prog = position_enum<id_prog_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Parent = position_enum<parent_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Ordre = Nom};

    using entity_ids<IDM,Id_1_Att,parent_attribut,id_prog_attribut,nc_nom_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_prog;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_parent;
    BASE_ENTITY(id_arbre_simple_id_prog_nc_nom_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_arbre_simple_id_prog_nc_nom_entity_temp(idt Id_Prog, idt id)
        : eaid(id)
        {set_id_prog(Id_Prog);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_arbre_simple_id_prog_nc_nom_entity_temp(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    id_arbre_simple_id_prog_nc_nom_entity_temp(Id_1_Trans id_1, const QString &nc, const QString &nom,
                                       idt parent = 0, idt Id_Prog = 0, idt id = 0)
        : id_arbre_simple_id_prog_nc_nom_entity_temp(id, nom) {
        set_id_1(id_1);
        set_id_prog(Id_Prog);
        set_nc(nc);
        set_parent(parent);
    }
};

template<entidt IDM , class Id_1_Att> id_arbre_simple_id_prog_nc_nom_entity_temp<IDM,Id_1_Att>
    ::~id_arbre_simple_id_prog_nc_nom_entity_temp() {}

template<entidt IDM > using id_arbre_simple_id_prog_nc_nom_entity
    = id_arbre_simple_id_prog_nc_nom_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_arbre_simple_id_prog_nc_nom(ID1) ENUM_id_arbre_simple_id_prog_nc_nom_temp(ID1)

template<entidt IDM > using id_null_arbre_simple_id_prog_nc_nom_entity
    = id_arbre_simple_id_prog_nc_nom_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_arbre_simple_id_prog_nc_nom(ID1) ENUM_id_arbre_simple_id_prog_nc_nom_temp(ID1)

template<entidt IDM > using id_neg_arbre_simple_id_prog_nc_nom_entity
    = id_arbre_simple_id_prog_nc_nom_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_arbre_simple_id_prog_nc_nom(ID1) ENUM_id_arbre_simple_id_prog_nc_nom_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé étrangère, une creation, une modification et un numéro.
#define ENUM_id_creation_modif_num_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Creation = mere::Creation, Modification = mere::Modification,\
    Num = mere::Num, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, creation, modification, et num.
 */
template<entidt IDM , class Id_1_Att> class id_creation_modif_num_entity_temp :
        public entity_ids<IDM,Id_1_Att,creation_attribut,modification_attribut,num_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,creation_attribut,modification_attribut,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Creation = position_enum<creation_attribut,eaid>::Position,
                   Modification = position_enum<modification_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,creation_attribut,modification_attribut,num_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_creation;
    using eaid::set_modification;
    using eaid::set_num;
    BASE_ENTITY(id_creation_modif_num_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_creation_modif_num_entity_temp(Id_1_Trans id_1, int num, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    id_creation_modif_num_entity_temp(Id_1_Trans id_1, const QDateTime &creation, const QDateTime &modif, int num, idt id = 0)
        : id_creation_modif_num_entity_temp(id_1,num,id) {
        set_creation(creation);
        set_modification(modif);
    }
};

template<entidt IDM , class Id_1_Att> id_creation_modif_num_entity_temp<IDM,Id_1_Att>::~id_creation_modif_num_entity_temp() {}

template<entidt IDM > using id_creation_modif_num_entity = id_creation_modif_num_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_creation_modif_num(ID1) ENUM_id_creation_modif_num_temp(ID1)

template<entidt IDM > using id_null_creation_modif_num_entity = id_creation_modif_num_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_creation_modif_num(ID1) ENUM_id_creation_modif_num_temp(ID1)

template<entidt IDM > using id_neg_creation_modif_num_entity = id_creation_modif_num_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_creation_modif_num(ID1) ENUM_id_creation_modif_num_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_id_date_time_type_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Date_Time = mere::Datetime, Type = mere::Type,\
    Valeur::mere::Valeur, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time et Num.
 */
template<entidt IDM , class Date_Time_Att, class Valeur_Att, class Id_1_Att> class id_date_time_type_valeur_entity_temp
        : public entity_ids<IDM,Id_1_Att,Date_Time_Att,type_attribut,Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,Date_Time_Att,type_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,Date_Time_Att,type_attribut,Valeur_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_date_time;
    using eaid::set_type;
    using eaid::set_valeur;
    BASE_ENTITY(id_date_time_type_valeur_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    id_date_time_type_valeur_entity_temp(Id_1_Trans id_1, idt type = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    id_date_time_type_valeur_entity_temp(Id_1_Trans id_1, const QDateTime &date_time, idt type, val_trans valeur, idt id = 0)
        : id_date_time_type_valeur_entity_temp(id_1,type,id) {
        set_date_time(date_time);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Id_1_Att, class Date_Time_Att, class Valeur_Att>
    id_date_time_type_valeur_entity_temp<IDM,Id_1_Att,Date_Time_Att, Valeur_Att>::~id_date_time_type_valeur_entity_temp() {}

template<entidt IDM > using id_date_time_current_type_valeur_double_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_id_date_time_current_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_date_time_current_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_id_date_time_current_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_date_time_current_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_id_date_time_current_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_date_time_valide_type_valeur_double_entity
= id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_id_date_time_valide_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_date_time_valide_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_id_date_time_valide_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_date_time_valide_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_id_date_time_valide_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_current_type_valeur_double_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_current_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_current_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_current_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_current_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_current_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_valide_type_valeur_double_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_valide_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_valide_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_valide_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_null_date_time_valide_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_id_null_date_time_valide_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_current_type_valeur_double_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_current_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_current_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_current_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_current_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_current_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_valide_type_valeur_double_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_valide_type_valeur_double(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_valide_type_valeur_int_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_valide_type_valeur_int(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_date_time_valide_type_valeur_variant_entity
    = id_date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_date_time_valide_type_valeur_variant(ID1) ENUM_id_date_time_type_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible et une cible.
#define ENUM_cible_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible et Cible.
 */
template<entidt IDM , class Id_1_Att> class cible_entity_temp : public entity_ids<IDM,Id_1_Att,cibles_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    BASE_ENTITY(cible_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    cible_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
    }
};

template<entidt IDM , class Id_1_Att> cible_entity_temp<IDM,Id_1_Att>::~cible_entity_temp() {}

template<entidt IDM > using cible_entity = cible_entity_temp<IDM,id_1_attribut>;
#define ENUM_cible(ID1) ENUM_cible_temp(ID1)

template<entidt IDM > using cible_null_entity = cible_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_cible_null(ID1) ENUM_cible_temp(ID1)

template<entidt IDM > using cible_negentity = cible_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_cible_neg(ID1) ENUM_cible_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible et une ref.
#define ENUM_cible_ref_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible, Ref = mere::Ref, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible, cible et ref.
 */
template<entidt IDM , class Id_1_Att> class cible_ref_entity_temp : public entity_ids<IDM,Id_1_Att,cibles_attribut,ref_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut,ref_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Ref = position_enum<ref_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut,ref_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_ref;
    BASE_ENTITY(cible_ref_entity_temp)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    cible_ref_entity_temp(const QString &ref, idt id = 0)
        : eaid(id)
        {set_ref(ref);}

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    cible_ref_entity_temp(idt id_cible, int cible, const QString &ref, idt id = 0)
        : cible_ref_entity_temp(ref,id) {
        set_id_cible(id_cible);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_ref_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QString &ref, idt id = 0)
        : cible_ref_entity_temp(id_cible,cible,ref,id)
        {set_id_1(id_1);}
};

template<entidt IDM , class Id_1_Att> cible_ref_entity_temp<IDM,Id_1_Att>::~cible_ref_entity_temp() {}

template<entidt IDM > using cible_ref_entity = cible_ref_entity_temp<IDM,id_1_attribut>;
#define ENUM_cible_ref(ID1) ENUM_cible_ref_temp(ID1)

template<entidt IDM > using cible_null_ref_entity = cible_ref_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_cible_nullRef(ID1) ENUM_cible_ref_temp(ID1)

template<entidt IDM > using cible_neg_ref_entity = cible_ref_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_cible_neg_ref(ID1) ENUM_cible_ref_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, un numéro et un type.
#define ENUM_cible_num_type_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible, Num = mere::Num,\
    Type = mere::Type, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible, Cible, Num et Type.
 */
template<entidt IDM , class Id_1_Att> class cible_num_type_entity_temp :
        public entity_ids<IDM,Id_1_Att,cibles_attribut,num_attribut,type_attribut>
{
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut,num_attribut,type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut,num_attribut,type_attribut>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_num;
    using eaid::set_type;
    BASE_ENTITY(cible_num_type_entity_temp)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    cible_num_type_entity_temp(idt id_cible, int cible, int num, idt type) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_num_type_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num, idt type, idt id = 0)
        : cible_num_type_entity_temp(id_cible, cible, num, type) {
        set_id(id);
        set_id_1(id_1);
    }
};

template<entidt IDM , class Id_1_Att> cible_num_type_entity_temp<IDM,Id_1_Att>::~cible_num_type_entity_temp() {}

template<entidt IDM > using cible_num_type_entity = cible_num_type_entity_temp<IDM,id_1_attribut>;
#define ENUM_cible_num_type(ID1) ENUM_cible_num_type_temp(ID1)

template<entidt IDM > using cible_null_num_type_entity = cible_num_type_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_cible_null_num_type(ID1) ENUM_cible_num_type_temp(ID1)

template<entidt IDM > using cible_neg_num_type_entity = cible_num_type_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_cible_neg_num_type(ID1) ENUM_cible_num_type_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une Date_Time et Valeur.
#define ENUM_cible_num_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible,\
    Num = mere::Num, Valeur = mere::Valeur, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut Id_Cible, cible, num et valeur.
 */
template<entidt IDM , class Valeur_Att, class Id_1_Att> class cible_num_valeur_entity_temp :
        public entity_ids<IDM,Id_1_Att, cibles_attribut, num_attribut, Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att, cibles_attribut, num_attribut, Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att, cibles_attribut, num_attribut, Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_num;
    using eaid::set_valeur;
    BASE_ENTITY(cible_num_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_num_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num = 0) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_num_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num, val_trans valeur, idt id = 0)
        : cible_num_valeur_entity_temp(id_1,id_cible,cible,num) {
        set_id(id);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Valeur_Att, class Id_1_Att>
    cible_num_valeur_entity_temp<IDM,Valeur_Att, Id_1_Att>::~cible_num_valeur_entity_temp() {}

template<entidt IDM > using cible_num_valeur_double_entity
    = cible_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_attribut>;
#define ENUM_cible_num_valeur_double(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_num_valeur_int_entity
    = cible_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_attribut>;
#define ENUM_cible_num_valeur_int(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_num_valeur_variant_entity
    = cible_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_attribut>;
#define ENUM_cible_num_valeur_variant(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_num_valeur_double_entity
    = cible_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_cible_null_num_valeur_double(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_num_valeur_int_entity
    = cible_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_cible_null_num_valeur_int(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_num_valeur_variant_entity
    = cible_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_cible_null_num_valeur_variant(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_num_valeur_double_entity
    = cible_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_num_valeur_double(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_num_valeur_int_entity
    = cible_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_num_valeur_int(ID1) ENUM_cible_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_num_valeur_variant_entity
    = cible_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_num_valeur_variant(ID1) ENUM_cible_num_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible et une Date_Time.
#define ENUM_cible_date_time_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible,\
    Date_Time = mere::Date_Time, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible, Cible et Date_Time.
 */
template<entidt IDM , class Date_Time_Att, class Id_1_Att> class cible_date_time_entity_temp :
        public entity_ids<IDM,Id_1_Att,cibles_attribut,Date_Time_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut,Date_Time_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut,Date_Time_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    BASE_ENTITY(cible_date_time_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_date_time_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QDateTime &date_time, idt id = 0)
        : cible_date_time_entity_temp(id_1,id_cible,cible,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Id_1_Att> cible_date_time_entity_temp<IDM,Date_Time_Att, Id_1_Att>
    ::~cible_date_time_entity_temp() {}

template<entidt IDM > using cible_date_time_current_entity
    = cible_date_time_entity_temp<IDM,date_time_current_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current(ID1) ENUM_cible_date_time_temp(ID1)

template<entidt IDM > using cible_date_time_valide_entity
    = cible_date_time_entity_temp<IDM,date_time_valide_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide(ID1) ENUM_cible_date_time_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_entity
    = cible_date_time_entity_temp<IDM,date_time_current_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current(ID1) ENUM_cible_date_time_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_entity
    = cible_date_time_entity_temp<IDM,date_time_valide_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide(ID1) ENUM_cible_date_time_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_entity
    = cible_date_time_entity_temp<IDM,date_time_current_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current(ID1) ENUM_cible_date_time_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_entity
    = cible_date_time_entity_temp<IDM,date_time_valide_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide(ID1) ENUM_cible_date_time_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num et une Date_Time.
#define ENUM_cible_date_time_num_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible, \
    Date_Time = mere::Date_Time, Num = mere::Num, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible, Cible, Date_Time et Num.
 */
template<entidt IDM , class Date_Time_Att, class Id_1_Att> class cible_date_time_num_entity_temp :
        public entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    using eaid::set_num;
    BASE_ENTITY(cible_date_time_num_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_date_time_num_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QDateTime &date_time, int num, idt id = 0)
        : cible_date_time_num_entity_temp(id_1,id_cible,cible,num,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Id_1_Att> cible_date_time_num_entity_temp<IDM,Date_Time_Att, Id_1_Att>
    ::~cible_date_time_num_entity_temp() {}

template<entidt IDM > using cible_date_time_current_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_current_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num(ID1) ENUM_cible_date_time_num_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_valide_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num(ID1) ENUM_cible_date_time_num_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_current_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num(ID1) ENUM_cible_date_time_num_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_valide_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num(ID1) ENUM_cible_date_time_num_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_current_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num(ID1) ENUM_cible_date_time_num_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_entity
    = cible_date_time_num_entity_temp<IDM,date_time_valide_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num(ID1) ENUM_cible_date_time_num_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, num, type et une Date_Time.
#define ENUM_cible_date_time_num_type_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible, \
    Date_Time = mere::Date_Time, Num = mere::Num, Type= mere::Type, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible, Cible, Date_Time et Num.
 */
template<entidt IDM , class Date_Time_Att, class Id_1_Att> class cible_date_time_num_type_entity_temp :
            public entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut, type_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut, type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                       Id_1 = position_enum<Id_1_Att,eaid>::Position,
                       Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                       Cible = position_enum<cible_attribut,eaid>::Position,
                       Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                       Num = position_enum<num_attribut,eaid>::Position,
                       Type = position_enum<type_attribut,eaid>::Position,
                       Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cibles_attribut, Date_Time_Att, num_attribut, type_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_type;
    BASE_ENTITY(cible_date_time_num_type_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_date_time_num_type_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num = 0, idt type = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_type_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QDateTime &date_time,
                                   int num, idt type, idt id = 0)
        : cible_date_time_num_type_entity_temp(id_1,id_cible,cible,num,type,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Id_1_Att> cible_date_time_num_type_entity_temp<IDM,Date_Time_Att, Id_1_Att>
    ::~cible_date_time_num_type_entity_temp() {}

template<entidt IDM > using cible_date_time_current_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_current_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_type(ID1) ENUM_cible_date_time_num_type_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_valide_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_type(ID1) ENUM_cible_date_time_num_type_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_current_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_currentTypeNum(ID1) ENUM_cible_date_time_num_type_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_valide_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_type(ID1) ENUM_cible_date_time_num_type_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_current_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_currentTypeNum(ID1) ENUM_cible_date_time_num_type_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_type_entity
    = cible_date_time_num_type_entity_temp<IDM,date_time_valide_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_type(ID1) ENUM_cible_date_time_num_type_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une Date_Time et Valeur.
#define ENUM_cible_date_time_num_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible,\
    Date_Time = mere::Date_Time, Num = mere::Num, Valeur = mere::Valeur, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut une clé, Id_Cible, cible, datetime, num et valeur.
 */
template<entidt IDM , class Date_Time_Att, class Valeur_Att, class Id_1_Att> class cible_date_time_num_valeur_entity_temp :
        public entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using ValeurTrans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_valeur;
    BASE_ENTITY(cible_date_time_num_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_date_time_num_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num = 0) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num, ValeurTrans valeur, idt id = 0)
        : cible_date_time_num_valeur_entity_temp(id_1,id_cible,cible,num) {
        set_id(id);
        set_valeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QDateTime &date_time,
                                           int num, ValeurTrans valeur, idt id = 0)
        : cible_date_time_num_valeur_entity_temp(id_1,id_cible,cible,num,valeur,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Valeur_Att, class Id_1_Att>
    cible_date_time_num_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att, Id_1_Att>::~cible_date_time_num_valeur_entity_temp() {}

template<entidt IDM > using cible_date_time_current_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_current_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_current_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_valeur_double_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_valeur_double(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_valeur_int_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_valeur_int(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_valeur_variant_entity
    = cible_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_valeur_variant(ID1) ENUM_cible_date_time_num_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une Date_Time,
//! un type_valeur et Valeur.
#define ENUM_cible_date_time_num_tp_val_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Cible = mere::Cible,\
    Date_Time = mere::Date_Time, Num = mere::Num, Tp_Val = mere::Tp_Val, Valeur = mere::Valeur, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut une clé, Id_Cible, cible, datetime, num, typeVal et valeur.
 */
template<entidt IDM , class Date_Time_Att, class Valeur_Att, class Id_1_Att> class cible_date_time_num_tp_val_valeur_entity_temp
        : public entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, tp_val_attribut, Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using ValeurTrans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, tp_val_attribut, Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Tp_Val = position_enum<tp_val_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att, cibles_attribut, Date_Time_Att, num_attribut, tp_val_attribut, Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_tp_val;
    using eaid::set_valeur;
    BASE_ENTITY(cible_date_time_num_tp_val_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_date_time_num_tp_val_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num = 0) {
        set_id_1(id_1);
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_tp_val_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, int num, int tp_val,
                                                  ValeurTrans valeur, idt id = 0)
        : cible_date_time_num_tp_val_valeur_entity_temp(id_1,id_cible,cible,num) {
        set_id(id);
        set_tp_val(tp_val);
        set_valeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_date_time_num_tp_val_valeur_entity_temp(Id_1_Trans id_1, idt id_cible, int cible, const QDateTime &date_time,
                                          int num, int tp_val, ValeurTrans valeur, idt id = 0)
        : cible_date_time_num_tp_val_valeur_entity_temp(id_1,id_cible,cible,num,tp_val,valeur,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Valeur_Att, class Id_1_Att>
    cible_date_time_num_tp_val_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att, Id_1_Att>
    ::~cible_date_time_num_tp_val_valeur_entity_temp() {}

template<entidt IDM > using cible_date_time_current_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_current_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_current_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_cible_date_time_current_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_date_time_valide_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_attribut>;
#define ENUM_cible_date_time_valide_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_current_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_current_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_null_date_time_valide_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_cible_null_date_time_valide_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_current_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_current_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_tp_val_valeur_double_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_tp_val_valeur_double(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_tp_val_valeur_int_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_tp_val_valeur_int(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

template<entidt IDM > using cible_neg_date_time_valide_num_tp_val_valeur_variant_entity
    = cible_date_time_num_tp_val_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_cible_neg_date_time_valide_num_tp_val_valeur_variant(ID1) ENUM_cible_date_time_num_tp_val_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, num et Valeur.
#define ENUM_id_num_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Num = mere::Num, Valeur = mere::Valeur, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, num et valeur.
 */
template<entidt IDM , class Valeur_Att, class Id_1_Att> class id_num_valeur_entity_temp :
            public entity_ids<IDM,Id_1_Att,num_attribut,Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,num_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,num_attribut,Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_num;
    BASE_ENTITY(id_num_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_num_valeur_entity_temp(Id_1_Trans id_1, int num) {
        set_id_1(id_1);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    id_num_valeur_entity_temp(Id_1_Trans id_1, int num, val_trans valeur, idt id = 0)
        : id_num_valeur_entity_temp(id_1,num) {
        set_id(id);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Valeur_Att, class Id_1_Att> id_num_valeur_entity_temp<IDM,Valeur_Att, Id_1_Att>
    ::~id_num_valeur_entity_temp() {}

template<entidt IDM > using id_num_valeur_double_entity
    = id_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_attribut>;
#define ENUM_id_num_valeur_double(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_num_valeur_int_entity
    = id_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_attribut>;
#define ENUM_id_num_valeur_int(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_num_valeur_variant_entity
    = id_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_attribut>;
#define ENUM_id_num_valeur_variant(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_null_num_valeur_double_entity
    = id_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_valeur_double(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_null_num_valeur_int_entity
    = id_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_valeur_int(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_null_num_valeur_variant_entity
    = id_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_valeur_variant(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_neg_num_valeur_double_entity
    = id_num_valeur_entity_temp<IDM,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_valeur_double(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_neg_num_valeur_int_entity
    = id_num_valeur_entity_temp<IDM,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_valeur_int(ID1) ENUM_id_num_valeur_temp(ID1)

template<entidt IDM > using id_neg_num_valeur_variant_entity
    = id_num_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_valeur_variant(ID1) ENUM_id_num_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, type et Valeur.
#define ENUM_id_type_valeur_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Type = mere::Type, Valeur = mere::Valeur, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, type et valeur.
 */
template<entidt IDM , class Valeur_Att, class Id_1_Att> class id_type_valeur_entity_temp :
            public entity_ids<IDM,Id_1_Att,type_attribut,Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,type_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,type_attribut,Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_type;
    BASE_ENTITY(id_type_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_type_valeur_entity_temp(Id_1_Trans id_1, idt type) {
        set_id_1(id_1);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    id_type_valeur_entity_temp(Id_1_Trans id_1, idt type, val_trans valeur, idt id = 0)
        : id_type_valeur_entity_temp(id_1, type) {
        set_id(id);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Valeur_Att, class Id_1_Att> id_type_valeur_entity_temp<IDM,Valeur_Att, Id_1_Att>
    ::~id_type_valeur_entity_temp() {}

template<entidt IDM > using id_type_valeur_double_entity
    = id_type_valeur_entity_temp<IDM,valeur_double_attribut,id_1_attribut>;
#define ENUM_id_type_valeur_double(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_type_valeur_int_entity
    = id_type_valeur_entity_temp<IDM,valeur_int_attribut,id_1_attribut>;
#define ENUM_id_type_valeur_int(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_type_valeur_variant_entity
    = id_type_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_attribut>;
#define ENUM_id_type_valeur_variant(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_null_type_valeur_double_entity
    = id_type_valeur_entity_temp<IDM,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_id_null_type_valeur_double(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_null_type_valeur_int_entity
    = id_type_valeur_entity_temp<IDM,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_id_null_type_valeur_int(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_null_type_valeur_variant_entity
    = id_type_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_id_null_type_valeur_variant(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_type_valeur_double_entity
    = id_type_valeur_entity_temp<IDM,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_type_valeur_double(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_type_valeur_int_entity
    = id_type_valeur_entity_temp<IDM,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_type_valeur_int(ID1) ENUM_id_type_valeur_temp(ID1)

template<entidt IDM > using id_neg_type_valeur_variant_entity
    = id_type_valeur_entity_temp<IDM,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_type_valeur_variant(ID1) ENUM_id_type_valeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, num, type, valeur et version.
#define ENUM_id_num_type_versionValeur_temp(ID1) /*! \brief positions des attributs */ \
enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Type = mere::Type, Valeur = mere::Valeur, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé, num, type, version et valeur.
 */
template<entidt IDM , class Valeur_Att, class Id_1_Att> class id_num_type_versionvaleur_entity_temp :
                public entity_ids<IDM,Id_1_Att,num_attribut,type_attribut,version_attribut,Valeur_Att> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,num_attribut,type_attribut,version_attribut,Valeur_Att>;
        //! positions des attributs.
        enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                       Id_1 = position_enum<id_1_attribut,eaid>::Position,
                       Num = position_enum<num_attribut,eaid>::Position,
                       Type = position_enum<type_attribut,eaid>::Position,
                       Valeur = position_enum<Valeur_Att,eaid>::Position,
                       Version = position_enum<version_attribut,eaid>::Position,
                       Nbr_Att = eaid::Nbr_Att};

        using entity_ids<IDM,Id_1_Att,num_attribut,type_attribut,version_attribut,Valeur_Att>::entity_ids;
        using eaid::set_id;
        using eaid::set_id_1;
        using eaid::set_num;
        using eaid::set_type;
        using eaid::set_version;
        BASE_ENTITY(id_num_type_versionvaleur_entity_temp)

        //! Constructeur à partir d'un jeux de valeurs attributs unique.
        id_num_type_versionvaleur_entity_temp(Id_1_Trans id_1, int num, idt id =0)
            : eaid(id) {
            set_id_1(id_1);
            set_num(num);
        }

        //! Constructeur à partir des valeurs attributs.
        id_num_type_versionvaleur_entity_temp(Id_1_Trans id_1, int num, idt type, val_trans valeur, int version, idt id = 0)
            : id_num_type_versionvaleur_entity_temp(id_1, num, id) {
            set_type(type);
            set_valeur(valeur);
            set_version(version);
        }
    };

template<entidt IDM , class Valeur_Att, class Id_1_Att> id_num_type_versionvaleur_entity_temp<IDM,Valeur_Att, Id_1_Att>
    ::~id_num_type_versionvaleur_entity_temp() {}

template<entidt IDM > using id_num_type_versionvaleur_double_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_double_attribut,id_1_attribut>;
#define ENUM_id_num_type_versionvaleur_double(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_num_type_versionvaleur_int_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_int_attribut,id_1_attribut>;
#define ENUM_id_num_type_versionvaleur_int(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_num_type_versionvaleur_variant_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_variant_attribut,id_1_attribut>;
#define ENUM_id_num_type_versionvaleur_variant(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_null_num_type_versionvaleur_double_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_double_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_type_versionvaleur_double(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_null_num_type_versionvaleur_int_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_int_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_type_versionvaleur_int(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_null_num_type_versionvaleur_variant_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_variant_attribut,id_1_null_attribut>;
#define ENUM_id_null_num_type_versionvaleur_variant(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_neg_num_type_versionvaleur_double_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_double_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_type_versionvaleur_double(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_neg_num_type_versionvaleur_int_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_int_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_type_versionvaleur_int(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

template<entidt IDM > using id_neg_num_type_versionvaleur_variant_entity
    = id_num_type_versionvaleur_entity_temp<IDM,valeur_variant_attribut,id_1_neg_attribut>;
#define ENUM_id_neg_num_type_versionvaleur_variant(ID1) ENUM_id_num_type_versionValeur_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une cible et un code.
#define ENUM_id_cible_code_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Cible = mere::Cible, Code = mere::Code, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible et Cible.
 */
template<entidt IDM , class Id_1_Att> class id_cible_code_entity_temp
            : public entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                    Id_1 = position_enum<Id_1_Att,eaid>::Position,
                    Cible = position_enum<cible_attribut,eaid>::Position,
                    Code = position_enum<code_attribut,eaid>::Position,
                    Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_cible;
    using eaid::set_code;
    BASE_ENTITY(id_cible_code_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    id_cible_code_entity_temp(Id_1_Trans id_1, int cible, flag code = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_cible(cible);
        set_code(code);
    }
};

template<entidt IDM , class Id_1_Att> id_cible_code_entity_temp<IDM,Id_1_Att>::~id_cible_code_entity_temp() {}

template<entidt IDM > using id_cible_code_entity = id_cible_code_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_cible_code(ID1) ENUM_id_cible_code_temp(ID1)

template<entidt IDM > using id_null_cible_code_entity = id_cible_code_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_cible_code(ID1) ENUM_id_cible_code_temp(ID1)

template<entidt IDM > using id_neg_cible_code_entity = id_cible_code_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_cible_code(ID1) ENUM_id_cible_code_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une cible, un code et une ref.
#define ENUM_id_cible_code_ref_temp(ID1) /*! \brief positions des attributs */ \
enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Cible = mere::Cible, Code = mere::Code, Ref = mere::Ref, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible et Cible.
 */
template<entidt IDM , class Id_1_Att> class id_cible_code_ref_entity_temp
            : public entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut,ref_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut,ref_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                    Id_1 = position_enum<Id_1_Att,eaid>::Position,
                    Cible = position_enum<cible_attribut,eaid>::Position,
                    Code = position_enum<code_attribut,eaid>::Position,
                    Ref = position_enum<ref_attribut,eaid>::Position,
                    Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cible_attribut,code_attribut,ref_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_cible;
    using eaid::set_code;
    using eaid::set_ref;
    BASE_ENTITY(id_cible_code_ref_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_cible_code_ref_entity_temp(const QString &ref, idt id = 0)
        : eaid(id)
    {set_ref(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    id_cible_code_ref_entity_temp(Id_1_Trans id_1, int cible, const QString &ref, idt id = 0)
        : id_cible_code_ref_entity_temp(ref,id) {
        set_id_1(id_1);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    id_cible_code_ref_entity_temp(Id_1_Trans id_1, int cible, flag code, const QString &ref = QString(), idt id = 0)
        : id_cible_code_ref_entity_temp(id_1,cible,ref,id)
    {set_code(code);}
};

template<entidt IDM , class Id_1_Att> id_cible_code_ref_entity_temp<IDM,Id_1_Att>::~id_cible_code_ref_entity_temp() {}

template<entidt IDM > using id_cible_code_ref_entity = id_cible_code_ref_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_cible_code_ref(ID1) ENUM_id_cible_code_ref_temp(ID1)

template<entidt IDM > using id_null_cible_code_ref_entity = id_cible_code_ref_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_cible_code_ref(ID1) ENUM_id_cible_code_ref_temp(ID1)

template<entidt IDM > using id_neg_cible_code_ref_entity = id_cible_code_ref_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_cible_code_ref(ID1) ENUM_id_cible_code_ref_temp(ID1)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une cible et un num.
#define ENUM_id_cible_num_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Cible = mere::Cible, Num = mere::Num, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut clé étrangère, id_cible et Cible.
 */
template<entidt IDM , class Id_1_Att> class id_cible_num_entity_temp
            : public entity_ids<IDM,Id_1_Att,cible_attribut,num_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cible_attribut,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cible_attribut,num_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_cible;
    using eaid::set_num;
    BASE_ENTITY(id_cible_num_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    id_cible_num_entity_temp(Id_1_Trans id_1, int cible, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_cible(cible);
        set_num(num);
    }
};

template<entidt IDM , class Id_1_Att> id_cible_num_entity_temp<IDM,Id_1_Att>::~id_cible_num_entity_temp() {}

template<entidt IDM > using id_cible_num_entity = id_cible_num_entity_temp<IDM,id_1_attribut>;
#define ENUM_id_cible_num(ID1) ENUM_id_cible_num_temp(ID1)

template<entidt IDM > using id_null_cible_num_entity = id_cible_num_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_id_null_cible_num(ID1) ENUM_id_cible_num_temp(ID1)

template<entidt IDM > using id_neg_cible_num_entity = id_cible_num_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_id_neg_cible_num(ID1) ENUM_id_cible_num_temp(ID1)

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut id_cible, cible et code.
 */
template<entidt IDM > class cible_simple_code_entity : public entity_ids<IDM,id_cible_attribut,cible_attribut,code_attribut> {
public:
    using eaid = entity_ids<IDM,id_cible_attribut,cible_attribut,code_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                    Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                    Cible = position_enum<cible_attribut,eaid>::Position,
                    Code = position_enum<code_attribut,eaid>::Position,
                    Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,id_cible_attribut,cible_attribut,code_attribut>::entity_ids;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_code;
    BASE_ENTITY(cible_simple_code_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_simple_code_entity(idt id_cible, int cible) {
        set_id_cible(id_cible);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_simple_code_entity(idt id_cible, int cible, flag code, idt id = 0)
        : eaid(id) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_code(code);
    }
};

template<entidt IDM > cible_simple_code_entity<IDM>::~cible_simple_code_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut id_cible, cible et num.
 */
template<entidt IDM > class cible_simple_num_entity : public entity_ids<IDM,id_cible_attribut,cible_attribut,num_attribut> {
public:
    using eaid = entity_ids<IDM,id_cible_attribut,cible_attribut,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,id_cible_attribut,cible_attribut,num_attribut>::entity_ids;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_num;
    BASE_ENTITY(cible_simple_num_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_simple_num_entity(idt id_cible, int cible) {
        set_id_cible(id_cible);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_simple_num_entity(idt id_cible, int cible, int num, idt id = 0)
        : eaid(id) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }
};

template<entidt IDM > cible_simple_num_entity<IDM>::~cible_simple_num_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut id_cible, Id_Etat, cible, etat, num et date_time.
 */
template<entidt IDM > class historique_entity
            : public entity_ids<IDM,id_cible_attribut,cible_attribut,id_etat_attribut,etat_attribut,
            num_attribut,date_time_valide_attribut> {
public:
    using eaid = entity_ids<IDM,id_cible_attribut,cible_attribut,id_etat_attribut,etat_attribut,
        num_attribut,date_time_valide_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Id_Etat = position_enum<id_etat_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<date_time_valide_attribut,eaid>::Position,
                   Etat = position_enum<etat_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,id_cible_attribut,cible_attribut,id_etat_attribut,etat_attribut,
    num_attribut,date_time_valide_attribut>::entity_ids;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_id_etat;
    using eaid::set_etat;
    using eaid::set_num;
    using eaid::set_date_time;
    BASE_ENTITY(historique_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    historique_entity(idt id_cible, int cible, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    historique_entity(idt id_cible, idt id_etat, int cible, const QDateTime &date, int etat, int num = 0, idt id = 0)
        :historique_entity(id_cible,cible,num,id) {
        set_id_etat(id_etat);
        set_etat(etat);
        set_date_time(date);
    }
};

template<entidt IDM > historique_entity<IDM>::~historique_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut Id_Cible, cible, type et valeur.
 */
template<entidt IDM , class Valeur_Att> class cible_simple_type_valeur_entity_temp :
                public entity_ids<IDM,cibles_attribut, type_attribut, Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,cibles_attribut, type_attribut, Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,cibles_attribut, type_attribut, Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_type;
    using eaid::set_valeur;
    BASE_ENTITY(cible_simple_type_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_simple_type_valeur_entity_temp(idt id_cible, int cible, idt type = 0) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_simple_type_valeur_entity_temp(idt id_cible, int cible, idt type, val_trans valeur, idt id = 0)
        : cible_simple_type_valeur_entity_temp(id_cible,cible,type,valeur)
        {set_id(id);}
};

template<entidt IDM , class Valeur_Att> cible_simple_type_valeur_entity_temp<IDM, Valeur_Att>
    ::~cible_simple_type_valeur_entity_temp() {}

template<entidt IDM > using cible_simple_type_valeur_double_entity
    = cible_simple_type_valeur_entity_temp<IDM,valeur_double_attribut>;
template<entidt IDM > using cible_simple_type_valeur_int_entity
    = cible_simple_type_valeur_entity_temp<IDM,valeur_int_attribut>;
template<entidt IDM > using cible_simple_type_valeur_variant_entity
    = cible_simple_type_valeur_entity_temp<IDM,valeur_variant_attribut>;


/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut Id_Cible, cible, datetime, num, type et valeur.
 */
template<entidt IDM , class Date_Time_Att, class Valeur_Att> class cible_simple_date_time_num_type_valeur_entity_temp :
            public entity_ids<IDM,cibles_attribut, Date_Time_Att, num_attribut, type_attribut, Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,cibles_attribut, Date_Time_Att, num_attribut, type_attribut, Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,cibles_attribut, Date_Time_Att, num_attribut, type_attribut, Valeur_Att>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_type;
    using eaid::set_valeur;
    BASE_ENTITY(cible_simple_date_time_num_type_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    cible_simple_date_time_num_type_valeur_entity_temp(idt id_cible, int cible, int num = 0, idt type =0) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_simple_date_time_num_type_valeur_entity_temp(idt id_cible, int cible, int num, idt type, val_trans valeur, idt id = 0)
        : cible_simple_date_time_num_type_valeur_entity_temp(id_cible,cible,num,type) {
        set_id(id);
        set_valeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    cible_simple_date_time_num_type_valeur_entity_temp(idt id_cible, int cible, const QDateTime &date_time,
                                               int num, idt type, val_trans valeur, idt id = 0)
        : cible_simple_date_time_num_type_valeur_entity_temp(id_cible,cible,num,type,valeur,id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Valeur_Att>
        cible_simple_date_time_num_type_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att>
        ::~cible_simple_date_time_num_type_valeur_entity_temp() {}

template<entidt IDM > using cible_simple_date_Time_current_num_type_valeur_double_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;

template<entidt IDM > using cible_simple_date_Time_current_num_type_valeur_int_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;

template<entidt IDM > using cible_simple_date_Time_current_num_type_valeur_variant_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;

template<entidt IDM > using cible_simple_date_time_valide_num_type_valeur_double_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;

template<entidt IDM > using cible_simple_date_time_valide_num_type_valeur_int_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;

template<entidt IDM > using cible_simple_date_time_valide_num_type_valeur_variant_entity
    = cible_simple_date_time_num_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une clé, une clé de cible, une cible, une Date_Time et Valeur.
#define ENUM_utilisation_temp(ID1) /*! \brief positions des attributs */ \
    enum position:post {Id = mere::Id, Id_1 = mere::Id_1, Id_Cible = mere::Id_Cible, Id_Etat = mere::Id_Etat, \
    Cible = mere::Cible, Date_Time = mere::Date_Time, Etat = mere::Etat, Num = mere::Num, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1};

/*! \ingroup groupe_base_entity
 *\brief Représentation de l'entité Utilisation.
 */
template<entidt IDM , class Id_1_Att> class utilisation_entity_temp :
            public entity_ids<IDM,Id_1_Att,cible_date_time_num_attribut,etats_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,cible_date_time_num_attribut,etats_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Id_Etat = position_enum<id_etat_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Date_Time = position_enum<date_time_valide_attribut,eaid>::Position,
                   Etat = position_enum<etat_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,cible_date_time_num_attribut,etats_attribut>::entity_ids;
    using eaid::set_id;
    using eaid::set_id_1;
    using eaid::set_id_cible;
    using eaid::set_cible;
    using eaid::set_id_etat;
    using eaid::set_etat;
    using eaid::set_num;
    using eaid::set_date_time;
    BASE_ENTITY(utilisation_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    utilisation_entity_temp(idt id_cible, int cible, int num = 0) {
        set_id_cible(id_cible);
        set_cible(cible);
        set_num(num);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    utilisation_entity_temp(idt id_cible, idt id_etat, int cible, int etat, int num = 0, idt id = 0)
        : utilisation_entity_temp(id_cible, cible, num) {
        set_id(id);
        set_id_etat(id_etat);
        set_etat(etat);
    }

    //! Constructeur à partir des valeurs attributs.
    utilisation_entity_temp(Id_1_Trans id_1, idt id_cible, idt id_etat, int cible,
                          const QDateTime &date_time, int etat, int num, idt id = 0)
        : utilisation_entity_temp(id_cible,cible,id_etat,etat,num,id) {
        set_id_1(id_1);
        set_date_time(date_time);
    }
};

template<entidt IDM , class Id_1_Att> utilisation_entity_temp<IDM,Id_1_Att>::~utilisation_entity_temp() {}

template<entidt IDM > using utilisation_entity = utilisation_entity_temp<IDM,id_1_attribut>;
#define ENUM_utilisation(ID1) ENUM_utilisation_temp(ID1)

template<entidt IDM > using utilisation_null_entity = utilisation_entity_temp<IDM,id_1_null_attribut>;
#define ENUM_utilisation_null(ID1) ENUM_utilisation_temp(ID1)

template<entidt IDM > using utilisation_neg_entity = utilisation_entity_temp<IDM,id_1_neg_attribut>;
#define ENUM_utilisation_neg(ID1) ENUM_utilisation_temp(ID1)
}}
#endif // ENTITY_CIBLE_H
