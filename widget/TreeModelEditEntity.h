/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef TREEMODELEDITENTITY_H
#define TREEMODELEDITENTITY_H

#include <QStack>
#include "TreeModelReadEntity.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre modifiable.
 */
template<class Ent> class TreeModelEditEntity : public TreeModelReadEntity<Ent> {
public:
    using TreeModelReadEntity<Ent>::TreeModelReadEntity;

    //! Destructeur par defaut.
    ~TreeModelEditEntity() override = default;

    //! Renvoie les autorisations de modification pour un index donné.
    virtual bool autorisation(const QModelIndex & /*index*/, bmps::autorisation /*role*/) const
        {return true;}

    //! Renvoie les drapeaux de l'index spécifié.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Remplit une nouvelle entité
    virtual bool hydrateNewEntity(Ent & /*entity*/, int /*row*/ = 0, const QModelIndex &/*parent*/ = QModelIndex())
        {return true;}

    //! Insert des lignes dans le model.
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Supprime l'entité de la base de donnée si nécéssaire.
    virtual bool removeEntity(const Ent & /*entity*/)
        {return true;}

    //! Supprime des lignes dans le model.
    bool removeRows(int row, int count, const QModelIndex &parent) override;

protected:
    using TreeModelReadEntity<Ent>::m_tree;
    using TreeModelReadEntity<Ent>::beginInsertRows;
    using TreeModelReadEntity<Ent>::beginRemoveRows;
    using TreeModelReadEntity<Ent>::beginResetModel;
    using TreeModelReadEntity<Ent>::createIndex;
    using TreeModelReadEntity<Ent>::endInsertRows;
    using TreeModelReadEntity<Ent>::endRemoveRows;
    using TreeModelReadEntity<Ent>::endResetModel;
    using TreeModelReadEntity<Ent>::getIter;
    using TreeModelReadEntity<Ent>::parent;
};

template<class Ent> Qt::ItemFlags TreeModelEditEntity<Ent>::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

template<class Ent> bool TreeModelEditEntity<Ent>::insertRows(int row, int count, const QModelIndex &parent) {
    if(!parent.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i) {
        Ent entity;
        if(hydrateNewEntity(entity, row, parent)) {
            beginInsertRows(parent,row,row);
                typename cmps::tree<Ent>::iterator iter = getIter(parent);
                iter.toChild(row);
                m_tree.insert(iter,std::move(entity));
            endInsertRows();
            ++row;
            ++comp;
        }
    }
    return comp == count;
}

template<class Ent> bool TreeModelEditEntity<Ent>::removeRows(int row, int count, const QModelIndex &parentIndex) {
    if(!parentIndex.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i) {
        std::forward_list<typename cmps::tree<Ent>::iterator> pile;
        pile.push_front(getIter(parentIndex).toChild(row));
        bool remove = true;
        while(!pile.empty() && remove) {
            if(!pile.front().leaf())
                pile.push_front(pile.front().crbeginChild());
            else {
                int n = pile.front().indexBrother();
                remove = removeEntity(*pile.front());
                if(remove) {
                    beginRemoveRows(parent(createIndex(0,0,pile.front())),n,n);
                        m_tree.erase(pile.front());
                        pile.pop_front();
                    endRemoveRows();
                }
            }
        }
        if(pile.empty())
            ++comp;
        ++row;
    }
    return comp == count;
}
}
#endif // TREEMODELEDITENTITY_H
