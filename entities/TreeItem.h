/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */

//! \defgroup groupeTree Les Arbres
//! \brief Cette partie regroupe l'implémentation des arbres.

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QStack>
#include <stdexcept>

/*! \ingroup groupeTree
 * \brief Cette classe est une représentation des noeuds d'un arbre.
 *
 * Cette classe représente les noeuds d'un arbre contenant les informations suivantes:
 *  - une donnée de type T attachée au noeud,
 *  - un pointeur sur le TreeItem parent dans l'arbre (l'élément racine posséde un pointeur parent nul),
 *  - un pointeur sur le frère précécent (le premier fils posséde un pointeur frère précédent nul).
 *  - un pointeur sur le frère suivant (le dernier fils posséde un pointeur frère suivant nul).
 *  - un pointeur sur le première fils (une feuille posséde un pointeur premier fils nul).
 *  - un pointeur sur le dernier fils (une feuille posséde un pointeur dernier fils nul).
 *
 * L'itérateur TreeItem::brotherIterator permet de parcourir une fratrie.
 * L'itérateur TreeItem::iterator propose différente façon de parcourir les noeuds d'un arbre de TreeItem.
 *
 * \sa Tree
 */
template<class T> class TreeItem
{
public:

    /*! \ingroup groupeTree
     * \brief Itérateur sur une fratrie.
     */
    class brotherIterator
    {
    protected:
        TreeItem<T> *m_ptr;     //!< Pointeur sur le frère courant.

    public:
        //! constructeur.
        brotherIterator(TreeItem<T> * node = nullptr) noexcept
            : m_ptr(node) {}

        //! constructeur de recopie.
        brotherIterator(const brotherIterator &) = default;

        //! Teste si l'itérateur pointe vers le noeud null.
        bool isNull() const noexcept
            {return !m_ptr;}

        //! Renvoie un pointeur sur le noeud courant.
        TreeItem<T> * operator *() const noexcept
            {return m_ptr;}

        //! Opérateur de pré-incrémentation.
        brotherIterator & operator ++() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->m_nextBrother;
            return *this;
        }

        //! Opérateur de post-incrémentation.
        brotherIterator operator ++(int) noexcept
        {
            brotherIterator it = *this;
            operator ++();
            return it;
        }

        //! Opérateur de pré-décrémentation.
        brotherIterator & operator --() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->m_prevBrother;
            return *this;
        }

        //! Opérateur de post-décrémentation.
        brotherIterator operator --(int) noexcept
        {
            brotherIterator it = *this;
            operator --();
            return it;
        }

        //! Opérateur d'affectation.
        brotherIterator & operator =(const brotherIterator & i) = default;

        //! Opérateur de conversion à partir d'un pointeur sur un noeud de l'arbre.
        brotherIterator & operator =(TreeItem<T> * node) noexcept
        {
            m_ptr = node;
            return *this;
        }

        //! Test d'égalité entre deux itérateurs.
        bool operator ==(const brotherIterator & i) const noexcept
            {return m_ptr == i.m_ptr;}

        //! Test de différence entre deux itérateurs.
        bool operator !=(const brotherIterator & i) const noexcept
            {return m_ptr != i.m_ptr;}

        //! Test si la position courante de l'itérateur correspond à un pointeur sur le noeud.
        bool operator ==(TreeItem<T> * node) const noexcept
            {return m_ptr == node;}

        //! Test si la position courante de l'itérateur est différente du pointeur sur le noeud.
        bool operator !=(TreeItem<T> * node) const noexcept
            {return m_ptr != node;}

        //! Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        brotherIterator & operator +=(int n) noexcept;

        //! Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
        brotherIterator & operator -=(int n) noexcept
            {return operator +=(-n);}

        //! L'addition d'un itérateur i et d'un entier n produit une copie de l'itérateur i incrémenté n fois.
        brotherIterator operator +(int n) const noexcept
        {
            brotherIterator i(*this);
            return i += n;
        }

        //! La soustraction d'un itérateur i et d'un entier n produit une copie de l'itérateur i décrémenté n fois.
        brotherIterator operator -(int n) const noexcept
        {
            brotherIterator i(*this);
            return i -= n;
        }

        //! Conversion en booléen.
        operator bool() const noexcept
            {return m_ptr;}

        //! Place l'itérateur sur le premier frère de la fratrie.
        brotherIterator & toFirstBrother() noexcept
        {
            if(m_ptr && m_ptr->m_parent)
                m_ptr = m_ptr->m_parent->m_firstChild;
            return *this;
        }

        //! Place l'itérateur sur le dernier frère de la fratrie.
        brotherIterator & toLastBrother() noexcept
        {
            if(m_ptr && m_ptr->m_parent)
                m_ptr = m_ptr->m_parent->m_lastChild;
            return *this;
        }
    };

    /*! \ingroup groupeTree
     * \brief Itérateur sur un arbre de TreeItem proposant différente façon de parcourir les noeuds.
     *
     * Cet itérateur propose deux méthodes pour parcourir les noeuds de l'arbre.
     *  + La méthode incrémentation-décrémentation.
     *  + La méthode suivant-précédent.
     *
     * Exemple:
     */
     /* \latexonly
     * Un exemple d'arbre de racine a {
     *  node [shape=record, fontname=Helvetica, fontsize=10];
     *  a [ label="a"];
     *  b [ label="b"];
     *  c [ label="c"];
     *  d [ label="d"];
     *  e [ label="e"];
     *  f [ label="f"];
     *  a -> b [ arrowhead="open", style="dashed" ];
     *  b -> c [ arrowhead="open", style="dashed" ];
     *  b -> d [ arrowhead="open", style="dashed" ];
     *  a -> d [ arrowhead="open", style="dashed" ];
     *  d -> f [ arrowhead="open", style="dashed" ];
     * }
     * \endlatexonly
     *                 1
     *              |       |
     *              2       5
     *          |       |   |
     *          3       4   6
     */
    class iterator
    {
    protected:
        TreeItem<T> *m_ptr;     //!< Pointeur vers la position courante de l'itérateur.
        bool m_sens;            //!< Indicateur du sens de parcours: true -> descente; false -> remontée.

    public:
        //! Constructeur de conversion à partir d'un pointeur sur un noeud.
        iterator(TreeItem<T> * node = nullptr) noexcept
            : m_ptr(node), m_sens(true)
            {}

        //! Constructeur de recopie.
        iterator(const iterator & i) noexcept
            : m_ptr(i.m_ptr), m_sens(i.m_sens)
            {}

        //! Test si l'itérateur pointe vers une feuille de l'arbre.
        bool hasChild() const noexcept
            {return m_ptr && m_ptr->m_firstChild;}

        //! Détermine la position du noeud courant par rapport à la racine.
        QList<int> index() const;

        /*! \brief Test si le noeud courant de l'itérateur est le noeud virtuel nul.
         * Cette méthode est plus éfficace que le test "itérateur == tree.end()" pour tester la fin de parcours.
         *
         * L'itérateur est placé sur la position virtuelle nullptr,
         * si l'itérateur n'est pas initialisé ou si l'on parcourt l'arbre au delà du dernier noeuds ou avant la racine.
         */
        bool isNull() const noexcept
            {return !m_ptr;}

        //! Teste si le neud courant est le sommet de l'arbre.
        bool isRoot() const noexcept
            {return m_ptr && !m_ptr->m_parent;}

        //! Déplace l'itérateur sur le frère suivant.
        iterator & nextBrother() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->m_nextBrother;
            return *this;
        }

        //! Conversion en booléen.
        operator bool() const noexcept
            {return m_ptr;}

        //! Opérateur d'affectation.
        iterator & operator =(const iterator & i) noexcept
        {
            m_ptr = i.m_ptr;
            m_sens = i.m_sens;
            return *this;
        }

        //! Opérateur de conversion à partir d'un pointeur sur un noeud de l'arbre.
        iterator & operator =(TreeItem<T> * node) noexcept
        {
            m_ptr = node;
            m_sens = true;
            return *this;
        }

        //! Opératateur de pré-incrémentation.
        //! Positionne l'itérateur sur le noeud suivant, les noeuds sont parcourus uniquement à la descente.
        iterator & operator ++() noexcept;

        //! Opérateur de post-incrémentation.
        iterator operator ++(int) noexcept
        {
            iterator i = *this;
            ++(*this);
            return i;
        }

        //! Opérateur de pré-décrémentation.
        iterator & operator --() noexcept;

        //! Opérateur de post-décrémentation.
        iterator operator --(int) noexcept
        {
            iterator i(*this);
            --(*this);
            return i;
        }

        //! Test d'égalité entre deux itérateurs.
        bool operator ==(const iterator & i) const noexcept
            {return m_ptr == i.m_ptr;}

        //! Test de différence entre deux itérateurs.
        bool operator !=(const iterator & i) const noexcept
            {return m_ptr != i.m_ptr;}

        //! Test si la position courante de l'itérateur correspond à un pointeur sur le noeud.
        bool operator ==(TreeItem<T> * node) const noexcept
            {return m_ptr == node;}

        //! Test si la position courante de l'itérateur est différente du pointeur sur le noeud.
        bool operator !=(TreeItem<T> * node) const noexcept
            {return m_ptr != node;}

        /*! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
         * Pour un n grand cet opérateur est plus efficace que d'appliquer n fois l'opérateur d'incrémemtation.
         */
        iterator & operator +=(int n) noexcept;

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         * Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         * l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         * a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        iterator & operator -=(int n) noexcept;

        //! L'addition d'un itérateur i et d'un entier n produit une copie de l'itérateur i incrémenté n fois.
        iterator operator +(int n) const noexcept
        {
            iterator i(*this);
            return i += n;
        }

        //! La soustraction d'un itérateur i et d'un entier n produit une copie de l'itérateur i décrémenté n fois.
        iterator operator -(int n) const noexcept
        {
            iterator i(*this);
            return i -= n;
        }

        //! Renvoie un pointeur sur le noeud courant.
        TreeItem<T> * operator *() const noexcept
            {return m_ptr;}

        //! Positionne l'itérateur sur le noeud correspond au chemin transmis en argument.
        /*! La postion souhaitée de l'itérateur est transmise par la liste des noeuds qu'il faut parcourir. Ces noeuds sont référencés
         * par leurs indices dans la liste des descendants directs du noeuds précédemment atteint. Le chemin contenue dans la liste
         * débute à la racine de l'arbre.
         */
        iterator & operator [](const QList<int> & list);

        //! Positionne l'itérateur sur le noeud précédent dans le parcours de type suivant-précédent:
        //! les noeuds sont parcours deux fois, à la descente et à la remonté.
        iterator & precedent() noexcept;

        //! Déplace l'itérateur sur le frère précédent.
        iterator & previousBrother() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->m_prevBrother;
            return *this;
        }

        /*! \brief Positionne l'itérateur sur le noeud de chemin d'indices list.
         *
         * Positionne l'itérateur sur le noeud de chemin des indices list des différents descendants empruntés.
         * Par défaut le chemin est relatif, mettre root = true si le chemin absolue (par rapport à la racine).
         * Si l'un des indices de la liste n'est pas valide, l'itérateur renjoyé sera nul.
         */
        iterator & seek(const QList<int> & list, bool root = false);

        /*! \brief Positionne l'itérateur sur le noeud de chemin de données list.
         *
         * Positionne l'itérateur sur le noeud de chemin des données list des différents descendants empruntés.
         * Si plusieurs noeuds frères possèdent les mêmes données c'est celui de plus faible indice qui sera emprunté.
         * Par défaut le chemin est relatif, mettre root = true si le chemin absolue (par rapport à la racine).
         * Si l'une des donnée de la liste n'existe pas, l'itérateur renjoyé sera nul.
         */
        iterator & seek(const QList<T> & list, bool root = true);

        //! Renvoie le sens de parcours (méthode suivant) de l'itérateur: true -> descente; false -> remontée.
        bool sens() const noexcept
            {return m_sens;}

        //! Modifie le sens de parcours de la méthode suivant-précédent.
        void setSens(bool sens) noexcept
            {m_sens = sens;}

        //! Positionne l'itérateur sur le noeud suivant dans le parcours de type suivant-précédent:
        //! les noeuds sont parcourus deux fois, à la descente et à la remontée.
        iterator & suivant() noexcept;

        //! Place l'itérateur sur le dernier des descendants de la ligné des ainés du noeud courant.
        iterator & toFirstLeaf() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->firstLeaf();
            return *this;
        }

        //! Place l'itérateur sur le dernier des descendants de la ligné des cadets du noeud courant.
        iterator & toLastLeaf() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->lastLeaf();
            return *this;
        }

        /*! \brief Positionne l'itérateur sur le n ieme frère suivant.
         *
         * Positionne l'itérateur sur le n ieme frère suivant. L'argument n peut être négatif.
         * Si n est trop négatif, l'itérateur est positionné sur le premier des frères
         * et si n est trop grand, l'itérateur est positionné sur le dernier des frères.
         */
        iterator & toNextBrother(int n = 1) noexcept;

        /*! \brief Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         * Si n est négatif, l'itérateur est positionné sur le n ième premier descendant direct.
         *
         * Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         * Si n est plus grand que le nombre de parents l'itérateur est positionné sur le noeud virtuel nul.
         * Si n est négatif, l'itérateur est positionné sur le n ième premier descendant direct tant qu'il y a des descendants.
         */
        iterator & toParent(int n = 1) noexcept;

        //! Place l'itérateur à la racine de l'arbre.
        iterator & toRoot() noexcept
        {
            if(m_ptr)
                m_ptr = m_ptr->root();
            return *this;
        }

    protected:
        //! Positionne l'itérateur sur le frère suivant du noeud courant.
        //! Si le noeud courant est le dernier des frères,
        //! on remonte jusqu'au premier parent possédent un cadet et l'on positionne l'itérateur dessus.
        //! Si le noeud courant est le tout dernier des descendants de l'arbre, l'itérateur est placé sur le noeud virtuel nul.
        void next() noexcept;
    };

    friend class brotherIterator;
    friend class iterator;

protected:
    TreeItem<T> *m_parent = nullptr;        //!< Pointeur vers le TreeItem parent dans l'arbre, est nul si le TreeItem est la racine de l'arbre.
    TreeItem<T> *m_prevBrother = nullptr;   //!< Pointeur vers le frère précédent, est nul si le TreeItem est le premier frère.
    TreeItem<T> *m_nextBrother = nullptr;   //!< Pointeur vers le frère suivant, est nul si le TreeItem est le dernier frère.
    TreeItem<T> *m_firstChild = nullptr;    //!< Pointeur vers le premier fils, est nul si le noeud est une feuille.
    TreeItem<T> *m_lastChild = nullptr;     //!< Pointeur vers le dernier fils, est nul si le noeud est une feuille..
    T m_data;                               //!< Donné accroché au noeud de l'arbre.

public:
    //! Constructeur par default;
    TreeItem() = default;

    //! Constructeur sans donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
    TreeItem(TreeItem<T> * parent) noexcept(noexcept(T()))
    {
        if(parent)
            parent->append(this);
    }

    //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants.
    TreeItem(const TreeItem<T> & tree);

    //! Constructeur par déplacement.
    TreeItem(TreeItem<T> && tree) noexcept(noexcept (T(std::move(tree.m_data))));

    //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
    TreeItem(const T & data) noexcept(noexcept(T(data)))
        : m_data(data)
    {}

    //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
    TreeItem(const T & data, TreeItem * parent) noexcept(noexcept(T(data)))
        : TreeItem(data)
    {
        if(parent)
            parent->append(this);
    }

    //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants, T doit posséder un constructeur à partie de U.
    template<class U> TreeItem(const TreeItem<U> & tree);

    //! Constructeur à partir d'un TreeItem de donnée U et une usine construisant une donnée T grâce à une donnée U.
    template<class U, class F> TreeItem(const TreeItem<U> & tree, F & usine);

    //! Destructeur. Détruit également récursivement l'ensemble des descendants.
    ~TreeItem();

    //! Ajoute nbr descendants directs au noeud et renvoie un pointeur sur le dernier descendant créé.
    TreeItem<T> * addChild(int nbr = 1);

    //! Ajoute un descendant de donnée associée data, puis renvoie un pointeur sur ce nouveau noeud.
    TreeItem<T> * addChild(const T & data)
        {return new TreeItem(data,this);}

    /*!
     * \brief Ajoute le noeud pointé par child aux descendants du noeud.
     *
     * Le noeud pointé par child est ajouté en dernière position dans la fratrie des descendants directs.
     * Si le descendant ajouté avait déjà un parent,
     * ce descendant est alors supprimé de la liste des descendants directs
     * de ce parent avant que ne soit changé le pointeur sur le parent du noeud ajouté.
     */
    void append(TreeItem<T> *child) noexcept;

    //! Ajout un dernier descendant.
    void append()
        {append(new TreeItem<T>);}

    //! Crée un itérateur initialisé sur le noeud.
    iterator begin() const noexcept
        {return this;}

    //! Crée un itérateur de frères initialisé sur le premier frère.
    brotherIterator beginBrother() const noexcept
        {return firstBrother();}

    //! Crée un itérateur de frère initialisé sur le premier fils.
    brotherIterator beginChild() const noexcept
        {return m_firstChild;}

    //! Crée un itérateur initialisé sur la racine.
    iterator beginRoot() const noexcept
        {return iterator(root());}

    //! Renvoie un pointeur sur le frère d'indice position.
    //! Les indices négatifs sont permis.
    //! Si position n'appartient pas à l'intervale [-nombre de frères, nombre de frères-1], le pointeur renvoyé sera nul.
    TreeItem<T> * brother(int position) const noexcept;

    //! Renvoie un pointeur sur le descendant direct d'indice position.
    //! Les indices négatifs sont permis.
    //! Si position n'appartient pas à l'intervale [-nombre de fils, nombre de fils-1], le pointeur renvoyé sera nul.
    TreeItem<T> * child(int position) const noexcept;

    //! Renvoie le nombre de descendants directs.
    int childCount() const noexcept;

    //! Renvoie la liste des pointerurs des descendants directs.
    QList<TreeItem<T>*> childs() const;

    //! Transmet une référence constante sur la donnée associée au noeud.
    const T & data() const noexcept
        {return m_data;}

    //! Crée un itérateur initialisé sur le noeud virtuel nul. Cette fonction permet la compatibilité avec les algorithmes standards.
    //! Utiliser de préférence la méthode isNull directement sur le pointeur ou la conversion de l'itérateur en booléen.
    iterator end() const noexcept
        {return nullptr;}

    //! Crée un itérateur de frère initialisé sur le noeud virtuel nul.
    //! Cette fonction permet la compatibilité avec les algorithmes standards.
    //! Utiliser de préférence la méthode isNull directement sur le pointeur.
    brotherIterator endBrother() const noexcept
        {return nullptr;}

    //! Renvoie un pointeur sur le premier frère.
    TreeItem<T> * firstBrother() const noexcept
        {return m_parent ? m_parent->m_firstChild : this;}

    //! Renvoie un pointeur sur le premier descendant direct.
    TreeItem<T> * firstChild() const noexcept
        {return m_firstChild;}

    //! Renvoie un pointeur sur la feuille correspondant aux descendants ainés.
    TreeItem<T> * firstLeaf() /*const*/ noexcept
    {
        if(m_firstChild)
        {
            auto node = m_firstChild;
            while(node->m_firstChild)
                node = node->m_firstChild;
            return node;
        }
        else
            return this;//pointeur const -> no const
    }

    //! Test si le noeud posséde un descendant direct.
    bool hasChild() const noexcept
        {return m_firstChild;}

    //! Insert le noeud pointé par child à l'indice position parmi les descendants directs,
    //! si position est négatif ou supérieur ou égale au nombre de descendants directs,
    //! le noeud pointé par child est ajouté en derniere position.
    void insert(int position, TreeItem<T> *child) noexcept;

    //! Créer une nouveau noeud de valeur T et l'insert à l'indice position dans la liste des descendants directs.
    void insert(int position, const T & data)
        {insert(position, new TreeItem<T>(data));}

    //! Insert un cadet.
    void insertBrother(TreeItem<T> *cadet) noexcept;

    //! Créé et insert un cadet de donnée data.
    void insertBrother(const T & data)
        {insertBrother(new TreeItem<T>(data));}

    //! Test si le noeud est le première enfant.
    bool isFirstChild() const noexcept
        {return !m_prevBrother;}

    //! Test si le noeud est le première enfant.
    bool isLastChild() const noexcept
        {return !m_nextBrother;}

    //! Test si le noeud est la racine de l'arbre.
    bool isRoot() const noexcept
        {return !m_parent;}

    //! Renvoie un pointeur sur le dernier frère.
    TreeItem<T> * lastBrother() const noexcept
        {return m_parent ? m_parent->m_lastChild : this;}

    //! Renvoie un pointeur sur le dernier descendant direct.
    TreeItem<T> * lastChild() const noexcept
        {return m_lastChild;}

    //! Renvoie un pointeur sur le dernier des descendants de la ligné des derniers né du noeud.
    TreeItem<T> * lastLeaf() const noexcept
    {
        if(m_lastChild)
        {
            auto node = m_lastChild;
            while(node->m_lastChild)
                node = node->m_lastChild;
            return node;
        }
        else
            return this;
    }

    //! Transmet une référence sur la donnée associée au noeud. La donnée pouvant être modifiée.
    T & modifData() noexcept
        {return m_data;}

    /*! \brief Déplace le descendant direct d'indice from à l'indice to.
     *
     * Déplace le descendant direct d'indice from à l'indice to.
     * Une exception est lancée si l'un des deux indices from ou to n'est pas valide.
     */
    void move(int from, int to);

    //! Renvoie un pointeur sur le frère suivant, celui-ci est nul si le noeud le dernier des fils.
    TreeItem<T> * nextBrother() const noexcept
        {return m_nextBrother;}

    //! Renvoie un pointeur sur le parent du noeud ou 0 si le noeud est la racine.
    TreeItem<T> * parent() const noexcept
        {return m_parent;}

    //! Renvoie l'indice du noeud parmis les descendants directs de son parent et 0 si le noeud est la racine.
    int position() const noexcept;

    //! Ajoute un fils en première position.
    void prepend(TreeItem<T> * child) noexcept;

    //! Ajoute un fils en première position.
    void prepend()
        {prepend(new TreeItem<T>);}

    //! Crée et ajoute un fils de donnée data en première position.
    void prepend(const T & data)
        {prepend(new TreeItem(data));}

    //! Détruit tous les descendants du noeud.
    void removeAllChild() noexcept;

     /*! \brief Retire le descendant d'indice position de la liste des descendants directs.
      *
      * Retire le descendant d'indice position de la liste des descendants directs.
      * Ce descendant est détruit ainsi que tous ses descedants.
      */
    void removeChild(int position) noexcept;

    /*! \brief Retire les descendants d'indices position à position+nbr-1 de la liste des descendants directs.
     *
     * Retire les descendants d'indices position à position+nbr-1 de la liste des descendants directs.
     * Ces descendants sont détruits ainsi que tous leurs descedants.
     */
    void removeChild(int position, int nbr) noexcept;

    //! Renvoie un pointeur sur la racine de l'arbre.
    TreeItem<T> * root() const noexcept;

    //! Modifie la donnée associée au noeud.
    void setData(const T & data) noexcept(noexcept (T::operateur = (data)))
        {m_data = data;}

    /*! \brief Renvoie le nombre de noeuds du sous-arbre ayant ce noeud pour racine.
     *
     * Méthode déterminant le nombre de noeuds du sous-arbre ayant ce noeud pour racine,
     * c'est-à-dire le nombre de descendants directs et indirects augmenté de un pour prendre en compte ce noeud.
     */
    int size() const noexcept;

    /*!
     * \brief Opérateur d'affectation de recopie.
     *
     * L'opérateur d'affectation modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds
     * pour les remplacer par une copie de ceux de tree.
     */
    TreeItem<T> & operator = (const TreeItem<T> & tree);

    /*!
     * \brief Opérateur d'affectation par déplacement.
     *
     * L'opérateur d'affectation par déplacement, modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds,
     * change en tree racine, puis récupére les liens de descendance de tree avant de les supprimer de tree.
     */
    TreeItem<T> & operator = (TreeItem<T> && tree) noexcept(noexcept (T(std::move(tree.m_data))));

    //! Ajoute une copie de tree et de ses descendants, aux descendants du noeud. Puis renvoie une référence de ce nouveau noeud.
    TreeItem<T> & operator << (const TreeItem<T> & tree)
    {
        TreeItem * child = new TreeItem<T>(tree);
        append(child);
        return *child;
    }

    //! Ajoute tree aux descendants du noeud en rattachant tree et ses descendants au noeud. Puis renvoie une référence de *tree.
    TreeItem<T> & operator << (TreeItem<T> * tree) noexcept
    {
        append(tree);
        return *tree;
    }

    //! Ajoute au noeud un descendant de donnée associée data.
    TreeItem<T> & operator << (const T & data) noexcept(noexcept(T(data)))
        {return *(addChild(data));}

protected:
    //! Renvoie un pointeur sur le frère d'indice position (n'est pas optimisé pour la racine).
    TreeItem<T> * brotherNoRoot(int position) const noexcept;

    //! Transforme le noeud d'un abre en racine.
    void becomeRoot() noexcept;

    //! Change l'hérédité du noeud en le supprimant des descendants de son noeud parent
    //! avant de remplacer le parent du noeud par le nouveau parent.
    void changeHeredite(TreeItem<T> *parent) noexcept;
};

template<class T> TreeItem<T>::TreeItem(const TreeItem<T> & tree)
    : m_data(tree.m_data)
{
    if(tree.hasChild())
    {
        for(brotherIterator i = tree.m_firstChild; i; ++i)
            append(new TreeItem<T>(**i));
    }
}

template<class T> TreeItem<T>::TreeItem(TreeItem<T> && tree) noexcept(noexcept (T(std::move(tree.m_data))))
    : m_firstChild(tree.m_firstChild),
      m_lastChild(tree.m_lastChild),
      m_data(std::move(tree.m_data))
{
    if(tree.hasChild())
    {
        for(brotherIterator i = tree.m_firstChild; i; ++i)
            (*i)->m_parent = this;
    }
    tree.becomeRoot();
    tree.m_firstChild = tree.m_lastChild = nullptr;
}

template<class T> template<class U> TreeItem<T>::TreeItem(const TreeItem<U> & tree)
    : m_data(tree.data())
{
    if(hasChild())
    {
        for(typename TreeItem<U>::brotherIterator i = tree.firstChild(); i; ++i)
            append(new TreeItem<T>(**i));
    }
}

template<class T> template<class U,class F> TreeItem<T>::TreeItem(const TreeItem<U> & tree, F & usine)
    : m_data(usine(tree.data()))
{
    if(hasChild())
    {
        for(typename TreeItem<U>::brotherIterator i = tree.firstChild(); i; ++i)
            append(new TreeItem<T>(**i,usine));
    }
}

template<class T> TreeItem<T>::~TreeItem()
{
    changeHeredite(nullptr);
    if(hasChild())
    {
        for(brotherIterator i = m_firstChild; i; ++i)
        {
            (*i)->m_parent = nullptr;
            delete *i;
        }
    }
}

template<class T> TreeItem<T> * TreeItem<T>::addChild(int nbr)
{
    for(auto i = 0; i != nbr; ++i)
        append(new TreeItem<T>);
    return m_lastChild;
}

template<class T> void TreeItem<T>::append(TreeItem<T> *child) noexcept
{
    child->changeHeredite(this);
    child->m_prevBrother = m_lastChild;
    child->m_nextBrother = nullptr;
    if(m_lastChild)
        m_lastChild = m_lastChild->m_nextBrother = child;
    else
        m_firstChild = m_lastChild = child;
}

template<class T> TreeItem<T> * TreeItem<T>::brother(int position) const noexcept
{
    if(m_parent)
    {
        if(position >= 0)
        {
            brotherIterator i(m_parent->m_firstChild);
            i += position;
            return *i;
        }
        else
        {
            brotherIterator i(m_parent->m_lastChild);
            i += position;
            return *i;
        }
    }
    return this;
}

template<class T> TreeItem<T> * TreeItem<T>::child(int position) const noexcept
{
    if(position >= 0)
    {
        brotherIterator i(m_firstChild);
        i += position;
        return *i;
    }
    else
    {
        brotherIterator i(m_lastChild);
        i += position;
        return *i;
    }
}

template<class T> int TreeItem<T>::childCount() const noexcept
{
    if(!hasChild())
        return 0;
    auto n = 1;
    auto *node = m_firstChild->m_nextBrother;
    while(node)
    {
        ++n;
        node = node->m_nextBrother;
    }
    return n;
}

template<class T> QList<TreeItem<T>*> TreeItem<T>::childs() const
{
    QList<TreeItem<T>*> nodes;
    if(hasChild())
    {
        for(brotherIterator i = m_firstChild; i; ++i)
            nodes.append(*i);
    }
    return nodes;
}

template<class T> void TreeItem<T>::insert(int position, TreeItem<T> *child) noexcept
{
    if(!position)
        prepend(child);
    else if(position > 0)
    {
        auto *node = m_firstChild;
        --position;
        while(position && node->m_nextBrother)
        {
            node = node->m_nextBrother;
            --position;
        }
        if(node->m_nextBrother)
            node->insertBrother(child);
        else
            append(child);
    }
    else
    {
        auto *node = m_lastChild;
        ++position;
        while(position && node->m_prevBrother)
        {
            node = node->m_prevBrother;
            ++position;
        }
        if(!node->m_nextBrother)
            append(child);
        else if(!node->m_prevBrother)
            prepend(child);
        else
            node->insertBrother(child);
    }

}

template<class T> void TreeItem<T>::insertBrother(TreeItem<T> *cadet) noexcept
{
    if(m_parent)
    {
        cadet->changeHeredite(m_parent);
        cadet->m_prevBrother = this;
        cadet->m_nextBrother = m_nextBrother;
        if(m_nextBrother)
            m_nextBrother->m_prevBrother = cadet;
        else
            m_parent->m_lastChild = cadet;
        m_nextBrother = cadet;
    }
    else
        prepend(cadet);
}

template<class T> TreeItem<T> & TreeItem<T>::operator = (const TreeItem<T> & tree)
{
    m_data = tree.m_data;
    removeAllChild();
    if(tree.hasChild())
    {
        for(brotherIterator i = tree.m_firstChild; i; ++i)
            append(new TreeItem<T>(**i));
    }
    return *this;
}

template<class T> TreeItem<T> & TreeItem<T>::operator = (TreeItem<T> && tree) noexcept(noexcept (T(std::move(tree.m_data))))
{
    m_data = std::move(tree.m_data);
    if(hasChild())
    {
        for(brotherIterator i = m_firstChild; i; ++i)
        {
            (*i)->m_parent = nullptr;
            delete *i;
        }
    }
    if(tree.hasChild())
    {
        for(brotherIterator i = tree.m_firstChild; i; ++i)
            (*i)->m_parent = this;
        m_firstChild = tree.m_firstChild;
        m_lastChild = tree.m_lastChild;
    }
    else
        m_firstChild = m_lastChild = nullptr;
    tree.changeHeredite(nullptr);
    //tree.m_prevBrother = tree.m_nextBrother = nullptr;
    tree.m_firstChild = tree.m_lastChild = nullptr;
}

template<class T> void TreeItem<T>::prepend(TreeItem<T> * child) noexcept
{
    child->changeHeredite(this);
    child->m_prevBrother = nullptr;
    child->m_nextBrother = m_firstChild;
    if(m_firstChild)
        m_firstChild = m_firstChild->m_prevBrother = child;
    else
        m_firstChild = m_lastChild = child;
}

template<class T> int TreeItem<T>::position() const noexcept
{
    if(m_parent)
    {
        auto * node = m_prevBrother;
        auto n = 0;
        while(node)
        {
            node = node->m_prevBrother;
            ++n;
        }
        return n;
    }
    else
        return 0;
}

template<class T> void TreeItem<T>::move(int from, int to)
{
    TreeItem<T> *nodeFrom = child(from);
    TreeItem<T> *nodeTo = child(to);
    if(nodeFrom && nodeTo)
        nodeTo->insertBrother(nodeFrom);
    else
        throw std::range_error(QString("void TreeItem<T>::move(int, int):\n"
                            "Indices invalides: from =").append(QString::number(from))
                               .append(",\n to =").append(QString::number(to))
                               .append("\net nombre de descendants=").append(QString::number(childCount())).toStdString());
}

template<class T> void TreeItem<T>::removeAllChild() noexcept
{
    if(hasChild())
    {
        for(brotherIterator i = m_firstChild; i; ++i)
        {
            (*i)->m_parent = nullptr;
            delete *i;
        }
        m_firstChild = m_lastChild = nullptr;
    }
}

template<class T> void TreeItem<T>::removeChild(int position) noexcept
{
    brotherIterator i = m_firstChild;
    i += position;
    if(i)
    {
        (*i)->changeHeredite(nullptr);
        delete *i;
    }
}

template<class T> void TreeItem<T>::removeChild(int position, int nbr) noexcept
{
    brotherIterator i = m_firstChild;
    i += position;
    while(i && nbr)
    {
        delete *(i++);
        --nbr;
    }
}

template<class T> TreeItem<T> * TreeItem<T>::root() const noexcept
{
    if(m_parent)
    {
        auto node = m_parent;
        while(node->m_parent)
            node = node->m_parent;
        return node;
    }
    else
        return this;
}

template<class T> int TreeItem<T>::size() const noexcept
{
    auto compteur = 1;
    if(hasChild())
    {
        iterator i = this;
        iterator fin = lastLeaf();
        while(i != fin)
        {
            ++compteur;
            ++i;
        }
        /*QStack<typename QList<TreeItem<T> *>::const_iterator> pile;
        pile.push(m_childs.cbegin());
        while(!pile.isEmpty())
        {
            while((*pile.top())->hasChild())
            {
                pile.push((*pile.top())->m_childs.cbegin());
                compteur += 1;
            }
            while(!pile.isEmpty() && (*pile.top())->m_parent->m_childs.cend() == ++(pile.top()))
                pile.pop();
            compteur += 1;
        }*/
    }
    return compteur;

    /*
    int c = 1;
    for(typename QList<T>::const_iterator i = m_childs.cbegin(); i != m_childs.cend(); ++i)
        c += (*i)->size();
    return c;
    */
}

// %%%%%%%%%%%%%% protected %%%%%%%%%%%%%%%

template<class T> TreeItem<T> * TreeItem<T>::brotherNoRoot(int position) const noexcept
{
    brotherIterator i(m_parent->m_firstChild);
    i += position;
    return *i;
}

template<class T> void TreeItem<T>::becomeRoot() noexcept
{
    changeHeredite(nullptr);
    m_prevBrother = m_nextBrother = nullptr;
}

template<class T> void TreeItem<T>::changeHeredite(TreeItem<T> *parent) noexcept
{
    if(m_parent)
    {
        if(m_prevBrother)
            m_prevBrother->m_nextBrother = m_nextBrother;
        else
            m_parent->m_firstChild = m_nextBrother;

        if(m_nextBrother)
            m_nextBrother->m_prevBrother = m_prevBrother;
        else
            m_parent->m_lastChild = m_prevBrother;
    }
    m_parent = parent;
}

// %%%%%%%%%%%%%% brother iterator %%%%%%%%

template<class T> typename TreeItem<T>::brotherIterator & TreeItem<T>::brotherIterator::operator +=(int n) noexcept
{
    if(n > 0)
    {
        while(n && m_ptr)
        {
            m_ptr = m_ptr->m_nextBrother;
            --n;
        }
    }
    else if(n < 0)
    {
        while(n && m_ptr)
        {
            m_ptr = m_ptr->m_prevBrother;
            ++n;
        }
    }
    return *this;
}

// %%%%%%%%%%%%%% iterator %%%%%%%%%%%%%

template<class T> QList<int> TreeItem<T>::iterator::index() const
{
    QList<int> list;
    auto * node = m_ptr;
    while(node->m_parent)
    {
        list.prepend(node->position());
        node = node->m_parent;
    }
    return list;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::operator ++() noexcept
{
    if(m_ptr)
    {
        if(m_ptr->hasChild())
            m_ptr = m_ptr->m_firstChild;
        else
            next();
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::operator --() noexcept
{
    if(m_ptr)
    {
        if(m_ptr->m_prevBrother)
            m_ptr = m_ptr->m_prevBrother->lastLeaf();
        else
            m_ptr = m_ptr->m_parent;
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::operator += (int n) noexcept
{
    if(m_ptr && n)
    {
        if (n < 0)
            return operator -=(-n);
        else
            while(n && m_ptr)
            {
                --n;
                if(m_ptr->hasChild())
                    m_ptr = m_ptr->m_firstChild;
                else
                    next();
            }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::operator -=(int n) noexcept
{
    if(m_ptr && n)
    {
        if (n < 0)
            return operator +=(-n);
        else
        {
            while(m_ptr && n)
            {
                if(m_ptr->m_prevBrother)
                    m_ptr = m_ptr->m_prevBrother->lastLeaf();
                else
                    m_ptr = m_ptr->m_parent;
                --n;
            }
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::operator [](const QList<int> & list)
{
    if(m_ptr)
    {
        brotherIterator j = m_ptr->root();
        for(auto i = list.cbegin(); i != list.cend() && j; ++i)
        {
            j = (*j)->m_firstChild;
            j += *i;
        }
        m_ptr = *j;
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::precedent() noexcept
{
    if(m_ptr)
    {
        if(m_sens)
        {
            if(m_ptr->m_prevBrother)
            {
                m_ptr = m_ptr->m_prevBrother;
                m_sens = false;
            }
            else
                m_ptr = m_ptr->m_parent;
        }
        else
        {
            if(m_ptr->hasChild())
                m_ptr = m_ptr->m_lastChild;
            else
                m_sens = true;
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::seek(const QList<int> & list, bool root)
{
    if(m_ptr)
    {
        m_sens = true;
        if(root)
            toRoot();
        brotherIterator j;
        for(auto i = list.cbegin(); m_ptr && i != list.cend(); ++i)
        {
            if(*i >= 0)
            {
                j = m_ptr->m_firstChild;
                j += *i;
                m_ptr = *j;
            }
            else
            {
                j = m_ptr->m_lastChild;
                j += *i;
                m_ptr = *j;
            }
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::seek(const QList<T> & list, bool root)
{
    if(m_ptr)
    {
        m_sens = true;
        if(root)
            toRoot();
        brotherIterator j;
        for(auto i = list.cbegin(); m_ptr && i != list.cend(); ++i)
        {
            j = m_ptr->m_firstChild;
            while(j && (*j)->m_data != *i)
                ++j;
            m_ptr = *j;
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::suivant() noexcept
{
    if(m_ptr)
    {
        if(m_sens)
        {
            if(m_ptr->hasChild())
                m_ptr = m_ptr->m_firstChild;
            else
                m_sens = false;
        }
        else
        {
            if(m_ptr->m_nextBrother)
            {
                m_ptr = m_ptr->m_nextBrother;
                m_sens = true;
            }
            else
                m_ptr = m_ptr->m_parent;
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::toNextBrother(int n) noexcept
{
    if(m_ptr)
    {
        if(n > 0)
        {
            for(; n && m_ptr->m_nextBrother; --n)
                m_ptr = m_ptr->m_nextBrother;
        }
        else if(n < 0)
        {
            while(n && m_ptr->m_prevBrother)
            {
                m_ptr = m_ptr->m_prevBrother;
                ++n;
            }
        }
    }
    return *this;
}

template<class T> typename TreeItem<T>::iterator & TreeItem<T>::iterator::toParent(int n) noexcept
{
    if(n && m_ptr)
    {
        if(n>0)
            while(n && m_ptr->m_parent)
            {
                m_ptr = m_ptr->m_parent;
                --n;
            }
        else if(n<0)
            while(n && m_ptr->hasChild())
            {
                m_ptr = m_ptr->m_firstChild;
                ++n;
            }
    }
    return *this;
}

// %%%%%%%%%%%%%%% protected %%%%%%%%%%%%%%

template<class T> void TreeItem<T>::iterator::next() noexcept
{
    while(m_ptr && !m_ptr->m_nextBrother)
        m_ptr = m_ptr->m_parent;
    if(m_ptr)
        m_ptr = m_ptr->m_nextBrother;
}

#endif // TREEITEM_H
