/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <forward_list>
#include <QAbstractItemModel>
#include "Tree.h"
#include "typemps.h"

//! Macro d'inclusion des membres virtual dans le model.
#define TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(TREE) /*! Renvoie l'index correxpondant à la ligne et colonne de parent.*/ \
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override \
        {return TREE.index(row,column,parent);} \
    /*! Teste si le noeud associé à un index valide est une feuille.*/ \
    bool leaf(const QModelIndex & index) const {return TREE.leaf(index);} \
    /*! Renvoie l'index du parent.*/ \
    QModelIndex parent(const QModelIndex &index) const override {return TREE.parent(index);} \
    /*! Renvoie le nombre de d'enfants.*/ \
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { \
    if(parent.column() != 0) return 0; \
    return TREE.rowCount(parent);}

namespace modelMPS {
namespace cmps = conteneurMPS;

template<class T> class TreeForModel;
/*! \ingroup groupeModel
 * \brief Classe Abstraite mère des models à colonnes et à nodes de type arbre utilisant le contenant treeFroModel.
 */
class AbstractModel : public QAbstractItemModel {
    Q_OBJECT
    template<class T> friend class TreeForModel;
public:
    using QAbstractItemModel::QAbstractItemModel;
};

/*! \ingroup groupeModel
 * \brief Classe template des données des models de type arbre de structure non-modifiable.
 */
template<class T, class Model, class Index> class TempTreeForModel {
protected:
    //! Classe de la structure de donnée.
    using Tree = cmps::tree<T>;
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = typename Tree::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = typename Tree::iterator;
    Model * m_model;            //!< Model contenant l'arbre.
    const bool m_racine;        //!< Racine permise.
    Tree m_tree;                //!< Arbre de donnée.
public:
    //! Constructeur.
    TempTreeForModel(Model * model, bool racine = false)
        : m_model(model), m_racine(racine) {}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T & getData(const Index &index) const
        {return *getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T & getData(const Index &index)
        {return *getIter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator getIter(const Index &index) const
        {return index.isValid() ? getValidIter(index)
                                : m_tree.cbegin();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator getIter(const Index &index)
        {return index.isValid() ? getValidIter(index)
                                : m_tree.cbegin();}

    //! Renvoie une référence sur la donnée de la racine.
    const T & getRootData() const
        {return *m_tree.cbegin();}

    //! Renvoie une référence sur la donnée de la racine.
    T & getRootData()
        {return *m_tree.begin();}

    //! Renvoie un itérateur sur la racine.
    const_iterator getRootIter() const
        {return m_tree.cbegin();}

    //! Renvoie un itérateur sur la racine.
    iterator getRootIter()
        {return m_tree.begin();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T & getValidData(const Index & index) const
        {return *getValidIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T & getValidData(const Index & index)
        {return *getIter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator getValidIter(const Index & index) const
        {return index.internalPointer();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator getValidIter(const Index & index)
        {return index.internalPointer();}

    //! Teste si le noeud associé à un index valide est une feuille.
    bool leaf(const Index & index) const
        {return  getValidIter(index).leaf();}

    //! Insert des lignes dans le model, ne vérifie pas les arguments.
    template<class Factory> bool insertRows(Factory factory,int row, int count, const Index &parent = Index());

    //! Acceseur de la racine.
    bool racine() const
        {return m_racine;}

    //! Supprime des lignes dans le model.
    template<class Deleter> bool removeRows(Deleter del, int row, int count, const Index &parent = Index());

    //! Renvoie le nombre de d'enfants.
    int rowCount(const Index &parent = Index()) const {
        if(!parent.isValid())
                return m_tree.cbegin().sizeChild();
        return getIter(parent).sizeChild();
    }

    //! Modifie l'arbre de donnée.
    void setTree(const cmps::tree<T> & tree) {
        if(m_racine){
            m_tree.clear();
            m_tree.push_back(m_tree.begin(),tree.cbegin());
        }
        else
            m_tree = tree;
    }

    //! Modifie l'arbre de donnée.
    void setTree(cmps::tree<T> && tree){
        if(m_racine){
            m_tree.clear();
            m_tree.push_back(m_tree.begin(),std::move(tree));
        }
        else
            m_tree = std::move(tree);
    }

    //! Accesseur de l'arbre.
    const cmps::tree<T> & tree() const
        {return m_tree;}

    //! Accesseur de l'arbre.
    cmps::tree<T> & tree()
        {return m_tree;}
};

/*! \ingroup groupeModel
 * \brief Classe template des données des models de type arbre de structure non-modifiable.
 */
template<class T> class TreeForModel : public TempTreeForModel<T,AbstractModel,QModelIndex> {
protected:
    using TFM = TempTreeForModel<T,AbstractModel,QModelIndex>;
    using TFM::m_model;
    using TFM::m_racine;
    using TFM::m_tree;
public:
    using TFM::getIter;
    //! Constructeur.
    using TempTreeForModel<T,AbstractModel,QModelIndex>::TempTreeForModel;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &index) const;
};

///////////////////////////////////// TempTreeForModel /////////////////////////////
template<class T, class Model, class Index> template<class Factory>
bool TempTreeForModel<T,Model,Index>::insertRows(Factory factory, int row, int count,const Index &parent) {
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



template<class T, class Model, class Index> template<class Deleter>
bool TempTreeForModel<T,Model,Index>::removeRows(Deleter del, int row, int count, const Index &parent) {
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

///////////////////////////////////// TreeForModel //////////////////////////////////
template<class T> QModelIndex TreeForModel<T>::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        if(parent.column() == 0) {
            auto iter = getIter(parent);
            iter.toChild(row);
            if (iter)
                return m_model->createIndex(row, column, iter.ptr());
        }
    }
    else if (!m_racine || row == 0){
        auto iter = m_tree.cbegin().toChild(row);
        if (iter)
            return m_model->createIndex(row, column, iter.ptr());
    }
    return QModelIndex();
}

template<class T> QModelIndex TreeForModel<T>::parent(const QModelIndex &index) const {
    if (index.isValid()) {
        auto iter = getIter(index);
        iter.toParent();
        if(iter && !iter.root())
            return m_model->createIndex(iter.indexBrother(), 0, iter.ptr());
    }
    return QModelIndex();
}
}
#endif // ABSTRACTMODEL_H
