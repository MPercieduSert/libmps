/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <forward_list>
#include <QAbstractItemModel>
#include "tree.h"
#include "type_mps.h"

//! Macro d'inclusion des membres virtual dans le model.
#define TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(TREE) /*! Renvoie l'index correxpondant à la ligne et colonne de parent.*/ \
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override \
        {return TREE.index(row,column,parent);} \
    /*! Teste si le noeud associé à un index valide est une feuille.*/ \
    bool leaf(const QModelIndex &index) const {return TREE.leaf(index);} \
    /*! Renvoie l'index du parent.*/ \
    QModelIndex parent(const QModelIndex &index) const override {return TREE.parent(index);} \
    /*! Renvoie le nombre de d'enfants.*/ \
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { \
    if(parent.column() != 0) return 0; \
    return TREE.row_count(parent);}

namespace mps {
namespace model_base {
template<class T> class tree_for_model;
/*! \ingroup groupe_model
 *\brief Classe Abstraite mère des models à colonnes et à nodes de type arbre utilisant le contenant treeFroModel.
 */
class abstract_model : public QAbstractItemModel {
    Q_OBJECT
    template<class T> friend class tree_for_model;
public:
    using QAbstractItemModel::QAbstractItemModel;
};

/*! \ingroup groupe_model
 *\brief Classe template des données des models de type arbre de structure non-modifiable.
 */
template<class T, class Model, class Index> class tree_for_model_temp {
protected:
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = typename tree<T>::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = typename tree<T>::iterator;
    Model *m_model;         //!< Model contenant l'arbre.
    const bool m_racine;    //!< Racine permise.
    tree<T> m_tree;         //!< arbre de donnée.
public:
    //! Constructeur.
    tree_for_model_temp(Model *model, bool racine = false)
        : m_model(model), m_racine(racine) {}

    //! Renvoie le nombre de d'enfants.
    szt child_count(const Index &parent = Index()) const {
        if(!parent.isValid())
                return m_tree.cbegin().size_child();
        return get_iter(parent).size_child();
    }

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T &get_data(const Index &index) const
        {return *get_iter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T &get_data(const Index &index)
        {return *get_iter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator get_iter(const Index &index) const
        {return index.isValid() ? get_valid_iter(index)
                                : m_tree.cbegin();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator get_iter(const Index &index)
        {return index.isValid() ? get_valid_iter(index)
                                : m_tree.cbegin();}

    //! Renvoie une référence sur la donnée de la racine.
    const T &get_root_data() const
        {return *m_tree.cbegin();}

    //! Renvoie une référence sur la donnée de la racine.
    T &get_root_data()
        {return *m_tree.begin();}

    //! Renvoie un itérateur sur la racine.
    const_iterator get_root_iter() const
        {return m_tree.cbegin();}

    //! Renvoie un itérateur sur la racine.
    iterator get_root_iter()
        {return m_tree.begin();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const T &get_valid_data(const Index &index) const
        {return *get_valid_iter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    T &get_valid_data(const Index &index)
        {return *get_valid_iter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator get_valid_iter(const Index &index) const
        {return index.internalPointer();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator get_valid_iter(const Index &index)
        {return index.internalPointer();}

    //! Teste si le noeud associé à un index valide est une feuille.
    bool leaf(const Index &index) const
        {return  get_valid_iter(index).leaf();}

    //! Insert des lignes dans le model, ne vérifie pas les arguments.
    template<class Factory> bool insert_nodes(Factory factory, szt row, szt count, const Index &parent = Index());

    //! Insert des lignes dans le model, ne vérifie pas les arguments.
    template<class Factory> bool insert_rows(Factory factory,int row, int count, const Index &parent = Index());

    //! position du noeud dans la fratrie.
    szt position(const Index &index) const
        {return index.is_valid() ? get_valid_iter(index).position()
                                : 0;}

    //! Acceseur de la racine.
    bool racine() const
        {return m_racine;}

    //! Supprime des lignes dans le model.
    template<class Deleter> bool remove_rows(Deleter del, int row, int count, const Index &parent = Index());

    //! Renvoie le nombre de d'enfants.
    int row_count(const Index &parent = Index()) const
        {return child_count(parent);}

    //! Modifie l'arbre de donnée.
    void set_tree(const tree<T> &tr) {
        if(m_racine){
            m_tree.clear();
            m_tree.push_back(m_tree.begin(),tr.cbegin());
        }
        else
            m_tree = tr;
    }

    //! Modifie l'arbre de donnée.
    void set_tree(tree<T> &&tr){
        if(m_racine){
            m_tree.clear();
            m_tree.push_back(m_tree.begin(),std::move(tr));
        }
        else
            m_tree = std::move(tr);
    }

    //! Accesseur de l'arbre.
    const tree<T> &tree_data() const
        {return m_tree;}

    //! Accesseur de l'arbre.
    tree<T> &tree_data()
        {return m_tree;}
};

/*! \ingroup groupe_model
 *\brief Classe template des données des models de type arbre de structure non-modifiable.
 */
template<class T> class tree_for_model : public tree_for_model_temp<T,abstract_model,QModelIndex> {
protected:
    using tfm = tree_for_model_temp<T,abstract_model,QModelIndex>;
    using tfm::m_model;
    using tfm::m_racine;
    using tfm::m_tree;
public:
    using tfm::get_iter;
    //! Constructeur.
    using tree_for_model_temp<T,abstract_model,QModelIndex>::tree_for_model_temp;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &index) const;
};

///////////////////////////////////// tree_for_model_temp /////////////////////////////
template<class T, class Model, class Index> template<class Factory>
bool tree_for_model_temp<T,Model,Index>::insert_nodes(Factory factory, szt row, szt count,const Index &parent) {
    auto iter = get_iter(parent);
    if(row == child_count(parent))
        for(szt i = 0; i != count; ++i)
            m_tree.push_back(iter,factory(row + i, parent));
    else {
        iter.to_child_u(row);
        for(szt i = count; i != 0; --i)
            m_tree.insert(iter,factory(row + i, parent));
    }
    return true;
}

template<class T, class Model, class Index> template<class Factory>
bool tree_for_model_temp<T,Model,Index>::insert_rows(Factory factory, int row, int count,const Index &parent) {
    auto iter = get_iter(parent);
    if(row == row_count(parent))
        for(int i = 0; i != count; ++i)
            m_tree.push_back(iter,factory(row + i, parent));
    else {
        iter.to_child(row);
        for(int i = count; i != 0; --i)
            m_tree.insert(iter,factory(row + i, parent));
    }
    return true;
}

template<class T, class Model, class Index> template<class Deleter>
bool tree_for_model_temp<T,Model,Index>::remove_rows(Deleter del, int row, int count, const Index &parent) {
    if(m_racine && !parent.isValid())
        return false;
    int comp = 0;
    for(int i = 0; i != count; ++i) {
        std::forward_list<iterator> pile;
        pile.push_front(get_iter(parent).to_child(row));
        bool remove = true;
        while(!pile.empty() &&remove) {
            if(!pile.front().leaf())
                pile.push_front(pile.front().crbegin_child());
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

///////////////////////////////////// tree_for_model //////////////////////////////////
template<class T> QModelIndex tree_for_model<T>::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        if(parent.column() == 0) {
            auto iter = get_iter(parent);
            iter.to_child(row);
            if (iter)
                return m_model->createIndex(row, column, iter.ptr());
        }
    }
    else if (!m_racine || row == 0){
        auto iter = m_tree.cbegin().to_child(row);
        if (iter)
            return m_model->createIndex(row, column, iter.ptr());
    }
    return QModelIndex();
}

template<class T> QModelIndex tree_for_model<T>::parent(const QModelIndex &index) const {
    if (index.isValid()) {
        auto iter = get_iter(index);
        iter.to_parent();
        if(iter && !iter.root())
            return m_model->createIndex(iter.index_brother(), 0, iter.ptr());
    }
    return QModelIndex();
}
}}
#endif // ABSTRACTMODEL_H
