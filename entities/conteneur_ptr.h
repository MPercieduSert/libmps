/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/11/2019
 */
#ifndef CONTENEUR_PTR_H
#define CONTENEUR_PTR_H

#include <forward_list>
#include <map>
#include <memory>
#include <vector>
#include "type_mps.h"

/*! \defgroup groupeConteneur Conteneur
 *\brief Ensemble de classes conteneurs.
 */

namespace mps {
template<class Ent> using map_id = std::map<idt,Ent>;
template<class T> class vector_ptr;

/*! \ingroup groupeConteneur
 *\brief Classe patron des listes de pointeurs.
 */
template<class T> class list_ptr : protected std::forward_list<T*> {
    template<class U> friend class vector_ptr;
protected:
    using l_ptr = std::forward_list<T*>;
    using std::forward_list<T*>::cbegin;
    using std::forward_list<T*>::cend;
    //using std::forward_list<T*>::clear;

public:
    using typename std::forward_list<T*>::size_type;
    using std::forward_list<T*>::push_front;
    using std::forward_list<T*>::empty;

    list_ptr() = default;

    //! Constructeur de déplacement.
    list_ptr(list_ptr<T> &&) = default;

    //! Destructeur.
    ~list_ptr() {
        for(auto it = cbegin(); it != cend(); ++it)
            delete *it;
    }

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear() noexcept {
        for(auto it = cbegin(); it != cend(); ++it)
            delete *it;
        clear_ptr();
    }

    //! Affectation par déplacement
    list_ptr<T> &operator = (list_ptr<T> &&liste) = default;

    //! Ajoute un pointeur au début de la liste de donnée ent.
    void push_front(const T &ent)
        {push_front(new T(ent));}

    //! Ajoute un pointeur au début de la liste de donnée ent.
    void push_front(T &&ent)
        {push_front(new T(std::move(ent)));}

    //! Renvoie le nombre d'élément de la liste.
    size_type size() const noexcept {
        size_type comp = 0;
        for (auto it = cbegin(); it != cend(); ++it)
            ++comp;
        return comp;
    }

    //! Renvoie le vecteur des éléments de la liste transformé par usine.
    template<class Usine> decltype(auto) vector_of(Usine f) const {
        std::vector<decltype (f(**cbegin()))> vec(size());
        auto it_vec = vec.rbegin();
        for (auto it = cbegin(); it != cend(); ++it)
            *it_vec = f(**it);
        return vec;
    }

protected:
    //! Efface la liste de pointeurs, ceux-ci doivent être préalablement déplacés ou les objet pointés détruit.
    void clear_ptr() noexcept
        {l_ptr::clear();}
};

/*! \ingroup groupeConteneur
 *\brief Classe patron des vector de pointeurs.
 */
template<class T> class vector_ptr : protected std::vector<T*> {
    template<class U> friend class vector_ptr;
protected:
    using v_ptr = std::vector<T*>;

public:
    /*! \ingroup groupeConteneur
     *\brief Itérateur constant.
     */
    class const_iterator : public v_ptr::const_iterator {
    protected:
        using iter_v_ptr = typename v_ptr::const_iterator;

    public:
        //! Constructeur.
        const_iterator(const iter_v_ptr &iter) noexcept : iter_v_ptr(iter) {}

        //! Opérator *.
        const T &operator *() const noexcept
            {return *iter_v_ptr::operator*();}

        //! Opérator ->.
        T const *operator -> () const noexcept
            {return iter_v_ptr::operator*();}
    };

    /*! \ingroup groupeConteneur
     *\brief Itérateur.
     */
    class iterator : public v_ptr::const_iterator {
    protected:
        using iter_v_ptr = typename v_ptr::const_iterator;

    public:
        //! Constructeur.
        iterator(const iter_v_ptr &iter) noexcept : iter_v_ptr(iter) {}

        //! Opérator *.
        const T &operator *() const noexcept
            {return *iter_v_ptr::operator*();}

        //! Opérator *.
        T &operator *() noexcept
            {return *iter_v_ptr::operator*();}

        //! Opérator ->.
        T const *operator -> () const noexcept
            {return iter_v_ptr::operator*();}

        //! Opérator ->.
        T *operator -> () noexcept
            {return iter_v_ptr::operator*();}
    };

    using typename v_ptr::size_type;
    using v_ptr::empty;
    using v_ptr::reserve;
    using v_ptr::size;
    using v_ptr::shrink_to_fit;

    //! Constructeur par defaut.
    vector_ptr() = default;

    //! Constructeur de recopie.
    vector_ptr(const vector_ptr<T> &vect)
        : v_ptr(vect.size()) {
        auto it_vec = v_ptr::begin();
        for(auto it = vect.cbegin(); it != vect.cend(); ++it, ++it_vec)
            *it_vec = new T(*it);
    }

    //! Constructeur de déplacement
    vector_ptr(vector_ptr<T> &&vect) = default;

    //! Constructeur à partir d'une liste de pointeurs.
    vector_ptr(const list_ptr<T> &liste);

    //! Constructeur à partir d'une liste de pointeurs temporaire.
    vector_ptr(list_ptr<T> &&liste) noexcept;

    //! Constructeur à partir d'une liste de pointeurs temporaire.
    template<class U> vector_ptr(vector_ptr<U> &&vect) noexcept;

    //! Constructeur à partir d'une liste de pointeurs temporaire.
    template<class U> vector_ptr(list_ptr<U> &&vect) noexcept;

    //! Constructeur
    vector_ptr(size_type n, const T &value = T())
        : v_ptr(n) {
        for(auto it = v_ptr::begin(); it != v_ptr::end(); ++it)
            *it = new T(value);
    }

    //! Constructeur
    vector_ptr(size_type n, T *ptr)
        : v_ptr(n,ptr) {}

    //! Destructeur.
    ~vector_ptr() {
        for(auto it = v_ptr::cbegin(); it != v_ptr::cend(); ++it)
            delete *it;
    }

    //! Renvoie une référence sur le n-ième élément (avec vérification).
    T &at(size_type n)
        {return *(v_ptr::at(n));}

    //! Renvoie une référence sur le n-ième élément (avec vérification)..
    const T &at(size_type n) const
        {return *(v_ptr::at(n));}

    //! Renvoie une référence sur la valeur du dernier élément.
    T &back()
        {return *(v_ptr::back());}

    //! Renvoie une référence sur la valeur du dernier élément.
    const T &back() const
        {return *(v_ptr::back());}

    //! Renvoie un itérateur sur le début du vecteur.
    iterator begin() const noexcept
        {return v_ptr::cbegin();}

    //! Renvoie un itérateur constant sur le début du vecteur.
    const_iterator cbegin() const noexcept
        {return v_ptr::cbegin();}

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear() noexcept {
        for(auto it = v_ptr::cbegin(); it != v_ptr::cend(); ++it)
            delete *it;
        v_ptr::clear();
    }

    //! Renvoie un itérateur sur la fin du vecteur.
    iterator end() const noexcept
        {return v_ptr::cend();}

    //! Renvoie un itérateur sur la fin du vecteur.
    const_iterator cend() const noexcept
        {return v_ptr::cend();}

    //! Supprime l'élément pointé par iter.
    iterator erase(const_iterator pos) {
        delete pos.v_ptr::const_iterator::operator*();
        return static_cast<iterator>(v_ptr::erase(pos));
    }

    //! Supprime l'élément pointé par iter.
    iterator erase(const_iterator first, const_iterator last) {
        for(auto pos = first; pos != last; ++pos)
            delete pos.v_ptr::const_iterator::operator*();
        return static_cast<iterator>(v_ptr::erase(first,last));
    }

    //! Renvoie un iterateur sur l'entité d'identifiant id si elle existe.
    const_iterator find_id(idt id) noexcept;

    //! Renvoie une référence sur la valeur du premier élément.
    T &front()
        {return *(v_ptr::front());}

    //! Renvoie une référence sur la valeur du premier élément.
    const T &front() const
        {return *(v_ptr::front());}

    //! Affectation par recopie.
    vector_ptr<T> &operator =(const vector_ptr<T> &vector);

    //! Affectation par déplacement.
    vector_ptr<T> &operator =(vector_ptr<T> &&vector) noexcept;

    //! Affectation par copie d'une liste de type ListeEntities.
    vector_ptr<T> &operator =(const list_ptr<T> &liste);

    //! Affectation par déplacement du liste (temporaire) de type ListeEntities.
    vector_ptr<T> &operator =(list_ptr<T> &&liste) noexcept;

    //! Ajoute les éléments de liste à la suite du vecteur.
    vector_ptr<T> &operator <<(const list_ptr<T> &liste);

    //! Ajoute les éléments de liste (temporaire) à la suite du vecteur.
    vector_ptr<T> &operator <<(list_ptr<T> &&liste);

    //! Ajoute les éléments de liste à la suite du vecteur.
    vector_ptr<T> &operator <<(const vector_ptr<T> &liste);

    //! Ajoute les éléments de liste (temporaire) à la suite du vecteur.
    vector_ptr<T> &operator <<(vector_ptr<T> &&liste);

    //! Renvoie une référence sur le n-ième élément (sans vérification).
    T &operator[](size_type n)
        {return *(v_ptr::operator[](n));}

    //! Renvoie une référence sur le n-ième élément (sans vérification).
    const T &operator[](size_type n) const
        {return *(v_ptr::operator[](n));}

    //! Ajoute un élément au vecteur.
    void push_back(const T &value)
        {v_ptr::push_back(new T(value));}

    //! Ajoute un élément au vecteur.
    void push_back(T &&value)
        {v_ptr::push_back(new T(std::move(value)));}

    //! Remplace l'élément pointé par i qui est détruit, par l'élément pointé par ptr dont le vecteur prend posséssion.
    void set_ptr(size_type pos,std::unique_ptr<T> &&ptr) {
        if(v_ptr::operator[](pos))
            delete v_ptr::operator[](pos);
        v_ptr::operator[](pos) = ptr.release();
    }

    //! Redimension le vecteur à la taille count.
    template<class Factory> void resize(size_type count, Factory factory){
        auto i = size();
        if(count < i)
            v_ptr::resize(count);
        else {
            resize_null(count);
            while(i < count) {
                v_ptr::operator[](i) = factory();
                ++i;
            }
        }
    }

protected:
    //! Efface la liste de pointeurs, ceux-ci doivent être préalablement déplacés ou les objet pointés détruit.
    void clear_ptr() noexcept
        {v_ptr::clear();}

    //! Redimension en ajoutant des pointeur nul.
    void resize_null(size_type count) {
        if(count < size())
            for (auto it = std::next(v_ptr::cbegin(),count); it != v_ptr::cend(); ++it)
                delete *it;
        v_ptr::resize(count,nullptr);
    }
};

//////////////////// VectPtr //////////////////

template<class T> vector_ptr<T>::vector_ptr(const list_ptr<T> &liste)
    : v_ptr(liste.size()) {
    if(!liste.empty()) {
        auto it_vec = v_ptr::rbegin();
        for(auto it = liste.cbegin(); it != liste.cend(); ++it_vec, ++it)
            *it_vec = new T(**it);
    }
}

template<class T> vector_ptr<T>::vector_ptr(list_ptr<T> &&liste) noexcept
    : v_ptr(liste.size()) {
    if(!liste.empty()) {
        auto it_vec = v_ptr::rbegin();
        for(auto it = liste.cbegin(); it != liste.cend(); ++it_vec, ++it)
            *it_vec = *it;
        liste.clear_ptr();
    }
}

template<class T> template<class U> vector_ptr<T>::vector_ptr(vector_ptr<U> &&vect) noexcept
    : v_ptr(vect.size()) {
    if(!vect.empty()) {
        auto it_vec = v_ptr::begin();
        for(auto it = vect.std::template vector<U*>::cbegin(); it != vect.std::template vector<U*>::cend(); ++it_vec, ++it)
            *it_vec = *it;
        vect.clear_ptr();
    }
}

template<class T> template<class U> vector_ptr<T>::vector_ptr(list_ptr<U> &&list) noexcept
    : v_ptr(list.size()) {
    if(!list.empty()) {
        auto it_vec = v_ptr::begin();
        for(auto it = list.std::template forward_list<U*>::cbegin();
            it != list.std::template forward_list<U*>::cend(); ++it_vec, ++it)
            *it_vec = *it;
        list.clear_ptr();
    }
}

template<class T> typename vector_ptr<T>::const_iterator vector_ptr<T>::find_id(idt id) noexcept{
    const_iterator it = begin();
    while(it != end() &&it->id() != id)
        ++it;
    return it;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator =(const list_ptr<T> &liste) {
    if(!liste.empty()) {
        resize_null(liste.size());
        auto it_vec = v_ptr::rbegin();
        for(auto it = liste.cbegin(); it != liste.cend(); ++it_vec, ++it)
            *it_vec = new T(**it);
    }
    else
        clear();
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator =(list_ptr<T> &&liste) noexcept{
    if(!liste.empty()) {
        resize_null(liste.size());
        auto it_vec = v_ptr::rbegin();
        for(auto it = liste.cbegin(); it != liste.cend(); ++it_vec, ++it)
            *it_vec = *it;
        liste.clear_ptr();
    }
    else
        clear();
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator =(const vector_ptr<T> &vector) {
    if(!vector.empty()) {
        resize_null(vector.size());
        auto it_vec = v_ptr::begin();
        for(auto it = vector.cbegin(); it != vector.cend(); ++it_vec, ++it)
            *it_vec = new T(*it);
    }
    else
        clear();
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator =(vector_ptr<T> &&vector) noexcept{
    if(!vector.empty()) {
        resize_null(vector.size());
        auto it_vec = v_ptr::begin();
        for(auto it = vector.v_ptr::cbegin(); it != vector.v_ptr::cend(); ++it_vec, ++it)
            *it_vec = *it;
        vector.clear_ptr();
    }
    else
        clear();
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator << (const list_ptr<T> &liste) {
    if(!liste.empty()) {
        int n = size();
        resize_null(n + liste.size());
        auto it_vec = v_ptr::rbegin() + n;
        for(auto it = liste.cbegin(); it != liste.cend(); ++it_vec, ++it)
            *it_vec = new T(**it);
    }
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator <<(list_ptr<T> &&liste) {
    if(!liste.empty()) {
        int n = size();
        resize_null(n + liste.size());
        auto it_vec = v_ptr::rbegin() + n;
        for(auto it = liste.std::template forward_list<T*>::cbegin();
            it != liste.std::template forward_list<T*>::cend(); ++it_vec, ++it)
            *it_vec = *it;
        liste.clear_ptr();
    }
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator << (const vector_ptr<T> &vec) {
    if(!vec.empty()) {
        int n = size();
        resize_null(n + vec.size());
        auto it_vec = v_ptr::rbegin() + n;
        for(auto it = vec.cbegin(); it != vec.cend(); ++it_vec, ++it)
            *it_vec = new T(**it);
    }
    return *this;
}

template<class T> vector_ptr<T> &vector_ptr<T>::operator <<(vector_ptr<T> &&vec) {
    if(!vec.empty()) {
        int n = size();
        resize_null(n + vec.size());
        auto it_vec = v_ptr::rbegin() + n;
        for(auto it = vec.v_ptr::cbegin(); it != vec.v_ptr::cend(); ++it_vec, ++it)
            *it_vec = *it;
        vec.clear_ptr();
    }
    return *this;
}
}
#endif // CONTENEUR_PTR_H
