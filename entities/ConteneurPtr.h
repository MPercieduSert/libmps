/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/11/2019
 */
#ifndef CONTENEURPTR_H
#define CONTENEURPTR_H

#include <forward_list>
#include <map>
#include <vector>
#include "infoEntity.h"

/*! \defgroup groupeConteneurs Conteneur
 * \brief Ensemble de classes conteneurs.
 */

/*! \ingroup groupeConteneur
 * \brief Espace de noms des conteneurs.
 */
namespace conteneurMPS {
using namespace typeMPS;
template<class Ent> using mapIdt = std::map<idt,Ent>;
template<class T> class VectorPtr;


/*! \ingroup groupeConteneur
 * \brief Classe patron des listes de pointeurs.
 */
template<class T> class ListPtr : protected std::forward_list<T*> {
    friend VectorPtr<T>;
protected:
    using Lptr = std::forward_list<T*>;
    using std::forward_list<T*>::cbegin;
    using std::forward_list<T*>::cend;
    //using std::forward_list<T*>::clear;

public:
    using typename std::forward_list<T*>::size_type;
    using std::forward_list<T*>::push_front;
    using std::forward_list<T*>::empty;

    ListPtr() = default;

    //! Constructeur de déplacement.
    ListPtr(ListPtr<T> && ) = default;

    //! Destructeur.
    ~ListPtr() {
        for(auto i = cbegin(); i != cend(); ++i)
            delete *i;
    }

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear() noexcept {
        for(auto i = cbegin(); i != cend(); ++i)
            delete *i;
        Lptr::clear();
    }

    //! Affectation par déplacement
    ListPtr<T> & operator = (ListPtr<T> && liste) = default;

    //! Ajoute un pointeur au début de la liste de donnée entity.
    void push_front(const T & entity)
        {push_front(new T(entity));}


    //! Ajoute un pointeur au début de la liste de donnée entity.
    void push_front(T && entity)
        {push_front(new T(std::move(entity)));}

    //! Renvoie le nombre d'élément de la liste.
    size_type size() const noexcept {
        size_type comp = 0;
        for (auto i = cbegin(); i != cend(); ++i)
            ++comp;
        return comp;
    }

    //! Renvoie le vecteur des éléments de la liste transformé par usine.
    template<class Usine> decltype(auto) vectorOf(Usine f) const {
        std::vector<decltype (f(**cbegin()))> vec(size());
        auto j = vec.rbegin();
        for (auto i = cbegin(); i != cend(); ++i) {
            *j = f(**i);
        }
        return vec;
    }

protected:
    //! Efface la liste de pointeurs, ceux-ci doivent être préalablement déplacés ou les objet pointés détruit.
    void clearPtr() noexcept
        {Lptr::clear();}

};

/*! \ingroup groupeConteneur
 * \brief Classe patron des vector de pointeurs.
 */
template<class T> class VectorPtr : protected std::vector<T*> {
protected:
    using Vptr = std::vector<T*>;

public:
    /*! \ingroup groupeConteneur
     * \brief Itérateur constant.
     */
    class const_iterator : public Vptr::const_iterator {
    protected:
        using VptrIter = typename Vptr::const_iterator;

    public:
        //! Constructeur.
        const_iterator(const VptrIter & iter) noexcept : VptrIter(iter) {}

        //! Opérator *.
        const T & operator *() const noexcept
            {return *VptrIter::operator*();}

        //! Opérator ->.
        T const * operator -> () const noexcept
            {return VptrIter::operator*();}
    };

    /*! \ingroup groupeConteneur
     * \brief Itérateur.
     */
    class iterator : public Vptr::const_iterator {
    protected:
        using VptrIter = typename Vptr::const_iterator;

    public:
        //! Constructeur.
        iterator(const VptrIter & iter) noexcept : VptrIter(iter) {}

        //! Opérator *.
        const T & operator *() const noexcept
            {return *VptrIter::operator*();}

        //! Opérator *.
        T & operator *() noexcept
            {return *VptrIter::operator*();}

        //! Opérator ->.
        T const * operator -> () const noexcept
            {return VptrIter::operator*();}

        //! Opérator ->.
        T * operator -> () noexcept
            {return VptrIter::operator*();}
    };

    using typename Vptr::size_type;
    using Vptr::empty;
    using Vptr::size;
    using Vptr::shrink_to_fit;

    //! Constructeur par defaut.
    VectorPtr() = default;

    //! Constructeur de recopie.
    VectorPtr(const VectorPtr<T> & vect)
        : Vptr(vect.size()) {
        auto j = Vptr::begin();
        for(auto i = vect.cbegin(); i != vect.cend(); ++i, ++j)
            *j = new T(*i);
    }

    //! Constructeur de déplacement
    VectorPtr(VectorPtr<T> && vect) = default;

    //! Constructeur à partir d'une liste de pointeurs.
    VectorPtr(const ListPtr<T> & liste);

    //! Constructeur à partir d'une liste de pointeurs temporaire.
    VectorPtr(ListPtr<T> && liste) noexcept;

    //! Constructeur
    VectorPtr(size_type n, const T & value = T())
        : Vptr(n) {
        for(auto i = Vptr::begin(); i != Vptr::end(); ++i)
            *i = new T(value);
    }

    //! Destructeur.
    ~VectorPtr() {
        for(auto i = Vptr::cbegin(); i != Vptr::cend(); ++i)
            delete *i;
    }

    //! Renvoie une référence sur le n-ième élément (avec vérification).
    T & at(size_type n)
        {return *(Vptr::at(n));}

    //! Renvoie une référence sur le n-ième élément (avec vérification)..
    const T & at(size_type n) const
        {return *(Vptr::at(n));}

    //! Renvoie un itérateur sur le début du vecteur.
    iterator begin() const noexcept
        {return Vptr::cbegin();}

    //! Renvoie un itérateur sur le début du vecteur.
    const_iterator cbegin() const noexcept
        {return Vptr::cbegin();}

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear() noexcept {
        for(auto i = cbegin(); i != cend(); ++i)
            delete *i;
        clear();
    }

    //! Renvoie un itérateur sur la fin du vecteur.
    iterator end() const noexcept
        {return Vptr::cend();}

    //! Renvoie un itérateur sur la fin du vecteur.
    const_iterator cend() const noexcept
        {return Vptr::cend();}

    //! Renvoie un iterateur sur l'entité d'identifiant id si elle existe.
    const_iterator findId(unsigned id) noexcept;

    //! Affectation par recopie.
    VectorPtr<T> &operator =(const VectorPtr<T> & vector);

    //! Affectation par déplacement.
    VectorPtr<T> &operator =(VectorPtr<T> && vector) noexcept;

    //! Affectation par copie d'une liste de type ListeEntities.
    VectorPtr<T> &operator =(const ListPtr<T> & liste);

    //! Affectation par déplacement du liste (temporaire) de type ListeEntities.
    VectorPtr<T> &operator =(ListPtr<T> && liste) noexcept;

    //! Ajoute les éléments de liste à la suite du vecteur.
    VectorPtr<T> &operator <<(const ListPtr<T> & liste);

    //! Ajoute les éléments de liste (temporaire) à la suite du vecteur.
    VectorPtr<T> &operator <<(ListPtr<T> && liste);

    //! Renvoie une référence sur le n-ième élément (sans vérification)..
    T & operator[](size_type n)
        {return *(Vptr::operator[](n));}

    //! Renvoie une référence sur le n-ième élément (sans vérification)..
    const T & operator[](size_type n) const
        {return *(Vptr::operator[](n));}

    //! Ajoute un élément au vecteur.
    void push_back(const T & value)
        {Vptr::push_back(new T(value));}

    //! Ajoute un élément au vecteur.
    void push_back(T && value)
        {Vptr::push_back(new T(std::move(value)));}
};

//////////////////// VectPtr //////////////////

template<class T> VectorPtr<T>::VectorPtr(const ListPtr<T> & liste)
    : Vptr(liste.size()) {
    if(!liste.empty()) {
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
}

template<class T> VectorPtr<T>::VectorPtr(ListPtr<T> && liste) noexcept
    : Vptr(liste.size()) {
    if(!liste.empty()) {
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clearPtr();
    }
}

template<class T> typename VectorPtr<T>::const_iterator VectorPtr<T>::findId(unsigned id) noexcept
{
    const_iterator i = begin();
    while(i != end() && i->id() != id)
        ++i;
    return i;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const ListPtr<T> & liste) {
    if(!liste.empty()) {
        resize(liste.size());
        //shrink_to_fit();
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(ListPtr<T> && liste) noexcept{
    if(!liste.empty()) {
        resize(liste.size());
        //shrink_to_fit();
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clearPtr();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const VectorPtr<T> & vector) {
    if(!vector.empty()) {
        resize(vector.size());
        //shrink_to_fit();
        auto i = Vptr::begin();
        for(auto j = vector.cbegin(); j != vector.cend(); ++j, ++i)
            *i = new T(*j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(VectorPtr<T> && vector) noexcept{
    if(!vector.empty()) {
        resize(vector.size());
        //shrink_to_fit();
        auto i = Vptr::begin();
        for(auto j = vector.Vptr::cbegin(); j != vector.Vptr::cend(); ++j, ++i)
            *i = *j;
        vector.clear();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator << (const ListPtr<T> & liste) {
    if(!liste.empty()) {
        int n = size();
        resize(n + liste.size());
        auto i = Vptr::rbegin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> &VectorPtr<T>::operator <<(ListPtr<T> && liste) {
    if(!liste.empty()) {
        int n = size();
        resize(n + liste.size());
        auto i = Vptr::rbegin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clearPtr();
    }
    return *this;
}
}
#endif // CONTENEURPTR_H
