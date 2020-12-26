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
 * \brief Classe template frabriquant les liens avec la base de donnée à partir de l'ordre des attributs des entité.
 */
template<class Attribut, class AttPre, class AttSuiv, post Pos> class LinkSqlDichot;
template<class Attribut, post Pos = 0> using LinkSqlAttribut
                                            = LinkSqlDichot<Attribut, typename Attribut::AttPre, typename Attribut::AttSuiv, Pos>;

template<class Attribut, class AttPre, class AttSuiv, post Pos> class LinkSqlDichot :
        public LinkSqlAttribut<AttPre, Pos>,
        public LinkSqlAttribut<AttSuiv, AttPre::NbrAtt + Pos> {
public:
    enum {PosPre = Pos, PosSuiv = AttPre::NbrAtt + Pos};
    using LinkPre = LinkSqlAttribut<AttPre, PosPre>;
    using LinkSuiv = LinkSqlAttribut<AttSuiv, PosSuiv>;

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut & entity) {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut & entity) {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut & entity) const {
        LinkPre::fromRequete(entity);
        LinkSuiv::fromRequete(entity);
    }
};

template<class Attribut, post Pos> class LinkSqlDichot<Attribut, attributMPS::NoAttribut, attributMPS::NoAttribut, Pos>
        : public virtual ReqSql {
public:
    enum {PosPre = Pos};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut & entity)
        {bindValue(PosPre - 1,entity.getToBdd());}

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut & entity)
        {bindValue(PosPre - 1,entity.getToBdd());}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut & entity) const
        {entity.set(value<typename Attribut::AttType>(PosPre));}
};

template<> class LinkSqlDichot<Entity, attributMPS::NoAttribut, attributMPS::NoAttribut, 0> : public virtual ReqSql {
public:
    enum {PosPre = 0};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Entity & /*entity*/)
        {}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Entity & entity) const
        {entity.setId(value<Entity::AttType>(PosPre));}

    //! Mutateur de l'identifiant.
    void setId(const Entity & entity, post pos = 0)
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
    Ent * newFromRequete() const {
        Ent * ptr = new Ent();
        fromRequete(*ptr);
        return ptr;
    }
};
}
#endif // ABSTRACTLINKSQL_H
