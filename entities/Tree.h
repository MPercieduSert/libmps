/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <list>
#include <stdexcept>
#include <utility>

#define TREE_ITER_COMMUN(ITER)protected: \
    /*! Opérateur d'affectation à partir d'un pointeur sur un noeud de l'arbre.*/ \
    ITER & operator =(Item * node) {const_abstract_iterator::operator=(node); return *this;} \
    public:\
    /*! Constructeur à l'aide d'un itérateur.*/ \
    ITER (const const_abstract_iterator & i) noexcept : const_abstract_iterator(i) {} \
    /*! Opérateur d'affectation à partir d'un iterator.*/ \
    ITER & operator =(const const_abstract_iterator & i) noexcept {const_abstract_iterator::operator=(i); return *this;}\
    /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
     *
     * Positionne l'itérateur sur le n ieme frère suivant. L'argument n peut être négatif.
     * Si n est trop négatif ou trop grand, l'itérateur sera nul.
     */ \
    ITER & toBrother(int n) noexcept {const_abstract_iterator::toBrother(n); return *this;} \
    /*! \brief Place l'itérateur sur le descendant direct d'indice n.
     *
     *  Place un pointeur sur le descendant direct d'indice n. Les indices négatifs sont permis.
     * Si position n'appartient pas à l'intervale [-nombre de fils, nombre de fils-1],
     * l'itérateur sera nul.
     */ \
    ITER & toChild(int position) noexcept {const_abstract_iterator::toChild(position); return *this;} \
    /*! Place l'itérateur sur l'ainé des frères du noeud courant.*/ \
    ITER & toFirstBrother() noexcept {const_abstract_iterator::toFirstBrother(); return *this;} \
    /*! Place l'itérateur sur l'ainé des fils du noeud courant.*/ \
    ITER & toFirstChild() noexcept {const_abstract_iterator::toFirstChild(); return *this;} \
    /*! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant.*/ \
    ITER & toFirstLeaf() noexcept {const_abstract_iterator::toFirstLeaf(); return *this;} \
    /*! Place l'itérateur sur le frère benjamin du noeud courant.*/ \
    ITER & toLastBrother() noexcept {const_abstract_iterator::toLastBrother(); return *this;} \
    /*! Place l'itérateur sur le fils benjamin  du noeud courant.*/ \
    ITER & toLastChild() noexcept {const_abstract_iterator::toLastChild(); return *this;} \
    /*! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant.*/ \
    ITER & toLastLeaf() noexcept {const_abstract_iterator::toLastLeaf(); return *this;} \
    /*! Place l'itérateur sur le frère cadet du noeud courant.*/ \
    ITER & toNextBrother() noexcept {const_abstract_iterator::toNextBrother(); return *this;} \
    /*! Place l'itérateur sur le parent du noeud courant.*/ \
    ITER & toParent() noexcept {const_abstract_iterator::toParent(); return *this;} \
    /*! \brief Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
     * Si n est négatif, l'itérateur est positionné sur le n ième fils ainé.
     *
     * Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
     * Si n est plus grand que le nombre de parents l'itérateur est positionné sur la racine.
     * Si n est négatif, l'itérateur est positionné sur le n ième fils ainé, en s'arrétant sur le dernier si n est trop grand.
     */ \
    ITER & toParent(int n) noexcept {const_abstract_iterator::toParent(n); return *this;} \
    /*! \brief Positionne l'itérateur sur le noeud de chemin de données list.
     *
     * Positionne l'itérateur sur le noeud de chemin des données list des différents descendants empruntés.
     * Si plusieurs noeuds frères possèdent les mêmes données c'est l'ainé qui sera emprunté.
     * Si l'une des donnée de la liste n'existe pas, l'itérateur renjoyé sera nul.
     */ \
    ITER & toPath(const std::list<T> & list) noexcept {const_abstract_iterator::toPath(list); return *this;} \
    /*! Place l'itérateur sur le frère ainé du noeud courant.*/ \
    ITER & toPrevBrother() noexcept {const_abstract_iterator::toPrevBrother(); return *this;} \
    /*! Place l'itérateur sur la racine du noeud courant.*/ \
    ITER & toRoot() noexcept {const_abstract_iterator::toRoot(); return *this;}


#define TREE_ITER_CONST(ITER) TREE_ITER_COMMUN(ITER) \
    /*! Opérateur de post-incrémentation.*/ \
    ITER operator ++(int) noexcept {ITER it = *this; operator ++(); return it;} \
    /*! Opérateur de post-décrémentation.*/ \
    ITER operator --(int) noexcept {ITER it = *this; operator --(); return it;} \
    /*! L'addition d'un itérateur i et d'un entier n, produit une copie de l'itérateur i incrémenté n fois.*/ \
    ITER operator +(int n) const noexcept {ITER it(*this); it += n; return it;} \
    /*! La soustraction d'un itérateur i et d'un entier n, produit une copie de l'itérateur i décrémenté n fois.*/ \
    ITER operator -(int n) const noexcept {ITER it(*this); it -= n; return it;} \
    /*! Affecte à data la donnée associée au noeud courant puis replace ce dernier sur le noeud suivant.*/ \
    ITER & operator >> (T & data) noexcept {data = operator * (); return operator ++ ();} \

#define TREE_ITER_NOCONST(ITER) TREE_ITER_COMMUN(ITER) \
    /*! Opérateur de pré-incrémentation.*/ \
    ITER & operator ++() noexcept {const_##ITER::operator ++(); return *this;} \
    /*! Opérateur de pré-décrémentation.*/ \
    ITER & operator --() noexcept {const_##ITER::operator --(); return *this;} \
    /*! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.*/ \
    ITER & operator +=(int n) noexcept {const_##ITER::operator +=(n); return *this;} \
    /*! Appelle +=(-n).*/ \
    ITER & operator -=(int n) noexcept {return operator +=(-n);} \
    /*! Opérateur de post-incrémentation.*/ \
    ITER operator ++(int) noexcept {ITER it(*this); const_##ITER::operator ++(); return it;} \
    /*! Opérateur de post-décrémentation.*/ \
    ITER operator --(int) noexcept {ITER it(*this); const_##ITER::operator --(); return it;} \
    /*! L'addition d'un itérateur i et d'un entier n, produit une copie de l'itérateur i incrémenté n fois.*/ \
    ITER operator +(int n) const noexcept {ITER it(*this); it.const_##ITER::operator +=(n); return it;} \
    /*! La soustraction d'un itérateur i et d'un entier n, produit une copie de l'itérateur i décrémenté n fois.*/ \
    ITER operator -(int n) const noexcept {ITER it(*this); it.const_##ITER::operator -=(n); return it;} \
    /*! Affecte à data la donnée associée au noeud courant puis replace ce dernier sur le noeud suivant.*/ \
    ITER & operator >> (T & data) noexcept {data = const_##ITER::operator * (); return operator ++ ();} \

namespace conteneurMPS {

/*! \ingroup groupeConteneur
 *  \brief Cette classe represente un arbre dont les noeuds sont constitués par des Item<T>.
 *
 * Cette classe represente un arbre. Elle est constituée de la racine d'un arbre dont les noeuds sont constitués par des Item<T>
 * et d'un itérateur sur cet arbre jouant le role d'un curseur, ce qui munit l'arbre d'un noeud courant.
 */
template<class T> class tree {
public:
    using size_type = unsigned long;
    class abstract_iterator;
    class brother_iterator;
    class const_abstract_iterator;
    class const_brother_iterator;
    class const_iterator;

protected:
    //////////////////////////////////// Item//////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Cette classe est une représentation des noeuds d'un arbre.
     *
     * Cette classe représente les noeuds d'un arbre contenant les informations suivantes:
     *  - une donnée de type T attachée au noeud,
     *  - un pointeur sur le Item parent dans l'arbre (l'élément racine posséde un pointeur parent nul),
     *  - un pointeur sur le frère précécent (le premier fils posséde un pointeur frère précédent nul).
     *  - un pointeur sur le frère suivant (le dernier fils posséde un pointeur frère suivant nul).
     *  - un pointeur sur le première fils (une feuille posséde un pointeur premier fils nul).
     *  - un pointeur sur le dernier fils (une feuille posséde un pointeur dernier fils nul).
     *
     * L'itérateur Item::brother_iterator permet de parcourir une fratrie.
     * L'itérateur Item::iterator propose différente façon de parcourir les noeuds d'un arbre de Item.
     *
     * \sa tree
     */
    class Item {
        friend const_abstract_iterator;
        friend abstract_iterator;
    protected:
        Item *m_parent = nullptr;        //!< Pointeur vers le Item parent dans l'arbre, est nul si le Item est la racine de l'arbre.
        Item *m_prevBrother = nullptr;   //!< Pointeur vers le frère précédent, est nul si le Item est le premier frère.
        Item *m_nextBrother = nullptr;   //!< Pointeur vers le frère suivant, est nul si le Item est le dernier frère.
        Item *m_firstChild = nullptr;    //!< Pointeur vers le premier fils, est nul si le noeud est une feuille.
        Item *m_lastChild = nullptr;     //!< Pointeur vers le dernier fils, est nul si le noeud est une feuille..
        T m_data;                               //!< Donnée accrochée au noeud de l'arbre.

    public:
        //! Constructeur par default, par défaut un noeud est la racine d'un l'arbre.
        Item() = default;

        //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants.
        Item(const Item & item);

        //! Constructeur par déplacement.
        //! Après déplacement, item n'appartient plus à un arbre, il faut le détruire.
        Item(Item && item) noexcept(noexcept(T(std::move(item.m_data))));

        //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
        Item(const T & data) noexcept(noexcept(T(data)))
            : m_data(data) {}

        //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
        Item(T && data) noexcept(noexcept(T(std::move(data))))
            : m_data(std::move(data)) {}

//        //! Constructeur de donné en place.
//        template< class... Args > Item(Args && ... args)
//            : m_data(std::forward<Args>(args)...) {}

        //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants,
        //! T doit posséder un constructeur à partie de U.
        template<class U> Item(const typename tree<U>::Item & item);

        //! Constructeur à partir d'un Item de donnée U et une usine construisant une donnée T grâce à une donnée U.
        template<class U, class F> Item(const typename tree<U>::Item & item, F usine);

        //! Destructeur. Détruit également récursivement l'ensemble des descendants.
        ~Item();

        //! Transforme le noeud d'un abre en racine.
        void becomeRoot() noexcept {
            changeHeredite(nullptr);
            m_prevBrother = m_nextBrother = nullptr;
        }

        ///////////////// begin ////////////

        //! Crée un brother_iterator initialisé sur le fils ainé.
        brother_iterator beginChild() noexcept
            {return m_firstChild;}

        //! Crée un const_iterator initialisé sur ce noeud.
        const_iterator cbegin() const noexcept
            {return this;}

        //! Crée un const_brother_iterator initialisé sur le fils ainé.
        const_brother_iterator cbeginChild() const noexcept
            {return m_firstChild;}

        //! Change l'hérédité du noeud en le supprimant des descendants de son noeud parent
        //! avant de remplacer le parent du noeud par le nouveau parent.
        void changeHeredite(Item *parent) noexcept;

        //! Détruit tous les descendants du noeud.
        void clearChilds() noexcept;

        //! Transmet une référence sur la donnée associée au noeud.
        T & data() noexcept
            {return m_data;}

        //! Transmet une référence constante sur la donnée associée au noeud.
        const T & data() const noexcept
            {return m_data;}

        //! Supprime les noeuds appartenant à [this,last) et leurs decendants.
        //! Le noeud last doive être de la même fratrie sauf si last est nul.
        void erase(Item * last) noexcept;

        //! Test si le noeud  est l'ainé des frères.
        bool firstBrother() const noexcept
            {return !m_prevBrother;}

        //! Insert le noeud pointé par item avant this dans la fratrie sauf si this est la racine,
        //! dans ce cas item est inseré en tant que fils ainé.
        //! item ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
        void insert(Item * item) noexcept;

        //! Insert le noeud pointé par item après this dans la fratrie sauf si this est la racine,
        //! dans ce cas item est inseré en tant que fils benjamin.
        //! item ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
        void insert_after(Item * item) noexcept;

        //! Test si le noeud est le benjamin des frères.
        bool lastBrother() const noexcept
            {return !m_nextBrother;}

        //! Test si le noeud posséde un descendant direct.
        bool leaf() const noexcept
            {return !m_firstChild;}

        //! Insert le noeud pointé par item avant this dans la fratrie sauf si this est la racine,
        //! dans ce cas item est inseré en tant que fils ainé.
        //! Si item pointe sur la racine d'un arbre, le noeud inséré est le déplacement de item.
        void move(Item * item) {
            if(item->m_parent)
                insert(item);
            else
                insert(new Item(std::move(*item)));
        }

        //! Insert le noeud pointé par item après this dans la fratrie sauf si this est la racine,
        //! dans ce cas item est inseré en tant que fils ainé.
        //! Si item pointe sur la racine d'un arbre, le noeud inséré est le déplacement de item.
        void move_after(Item * item) {
            if(item->m_parent)
                insert_after(item);
            else
                insert_after(new Item(std::move(*item)));
        }

        //! Ajout le noeud pointé par item en tant que benjamin des descendants du noeud.
        //! Si item pointe sur la racine d'un arbre, le noeud ajouté est le déplacement de item.
        void move_back(Item * item) {
            if(item->m_parent)
                push_back(item);
            else
                push_back(new Item(std::move(*item)));
        }

        //! Ajout le noeud pointé par item en tant qu'ainé des descendants du noeud.
        //! Si item pointe sur la racine d'un arbre, le noeud ajouté est le déplacement de item.
        void move_front(Item * item) {
            if(item->m_parent)
                push_front(item);
            else
                push_front(new Item(std::move(*item)));
        }

        /*!
         * \brief Opérateur d'affectation de recopie.
         *
         * L'opérateur d'affectation modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds
         * pour les remplacer par une copie de ceux de tree.
         */
        Item & operator = (const Item & item);

        /*!
         * \brief Opérateur d'affectation par déplacement.
         *
         * L'opérateur d'affectation par déplacement, modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds,
         * change en tree racine, puis récupére les liens de descendance de tree avant de les supprimer de tree.
         */
        Item & operator = (Item && item) noexcept(noexcept (T(std::move(item.m_data))));

        /*!
         * \brief Ajoute le noeud pointé par child en tant que benjamin des descendants du noeud.
         * item ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
         *
         * Le noeud pointé par child est ajouté en dernière position dans la fratrie des descendants directs.
         * Si le descendant ajouté avait déjà un parent,
         * ce descendant est alors supprimé de la liste des descendants directs
         * de ce parent avant que ne soit changé le pointeur sur le parent du noeud ajouté.
         */
        void push_back(Item *child) noexcept;

        /*!
         * \brief Ajoute le noeud pointé par child en tant qu'ainé des descendants du noeud.
         * item ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
         *
         * Le noeud pointé par child est ajouté en première position dans la fratrie des descendants directs.
         * Si le descendant ajouté avait déjà un parent,
         * ce descendant est alors supprimé de la liste des descendants directs
         * de ce parent avant que ne soit changé le pointeur sur le parent du noeud ajouté.
         */
        void push_front(Item *child) noexcept;

        //! Modifie la donnée associée au noeud.
        void setData(const T & data) noexcept(noexcept (T::operateur = (data)))
            {m_data = data;}

        //! Modifie la donnée associée au noeud.
        void setData(T && data) noexcept(noexcept (T::operateur = (std::move(data))))
            {m_data = std::move(data);}

        /*! \brief Renvoie le nombre de noeuds du sous-arbre ayant ce noeud pour racine.
         *
         * Méthode déterminant le nombre de noeuds du sous-arbre ayant ce noeud pour racine,
         * c'est-à-dire le nombre de descendants directs et indirects augmenté de un pour prendre en compte ce noeud.
         */
        size_type size() const noexcept;

        //! Renvoie le nombre de descendants directs.
        size_type sizeChild() const noexcept;
    };

public:
    ///////////////////////////////////// const_abstract_iterator //////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Classe parente des itérateurs sur un arbre.
     */
    class const_abstract_iterator {
        friend tree;
    protected:
        Item * m_ptr;     //!< Pointeur sur le noeud courant.

    public:
        //! Constructeur.
        const_abstract_iterator(void * node) noexcept
            : m_ptr(static_cast<Item *>(node)) {}

        ///////////////// begin ////////////

        //! Crée un const_brother_iterator initialisé sur le fils ainé.
        const_brother_iterator cbeginChild() const noexcept
            {return m_ptr->m_firstChild;}

        //! Crée un const_brother_iterator initialisé sur le fils benjamin.
        const_brother_iterator crbeginChild() const noexcept
            {return m_ptr->m_lastChild;}

        //! Test si le noeud courant est l'ainé des frères.
        bool firstBrother() const noexcept
            {return m_ptr && !m_ptr->m_prevBrother;}

        //! Détermine la position du noeud courant par rapport à la racine.
        std::list<int> index() const;

        //! Détermine la position du noeud courant dans la fratrie.
        int indexBrother() const noexcept;

        //! Test si le noeud courant est l'ainé des frères.
        bool lastBrother() const noexcept
            {return m_ptr && !m_ptr->m_nextBrother;}

        //! Test si le noeud courant est une feuille.
        bool leaf() const noexcept
            {return m_ptr && !m_ptr->m_firstChild;}

        //! Conversion en booléen.
        operator bool() const noexcept
            {return m_ptr;}

        //! Renvoie une référence sur la donnée du noeud courant.
        const T & operator *() const noexcept
            {return m_ptr->data();}

        //! Déférencement sur la donnée du noeud courant.
        T const *  operator ->() const noexcept
            {return &m_ptr->data();}

        //! Test d'égalité entre deux itérateurs.
        bool operator ==(const const_abstract_iterator & i) const noexcept
            {return m_ptr == i.m_ptr;}

        //! Test d'égalité entre deux itérateurs.
        bool operator !=(const const_abstract_iterator & i) const noexcept
            {return m_ptr != i.m_ptr;}

        //! Crée un const_brother_iterator initialisé sur le parent.
        const_abstract_iterator parent() const noexcept
            {return m_ptr ? m_ptr->m_parent : nullptr;}

        //! Acceseur du pointeur.
        void * ptr() const
            {return m_ptr;}

        //! Teste si le neud courant est le sommet de l'arbre.
        bool root() const noexcept
            {return m_ptr && !m_ptr->m_parent;}

        //! Retourne le nombre de descendants du noeud courant.
        size_type size() const noexcept
            {return m_ptr ? m_ptr->size() : 0;}

        //! Retourne le nombre de descendants directs du noeud courant.
        size_type sizeChild() const noexcept
            {return m_ptr ? m_ptr->sizeChild() : 0;}

    protected:
        //! Constructeur.
        const_abstract_iterator(Item * node = nullptr) noexcept
            : m_ptr(node) {}

        //! Renvoie un référence sur le noeud courant.
        const Item & item() const noexcept
            {return *m_ptr;}

        //! Opérateur d'affectation à partir d'un pointeur sur un noeud de l'arbre.
        const_abstract_iterator & operator =(Item * node) noexcept {
            m_ptr = node;
            return *this;
        }

        //! Template des opérateur +=.
        template<class iter> iter & opAddAssign(int n) noexcept;

        //! Déplace l'itérateur sur le fils de position pos (sans vérification).
        template<class iter, class broIter> iter & opSubscript(int pos) noexcept {
            broIter j(m_ptr);
            j += pos;
            this->operator=(j);
            return *this;
        }

        //! Place l'itérateur sur l'ainé des frères du noeud courant (en supposant la validité des pointeurs).
        void to_firstBrother() noexcept
            {m_ptr = m_ptr->m_parent->m_firstChild;}

        //! Place l'itérateur sur l'ainé des fils du noeud courant (en supposant la validité des pointeurs).
        void to_firstChild() noexcept
            {m_ptr = m_ptr->m_firstChild;}

        //! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant (en supposant la validité des pointeurs).
        void to_firstLeaf() noexcept {
            while(m_ptr->m_firstChild)
                    m_ptr = m_ptr->m_firstChild;
        }

        //! Place l'itérateur sur le frère benjamin du noeud courant (en supposant la validité des pointeurs).
        void to_lastBrother() noexcept
            {m_ptr = m_ptr->m_parent->m_lastChild;}

        //! Place l'itérateur sur le fils benjamin  du noeud courant (en supposant la validité des pointeurs).
        void to_lastChild() noexcept
            {m_ptr = m_ptr->m_lastChild;}

        //! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant (en supposant la validité des pointeurs).
        void to_lastLeaf() noexcept {
            while(m_ptr->m_lastChild)
                    m_ptr = m_ptr->m_lastChild;
        }

        //! Place l'itérateur sur le frère cadet du noeud courant (en supposant la validité des pointeurs).
        void to_nextBrother() noexcept
            {m_ptr = m_ptr->m_nextBrother;}

        //! Positionne l'itérateur sur le frère cadet du noeud courant s'il existe.
        //! Si le noeud courant est le benjamin,
        //! on remonte jusqu'au premier parent possédant un frère cadet et l'on positionne l'itérateur dessus.
        //! Si le noeud courant est le benjamin de l'arbre, l'itérateur est placé sur le noeud virtuel nul.
        void to_nextBrotherIfNotUncle() noexcept {
            while(lastBrother())
                to_parent();
            if(m_ptr)
                to_nextBrother();
        }

        //! Place l'itérateur sur le parent du noeud courant (en supposant la validité des pointeurs).
        void to_parent() noexcept
            {m_ptr = m_ptr->m_parent;}

        //! Place l'itérateur sur le frère ainé du noeud courant (en supposant la validité des pointeurs).
        void to_prevBrother() noexcept
            {m_ptr = m_ptr->m_prevBrother;}

        //! Positionne l'itérateur sur le frère ainé du noeud courant s'il existe.
        //! Si le noeud courant est l'ainé,
        //! on remonte jusqu'au premier parent possédant un frère ainé et l'on positionne l'itérateur dessus.
        //! Si le noeud courant est le dernier descendant des ainés de l'arbre, l'itérateur est placé sur le noeud virtuel nul.
        void to_prevBrotherIfNotUncle() noexcept {
            while(firstBrother())
                to_parent();
            if(m_ptr)
                to_prevBrother();
        }

        //! Place l'itérateur sur la racine du noeud courant (en supposant la validité des pointeurs).
        void to_root() noexcept {
            while(m_ptr->m_parent)
                    m_ptr = m_ptr->m_parent;
        }

        /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
         *
         * Positionne l'itérateur sur le n ieme frère suivant. L'argument n peut être négatif.
         * Si n est trop négatif, l'itérateur est positionné sur le premier des frères
         * et si n est trop grand, l'itérateur est positionné sur le dernier des frères.
         */
        void toBrother(int n) noexcept;

        /*! \brief Place un pointeur sur le descendant direct d'indice n.
         *
         *  Place un pointeur sur le descendant direct d'indice n. Les indices négatifs sont permis.
         * Si position n'appartient pas à l'intervale [-nombre de fils, nombre de fils-1],
         * l'itérateur sera placé sur le fils ainé ou benjamin.
         */
        void toChild(int position) noexcept;

        //! Place l'itérateur sur l'ainé des frères du noeud courant.
        void toFirstBrother() noexcept {
            if(m_ptr && m_ptr->m_parent)
                to_firstBrother();
        }

        //! Place l'itérateur sur l'ainé des fils du noeud courant.
        void toFirstChild() noexcept {
            if(m_ptr)
                to_firstChild();
        }

        //! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant.
        void toFirstLeaf() noexcept {
            if(m_ptr)
                to_firstLeaf();
        }

        //! Place l'itérateur sur le frère benjamin du noeud courant.
        void toLastBrother() noexcept {
            if(m_ptr && m_ptr->m_parent)
                to_lastBrother();
        }

        //! Place l'itérateur sur le fils benjamin  du noeud courant.
        void toLastChild() noexcept {
            if(m_ptr)
                to_lastChild();
        }

        //! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant.
        void toLastLeaf() noexcept {
            if(m_ptr)
                to_lastLeaf();
        }

        //! Place l'itérateur sur le frère cadet du noeud courant.
        void toNextBrother() noexcept {
            if(m_ptr)
                to_nextBrother();
        }

        //! Place l'itérateur sur le parent du noeud courant.
        void toParent() noexcept {
            if(m_ptr)
                to_parent();
        }

        /*! \brief Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         * Si n est négatif, l'itérateur est positionné sur le n ième fils ainé.
         *
         * Positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         * Si n est plus grand que le nombre de parents l'itérateur est positionné sur la racine.
         * Si n est négatif, l'itérateur est positionné sur le n ième fils ainé, en s'arrétant sur le dernier si n est trop grand.
         */
        void toParent(int n) noexcept;


        /*! \brief Positionne l'itérateur sur le noeud de chemin de données list.
         *
         * Positionne l'itérateur sur le noeud de chemin des données list des différents descendants empruntés.
         * Si plusieurs noeuds frères possèdent les mêmes données c'est l'ainé qui sera emprunté.
         * Si l'une des donnée de la liste n'existe pas, l'itérateur renjoyé sera nul.
         */
        void toPath(const std::list<T> & list) noexcept;

        //! Place l'itérateur sur le frère ainé du noeud courant.
        void toPrevBrother() noexcept {
            if(m_ptr)
                to_prevBrother();
        }

        //! Place l'itérateur sur la racine du noeud courant.
        void toRoot() noexcept {
            if(m_ptr)
                to_root();
        }
    };

    ////////////////////////////////////// abstract_iterator ////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Classe parente des itérateurs sur un arbre non constant.
     */
    class abstract_iterator : public virtual const_abstract_iterator {
        friend Item;
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        using const_abstract_iterator::operator->;

        //! Renvoie un référence sur le noeud courant.
        Item & item() noexcept
            {return *m_ptr;}

        //! Crée un brother_iterator initialisé sur le fils ainé.
        brother_iterator beginChild() noexcept
            {return m_ptr->m_firstChild;}

        //! Renvoie une référence sur la donnée du noeud courant.
        T & operator *() noexcept
            {return m_ptr->data();}

        //! Déférencement sur la donnée du noeud courant.
        T  *  operator ->() noexcept
            {return &m_ptr->data();}

    protected:
        //! Mutateur du parent du noeud courant.
        void setParent(Item * parent)
            {m_ptr->m_parent = parent;}

        //! Acceseur du pointeur.
        Item * itemPtr() const
            {return m_ptr;}
    };
    ///////////////////////////////////// const_brother_iterator /////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur constant sur une fratrie.
     */
    class const_brother_iterator : public virtual const_abstract_iterator {
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::toNextBrother;
        using const_abstract_iterator::toPrevBrother;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_brother_iterator)

        //! Opérateur de pré-incrémentation.
        const_brother_iterator & operator ++() noexcept
            {return toNextBrother();}

        //! Opérateur de pré-décrémentation.
        const_brother_iterator & operator --() noexcept
            {return toPrevBrother();}

        //! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.
        const_brother_iterator & operator +=(int n) noexcept
            {return const_abstract_iterator::template opAddAssign<const_brother_iterator>(n);}

        //! Appelle +=(-n).
        const_brother_iterator & operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    //////////////////////////////////// const_reverse_brother_iterator //////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur-inverse constant sur une fratrie.
     */
    class const_reverse_brother_iterator : public const_brother_iterator {
    protected:
        using const_abstract_iterator::toNextBrother;
        using const_abstract_iterator::toPrevBrother;

    public:
        using const_brother_iterator::const_brother_iterator;
        using const_brother_iterator::operator*;
        TREE_ITER_CONST(const_reverse_brother_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_brother_iterator & operator ++() noexcept
            {return toPrevBrother();}

        //! Opérateur de pré-décrémentation.
        const_reverse_brother_iterator & operator --() noexcept
            {return toNextBrother();}

        //! Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_reverse_brother_iterator & operator +=(int n) noexcept
            {return const_abstract_iterator::template opAddAssign<const_reverse_brother_iterator>(n);}

        //! Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
        const_reverse_brother_iterator & operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    ////////////////////////////////////////// const_iterator //////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur constant sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
     */
    class const_iterator : public virtual const_abstract_iterator {
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_firstChild;
        using const_abstract_iterator::to_lastLeaf;
        using const_abstract_iterator::to_nextBrotherIfNotUncle;
        using const_abstract_iterator::to_parent;
        using const_abstract_iterator::to_prevBrother;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_iterator)

        //! Opératateur de pré-incrémentation.
        //! Positionne l'itérateur sur le noeud suivant, les noeuds sont parcourus uniquement à la descente.
        const_iterator & operator ++() noexcept;

        //! Opérateur de pré-décrémentation.
        const_iterator & operator --() noexcept;

        /*! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
         * Pour un n grand cet opérateur est plus efficace que d'appliquer n fois l'opérateur d'incrémemtation.
         */
        const_iterator & operator +=(int n) noexcept;

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         * Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         * l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         * a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_iterator & operator -=(int n) noexcept;
    };
    //////////////////////////////////////// const_reverse_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur-inverse constant sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les benjamins.
     */
    class const_reverse_iterator : public const_iterator
    {
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_firstLeaf;
        using const_abstract_iterator::to_lastChild;
        using const_abstract_iterator::to_nextBrother;
        using const_abstract_iterator::to_parent;
        using const_abstract_iterator::to_prevBrother;
        using const_abstract_iterator::to_prevBrotherIfNotUncle;

    public:
        using const_iterator::const_iterator;
        using const_iterator::operator*;
        TREE_ITER_CONST(const_reverse_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_iterator & operator ++() noexcept;

        //! Opérateur de pré-décrémentation.
        const_reverse_iterator & operator --() noexcept;

        /*! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
         * Pour un n grand cet opérateur est plus efficace que d'appliquer n fois l'opérateur d'incrémemtation.
         */
        const_reverse_iterator & operator +=(int n) noexcept;

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         * Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         * l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         * a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_reverse_iterator & operator -=(int n) noexcept;
    };
    ////////////////////////////////////////// const_leaf_iterator //////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur constant sur les feuilles de l'arbre.
     */
    class const_leaf_iterator : public virtual const_abstract_iterator {
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_nextBrotherIfNotUncle;
        using const_abstract_iterator::to_prevBrotherIfNotUncle;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_leaf_iterator)

        //! Opératateur de pré-incrémentation.
        //! Positionne l'itérateur sur la feuille suivante.
        const_leaf_iterator & operator ++() noexcept {
            to_nextBrotherIfNotUncle();
            return toFirstLeaf();
        }

        //! Opérateur de pré-décrémentation.
        const_leaf_iterator & operator --() noexcept {
            to_prevBrotherIfNotUncle();
            return toLastLeaf();
        }

        //! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_leaf_iterator & operator +=(int n) noexcept
            {return const_abstract_iterator::template opAddAssign<const_leaf_iterator>(n);}

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         * Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         * l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         * a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_leaf_iterator & operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    //////////////////////////////////////// const_reverse_leaf_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur-inverse constant sur les feuilles de l'arbre.
     */
    class const_reverse_leaf_iterator : public const_leaf_iterator
    {
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_nextBrotherIfNotUncle;
        using const_abstract_iterator::to_prevBrotherIfNotUncle;

    public:
        using const_iterator::const_iterator;
        using const_iterator::operator*;
        TREE_ITER_CONST(const_reverse_leaf_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_leaf_iterator & operator ++() noexcept {
            to_prevBrotherIfNotUncle();
            return toLastLeaf();
        }

        //! Opérateur de pré-décrémentation.
        const_reverse_leaf_iterator & operator --() noexcept {
            to_nextBrotherIfNotUncle();
            return toFirstLeaf();
        }

        //! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_reverse_leaf_iterator & operator +=(int n) noexcept
            {return const_abstract_iterator::template opAddAssign<const_reverse_leaf_iterator>(n);}

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         * Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         * l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         * a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_reverse_leaf_iterator & operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    /////////////////////////////////////////// const_prevsuiv_iterator ///////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur constant sur l'arbre parcourant les noeuds à la descente et à la remonté en commeçant par les ainés.
     */
    class const_prevsuiv_iterator : public virtual const_abstract_iterator {
    protected:
        using const_abstract_iterator::m_ptr;
        bool m_sens = true;            //!< Indicateur du sens de parcours: true -> descente; false -> remontée.
        using const_abstract_iterator::to_firstChild;
        using const_abstract_iterator::to_lastChild;
        using const_abstract_iterator::to_nextBrother;
        using const_abstract_iterator::to_parent;
        using const_abstract_iterator::to_prevBrother;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_prevsuiv_iterator)

        //! Positionne l'itérateur sur le noeud suivant dans le parcours de type suivant-précédent:
        //! les noeuds sont parcourus deux fois, à la descente et à la remontée.
        const_prevsuiv_iterator & operator ++() noexcept;

        //! Positionne l'itérateur sur le noeud précédent dans le parcours de type suivant-précédent:
        //! les noeuds sont parcours deux fois, à la descente et à la remonté.
        const_prevsuiv_iterator & operator --() noexcept;

        //! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.
        const_prevsuiv_iterator & operator +=(int n) noexcept
            {return const_abstract_iterator::template opAddAssign<const_prevsuiv_iterator>(n);}

        //! Appelle +=(-n).
        const_prevsuiv_iterator & operator -=(int n) noexcept
            {return operator +=(-n);}

        //! Renvoie le sens de parcours de l'itérateur: true -> descente; false -> remontée.
        bool sens() const noexcept
            {return m_sens;}

        //! Modifie le sens de parcours de la méthode suivant-précédent.
        void setSens(bool sens) noexcept
            {m_sens = sens;}
    };
    //////////////////////////////////////// brother_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur sur une fratrie.
     */
    class brother_iterator : public abstract_iterator, public virtual const_brother_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(brother_iterator)
    };
    ////////////////////////////////////// iterator ///////////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
     */
    class iterator : public abstract_iterator, public const_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(iterator)
    };
    ////////////////////////////////////// leaf_iterator ///////////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur sur l'arbre parcourant les feuilles.
     */
    class leaf_iterator : public abstract_iterator, public const_leaf_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(leaf_iterator)
    };
    /////////////////////////////////////// prevsuiv_iterator //////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateursur l'arbre parcourant les noeuds à la descente et à la remonté en commeçant par les ainés.
     */
    class prevsuiv_iterator : public abstract_iterator, public const_prevsuiv_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(prevsuiv_iterator)
    };
    //////////////////////////////////////// reverse_brother_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur inverse sur une fratrie.
     */
    class reverse_brother_iterator : public abstract_iterator, public const_brother_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(reverse_brother_iterator)
    };
    /////////////////////////////////////////// reverse_iterator //////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur inverse sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
     */
    class reverse_iterator : public abstract_iterator, public const_reverse_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(reverse_iterator)
    };
    /////////////////////////////////////////// reverse_iterator //////////////////////////////////////
    /*! \ingroup groupeConteneur
     * \brief Itérateur inverse sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
     */
    class reverse_leaf_iterator : public abstract_iterator, public const_reverse_leaf_iterator {
        friend tree;
    protected:
        using abstract_iterator::m_ptr;
    public:
        using abstract_iterator::abstract_iterator;
        using abstract_iterator::operator*;
        TREE_ITER_NOCONST(reverse_leaf_iterator)
    };

    ///////////////////////////////////////////// Tree/////////////////////////////////////////////////////
    template<typename> friend class tree;
protected:
    Item * m_root;      //!< Pointeur sur la racine de l'arbre.

public:
    //! Constructeur sans argument.
    tree()
        : tree(new Item) {}

    //! Constructeur de recopie.
    tree(const tree & t)
        : tree(new Item(*t.m_root)) {}

    //! Constructeur de déplacement.
    tree(tree && t) noexcept
        : tree(t.m_root) {
        t.m_root = nullptr;
    }

    //! Constructeur, crée une racine de donnée associée data et initialise l'itérateur sur cette racine.
    explicit tree(const T & data)
        : tree(new Item(data)){}

    //! Constructeur, crée une racine de donnée associée data et initialise l'itérateur sur cette racine.
    explicit tree(T && data)
        : tree(new Item(std::move(data))){}

    //! Constructeur, créer un arbre possédant la même structure et les même valeur que tree,
    //! T doit posséder un constructeur à partir de U.
    template<class U> tree(const tree<U> & t)
        : tree(new Item(*(t.m_root))) {}

    //! Constructeur d'un arbre de donnée de type T à partir d'un arbre de donnée de type U,
    //! le foncteur usine construit une donnée de type T à partir d'une donnée de type U.
    template<class U, class F> tree(const tree<U> & t, F usine)
       // : tree(new Item(*(t.m_root),usine)) {}
    {
        auto iterU = t.cbegin();
        m_root = new Item(usine(*iterU));
        auto iterT = begin();
        while (iterU) {
            if(!iterU.leaf())
                for (auto i = iterU.cbeginChild(); i; ++i)
                    push_back(iterT, usine(*i));
            ++iterU;
            ++iterT;
        }
    }

    //! Destructeur. Détruit tous les noeuds de l'arbre.
    ~tree()
        {delete m_root;}

    //! Renvoie une référence sur la donnée de chemin pos (avec vérification pouvant conduire à une exception std::out_of_range).
    T & at(std::list<int> pos) {
        auto i = begin();
        return *i.to(pos);
    }

    //! Renvoie une référence sur la donnée de chemin pos (avec vérification pouvant conduire à une exception std::out_of_range).
    const T & at(std::list<int> pos) const {
        auto i = cbegin();
        return *i.to(pos);
    }

    /////// begin iterator ////////

    //! Renvoie un itérateur initialisé sur la racine.
    iterator begin() noexcept
        {return m_root;}

    //! Renvoie un itérateur constant initialisé sur la racine.
    const_iterator cbegin() const noexcept
        {return m_root;}

    //! Renvoie un itérateur inverse initialisé sur la racine.
    reverse_iterator rbegin() noexcept
        {return m_root;}

    //! Renvoie un itérateur inverse constant initialisé sur la racine.
    const_reverse_iterator crbegin() const noexcept
        {return m_root;}

    /////// begin brother_iterator ////////

    //! Renvoie un itérateur de frère initialisé sur la racine.
    brother_iterator beginBrother() const noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère constant initialisé sur la racine.
    const_brother_iterator cbeginBrother() const noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère inverse initialisé sur la racine.
    reverse_brother_iterator rbeginBrother() const noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère inverse constant initialisé sur la racine.
    const_reverse_brother_iterator crbeginBrother() const noexcept
        {return m_root;}

    /////// begin brother_iterator ////////

    //! Renvoie un itérateur de feuille initialisé sur la première feuille.
    leaf_iterator beginLeaf() const noexcept
        {return const_abstract_iterator(m_root).toFirstLeaf();}

    //! Renvoie un itérateur de feuille constant initialisé sur la première feuille.
    const_leaf_iterator cbeginLeaf() const noexcept
        {return const_abstract_iterator(m_root).toFirstLeaf();}

    //! Renvoie un itérateur de feuille inverse initialisé sur la dernière feuille.
    reverse_brother_iterator rbeginLeaf() const noexcept
        {return const_abstract_iterator(m_root).toLastLeaf();}

    //! Renvoie un itérateur de feuille inverse constant initialisé sur la dernière feuille.
    const_reverse_brother_iterator crbeginLeaf() const noexcept
        {return const_abstract_iterator(m_root).toLastLeaf();}

    /////// begin prevsuiv_iterator ////////

    //! Renvoie un itérateur de type prevSuiv initialisé sur la racine.
    prevsuiv_iterator beginPrevsuiv() noexcept
        {return m_root;}

    //! Renvoie un itérateur de type prevSuiv constant initialisé sur la racine.
    const_prevsuiv_iterator cbeginPrevsuiv() const noexcept
        {return m_root;}

    //! Revoie la liste des données des descendants directs du noeud pointé par pos.
    std::list<T> childs(const_abstract_iterator & pos) {
        std::list<T> liste;
        for (auto i = pos.beginChild(); i; ++i)
            liste.push_back(*i);
        return liste;
    }

    //! Détruit l'ensemble des descendants de la racine. Laisse inchangé la donnée de la racine.
    void clear() noexcept
        {m_root->clearChilds();}

    //! Test si l'arbre est vide, c'est-à-dire, si la racine n'est pas nulle.
    bool empty() const noexcept
        {return m_root;}

//    //! Insert un nouveau noeud de donnée crée en place à partir de args
//    //! avant le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
//    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
//    template< class... Args > iterator emplace( abstract_iterator & pos, Args&&... args ) {
//        auto * node = new Item(std::forward<Args>(args)...);
//        pos.m_ptr->insert(node);
//        return node;
//    }

//    //! Insert un nouveau noeud de donnée crée en place à partir de args
//    //! après le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
//    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
//    template< class... Args > iterator emplace_after( abstract_iterator & pos, Args&&... args ) {
//        auto * node = new Item(std::forward<Args>(args)...);
//        pos.m_ptr->insert_after(node);
//        return node;
//    }

//    //! Insert un fils benjamin de donnée crée en place à partir de args. Retourne un itérateur sur ce nouveau noeud.
//    template< class... Args > iterator emplace_back( abstract_iterator & pos, Args&&... args ) {
//        auto * node = new Item(std::forward<Args>(args)...);
//        pos.m_ptr->push_back(node);
//        return node;
//    }

//    //! Insert un fils ainé de donnée crée en place à partir de args. Retourne un itérateur sur ce nouveau noeud.
//    template< class... Args > iterator emplace_front( abstract_iterator & pos, Args&&... args ) {
//        auto * node = new Item(std::forward<Args>(args)...);
//        pos.m_ptr->push_front(node);
//        return node;
//    }

    //! Crée un itérateur initialisé sur le noeud virtuel nul. Cette fonction permet la compatibilité avec les algorithmes standards.
    //! Utiliser de préférence la conversion de l'itérateur en booléen.
    const_abstract_iterator end() const noexcept
        {return const_abstract_iterator();}

    //! Supprime l'élément pointé par l'itérateur pos et ses descendants.
    //! Si pos pointe sur la racine, seul les descendants sont supprimés.
    //! Retourne un itérateur sur le suivant celui pointé par pos.
    template<class iter_tree> iter_tree erase(iter_tree pos) noexcept;

    //! Supprime les noeuds appartenant à [first,last) et leurs decendants.
    //! Les noeuds pointés par first et last doivent être d'une même fratrie sauf si last est nul.
    //! Retourne un itérateur sur le noeud suivant le dernier noeud supprimé.
    template<class iter_tree> iter_tree erase(iter_tree first, iter_tree last) noexcept {
        first.m_ptr->erase(last.m_ptr);
        return last;
    }

    //! Extrait le noeud pointer par pos de l'arbre pour en faire un nouvel arbre
    template<class iter_tree> tree extract(iter_tree pos) {
        pos.m_ptr->becomeRoot();
        return tree(pos.m_ptr);
    }

    //! Insert un nouveau noeud de donnée data avant le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert(iter_tree pos, const T & data = T()) {
        auto * node = new Item(data);
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée data avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert(iter_tree pos, T && data) {
        auto * node = new Item(std::move(data));
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert une copie du noeud (et des descendant) pointé par other avant le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree> iter_tree insert(iter_tree pos, const_iter_tree  other) {
        auto * node = new Item(other.item());
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée coorespondant à la racine de tree (avec ses descendants) avant le noeud pointé par pos
    //! dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    //! La racine tree devient nulle.
    template<class iter_tree> iter_tree insert(iter_tree pos, tree<T> && tree) {
        auto * node = tree.m_root;
        pos.m_ptr->insert(node);
        tree.m_root = nullptr;
        return node;
    }

    //! Insert count nouveaux noeuds de donnée data avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils ainé. Retourne un itérateur sur le premier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert(iter_tree pos, size_type count, const T & data)
        {return add(pos, count, data, this->insert);}

    //! Insert de nouveaux noeuds de donnée correspondant au la plage d'itérateur [first, last)
    //! avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils ainé. Retourne un itérateur sur le premier des nouveaux noeuds.
    template<class iter_tree, class InputIt> iter_tree insert(iter_tree pos, InputIt first, InputIt last)
        {return add(pos, first, last, this->insert);}
    //! Insert de nouveaux noeuds de donnée contenu dans ilist
    //! avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils ainé. Retourne un itérateur sur le premier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert(iter_tree pos, std::initializer_list<T> ilist)
        {return insert(pos,ilist.begin(),ilist.end());}

    //! Insert un nouveau noeud de donnée data après le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, const T & data = T()) {
        auto * node = new Item(data);
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée data après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, T && data) {
        auto * node = new Item(std::move(data));
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert une copie du noeud (et des descendants) pointé par other après le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree> iter_tree insert_after(iter_tree pos, const_iter_tree other) {
        auto * node = new Item(other.item());
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée coorespondant à la racine de tree (avec ses descendants) après le noeud pointé par pos
    //! dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, tree<T> && tree) {
        auto * node = tree.m_root;
        pos.m_ptr->insert_after(node);
        tree.m_root = nullptr;
        return node;
    }

    //! Insert count nouveaux noeuds de donnée data après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils benjamin. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, size_type count, const T & data)
        {return add(pos, count, data, this->insert_after);}

    //! Insert de nouveaux noeuds de donnée correspondant au la plage d'itérateur [first, last)
    //! après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils benjamin. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree, class InputIt> iter_tree insert_after(iter_tree pos, InputIt first, InputIt last)
        {return add(pos, first, last, this->insert_after);}

    //! Insert de nouveaux noeuds de donnée contenu dans ilist
    //! après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils benjamin. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, std::initializer_list<T> ilist)
        {return insert_after(pos,ilist.begin(),ilist.end());}

    //! Déplace le noeud pointé par other avant le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le noeud est inseré en tant que fils ainé.
    template<class iter_tree> void move(iter_tree pos, iter_tree other)
        {pos.m_ptr->move(other.m_ptr);}

    //! Déplace le noeud pointé par other avant le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le noeud est inseré en tant que fils benjamin.
    template<class iter_tree> void move_after(iter_tree pos, iter_tree other)
        {pos.m_ptr->move_after(other.m_ptr);}

    //! Déplace le noeud pointé par other en fils benjamin du noeud pointé par pos.
    template<class iter_tree> void move_back(iter_tree pos, iter_tree other)
        {pos.m_ptr->move_back(other.m_ptr);}

    //! Déplace le noeud pointé par other en fils ainé du noeud pointé par pos.
    template<class iter_tree> void move_front(iter_tree pos, iter_tree other)
        {pos.m_ptr->move_front(other.m_ptr);}

    //! Renvoie un arbre contenant seulement une copie du noeud pointé par pos et de ses enfants directs.
    template<class iter_tree> tree<T> parentWithChilds(iter_tree pos);

    //! Ajoute un fils benjamin au noeud pointé par pos de donnée data.iterator Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_back(iter_tree pos, const T & data = T()) {
        auto * node = new Item(data);
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos de donnée data. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_back(iter_tree pos, T && data) {
        auto * node = new Item(std::move(data));
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos correspondant à une copie du noeud (et des descendants) pointé par other.
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree > iter_tree push_back(iter_tree pos, const const_iter_tree other) {
        auto * node = new Item(other.item());
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos correspondant à la racine de tree (avec ses descendants).
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iterator push_back(iter_tree pos, tree<T> && tree) {
        auto * node = tree.m_root;
        pos.m_ptr->push_back(node);
        tree.m_root = nullptr;
        return node;
    }

    //! Ajoute count fils benjamins au noeud pointé par pos de donnée data. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_back(iter_tree pos, size_type count, const T & data)
        {return add(pos, count, data, this->push_back);}

    //! Ajoute de nouveaux fils benjamins au noeud pointé par pos de donnée correspondant au la plage d'itérateur [first, last).
    //! Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree, class InputIt> iter_tree push_back(iter_tree pos, InputIt first, InputIt last)
        {return add(pos, first, last, this->push_back);}

    //! Ajoute de nouveaux fils benjamins au noeud pointé par pos de donnée contenu dans ilist.
    //! Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_back(iter_tree pos, std::initializer_list<T> ilist)
        {return push_back(pos,ilist.begin(),ilist.end());}

    //! Ajoute un fils ainé au noeud pointé par pos de donnée data. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_front(iter_tree pos, const T & data = T()) {
        auto * node = new Item(data);
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos de donnée data. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_front(iter_tree pos, T && data) {
        auto * node = new Item(std::move(data));
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos correspondant à une copie du noeud (et des descendants) pointé par other.
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree> iter_tree push_front(iter_tree pos, const_iter_tree other) {
        auto * node = new Item(other.item());
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos correspondant à la racine de tree (avec ses descendants).
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_front(iter_tree pos, tree<T> && tree) {
        auto * node = tree.m_root;
        pos.m_ptr->push_front(node);
        tree.m_root = nullptr;
        return node;
    }

    //! Ajoute count fils ainés au noeud pointé par pos de donnée data. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_front(iter_tree pos, size_type count, const T & data)
        {return add(pos, count, data, this->push_front);}

    //! Ajoute de nouveaux fils ainés au noeud pointé par pos de donnée correspondant au la plage d'itérateur [first, last).
    //! Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree, class InputIt> iter_tree push_front(iter_tree pos, InputIt first, InputIt last)
        {return add(pos, first, last, this->push_front);}

    //! Ajoute de nouveaux fils ainés au noeud pointé par pos de donnée contenu dans ilist.
    //! Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_front(iter_tree pos, std::initializer_list<T> ilist)
        {return push_front(pos,ilist.begin(),ilist.end());}

    //! Affectation par recopie.
    tree & operator = (const tree & tree) {
        delete m_root;
        m_root = new Item(*tree.m_root);
        return *this;
    }

    //! Affectation par deplacement.
    tree & operator = (tree && tree) {
        delete m_root;
        m_root = tree.m_root;
        tree.m_root = nullptr;
        return *this;
    }

    //! Renvoie une référence sur la donnée de chemin pos (sans vérification).
    T & operator[](std::list<int> pos) {
        auto i = begin();
        return *i[pos];
    }

    //! Renvoie une référence sur la donnée de chemin pos (sans vérification).
    const T & operator[](std::list<int> pos) const {
        auto i = cbegin();
        return *i[pos];
    }

    //! Supprime tous les noeuds (et leurs descendant) de donnée data.
    //! Si la racine est de donnée data, seulement les descendants sont supprimés.
    size_type remove(const T & data) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const const_abstract_iterator & )) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_prevsuiv_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type removeIf(U predicat) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const T & )) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type removeIfData(U predicat) noexcept;

    //! Supprime toutes les feuilles (actuelle ou obtenue) de donnée data.
    size_type removeLeaf(const T & data) noexcept
        {return removeLeafIf([&data](const const_abstract_iterator & iter)->bool{return data == *iter;});}

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const const_abstract_iterator & )) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type removeLeafIf(U predicat) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const T & )) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type removeLeafIfData(U predicat) noexcept
        {return removeLeafIf([&predicat](const const_abstract_iterator & iter)->bool{return predicat(*iter);});}

    //! Teste si l'arbre est réduit à sa racine (valide).
    bool rootLeaf() const noexcept
        {return m_root && m_root->leaf();}

    //! Retourne le nombre de noeuds de l'arbre.
    size_type size() const noexcept
        {return m_root->size();}

    //! Echange le contenu de l'arbre avec other.
    void swap(tree & other) noexcept
        {std::swap(m_root,other.m_root);}

    //! Echange les positions des noeuds pointé par pos1 et pos2, ainsi que leurs descendant.
    template<class iter_tree> void swap(iter_tree pos1, iter_tree pos2) noexcept;

protected:
    //! Constructeur, prend le neud transmis pour racine (doit être créer dynamiquement et ne doit pas avoir de parent).
    tree(Item * root) noexcept
        : m_root(root) {}

    //! Ajoute count nouveaux noeuds de donnée data au noeud pointé par pos selon la méthode ajout.
    //! Renvoie un pointeur sur le dernier noeud créé.
    template<class iter_tree, class Ajout> iter_tree add(iter_tree pos, size_type count, const T & data, Ajout ajout);

    //! Ajoute de nouveaux noeuds de donnée correspondant au la plage d'itérateur [first, last)
    //! au noeud pointé par pos selon la méthode ajout.
    //! Renvoie un pointeur sur le dernier noeud créé.
    template<class iter_tree, class InputIt, class Ajout> iter_tree add(iter_tree pos, InputIt first, InputIt last, Ajout ajout);
};

//////////////// tree //////////////////

template<class T> template<class iter_tree, class Ajout> iter_tree
                        tree<T>::add(iter_tree pos, size_type count, const T & data, Ajout ajout) {
    abstract_iterator node = pos;
    while(count) {
        node = ajout(node,data);
        --count;
    }
    return node.m_ptr;
}

template<class T> template<class iter_tree, class InputIt, class Ajout> iter_tree
                        tree<T>::add(iter_tree pos, InputIt first, InputIt last, Ajout ajout) {
    abstract_iterator node = pos;
    while(first != last) {
        node = ajout(node,*first);
        ++first;
    }
    return node.m_ptr;
}

template<class T> template<class iter_tree> iter_tree tree<T>::erase(iter_tree pos) noexcept {
    if(pos.root()) {
        clear();
        return pos;
    }
    else {
        brother_iterator iter = pos;
        delete (iter++).itemPtr();
        return iter;
    }
}

template<class T> template<class iter_tree> tree<T> tree<T>::parentWithChilds(iter_tree pos) {
    tree<T> tree(*pos);
    auto root = tree.begin();
    if(!pos.leaf()) {
        for(auto i = pos.beginChild(); i; ++i)
            tree.push_back(root,*i);
    }
    return tree;
}

template<class T> typename tree<T>::size_type tree<T>::remove(const T & data) noexcept {
    auto i = begin();
    size_type comp = 0;
    while (i) {
        if(*i == data) {
            auto j = i;
            i.to_nextBrotherIfNotUncle();
            erase(j);
            ++comp;
        }
        else
            ++i;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::removeIf(U predicat) noexcept {
    auto i = beginPrevsuiv();
    size_type comp = 0;
    while(i) {
        if(predicat(i))
            erase(i++);
        else
            ++i;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::removeIfData(U predicat) noexcept {
    auto i = begin();
    size_type comp = 0;
    while(i) {
        if(predicat(*i))
            erase(i++);
        else
            ++i;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::removeLeafIf(U predica) noexcept {
    auto i = begin();
    size_type comp = 0;
    i.to_firstLeaf();
    while (i) {
        if(predica(i)) {
            auto j = i;
            if(i.firstBrother() && i.lastBrother())
                    i.to_parent();
            else {
                i.to_nextBrotherIfNotUncle();
                if(i)
                    i.to_firstLeaf();
            }
            erase(j);
            ++comp;
        }
        else {
            i.to_nextBrotherIfNotUncle();
            if(i)
                i.to_firstLeaf();
        }
    }
    return comp;
}

template<class T> template<class iter_tree> void tree<T>::swap(iter_tree pos1, iter_tree pos2) noexcept {
    if(pos1.root() || pos2.root()){
        std::swap(pos1.m_ptr->m_data,pos2.m_ptr->m_data);
        for(auto i = pos1.beginChild(); i; ++i)
            i.m_ptr->m_parent = pos2.m_ptr;
        for(auto i = pos2.beginChild(); i; ++i)
            i.m_ptr->m_parent = pos2.m_ptr;
        std::swap(pos1.m_ptr->m_firstChild,pos2.m_ptr->m_firstChild);
        std::swap(pos1.m_ptr->m_lastChild,pos2.m_ptr->m_lastChild);
    }
    else {
        std::swap(pos1.m_ptr->m_parent,pos2.m_ptr->m_parent);
        std::swap(pos1.m_ptr->m_prevBrother,pos2.m_ptr->m_prev_Brother);
        std::swap(pos1.m_ptr->m_nextBrother,pos2.m_ptr->m_nextBrother);
        if(pos1.m_ptr->m_prevBrother)
            pos1.m_ptr->m_prevBrother->m_nextBrother = pos1.m_ptr;
        if(pos1.m_ptr->nextBrother)
            pos1.m_ptr->m_nextBrother->m_prevBrother = pos1.m_ptr;
        if(pos2.m_ptr->m_prevBrother)
            pos2.m_ptr->m_prevBrother->m_nextBrother = pos2.m_ptr;
        if(pos2.m_ptr->nextBrother)
            pos2.m_ptr->m_nextBrother->m_prevBrother = pos2.m_ptr;
    }
}

//////////////// Item //////////////////

template<class T> tree<T>::Item::Item(const Item & item)
    : m_data(item.m_data) {
    if(!item.leaf())
        for(auto i = item.cbeginChild(); i; ++i)
            push_back(new Item(i.item()));
}

template<class T> tree<T>::Item::Item(Item && item) noexcept(noexcept (T(std::move(item.m_data))))
    : m_firstChild(item.m_firstChild),
      m_lastChild(item.m_lastChild),
      m_data(std::move(item.m_data)) {
    if(!item.leaf())
        for(auto i = item.beginChild(); i; ++i)
            i.setParent(this);
    item.becomeRoot();
    item.m_firstChild = item.m_lastChild = nullptr;
}

template<class T> template<class U> tree<T>::Item::Item(const typename tree<U>::Item & item)
    : m_data(item.data()) {
    if(!leaf())
        for(auto i = item.cbeginChild(); i; ++i)
            push_back(new Item(i.item()));
}

template<class T> template<class U,class F> tree<T>::Item::Item(const typename tree<U>::Item & item, F  usine)
    : m_data(usine(item.data())) {
    if(!leaf())
        for(auto i = item.cbeginChild(); i; ++i)
            push_back(new Item(i.item(),usine));
}

template<class T> tree<T>::Item::~Item() {
    changeHeredite(nullptr);
    if(!leaf()){
        auto i = beginChild();
        while(i) {
            i.setParent(nullptr);
            delete (i++).itemPtr();
        }
    }
}

template<class T> void tree<T>::Item::changeHeredite(Item *parent) noexcept {
    if(m_parent) {
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

template<class T> void tree<T>::Item::clearChilds() noexcept {
    if(!leaf()) {
        auto i = beginChild();
        while(i) {
            i.setParent(nullptr);
            delete (i++).itemPtr();
        }
        m_firstChild = m_lastChild = nullptr;
    }
}

template<class T> void tree<T>::Item::erase(Item * last) noexcept {
    if(!last && firstBrother())
        m_parent->clearChilds();
    else if(!last || m_parent == last->m_parent){
        if(!last) {
            m_prevBrother->m_nextBrother = nullptr;
            m_parent->m_lastChild = m_prevBrother;
        }
        else {
            last->m_prevBrother->m_nextBrother = nullptr;
            last->m_prevBrother = m_prevBrother;
            if(firstBrother())
                m_parent->m_firstChild = last;
            else
                m_prevBrother->m_nextBrother = last;
        }
        auto * ptr = this;
        while (ptr) {
            auto * delPtr = ptr;
            delPtr->m_parent = nullptr;
            ptr = ptr->m_nextBrother;
            delete delPtr;
        }
    }
}

template<class T> void tree<T>::Item::insert(Item * item) noexcept {
    if(m_parent){
        item->changeHeredite(m_parent);
        item->m_prevBrother = m_prevBrother;
        item->m_nextBrother = this;
        if(m_prevBrother)
            m_prevBrother->m_nextBrother = item;
        else
            m_parent->m_firstChild = item;
        m_prevBrother = item;
    }
    else
        push_front(item);
}

template<class T> void tree<T>::Item::insert_after(Item * item) noexcept {
    if(m_parent) {
        item->changeHeredite(m_parent);
        item->m_prevBrother = this;
        item->m_nextBrother = m_nextBrother;
        if(m_nextBrother)
            m_nextBrother->m_prevBrother = item;
        else
            m_parent->m_lastChild = item;
        m_nextBrother = item;
    }
    else
        push_back(item);
}

template<class T> typename tree<T>::Item & tree<T>::Item::operator = (const Item & item) {
    m_data = item.m_data;
    clearChilds();
    if(!item.leaf())
        for(auto i = item.cbeginChild(); i; ++i)
            push_back(new Item(i.item()));
    return *this;
}

template<class T> typename tree<T>::Item & tree<T>::Item::operator = (Item && item) noexcept(noexcept (T(std::move(item.m_data)))) {
    m_data = std::move(item.m_data);
    if(!leaf())
        for(auto i = beginChild(); i; ++i) {
            i.setParent(nullptr);
            delete i.m_ptr;
        }
    if(!item.leaf()) {
        for(auto i = item.beginChild(); i; ++i)
            i.setParent(this);
        m_firstChild = item.m_firstChild;
        m_lastChild = item.m_lastChild;
    }
    else
        m_firstChild = m_lastChild = nullptr;
    item.becomeRoot();
    item.m_firstChild = item.m_lastChild = nullptr;
}

template<class T> void tree<T>::Item::push_back(Item *child) noexcept {
    child->changeHeredite(this);
    child->m_prevBrother = m_lastChild;
    child->m_nextBrother = nullptr;
    if(m_lastChild)
        m_lastChild = m_lastChild->m_nextBrother = child;
    else
        m_firstChild = m_lastChild = child;
}

template<class T> void tree<T>::Item::push_front(Item * child) noexcept {
    child->changeHeredite(this);
    child->m_prevBrother = nullptr;
    child->m_nextBrother = m_firstChild;
    if(m_firstChild)
        m_firstChild = m_firstChild->m_prevBrother = child;
    else
        m_firstChild = m_lastChild = child;
}

template<class T> typename tree<T>::size_type tree<T>::Item::size() const noexcept {
    size_type compteur = 1;
    if(!leaf()) {
        auto i = cbegin();
        auto fin = cbegin();
        fin.to_lastLeaf();
        while(i != fin) {
            ++compteur;
            ++i;
        }
    }
    return compteur;
}

template<class T> typename tree<T>::size_type tree<T>::Item::sizeChild() const noexcept {
    if(leaf())
        return 0;
    size_type n = 1;
    auto * node = m_firstChild->m_nextBrother;
    while(node) {
        ++n;
        node = node->m_nextBrother;
    }
    return n;
}

//////////////// const_abstract_iterator //

template<class T> std::list<int> tree<T>::const_abstract_iterator::index() const {
    std::list<int> list;
    const_abstract_iterator node(*this);
    while(!node.root()) {
        list.push_front(node.position());
        node.to_parent();
    }
    return list;
}

template<class T> int tree<T>::const_abstract_iterator::indexBrother() const noexcept {
    if(firstBrother())
        return 0;
    const_abstract_iterator node(*this);
    auto n = 0;
    while(node) {
        node.to_prevBrother();
        ++n;
    }
    return n;
}

template<class T> template<class iter> iter & tree<T>::const_abstract_iterator::opAddAssign(int n) noexcept {
    if(n > 0)
        while(n && iter::operator++())
            --n;
    else if(n < 0)
        while(n && iter::operator--())
            ++n;
    return *this;
}

template<class T> void tree<T>::const_abstract_iterator::toBrother(int n) noexcept {
    if(m_ptr) {
        if(n > 0)
            while(n && m_ptr) {
                to_nextBrother();
                --n;
            }
        else if(n < 0)
            while(n && m_ptr) {
                to_prevBrother();
                ++n;
            }
    }
}

template<class T> void tree<T>::const_abstract_iterator::toChild(int n) noexcept {
    if(n >= 0) {
        to_firstChild();
        while(n && m_ptr) {
            to_nextBrother();
            --n;
        }
    }
    else {
        to_lastChild();
        ++n;
        while(n && m_ptr) {
            to_prevBrother();
            ++n;
        }
    }
}

template<class T> void tree<T>::const_abstract_iterator::toParent(int n) noexcept {
    if(n && m_ptr) {
        if(n>0)
            while(n && m_ptr) {
                to_parent();
                --n;
            }
        else if(n<0)
            while(n && m_ptr) {
                to_firstChild();
                ++n;
            }
    }
}

template<class T> void tree<T>::const_abstract_iterator::toPath(const std::list<T> & list) noexcept {
    if(m_ptr) {
        for(auto i = list.cbegin(); m_ptr && i != list.cend(); ++i) {
            auto j = m_ptr->beginChild();
            while(j && *j != *i)
                ++j;
            m_ptr = j.m_ptr;
        }
    }
}

//////////////// const_iterator ////////////////

template<class T> typename tree<T>::const_iterator & tree<T>::const_iterator::operator ++() noexcept {
    if(m_ptr) {
        if(m_ptr->leaf())
            to_nextBrotherIfNotUncle();
        else
            to_firstChild();
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator & tree<T>::const_iterator::operator --() noexcept {
    if(m_ptr) {
        if(m_ptr->firstBrother())
            to_parent();
        else {
            to_prevBrother();
            to_lastLeaf();
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator & tree<T>::const_iterator::operator += (int n) noexcept {
    if(m_ptr && n) {
        if (n < 0)
            return operator -=(-n);
        else
            while(n && m_ptr) {
                if(m_ptr->leaf())
                    to_nextBrotherIfNotUncle();
                else
                    to_firstChild();
                --n;
            }
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator & tree<T>::const_iterator::operator -=(int n) noexcept {
    if(m_ptr && n) {
        if (n < 0)
            return operator +=(-n);
        else
            while(n && m_ptr) {
                if(m_ptr->firstBrother())
                    to_parent();
                else {
                    to_prevBrother();
                    to_lastLeaf();
                }
            }
    }
    return *this;
}

/////////////const_reverse_iterator //////

template<class T> typename tree<T>::const_reverse_iterator & tree<T>::const_reverse_iterator::operator ++() noexcept {
    if(m_ptr) {
        if(m_ptr->leaf())
            to_prevBrotherIfNotUncle();
        else
            to_lastChild();
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator & tree<T>::const_reverse_iterator::operator --() noexcept {
    if(m_ptr) {
        if(m_ptr->lastBrother())
            to_parent();
        else {
            to_nextBrother();
            to_firstLeaf();
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator & tree<T>::const_reverse_iterator::operator += (int n) noexcept {
    if(m_ptr && n) {
        if (n < 0)
            return operator -=(-n);
        else
            while(n && m_ptr) {
                if(m_ptr->leaf())
                    to_prevBrotherIfNotUncle();
                else
                    to_lastChild();
                --n;
            }
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator & tree<T>::const_reverse_iterator::operator -=(int n) noexcept {
    if(m_ptr && n) {
        if (n < 0)
            return operator +=(-n);
        else
            while(n && m_ptr) {
                if(m_ptr->lastBrother())
                    to_parent();
                else {
                    to_prevBrother();
                    to_firstLeaf();
                }
                --n;
            }
    }
    return *this;
}

///////////// const_prevsuiv_iterator ///////

template<class T> typename tree<T>::const_prevsuiv_iterator & tree<T>::const_prevsuiv_iterator::operator++() noexcept {
    if(m_ptr) {
        if(m_sens) {
            if(m_ptr->leaf())
                m_sens = false;
            else
                to_firstChild();
        }
        else {
            if(m_ptr->lastBrother())
                to_parent();
            else {
                to_nextBrother();
                m_sens = true;
            }
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_prevsuiv_iterator & tree<T>::const_prevsuiv_iterator::operator--() noexcept {
    if(m_ptr) {
        if(m_sens) {
            if(m_ptr->firstBrother())
                to_parent();
            else {
                to_prevBrother();
                m_sens = false;
            }
        }
        else {
            if(m_ptr->leaf())
                m_sens = true;
            else
                to_lastChild();
        }
    }
    return *this;
}
}
#endif // TREE_H
