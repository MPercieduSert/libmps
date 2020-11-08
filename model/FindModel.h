/*Auteur: PERCIE DU SERT Maxime
 *Date: 31/10/2020
 */
#ifndef FINDMODEL_H
#define FINDMODEL_H

#include <memory>
#include <QDate>
#include <QRegularExpression>
#include "AbstractNodeModel.h"
#include "AbstractColonnesModel.h"
#include "ColonnesForModel.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Classe mère des model de recherche.
 */
class FindModel : public TreeNodeModel, public AbstractFindModel {
    Q_OBJECT
public:
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

    //! Cible des données.
    enum dataCible {ComparaisonCible,
                 CaseCible,
                 ColonneCible,
                 DateCible,
                 FalseCible,
                 NegCible,
                 OpCible,
                 RegexCible,
                 TexteCible,
                 TrueCible
                };

    //! Ensemble de noeuds
    enum setNode : unsigned {NoSet = 0,
                            ComparaisonSet = 1};

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
   //! Structure d'informations sur une colonne du model.
    struct Colonne {
        int type = NoType;      //!< Type de la colonne.
        QString nom;      //!< Nom de la colonne.
    };

protected:
    std::vector<Colonne> m_colonnes;                //!< Informations sur les colonnes.
    AbstractColonnesModel * m_model;      //!< Model filtré.
    using AbstractNodeModel::createIndex;
public:
    //! Constructeur.
    FindModel(AbstractColonnesModel * model = nullptr, QObject * parent = nullptr);

    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operationNode un noeud choiceNode est ajouté avant row.
    //! Si le noeuds parent n'est pas un operationNode, un nouveau operationNode prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud ChoiceNode est ajouté en cadet.
    void insertNode(int row, const NodeIndex & parent);

    //! Accesseur du model filtré.
    AbstractColonnesModel * model() const
        {return m_model;}

    //! Donne la liste des noms des colonnes du model associé.
    std::vector<QString> nomColonnes() const;

    //! Supprime le noeud de la ligne row de parent.
    void removeNode(int row, const NodeIndex & parent);

    //! Teste si l'arbre est réduit à sa racine.
    bool rootLeaf() const override
        {return m_data.tree().cbegin().toFirstChild().leaf();}

    //! Mutateur des données du model.
    bool setData(const NodeIndex &index, int cible, const QVariant &value, int role = DataRole, szt num = 0) override;

    //! Mutateur du model filtré.
    void setModel(AbstractColonnesModel * model);

    //! Teste si la ligne d'indice id vérifie la condition de la racine.
    bool testRoot(szt id) const override;

    //! Teste si la ligne d'indice id vérifie l'arbre des conditions.
    bool testTree(szt id) const override;

public slots:
    //! Applique la recherche au model à filtré.
    void find();

    //! Réinitialise le model de recherche.
    void reset();

protected:
    //! Fabrique des noeuds.
    Node nodeFactory(int type, int row, const NodeIndex & parent) override;

    //! Fabrique des noeuds de condition.
    Node nodeConditionFactory(szt pos);
};

//////////////////////////////////// Noeud ///////////////////

namespace findNodeModel {
/*! \ingroup groupeModel
 * \brief Classe mère des neuds de recherche.
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
    QVariant data(int cible, int role = DataRole, szt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::NegCible)
            return Qt::ItemIsEnabled;
        return AbstractNode::flags(cible,num);
    }

    //! Accesseur de la négation.
    bool negation() const
        {return m_negation;}

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche définissant une condition.
 */
class AbstractConditionNode : public AbstractNegationNode {
protected:
    szt m_pos = 0;            //!< Position de la colonne dans le model filtré.
public:
    //! Constructeur.
    AbstractConditionNode() = default;

    //!Constructeur.
    AbstractConditionNode(szt pos, int type = NoType)
        : AbstractNegationNode(type), m_pos(pos) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::ColonneCible)
            return AbstractNegationNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNode::flags(cible,num);
    }

    //! Accesseur de position.
    szt pos() const
        {return m_pos;}

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

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
    static const std::array<QString, FindModel::NbrComparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    AbstractComparaisonNode(szt pos, szt comp = FindModel::Egal,int type = NoType)
        : AbstractConditionNode(pos,type), m_comp(comp) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::ComparaisonCible)
            return AbstractConditionNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractConditionNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur des booléen.
 */
class BoolNode : public AbstractConditionNode {
protected:
    bool m_false;           //!< Filtre contenant les faux.
    bool m_true;            //!< Filtre contenant les vraies.
    QString m_falseLabel;   //!< Label du filtre faux.
    QString m_trueLabel;    //!< Label du filtre faux.
public:
    //! Constructeur.
    BoolNode(szt pos, const QString & falseLabel = QString(), const QString trueLabel = QString(),
             bool trueChecked = true, bool falseChecked = true)
        : AbstractConditionNode(pos,FindModel::BoolNodeType),
          m_false(falseChecked), m_true(trueChecked),
          m_falseLabel(falseLabel), m_trueLabel(trueLabel)  {}

    //! Destructeur.
    ~BoolNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_true && m_false;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::TrueCible || cible == FindModel::FalseCible)
            return AbstractConditionNode::flags(cible,num) | Qt::ItemIsEnabled;
        return AbstractConditionNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

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
    ChoiceNode() : AbstractFindNode(FindModel::ChoiceNodeType) {}

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return true;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt /*num*/ = 0) const override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les dates.
 */
class DateNode : public AbstractComparaisonNode {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    DateNode(szt pos,const QDate & date = QDate(), szt comp = FindModel::Egal)
        : AbstractComparaisonNode(pos,comp,FindModel::DateNodeType), m_date(date) {}

    //! Destructeur.
    ~DateNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return !m_date.isValid();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::DateCible)
            return AbstractComparaisonNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractComparaisonNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

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
    static const std::array<QString, FindModel::NbrOperation> Strings;        //!< Labels des opération.
    //! Constructeur.
    OperationNode(szt op = FindModel::Et) : AbstractNegationNode(FindModel::OperationNodeType), m_operation(op) {}

    //! Destructeur.
    ~OperationNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::OpCible)
            return AbstractNegationNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNode::flags(cible,num);
    }

    //! Accesseur de l'opération.
    szt operation() const
        {return m_operation;}

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les textes.
 */
class TexteNode : public AbstractConditionNode {
protected:
    QString m_texte;                //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;                    //!< Recherche sensible à la case.
    bool m_regex;                   //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    TexteNode(szt pos,const QString & texte = QString(), bool c = false,bool regex = false)
        : AbstractConditionNode(pos,FindModel::TexteNodeType), m_texte(texte), m_case(c), m_regex(regex) {
        if(m_regex){
            m_regular.setPattern(m_texte);
            if(!m_case)
                m_regular.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
    }

    //! Destructeur.
    ~TexteNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::TexteCible)
            return AbstractConditionNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        if(cible == FindModel::CaseCible || cible == FindModel::RegexCible)
            return AbstractConditionNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return AbstractConditionNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};
}// end namespace findNodeModel
}// end namespace modelMPS
#endif // FINDMODEL_H
