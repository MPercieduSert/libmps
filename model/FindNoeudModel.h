/*Auteur: PERCIE DU SERT Maxime
 *Date: 26/04/2020
 */
#ifndef FINDMODEL_H
#define FINDMODEL_H

#include <memory>
#include <QDate>
#include <QRegularExpression>
#include "ColonnesForModel.h"
#include "TreeNoeudModel.h"

namespace modelMPS {
//! Déclaration anticipée.
class AbstractColonnesModel;
namespace findNoeudModel {
//! Type des noeuds de l'arbre.
enum typeNoeud {NoType = -1,
               BoolNoeudType,
               ConstanteNoeudType,
               DateNoeudType,
               DateTimeNoeudType,
               DoubleNoeudType,
               IntNoeudType,
               TexteNoeudType,
               UIntNoeudType,
               OperationNoeudType,
               ChoiceNoeudType,
               ConditionNoeudType,
               NbrType};

//! Ensemble de noeuds
enum setNoeud : unsigned {NoSet = 0,
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
class FindNoeudModel : public TreeNoeudModel, public AbstractFindModel {
    Q_OBJECT
public:
   //! Structure d'informations sur une colonne du model.
    struct Colonne {
        int type = findNoeudModel::NoType;      //!< Type de la colonne.
        QString nom;      //!< Nom de la colonne.
    };

protected:
    std::vector<Colonne> m_colonnes;        //!< Informations sur les colonnes.
    AbstractColonnesModel * m_model = nullptr;        //!< Model filtré.
    using QAbstractItemModel::createIndex;
public:
    //! Constructeur.
    FindNoeudModel(QObject * parent = nullptr);

    //! Renvoie le nombre de column pour un parent donné.
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
        {return findNoeudModel::NbrColumn;}

    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operationNoeud un noeud choiceNoeud est ajouté avant row.
    //! Si le noeuds parent n'est pas un operationNoeud, un nouveau operationNoeud prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud ChoiceNoeud est ajouté en cadet.
    void insertNoeud(int row, const QModelIndex & parent);

    //! Donne la liste des noms des colonnes du model associé.
    std::vector<QString> nomColonnes() const;

    //! Supprime le noeud de la ligne row de parent.
    void removeNoeud(int row, const QModelIndex & parent);

    //! Teste si l'arbre est réduit à sa racine.
    bool rootLeaf() const override
        {return m_data.tree().cbegin().toFirstChild().leaf();}

    //! Mutateur des données du model.
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //! Mutateur du model filtré.
    void setModel(AbstractColonnesModel * model);

    //! Teste si la ligne d'indice id vérifie la condition de la racine.
    bool testRoot(szt id) const override;

    //! Teste si la ligne d'indice id vérifie l'arbre des conditions.
    bool testTree(szt id) const override;

public slots:
    //! Applique la recherche au model à filtré.
    void find();

protected:
    //! Fabrique des noeuds.
    Noeud nodeFactory(int type, int row, const QModelIndex & parent) override;

    //! Fabrique des noeuds de condition.
    Noeud nodeConditionFactory(szt pos);
};

//////////////////////////////////// Noeud ///////////////////

namespace findNoeudModel {
/*! \ingroup groupeModel
 * \brief Classe mère des feuilles de recherche (héritage multiple).
 */
class AbstractFindNoeud : public TreeNoeudModel::AbstractNoeud {
public:
    //! Constructeur.
    using AbstractNoeud::AbstractNoeud;

    //! Destructeur.
    virtual ~AbstractFindNoeud();

    //! Test si le noeud n'intervient pas dans la recherche.
    virtual bool empty() const
        {return false;}
};


/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche avec négation.
 */
class AbstractNegationNoeud : public AbstractFindNoeud {
protected:
    bool m_negation;        //!< Négation.
public:
    //! Constructeur.
    using AbstractFindNoeud::AbstractFindNoeud;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == NegColumn)
            return Qt::ItemIsEnabled;
        if(column == OpColumn)
            return Qt::ItemIsSelectable;
        return AbstractNoeud::flags(column);
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
class AbstractConditionNoeud : public AbstractNegationNoeud {
protected:
    uint m_pos = 0;            //!< Position de la colonne dans le model filtré.
    QString m_label;    //!< Label de la condition.
public:
    //! Constructeur.
    AbstractConditionNoeud() = default;

    //!Constructeur.
    AbstractConditionNoeud(uint pos, const QString & label, int type = NoType)
        : AbstractNegationNoeud(type), m_pos(pos), m_label(label) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn)
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if(column == ColonneColumn)
            return AbstractNegationNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNoeud::flags(column);
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
    void setPos(uint pos)
        {m_pos = pos;}

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test(szt id, AbstractColonnesModel * model) const;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    virtual bool testValue(const QVariant & value) const = 0;
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds de recherche définissant une condition de comparaison.
 */
class AbstractComparaisonNoeud : public AbstractConditionNoeud {
protected:
    uint m_comp;            //!< Indice de la comparaison.
public:
    static const std::array<QString, NbrComparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    AbstractComparaisonNoeud(uint pos, const QString & label, uint comp = Egal,int type = NoType)
        : AbstractConditionNoeud(pos,label,type), m_comp(comp) {}

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == ComparaisonColumn)
            return AbstractConditionNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractConditionNoeud::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de filtrage sur les textes.
 */
class BoolNoeud : public AbstractConditionNoeud {
protected:
    QString m_trueLabel;    //!< Label du booléen vraie.
    QString m_falseLabel;   //!< Label du booléen faux.
    bool m_true;            //!< Filtre contenant les vraies.
    bool m_false;            //!< Filtre contenant les faux.
public:
    //! Constructeur.
    BoolNoeud(uint pos, const QString & label,
             const QString & trueLabel = QString(), const QString & falseLabel = QString(),
             bool trueChecked = true, bool falseChecked = true)
        : AbstractConditionNoeud(pos,label,BoolNoeudType),
          m_trueLabel(trueLabel), m_falseLabel(falseLabel),
          m_true(trueChecked), m_false(falseChecked){}

    //! Destructeur.
    ~BoolNoeud() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_true && m_false;}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == TrueColumn || column == FalseColumn)
            return AbstractConditionNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return AbstractConditionNoeud::flags(column);
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
class ChoiceNoeud : public AbstractFindNoeud {
public:
    //! Constructeur.
    ChoiceNoeud() : AbstractFindNoeud(ChoiceNoeudType) {}

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
class DateNoeud : public AbstractComparaisonNoeud {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    DateNoeud(uint pos, const QString & label,const QDate & date = QDate(), uint comp = Egal)
        : AbstractComparaisonNoeud(pos,label,comp,DateNoeudType), m_date(date) {}

    //! Destructeur.
    ~DateNoeud() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return !m_date.isValid();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == DateColumn)
            return AbstractComparaisonNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractComparaisonNoeud::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds de type opération.
 */
class OperationNoeud : public AbstractNegationNoeud {
protected:
    uint m_operation;        //!< Identifiant de l'opération
public:
    static const std::array<QString, NbrOperation> Strings;        //!< Labels des opération.
    //! Constructeur.
    OperationNoeud(uint op = Et) : AbstractNegationNoeud(OperationNoeudType), m_operation(op) {}

    //! Destructeur.
    ~OperationNoeud() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == OpColumn)
            return AbstractNegationNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        return AbstractNegationNoeud::flags(column);
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
class TexteNoeud : public AbstractConditionNoeud {
protected:
    QString m_texte;         //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;            //!< Recherche sensible à la case.
    bool m_regex;           //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    TexteNoeud(uint pos, const QString & label,const QString & texte = QString(), bool c = false,bool regex = false)
        : AbstractConditionNoeud(pos,label,TexteNoeudType), m_texte(texte), m_case(c), m_regex(regex) {
        if(m_regex){
            m_regular.setPattern(m_texte);
            if(!m_case)
                m_regular.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
    }

    //! Destructeur.
    ~TexteNoeud() override = default;

    //! Accesseur de la donnée associé à column.
    QVariant data(int column, int role = Qt::DisplayRole) const override;

    //! Test si le noeud intervient dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Accesseur des drapeaux associés à column.
    Qt::ItemFlags flags(int column) const override {
        if(column == TexteColumn)
            return AbstractConditionNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        if(column == CaseColumn || column == RegexColumn)
            return AbstractConditionNoeud::flags(column) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        return AbstractConditionNoeud::flags(column);
    }

    //! Mutateur de la donnée associé à column.
    bool setData(int column, const QVariant & value, int role = Qt::EditRole) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool testValue(const QVariant & value) const override;
};
}// end namespace findNoeudModel
}// end namespace modelMPS
#endif // FINDMODEL_H
