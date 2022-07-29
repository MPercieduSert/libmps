/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACT_LINK_SQL_H
#define ABSTRACT_LINK_SQL_H

#include <array>
#include <QSqlQuery>
#include <stdexcept>
#include "req_sql.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template frabriquant les liens avec la base de donnée à partir de l'ordre des attributs des entité.
 */
template<class Attribut, class att_pre, class att_suiv, post Pos> class link_sql_dichotomie;
template<class Attribut, post Pos = 0> using link_sql_attribut
        = link_sql_dichotomie<Attribut, typename Attribut::att_pre, typename Attribut::att_suiv, Pos>;

template<class Attribut, class att_pre, class att_suiv, post Pos> class link_sql_dichotomie :
        public link_sql_attribut<att_pre, Pos>,
        public link_sql_attribut<att_suiv, att_pre::Nbr_Att + Pos> {
public:
    enum {PosPre = Pos, PosSuiv = att_pre::Nbr_Att + Pos};
    using link_pre = link_sql_attribut<att_pre, PosPre>;
    using link_suiv = link_sql_attribut<att_suiv, PosSuiv>;

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bind_values(const Attribut &ent) {
        link_pre::bind_values(ent);
        link_suiv::bind_values(ent);
    }

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bind_values(Attribut &ent) {
        link_pre::bind_values(ent);
        link_suiv::bind_values(ent);
    }

    //! Hydrate l'entité entity avec à partir de la requète.
    void from_requete(Attribut &ent) const {
        link_pre::from_requete(ent);
        link_suiv::from_requete(ent);
    }
};

template<class Attribut, post Pos> class link_sql_dichotomie<Attribut, attribut::no_attribut, attribut::no_attribut, Pos>
        : public virtual req_sql {
public:
    enum {PosPre = Pos};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bind_values(const Attribut &ent)
        {bind_value(PosPre - 1,ent.get_to_bdd());}

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bind_values(Attribut &ent)
        {bind_value(PosPre - 1,ent.get_to_bdd());}

    //! Hydrate l'entité entity avec à partir de la requète.
    void from_requete(Attribut &ent) const
        {ent.set(value<typename Attribut::att_type>(PosPre));}
};

template<> class link_sql_dichotomie<entity, attribut::no_attribut, attribut::no_attribut, 0> : public virtual req_sql {
public:
    enum {PosPre = 0};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bind_values(const entity &/*ent*/)
        {}

    //! Hydrate l'entité entity avec à partir de la requète.
    void from_requete(entity &ent) const
        {ent.set_id(value<entity::att_type>(PosPre));}

    //! Mutateur de l'identifiant.
    void set_id(const entity &ent, post pos = 0)
        {m_requete->bindValue(static_cast<int>(pos),ent.id());}
};

template<class Ent> class link_sql : public link_sql_attribut<Ent> {
public:
    using link_sql_attribut<Ent>::from_requete;
    using link_sql_attribut<Ent>::id;
    CONSTR_DEFAUT(link_sql)
    DESTR_VIDE(link_sql)

    //! Crée dynamiquement une nouvelle entité de type Classe, l'hydrate à partir de la requète SQL.
    //! Puis retourne un  pointeur vers cette nouvelle entité.
    Ent *new_from_requete() const {
        Ent *ptr = new Ent();
        from_requete(*ptr);
        return ptr;
    }
};
}}
#endif // ABSTRACT_LINK_SQL_H
