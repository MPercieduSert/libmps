/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef TREEMODELREADTEMP_H
#define TREEMODELREADTEMP_H

#include <QItemSelection>
#include <QMessageBox>

#include "AbstractModel.h"
#include "Tree.h"

namespace modelMPS {
namespace cmps = conteneurMPS;
/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre non-modifiable.
 */
template<class T> class TreeModelReadTemp : public AbstractModel {
protected:
    QStringList m_header;   //!< Liste des noms des colonnes.
    cmps::tree<T> m_tree;       //!< Arbre de donnée.

public:
    using AbstractModel::AbstractModel;
    using AbstractModel::setHeaderData;

    //! Constructeur.
    explicit TreeModelReadTemp(QObject *parent = nullptr)
        : AbstractModel(parent) {}

    //! Destructeur par defaut.
    ~TreeModelReadTemp() override = default;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T & getData(const QModelIndex &index) const
        {return *getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T & getData(const QModelIndex &index)
        {return *static_cast<typename conteneurMPS::tree<T>::iterator>(getIter(index));}

    //! Renvoie l'index correxpondant à la ligne et au parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &index) const override;

    //! Renvoie le nombre de d'enfants.
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
        {return getIter(parent).sizeChild();}

    //! Modifie l'arbre de donnée.
    void setDataTree(const cmps::tree<T> & tree) {
        beginResetModel();
        m_tree = tree;
        endResetModel();
    }

    //! Modifie l'arbre de donnée.
    void setDataTree(cmps::tree<T> && tree) {
        beginResetModel();
        m_tree = std::move(tree);
        endResetModel();
    }

    //! Modifie les entêtes
    virtual void setHeaderData(const QStringList & listeHeader);

protected:
    typename cmps::tree<T>::const_iterator getIter(const QModelIndex &index) const;
};

template<class T> typename cmps::tree<T>::const_iterator TreeModelReadTemp<T>::getIter(const QModelIndex & index) const {
    if (index.isValid()) {
        typename cmps::tree<T>::iterator iter = index.internalPointer();
        if (iter)
            return iter;
    }
    return m_tree.cbegin();
}

template<class T> QModelIndex TreeModelReadTemp<T>::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    auto iter = getIter(parent);
    iter.toChild(row);
    if (iter)
        return createIndex(row, column, iter.ptr());
    else
        return QModelIndex();
}

template<class T> QModelIndex TreeModelReadTemp<T>::parent(const QModelIndex &index) const {
    if (index.isValid()) {
        auto iter = getIter(index);
        iter.toParent();
        if(iter)
            return createIndex(iter.indexBrother(), 0, iter.ptr());
    }
    return QModelIndex();
}

template<class T> void TreeModelReadTemp<T>::setHeaderData(const QStringList & listeHeader) {
    for(int i = 0; i < listeHeader.count(); ++i)
        setHeaderData(i,Qt::Horizontal,listeHeader.at(i),Qt::DisplayRole);
}
}
#endif // TREEMODELREADTEMP_H
