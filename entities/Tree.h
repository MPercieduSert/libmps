/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef Tree_H
#define Tree_H

#include "TreeItem.h"

/*! \ingroup groupeTree
 *  \brief Cette classe represente un arbre dont les noeuds sont constitués par des TreeItem<T> et d'un noeud courant (curseur).
 *
 * Cette classe represente un arbre. Elle est constituée de la racine d'un arbre dont les noeuds sont constitués par des TreeItem<T>
 * et d'un itérateur sur cet arbre jouant le role d'un curseur, ce qui munit l'arbre d'un noeud courant.
 */

template<class T> class Tree
{
protected:
    TreeItem<T> * m_root;                           //!< Pointeur sur la racine de l'arbre.
    mutable typename TreeItem<T>::iterator m_i;     //!< Itérateur sur l'arbre de racine pointée par m_root.

public:
    //! Constructeur, prend le neud transmis pour racine (doit être créer dynamiquement et ne dois pas avoir de parent)
    //! et initialise l'itérateur sur cette racine.
    Tree(TreeItem<T> * root) noexcept
        : m_root(root),
          m_i(m_root)
        {}

    //! Constructeur sans argument.
    Tree() : Tree(new TreeItem<T>) {}

    //! Constructeur de recopie.
    Tree(const Tree<T> & tree)
        : Tree(new TreeItem<T>(*(tree.m_root)))
        {}

    //! Constructeur de déplacement.
    Tree(Tree<T> && tree) noexcept
        : Tree(tree.m_root)
    {
        tree.m_root = nullptr;
        tree.m_i = nullptr;
    }

    //! Constructeur, crée une racine de donnée associée data et initialise l'itérateur sur cette racine.
    Tree(const T & data)
        : Tree(new TreeItem<T>(data))
        {}

    //! Constructeur, créer un arbre possédant la même structure et les même valeur que tree,
    //! T doit posséder un constructeur à partir de U.
    template<class U> Tree(const Tree<U> & tree)
        : Tree(new TreeItem<T>(*tree.root()))
        {}

    //! Constructeur d'un arbre de donnée de type T à partir d'un arbre de donnée de type U,
    //! le foncteur usine construit une donnée de type T à partir d'une donnée de type U.
    template<class U, class F> Tree(const Tree<U> & tree, F usine)
        : Tree(new TreeItem<T>(*tree.root(),usine))
        {}

    //! Destructeur. Détruit tous les noeuds de l'arbre.
    ~Tree()
        {delete m_root;}

    //! Crée un nouveau descendant direct au noeud courant, ce nouveau noeud à pour donnée associée data (voir l'opérateur <<).
    void addChild(const T & data)
    {
        if(m_i)
            m_i = (*m_i)->addChild(data);
        else
            throw std::runtime_error(QString("void Tree<T>::addChild(const T &): Itérateur interne invalide.").toStdString());
    }

    //! Réinitialise l'itérateur sur la racine.
    typename TreeItem<T>::iterator begin() const noexcept
        {return m_i = m_root;}

    //! Renvoie la liste des descendants directs du noeud courant.
    QList<TreeItem<T>*> childs() const
    {
        if(m_i)
            return (*m_i)->childs();
        else
            return QList<TreeItem<T>*>();
    }

    //! Réinitialise l'itérateur sur la racine et détruit l'ensemble des descendants de la racine.
    void clear() noexcept
    {
        m_i = m_root;
        m_root->removeAllChild();
    }

    //! Renvoie la donnée associée au noeuds courant.
    const T & data() const
    {
        if(m_i)
            return (*m_i)->data();
        else
            throw std::runtime_error(QString("const T & data() const: itérateur interne invalide.").toStdString());
    }

    //! Supprime les noeuds ne vérifiant pas le prédicat (bool predicat(TreeItem<T> *)) ainsi que leurs descendances.
    template<class U> const Tree<T> & elagage(U predicat);

    //! Supprime les noeuds ne vérifiant pas le prédicat (bool predicat(const & T)) ainsi que leurs descendances.
    template<class U> const Tree<T> & elagageData(U predicat)
    {
        elagage([&predicat](TreeItem<T> * node)->bool{return predicat(node->data());});
        return *this;
    }

    //! Supprime les feuilles ne vérifiant pas le prédicat (bool predicat(TreeItem<T> *)) ainsi que leurs descendances.
    template<class U> const Tree<T> & elagageFeuille(U predicat);

    //! Supprime les noeuds ne vérifiant pas le prédicat (bool predicat(const & T)) ainsi que leurs descendances.
    template<class U> const Tree<T> & elagageFeuilleData(U predicat)
    {
        elagageFeuille([&predicat](TreeItem<T> * node)->bool{return predicat(node->data());});
        return *this;
    }

    //! Crée un itérateur initialisé sur le noeud virtuel nul. Cette fonction permet la compatibilité avec les algorithmes standards.
    //! Utiliser de préférence la méthode isNull directement sur le pointeur.
    typename TreeItem<T>::iterator end() const noexcept
        {return nullptr;}

    //! Créer une nouveau de valeur T est l'insert à l'indice position dans la liste des descendants directs du noeud courant.
    void insert(const int position, const T & data)
    {
        if(m_i)
            (*m_i)->insert(position, data);
        else
            throw std::runtime_error(QString("void Tree<T>::insert(const int, const T &): Itérateur interne invalide.").toStdString());
    }

    //! Insert une copie de tree à l'indice position dans la liste des descendants directs du noeud courant.
    void insert(const int position, const Tree<T> & tree)
    {
        if(m_i)
            (*m_i)->insert(position, new TreeItem<T>(tree.root()));
        else
            throw std::runtime_error(QString("void Tree<T>::insert(const int, const T &): Itérateur interne invalide.").toStdString());
    }

    //! Insert tree à l'indice position dans la liste des descendants directs du noeud courant.
    void insert(const int position, Tree<T> && tree)
    {
        if(m_i)
        {
            (*m_i)->insert(position, tree.root());
            tree.rootNull();
        }
        else
            throw std::runtime_error(QString("void Tree<T>::insert(const int, T &&): Itérateur interne invalide.").toStdString());
    }

    //! Replace l'itérateur sur le noeud suivant (méthode de parcours incrémentation-décrémentation)
    //! puis test si le nouveau noeud n'est pas la racine.
    bool next() const noexcept
    {
        ++m_i;
        return !m_i.isNull();
    }

    //! Déplace l'itérateur sur le frère suivant et retourne true sauf si l'itérateur est invalide ou un dernier né.
    bool nextBrother() noexcept
    {
        m_i.nextBrother;
        return m_i && (*m_i)->lastChild();
    }

    //! Renvoie un arbre contenant seulement une copie de la racine et ses enfants directs.
    Tree<T> static parentWithChilds(TreeItem<T> * parent)
    {
        Tree<T> tree(parent->data());
        if(parent->hasChild())
        {
            for(typename TreeItem<T>::brotherIterator i = parent->firstChild(); i.isNull(); ++i)
                tree.root()->addChild((*i)->data());
        }
        return tree;
    }

    //! Replace l'itérateur sur le noeud précédent (méthode de parcours suivant-précédent)
    //! puis test si le nouveau noeud n'est pas la racine.
    bool precedent() const noexcept
    {
        m_i.precedent();
        return !m_i.isRoot();
    }

    //! Replace l'itérateur sur le noeud précédent (méthode de parcours incrémentation-décrémentation)
    //! puis test si le nouveau noeud n'est pas la racine.
    bool previous() const noexcept
    {
        --m_i;
        return !m_i.isNull();
    }

    //! Déplace l'itérateur sur le frère suivant et retourne true sauf si l'itérateur est invalide ou un dernier né.
    bool prevBrother() noexcept
    {
        m_i.prevBrother;
        return m_i && (*m_i)->firstChild();
    }

    //! Renvoie un pointeur constant sur la racine de l'arbre.
    TreeItem<T> * root() const noexcept
        {return m_root;}

    /*! \brief Replace le noeud courant sur le noeud de chemin d'indices list.
     *
     * Replace le noeud courant sur le noeud de chemin d'indices list. Par défaut ce chemin est absolu,
     * mettre root=false si le chemin est relatif.
     *
     * Si les indices du chemin ne sont pas valides, l'itérateur interne sera invalide.
     */
    Tree<T> & seek(const QList<int> & list, bool root = true) const;

    /*! \brief Replace le noeud courant sur le noeud de chemin de données list.
     *
     * Replace le noeud courant sur le noeud de chemin de données list. Par défaut ce chemin est absolu,
     * mettre root=false si le chemin est relatif.
     *
     * Si l'une des donnée de la liste n'existe pas, l'itérateur interne sera invalide.
     */
    Tree<T> & seek(const QList<T> & list, bool root = true) const;

    //! Modifie la donnée associée au noeuds courant.
    void setData(const T & data)
    {
        if(m_i)
            (*m_i)->setData(data);
        else
            throw std::runtime_error(QString("void Tree<T>::setData(const T &): Itérateur interne invalide.").toStdString());
    }

    //! Renvoie le sens de parcours (méthode suivant) du noeud courant: true -> descente; false -> remontée.
    bool sens() const noexcept
        {return m_i.sens();}

    //! Modifie le sens de parcours de la méthode suivant-précédent.
    void setSens(bool sens) const noexcept
        {m_i.setSens(sens);}

    //! Replace l'itérateur sur le noeud suivant (méthode de parcours suivant-précédent)
    //! puis test si le nouveau noeud n'est pas la racine.
    bool suivant() const noexcept
    {
        m_i.suivant();
        return !m_i.isRoot();
    }

    //! Replace le noeud courant au niveau du parent du précédent noeud courant.
    void toParent() const noexcept
        {m_i.toParent();}

    /*! \brief Renvoie la valeur de la donnée contenue par le noeud de chemin d'indices list.
     *
     * Renvoie la valeur de la donnée contenue par le noeud de chemin d'indices list. Par défaut ce chemin est absolu,
     * mettre root=false si le chemin est relatif. Cette méthode ne déplace pas le noeud courant.
     * Par defaut si les indices du chemin ne sont pas valides,
     * de nouveaux noeuds sont créés, mettre newvalue=false pour qu'un execption soit lancée.
     */
    const T & value(const QList<int> & list, bool root = true, bool newvalue = true) const;

    //! Affectation par recopie.
    Tree<T> & operator = (const Tree<T> & tree)
    {
        delete m_root;
        m_root = new TreeItem<T>(*tree.m_root);
        m_i = tree.m_i;
        return *this;
    }

    //! Affectation par deplacement.
    Tree<T> & operator = (Tree<T> && tree)
    {
        delete m_root;
        m_root = tree.m_root;
        tree.m_root = nullptr;
        m_i = tree.m_i;
        return *this;
    }

    //! Transmet une réfrence sur la donnée associé au neud courant.
    T & operator * ()
    {
        if(m_i)
            return (*m_i)->modifData();
        else
            throw std::runtime_error(QString("T& Tree<T>::oterateur*(): Itérateur interne invalide.").toStdString());
    }

    //! Ajoute une copie de Tree et de ses descendant aux descendants directs du noeud courant sans modifier ce dernier.
    Tree<T> & operator << (const Tree<T> & tree)
    {
        if(m_i)
        {
            (**m_i) << &tree.root();
            return *this;
        }
        else
            throw std::runtime_error(QString("Tree<T> & Tree<T>::oterateur<<(const Tree<T>): Itérateur interne invalide.").toStdString());
    }

    //! Ajoute Tree et de ses descendant aux descendants directs du noeud courant sans modifier ce dernier.
    Tree<T> & operator << (Tree<T> && tree)
    {
        if(m_i)
        {
            (**m_i) << tree.root();
            tree.rootNull();
            return *this;
        }
        else
            throw std::runtime_error(QString("Tree<T> & Tree<T>::oterateur<<(const Tree<T>): Itérateur interne invalide.").toStdString());
    }

    //! Crée un nouveau descendant direct au noeud courant, ce nouveau noeud à pour donnée associée data (voir la méthode addchild).
    //! Puis replace le noeud courant sur ce nouveau noeud avant de renvoyer un réfrence de l'arbre.
    Tree<T> & operator << (const T & data)
    {
        if(m_i)
        {
            m_i = &((**m_i)<<data);
            return *this;
        }
        else
            throw std::runtime_error(QString("Tree<T> & Tree<T>::oterateur<<(const T &): Itérateur interne invalide.").toStdString());
    }

    //! Affecte à data la donnée associée au noeud courant puis replace ce dernier sur le noeud suivant (pour l'incrémentation).
    const Tree<T> & operator >> ( T & data) const
    {
        if(m_i)
        {
            data = (*m_i)->data();
            ++m_i;
            return *this;
        }
        else
            throw std::runtime_error(QString("Tree<T> & Tree<T>::oterateur<<(const Tree<T>) const: Itérateur interne invalide.").toStdString());
    }

    //! Transmet une référence sur la donnée du noeud de chemin absolue d'indices list.
    //! Cette méthode ne déplace pas le noeud courant. Alias de value(list,true,true)
    T & operator [](const QList<int> & list)
        {return value(list, true, true);}

protected:
    //! Met la racine à zero.
    void rootNull() noexcept
        {m_root = nullptr;}
};

template<class T> template <class U> const Tree<T> & Tree<T>::elagage(U predicat)
{
    begin();
    while(m_i)
    {
        if(m_i.isRoot() || predicat(*m_i))
            m_i.suivant();
        else
        {
            TreeItem<T> * node = *m_i;
            m_i.setSens(false);
            m_i.suivant();
            delete node;
        }
    }
    return *this;
}

template<class T> template <class U> const Tree<T> & Tree<T>::elagageFeuille(U predicat)
{
    begin();
    while(m_i)
    {
        if(m_i.hasChild())
            m_i.toFirstLeaf();
        else
        {
            if(m_i.isRoot() || predicat(*m_i))
                ++m_i;
            else
            {
                auto node = *m_i;
                m_i.toParent();
                delete node;
            }
        }
    }
    return *this;
}

template<class T> Tree<T> & Tree<T>::seek(const QList<int> & list, bool root) const
{
    if(root)
        begin();
    m_i.seek(list, false);
    return *this;
}

template<class T> Tree<T> & Tree<T>::seek(const QList<T> & list, bool root) const
{
    if(root)
        begin();
    m_i.seek(list, false);
    return *this;
}

template<class T> const T & Tree<T>::value(const QList<int> & list, bool root, bool newvalue) const
{
    auto sav_m_i =m_i;
    if(root)
        begin();
    else if(!m_i)
        throw std::runtime_error(QString("const T & Tree<T>::value(const QList<int> &, bool, bool) const: Itérateur interne invalide.")
                                 .toStdString());
    if(!newvalue)
    {
        m_i.seek(list, false);
        if(!m_i)
            throw std::invalid_argument(QString("const T & Tree<T>::value(const QList<int> &, bool, bool) const: Chemin de noeuds invalide.")
                                     .toStdString());
    }
    else
    {
        for(auto i = list.cbegin(); i != list.cend(); ++i)
        {
            if(m_i.hasChild())
            {
                if(*i >= 0)
                {
                    ++m_i;
                    auto j = *i;
                    while ((*m_i)->nextBrother() && j)
                    {
                        m_i = (*m_i)->nextBrother();
                        --j;
                    }
                    if(j)
                        m_i = (*m_i)->parent()->addChild(j);
                }
                else
                {
                    m_i = (*m_i)->lastChild();
                    auto j = *i + 1;
                    while((*m_i)->prevBrother() && j)
                    {
                        m_i = (*m_i)->prevBrother();
                        ++j;
                    }
                    if(j)
                    {
                        m_i.toParent();
                        while (j)
                        {
                            (*m_i)->prepend(new TreeItem<T>);
                            ++j;
                        }
                        ++m_i;
                    }
                }
            }
            else
            {
                if(*i >= 0)
                    m_i = (*m_i)->addChild(*i + 1);
                else
                {
                    (*m_i)->addChild(-(*i));
                    ++m_i;
                }
            }
        }
    }
    auto node = *m_i;
    m_i = sav_m_i;
    return node->data();
}

#endif // Tree_H
