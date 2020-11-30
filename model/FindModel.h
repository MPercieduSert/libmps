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

    //! Position des sous-noeud.
    enum positionNode{ZeroPosition,
                      UnPosition,
                      DeuxPosition,
                      TroisPosition,
                      QuatrePosition,
                      CinqPosition,
                      NegationPosition = ZeroPosition,
                      ColonnePosition = UnPosition,
                      ComparaisonPosition = DeuxPosition,
                      OperationChoicePosition = ZeroPosition,
                      OperationOperationPosition = UnPosition,
                      TruePosition = DeuxPosition,
                      FalsePosition = TroisPosition,
                      DatePosition = TroisPosition,
                      TextePosition = DeuxPosition,
                      CasePosition = TroisPosition,
                      RegexPosition = QuatrePosition
                     };
    //! Nombre de sous-noueds.
    enum dataCountNode {
        BoolDataCount = 4,
        DateDataCount = 4,
        ChoiceDataCount = 2,
        OperationDatacount = 2,
        TexteDataCount = 5
    };

protected:
    std::vector<Colonne> m_colonnes;                //!< Informations sur les colonnes.
    AbstractColonnesModel * m_model;                //!< Model filtré.
    using AbstractNodeModel::createIndex;
public:
    //! Constructeur.
    FindModel(AbstractColonnesModel * model = nullptr, QObject * parent = nullptr);

    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operationNode un noeud choiceNode est ajouté avant row.
    //! Si le noeuds parent n'est pas un operationNode, un nouveau operationNode prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud ChoiceNode est ajouté en cadet.
    void insertNode(const NodeIndex & parent, szt pos);

    //! Accesseur du model filtré.
    AbstractColonnesModel * model() const
        {return m_model;}

    //! Donne la liste des noms des colonnes du model associé.
    QMap<QString,QVariant> nomColonnes() const;

    //! Supprime le noeud et ses descendants.
    void removeNode(const NodeIndex & index);

    //! Teste si l'arbre est réduit à sa racine.
    bool rootLeaf() const override
        {return m_data.tree().cbegin().toFirstChild().leaf();}

    //! Mutateur des données du model.
    bool setData(const NodeIndex &index, const QVariant &value, int role = DataRole) override;

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
    Node nodeFactory(const NodeIndex & parent, szt pos, int type) override;

    //! Fabrique des noeuds de condition.
    Node nodeConditionFactory(szt pos);
};

//////////////////////////////////// Noeud ///////////////////

namespace findNodeModel {
/*! \ingroup groupeModel
 * \brief Classe mère des neuds de recherche.
 */
class FindNode : public TreeNodeModel::AbstractNode {
protected:
    bool m_negation = false;        //!< Négation.
    FindModel * m_model;    //!< Pointeur sur le model.
    szt m_pos;              //!< Position de la colonne dans le model filtré.
public:
    enum {Vide = -1};
    //! Constructeur.
    FindNode(FindModel * model, szt pos, int type = NoType)
        : AbstractNode(type), m_model(model), m_pos(pos) {}

    //! Destructeur.
    ~FindNode() override;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role = DataRole, szt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    szt dataCount(int cible) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::NegCible)
            return Qt::ItemIsEnabled;
        return AbstractNode::flags(cible,num);
    }

    //! Test si le noeud n'intervient pas dans la recherche.
    virtual bool empty() const
        {return true;}

    //! Accesseur de la négation.
    bool negation() const
        {return m_negation;}

    //! Accesseur de position.
    szt pos() const
        {return m_pos;}

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

    //! Mutateur de position.
    void setPos(szt pos)
        {m_pos = pos;}

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test(szt id, AbstractColonnesModel * model) const
        {return testValue(model->colonne(m_pos).dataTest(id));}

    //! Teste une valeur.
    virtual bool testValue(const QVariant & /*value*/) const {return true;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche définissant une condition de comparaison.
 */
class ComparaisonNode : public FindNode {
protected:
    szt m_comp;            //!< Indice de la comparaison.
public:
    static const std::array<QString, FindModel::NbrComparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    ComparaisonNode(FindModel * model, szt pos, szt comp = FindModel::Egal,int type = NoType)
        : FindNode(model,pos,type), m_comp(comp) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::ComparaisonCible)
            return FindNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return FindNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur des booléen.
 */
class BoolNode : public FindNode {
protected:
    bool m_false;           //!< Filtre contenant les faux.
    bool m_true;            //!< Filtre contenant les vraies.
    QString m_falseLabel;   //!< Label du filtre faux.
    QString m_trueLabel;    //!< Label du filtre faux.
public:
    //! Constructeur.
    BoolNode(FindModel * model, szt pos, const QString & falseLabel = QString(), const QString trueLabel = QString(),
             bool trueChecked = true, bool falseChecked = true)
        : FindNode(model,pos,FindModel::BoolNodeType),
          m_false(falseChecked), m_true(trueChecked),
          m_falseLabel(falseLabel), m_trueLabel(trueLabel)  {}

    //! Destructeur.
    ~BoolNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    szt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_true && m_false;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::TrueCible || cible == FindModel::FalseCible)
            return FindNode::flags(cible,num) | Qt::ItemIsEnabled;
        return FindNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override
        {return value.toBool() ? m_true : m_false;}
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les dates.
 */
class DateNode : public ComparaisonNode {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    DateNode(FindModel * model,szt pos,const QDate & date = QDate(), szt comp = FindModel::Egal)
        : ComparaisonNode(model,pos,comp,FindModel::DateNodeType), m_date(date) {}

    //! Destructeur.
    ~DateNode() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role = DataRole, szt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    szt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return !m_date.isValid();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::DateCible)
            return ComparaisonNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return ComparaisonNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les textes.
 */
class TexteNode : public FindNode {
protected:
    QString m_texte;                //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;                    //!< Recherche sensible à la case.
    bool m_regex;                   //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    TexteNode(FindModel * model,szt pos,const QString & texte = QString(), bool c = false,bool regex = false)
        : FindNode(model,pos,FindModel::TexteNodeType), m_texte(texte), m_case(c), m_regex(regex) {
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

    //! Nombre de donnée associé au noeud pour une cible donnée.
    szt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int cible, szt num = 0) const override {
        if(cible == FindModel::TexteCible)
            return FindNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        if(cible == FindModel::CaseCible || cible == FindModel::RegexCible)
            return FindNode::flags(cible,num) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return FindNode::flags(cible,num);
    }

    //! Mutateur de la donnée associé à column.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};
}// end namespace findNodeModel
}// end namespace modelMPS
#endif // FINDMODEL_H
