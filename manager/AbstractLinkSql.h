/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTLINKSQL_H
#define ABSTRACTLINKSQL_H

#include <array>
#include <QSqlQuery>
#include <stdexcept>
#include "ReqSql.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template frabriquant les liens avec la base de donnée à partir de l'ordre des attributs des entité.
 */
template<class Attribut, class att_pre, class att_suiv, post Pos> class LinkSqlDichot;
template<class Attribut, post Pos = 0> using LinkSqlAttribut
                                            = LinkSqlDichot<Attribut, typename Attribut::att_pre, typename Attribut::att_suiv, Pos>;

template<class Attribut, class att_pre, class att_suiv, post Pos> class LinkSqlDichot :
        public LinkSqlAttribut<att_pre, Pos>,
        public LinkSqlAttribut<att_suiv, att_pre::Nbr_Att + Pos> {
public:
    enum {PosPre = Pos, PosSuiv = att_pre::Nbr_Att + Pos};
    using LinkPre = LinkSqlAttribut<att_pre, PosPre>;
    using LinkSuiv = LinkSqlAttribut<att_suiv, PosSuiv>;

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut &ent) {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut &ent) {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut &ent) const {
        LinkPre::fromRequete(entity);
        LinkSuiv::fromRequete(entity);
    }
};

template<class Attribut, post Pos> class LinkSqlDichot<Attribut, attributMPS::no_attribut, attributMPS::no_attribut, Pos>
        : public virtual ReqSql {
public:
    enum {PosPre = Pos};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut &ent)
        {bindValue(PosPre - 1,entity.get_to_bdd());}

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut &ent)
        {bindValue(PosPre - 1,entity.get_to_bdd());}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut &ent) const
        {entity.set(value<typename Attribut::att_type>(PosPre));}
};

template<> class LinkSqlDichot<entity, attributMPS::no_attribut, attributMPS::no_attribut, 0> : public virtual ReqSql {
public:
    enum {PosPre = 0};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const entity &/*ent*/)
        {}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(entity &ent) const
        {entity.set_id(value<entity::att_type>(PosPre));}

    //! Mutateur de l'identifiant.
    void set_id(const entity &ent, post pos = 0)
        {m_requete->bindValue(static_cast<int>(pos),entity.id());}
};

template<class Ent> class LinkSql : public LinkSqlAttribut<Ent> {
public:
    using LinkSqlAttribut<Ent>::fromRequete;
    using LinkSqlAttribut<Ent>::id;
    CONSTR_DEFAUT(LinkSql)
    DESTR_VIDE(LinkSql)

    //! Crée dynamiquement une nouvelle entité de type Classe, l'hydrate à partir de la requète SQL.
    //! Puis retourne un  pointeur vers cette nouvelle entité.
    Ent *newFromRequete() const {
        Ent *ptr = new Ent();
        fromRequete(*ptr);
        return ptr;
    }
};
}
#endif // ABSTRACTLINKSQL_H
