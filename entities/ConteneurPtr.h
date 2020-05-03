/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/11/2019
 */
#ifndef CONTENEURPTR_H
#define CONTENEURPTR_H

#include <forward_list>
#include <map>
#include <memory>
#include <vector>
#include "typemps.h"

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
        clearPtr();
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
        for (auto i = cbegin(); i != cend(); ++i)
            *j = f(**i);
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
    template<class U> friend class VectorPtr;
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
    using Vptr::reserve;
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

    //! Constructeur à partir d'une liste de pointeurs temporaire.
    template<class U> VectorPtr(VectorPtr<U> && vect) noexcept;

    //! Constructeur
    VectorPtr(size_type n, const T & value = T())
        : Vptr(n) {
        for(auto i = Vptr::begin(); i != Vptr::end(); ++i)
            *i = new T(value);
    }

    //! Constructeur
    VectorPtr(size_type n, T * ptr)
        : Vptr(n,ptr) {}

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

    //! Renvoie une référence sur la valeur du dernier élément.
    T & back()
        {return *(Vptr::back());}

    //! Renvoie une référence sur la valeur du dernier élément.
    const T & back() const
        {return *(Vptr::back());}

    //! Renvoie un itérateur sur le début du vecteur.
    iterator begin() const noexcept
        {return Vptr::cbegin();}

    //! Renvoie un itérateur constant sur le début du vecteur.
    const_iterator cbegin() const noexcept
        {return Vptr::cbegin();}

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear() noexcept {
        for(auto i = Vptr::cbegin(); i != Vptr::cend(); ++i)
            delete *i;
        Vptr::clear();
    }

    //! Renvoie un itérateur sur la fin du vecteur.
    iterator end() const noexcept
        {return Vptr::cend();}

    //! Renvoie un itérateur sur la fin du vecteur.
    const_iterator cend() const noexcept
        {return Vptr::cend();}

    //! Supprime l'élément pointé par iter.
    iterator erase(const_iterator pos) {
        delete pos.Vptr::const_iterator::operator*();
        return static_cast<iterator>(Vptr::erase(pos));
    }

    //! Supprime l'élément pointé par iter.
    iterator erase(const_iterator first, const_iterator last) {
        for(auto pos = first; pos != last; ++pos)
            delete pos.Vptr::const_iterator::operator*();
        return static_cast<iterator>(Vptr::erase(first,last));
    }

    //! Renvoie un iterateur sur l'entité d'identifiant id si elle existe.
    const_iterator findId(unsigned id) noexcept;

    //! Renvoie une référence sur la valeur du premier élément.
    T & front()
        {return *(Vptr::front());}

    //! Renvoie une référence sur la valeur du premier élément.
    const T & front() const
        {return *(Vptr::front());}

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

    //! Renvoie une référence sur le n-ième élément (sans vérification).
    T & operator[](size_type n)
        {return *(Vptr::operator[](n));}

    //! Renvoie une référence sur le n-ième élément (sans vérification).
    const T & operator[](size_type n) const
        {return *(Vptr::operator[](n));}

    //! Ajoute un élément au vecteur.
    void push_back(const T & value)
        {Vptr::push_back(new T(value));}

    //! Ajoute un élément au vecteur.
    void push_back(T && value)
        {Vptr::push_back(new T(std::move(value)));}

    //! Remplace l'élément pointé par i qui est détruit, par l'élément pointé par ptr dont le vecteur prend posséssion.
    void setPtr(size_type pos,std::unique_ptr<T> && ptr) {
        if(Vptr::operator[](pos))
            delete Vptr::operator[](pos);
        Vptr::operator[](pos) = ptr.release();
    }

    //! Redimension le vecteur à la taille count.
    template<class Factory> void resize(size_type count, Factory factory){
        auto i = size();
        if(count < i)
            Vptr::resize(count);
        else {
            resizeNull(count);
            while(i < count) {
                Vptr::operator[](i) = factory();
                ++i;
            }
        }
    }

protected:
    //! Redimension en ajoutant des pointeur nul.
    void resizeNull(size_type count)
        {Vptr::resize(count,nullptr);}
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

template<class T> template<class U> VectorPtr<T>::VectorPtr(VectorPtr<U> && vect) noexcept
    : Vptr(vect.size()) {
    if(!vect.empty()) {
        auto i = Vptr::begin();
        for(auto j = vect.std::template vector<U*>::cbegin(); j != vect.std::template vector<U*>::cend(); ++i, ++j)
            *i = *j;
        vect.std::template vector<U*>::clear();
    }
}

template<class T> typename VectorPtr<T>::const_iterator VectorPtr<T>::findId(unsigned id) noexcept{
    const_iterator i = begin();
    while(i != end() && i->id() != id)
        ++i;
    return i;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const ListPtr<T> & liste) {
    if(!liste.empty()) {
        resizeNull(liste.size());
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(ListPtr<T> && liste) noexcept{
    if(!liste.empty()) {
        resizeNull(liste.size());
        auto i = Vptr::rbegin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clearPtr();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const VectorPtr<T> & vector) {
    if(!vector.empty()) {
        resizeNull(vector.size());
        auto i = Vptr::begin();
        for(auto j = vector.cbegin(); j != vector.cend(); ++j, ++i)
            *i = new T(*j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(VectorPtr<T> && vector) noexcept{
    if(!vector.empty()) {
        resizeNull(vector.size());
        auto i = Vptr::begin();
        for(auto j = vector.Vptr::cbegin(); j != vector.Vptr::cend(); ++j, ++i)
            *i = *j;
        vector.Vptr::clear();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator << (const ListPtr<T> & liste) {
    if(!liste.empty()) {
        int n = size();
        resizeNull(n + liste.size());
        auto i = Vptr::rbegin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> &VectorPtr<T>::operator <<(ListPtr<T> && liste) {
    if(!liste.empty()) {
        int n = size();
        resizeNull(n + liste.size());
        auto i = Vptr::rbegin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clearPtr();
    }
    return *this;
}
}
#endif // CONTENEURPTR_H
