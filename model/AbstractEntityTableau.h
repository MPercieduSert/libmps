/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef ABSTRACTENTITYTABLEAU_H
#define ABSTRACTENTITYTABLEAU_H

#include "AbstractColonnesModel.h"

namespace modelMPS {
namespace cmps = conteneurMPS;
using Entity = entityMPS::Entity;
/*! \ingroup groupeModel
 * \brief Classe mère des models de type tableau d'entités.
 */
class AbstractEntityTableau : public AbstractColonnesModel::AbstractTableau {
public:
    //! Type du tableau de d'entités.
    using Tableau = std::list<cmps::VectorPtr<Entity>>;

protected:
    bddMPS::Bdd & m_bdd;    //! Lien avec la base de données.
    Tableau m_data;         //!< Donnée du model.

public:
    //! Constructeur.
    AbstractEntityTableau(bddMPS::Bdd & bdd) : m_bdd(bdd) {}

    //! Destructeur.
    ~AbstractEntityTableau() override = default;

    // Ajoute count lignes au tableau.
    void add(szt count) override;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override;

    //! Supprime les lignes [first,last] des données du model.
    void erase(szt first, szt last) override;

//    //! Fabrique une colonne lié aux données du tableau.
//    virtual std::unique_ptr<AbstractColonne> makeColonne(const QString & name, int type,
//                                                         Qt::ItemFlags flags,int pos) const = 0;

    //! Taille (nombre de lignes).
    szt size() const override {return m_data.empty() ? 0 : m_data.front().size();}

//    //! Selectionne les lignes à afficher vérifiant une condition
//    template<class UnaryPredicat> void find(UnaryPredicat predicat, bool reset = true);

    //! Ajoute une colonne de données et renvoie son identifiant (Factory: unique_ptr<Entity>(*factory)(szt)).
    template<class Factory> Tableau::iterator push_backData(Factory factory, szt count = 0);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_backData(const cmps::VectorPtr<Ent> & vec);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_backData(cmps::VectorPtr<Ent> && vec);

protected:
    //! Renvoie un pointeur vers une entity.
    virtual Entity * entityFactory(szt ligne, szt pos) = 0;
};

/////////////////////////////////// AbstractTableEntityModel /////////////////////////////////////////

//template<class UnaryPredicat> void AbstractTableEntityModel::find(UnaryPredicat predicat, bool reset) {
//    beginResetModel();
//        if(reset && nbrRow() != nbrLignes())
//            resetRowToLigne();
//        szt size = 0;
//        auto write = m_rowToLigne.begin();
//        for (auto read = m_rowToLigne.cbegin(); read != m_rowToLigne.cend(); ++read) {
//            if(predicat(*read)) {
//                *write = *read;
//                ++write;
//                ++size;
//            }
//        }
//        m_rowToLigne.resize(size);
//    endResetModel();
//}

template<class Factory> AbstractEntityTableau::Tableau::iterator
        AbstractEntityTableau::push_backData(Factory factory, szt count) {
    if(count == 0)
        count = size();
    else if(!m_data.empty() &&  size() != count)
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(szt):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    auto col = cmps::VectorPtr<Entity>(count,nullptr);
    for (szt i = 0; i < col.size(); ++i)
        col.setPtr(i,factory(i));
    return m_data.insert(m_data.end(),std::move(col));
}

template<class Ent> AbstractEntityTableau::Tableau::iterator
        AbstractEntityTableau::push_backData(const cmps::VectorPtr<Ent> & vec) {
    if(!m_data.empty() && vec.size() != size())
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(const cmps::VectorPtr<Ent> &):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    return m_data.insert(m_data.end(),vec);
}

template<class Ent> AbstractEntityTableau::Tableau::iterator
        AbstractEntityTableau::push_backData(cmps::VectorPtr<Ent> && vec) {
    if(!m_data.empty() && vec.size() != size())
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(cmps::VectorPtr<Ent> &&):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre de lignes du model.");
    return m_data.insert(m_data.end(),std::move(vec));
}
}
#endif // ABSTRACTENTITYTABLEAU_H
