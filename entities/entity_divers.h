/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITY_DIVERS
#define ENTITY_DIVERS

#include "entity.h"

/*! \defgroup groupe_base_entity Entités de base
 *\ingroup groupe_entity
 *\brief Ensemble de classes représentant les entités de base.
 */
namespace mps {
/*! \ingroup groupe_base_entity
 *\brief Espace de nom de entités de base.
 */
namespace entities_base {
using namespace entities;
using namespace attribut;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time, Nom et type.
 */
class arbre : public entity_id<info_entity::arbre,arbre_attribut> {
public:
    using eaid = entity_id<info_entity::arbre,arbre_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Feuille = position_enum<feuille_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Parent = position_enum<parent_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using eaid::entity_id;
    BASE_ENTITY(arbre)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    arbre(int num, idt parent, idt id = 0)
        : eaid(id) {
        set_num(num);
        set_parent(parent);
    }

    //! Constructeur à partir des valeurs attributs.
    arbre(bool feuille, int num, idt parent, idt id = 0)
        : arbre(num,parent,id)
        {set_feuille(feuille);}
};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time, Nom et type.
 */
template<entidt IDM , class Date_Time_Att> class date_time_nom_type_entity_temp
        : public entity_ids<IDM,Date_Time_Att,nom_attribut,type_attribut> {
public:
    using eaid = entity_ids<IDM,Date_Time_Att,nom_attribut,type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Date_Time_Att,nom_attribut,type_attribut>::entity_ids;
    using eaid::set_date_time;
    using eaid::set_nom;
    using eaid::set_type;
    BASE_ENTITY(date_time_nom_type_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    date_time_nom_type_entity_temp(const QString &nom, idt type = 0, idt id = 0)
        : eaid(id) {
        set_nom(nom);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    date_time_nom_type_entity_temp(const QDateTime &date_time, const QString &nom, idt type, idt id = 0)
        : date_time_nom_type_entity_temp(nom, type, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att> date_time_nom_type_entity_temp<IDM,Date_Time_Att>::~date_time_nom_type_entity_temp() {}

template<entidt IDM > using date_time_current_nom_type_entity = date_time_nom_type_entity_temp<IDM,date_time_current_attribut>;
template<entidt IDM > using date_time_valide_nom_type_entity = date_time_nom_type_entity_temp<IDM,date_time_valide_attribut>;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time, Nc, Nom et type.
 */
template<entidt IDM , class Date_Time_Att> class date_time_nc_nom_type_entity_temp
        : public entity_ids<IDM,Date_Time_Att,nc_nom_attribut,type_attribut> {
public:
    using eaid = entity_ids<IDM,Date_Time_Att,nc_nom_attribut,type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Date_Time_Att,nc_nom_attribut,type_attribut>::entity_ids;
    using eaid::set_date_time;
    using eaid::set_nom;
    using eaid::set_nc;
    using eaid::set_type;
    BASE_ENTITY(date_time_nc_nom_type_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    date_time_nc_nom_type_entity_temp(const QString &nom, idt type = 0, idt id = 0)
        : eaid(id) {
        set_nom(nom);
        set_type(type);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    date_time_nc_nom_type_entity_temp(const QString &nc, const QString &nom, idt type = 0, idt id = 0)
        : date_time_nc_nom_type_entity_temp(nom, type, id)
        {set_nc(nc);}

    //! Constructeur à partir des valeurs attributs.
    date_time_nc_nom_type_entity_temp(const QDateTime &date_time, const QString &nc, const QString &nom, idt type, idt id = 0)
        : date_time_nc_nom_type_entity_temp(nc, nom, type, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att> date_time_nc_nom_type_entity_temp<IDM,Date_Time_Att>
    ::~date_time_nc_nom_type_entity_temp() {}

template<entidt IDM > using date_time_current_nc_nom_type_entity
    = date_time_nc_nom_type_entity_temp<IDM,date_time_current_attribut>;
template<entidt IDM > using date_time_valide_nc_nom_type_entity
    = date_time_nc_nom_type_entity_temp<IDM,date_time_valide_attribut>;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time et Num.
 */
template<entidt IDM , class Date_Time_Att> class date_time_num_entity_temp : public entity_ids<IDM,Date_Time_Att,num_attribut> {
public:
    using eaid = entity_ids<IDM,Date_Time_Att,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};
    using entity_ids<IDM,Date_Time_Att,num_attribut>::entity_ids;
    using eaid::set_date_time;
    using eaid::set_num;
    BASE_ENTITY(date_time_num_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    date_time_num_entity_temp(int num, idt id)
        : eaid(id)
        {set_num(num);}

    //! Constructeur à partir des valeurs attributs.
    date_time_num_entity_temp(const QDateTime &date_time, int num, idt id = 0)
        : date_time_num_entity_temp(num, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att> date_time_num_entity_temp<IDM,Date_Time_Att>::~date_time_num_entity_temp() {}

template<entidt IDM > using date_time_current_num_entity = date_time_num_entity_temp<IDM,date_time_current_attribut>;
template<entidt IDM > using date_time_valide_num_entity = date_time_num_entity_temp<IDM,date_time_valide_attribut>;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut date_time, type et valeur.
 */
template<entidt IDM , class Date_Time_Att, class Valeur_Att> class date_time_type_valeur_entity_temp
        : public entity_ids<IDM,Date_Time_Att,type_attribut,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,Date_Time_Att,type_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Date_Time_Att,type_attribut,Valeur_Att>::entity_ids;
    using eaid::set_date_time;
    using eaid::set_type;
    using eaid::set_valeur;
    BASE_ENTITY(date_time_type_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    date_time_type_valeur_entity_temp(idt type, val_trans valeur, idt id = 0)
        : eaid(id) {
        set_type(type);
        set_valeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    date_time_type_valeur_entity_temp(const QDateTime &date_time, idt type, val_trans valeur, idt id = 0)
        : date_time_type_valeur_entity_temp(type, valeur, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att, class Valeur_Att> date_time_type_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att>
                                                                    ::~date_time_type_valeur_entity_temp() {}

template<entidt IDM > using date_time_current_type_valeur_double_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;
template<entidt IDM > using date_time_current_type_valeur_int_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;
template<entidt IDM > using date_time_current_type_valeur_variant_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;
template<entidt IDM > using date_time_valide_type_valeur_double_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;
template<entidt IDM > using date_time_valide_type_valeur_int_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;
template<entidt IDM > using date_time_valide_type_valeur_variant_entity
    = date_time_type_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant des attributs parent, nc et nom.
 */
template<entidt IDM > class arbre_simple_nc_nom_entity : public entity_ids<IDM,parent_attribut,nc_attribut,nom_attribut> {
public:
    using eaid = entity_ids<IDM,parent_attribut,nc_attribut,nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Parent = position_enum<parent_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Ordre = Nom};

    using entity_ids<IDM,parent_attribut,nc_attribut,nom_attribut>::entity_ids;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_parent;
    BASE_ENTITY(arbre_simple_nc_nom_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    arbre_simple_nc_nom_entity(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    arbre_simple_nc_nom_entity(const QString &nc, const QString &nom, idt parent = 0, idt id = 0)
        : arbre_simple_nc_nom_entity(nom, id) {
        set_nc(nc);
        set_parent(parent);
    }
};

template<entidt IDM > arbre_simple_nc_nom_entity<IDM>::~arbre_simple_nc_nom_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant des attributs parent, nc, nom et ref.
 */
template<entidt IDM > class arbre_simple_nc_nom_ref_entity : public entity_ids<IDM,parent_attribut,nc_nom_ref_attribut> {
public:
    using eaid = entity_ids<IDM,parent_attribut,nc_nom_ref_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Parent = position_enum<parent_attribut,eaid>::Position,
                   Ref = position_enum<ref_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Ordre = Nom};

    using entity_ids<IDM,parent_attribut,nc_nom_ref_attribut>::entity_ids;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_parent;
    using eaid::set_ref;
    BASE_ENTITY(arbre_simple_nc_nom_ref_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    arbre_simple_nc_nom_ref_entity(const QString &ref, idt id = 0)
        : eaid(id)
    {set_ref(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    arbre_simple_nc_nom_ref_entity(const QString &nom, idt parent, idt id = 0)
        : eaid(id) {
        set_nom(nom);
        set_parent(parent);
    }

    //! Constructeur à partir des valeurs attributs.
    arbre_simple_nc_nom_ref_entity(const QString &nc, const QString &nom, idt parent = 0,
                                   const QString &ref = QString(), idt id = 0)
        : arbre_simple_nc_nom_ref_entity(nom, parent, id) {
        set_nc(nc);
        set_parent(parent);
        set_ref(ref);
    }
};

template<entidt IDM > arbre_simple_nc_nom_ref_entity<IDM>::~arbre_simple_nc_nom_ref_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut nom et nc.
 */
template<entidt IDM > class nc_nom_entity : public entity_id<IDM,nc_nom_attribut> {
public:
    using eaid = entity_id<IDM,nc_nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nc_nom_attribut>::entity_id;
    using eaid::set_nc;
    using eaid::set_nom;
    BASE_ENTITY(nc_nom_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_entity(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    nc_nom_entity(const QString &nc, const QString &nom, idt id = 0)
        : nc_nom_entity(nom, id)
        {set_nc(nc);}
};

template<entidt IDM > nc_nom_entity<IDM>::~nc_nom_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant des attributs nc, nom et type.
 */
template<entidt IDM > class nc_nom_type_entity : public entity_id<IDM,nc_nom_type_attribut> {
public:
    using eaid = entity_id<IDM,nc_nom_type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nc_nom_type_attribut>::entity_id;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_type;
    BASE_ENTITY(nc_nom_type_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_type_entity(const QString &nom)
        {set_nom(nom);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_type_entity(const QString &nom, idt type)
        : nc_nom_type_entity(nom)
        {set_type(type);}

    //! Constructeur à partir des valeurs attributs.
    nc_nom_type_entity(const QString &nc, const QString &nom, idt type, idt id = 0)
        : eaid(id) {
        set_nc(nc);
        set_nom(nom);
        set_type(type);
    }
};

template<entidt IDM > nc_nom_type_entity<IDM>::~nc_nom_type_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant des attributs nc, nom et ref.
 */
template<entidt IDM > class nc_nom_ref_entity : public entity_id<IDM,nc_nom_ref_attribut> {
public:
    using eaid = entity_id<IDM,nc_nom_ref_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Ref = position_enum<ref_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nc_nom_ref_attribut>::entity_id;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_ref;
    BASE_ENTITY(nc_nom_ref_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_ref_entity(const QString &ref)
        {set_ref(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_ref_entity(const QString &nom, idt id = 0)
        : eaid (id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    nc_nom_ref_entity(const QString &nc, const QString &nom, const QString &ref, idt id = 0)
        : nc_nom_ref_entity(nom,id) {
        set_nc(nc);
        set_ref(ref);
    }
};

template<entidt IDM > nc_nom_ref_entity<IDM>::~nc_nom_ref_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant des attributs nc, nom, ref et type.
 */
template<entidt IDM > class nc_nom_ref_type_entity : public entity_id<IDM,nc_nom_ref_type_attribut> {
public:
    using eaid = entity_id<IDM,nc_nom_ref_type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nc = position_enum<nc_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Ref = position_enum<ref_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nc_nom_ref_type_attribut>::entity_id;
    using eaid::set_nc;
    using eaid::set_nom;
    using eaid::set_ref;
    using eaid::set_type;
    BASE_ENTITY(nc_nom_ref_type_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_ref_type_entity(const QString &ref)
        {set_ref(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nc_nom_ref_type_entity(const QString &nom, idt type, idt id = 0)
        : eaid (id) {
        set_nom(nom);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    nc_nom_ref_type_entity(const QString &nc, const QString &nom, const QString &ref, idt type, idt id = 0)
        : nc_nom_ref_type_entity(nom,type,id) {
        set_nc(nc);
        set_ref(ref);
    }
};

template<entidt IDM > nc_nom_ref_type_entity<IDM>::~nc_nom_ref_type_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut nom.
 */
template<entidt IDM > class nom_entity : public entity_id<IDM,nom_attribut> {
public:
    using eaid = entity_id<IDM,nom_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nom_attribut>::entity_id;
    using eaid::set_nom;
    BASE_ENTITY(nom_entity)

    //! Constructeur à partir des valeurs attributs.
    nom_entity(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}
};

template<entidt IDM > nom_entity<IDM>::~nom_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut nom et type.
 */
template<entidt IDM > class nom_type_entity : public entity_id<IDM,nom_type_attribut> {
public:
    using eaid = entity_id<IDM,nom_type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Nom = position_enum<nom_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,nom_type_attribut>::entity_id;
    using eaid::set_nom;
    using eaid::set_type;
    BASE_ENTITY(nom_type_entity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    nom_type_entity(const QString &nom)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    nom_type_entity(const QString &nom, idt type, idt id = 0)
        : eaid(id) {
        set_nom(nom);
        set_type(type);
    }
};

template<entidt IDM > nom_type_entity<IDM>::~nom_type_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut num.
 */
template<entidt IDM > class num_entity : public entity_id<IDM,num_attribut> {
public:
    using eaid = entity_id<IDM,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,num_attribut>::entity_id;
    using eaid::set_num;
    BASE_ENTITY(num_entity)

    //! Constructeur à partir des valeurs attributs.
    num_entity(int num, idt id = 0)
        : eaid(id)
        {set_num(num);}
};

template<entidt IDM > num_entity<IDM>::~num_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut Texte et deux Date_Time.
 */
template<entidt IDM > class texte_entity : public entity_id<IDM,textes_attribut> {
public:
    using eaid = entity_id<IDM,textes_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Creation = position_enum<creation_attribut,eaid>::Position,
                   Modification = position_enum<modification_attribut,eaid>::Position,
                   Texte = position_enum<texte_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,textes_attribut>::entity_id;
    using eaid::set_creation;
    using eaid::set_modification;
    using eaid::set_texte;
    BASE_ENTITY(texte_entity)

    //! Constructeur à partir des valeurs attributs.
    texte_entity(const QString &texte, idt id = 0)
        : eaid(id)
        {set_texte(texte);}

    //! Constructeur à partir des valeurs attributs.
    texte_entity(const QDateTime &creation, const QDateTime &modification, const QString &texte, idt id = 0)
        : texte_entity(texte,id) {
        set_creation(creation);
        set_modification(modification);
    }
};

template<entidt IDM > texte_entity<IDM>::~texte_entity() {}

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant un attribut type, valeur et version.
 */
template<entidt IDM , class Valeur_Att> class type_valeur_version_entity_temp
        : public entity_ids<IDM,type_attribut,Valeur_Att,version_attribut> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,type_attribut,Valeur_Att,version_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Version = position_enum<version_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,type_attribut,Valeur_Att,version_attribut>::entity_ids;
    using eaid::set_type;
    using eaid::set_valeur;
    using eaid::set_version;
    BASE_ENTITY(type_valeur_version_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    type_valeur_version_entity_temp(idt type, val_trans valeur, int version, idt id = 0)
        : eaid(id) {
        set_type(type);
        set_valeur(valeur);
        set_version(version);
    }
};

template<entidt IDM , class Valeur_Att> type_valeur_version_entity_temp<IDM, Valeur_Att>
                                                                    ::~type_valeur_version_entity_temp() {}

template<entidt IDM > using type_valeur_double_version_entity = type_valeur_version_entity_temp<IDM,valeur_double_attribut>;
template<entidt IDM > using type_valeur_int_version_entity = type_valeur_version_entity_temp<IDM,valeur_int_attribut>;
template<entidt IDM > using type_valeur_variant_version_entity = type_valeur_version_entity_temp<IDM,valeur_variant_attribut>;

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant les attributs type et version.
 */
template<entidt IDM > class type_version_entity : public entity_ids<IDM,type_attribut,version_attribut> {
public:
    using eaid = entity_ids<IDM,type_attribut,version_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Version = position_enum<version_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,type_attribut,version_attribut>::entity_ids;
    using eaid::set_type;
    using eaid::set_version;
    BASE_ENTITY(type_version_entity)

    //! Constructeur à partir des valeurs attributs.
    type_version_entity(idt type, int version, idt id = 0)
        : eaid(id) {
        set_type(type);
        set_version(version);
    }
};

template<entidt IDM > type_version_entity<IDM>::~type_version_entity() {}
}

namespace entities {
/*! \ingroup groupe_entity
 *\brief Représentation un changment de version de la base de donnée.
 */
class version_bdd : public entity_ids<info_entity::Version_Bdd,attribut::date_time_current_attribut,
                                                               attribut::num_attribut,
                                                               attribut::type_id_attribut> {
protected:
    template<class T> using position_enum = position_enum<T,version_bdd>;
public:
    using eaid = entity_ids<info_entity::Version_Bdd,attribut::date_time_current_attribut,
                                                     attribut::num_attribut,
                                                     attribut::type_id_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut>::Position,
                       Date_Time = position_enum<date_time_current_attribut>::Position,
                       Num = position_enum<num_attribut>::Position,
                       Type = position_enum<type_id_attribut>::Position,
                       Nbr_Att = eaid::Nbr_Att};

    using eaid::entity_id;
    BASE_ENTITY(version_bdd)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    version_bdd(int num, idt type, idt id = 0)
        : eaid(id) {
        set_num(num);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    version_bdd(const QDateTime &dt, int num, idt type, idt id = 0)
        : version_bdd(num, type, id)
        {set_date_time(dt);}
};
}}
#endif // ENTITY_DIVERS
