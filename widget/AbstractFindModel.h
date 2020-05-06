/*Auteur: PERCIE DU SERT Maxime
 *Date: 26/04/2020
 */
#ifndef ABSTRACTFINDMODEL_H
#define ABSTRACTFINDMODEL_H

#include <memory>
#include <QComboBox>
#include <QDate>
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include "TreeNodeModel.h"

namespace modelMPS {
//! Type des noeuds de l'arbre.
enum colonneType {BooleenColonne,
               ConstanteColonne,
               DateColonne,
               DateTimeColonne,
               DoubleColonne,
               EntierColonne,
               TexteColonne,
               NbrTypeColonne};

namespace findModelNode {
//! Type des noeuds de l'arbre.
enum nodeType {NoType = -1,
               OperationNodeType,
               ChoiceNodeType,
               BooleenNodeType,
               ConstanteNodeType,
               ConditionNodeType,
               DateNodeType,
               DateTimeNodeType,
               DoubleNodeType,
               EntierNodeType,
               TexteNodeType,
               NbrType};

//! Indice des colonnes.
enum column {NegColumn,
             OpColumn,
             TypeColumn,
             NbrColumn};

//! Opérations binaire.
enum nodeOperation {Et,
                    Ou,
                    OuExclusif,
                    NbrOperation};

//! Comparaison à une valeur.
enum comparaison {Egal,
                 Different,
                 Inferieure,
                 Superieure,
                 InfEgal,
                 SupEgal};
}

/*! \ingroup groupeModel
 * \brief Classe mère des model de recherche.
 */
class AbstractFindModel : public TreeNodeModel {
    Q_OBJECT
public:
   //! Structure d'informations sur une colonne du model.
    struct Colonne {
        int type = findModelNode::NoType;      //!< Type de la colonne.
        QString nom;      //!< Nom de la colonne.
    };

protected:
    std::vector<Colonne> m_colonnes;        //!< Informations sur les colonnes.
    using QAbstractItemModel::createIndex;
public:
    //! Constructeur.
    AbstractFindModel(QObject * parent = nullptr);

    //! Renvoie le nombre de column pour un parent donné.
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
        {return findModelNode::NbrColumn;}

    //! Mutateur des données du model.
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    //! Fabrique des noeuds.
    Node nodeFactory(int type, int row, const QModelIndex & parent) override;
};

//////////////////////////////////// Noeud ///////////////////

namespace findModelNode {
/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche avec négation.
 */
class AbstractNegationNode : public TreeNodeModel::AbstractNode {
protected:
    bool m_negation;        //!< Négation.
public:
    //! Constructeur.
    using AbstractNode::AbstractNode;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == NegColumn)
            return AbstractNode::flags(column) | Qt::ItemIsEnabled;
        return AbstractNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override {
        if(column == NegColumn && role == Qt::EditRole) {
            m_negation = value.toBool();
            return true;
        }
        return false;
    }
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de recherche indéterminé.
 */
class ChoiceNode : public AbstractNegationNode {
public:
    //! Constructeur.
    ChoiceNode() : AbstractNegationNode(ChoiceNodeType) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn)
            return AbstractNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNode::flags(column);
    }
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de type opération.
 */
class OperationNode : public AbstractNegationNode {
protected:
    szt m_operation;        //!< Identifiant de l'opération
public:
    static const std::array<QString, findModelNode::NbrOperation> OperationStrings;        //!< Labels des opération.

    //! Constructeur.
    OperationNode(szt op = Et) : AbstractNegationNode(OperationNodeType), m_operation(op) {}

    //! Destructeur.
    ~OperationNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn)
            return AbstractNegationNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override {
        if(column == OpColumn && role == Qt::EditRole) {
            m_operation = value.toUInt();
            return true;
        }
        return AbstractNegationNode::setData(column,value,role);
    }
};
}
}
//////////////////////////////////////// FindDelegate /////////////////////////
/*! \defgroup groupeDelegate Delegate
 * \brief Ensemble des classes des delegates.
 */

/*! \ingroup groupeDelegate
 * \brief Espace de noms des delegates.
 */
namespace delegateMPS {
class FindDelegate : public QStyledItemDelegate {
    Q_OBJECT
protected:
    using Afm = modelMPS::AbstractFindModel;
public:
    //! Constructeur.
    FindDelegate(QObject * parent = nullptr)
        : QStyledItemDelegate(parent) {}

    //! Fabrique d'éditeur.
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //! Gestionnaire d'évenement.
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

    //! Hydrate l'éditeur.
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    //! Transmet les données au model.
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};
}
#endif // ABSTRACTFINDMODEL_H
