/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/05/2019
 */
#ifndef TREECOLONNESMODEL_H
#define TREECOLONNESMODEL_H

#include "AbstractColonnesModel.h"
#include "tree.h"
#include "type_mps.h"

namespace modelMPS {
using namespace type_mps;
/*! \ingroup groupeModel
 *\brief Classe mère des model de type arbre à colonnes homogènes.
 */
class TreeColonnesModel : public AbstractColonnesModel {
    Q_OBJECT
public:
    template<class T> using tree = conteneurMPS::tree<T>;
    using arbre = TreeForModel<szt>;
protected:
    arbre m_tree;           //! arbre de correspondance des lignes visibles.
    tree<szt> m_fullTree;   //! arbre de correspondance de toutes les lignes.
public:
    TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(m_tree)    // Implémentation des méthodes virtuelles index, parent, rowCount.

    //! Constructeur.
    TreeColonnesModel(bool racine, QObject *parent);

    //! Destructeur.
    ~TreeColonnesModel() override = default;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    void find(AbstractFindModel *findModel) override;

    //! Insert count lignes dans le model juste avant la ligne row.
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Supprime count lignes en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

protected:
    //! Fait la correspondance entre l'index et la ligne de donnée.
    szt ligne(const QModelIndex &index) const override
        {return m_tree.getData(index);}

    //! Nombre de ligne afficher en szt.
    szt nbrRow() const override
        {return static_cast<szt>(m_tree.tree().size());}

    //! Met le vecteur des correspondance de ligne à l'identitée.
    void resetRowToLigne() override
        {m_tree.setTree(m_fullTree);}
};
}
#endif // TREECOLONNESMODEL_H
