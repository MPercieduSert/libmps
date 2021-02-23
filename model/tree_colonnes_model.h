/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/05/2019
 */
#ifndef TREE_COLONNES_MODEL_H
#define TREE_COLONNES_MODEL_H

#include "abstract_colonnes_model.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Classe mère des model de type arbre à colonnes homogènes.
 */
class tree_colonnes_model : public abstract_colonnes_model {
    Q_OBJECT
public:
    using arbre = tree_for_model<szt>;
protected:
    arbre m_tree;           //! arbre de correspondance des lignes visibles.
    tree<szt> m_full_tree;   //! arbre de correspondance de toutes les lignes.
public:
    TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(m_tree)    // Implémentation des méthodes virtuelles index, parent, row_count.

    //! Constructeur.
    tree_colonnes_model(bool racine, QObject *parent);

    //! Destructeur.
    ~tree_colonnes_model() override = default;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    void find(Abstract_Find_Model *find_model) override;

    //! Insert count lignes dans le model juste avant la ligne row.
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Supprime count lignes en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

protected:
    //! Fait la correspondance entre l'index et la ligne de donnée.
    szt ligne(const QModelIndex &index) const override
        {return m_tree.get_data(index);}

    //! Nombre de ligne afficher en szt.
    szt nbr_row() const override
        {return static_cast<szt>(m_tree.tree_data().size());}

    //! Met le vecteur des correspondance de ligne à l'identitée.
    void reset_row_to_ligne() override
        {m_tree.set_tree(m_full_tree);}
};
}}
#endif // TREE_COLONNES_MODEL_H
