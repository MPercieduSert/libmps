/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/09/2020
 */
#ifndef TABLEAU_FOR_MODEL_H
#define TABLEAU_FOR_MODEL_H

#include "colonnes_for_model.h"

namespace mps {
namespace model_base {
///////////////////////////////////////// Tableau ///////////////////////////////////////////////////////////
/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de donné.
 */
template<class T, class Vec> class abstract_vec_tableau : public abstract_colonnes_model::abstract_tableau {
protected:
    Vec m_vec;       //! Vecteur de donnée.
public:
    //! Constructeur d'une colonne vide.
    abstract_vec_tableau() = default;
    //! Constructeur d'un tableau d'une colonne de size value.
    abstract_vec_tableau(szt size, const T &value = T()) : m_vec(size,value) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_vec_tableau(const Vec &vec) : m_vec(vec) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_vec_tableau(Vec &vec) : m_vec(std::move(vec)) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> abstract_vec_tableau(szt size, Factory factory);

    //! Efface les données du tableau.
    void clear() override
        {m_vec.clear();}

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override {return m_vec[ligne_1] == m_vec[ligne_2];}

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override {m_vec.erase(std::next(m_vec.cbegin(),ligne));}

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Accesseur de la donnée d'une ligne.
    const T &internal_data(szt ligne) const
        {return m_vec.at(ligne);}

    //! Accesseur de la donnée d'une ligne.
    T &internal_data(szt ligne)
        {return m_vec.at(ligne);}

    //! Mutateur de la donnée d'une ligne.
    void set_internal_data(szt ligne, const T &data)
        {m_vec.at(ligne) = data;}

    //! Mutateur du vecteur de donné.
    void set_vec_data(const Vec &data)
        {m_vec = data;}

    //! Mutateur du vecteur de donné.
    void set_vec_data(Vec &&data)
        {m_vec = std::move(data);}

    //! Taille (nombre de lignes).
    szt size() const override {return m_vec.size();}
};

/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de type std::vector<T>.
 */
template<class T> class vector_tableau : public abstract_vec_tableau<T,std::vector<T>>{
protected:
    using abstract_vec_tableau<T,std::vector<T>>::m_vec;
public:
    //! Constructeur.
    using abstract_vec_tableau<T,std::vector<T>>::abstract_vec_tableau;

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count);}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<abstract_colonnes_model::abstract_colonne>
        make_colonne(const abstract_colonnes_model::new_colonne_info &info) override;
};


/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent>.
 */
template<class Ent> class abstract_vector_ent_tableau : public abstract_vec_tableau<Ent,vector_ptr<Ent>> {
protected:
    using abstract_vec_tableau<Ent,vector_ptr<Ent>>::m_vec;
public:
    //! Constructeur.
    using abstract_vec_tableau<Ent,vector_ptr<Ent>>::abstract_vec_tableau;

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count, [](){return new Ent();});}

    //! Teste si la ligne correspond à une donnée interne.
    bool exists_internal_data(szt ligne) const override {return !m_vec[ligne].is_new();}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const {return m_vec[ligne].is_valid();}
};

/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent> lier à la base de donnée.
 */
template<class Ent> class abstract_bdd_vector_ent_tableau : public abstract_vector_ent_tableau<Ent> {
protected:
    using abstract_vector_ent_tableau<Ent>::m_vec;
    b2d::bdd &m_bdd;        //! Lien avec la base de données.
public:
    //! Constructeur d'une colonne vide
    abstract_bdd_vector_ent_tableau(b2d::bdd &bdd) : m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size value.
    abstract_bdd_vector_ent_tableau(b2d::bdd &bdd, szt size, const Ent &value = Ent())
        : abstract_vector_ent_tableau<Ent>(size,value), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_bdd_vector_ent_tableau(b2d::bdd &bdd, const vector_ptr<Ent> &vec)
        : abstract_vector_ent_tableau<Ent>(vec), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_bdd_vector_ent_tableau(b2d::bdd &bdd, vector_ptr<Ent> &&vec)
        : abstract_vector_ent_tableau<Ent>(std::move(vec)), m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> abstract_bdd_vector_ent_tableau(b2d::bdd &bdd, szt size, Factory factory)
        : abstract_vector_ent_tableau<Ent>(size,factory), m_bdd(bdd) {}

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {m_bdd.get(m_vec[ligne]);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool new_or_modif(szt ligne) const override {return m_vec[ligne].is_new() || !m_bdd.same_in_bdd(m_vec[ligne]);}

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool remove_internal_data(szt ligne) override {return m_bdd.del(m_vec[ligne]);}

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override {m_bdd.save(m_vec[ligne]);}
};

/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de type std::vector<std::list<T>> avec la liste ordonnée.
 */
template<class T> class abstract_vector_list_tableau : public abstract_vec_tableau<T,std::vector<std::list<T>>>{
protected:
    using abstract_vec_tableau<T,std::vector<std::list<T>>>::m_vec;
public:
    //! Constructeur d'une colonne vide.
    abstract_vector_list_tableau() = default;
    //! Constructeur d'un tableau constitué de vec.
    abstract_vector_list_tableau(const std::vector<std::list<T>> &vec)
        : abstract_vec_tableau<T,std::vector<std::list<T>>>(vec) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_vector_list_tableau(std::vector<std::list<T>> &vec)
        : abstract_vec_tableau<T,std::vector<std::list<T>>>(std::move(vec)) {}

    //! Constructeur d'un tableau d'une colonne de size value.
    abstract_vector_list_tableau(szt size, const std::list<T> &value = std::list<T>())
        : abstract_vec_tableau<T,std::vector<std::list<T>>>(std::vector<std::list<T>>(size,value)) {}

    //! Constructeur d'un tableau d'une colonne de size value.
    abstract_vector_list_tableau(szt size, const T &value)
        : abstract_vec_tableau<T,std::vector<std::list<T>>>(std::vector<std::list<T>>(size,std::list<T>({value}))) {}

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count);}

    //! Ajoute un élément à une ligne.
    void add_element(szt ligne, const T &element) {
        m_vec.at(ligne).push_back(element);
        m_vec.at(ligne).sort();
    }

    //! Ajoute un élément à une ligne.
    void add_element(szt ligne, T &&element) {
        m_vec.at(ligne).push_back(std::move(element));
        m_vec.at(ligne).sort();
    }

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Comparaison d'égalité entre deux éléments d'une liste.
    virtual bool egal_element(const T &element1, const T &element2) const = 0;

    //! Accesseur de la donnée d'une ligne.
    const std::list<T> &internal_data(szt ligne) const
        {return m_vec.at(ligne);}

    //! Accesseur de la donnée d'une ligne.
    std::list<T> &internal_data(szt ligne)
        {return m_vec.at(ligne);}

    //! Mutateur de la donnée d'une ligne.
    void set_internal_data(szt ligne, const std::list<T> &data) {
        m_vec.at(ligne) = data;
        m_vec.at(ligne).sort();
    }

    //! Mutateur de la donnée d'une ligne.
    void set_internal_data(szt ligne, std::list<T> &&data) {
        m_vec.at(ligne) = std::move(data);
        m_vec.at(ligne).sort();
    }
};

/*! \ingroup groupe_model
 *\brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent> lier à la base de donnée.
 */
template<class Ent> class abstract_bdd_vector_list_tableau : public abstract_vector_list_tableau<Ent> {
protected:
    using abstract_vector_list_tableau<Ent>::m_vec;
    b2d::bdd &m_bdd;        //! Lien avec la base de données.
public:
    //! Constructeur d'une colonne vide
    abstract_bdd_vector_list_tableau(b2d::bdd &bdd) : m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size value.
    abstract_bdd_vector_list_tableau(b2d::bdd &bdd, szt size)
        : abstract_vector_list_tableau<Ent>(size), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    abstract_bdd_vector_list_tableau(b2d::bdd &bdd, const vector_ptr<Ent> &vec)
        : abstract_vector_ent_tableau<Ent>(vec.size()), m_bdd(bdd) {
        auto vec_it = m_vec.begin();
        for(auto it = vec.cbegin(); it != vec.cend(); ++it)
            vec_it->push_back(*it);
    }

    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> abstract_bdd_vector_list_tableau(b2d::bdd &bdd, szt size, Factory factory)
        : abstract_vector_list_tableau<Ent>(size,factory), m_bdd(bdd) {}

    //! Teste si la ligne correspond à une donnée interne.
    bool exists_internal_data(szt ligne) const override;

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {
        for(auto it = m_vec[ligne].begin(); it != m_vec[ligne].end(); ++it)
            m_bdd.get(*it);
    }

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool remove_internal_data(szt ligne) override;

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const override;
};

////////////////////////////////////////////////////////// composition_tableau /////////////////////////////////////
//! Macro de définition des membres test de composition_tableau.
#define TEST_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto tab_it = m_tableaux.cbegin(); \
    auto test_it = m_module[MODULE].cbegin(); \
    while(tab_it != m_tableaux.cend() &&(!*test_it || (*tab_it)->MEMBRE)){ \
        ++tab_it; ++test_it;} \
    return tab_it == m_tableaux.cend();}

//! Macro de définition des membres modulaires de CompotionTableau.
#define OPERATION_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto actif_it = m_module[MODULE].cbegin(); \
    for(auto it = m_tableaux.begin(); it != m_tableaux.end(); ++it, ++actif_it)  \
        if(*actif_it) (*it)->MEMBRE;}

/*! \ingroup groupe_model
 *\brief Classe de composition paramétrée de plusieurs tableaux en un.
 */
class composition_tableau : public abstract_colonnes_model::abstract_tableau {
protected:
    //! Type du tableau des modules actifs par tableaux.
    using module_actif = std::vector<std::vector<bool>>;
    std::vector<std::unique_ptr<abstract_tableau>> m_tableaux;   //!< Vecteur des pointeurs de tableau.
    module_actif m_module;                                       //!< Tableau des modules actif.
public:
    //! Module
    enum module {Egal_Module,
                 Exists_Module,
                 Hydrate_Module,
                 New_Or_Modif_Module,
                 Remove_Module,
                 Save_Module,
                 Valide_Module,
                 Nbr_Module};

    //! Constructeur.
    composition_tableau() : m_module(Nbr_Module) {}

    //!Destructeur.
    ~composition_tableau() override = default;

    //! Acceseur des modules actif.
    bool actif(szt module, szt tableau) const
        {return m_module.at(module).at(tableau);}

    //! Ajoute count lignes au tableau.
    void add(szt count) override;

    //! Efface les données du tableau.
    void clear() override;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override;

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Teste si la ligne correspond à une donnée interne.
    bool exists_internal_data(szt ligne) const override;

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<abstract_colonnes_model::abstract_colonne> make_colonne(const abstract_colonnes_model::new_colonne_info &info) override
        {return m_tableaux.at(info.tableau)->make_colonne(info);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool new_or_modif(szt ligne) const override;

    //! Ajoute un tableau à la composition. Le nouveau tableau doit avoir la même taille que ceux auquel il est associé.
    void push_back(std::unique_ptr<abstract_tableau> &&tableau, bool actif = true);

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool remove_internal_data(szt ligne) override;

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;

    //! Mutateurs des modules actifs.
    void set_actif(szt module, szt tableau, bool actif = true)
        {m_module.at(module).at(tableau) = actif;}

    //! Taille (nombre de lignes).
    szt size() const override
        {return m_tableaux.empty() ? 0 : m_tableaux.front()->size();}

    //! Accesseur des tableaux.
    abstract_tableau &tableau(szt pos)
        {return *m_tableaux.at(pos);}

    //! Accesseur des tableaux.
    const abstract_tableau &tableau(szt pos) const
        {return *m_tableaux.at(pos);}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const override;
};

////////////////////////////////////////////////////////// heterogene_taille_tableau /////////////////////////////////////

/*! \ingroup groupe_model
 *\brief Classe d'un tableau composé de listes de longueurs hétérogènes.
 */
template<class T> class heterogene_taille_tableau : public abstract_colonnes_model::abstract_tableau {
protected:
    szt m_nbrLine = 0;                                          //!< Nombre de lignes du tableau (possiblement virtuel).
    std::vector<std::unique_ptr<std::vector<T>>> m_tableau;     //!< Vecteur des lists.
public:
    //! Constructeur.
    heterogene_taille_tableau(szt count = 0) : m_tableau(count) {}

    //!Destructeur.
    ~heterogene_taille_tableau() override = default;

    //! Ajoute count lignes au tableau.
    void add(szt count) override
        {m_nbrLine += count;}

    //! Efface les données du tableau.
    void clear() override {
        for (auto it = m_tableau.begin();it != m_tableau.end(); ++it)
            (**it).clear();
        m_nbrLine = 0;
    }

    //! Acceseur d'une des colonnes du tableau avec vérification.
    const std::vector<T> &colonne_at(szt num) const
        {return *m_tableau.at(num);}

    //! Acceseur d'une des colonnes du tableau avec vérification.
    std::vector<T> &colonne_at(szt num)
        {return *m_tableau.at(num);}

    //! Acceseur d'une des colonnes du tableau sans vérification.
    const std::vector<T> &colonne(szt num) const
        {return *m_tableau[num];}

    //! Acceseur d'une des colonnes du tableau sans vérification.
    std::vector<T> &colonne(szt num)
        {return *m_tableau[num];}

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override {
        if(ligne < m_nbrLine) {
            for (auto it = m_tableau.begin();it != m_tableau.end(); ++it)
                if(ligne < (**it).size())
                    (**it).erase(std::next((**it).cbegin(),ligne));
            m_nbrLine -= 1;
        }
    }

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Ajoute un tableau de taille size d'élément par défaut.
    void push_back(typename std::vector<T>::size_type size)
        {m_tableau.push_back(std::make_unique<std::vector<T>>(size));}

    //! Ajoute un tableau.
    void push_back(const std::vector<T> &vec = std::vector<T>())
        {m_tableau.push_back(std::make_unique<std::vector<T>>(vec));}

    //! Ajoute un tableau.
    void push_back(std::vector<T> &&vec)
        {m_tableau.push_back(std::make_unique<std::vector<T>>(std::move(vec)));}

    //! Taille (nombre de lignes).
    szt size() const override
        {return m_nbrLine;}

    //! Nombre de colonnes.
    szt size_column() const
        {return m_tableau.size();}

    //! Taille réelle de la colonne.
    szt size_of_column(szt num) const
        {return m_tableau.at(num)->size();}

    //! Met-à-jour le nombre de ligne.
    void update_nbr_line();
};

////////////////////////////////////////////////// Définition //////////////////////////////////////////////////
template<class T, class Vec> template<class Factory> abstract_vec_tableau<T,Vec>::abstract_vec_tableau(szt size, Factory factory) {
    m_vec.reserve(size);
    for(szt i = 0; i != size; ++i)
        m_vec.push_back(factory(i));
}

template<class T, class Vec> void abstract_vec_tableau<T,Vec>::erase(szt first, szt last)
    {m_vec.erase(std::next(m_vec.cbegin(),first),std::next(m_vec.cbegin(),last));}

template<class T> bool abstract_vector_list_tableau<T>::egal(szt ligne_1, szt ligne_2) const {
    if(m_vec[ligne_1].size() != m_vec[ligne_2].size())
        return false;
    auto it_1 = m_vec[ligne_1].cbegin();
    auto it_2 = m_vec[ligne_2].cbegin();
    while(it_1 != m_vec[ligne_1].cend() &&egal_element(*it_1,*it_2)) {
        ++it_1;
        ++it_2;
    }
    return it_1 == m_vec[ligne_1].cend();
}

template<class T> bool abstract_bdd_vector_list_tableau<T>::exists_internal_data(szt ligne) const {
    if(m_vec[ligne].empty())
        return false;
    auto it = m_vec[ligne].cbegin();
    while(it != m_vec[ligne].cend() &&it->is_new())
        ++it;
    return it == m_vec[ligne].cend();
}

template<class T> bool abstract_bdd_vector_list_tableau<T>::remove_internal_data(szt ligne) {
    if(m_vec[ligne].empty())
        return true;
    auto it = m_vec[ligne].cbegin();
    while(it != m_vec[ligne].cend() &&m_bdd.del(*it))
        ++it;
    return it == m_vec[ligne].cend();
}

template<class T> void abstract_bdd_vector_list_tableau<T>::save(szt ligne) {
    if(!m_vec[ligne].empty())
        for(auto it = m_vec[ligne].cbegin(); it != m_vec[ligne].cend(); ++it)
            m_bdd.save(*it);
}

template<class T> bool abstract_bdd_vector_list_tableau<T>::valide(szt ligne) const {
    if(m_vec[ligne].empty())
        return true;
    auto it = m_vec[ligne].cbegin();
    while(it != m_vec[ligne].cend() &&it->is_valid())
        ++it;
    return it == m_vec[ligne].cend();
}

template <class T> std::unique_ptr<abstract_colonnes_model::abstract_colonne>
                        vector_tableau<T>::make_colonne(const abstract_colonnes_model::new_colonne_info &info) {
    switch (info.type) {
    case Bool_Colonne_Type:
        if(info.args.size() == 0)
            return std::make_unique<vector_val_colonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;});
        if(info.args.size() == 2)
            return std::make_unique<vector_val_colonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;},
                                                         info.args[0].toString(),info.args[1].toString());
    case Double_Colonne_Type:
    case Int_Colonne_Type:
    case UInt_Colonne_Type:
        return std::make_unique<vector_val_colonne<T>>(info.name,info.flags,info.type,m_vec,
                    [](T element, int role){
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                            return element;
                        return QVariant();},
                    [](const QVariant &value, T element, int role){
                        if(role == Qt::EditRole)
                            element = value.value<T>();
                        return QVariant();});
    default:
        return std::make_unique<vector_ref_colonne<T>>(info.name,info.flags,info.type,m_vec,
                    [](const T &element, int role){
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                            return element;
                        return QVariant();},
                    [](const QVariant &value, T &element, int role){
                        if(role == Qt::EditRole)
                            element = value.value<T>();
                        return QVariant();});
    }
}

template<class T> bool heterogene_taille_tableau<T>::egal(szt ligne_1, szt ligne_2) const {
    auto it = m_tableau.begin();
    while(it != m_tableau.end()
          &&((ligne_1 >= (**it).size() &&ligne_2 >= (**it).size())
               || (ligne_1 < (**it).size() &&ligne_2 < (**it).size() &&(**it).operator[](ligne_1) == (**it).operator[](ligne_2))))
        ++it;
    return it == m_tableau.end();
}

template<class T> void heterogene_taille_tableau<T>::erase(szt first, szt last) {
     if(first < m_nbrLine &&last <= m_nbrLine &&first < last) {
        for (auto it = m_tableau.begin();it != m_tableau.end(); ++it)
            if(first < (**it).size()) {
                if(last < (**it).size())
                    (**it).erase(std::next((**it).cbegin(),first),std::next((**it).cbegin(),last));
                else
                    (**it).erase(std::next((**it).cbegin(),first),(**it).cend());
            }
        m_nbrLine -= last-first;
    }
}

template<class T> void heterogene_taille_tableau<T>::update_nbr_line(){
    m_nbrLine = 0;
    for (auto it = m_tableau.cbegin(); it != m_tableau.cend(); ++it) {
        if(m_nbrLine < (**it).size())
            m_nbrLine = (**it).size();
    }
}
}}
#endif // TABLEAU_FOR_MODEL_H
