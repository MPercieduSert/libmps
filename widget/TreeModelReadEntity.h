/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/09/2018
 */
#ifndef TREEMODELREADENTITY_H
#define TREEMODELREADENTITY_H

#include "Entity.h"
#include "TreeModelReadTemp.h"

/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre non-modifiable pour les entités.
 */
template<class Ent> class TreeModelReadEntity: public TreeModelReadTemp<Ent>
{
protected:
    QList<typename Ent::Position> m_atts;   //!< Liste des attributs à afficher.
    using TreeModelReadTemp<Ent>::m_tree;
    using TreeModelReadTemp<Ent>::getItem;

public:
    using TreeModelReadTemp<Ent>::createIndex;

    explicit TreeModelReadEntity(const QList<typename Ent::Position> & atts, QObject *parent = nullptr)
        : TreeModelReadTemp<Ent>(parent), m_atts(atts) {}

    //! Destructeur par defaut.
    ~TreeModelReadEntity() override = default;

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/) const override
        {return m_atts.isEmpty() ? Ent::NbrAtt : m_atts.count();}

    //! Renvoie la donnée correspondant à l'index et au role.
    QVariant data(const QModelIndex &index, int role) const override;

    //! Renvoie un index sur le premier (dans le sens de parcourt) noeud de l'arbre ayant pour donnée un entité d'identifiant id.
    QItemSelection foundId(idt id) const;

    //! Renvoie les labels des colonnes
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

template<class Ent> QVariant TreeModelReadEntity<Ent>::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem<Ent> *item = getItem(index);
    return m_atts.isEmpty() ? item->data().data(index.column()) : item->data().data(m_atts.at(index.column()));
}

template<class Ent> QItemSelection TreeModelReadEntity<Ent>::foundId(idt id) const
{
    typename TreeItem<Ent>::iterator i = m_tree.begin();
    while(i != m_tree.end() && (*i)->data().id() != id )
        ++i;
    if(i != m_tree.end())
        return QItemSelection(createIndex((*i)->position(),0,(*i)),createIndex((*i)->position(),m_atts.count()-1,(*i)));
    else
        return QItemSelection();
}

template<class Ent> QVariant TreeModelReadEntity<Ent>::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_atts.isEmpty() ? Ent::nomAttribut(section) : Ent::nomAttribut(m_atts.at(section));
    return QVariant();
}

#endif // TREEMODELREADENTITY_H
