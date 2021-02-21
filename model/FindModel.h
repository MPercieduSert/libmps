/*Auteur: PERCIE DU SERT Maxime
 *Date: 31/10/2020
 */
#ifndef FINDMODEL_H
#define FINDMODEL_H

#include <memory>
#include <QDate>
#include <QRegularExpression>
#include "ItemNodeModel.h"
#include "AbstractColonnesModel.h"
#include "ColonnesForModel.h"

namespace modelMPS {
/*! \ingroup groupeModel
 *\brief Classe mère des model de recherche.
 */
class FindModel : public ItemNodeModel, public AbstractFindModel {
    Q_OBJECT
public:
    //! Type des noeuds de l'arbre.
    enum typeNode {NoType = -1,
                   BoolNodeType,
                   ConstanteNodeType,
                   DateNodeType,
                   Date_TimeNodeType,
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
                 texte_cible,
                 TrueCible};

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

    //! position des sous-noeud.
    enum positionNode{Zeroposition,
                      Unposition,
                      Deuxposition,
                      Troisposition,
                      Quatreposition,
                      Cinqposition,
                      Negationposition = Zeroposition,
                      Colonneposition = Unposition,
                      Comparaisonposition = Deuxposition,
                      OperationChoiceposition = Zeroposition,
                      OperationOperationposition = Unposition,
                      Trueposition = Deuxposition,
                      Falseposition = Troisposition,
                      Dateposition = Troisposition,
                      Texteposition = Deuxposition,
                      Caseposition = Troisposition,
                      Regexposition = Quatreposition};

    //! Nombre de sous-noueds.
    enum dataCountNode {
        BoolDataCount = 4,
        DateDataCount = 4,
        ChoiceDataCount = 1,
        OperationDatacount = 2,
        TexteDataCount = 5};

protected:
    std::vector<Colonne> m_colonnes;                //!< Informations sur les colonnes.
    AbstractColonnesModel *m_model;                //!< Model filtré.
    using ItemNodeModel::createIndex;
public:
    //! Constructeur.
    FindModel(AbstractColonnesModel *model = nullptr, QObject *parent = nullptr);

    //! Accesseur du model filtré.
    AbstractColonnesModel *model() const
        {return m_model;}

    //! Donne la liste des noms des colonnes du model associé.
    QMap<QString,QVariant> nomColonnes() const;

    //! Teste si l'arbre est réduit à sa racine.
    bool root_leaf() const override
        {return m_data.cbegin().leaf();}

    //! Mutateur du model filtré.
    void setModel(AbstractColonnesModel *model);

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
    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operationNode un noeud choiceNode est ajouté avant pos.
    //! Si le noeuds parent n'est pas un operationNode, un nouveau operationNode prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud ChoiceNode est ajouté en cadet.
    //! Insert count noeuds de nature type avant la position pos de parent.
    std::list<IterNode> insert(const NodeIndex &parent, numt pos, numt count, int type = DefaultType) override;

    //! Fabrique des noeuds.
    Node nodeFactory(const NodeIndex &parent, post pos, int type) override;

    //! Fabrique des noeuds de condition pour une colonne.
    Node nodeConditionFactory(szt colonne);

    //! Supprime le noeud et ses descendants.
    bool remove(const NodeIndex &index, numt count = 1) override;

    //! Mutateur des données du model.
    bool set(const NodeIndex &index, const QVariant &value, int role) override;
};

//////////////////////////////////// Noeud ///////////////////

namespace findNodeModel {
/*! \ingroup groupeModel
 *\brief Classe mère des neuds de recherche.
 */
class FindNode : public ItemNode {
protected:
    bool m_negation = false;        //!< Négation.
    FindModel *m_model;    //!< Pointeur sur le model.
    numt m_pos;              //!< position de la colonne dans le model filtré.
public:
    enum {Vide = -1,
          NoColonne = 0};
    //! Constructeur.
    FindNode(FindModel *model, numt pos, int type = NoType)
        : ItemNode(type), m_model(model), m_pos(pos) {}

    NODE_COPIE(FindNode)

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    post dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    virtual bool empty() const
        {return true;}

    //! Accesseur de la négation.
    bool negation() const
        {return m_negation;}

    //! Accesseur de position.
    post pos() const
        {return m_pos;}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Mutateur de position.
    void setPos(numt pos)
        {m_pos = pos;}

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test(szt id, AbstractColonnesModel *model) const
        {return testValue(model->colonne(m_pos).dataTest(id));}

    //! Teste une valeur.
    virtual bool testValue(const QVariant &/*value*/) const {return true;}
};

/*! \ingroup groupeModel
 *\brief Classe mère des noeuds de recherche définissant une condition de comparaison.
 */
class ComparaisonNode : public FindNode {
protected:
    numt m_comp;            //!< Indice de la comparaison.
public:
    static const std::array<QString, FindModel::NbrComparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    ComparaisonNode(FindModel *model, numt pos, numt comp = FindModel::Egal,int type = NoType)
        : FindNode(model,pos,type), m_comp(comp) {}

    NODE_COPIE(ComparaisonNode)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;
};

/*! \ingroup groupeModel
 *\brief Classe des noeuds de filtrage sur des booléen.
 */
class BoolNode : public FindNode {
protected:
    bool m_false;           //!< Filtre contenant les faux.
    bool m_true;            //!< Filtre contenant les vraies.
    QString m_falseLabel;   //!< Label du filtre faux.
    QString m_trueLabel;    //!< Label du filtre faux.
public:
    //! Constructeur.
    BoolNode(FindModel *model, numt pos, const QString &falseLabel = QString(), const QString trueLabel = QString(),
             bool trueChecked = true, bool falseChecked = true)
        : FindNode(model,pos,FindModel::BoolNodeType),
          m_false(falseChecked), m_true(trueChecked),
          m_falseLabel(falseLabel), m_trueLabel(trueLabel)  {}

    //! Destructeur.
    ~BoolNode() override = default;

    NODE_COPIE(BoolNode)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    numt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_true &&m_false;}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant &value) const override
        {return value.toBool() ? m_true : m_false;}
};

/*! \ingroup groupeModel
 *\brief Classe des noeuds de filtrage sur les dates.
 */
class DateNode : public ComparaisonNode {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    DateNode(FindModel *model, numt pos,const QDate &date = QDate(), numt comp = FindModel::Egal)
        : ComparaisonNode(model,pos,comp,FindModel::DateNodeType), m_date(date) {}

    //! Destructeur.
    ~DateNode() override = default;

    NODE_COPIE(DateNode)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    numt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return !m_date.is_valid();}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant &value) const override;
};

/*! \ingroup groupeModel
 *\brief Classe des noeuds de filtrage sur les textes.
 */
class TexteNode : public FindNode {
protected:
    QString m_texte;                //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;                    //!< Recherche sensible à la case.
    bool m_regex;                   //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    TexteNode(FindModel *model, numt pos,const QString &texte = QString(), bool c = false,bool regex = false)
        : FindNode(model,pos,FindModel::TexteNodeType), m_texte(texte), m_case(c), m_regex(regex) {
        if(m_regex){
            m_regular.setPattern(m_texte);
            if(!m_case)
                m_regular.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
    }

    //! Destructeur.
    ~TexteNode() override = default;

    NODE_COPIE(TexteNode)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    numt dataCount(int cible) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant &value) const override;
};
}// end namespace findNodeModel
}// end namespace modelMPS
#endif // FINDMODEL_H
