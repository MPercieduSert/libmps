/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef ABSTRACTENTITYTABLEAU_H
#define ABSTRACTENTITYTABLEAU_H

#include "abstract_colonnes_model.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Classe mère des models de type tableau d'entités.
 */
class abstract_entity_tableau : public abstract_colonnes_model::abstract_tableau {
public:
    //! Type du tableau de d'entités.
    using Tableau = std::list<vector_ptr<entities::entity>>;

protected:
    b2d::bdd &m_bdd;    //! Lien avec la base de données.
    Tableau m_data;         //!< Donnée du model.

public:
    //! Constructeur.
    abstract_entity_tableau(b2d::bdd &bdd) : m_bdd(bdd) {}

    //! Destructeur.
    ~abstract_entity_tableau() override = default;

    // Ajoute count lignes au tableau.
    void add(szt count) override;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override;

    //! Supprime les lignes [first,last] des données du model.
    void erase(szt first, szt last) override;

    //! Taille (nombre de lignes).
    szt size() const override {return m_data.empty() ? 0 : m_data.front().size();}

    //! Ajoute une colonne de données et renvoie son identifiant (Factory: unique_ptr<entity>(*factory)(szt)).
    template<class Factory> Tableau::iterator push_back_data(Factory factory, szt count = 0);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_back_data(const vector_ptr<Ent> &vec);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_back_data(vector_ptr<Ent> &&vec);

protected:
    //! Renvoie un pointeur vers une entity.
    virtual entities::entity *entity_factory(szt ligne, szt pos) = 0;
};

template<class Factory> abstract_entity_tableau::Tableau::iterator
        abstract_entity_tableau::push_back_data(Factory factory, szt count) {
    if(count == 0)
        count = size();
    else if(!m_data.empty() && size() != count)
        throw std::invalid_argument("AbstractTableentityModel::Tableau::iterator "
                                    "AbstractTableentityModel::push_back_data(szt):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    auto col = vector_ptr<entities::entity>(count,nullptr);
    for (szt i = 0; i < col.size(); ++i)
        col.set_ptr(i,factory(i));
    return m_data.insert(m_data.end(),std::move(col));
}

template<class Ent> abstract_entity_tableau::Tableau::iterator
        abstract_entity_tableau::push_back_data(const vector_ptr<Ent> &vec) {
    if(!m_data.empty() &&vec.size() != size())
        throw std::invalid_argument("AbstractTableentityModel::Tableau::iterator "
                                    "AbstractTableentityModel::push_back_data(const vector_ptr<Ent> &):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    return m_data.insert(m_data.end(),vec);
}

template<class Ent> abstract_entity_tableau::Tableau::iterator
        abstract_entity_tableau::push_back_data(vector_ptr<Ent> &&vec) {
    if(!m_data.empty() &&vec.size() != size())
        throw std::invalid_argument("AbstractTableentityModel::Tableau::iterator "
                                    "AbstractTableentityModel::push_back_data(vector_ptr<Ent> &&):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre de lignes du model.");
    return m_data.insert(m_data.end(),std::move(vec));
}
}}
#endif // ABSTRACTENTITYTABLEAU_H
