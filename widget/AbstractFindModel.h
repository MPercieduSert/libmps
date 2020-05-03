/*Auteur: PERCIE DU SERT Maxime
 *Date: 26/04/2020
 */
#ifndef ABSTRACTFINDMODEL_H
#define ABSTRACTFINDMODEL_H

#include <memory>
#include <QAbstractItemModel>
#include <QComboBox>
#include <QDate>
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include "AbstractTreeModel.h"

namespace modelMPS {
using namespace typeMPS;

//! Type des noeuds de l'arbre.
enum colonneType {BooleenColonne,
               ConstanteColonne,
               DateColonne,
               DateTimeColonne,
               DoubleColonne,
               EntierColonne,
               TexteColonne,
               NbrTypeColonne};

//    //! Node abstré des Opération et conditions.
//    class AbstractOpCondNode : public AbstractNode {
//    public :
//        using AbstractNode::AbstractNode;
//        bool negation;              //!< négation.
//    };

//    //! Structure des opérations.
//    class OperationNode : public AbstractOpCondNode {
//    public:
//        //static const int NbrColum = OperationNbrCol;
//        nodeOperation value;        //!< Valeur.

//        //! Constructeur.
//        OperationNode() : AbstractOpCondNode(Operation) {}
//    };

//    //! Structure d'entête des coditions.
//    class ConditionNode : public AbstractOpCondNode {
//    public:
//        //static const int NbrColum = OperationNode::NbrColum + 1;
//        szt value;     //!< Indice de la colonne.
//        bool noColumn = true;
//        //! Constructeur.
//        ConditionNode() : AbstractOpCondNode(Condition) {}
//    };

//    //! Structure de condition sur des booleén.
//    class BoolNode : public AbstractNode {
//    public:
//        //static const int NbrColum = OperationNode::NbrColum + 1;
//        bool value;     //!< Valeur.
//    };

//    //! Structure de condition sur des dates.
//    class DateNode : public AbstractNode {
//    public:
//        //static const int NbrColum = OperationNode::NbrColum + 2;
//        QDate value;        //!< Valeur.
//        comparaison comp;   //!< Comparaison.
//    };

//    //! Structure de condition sur des datetimes.
//    struct DateTimeNode : public AbstractNode {
//    public:
//        //static const int NbrColum = OperationNode::NbrColum + 2;
//        QDateTime value;        //!< Valeur.
//        comparaison comp;   //!< Comparaison.
//    };

//    //! Structure de condition sur des doubles.
//    struct DoubleNode : public AbstractNode {
//    public:
//        //static const int NbrColum = OperationNode::NbrColum + 2;
//        double value;        //!< Valeur.
//        comparaison comp;   //!< Comparaison.
//    };

//    //! Structure de condition sur des entiers.
//    struct IntItem {
//        int value;        //!< Valeur.
//        comparaison comp;   //!< Comparaison.
//    };

//    //! Structure de condition sur des textes.
//    struct TexteItem {
//        QString value;        //! Valeur.
//        //Comparaison comp;   //! Comparaison.
//    };

/*! \ingroup groupeModel
 * \brief Classe mère des model de recherche.
 */
class AbstractFindModel : public AbstractTreeModel {
    Q_OBJECT
public:
    //! Type des noeuds de l'arbre.
    enum nodeType {NoType,
                   Operation,
                   Choice,
                   Booleen,
                   Constante,
                   Condition,
                   Date,
                   DateTime,
                   Double,
                   Entier,
                   Texte,
                   NbrType};

    //! Indice des colonnes.
    enum column {NegColumn,
                 OpColumn,
                 TypeColumn,
                 NbrColumn};



    //! Nombre de colunm par type de noeuds.
    enum nbrColumnByNode {OperationNbrCol = 3,
                          BooleenNbrCol = 1,
                          ConstanteNbrCol = 1,
                          ConditionNbrCol = OperationNbrCol,
                          DateNbrCol = 2,
                          DateTimeNbrCol = 2,
                          DoubleNbrCol = 2,
                          EntierNbrCol = 2,
                          TexteNbrCol = 1};

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

    //! Noeud abstré de l'arbre de recherche.
    class AbstractNode {
    protected:
        using Afm = AbstractFindModel;
    public:
        //! Constructeur.
        AbstractNode() = default;

        virtual ~AbstractNode();

        //! Accesseur de la donnée associé à column.
        virtual QVariant data(int /*column*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

        //! Accesseur des drapeaux associés à column.
        virtual Qt::ItemFlags flags(int /*column*/) const {return Qt::ItemIsSelectable;}

        //! Mutateur de la donnée associé à column.
        virtual bool setData(int /*column*/, const QVariant & /*value*/, int /*role*/ = Qt::EditRole) {return true;}

        //! Accesseur du type du noeud.
        virtual int type() const = 0;
    };

    //! Structure d'informations sur une colonne du model.
    struct Colonne {
        int type = NoType;      //!< Type de la colonne.
        QString nom;      //!< Nom de la colonne.
    };
    //! Classe des noeud de l'arbre de données
    using Node = std::unique_ptr<AbstractNode>;
    //! Classe structurant les données.
    using Tree = conteneurMPS::tree<Node>;
    //! Classe du protomodel.
    using TreeModel = TreeForModel<Node>;
    friend ReadableTreeForModel<Node>;
protected:
    TreeModel m_tree;                       //!< Arbre de données pour le model.
    //std::array<int,NbrType> m_nbrColumn;    //!< Nombre de colonnes.
    std::vector<Colonne> m_colonnes;        //!< Informations sur les colonnes.
    std::vector<QString> m_header;          //!< Entête des colonnes.

    using QAbstractItemModel::createIndex;
public:
    static const std::array<QString,NbrOperation> OperationStrings;        //!< Labels des opération.
    TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(m_tree)

    //! Constructeur.
    AbstractFindModel(QObject * parent = nullptr);

    //! Renvoie le nombre de column pour un parent donné.
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
        {return NbrColumn;}

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Renvoie les drapeaux associé à un index.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const AbstractNode & getData(const QModelIndex &index) const
        {return **m_tree.getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    AbstractNode & getData(const QModelIndex &index)
        {return **m_tree.getIter(index);}

    //! Accesseur des entêtes des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole
                && 0 <= section  && section < static_cast<int>(m_header.size()))
            return m_header[static_cast<szt>(section)];
        return QVariant();
    }

    //! Fabrique des noeuds.
    Node nodeFactory(int type);

//    //! Accesseurs des labels des opérations.
//    const QString & operationString(szt pos) const
//        {return m_operationStrings.at(pos);}

    //! Mutateur la donnée associé à un couple (index,role).
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

//////////////////////////////////// Noeud ///////////////////

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche avec négation.
 */
class AbstractNegationNode : public AbstractFindModel::AbstractNode {
protected:
    bool m_negation;        //!< Négation.
public:
    //! Constructeur.
    AbstractNegationNode() = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == Afm::NegColumn)
            return AbstractNode::flags(column) | Qt::ItemIsEnabled;
        return AbstractNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override {
        if(column == Afm::NegColumn && role == Qt::EditRole) {
            m_negation = value.toBool();
            return true;
        }
        return false;
    }
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de recherche indéterminé.
 */
class ChoiceNode : public AbstractFindModel::AbstractNode {
public:
    //! Constructeur.
    ChoiceNode() = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == Afm::OpColumn)
            return AbstractNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNode::flags(column);
    }

    //! Accesseur du type du noeud.
    int type() const override
        {return Afm::Choice;}
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de type opération.
 */
class OperationNode : public AbstractNegationNode {
protected:
    szt m_operation;        //!< Identifiant de l'opération
public:
    //! Constructeur.
    OperationNode(szt op = Afm::Et) : m_operation(op) {}

    //! Destructeur.
    ~OperationNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == Afm::OpColumn)
            return AbstractNegationNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override {
        if(column == Afm::OpColumn && role == Qt::EditRole) {
            m_operation = value.toUInt();
            return true;
        }
        return AbstractNegationNode::setData(column,value,role);
    }

    //! Accesseur du type du noeud.
    int type() const override
        {return Afm::Operation;}
};
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
