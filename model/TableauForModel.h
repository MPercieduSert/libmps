/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/09/2020
 */
#ifndef TABLEAUFORMODEL_H
#define TABLEAUFORMODEL_H

#include "ColonnesForModel.h"

namespace modelMPS {
///////////////////////////////////////// Tableau ///////////////////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de donné.
 */
template<class T, class Vec> class AbstractVecTableau : public AbstractColonnesModel::AbstractTableau {
protected:
    Vec m_vec;       //! Vecteur de donnée.
public:
    //! Constructeur d'une colonne vide.
    AbstractVecTableau() = default;
    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractVecTableau(szt size, const T & value = T()) : m_vec(size,value) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractVecTableau(const Vec & vec) : m_vec(vec) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractVecTableau(Vec & vec) : m_vec(std::move(vec)) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> AbstractVecTableau(szt size, Factory factory);

    //! Efface les données du tableau.
    void clear() override
        {m_vec.clear();}

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override {return m_vec[ligne1] == m_vec[ligne2];}

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override {m_vec.erase(std::next(m_vec.cbegin(),ligne));}

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Accesseur de la donnée d'une ligne.
    const T & internalData(szt ligne) const
        {return m_vec.at(ligne);}

    //! Accesseur de la donnée d'une ligne.
    T & internalData(szt ligne)
        {return m_vec.at(ligne);}

    //! Mutateur de la donnée d'une ligne.
    void setInternalData(szt ligne, const T & data)
        {m_vec.at(ligne) = data;}

    //! Mutateur du vecteur de donné.
    void setVecData(const Vec & data)
        {m_vec = data;}

    //! Mutateur du vecteur de donné.
    void setVecData(Vec && data)
        {m_vec = std::move(data);}

    //! Taille (nombre de lignes).
    szt size() const override {return m_vec.size();}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type std::vector<T>.
 */
template<class T> class VectorTableau : public AbstractVecTableau<T,std::vector<T>>{
protected:
    using AbstractVecTableau<T,std::vector<T>>::m_vec;
public:
    //! Constructeur.
    using AbstractVecTableau<T,std::vector<T>>::AbstractVecTableau;

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count);}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<AbstractColonnesModel::AbstractColonne>
        makeColonne(const AbstractColonnesModel::NewColonneInfo &info) override;
};


/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent>.
 */
template<class Ent> class AbstractVectorEntTableau : public AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>> {
protected:
    using AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>>::m_vec;
public:
    //! Constructeur.
    using AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>>::AbstractVecTableau;

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count, [](){return new Ent();});}

    //! Teste si la ligne correspond à une donnée interne.
    bool existsInternalData(szt ligne) const override {return !m_vec[ligne].isNew();}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const {return m_vec[ligne].isValid();}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent> lier à la base de donnée.
 */
template<class Ent> class AbstractBddVectorEntTableau : public AbstractVectorEntTableau<Ent> {
protected:
    using AbstractVectorEntTableau<Ent>::m_vec;
    bddMPS::Bdd & m_bdd;        //! Lien avec la base de données.
public:
    //! Constructeur d'une colonne vide
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd) : m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, szt size, const Ent & value = Ent())
        : AbstractVectorEntTableau<Ent>(size,value), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, const conteneurMPS::VectorPtr<Ent> & vec)
        : AbstractVectorEntTableau<Ent>(vec), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, conteneurMPS::VectorPtr<Ent> && vec)
        : AbstractVectorEntTableau<Ent>(std::move(vec)), m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, szt size, Factory factory)
        : AbstractVectorEntTableau<Ent>(size,factory), m_bdd(bdd) {}

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {m_bdd.get(m_vec[ligne]);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool newOrModif(szt ligne) const override {return m_vec[ligne].isNew() || !m_bdd.sameInBdd(m_vec[ligne]);}

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne) override {return m_bdd.del(m_vec[ligne]);}

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override {m_bdd.save(m_vec[ligne]);}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type std::vector<std::list<T>> avec la liste ordonnée.
 */
template<class T> class AbstractVectorListTableau : public AbstractVecTableau<T,std::vector<std::list<T>>>{
protected:
    using AbstractVecTableau<T,std::vector<std::list<T>>>::m_vec;
public:
    //! Constructeur d'une colonne vide.
    AbstractVectorListTableau() = default;
    //! Constructeur d'un tableau constitué de vec.
    AbstractVectorListTableau(const std::vector<std::list<T>> & vec)
        : AbstractVecTableau<T,std::vector<std::list<T>>>(vec) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractVectorListTableau(std::vector<std::list<T>> & vec)
        : AbstractVecTableau<T,std::vector<std::list<T>>>(std::move(vec)) {}

    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractVectorListTableau(szt size, const std::list<T> & value = std::list<T>())
        : AbstractVecTableau<T,std::vector<std::list<T>>>(std::vector<std::list<T>>(size,value)) {}

    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractVectorListTableau(szt size, const T & value)
        : AbstractVecTableau<T,std::vector<std::list<T>>>(std::vector<std::list<T>>(size,std::list<T>({value}))) {}

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count);}

    //! Ajoute un élément à une ligne.
    void addElement(szt ligne, const T & element) {
        m_vec.at(ligne).push_back(element);
        m_vec.at(ligne).sort();
    }

    //! Ajoute un élément à une ligne.
    void addElement(szt ligne, T && element) {
        m_vec.at(ligne).push_back(std::move(element));
        m_vec.at(ligne).sort();
    }

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;

    //! Comparaison d'égalité entre deux éléments d'une liste.
    virtual bool egalElement(const T & element1, const T & element2) const = 0;

    //! Accesseur de la donnée d'une ligne.
    const std::list<T> & internalData(szt ligne) const
        {return m_vec.at(ligne);}

    //! Accesseur de la donnée d'une ligne.
    std::list<T> & internalData(szt ligne)
        {return m_vec.at(ligne);}

    //! Mutateur de la donnée d'une ligne.
    void setInternalData(szt ligne, const std::list<T> & data) {
        m_vec.at(ligne) = data;
        m_vec.at(ligne).sort();
    }

    //! Mutateur de la donnée d'une ligne.
    void setInternalData(szt ligne, std::list<T> && data) {
        m_vec.at(ligne) = std::move(data);
        m_vec.at(ligne).sort();
    }
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent> lier à la base de donnée.
 */
template<class Ent> class AbstractBddVectorListTableau : public AbstractVectorListTableau<Ent> {
protected:
    using AbstractVectorListTableau<Ent>::m_vec;
    bddMPS::Bdd & m_bdd;        //! Lien avec la base de données.
public:
    //! Constructeur d'une colonne vide
    AbstractBddVectorListTableau(bddMPS::Bdd & bdd) : m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractBddVectorListTableau(bddMPS::Bdd & bdd, szt size)
        : AbstractVectorListTableau<Ent>(size), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractBddVectorListTableau(bddMPS::Bdd & bdd, const conteneurMPS::VectorPtr<Ent> & vec)
        : AbstractVectorEntTableau<Ent>(vec.size()), m_bdd(bdd) {
        auto iterVec = m_vec.begin();
        for(auto iter = vec.cbegin(); iter != vec.cend(); ++iter)
            iterVec->push_back(*iter);
    }

    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> AbstractBddVectorListTableau(bddMPS::Bdd & bdd, szt size, Factory factory)
        : AbstractVectorListTableau<Ent>(size,factory), m_bdd(bdd) {}

    //! Teste si la ligne correspond à une donnée interne.
    bool existsInternalData(szt ligne) const override;

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {
        for(auto iter = m_vec[ligne].begin(); iter != m_vec[ligne].end(); ++iter)
            m_bdd.get(*iter);
    }

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne) override;

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const override;
};

////////////////////////////////////////////////////////// CompositionTableau /////////////////////////////////////
//! Macro de définition des membres test de CompositionTableau.
#define TEST_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto iterTableau = m_tableaux.cbegin(); \
    auto iterTest = m_module[MODULE].cbegin(); \
    while(iterTableau != m_tableaux.cend() && (!*iterTest || (*iterTableau)->MEMBRE)){ \
        ++iterTableau; ++iterTest;} \
    return iterTableau == m_tableaux.cend();}

//! Macro de définition des membres modulaires de CompotionTableau.
#define OPERATION_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto iterActif = m_module[MODULE].cbegin(); \
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter, ++iterActif)  \
        if(*iterActif) (*iter)->MEMBRE;}

/*! \ingroup groupeModel
 * \brief Classe de composition paramétrée de plusieurs tableaux en un.
 */
class CompositionTableau : public AbstractColonnesModel::AbstractTableau {
protected:
    //! Type du tableau des modules actifs par tableaux.
    using ModuleActif = std::vector<std::vector<bool>>;
    std::vector<std::unique_ptr<AbstractTableau>> m_tableaux;   //!< Vecteur des pointeurs de tableau.
    ModuleActif m_module;                                       //!< Tableau des modules actif.
public:
    //! Module
    enum module {EgalModule,
                 ExistsModule,
                 HydrateModule,
                 NewOrModifModule,
                 RemoveModule,
                 SaveModule,
                 ValideModule,
                 NbrModule};

    //! Constructeur.
    CompositionTableau() : m_module(NbrModule) {}

    //!Destructeur.
    ~CompositionTableau() override = default;

    //! Acceseur des modules actif.
    bool actif(szt module, szt tableau) const
        {return m_module.at(module).at(tableau);}

    //! Ajoute count lignes au tableau.
    void add(szt count) override;

    //! Efface les données du tableau.
    void clear() override;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override;

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Teste si la ligne correspond à une donnée interne.
    bool existsInternalData(szt ligne) const override;

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<AbstractColonnesModel::AbstractColonne> makeColonne(const AbstractColonnesModel::NewColonneInfo & info) override
        {return m_tableaux.at(info.tableau)->makeColonne(info);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool newOrModif(szt ligne) const override;

    //! Ajoute un tableau à la composition. Le nouveau tableau doit avoir la même taille que ceux auquel il est associé.
    void push_back(std::unique_ptr<AbstractTableau> && tableau, bool actif = true);

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne) override;

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;

    //! Mutateurs des modules actifs.
    void setActif(szt module, szt tableau, bool actif = true)
        {m_module.at(module).at(tableau) = actif;}

    //! Taille (nombre de lignes).
    szt size() const override
        {return m_tableaux.empty() ? 0 : m_tableaux.front()->size();}

    //! Accesseur des tableaux.
    AbstractTableau & tableau(szt pos)
        {return *m_tableaux.at(pos);}

    //! Accesseur des tableaux.
    const AbstractTableau & tableau(szt pos) const
        {return *m_tableaux.at(pos);}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const override;
};

////////////////////////////////////////////////////////// HeterogeneTailleTableau /////////////////////////////////////

/*! \ingroup groupeModel
 * \brief Classe d'un tableau composé de listes de longueurs hétérogènes.
 */
template<class T> class HeterogeneTailleTableau : public AbstractColonnesModel::AbstractTableau {
protected:
    szt m_nbrLine = 0;                     //!< Nombre de lignes du tableau (possiblement virtuel).
    std::vector<std::unique_ptr<std::vector<T>>> m_tableau;   //!< Vecteur des lists.
public:
    //! Constructeur.
    HeterogeneTailleTableau(szt count = 0) : m_tableau(count) {}

    //!Destructeur.
    ~HeterogeneTailleTableau() override = default;

    //! Ajoute count lignes au tableau.
    void add(szt count) override
        {m_nbrLine += count;}

    //! Efface les données du tableau.
    void clear() override {
        for (auto iter = m_tableau.begin();iter != m_tableau.end(); ++iter)
            (**iter).clear();
        m_nbrLine = 0;
    }

    //! Acceseur d'une des colonnes du tableau avec vérification.
    const std::vector<T> & colonneAt(szt num) const
        {return *m_tableau.at(num);}

    //! Acceseur d'une des colonnes du tableau avec vérification.
    std::vector<T> & colonneAt(szt num)
        {return *m_tableau.at(num);}

    //! Acceseur d'une des colonnes du tableau sans vérification.
    const std::vector<T> & colonne(szt num) const
        {return *m_tableau[num];}

    //! Acceseur d'une des colonnes du tableau sans vérification.
    std::vector<T> & colonne(szt num)
        {return *m_tableau[num];}

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override {
        if(ligne < m_nbrLine) {
            for (auto iter = m_tableau.begin();iter != m_tableau.end(); ++iter)
                if(ligne < (**iter).size())
                    (**iter).erase(std::next((**iter).cbegin(),ligne));
            m_nbrLine -= 1;
        }
    }

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Ajoute un tableau de taille size d'élément par défaut.
    void push_back(typename std::vector<T>::size_type size)
        {m_tableau.push_back(std::make_unique<std::vector<T>>(size));}

    //! Ajoute un tableau.
    void push_back(const std::vector<T> & vec = std::vector<T>())
        {m_tableau.push_back(std::make_unique<std::vector<T>>(vec));}

    //! Ajoute un tableau.
    void push_back(std::vector<T> && vec)
        {m_tableau.push_back(std::make_unique<std::vector<T>>(std::move(vec)));}

    //! Taille (nombre de lignes).
    szt size() const override
        {return m_nbrLine;}

    //! Nombre de colonnes.
    szt sizeColumn() const
        {return m_tableau.size();}

    //! Taille réelle de la colonne.
    szt sizeOfColumn(szt num) const
        {return m_tableau.at(num)->size();}

    //! Met-à-jour le nombre de ligne.
    void updateNbrLine();
};

////////////////////////////////////////////////// Définition //////////////////////////////////////////////////
template<class T, class Vec> template<class Factory> AbstractVecTableau<T,Vec>::AbstractVecTableau(szt size, Factory factory) {
    m_vec.reserve(size);
    for(szt i = 0; i != size; ++i)
        m_vec.push_back(factory(i));
}

template<class T, class Vec> void AbstractVecTableau<T,Vec>::erase(szt first, szt last)
    {m_vec.erase(std::next(m_vec.cbegin(),first),std::next(m_vec.cbegin(),last));}

template<class T> bool AbstractVectorListTableau<T>::egal(szt ligne1, szt ligne2) const {
    if(m_vec[ligne1].size() != m_vec[ligne2].size())
        return false;
    auto iter1 = m_vec[ligne1].cbegin();
    auto iter2 = m_vec[ligne2].cbegin();
    while(iter1 != m_vec[ligne1].cend() && egalElement(*iter1,*iter2)) {
        ++iter1;
        ++iter2;
    }
    return iter1 == m_vec[ligne1].cend();
}

template<class T> bool AbstractBddVectorListTableau<T>::existsInternalData(szt ligne) const {
    if(m_vec[ligne].empty())
        return false;
    auto iter = m_vec[ligne].cbegin();
    while(iter != m_vec[ligne].cend() && iter->isNew())
        ++iter;
    return iter == m_vec[ligne].cend();
}

template<class T> bool AbstractBddVectorListTableau<T>::removeInternalData(szt ligne) {
    if(m_vec[ligne].empty())
        return true;
    auto iter = m_vec[ligne].cbegin();
    while(iter != m_vec[ligne].cend() && m_bdd.del(*iter))
        ++iter;
    return iter == m_vec[ligne].cend();
}

template<class T> void AbstractBddVectorListTableau<T>::save(szt ligne) {
    if(!m_vec[ligne].empty())
        for(auto iter = m_vec[ligne].cbegin(); iter != m_vec[ligne].cend(); ++iter)
            m_bdd.save(*iter);
}

template<class T> bool AbstractBddVectorListTableau<T>::valide(szt ligne) const {
    if(m_vec[ligne].empty())
        return true;
    auto iter = m_vec[ligne].cbegin();
    while(iter != m_vec[ligne].cend() && iter->isValid())
        ++iter;
    return iter == m_vec[ligne].cend();
}

template <class T> std::unique_ptr<AbstractColonnesModel::AbstractColonne>
                        VectorTableau<T>::makeColonne(const AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.type) {
    case AbstractColonnesModel::BoolColonne:
        if(info.args.size() == 0)
            return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;});
        if(info.args.size() == 2)
            return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;},
                                                         info.args[0].toString(),info.args[1].toString());
    case AbstractColonnesModel::DoubleColonne:
    case AbstractColonnesModel::IntColonne:
    case AbstractColonnesModel::UIntColonne:
        return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                    [](T element, int role){
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                            return element;
                        return QVariant();},
                    [](const QVariant & value, T element, int role){
                        if(role == Qt::EditRole)
                            element = value.value<T>();
                        return QVariant();});
    default:
        return std::make_unique<VectorRefColonne<T>>(info.name,info.flags,info.type,m_vec,
                    [](const T & element, int role){
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                            return element;
                        return QVariant();},
                    [](const QVariant & value, T & element, int role){
                        if(role == Qt::EditRole)
                            element = value.value<T>();
                        return QVariant();});
    }
}

template<class T> bool HeterogeneTailleTableau<T>::egal(szt ligne1, szt ligne2) const {
    auto iter = m_tableau.begin();
    while(iter != m_tableau.end()
          && ((ligne1 >= (**iter).size() && ligne2 >= (**iter).size())
               || (ligne1 < (**iter).size() && ligne2 < (**iter).size() && (**iter).operator[](ligne1) == (**iter).operator[](ligne2))))
        ++iter;
    return iter == m_tableau.end();
}

template<class T> void HeterogeneTailleTableau<T>::erase(szt first, szt last) {
     if(first < m_nbrLine && last <= m_nbrLine && first < last) {
        for (auto iter = m_tableau.begin();iter != m_tableau.end(); ++iter)
            if(first < (**iter).size()) {
                if(last < (**iter).size())
                    (**iter).erase(std::next((**iter).cbegin(),first),std::next((**iter).cbegin(),last));
                else
                    (**iter).erase(std::next((**iter).cbegin(),first),(**iter).cend());
            }
        m_nbrLine -= last-first;
    }
}

template<class T> void HeterogeneTailleTableau<T>::updateNbrLine(){
    m_nbrLine = 0;
    for (auto iter = m_tableau.cbegin(); iter != m_tableau.cend(); ++iter) {
        if(m_nbrLine < (**iter).size())
            m_nbrLine = (**iter).size();
    }
}
}
#endif // TABLEAUFORMODEL_H
