/*Auteur: PERCIE DU SERT Maxime
 *Date: 26/04/2020
 */
#ifndef ABSTRACTFINDMODEL_H
#define ABSTRACTFINDMODEL_H

#include <memory>
#include <QComboBox>
#include <QDate>
#include <QMouseEvent>
#include <QRegularExpression>
#include <QStyledItemDelegate>
#include "TreeNodeModel.h"

namespace modelMPS {
//! Déclaration anticipée.
class AbstractColonnesModel;
namespace findNodeModel {
//! Type des noeuds de l'arbre.
enum typeNode {NoType = -1,
               BoolNodeType,
               ConstanteNodeType,
               DateNodeType,
               DateTimeNodeType,
               DoubleNodeType,
               IntNodeType,
               TexteNodeType,
               UIntNodeType,
               OperationNodeType,
               ChoiceNodeType,
               ConditionNodeType,
               NbrType};

//! Ensemble de noeuds
enum setNode : unsigned {NoSet = 0,
                        ComparaisonSet = 1};

//! Indice des colonnes.
enum column {NegColumn,
             OpColumn,
             ColonneColumn,
             Condition1,
             Condition2,
             Condition3,
             NbrColumn,
             TexteColumn = Condition1,
             CaseColumn = Condition2,
             RegexColumn = Condition3,
             ComparaisonColumn = Condition1,
             DateColumn = Condition2,
             TrueColumn = Condition1,
             FalseColumn = Condition2
            };

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
                 SupEgal,
                 NbrComparaison};
}

/*! \ingroup groupeModel
 * \brief Classe mère des model de recherche.
 */
class AbstractFindModel : public TreeNodeModel {
    Q_OBJECT
public:
   //! Structure d'informations sur une colonne du model.
    struct Colonne {
        int type = findNodeModel::NoType;      //!< Type de la colonne.
        QString nom;      //!< Nom de la colonne.
    };

protected:
    std::vector<Colonne> m_colonnes;        //!< Informations sur les colonnes.
    AbstractColonnesModel * m_model = nullptr;        //!< Model filtré.
    using QAbstractItemModel::createIndex;
public:
    //! Constructeur.
    AbstractFindModel(QObject * parent = nullptr);

    //! Renvoie le nombre de column pour un parent donné.
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
        {return findNodeModel::NbrColumn;}

    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operationNode un noeud choiceNode est ajouté avant row.
    //! Si le noeuds parent n'est pas un operationNode, un nouveau operationNode prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud ChoiceNode est ajouté en cadet.
    void insertNode(int row, const QModelIndex & parent);

    //! Donne la liste des noms des colonnes du model associé.
    std::vector<QString> nomColonnes() const;

    //! Supprime le noeud de la ligne row de parent.
    void removeNode(int row, const QModelIndex & parent);

    //! Teste si l'arbre est réduit à sa racine.
    bool rootLeaf() const
        {return m_data.tree().cbegin().toFirstChild().leaf();}

    //! Mutateur des données du model.
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //! Mutateur du model filtré.
    void setModel(AbstractColonnesModel * model);

    //! Teste si la ligne d'indice id vérifie la condition de la racine.
    bool testRoot(szt id) const;

    //! Teste si la ligne d'indice id vérifie l'arbre des conditions.
    bool testTree(szt id) const;

public slots:
    //! Applique la recherche au model à filtré.
    void find();

protected:
    //! Fabrique des noeuds.
    Node nodeFactory(int type, int row, const QModelIndex & parent) override;

    //! Fabrique des noeuds de condition.
    Node nodeConditionFactory(szt pos);
};

//////////////////////////////////// Noeud ///////////////////

namespace findNodeModel {
/*! \ingroup groupeModel
 * \brief Classe mère des feuilles de recherche (héritage multiple).
 */
class AbstractFindNode : public TreeNodeModel::AbstractNode {
public:
    //! Constructeur.
    using AbstractNode::AbstractNode;

    //! Destructeur.
    virtual ~AbstractFindNode();

    //! Test si le noeud n'intervient pas dans la recherche.
    virtual bool empty() const
        {return false;}
};


/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche avec négation.
 */
class AbstractNegationNode : public AbstractFindNode {
protected:
    bool m_negation;        //!< Négation.
public:
    //! Constructeur.
    using AbstractFindNode::AbstractFindNode;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == NegColumn)
            return Qt::ItemIsEnabled;
        if(column == OpColumn)
            return Qt::ItemIsSelectable;
        return AbstractNode::flags(column);
    }

    //! Accesseur de la négation.
    bool negation() const
        {return m_negation;}

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche définissant une condition.
 */
class AbstractConditionNode : public AbstractNegationNode {
protected:
    szt m_pos = 0;            //!< Position de la colonne dans le model filtré.
    QString m_label;    //!< Label de la condition.
public:
    //! Constructeur.
    AbstractConditionNode() = default;

    //!Constructeur.
    AbstractConditionNode(szt pos, const QString & label, int type = NoType)
        : AbstractNegationNode(type), m_pos(pos), m_label(label) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn)
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if(column == ColonneColumn)
            return AbstractNegationNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNode::flags(column);
    }

    //! Accesseur du label.
    const QString & label() const
        {return m_label;}

    //! Accesseur de position.
    szt pos() const
        {return m_pos;}

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Mutateur du label.
    void setLabel(const QString & label)
        {m_label = label;}

    //! Mutateur de position.
    void setPos(szt pos)
        {m_pos = pos;}

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test(szt id, AbstractColonnesModel * model) const;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    virtual bool testValue(const QVariant & value) const = 0;
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche définissant une condition de comparaison.
 */
class AbstractComparaisonNode : public AbstractConditionNode {
protected:
    szt m_comp;            //!< Indice de la comparaison.
public:
    static const std::array<QString, NbrComparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    AbstractComparaisonNode(szt pos, const QString & label, szt comp = Egal,int type = NoType)
        : AbstractConditionNode(pos,label,type), m_comp(comp) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == ComparaisonColumn)
            return AbstractConditionNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractConditionNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les textes.
 */
class BoolNode : public AbstractConditionNode {
protected:
    QString m_trueLabel;    //!< Label du booléen vraie.
    QString m_falseLabel;   //!< Label du booléen faux.
    bool m_true;            //!< Filtre contenant les vraies.
    bool m_false;            //!< Filtre contenant les faux.
public:
    //! Constructeur.
    BoolNode(szt pos, const QString & label,
             const QString & trueLabel = QString(), const QString & falseLabel = QString(),
             bool trueChecked = true, bool falseChecked = true)
        : AbstractConditionNode(pos,label,BoolNodeType),
          m_trueLabel(trueLabel), m_falseLabel(falseLabel),
          m_true(trueChecked), m_false(falseChecked){}

    //! Destructeur.
    ~BoolNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_true && m_false;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == TrueColumn || column == FalseColumn)
            return AbstractConditionNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return AbstractConditionNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override
        {return value.toBool() ? m_true : m_false;}
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de recherche indéterminé.
 */
class ChoiceNode : public AbstractFindNode {
public:
    //! Constructeur.
    ChoiceNode() : AbstractFindNode(ChoiceNodeType) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return true;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn || column == ColonneColumn)
            return Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return Qt::NoItemFlags;
    }
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les dates.
 */
class DateNode : public AbstractComparaisonNode {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    DateNode(szt pos, const QString & label,const QDate & date = QDate(), szt comp = Egal)
        : AbstractComparaisonNode(pos,label,comp,DateNodeType), m_date(date) {}

    //! Destructeur.
    ~DateNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return !m_date.isValid();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == DateColumn)
            return AbstractComparaisonNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractComparaisonNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de type opération.
 */
class OperationNode : public AbstractNegationNode {
protected:
    szt m_operation;        //!< Identifiant de l'opération
public:
    static const std::array<QString, NbrOperation> Strings;        //!< Labels des opération.
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

    //! Accesseur de l'opération.
    szt operation() const
        {return m_operation;}

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les textes.
 */
class TexteNode : public AbstractConditionNode {
protected:
    QString m_texte;         //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;            //!< Recherche sensible à la case.
    bool m_regex;           //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    TexteNode(szt pos, const QString & label,const QString & texte = QString(), bool c = false,bool regex = false)
        : AbstractConditionNode(pos,label,TexteNodeType), m_texte(texte), m_case(c), m_regex(regex) {
        if(m_regex){
            m_regular.setPattern(m_texte);
            if(!m_case)
                m_regular.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
    }

    //! Destructeur.
    ~TexteNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == TexteColumn)
            return AbstractConditionNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        if(column == CaseColumn || column == RegexColumn)
            return AbstractConditionNode::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return AbstractConditionNode::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};
}// end namespace findNodeModel
}// end namespace modelMPS
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
