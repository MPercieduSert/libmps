/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef TREEMODELREADTEMP_H
#define TREEMODELREADTEMP_H

#include "MAbstractTreeModel.h"
#include "Tree.h"
#include <QMessageBox>
#include <QItemSelection>

/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre non-modifiable.
 */
template<class T> class TreeModelReadTemp : public MAbstractTreeModel
{
protected:
    QStringList m_header;   //!< Liste des noms des colonnes.
    Tree<T> m_tree;       //!< Arbre de donnée.

public:
    using MAbstractTreeModel::MAbstractTreeModel;
    using MAbstractTreeModel::setHeaderData;

    //! Constructeur.
    explicit TreeModelReadTemp(QObject *parent = nullptr)
        : MAbstractTreeModel(parent) {}

    //! Destructeur par defaut.
    ~TreeModelReadTemp() override = default;

    //! Renvoie l'index correxpondant à la ligne et au parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &index) const override;

    //! Renvoie le nombre de d'enfants.
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
        {return getItem(parent)->childCount();}

    //! Modifie l'arbre de donnée.
    void setDataTree(const Tree<T> & tree)
    {
        beginResetModel();
        m_tree = tree;
        endResetModel();
    }

    //! Modifie l'arbre de donnée.
    void setDataTree(Tree<T> && tree)
    {
        beginResetModel();
        m_tree = std::move(tree);
        endResetModel();
    }

    //! Modifie les entêtes
    virtual void setHeaderData(const QStringList & listeHeader);

protected:
    TreeItem<T> *getItem(const QModelIndex &index) const;
};

template<class T> TreeItem<T>* TreeModelReadTemp<T>::getItem(const QModelIndex & index) const
{
    if (index.isValid())
    {
        TreeItem<T> *item = static_cast<TreeItem<T>*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_tree.root();
}

template<class T> QModelIndex TreeModelReadTemp<T>::index(int row, int column, const QModelIndex &parent) const
{  
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem<T> *parentItem = getItem(parent);
    TreeItem<T> *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

template<class T> QModelIndex TreeModelReadTemp<T>::parent(const QModelIndex &index) const
{
    if (index.isValid())
    {
        TreeItem<T> * child = getItem(index);
        TreeItem<T> * parent = child->parent();
        if(parent)
            return createIndex(parent->position(), 0, parent);
    }
    return QModelIndex();
}

template<class T> void TreeModelReadTemp<T>::setHeaderData(const QStringList & listeHeader)
{
    for(int i = 0; i < listeHeader.count(); ++i)
        setHeaderData(i,Qt::Horizontal,listeHeader.at(i),Qt::DisplayRole);
}
#endif // TREEMODELREADTEMP_H
