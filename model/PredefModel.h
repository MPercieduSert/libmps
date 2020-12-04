/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef PREDEFMODEL_H
#define PREDEFMODEL_H

#include "AbstractNodeModel.h"
#include "BddPredef.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Model de gestion des types.
 */
class AbstractPermissionModel : public TreeNodeModel {
    Q_OBJECT
protected:
    const szt m_offset;             //!< Offset de postion des cibles.
    bddMPS::BddPredef & m_bdd;      //!< Référence à la base de donnée
    std::vector<int> m_cibleVec;    //!< Vecteurs de cibles à afficher.
public:
    enum {TypeNode = 1,
         NcNomOffset = 2};
    //! Cible des données du model.
    enum dataCible {NcCible,
                    NomCible,
                    RefCible,
                    PermissionCible,
                    CibleCible};
    //! Position des sous-noeud.
    enum positionNode{ZeroPosition,
                      UnPosition,
                      NomPosition = ZeroPosition,
                      NcPosition = UnPosition};
    //! Constructeur.
    AbstractPermissionModel(bddMPS::BddPredef & bdd, szt offset = NcNomOffset, QObject * parent = nullptr);

    //! Nombre de donné associé à une cible.
    szt dataCount(const NodeIndex & index) const override;

    //! Accesseur des cibles.
    int cible(szt num) const
        {return m_cibleVec.at(num);}

    //! Accesseur de l'offset de postion des cibles
    szt offset() const noexcept
        {return m_offset;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des neuds de recherche.
 */
template<class Ent> class PermissionNode : public TreeNodeModel::AbstractNode {
protected:
    Ent m_ent;                   //!< Type associé au noeud.
    std::map<int,flag> m_permissionMap;     //!< Map des permission du type.
    AbstractPermissionModel * m_model;      //!< Pointeur sur le model.
public:
    //! Constructeur.
    PermissionNode(AbstractPermissionModel * model)
        : AbstractNode(AbstractPermissionModel::TypeNode), m_model(model) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role = DataRole, szt num = 0) const override;

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};
}

#endif // PREDEFMODEL_H
