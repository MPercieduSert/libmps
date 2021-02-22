/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <utility>

#define TREE_ITER_COMMUN(ITER)protected: \
    /*! Opérateur d'affectation à partir d'un pointeur sur un noeud de l'arbre.*/ \
    ITER &operator =(item *node) {const_abstract_iterator::operator=(node); return *this;} \
    public:\
    /*! Constructeur à l'aide d'un itérateur.*/ \
    ITER (const const_abstract_iterator &it) noexcept : const_abstract_iterator(it) {} \
    /*! Opérateur d'affectation à partir d'un iterator.*/ \
    ITER &operator =(const const_abstract_iterator &it) noexcept {const_abstract_iterator::operator=(it); return *this;}\
    /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
     *
     *positionne l'itérateur sur le n ieme frère suivant. L'argument n peut être négatif.
     *Si n est trop négatif ou trop grand, l'itérateur sera nul.
     */ \
    ITER &to_brother(int n) noexcept {const_abstract_iterator::to_brother(n); return *this;} \
    /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
     *
     *positionne l'itérateur sur le n ieme frère suivant.
     *Si n est trop grand, l'itérateur est positionné sur le dernier des frères.
     */ \
    ITER &to_brotherU(size_type n) noexcept {const_abstract_iterator::to_brotherU(n); return *this;} \
    /*! \brief Place l'itérateur sur le descendant direct d'indice n.
     *
     *Place un pointeur sur le descendant direct d'indice n. Les indices négatifs sont permis.
     *Si position n'appartient pas à l'intervale [-nombre de fils, nombre de fils-1],
     *l'itérateur sera nul.
     */ \
    ITER &to_child(int n) noexcept {const_abstract_iterator::to_child(n); return *this;} \
    /*! \brief Place un pointeur sur le descendant direct d'indice n.
     *
     *Place un pointeur sur le descendant direct d'indice n.
     *Si n > nombre de fils-1],
     *l'itérateur sera placé sur le fils ainé.
     */ \
    ITER &to_childU(size_type n) noexcept {const_abstract_iterator::to_childU(n); return *this;} \
    /*! Place l'itérateur sur l'ainé des frères du noeud courant.*/ \
    ITER &to_first_brother() noexcept {const_abstract_iterator::to_first_brother(); return *this;} \
    /*! Place l'itérateur sur l'ainé des fils du noeud courant.*/ \
    ITER &to_first() noexcept {const_abstract_iterator::to_first(); return *this;} \
    /*! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant.*/ \
    ITER &to_first_leaf() noexcept {const_abstract_iterator::to_first_leaf(); return *this;} \
    /*! Place l'itérateur sur le frère benjamin du noeud courant.*/ \
    ITER &to_last_brother() noexcept {const_abstract_iterator::to_last_brother(); return *this;} \
    /*! Place l'itérateur sur le fils benjamin  du noeud courant.*/ \
    ITER &to_last() noexcept {const_abstract_iterator::to_last(); return *this;} \
    /*! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant.*/ \
    ITER &to_last_leaf() noexcept {const_abstract_iterator::to_last_leaf(); return *this;} \
    /*! Place l'itérateur sur le frère cadet du noeud courant.*/ \
    ITER &to_next() noexcept {const_abstract_iterator::to_next(); return *this;} \
    /*! Place l'itérateur sur le parent du noeud courant.*/ \
    ITER &to_parent() noexcept {const_abstract_iterator::to_parent(); return *this;} \
    /*! \brief positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
     *Si n est négatif, l'itérateur est positionné sur le n ième fils ainé.
     *
     *positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
     *Si n est plus grand que le nombre de parents l'itérateur est positionné sur la racine.
     *Si n est négatif, l'itérateur est positionné sur le n ième fils ainé, en s'arrétant sur le dernier si n est trop grand.
     */ \
    ITER &to_parent(int n) noexcept {const_abstract_iterator::to_parent(n); return *this;} \
    /*! \brief positionne l'itérateur sur le noeud de chemin de données list.
     *
     *positionne l'itérateur sur le noeud de chemin des données list des différents descendants empruntés.
     *Si plusieurs noeuds frères possèdent les mêmes données c'est l'ainé qui sera emprunté.
     *Si l'une des donnée de la liste n'existe pas, l'itérateur renjoyé sera nul.
     */ \
    ITER &to_path(const std::list<T> &list) noexcept {const_abstract_iterator::to_path(list); return *this;} \
    /*! Place l'itérateur sur le frère ainé du noeud courant.*/ \
    ITER &to_prev() noexcept {const_abstract_iterator::to_prev(); return *this;} \
    /*! Place l'itérateur sur la racine du noeud courant.*/ \
    ITER &to_root() noexcept {const_abstract_iterator::to_root(); return *this;}


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
    ITER &operator >> (T &data) noexcept {data = operator *(); return operator ++ ();} \

#define TREE_ITER_NOCONST(ITER) TREE_ITER_COMMUN(ITER) \
    /*! Opérateur de pré-incrémentation.*/ \
    ITER &operator ++() noexcept {const_##ITER::operator ++(); return *this;} \
    /*! Opérateur de pré-décrémentation.*/ \
    ITER &operator --() noexcept {const_##ITER::operator --(); return *this;} \
    /*! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.*/ \
    ITER &operator +=(int n) noexcept {const_##ITER::operator +=(n); return *this;} \
    /*! Appelle +=(-n).*/ \
    ITER &operator -=(int n) noexcept {return operator +=(-n);} \
    /*! Opérateur de post-incrémentation.*/ \
    ITER operator ++(int) noexcept {ITER it(*this); const_##ITER::operator ++(); return it;} \
    /*! Opérateur de post-décrémentation.*/ \
    ITER operator --(int) noexcept {ITER it(*this); const_##ITER::operator --(); return it;} \
    /*! L'addition d'un itérateur i et d'un entier n, produit une copie de l'itérateur i incrémenté n fois.*/ \
    ITER operator +(int n) const noexcept {ITER it(*this); it.const_##ITER::operator +=(n); return it;} \
    /*! La soustraction d'un itérateur i et d'un entier n, produit une copie de l'itérateur i décrémenté n fois.*/ \
    ITER operator -(int n) const noexcept {ITER it(*this); it.const_##ITER::operator -=(n); return it;} \
    /*! Affecte à data la donnée associée au noeud courant puis replace ce dernier sur le noeud suivant.*/ \
    ITER &operator >> (T &data) noexcept {data = const_##ITER::operator *(); return operator ++ ();} \

namespace mps {

/*! \ingroup groupeConteneur
 *\brief Cette classe represente un arbre dont les noeuds sont constitués par des item<T>.
 *
 *Cette classe represente un arbre. Elle est constituée de la racine d'un arbre dont les noeuds sont constitués par des item<T>.
 */
template<class T> class tree {
public:
    using size_type = unsigned;
    class abstract_iterator;
    class brother_iterator;
    class const_abstract_iterator;
    class const_brother_iterator;
    class const_iterator;

protected:
    //////////////////////////////////// item//////////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Cette classe est une représentation des noeuds d'un arbre.
     *
     *Cette classe représente les noeuds d'un arbre contenant les informations suivantes:
     *- une donnée de type T attachée au noeud,
     *- un pointeur sur le item parent dans l'arbre (l'élément racine posséde un pointeur parent nul),
     *- un pointeur sur le frère précécent (le premier fils posséde un pointeur frère précédent nul).
     *- un pointeur sur le frère suivant (le dernier fils posséde un pointeur frère suivant nul).
     *- un pointeur sur le première fils (une feuille posséde un pointeur premier fils nul).
     *- un pointeur sur le dernier fils (une feuille posséde un pointeur dernier fils nul).
     *
     *L'itérateur item::brother_iterator permet de parcourir une fratrie.
     *L'itérateur item::iterator propose différente façon de parcourir les noeuds d'un arbre de item.
     *
     *\sa tree
     */
    class item {
        friend const_abstract_iterator;
        friend abstract_iterator;
    protected:
        item *m_parent = nullptr;       //!< Pointeur vers le item parent dans l'arbre, est nul si le item est la racine de l'arbre.
        item *m_prev = nullptr;         //!< Pointeur vers le frère précédent, est nul si le item est le premier frère.
        item *m_next = nullptr;         //!< Pointeur vers le frère suivant, est nul si le item est le dernier frère.
        item *m_first = nullptr;        //!< Pointeur vers le premier fils, est nul si le noeud est une feuille.
        item *m_last = nullptr;         //!< Pointeur vers le dernier fils, est nul si le noeud est une feuille..
        T m_data;                       //!< Donnée accrochée au noeud de l'arbre.

    public:
        //! Constructeur par default, par défaut un noeud est la racine d'un l'arbre.
        item() = default;

        //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants.
        item(const item &node);

        //! Constructeur par déplacement.
        //! Après déplacement, node n'appartient plus à un arbre, il faut le détruire.
        item(item &&node) noexcept(noexcept(T(std::move(node.m_data))));

        //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
        item(const T &data) noexcept(noexcept(T(data)))
            : m_data(data) {}

        //! Constructeur avec une donnée associée au noeud. Par défaut un noeud est la racine d'un arbre.
        item(T &&data) noexcept(noexcept(T(std::move(data))))
            : m_data(std::move(data)) {}

        //! Constructeur de donné en place.
        template< class... Args, typename = std::enable_if_t<std::is_convertible<T, Args...>::value>>
                    item(Args &&... args) : m_data(std::forward<Args>(args)...) {}

        //! Constructeur de recopie. Recopie également récursivement l'ensemble des descendants,
        //! T doit posséder un constructeur à partie de U.
        template<class U> item(const typename tree<U>::item &node);

        //! Constructeur à partir d'un item de donnée U et une usine construisant une donnée T grâce à une donnée U.
        template<class U, class F> item(const typename tree<U>::item &node, F usine);

        //! Destructeur. Détruit également récursivement l'ensemble des descendants.
        ~item();

        //! Transforme le noeud d'un arbre en racine.
        void become_root() noexcept {
            change_heredite(nullptr);
            m_prev = m_next = nullptr;
        }

        ///////////////// begin ////////////

        //! Crée un brother_iterator initialisé sur le fils ainé.
        brother_iterator begin_child() noexcept
            {return m_first;}

        //! Crée un const_iterator initialisé sur ce noeud.
        const_iterator cbegin() const noexcept
            {return const_cast<item *>(this);}

        //! Crée un const_brother_iterator initialisé sur le fils ainé.
        const_brother_iterator cbegin_child() const noexcept
            {return m_first;}

        //! Change l'hérédité du noeud en le supprimant des descendants de son noeud parent
        //! avant de remplacer le parent du noeud par le nouveau parent.
        void change_heredite(item *parent) noexcept;

        //! Détruit tous les descendants du noeud.
        void clear_childs() noexcept;

        //! Transmet une référence sur la donnée associée au noeud.
        T &data() noexcept
            {return m_data;}

        //! Transmet une référence constante sur la donnée associée au noeud.
        const T &data() const noexcept
            {return m_data;}

        //! Supprime les noeuds appartenant à [this,last) et leurs decendants.
        //! Le noeud last doive être de la même fratrie sauf si last est nul.
        void erase(item *last) noexcept;

        //! Test si le noeud  est l'ainé des frères.
        bool first() const noexcept
            {return !m_prev;}

        //! Insert le noeud pointé par node avant this dans la fratrie sauf si this est la racine,
        //! dans ce cas node est inseré en tant que fils ainé.
        //! node ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
        void insert(item *node) noexcept;

        //! Insert le noeud pointé par node après this dans la fratrie sauf si this est la racine,
        //! dans ce cas node est inseré en tant que fils benjamin.
        //! node ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
        void insert_after(item *node) noexcept;

        //! Test si le noeud est le benjamin des frères.
        bool last() const noexcept
            {return !m_next;}

        //! Test si le noeud posséde un descendant direct.
        bool leaf() const noexcept
            {return !m_first;}

        //! Insert le noeud pointé par node avant this dans la fratrie sauf si this est la racine,
        //! dans ce cas node est inseré en tant que fils ainé.
        //! Si node pointe sur la racine d'un arbre, le noeud inséré est le déplacement de node.
        void move(item *node) {
            if(node->m_parent)
                insert(node);
            else
                insert(new item(std::move(*node)));
        }

        //! Insert le noeud pointé par node après this dans la fratrie sauf si this est la racine,
        //! dans ce cas node est inseré en tant que fils ainé.
        //! Si node pointe sur la racine d'un arbre, le noeud inséré est le déplacement de node.
        void move_after(item *node) {
            if(node->m_parent)
                insert_after(node);
            else
                insert_after(new item(std::move(*node)));
        }

        //! Ajout le noeud pointé par node en tant que benjamin des descendants du noeud.
        //! Si node pointe sur la racine d'un arbre, le noeud ajouté est le déplacement de node.
        void move_back(item *node) {
            if(node->m_parent)
                push_back(node);
            else
                push_back(new item(std::move(*node)));
        }

        //! Ajout le noeud pointé par node en tant qu'ainé des descendants du noeud.
        //! Si node pointe sur la racine d'un arbre, le noeud ajouté est le déplacement de node.
        void move_front(item *node) {
            if(node->m_parent)
                push_front(node);
            else
                push_front(new item(std::move(*node)));
        }

        /*!
         *\brief Opérateur d'affectation de recopie.
         *
         *L'opérateur d'affectation modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds
         *pour les remplacer par une copie de ceux de tree.
         */
        item &operator = (const item &node);

        /*!
         *\brief Opérateur d'affectation par déplacement.
         *
         *L'opérateur d'affectation par déplacement, modifie la donnée du noeud par celle de tree, détruit tous les descendants du noeuds,
         *change en tree racine, puis récupére les liens de descendance de tree avant de les supprimer de tree.
         */
        item &operator = (item &&node) noexcept(noexcept (T(std::move(node.m_data))));

        //! Retourne la position du noeud dans la fratrie.
        size_type position() const noexcept {
            size_type pos = 0;
            auto node = m_prev;
            while (node) {
                node = node->m_prev;
                ++pos;
            }
            return pos;
        }

        /*!
         *\brief Ajoute le noeud pointé par child en tant que benjamin des descendants du noeud.
         *node ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
         *
         *Le noeud pointé par child est ajouté en dernière position dans la fratrie des descendants directs.
         *Si le descendant ajouté avait déjà un parent,
         *ce descendant est alors supprimé de la liste des descendants directs
         *de ce parent avant que ne soit changé le pointeur sur le parent du noeud ajouté.
         */
        void push_back(item *child) noexcept;

        /*!
         *\brief Ajoute le noeud pointé par child en tant qu'ainé des descendants du noeud.
         *node ne doit pas pointé sur la racine d'un arbre appartenant à un objet tree.
         *
         *Le noeud pointé par child est ajouté en première position dans la fratrie des descendants directs.
         *Si le descendant ajouté avait déjà un parent,
         *ce descendant est alors supprimé de la liste des descendants directs
         *de ce parent avant que ne soit changé le pointeur sur le parent du noeud ajouté.
         */
        void push_front(item *child) noexcept;

        /*!
         *\brief Remplace le noeud par le noeud node dans l'arbre ainsi que leur déscendants.
         *
         *Remplace le noeud par le noeud node dans l'arbre ainsi que leur déscendants.
         *Le noeud et ses descendants, privée éventuellemment de node et de ses descendants, sont détruits.
         */
        void replace(item *node) noexcept;

        //! Modifie la donnée associée au noeud.
        void set_data(const T &data) noexcept(noexcept (T::operateur = (data)))
            {m_data = data;}

        //! Modifie la donnée associée au noeud.
        void set_data(T &&data) noexcept(noexcept (T::operateur = (std::move(data))))
            {m_data = std::move(data);}

        /*! \brief Renvoie le nombre de noeuds du sous-arbre ayant ce noeud pour racine.
         *
         *Méthode déterminant le nombre de noeuds du sous-arbre ayant ce noeud pour racine,
         *c'est-à-dire le nombre de descendants directs et indirects augmenté de un pour prendre en compte ce noeud.
         */
        size_type size() const noexcept;

        //! Renvoie le nombre de descendants directs.
        size_type size_child() const noexcept;
    };

public:
    ///////////////////////////////////// const_abstract_iterator //////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Classe parente des itérateurs sur un arbre.
     */
    class const_abstract_iterator {
        friend tree;
    protected:
        item *m_ptr;     //!< Pointeur sur le noeud courant.

    public:
        //! Constructeur.
        const_abstract_iterator(void *node = nullptr) noexcept
            : m_ptr(static_cast<item *>(node)) {}

        ///////////////// begin ////////////

        //! Crée un const_brother_iterator initialisé sur le fils ainé.
        const_brother_iterator cbegin_child() const noexcept
            {return m_ptr->m_first;}

        //! Crée un const_brother_iterator initialisé sur le fils benjamin.
        const_brother_iterator crbegin_child() const noexcept
            {return m_ptr->m_last;}

        //! Test si le noeud courant est l'ainé des frères.
        bool first() const noexcept
            {return m_ptr &&!m_ptr->m_prev;}

        //! Détermine la position du noeud courant par rapport à la racine.
        std::list<int> index() const;

        //! Détermine la position du noeud courant dans la fratrie.
        int index_brother() const noexcept;

        static bool Less_ptr (const_abstract_iterator it_1, const_abstract_iterator it_2) noexcept
            {return it_1.m_ptr < it_2.m_ptr;}

        //! Test si le noeud courant est l'ainé des frères.
        bool last() const noexcept
            {return m_ptr &&!m_ptr->m_next;}

        //! Test si le noeud courant est une feuille.
        bool leaf() const noexcept
            {return m_ptr &&!m_ptr->m_first;}

        //! Conversion en booléen.
        operator bool() const noexcept
            {return m_ptr;}

        //! Renvoie une référence sur la donnée du noeud courant.
        const T &operator *() const noexcept
            {return m_ptr->data();}

        //! Déférencement sur la donnée du noeud courant.
        T const *operator ->() const noexcept
            {return &m_ptr->data();}

        //! Test d'égalité entre deux itérateurs.
        bool operator ==(const const_abstract_iterator &it) const noexcept
            {return m_ptr == it.m_ptr;}

        //! Test d'égalité entre deux itérateurs.
        bool operator !=(const const_abstract_iterator &it) const noexcept
            {return m_ptr != it.m_ptr;}

        //! Test d'ordre inférieur strict entre deux itérateurs.
        bool operator <(const const_abstract_iterator &it) const noexcept;

        //! Crée un const_brother_iterator initialisé sur le parent.
        const_abstract_iterator parent() const noexcept
            {return m_ptr ? m_ptr->m_parent : nullptr;}

        //! Renvoie la liste des données de la racine jusqu'au noeud inclus.
        std::list<T> path() {
            std::list<T> list;
            while(m_ptr){
                list.push_front(m_ptr->data());
                v_to_parent();
            }
            return list;
        }

        //! Retourne la position du noeud dans la fratrie.
        size_type position() const noexcept
            {return  m_ptr ? m_ptr->position() : 0;}

        //! Acceseur du pointeur.
        void *ptr() const
            {return m_ptr;}

        //! Teste si le neud courant est le sommet de l'arbre.
        bool root() const noexcept
            {return m_ptr &&!m_ptr->m_parent;}

        //! Retourne le nombre de descendants du noeud courant.
        size_type size() const noexcept
            {return m_ptr ? m_ptr->size() : 0;}

        //! Retourne le nombre de descendants directs du noeud courant.
        size_type size_child() const noexcept
            {return m_ptr ? m_ptr->size_child() : 0;}

    protected:
        //! Constructeur.
        const_abstract_iterator(item *node) noexcept
            : m_ptr(node) {}

        //! Renvoie une référence sur le noeud courant.
        const item &item_ref() const noexcept
            {return *m_ptr;}

        //! Opérateur d'affectation à partir d'un pointeur sur un noeud de l'arbre.
        const_abstract_iterator &operator =(item *node) noexcept {
            m_ptr = node;
            return *this;
        }

        //! _template des opérateur +=.
        template<class iter> iter &op_add_assign(int n) noexcept;

        //! Déplace l'itérateur sur le fils de position n (sans vérification).
        template<class iter, class broIter> iter &op_brother(int n) noexcept {
            broIter it(m_ptr);
            it += n;
            this->operator=(it);
            return *this;
        }

        /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
         *
         *positionne l'itérateur sur le n ieme frère suivant. L'argument n peut être négatif.
         *Si n est trop négatif, l'itérateur est positionné sur le premier des frères
         *et si n est trop grand, l'itérateur est positionné sur le dernier des frères.
         */
        void to_brother(int n) noexcept;

        /*! \brief Place l'itérateur sur le n ieme frère du noeud courant.
         *
         *positionne l'itérateur sur le n ieme frère suivant.
         *Si n est trop grand, l'itérateur est positionné sur le dernier des frères.
         */
        void to_brotherU(size_type n) noexcept {
            while(n &&m_ptr) {
                v_to_next();
                --n;
            }
        }

        /*! \brief Place un pointeur sur le descendant direct d'indice n.
         *
         *Place un pointeur sur le descendant direct d'indice n. Les indices négatifs sont permis.
         *Si position n'appartient pas à l'intervale [-nombre de fils, nombre de fils-1],
         *l'itérateur sera placé sur le fils ainé ou benjamin.
         */
        void to_child(int n) noexcept;

        /*! \brief Place un pointeur sur le descendant direct d'indice n.
         *
         *Place un pointeur sur le descendant direct d'indice n.
         *Si position > nombre de fils-1],
         *l'itérateur sera placé sur le fils ainé.
         */
        void to_childU(size_type n) noexcept {
            to_first();
            to_brother(n);
        }

        //! Place l'itérateur sur l'ainé des fils du noeud courant.
        void to_first() noexcept {
            if(m_ptr)
                v_to_first();
        }

        //! Place l'itérateur sur l'ainé des frères du noeud courant.
        void to_first_brother() noexcept {
            if(m_ptr &&m_ptr->m_parent)
                v_to_first_brother();
        }

        //! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant.
        void to_first_leaf() noexcept {
            if(m_ptr)
                v_to_first_leaf();
        }

        //! Place l'itérateur sur le fils benjamin  du noeud courant.
        void to_last() noexcept {
            if(m_ptr)
                v_to_last();
        }

        //! Place l'itérateur sur le frère benjamin du noeud courant.
        void to_last_brother() noexcept {
            if(m_ptr &&m_ptr->m_parent)
                v_to_last_brother();
        }

        //! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant.
        void to_last_leaf() noexcept {
            if(m_ptr)
                v_to_last_leaf();
        }

        //! Place l'itérateur sur le frère cadet du noeud courant.
        void to_next() noexcept {
            if(m_ptr)
                v_to_next();
        }

        //! Place l'itérateur sur le parent du noeud courant.
        void to_parent() noexcept {
            if(m_ptr)
                v_to_parent();
        }

        /*! \brief positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         *Si n est négatif, l'itérateur est positionné sur le n ième fils ainé.
         *
         *positionne l'itérateur sur le n ieme parent du noeud courant si n est positif.
         *Si n est plus grand que le nombre de parents l'itérateur est positionné sur la racine.
         *Si n est négatif, l'itérateur est positionné sur le n ième fils ainé, en s'arrétant sur le dernier si n est trop grand.
         */
        void to_parent(int n) noexcept;


        /*! \brief positionne l'itérateur sur le noeud de chemin de données list.
         *
         *positionne l'itérateur sur le noeud de chemin des données list des différents descendants empruntés.
         *Si plusieurs noeuds frères possèdent les mêmes données c'est l'ainé qui sera emprunté.
         *Si l'une des donnée de la liste n'existe pas, l'itérateur renjoyé sera nul.
         */
        void to_path(const std::list<T> &list) noexcept;

        //! Place l'itérateur sur le frère ainé du noeud courant.
        void to_prev() noexcept {
            if(m_ptr)
                v_to_prev();
        }

        //! Place l'itérateur sur la racine du noeud courant.
        void to_root() noexcept {
            if(m_ptr)
                v_to_root();
        }

        //! positionne l'itérateur sur le frère cadet du noeud courant s'il existe.
        //! Si le noeud courant est le benjamin,
        //! on remonte jusqu'au premier parent possédant un frère cadet et l'on positionne l'itérateur dessus.
        //! Si le noeud courant est le benjamin de l'arbre, l'itérateur est placé sur le noeud virtuel nul.
        void to_next_if_not_uncle() noexcept {
            while(last())
                v_to_parent();
            if(m_ptr)
                v_to_next();
        }

        //! Place l'itérateur sur l'ainé des fils du noeud courant (en supposant la validité des pointeurs).
        void v_to_first() noexcept
            {m_ptr = m_ptr->m_first;}

        //! Place l'itérateur sur l'ainé des frères du noeud courant (en supposant la validité des pointeurs).
        void v_to_first_brother() noexcept
            {m_ptr = m_ptr->m_parent->m_first;}

        //! Place l'itérateur sur le dernier descendant de la ligné des ainé né du noeud courant (en supposant la validité des pointeurs).
        void v_to_first_leaf() noexcept {
            while(m_ptr->m_first)
                    m_ptr = m_ptr->m_first;
        }

        //! Place l'itérateur sur le fils benjamin  du noeud courant (en supposant la validité des pointeurs).
        void v_to_last() noexcept
            {m_ptr = m_ptr->m_last;}

        //! Place l'itérateur sur le frère benjamin du noeud courant (en supposant la validité des pointeurs).
        void v_to_last_brother() noexcept
            {m_ptr = m_ptr->m_parent->m_last;}

        //! Place l'itérateur sur le dernier descendant de la ligné des benjamin du noeud courant (en supposant la validité des pointeurs).
        void v_to_last_leaf() noexcept {
            while(m_ptr->m_last)
                    m_ptr = m_ptr->m_last;
        }

        //! Place l'itérateur sur le frère cadet du noeud courant (en supposant la validité des pointeurs).
        void v_to_next() noexcept
            {m_ptr = m_ptr->m_next;}

        //! Place l'itérateur sur le parent du noeud courant (en supposant la validité des pointeurs).
        void v_to_parent() noexcept
            {m_ptr = m_ptr->m_parent;}

        //! Place l'itérateur sur le frère ainé du noeud courant (en supposant la validité des pointeurs).
        void v_to_prev() noexcept
            {m_ptr = m_ptr->m_prev;}

        //! positionne l'itérateur sur le frère ainé du noeud courant s'il existe.
        //! Si le noeud courant est l'ainé,
        //! on remonte jusqu'au premier parent possédant un frère ainé et l'on positionne l'itérateur dessus.
        //! Si le noeud courant est le dernier descendant des ainés de l'arbre, l'itérateur est placé sur le noeud virtuel nul.
        void v_to_prev_if_not_uncle() noexcept {
            while(first())
                v_to_parent();
            if(m_ptr)
                v_to_prev();
        }

        //! Place l'itérateur sur la racine du noeud courant (en supposant la validité des pointeurs).
        void v_to_root() noexcept {
            while(m_ptr->m_parent)
                    m_ptr = m_ptr->m_parent;
        }
    };

    ////////////////////////////////////// abstract_iterator ////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Classe parente des itérateurs sur un arbre non constant.
     */
    class abstract_iterator : public virtual const_abstract_iterator {
        friend item;
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        using const_abstract_iterator::operator->;

        //! Renvoie un référence sur le noeud courant.
        item &item_ref() noexcept
            {return *m_ptr;}

        //! Crée un brother_iterator initialisé sur le fils ainé.
        brother_iterator begin_child() noexcept
            {return m_ptr->m_first;}

        //! Renvoie une référence sur la donnée du noeud courant.
        T &operator *() noexcept
            {return m_ptr->data();}

        //! Déférencement sur la donnée du noeud courant.
        T *operator ->() noexcept
            {return &m_ptr->data();}

    protected:
        //! Mutateur du parent du noeud courant.
        void set_parent(item *parent)
            {m_ptr->m_parent = parent;}

        //! Acceseur du pointeur.
        item *item_ptr() const
            {return m_ptr;}
    };
    ///////////////////////////////////// const_brother_iterator /////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur constant sur une fratrie.
     */
    class const_brother_iterator : public virtual const_abstract_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_next;
        using const_abstract_iterator::to_prev;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_brother_iterator)

        //! Opérateur de pré-incrémentation.
        const_brother_iterator &operator ++() noexcept
            {return to_next();}

        //! Opérateur de pré-décrémentation.
        const_brother_iterator &operator --() noexcept
            {return to_prev();}

        //! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.
        const_brother_iterator &operator +=(int n) noexcept
            {return const_abstract_iterator::template op_add_assign<const_brother_iterator>(n);}

        //! Appelle +=(-n).
        const_brother_iterator &operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    //////////////////////////////////// const_reverse_brother_iterator //////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur-inverse constant sur une fratrie.
     */
    class const_reverse_brother_iterator : public const_brother_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::to_next;
        using const_abstract_iterator::to_prev;

    public:
        using const_brother_iterator::const_brother_iterator;
        using const_brother_iterator::operator*;
        TREE_ITER_CONST(const_reverse_brother_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_brother_iterator &operator ++() noexcept
            {return to_prev();}

        //! Opérateur de pré-décrémentation.
        const_reverse_brother_iterator &operator --() noexcept
            {return to_next();}

        //! Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_reverse_brother_iterator &operator +=(int n) noexcept
            {return const_abstract_iterator::template op_add_assign<const_reverse_brother_iterator>(n);}

        //! Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
        const_reverse_brother_iterator &operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    ////////////////////////////////////////// const_iterator //////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur constant sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
     */
    class const_iterator : public virtual const_abstract_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::v_to_first;
        using const_abstract_iterator::v_to_last_leaf;
        using const_abstract_iterator::to_next_if_not_uncle;
        using const_abstract_iterator::v_to_parent;
        using const_abstract_iterator::v_to_prev;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_iterator)

        //! Opératateur de pré-incrémentation.
        //! positionne l'itérateur sur le noeud suivant, les noeuds sont parcourus uniquement à la descente.
        const_iterator &operator ++() noexcept;

        //! Opérateur de pré-décrémentation.
        const_iterator &operator --() noexcept;

        /*! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
         *Pour un n grand cet opérateur est plus efficace que d'appliquer n fois l'opérateur d'incrémemtation.
         */
        const_iterator &operator +=(int n) noexcept;

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         *Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         *l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         *a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_iterator &operator -=(int n) noexcept;
    };
    //////////////////////////////////////// const_reverse_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur-inverse constant sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les benjamins.
     */
    class const_reverse_iterator : public const_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::v_to_first_leaf;
        using const_abstract_iterator::v_to_last;
        using const_abstract_iterator::v_to_next;
        using const_abstract_iterator::v_to_parent;
        using const_abstract_iterator::v_to_prev;
        using const_abstract_iterator::v_to_prev_if_not_uncle;

    public:
        using const_iterator::const_iterator;
        using const_iterator::operator*;
        TREE_ITER_CONST(const_reverse_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_iterator &operator ++() noexcept;

        //! Opérateur de pré-décrémentation.
        const_reverse_iterator &operator --() noexcept;

        /*! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
         *Pour un n grand cet opérateur est plus efficace que d'appliquer n fois l'opérateur d'incrémemtation.
         */
        const_reverse_iterator &operator +=(int n) noexcept;

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         *Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         *l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         *a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_reverse_iterator &operator -=(int n) noexcept;
    };
    ////////////////////////////////////////// const_leaf_iterator //////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur constant sur les feuilles de l'arbre.
     */
    class const_leaf_iterator : public virtual const_abstract_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_next_if_not_uncle;
        using const_abstract_iterator::v_to_prev_if_not_uncle;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_leaf_iterator)

        //! Opératateur de pré-incrémentation.
        //! positionne l'itérateur sur la feuille suivante.
        const_leaf_iterator &operator ++() noexcept {
            to_next_if_not_uncle();
            return to_first_leaf();
        }

        //! Opérateur de pré-décrémentation.
        const_leaf_iterator &operator --() noexcept {
            v_to_prev_if_not_uncle();
            return to_last_leaf();
        }

        //! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_leaf_iterator &operator +=(int n) noexcept
            {return const_abstract_iterator::template op_add_assign<const_leaf_iterator>(n);}

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         *Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         *l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         *a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_leaf_iterator &operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    //////////////////////////////////////// const_reverse_leaf_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur-inverse constant sur les feuilles de l'arbre.
     */
    class const_reverse_leaf_iterator : public const_leaf_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        using const_abstract_iterator::to_next_if_not_uncle;
        using const_abstract_iterator::v_to_prev_if_not_uncle;

    public:
        using const_iterator::const_iterator;
        using const_iterator::operator*;
        TREE_ITER_CONST(const_reverse_leaf_iterator)

        //! Opérateur de pré-incrémentation.
        const_reverse_leaf_iterator &operator ++() noexcept {
            v_to_prev_if_not_uncle();
            return to_last_leaf();
        }

        //! Opérateur de pré-décrémentation.
        const_reverse_leaf_iterator &operator --() noexcept {
            to_next_if_not_uncle();
            return to_first_leaf();
        }

        //! \brief Incrémente n fois l'itérateur. Si n est négatif, -=-n est appliqué.
        const_reverse_leaf_iterator &operator +=(int n) noexcept
            {return const_abstract_iterator::template op_add_assign<const_reverse_leaf_iterator>(n);}

        /*! \brief Décrémente n fois l'itérateur. Si n est négatif, +=-n est appliqué.
         *Cet opérateur applique n fois l'opérateur de décrémemtation, sauf si
         *l'itérateur est décrémenté alors qu'il est positionné sur la racine,
         *a ce moment l'itérateur est positionné sur le noeud virtuel nullptr.
         */
        const_reverse_leaf_iterator &operator -=(int n) noexcept
            {return operator +=(-n);}
    };
    /////////////////////////////////////////// const_prevsuiv_iterator ///////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur constant sur l'arbre parcourant les noeuds à la descente et à la remonté en commeçant par les ainés.
     */
    class const_prevsuiv_iterator : public virtual const_abstract_iterator {
        friend tree;
    protected:
        using const_abstract_iterator::m_ptr;
        bool m_sens = true;            //!< Indicateur du sens de parcours: true -> descente; false -> remontée.
        using const_abstract_iterator::v_to_first;
        using const_abstract_iterator::v_to_last;
        using const_abstract_iterator::v_to_next;
        using const_abstract_iterator::v_to_parent;
        using const_abstract_iterator::v_to_prev;

    public:
        using const_abstract_iterator::const_abstract_iterator;
        using const_abstract_iterator::operator*;
        TREE_ITER_CONST(const_prevsuiv_iterator)

        //! positionne l'itérateur sur le noeud suivant dans le parcours de type suivant-précédent:
        //! les noeuds sont parcourus deux fois, à la descente et à la remontée.
        const_prevsuiv_iterator &operator ++() noexcept;

        //! positionne l'itérateur sur le noeud précédent dans le parcours de type suivant-précédent:
        //! les noeuds sont parcours deux fois, à la descente et à la remonté.
        const_prevsuiv_iterator &operator --() noexcept;

        //! Incrémente n fois l'itérateur. Si n est négatif, décrémente -n fois l'itérateur.
        const_prevsuiv_iterator &operator +=(int n) noexcept
            {return const_abstract_iterator::template op_add_assign<const_prevsuiv_iterator>(n);}

        //! Appelle +=(-n).
        const_prevsuiv_iterator &operator -=(int n) noexcept
            {return operator +=(-n);}

        //! Renvoie le sens de parcours de l'itérateur: true -> descente; false -> remontée.
        bool sens() const noexcept
            {return m_sens;}

        //! Modifie le sens de parcours de la méthode suivant-précédent.
        void set_sens(bool sens) noexcept
            {m_sens = sens;}
    };
    //////////////////////////////////////// brother_iterator /////////////////////////////////
    /*! \ingroup groupeConteneur
     *\brief Itérateur sur une fratrie.
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
     *\brief Itérateur sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
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
     *\brief Itérateur sur l'arbre parcourant les feuilles.
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
     *\brief Itérateursur l'arbre parcourant les noeuds à la descente et à la remonté en commeçant par les ainés.
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
     *\brief Itérateur inverse sur une fratrie.
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
     *\brief Itérateur inverse sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
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
     *\brief Itérateur inverse sur l'arbre parcourant les noeuds uniquement à la descente en commeçant par les ainés.
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
    item *m_root;      //!< Pointeur sur la racine de l'arbre.

public:
    //! Constructeur sans argument.
    tree()
        : tree(new item) {}

    //! Constructeur de recopie.
    tree(const tree &tr)
        : tree(new item(*tr.m_root)) {}

    //! Constructeur de déplacement.
    tree(tree &&tr) noexcept
        : tree(tr.m_root) {
        tr.m_root = nullptr;
    }

    //! Constructeur, crée une racine de donnée associée data et initialise l'itérateur sur cette racine.
    explicit tree(const T &data)
        : tree(new item(data)){}

    //! Constructeur, crée une racine de donnée associée data et initialise l'itérateur sur cette racine.
    explicit tree(T &&data)
        : tree(new item(std::move(data))){}

    //! Constructeur, créer un arbre possédant la même structure et les même valeur que tr,
    //! T doit posséder un constructeur à partir de U.
    template<class U> tree(const tree<U> &tr)
        : tree(new item(*(tr.m_root))) {}

    //! Constructeur d'un arbre de donnée de type T à partir d'un arbre de donnée de type U,
    //! le foncteur usine construit une donnée de type T à partir d'une donnée de type U.
    template<class U, class F> tree(const tree<U> &tr, F usine) {
        auto itU = tr.cbegin();
        m_root = new item(usine(*itU));
        auto itT = begin();
        while (itU) {
            if(!itU.leaf())
                for (auto i = itU.cbegin_child(); i; ++i)
                    push_back(itT, usine(*i));
            ++itU;
            ++itT;
        }
    }

    //! Destructeur. Détruit tous les noeuds de l'arbre.
    ~tree()
        {delete m_root;}

    //! Renvoie une référence sur la donnée de chemin list (avec vérification pouvant conduire à une exception std::out_of_range).
    T &at(std::list<int> list) {
        auto it = begin();
        return it->to(list);
    }

    //! Renvoie une référence sur la donnée de chemin list (avec vérification pouvant conduire à une exception std::out_of_range).
    const T &at(std::list<int> list) const {
        auto it = cbegin();
        return it->to(list);
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
    brother_iterator begin_brother() noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère constant initialisé sur la racine.
    const_brother_iterator cbegin_brother() const noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère inverse initialisé sur la racine.
    reverse_brother_iterator rbegin_brother() noexcept
        {return m_root;}

    //! Renvoie un itérateur de frère inverse constant initialisé sur la racine.
    const_reverse_brother_iterator crbegin_brother() const noexcept
        {return m_root;}

    /////// begin leaf_iterator ////////

    //! Renvoie un itérateur de feuille initialisé sur la première feuille.
    leaf_iterator begin_leaf() noexcept
        {return leaf_iterator(m_root).to_first_leaf();}

    //! Renvoie un itérateur de feuille constant initialisé sur la première feuille.
    const_leaf_iterator cbegin_leaf() const noexcept
        {return const_leaf_iterator(m_root).to_first_leaf();}

    //! Renvoie un itérateur de feuille inverse initialisé sur la dernière feuille.
    reverse_leaf_iterator rbegin_leaf() noexcept
        {return reverse_leaf_iterator(m_root).to_last_leaf();}

    //! Renvoie un itérateur de feuille inverse constant initialisé sur la dernière feuille.
    const_reverse_leaf_iterator crbegin_leaf() const noexcept
        {return const_reverse_leaf_iterator(m_root).to_last_leaf();}

    /////// begin prevsuiv_iterator ////////

    //! Renvoie un itérateur de type prevSuiv initialisé sur la racine.
    prevsuiv_iterator begin_prevsuiv() noexcept
        {return m_root;}

    //! Renvoie un itérateur de type prevSuiv constant initialisé sur la racine.
    const_prevsuiv_iterator cbegin_prevsuiv() const noexcept
        {return m_root;}

    //! Revoie la liste des données des descendants directs du noeud pointé par it.
    std::list<T> childs(const_abstract_iterator &it) {
        std::list<T> liste;
        for (auto i = it.begin_child(); i; ++i)
            liste.push_back(*i);
        return liste;
    }

    //! Détruit l'ensemble des descendants de la racine. Laisse inchangé la donnée de la racine.
    void clear() noexcept
        {m_root->clear_childs();}

    //! Test si l'arbre est vide, c'est-à-dire, si la racine n'est pas nulle.
    bool empty() const noexcept
        {return m_root;}

    //! Insert un nouveau noeud de donnée crée en place à partir de args
    //! avant le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class... Args> iter_tree emplace(iter_tree pos, Args&&... args ) {
        auto *node = new item(std::forward<Args>(args)...);
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée crée en place à partir de args
    //! après le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class... Args> iter_tree emplace_after(iter_tree pos, Args&&... args ) {
        auto *node = new item(std::forward<Args>(args)...);
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert un fils benjamin de donnée crée en place à partir de args. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class... Args> iter_tree emplace_back(iter_tree pos, Args&&... args ) {
        auto *node = new item(std::forward<Args>(args)...);
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Insert un fils ainé de donnée crée en place à partir de args. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class... Args> iter_tree emplace_front(iter_tree pos, Args&&... args ) {
        auto *node = new item(std::forward<Args>(args)...);
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Crée un itérateur initialisé sur le noeud virtuel nul. Cette fonction permet la compatibilité avec les algorithmes standards.
    //! Utiliser de préférence la conversion de l'itérateur en booléen.
    const_abstract_iterator end() const noexcept
        {return const_abstract_iterator();}

    //! Supprime l'élément pointé par l'itérateur pos et ses descendants.
    //! Si pos pointe sur la racine, seul les descendants sont supprimés.
    //! Retourne un itérateur sur le suivant celui pointé par pos.
    template<class iter_tree> iter_tree erase(iter_tree pos) noexcept;

    //! Supprime les noeuds appartenant à [first,last) et leurs decendants.
    //! Le noeud pointé par first ne doit pas être la racine.
    //! Les noeuds pointés par first et last doivent être d'une même fratrie sauf si last est nul.
    //! Retourne un itérateur sur le noeud suivant le dernier noeud supprimé.
    template<class iter_tree> iter_tree erase(iter_tree first, iter_tree last) noexcept {
        first.m_ptr->erase(last.m_ptr);
        return last;
    }

    //! Extrait le noeud pointer par pos de l'arbre pour en faire un nouvel arbre
    template<class iter_tree> tree extract(iter_tree pos) {
        pos.m_ptr->become_root();
        return tree(pos.m_ptr);
    }

    //! Insert un nouveau noeud de donnée data avant le noeud pointé par pos dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert(iter_tree pos, const T &data = T()) {
        auto *node = new item(data);
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée data avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert(iter_tree pos, T &&data) {
        auto *node = new item(std::move(data));
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert une copie du noeud (et des descendant) pointé par other avant le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree,
             typename = std::enable_if_t<std::is_base_of<const_abstract_iterator,const_iter_tree>::value>>
                iter_tree insert(iter_tree pos, const_iter_tree  other) {
        auto *node = new item(other.item_ref());
        pos.m_ptr->insert(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée coorespondant à la racine de tree (avec ses descendants) avant le noeud pointé par pos
    //! dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils ainé. Retourne un itérateur sur ce nouveau noeud.
    //! La racine tree devient nulle.
    template<class iter_tree> iter_tree insert(iter_tree pos, tree<T> &&tr) {
        auto *node = tr.m_root;
        pos.m_ptr->insert(node);
        tr.m_root = nullptr;
        return node;
    }

    //! Insert count nouveaux noeuds de donnée data avant le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils ainé. Retourne un itérateur sur le premier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert(iter_tree pos, size_type count, const T &data)
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
    template<class iter_tree> iter_tree insert_after(iter_tree pos, const T &data = T()) {
        auto *node = new item(data);
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée data après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, T &&data) {
        auto *node = new item(std::move(data));
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert une copie du noeud (et des descendants) pointé par other après le noeud pointé par pos dans la fratrie
    //!  sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree,
                typename = std::enable_if_t<std::is_base_of<const_abstract_iterator,const_iter_tree>::value>>
                    iter_tree insert_after(iter_tree pos, const_iter_tree other) {
        auto *node = new item(other.item_ref());
        pos.m_ptr->insert_after(node);
        return node;
    }

    //! Insert un nouveau noeud de donnée coorespondant à la racine de tree (avec ses descendants) après le noeud pointé par pos
    //! dans la fratrie sauf si le pointé est la racine,
    //! dans ce cas le nouveau noeud est inseré en tant que fils benjamin. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, tree<T> &&tr) {
        auto *node = tr.m_root;
        pos.m_ptr->insert_after(node);
        tr.m_root = nullptr;
        return node;
    }

    //! Insert count nouveaux noeuds de donnée data après le noeud pointé par pos dans la fratrie sauf si le noeud pointé est la racine,
    //! dans ce cas les nouveaux noeuds sont inserés en tant que fils benjamin. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree insert_after(iter_tree pos, size_type count, const T &data)
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
    template<class iter_tree> iter_tree push_back(iter_tree pos, const T &data = T()) {
        auto *node = new item(data);
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos de donnée data. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_back(iter_tree pos, T &&data) {
        auto *node = new item(std::move(data));
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos correspondant à une copie du noeud (et des descendants) pointé par other.
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree,
             typename = std::enable_if_t<std::is_base_of<const_abstract_iterator,const_iter_tree>::value>>
                iter_tree push_back(iter_tree pos, const const_iter_tree other) {
        auto *node = new item(other.item_ref());
        pos.m_ptr->push_back(node);
        return node;
    }

    //! Ajoute un fils benjamin au noeud pointé par pos correspondant à la racine de tree (avec ses descendants).
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iterator push_back(iter_tree pos, tree<T> &&tr) {
        auto *node = tr.m_root;
        pos.m_ptr->push_back(node);
        tr.m_root = nullptr;
        return node;
    }

    //! Ajoute count fils benjamins au noeud pointé par pos de donnée data. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_back(iter_tree pos, size_type count, const T &data)
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
    template<class iter_tree> iter_tree push_front(iter_tree pos, const T &data = T()) {
        auto *node = new item(data);
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos de donnée data. Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_front(iter_tree pos, T &&data) {
        auto *node = new item(std::move(data));
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos correspondant à une copie du noeud (et des descendants) pointé par other.
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree, class const_iter_tree,
             typename = std::enable_if_t<std::is_base_of<const_abstract_iterator,const_iter_tree>::value>>
                        iter_tree push_front(iter_tree pos, const_iter_tree other) {
        auto *node = new item(other.item_ref());
        pos.m_ptr->push_front(node);
        return node;
    }

    //! Ajoute un fils ainé au noeud pointé par pos correspondant à la racine de tree (avec ses descendants).
    //! Retourne un itérateur sur ce nouveau noeud.
    template<class iter_tree> iter_tree push_front(iter_tree pos, tree<T> &&tr) {
        auto *node = tr.m_root;
        pos.m_ptr->push_front(node);
        tr.m_root = nullptr;
        return node;
    }

    //! Ajoute count fils ainés au noeud pointé par pos de donnée data. Retourne un itérateur sur le dernier des nouveaux noeuds.
    template<class iter_tree> iter_tree push_front(iter_tree pos, size_type count, const T &data)
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
    tree &operator = (const tree &tr) {
        delete m_root;
        m_root = new item(*tr.m_root);
        return *this;
    }

    //! Affectation par deplacement.
    tree &operator = (tree &&tr) {
        delete m_root;
        m_root = tr.m_root;
        tr.m_root = nullptr;
        return *this;
    }

    //! Renvoie une référence sur la donnée de chemin pos (sans vérification).
    T &operator[](std::list<int> list) {
        auto i = begin();
        return *i[list];
    }

    //! Renvoie une référence sur la donnée de chemin pos (sans vérification).
    const T &operator[](std::list<int> list) const {
        auto i = cbegin();
        return *i[list];
    }

    //! Supprime tous les noeuds (et leurs descendant) de donnée data.
    //! Si la racine est de donnée data, seulement les descendants sont supprimés.
    size_type remove(const T &data) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const const_abstract_iterator &)) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_prevsuiv_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type remove_if(U predicat) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const T &)) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type remove_if_data(U predicat) noexcept;

    //! Supprime toutes les feuilles (actuelle ou obtenue) de donnée data.
    size_type remove_leaf(const T &data) noexcept
        {return remove_leaf_if([&data](const const_abstract_iterator &it)->bool{return data == *it;});}

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const const_abstract_iterator &)) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type remove_leaf_if(U predicat) noexcept;

    //! Supprime les noeuds vérifiant le prédicat (bool predicat(const T &)) ainsi que leurs descendances.
    //! Le sens de parcours est celui d'un itérateur const_itérator.
    //! Si la racine vérifie le prédicat, seulement les descendants sont supprimés.
    template<class U> size_type remove_leaf_if_data(U predicat) noexcept
        {return remove_leaf_if([&predicat](const const_abstract_iterator &it)->bool{return predicat(*it);});}

    //! Remplace le noeud old par le noeud new ainsi que leur déscendants.
    //! Le noeud old et ses descendants, privée éventuellemment de new de ses descendants, sont détruits.
    //! Si old pointe sur la racine, la racine est remplacé par le noeud new.
    //! Attention new ne doit pas être la racine d'un arbre.
    template<class iter_tree> iter_tree replace(iter_tree old_it, iter_tree new_it) noexcept {
        if(old_it.root())
            m_root = new_it.m_ptr();
        old_it.m_ptr->replace(new_it->m_ptr);
        return new_it;
    }

    //! Teste si l'arbre est réduit à sa racine (valide).
    bool root_leaf() const noexcept
        {return m_root &&m_root->leaf();}

    //! Retourne le nombre de noeuds de l'arbre.
    size_type size() const noexcept
        {return m_root->size();}

    //! Echange le contenu de l'arbre avec other.
    void swap(tree &other) noexcept
        {std::swap(m_root,other.m_root);}

    //! Echange les positions des noeuds pointé par pos1 et pos2, ainsi que leurs descendant.
    template<class iter_tree> void swap(iter_tree it_1, iter_tree it_2) noexcept;

protected:
    //! Constructeur, prend le neud transmis pour racine (doit être créer dynamiquement et ne doit pas avoir de parent).
    tree(item *root) noexcept
        : m_root(root) {}

    //! Ajoute count nouveaux noeuds de donnée data au noeud pointé par pos selon la méthode ajout.
    //! Renvoie un pointeur sur le dernier noeud créé.
    template<class iter_tree, class Ajout> iter_tree add(iter_tree pos, size_type count, const T &data, Ajout ajout);

    //! Ajoute de nouveaux noeuds de donnée correspondant au la plage d'itérateur [first, last)
    //! au noeud pointé par pos selon la méthode ajout.
    //! Renvoie un pointeur sur le dernier noeud créé.
    template<class iter_tree, class InputIt, class Ajout> iter_tree add(iter_tree pos, InputIt first, InputIt last, Ajout ajout);
};

//////////////// tree //////////////////

template<class T> template<class iter_tree, class Ajout> iter_tree
                        tree<T>::add(iter_tree pos, size_type count, const T &data, Ajout ajout) {
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
        delete (iter++).item_ptr();
        return iter;
    }
}

template<class T> template<class iter_tree> tree<T> tree<T>::parentWithChilds(iter_tree pos) {
    tree<T> tree(*pos);
    auto root = tree.begin();
    if(!pos.leaf()) {
        for(auto it = pos.begin_child(); it; ++it)
            tree.push_back(root,*it);
    }
    return tree;
}

template<class T> typename tree<T>::size_type tree<T>::remove(const T &data) noexcept {
    auto it = begin();
    size_type comp = 0;
    while (it) {
        if(*it == data) {
            auto it_save = it;
            it.to_next_if_not_uncle();
            erase(it_save);
            ++comp;
        }
        else
            ++it;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::remove_if(U predicat) noexcept {
    auto it = begin_prevsuiv();
    size_type comp = 0;
    while(it) {
        if(predicat(it))
            erase(it++);
        else
            ++it;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::remove_if_data(U predicat) noexcept {
    auto it = begin();
    size_type comp = 0;
    while(it) {
        if(predicat(*it))
            erase(it++);
        else
            ++it;
    }
    return comp;
}

template<class T> template<class U> typename tree<T>::size_type tree<T>::remove_leaf_if(U predica) noexcept {
    auto it = begin_leaf();
    size_type comp = 0;
    while (it) {
        if(predica(it)) {
            auto it_save = it;
            if(it.first() &&it.last())
                    it.v_to_parent();
            else
                ++it;
            erase(it_save);
            ++comp;
        }
        else
            ++it;
    }
    return comp;
}

template<class T> template<class iter_tree> void tree<T>::swap(iter_tree it_1, iter_tree it_2) noexcept {
    if(it_1.root() || it_2.root()){
        std::swap(it_1.m_ptr->m_data,it_2.m_ptr->m_data);
        for(auto i = it_1.begin_child(); i; ++i)
            i.m_ptr->m_parent = it_2.m_ptr;
        for(auto i = it_2.begin_child(); i; ++i)
            i.m_ptr->m_parent = it_2.m_ptr;
        std::swap(it_1.m_ptr->m_first,it_2.m_ptr->m_first);
        std::swap(it_1.m_ptr->m_last,it_2.m_ptr->m_last);
    }
    else {
        std::swap(it_1.m_ptr->m_parent,it_2.m_ptr->m_parent);
        std::swap(it_1.m_ptr->m_prev,it_2.m_ptr->m_prev_Brother);
        std::swap(it_1.m_ptr->m_next,it_2.m_ptr->m_next);
        if(it_1.m_ptr->m_prev)
            it_1.m_ptr->m_prev->m_next = it_1.m_ptr;
        if(it_1.m_ptr->nextBrother)
            it_1.m_ptr->m_next->m_prev = it_1.m_ptr;
        if(it_2.m_ptr->m_prev)
            it_2.m_ptr->m_prev->m_next = it_2.m_ptr;
        if(it_2.m_ptr->nextBrother)
            it_2.m_ptr->m_next->m_prev = it_2.m_ptr;
    }
}

//////////////// item //////////////////

template<class T> tree<T>::item::item(const item &node)
    : m_data(node.m_data) {
    if(!node.leaf())
        for(auto i = node.cbegin_child(); i; ++i)
            push_back(new item(i.item_ref()));
}

template<class T> tree<T>::item::item(item &&node) noexcept(noexcept (T(std::move(node.m_data))))
    : m_first(node.m_first),
      m_last(node.m_last),
      m_data(std::move(node.m_data)) {
    if(!node.leaf())
        for(auto it = node.begin_child(); it; ++it)
            it.set_parent(this);
    node.become_root();
    node.m_first = node.m_last = nullptr;
}

template<class T> template<class U> tree<T>::item::item(const typename tree<U>::item &node)
    : m_data(node.data()) {
    if(!leaf())
        for(auto it = node.cbegin_child(); it; ++it)
            push_back(new item(it.item_ref()));
}

template<class T> template<class U,class F> tree<T>::item::item(const typename tree<U>::item &node, F  usine)
    : m_data(usine(node.data())) {
    if(!leaf())
        for(auto it = node.cbegin_child(); it; ++it)
            push_back(new item(it.item_ref(),usine));
}

template<class T> tree<T>::item::~item() {
    change_heredite(nullptr);
    if(!leaf()){
        auto it = begin_child();
        while(it) {
            it.set_parent(nullptr);
            delete (it++).item_ptr();
        }
    }
}

template<class T> void tree<T>::item::change_heredite(item *parent) noexcept {
    if(m_parent) {
        if(m_prev)
            m_prev->m_next = m_next;
        else
            m_parent->m_first = m_next;

        if(m_next)
            m_next->m_prev = m_prev;
        else
            m_parent->m_last = m_prev;
    }
    m_parent = parent;
}

template<class T> void tree<T>::item::clear_childs() noexcept {
    if(!leaf()) {
        auto it = begin_child();
        while(it) {
            it.set_parent(nullptr);
            delete (it++).item_ptr();
        }
        m_first = m_last = nullptr;
    }
}

template<class T> void tree<T>::item::erase(item *last) noexcept {
    if(!last &&first())
        m_parent->clear_childs();
    else if(!last || m_parent == last->m_parent){
        if(!last) {
            m_prev->m_next = nullptr;
            m_parent->m_last = m_prev;
        }
        else {
            last->m_prev->m_next = nullptr;
            last->m_prev = m_prev;
            if(first())
                m_parent->m_first = last;
            else
                m_prev->m_next = last;
        }
        auto *ptr = this;
        while (ptr) {
            auto *del_vtr = ptr;
            del_vtr->m_parent = nullptr;
            ptr = ptr->m_next;
            delete del_vtr;
        }
    }
}

template<class T> void tree<T>::item::insert(item *node) noexcept {
    if(m_parent){
        node->change_heredite(m_parent);
        node->m_prev = m_prev;
        node->m_next = this;
        if(m_prev)
            m_prev->m_next = node;
        else
            m_parent->m_first = node;
        m_prev = node;
    }
    else
        push_front(node);
}

template<class T> void tree<T>::item::insert_after(item *node) noexcept {
    if(m_parent) {
        node->change_heredite(m_parent);
        node->m_prev = this;
        node->m_next = m_next;
        if(m_next)
            m_next->m_prev = node;
        else
            m_parent->m_last = node;
        m_next = node;
    }
    else
        push_back(node);
}

template<class T> typename tree<T>::item &tree<T>::item::operator = (const item &node) {
    m_data = node.m_data;
    clear_childs();
    if(!node.leaf())
        for(auto it = node.cbegin_child(); it; ++it)
            push_back(new item(it.item_ref()));
    return *this;
}

template<class T> typename tree<T>::item &tree<T>::item::operator = (item &&node) noexcept(noexcept (T(std::move(node.m_data)))) {
    m_data = std::move(node.m_data);
    if(!leaf())
        for(auto it = begin_child(); it; ++it) {
            it.set_parent(nullptr);
            delete it.m_ptr;
        }
    if(!node.leaf()) {
        for(auto it = node.begin_child(); it; ++it)
            it.set_parent(this);
        m_first = node.m_first;
        m_last = node.m_last;
    }
    else
        m_first = m_last = nullptr;
    node.become_root();
    node.m_first = node.m_last = nullptr;
}

template<class T> void tree<T>::item::push_back(item *child) noexcept {
    child->change_heredite(this);
    child->m_prev = m_last;
    child->m_next = nullptr;
    if(m_last)
        m_last = m_last->m_next = child;
    else
        m_first = m_last = child;
}

template<class T> void tree<T>::item::push_front(item *child) noexcept {
    child->change_heredite(this);
    child->m_prev = nullptr;
    child->m_next = m_first;
    if(m_first)
        m_first = m_first->m_prev = child;
    else
        m_first = m_last = child;
}

template<class T> void tree<T>::item::replace(item *node) noexcept{
    node->change_heredite(m_parent);
    if(m_parent) {
        if(m_prev)
            m_prev->m_next = node;
        else
            m_parent->m_first = node;
        if(m_next)
            m_next->m_prev = node;
        else
            m_parent->m_last = node;
    }
    node->m_prev = m_prev;
    node->m_next= m_next;
    m_parent = nullptr;
    delete this;
}

template<class T> typename tree<T>::size_type tree<T>::item::size() const noexcept {
    size_type compteur = 1;
    if(!leaf()) {
        auto it = cbegin();
        auto fin = cbegin();
        fin.to_last_leaf();
        while(it != fin) {
            ++compteur;
            ++it;
        }
    }
    return compteur;
}

template<class T> typename tree<T>::size_type tree<T>::item::size_child() const noexcept {
    if(leaf())
        return 0;
    size_type n = 1;
    auto *node = m_first->m_next;
    while(node) {
        ++n;
        node = node->m_next;
    }
    return n;
}

//////////////// const_abstract_iterator //

template<class T> std::list<int> tree<T>::const_abstract_iterator::index() const {
    std::list<int> list;
    const_abstract_iterator node(*this);
    while(!node.root()) {
        list.push_front(node.position());
        node.v_to_parent();
    }
    return list;
}

template<class T> int tree<T>::const_abstract_iterator::index_brother() const noexcept {
    if(first())
        return 0;
    const_abstract_iterator node(*this);
    auto n = 0;
    while(node) {
        node.v_to_prev();
        ++n;
    }
    return n;
}

template<class T> template<class iter> iter &tree<T>::const_abstract_iterator::op_add_assign(int n) noexcept {
    if(n > 0)
        while(n &&iter::operator++())
            --n;
    else if(n < 0)
        while(n &&iter::operator--())
            ++n;
    return *this;
}

template<class T> bool tree<T>::const_abstract_iterator::operator <(const const_abstract_iterator &it) const noexcept {
    std::list<const_abstract_iterator> list_1;
    auto iter = *this;
    while(iter) {
        list_1.push_front(iter);
        iter.v_to_parent();
    }
    iter = it;
    std::list<const_abstract_iterator> list_2;
    while(iter) {
        list_2.push_front(iter);
        iter.v_to_parent();
    }
    auto it_1 = list_1.cbegin();
    auto it_2 = list_2.cbegin();
    while(it_1 != list_1.cend() &&it_2 != list_2.cend() &&*it_1 == *it_2) {
        ++it_1;
        ++it_2;
    }
    if(it_2 == list_2.cend())
        return false;
    if(it_1 == list_1.cend()) // place de if avant le précédent pour obtenir une inégalité large.
        return true;
    iter = *it_1;
    while(iter &&iter != *it_2)
        iter.v_to_prev();
    return !iter;
}

template<class T> void tree<T>::const_abstract_iterator::to_brother(int n) noexcept {
    if(m_ptr) {
        if(n > 0)
            while(n &&m_ptr) {
                v_to_next();
                --n;
            }
        else if(n < 0)
            while(n &&m_ptr) {
                v_to_prev();
                ++n;
            }
    }
}

template<class T> void tree<T>::const_abstract_iterator::to_child(int n) noexcept {
    if(n >= 0) {
        v_to_first();
        while(n &&m_ptr) {
            v_to_next();
            --n;
        }
    }
    else {
        v_to_last();
        ++n;
        while(n &&m_ptr) {
            v_to_prev();
            ++n;
        }
    }
}

template<class T> void tree<T>::const_abstract_iterator::to_parent(int n) noexcept {
    if(n &&m_ptr) {
        if(n>0)
            while(n &&m_ptr) {
                v_to_parent();
                --n;
            }
        else if(n<0)
            while(n &&m_ptr) {
                v_to_first();
                ++n;
            }
    }
}

template<class T> void tree<T>::const_abstract_iterator::to_path(const std::list<T> &list) noexcept {
    if(m_ptr) {
        for(auto it = list.cbegin(); m_ptr &&it != list.cend(); ++it) {
            auto it_find = m_ptr->begin_child();
            while(it_find &&*it_find != *it)
                ++it_find;
            m_ptr = it_find.m_ptr;
        }
    }
}

//////////////// const_iterator ////////////////

template<class T> typename tree<T>::const_iterator &tree<T>::const_iterator::operator ++() noexcept {
    if(m_ptr) {
        if(m_ptr->leaf())
            to_next_if_not_uncle();
        else
            v_to_first();
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator &tree<T>::const_iterator::operator --() noexcept {
    if(m_ptr) {
        if(m_ptr->first())
            v_to_parent();
        else {
            v_to_prev();
            v_to_last_leaf();
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator &tree<T>::const_iterator::operator += (int n) noexcept {
    if(m_ptr &&n) {
        if (n < 0)
            return operator -=(-n);
        else
            while(n &&m_ptr) {
                if(m_ptr->leaf())
                    to_next_if_not_uncle();
                else
                    v_to_first();
                --n;
            }
    }
    return *this;
}

template<class T> typename tree<T>::const_iterator &tree<T>::const_iterator::operator -=(int n) noexcept {
    if(m_ptr &&n) {
        if (n < 0)
            return operator +=(-n);
        else
            while(n &&m_ptr) {
                if(m_ptr->first())
                    v_to_parent();
                else {
                    v_to_prev();
                    v_to_last_leaf();
                }
            }
    }
    return *this;
}

/////////////const_reverse_iterator //////

template<class T> typename tree<T>::const_reverse_iterator &tree<T>::const_reverse_iterator::operator ++() noexcept {
    if(m_ptr) {
        if(m_ptr->leaf())
            v_to_prev_if_not_uncle();
        else
            v_to_last();
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator &tree<T>::const_reverse_iterator::operator --() noexcept {
    if(m_ptr) {
        if(m_ptr->last())
            v_to_parent();
        else {
            v_to_next();
            v_to_first_leaf();
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator &tree<T>::const_reverse_iterator::operator += (int n) noexcept {
    if(m_ptr &&n) {
        if (n < 0)
            return operator -=(-n);
        else
            while(n &&m_ptr) {
                if(m_ptr->leaf())
                    v_to_prev_if_not_uncle();
                else
                    v_to_last();
                --n;
            }
    }
    return *this;
}

template<class T> typename tree<T>::const_reverse_iterator &tree<T>::const_reverse_iterator::operator -=(int n) noexcept {
    if(m_ptr &&n) {
        if (n < 0)
            return operator +=(-n);
        else
            while(n &&m_ptr) {
                if(m_ptr->last())
                    v_to_parent();
                else {
                    v_to_prev();
                    v_to_first_leaf();
                }
                --n;
            }
    }
    return *this;
}

///////////// const_prevsuiv_iterator ///////

template<class T> typename tree<T>::const_prevsuiv_iterator &tree<T>::const_prevsuiv_iterator::operator++() noexcept {
    if(m_ptr) {
        if(m_sens) {
            if(m_ptr->leaf())
                m_sens = false;
            else
                v_to_first();
        }
        else {
            if(m_ptr->last())
                v_to_parent();
            else {
                v_to_next();
                m_sens = true;
            }
        }
    }
    return *this;
}

template<class T> typename tree<T>::const_prevsuiv_iterator &tree<T>::const_prevsuiv_iterator::operator--() noexcept {
    if(m_ptr) {
        if(m_sens) {
            if(m_ptr->first())
                v_to_parent();
            else {
                v_to_prev();
                m_sens = false;
            }
        }
        else {
            if(m_ptr->leaf())
                m_sens = true;
            else
                v_to_last();
        }
    }
    return *this;
}
}
#endif // TREE_H
