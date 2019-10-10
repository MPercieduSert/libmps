/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef TREEMODELEDITENTITY_H
#define TREEMODELEDITENTITY_H

#include <QStack>
#include "AbstractManager.h"
#include "TreeModelReadEntity.h"

/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre modifiable.
 */
template<class Ent> class TreeModelEditEntity : public TreeModelReadEntity<Ent>
{
public:
    using TreeModelReadEntity<Ent>::TreeModelReadEntity;

    //! Destructeur par defaut.
    ~TreeModelEditEntity() override = default;

    //! Renvoie les autorisations de modification pour un index donné.
    virtual bool autorisation(const QModelIndex & /*index*/, bdd::Autorisation /*role*/) const
        {return true;}

    //! Renvoie les drapeaux de l'index spécifié.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Créer une nouvelle entité
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
    using TreeModelReadEntity<Ent>::beginInsertRows;
    using TreeModelReadEntity<Ent>::beginRemoveRows;
    using TreeModelReadEntity<Ent>::beginResetModel;
    using TreeModelReadEntity<Ent>::createIndex;
    using TreeModelReadEntity<Ent>::endInsertRows;
    using TreeModelReadEntity<Ent>::endRemoveRows;
    using TreeModelReadEntity<Ent>::endResetModel;
    using TreeModelReadEntity<Ent>::getItem;
    using TreeModelReadEntity<Ent>::parent;
};

template<class Ent> Qt::ItemFlags TreeModelEditEntity<Ent>::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

template<class Ent> bool TreeModelEditEntity<Ent>::insertRows(int row, int count, const QModelIndex &parent)
{
    if(!parent.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i)
    {
        Ent entity;
        if(hydrateNewEntity(entity, row, parent))
        {
            beginInsertRows(parent,row,row);
                getItem(parent)->insert(row, entity);
            endInsertRows();
            ++row;
            ++comp;
        }
    }
    return comp == count;
}

template<class Ent> bool TreeModelEditEntity<Ent>::removeRows(int row, int count, const QModelIndex &parentIndex)
{
    if(!parentIndex.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i)
    {
        QStack<TreeItem<Ent> *> pile;
        pile.push(getItem(parentIndex)->child(row));
        bool remove = true;
        while(!pile.isEmpty() && remove)
        {
            if(pile.top()->hasChild())
                pile.push(pile.top()->lastChild());
            else
            {
                int n = pile.top()->position();
                remove = removeEntity(pile.top()->data());
                if(remove)
                {
                    beginRemoveRows(parent(createIndex(0,0,pile.top())),n,n);
                        delete pile.pop();
                    endRemoveRows();
                }
            }
        }
        if(pile.isEmpty())
            ++comp;
        ++row;
    }
    return comp == count;
}

#endif // TREEMODELEDITENTITY_H
