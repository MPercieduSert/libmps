/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MABSTRACTTREEMODEL_H
#define MABSTRACTTREEMODEL_H

#include <forward_list>
#include <QAbstractItemModel>
#include "Tree.h"
#include "typemps.h"

//! Macro d'inclusion des membres virtual dans le model.
#define TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(TREE) /*! Renvoie l'index correxpondant à la ligne et colonne de parent.*/ \
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override \
        {return TREE.index(row,column,parent);} \
    /*! Renvoie l'index du parent.*/ \
    QModelIndex parent(const QModelIndex &index) const override {return TREE.parent(index);} \
    /*! Renvoie le nombre de d'enfants.*/ \
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {return TREE.rowCount(parent);}

namespace modelMPS {
template<class T> class ReadableTreeForModel;
/*! \ingroup groupeModel
 * \brief Classe Abstraite mère des model de type arbre utilisant le contenant treeFroModel.
 */
class AbstractTreeModel : public QAbstractItemModel {
    Q_OBJECT
    template<class T> friend class ReadableTreeForModel;
public:
    using QAbstractItemModel::QAbstractItemModel;
};


namespace cmps = conteneurMPS;
/*! \ingroup groupeModel
 * \brief Classe template des données des models de type arbre de structure non-modifiable.
 */
template<class T> class ReadableTreeForModel {
protected:
    //! Classe de la structure de donnée.
    using Tree = cmps::tree<T>;
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = typename Tree::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = typename Tree::iterator;
    AbstractTreeModel * m_parent;      //!< Model contenant l'arbre.
    const bool m_racine;      //!< Racine permise.
    Tree m_tree;        //!< Arbre de donnée.
public:
    //! Constructeur.
    ReadableTreeForModel(AbstractTreeModel * parent, bool racine = false)
        : m_parent(parent), m_racine(racine) {}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T & getData(const QModelIndex &index) const
        {return *getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T & getData(const QModelIndex &index)
        {return *getIter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator getIter(const QModelIndex &index) const
        {return index.isValid() ? getValidIter(index)
                                : m_tree.cbegin();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator getIter(const QModelIndex &index)
        {return index.isValid() ? getValidIter(index)
                                : m_tree.cbegin();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T & getValidData(const QModelIndex &index) const
        {return *getValidIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T & getValidData(const QModelIndex &index)
        {return *getIter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator getValidIter(const QModelIndex &index) const
        {return index.internalPointer();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator getValidIter(const QModelIndex &index)
        {return index.internalPointer();}

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &index) const;

    //! Renvoie le nombre de d'enfants.
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if(m_racine && !parent.isValid())
            return 1;
        return getIter(parent).sizeChild();
    }

    //! Modifie l'arbre de donnée.
    void setTree(const cmps::tree<T> & tree)
        {m_tree = tree;}

    //! Modifie l'arbre de donnée.
    void setTree(cmps::tree<T> && tree)
        {m_tree = std::move(tree);}
};

/*! \ingroup groupeModel
 * \brief Classe template des données des models de type arbre modifiable.
 */
template<class T> class TreeForModel : public ReadableTreeForModel<T> {
protected:
    using ReadableTreeForModel<T>::m_racine;
    using ReadableTreeForModel<T>::m_tree;
    using iterator = typename ReadableTreeForModel<T>::iterator;
public:
    using ReadableTreeForModel<T>::ReadableTreeForModel;
    using ReadableTreeForModel<T>::getIter;
    using ReadableTreeForModel<T>::rowCount;

    //! Insert des lignes dans le model.
    template<class Factory> bool insertRows(Factory factory,int row, int count, const QModelIndex &parent = QModelIndex());

    //! Supprime des lignes dans le model.
    template<class Deleter> bool removeRows(Deleter del, int row, int count, const QModelIndex &parent = QModelIndex());
};

///////////////////////////////////// ReadableTreeForModel //////////////////////////////////
template<class T> QModelIndex ReadableTreeForModel<T>::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        if(parent.column() == 0) {
            auto iter = getIter(parent);
            iter.toChild(row);
            if (iter)
                return m_parent->createIndex(row, column, iter.ptr());
        }
    }
    else if(m_racine){
        if(row == 0)
            return m_parent->createIndex(0,column,m_tree.cbegin().ptr());
    }
    else {
        auto iter = m_tree.cbegin().toChild(row);
        if (iter)
            return m_parent->createIndex(row, column, iter.ptr());
    }
    return QModelIndex();
}

template<class T> QModelIndex ReadableTreeForModel<T>::parent(const QModelIndex &index) const {
    if (index.isValid()) {
        auto iter = getIter(index);
        iter.toParent();
        if(iter)
            return m_parent->createIndex(iter.indexBrother(), 0, iter.ptr());
    }
    return QModelIndex();
}

//////////////////////////////////// TreeForModel /////////////////////////////
template<class T> template<class Factory> bool TreeForModel<T>::insertRows(Factory factory, int row, int count,
                                                                           const QModelIndex &parent) {
    if(m_racine && !parent.isValid())
        return false;
    auto iter = getIter(parent);
    if(row == rowCount(parent))
        for(int i = 0; i != count; ++i)
            m_tree.push_back(iter,factory(row + i, parent));
    else {
        iter.toChild(row);
        for(int i = 0; i != count; ++i)
            m_tree.insert(iter,factory(row + i, parent));
    }
    return true;
}

template<class T> template<class Deleter> bool TreeForModel<T>::removeRows(Deleter del, int row, int count, const QModelIndex &parent) {
    if(m_racine && !parent.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i) {
        std::forward_list<iterator> pile;
        pile.push_front(getIter(parent).toChild(row));
        bool remove = true;
        while(!pile.empty() && remove) {
            if(!pile.front().leaf())
                pile.push_front(pile.front().crbeginChild());
            else {
                //int n = pile.front().indexBrother();
                remove = del(*pile.front());
                if(remove) {
                    m_tree.erase(pile.front());
                    pile.pop_front();
                }
            }
        }
        if(pile.empty())
            ++comp;
        else
            ++row;
    }
    return comp == count;
}
}
#endif // MABSTRACTTREEMODEL_H
