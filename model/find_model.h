/*Auteur: PERCIE DU SERT Maxime
 *Date: 31/10/2020
 */
#ifndef FIND_MODEL_H
#define FIND_MODEL_H

#include <memory>
#include <QDate>
#include <QRegularExpression>
#include "item_node_model.h"
#include "abstract_colonnes_model.h"
#include "colonnes_for_model.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Classe mère des model de recherche.
 */
class find_model : public item_node_model, public abstract_find_model {
    Q_OBJECT
public:
    //! Type des noeuds de l'arbre.
    enum type_node {No_Type = -1,
                   Bool_Node_Type,
                   Constante_Node_Type,
                   Date_Node_Type,
                   Date_Time_Node_Type,
                   Double_Node_Type,
                   Int_Node_Type,
                   Texte_Node_Type,
                   UInt_Node_Type,
                   Operation_Node_Type,
                   Choice_Node_Type,
                   Condition_Node_Type,
                   Nbr_Type};

    //! Cible des données.
    enum data_cible {Comparaison_Cible,
                 Case_Cible,
                 Colonne_Cible,
                 Date_Cible,
                 False_Cible,
                 Neg_Cible,
                 Op_Cible,
                 Regex_Cible,
                 Texte_Cible,
                 True_Cible};

    //! Opérations binaire.
    enum nodeOperation {Et,
                        Ou,
                        Ou_Exclusif,
                        Nbr_Operation};

    //! Comparaison à une valeur.
    enum comparaison {Egal,
                     Different,
                     Inferieure,
                     Superieure,
                     Inf_Egal,
                     Sup_Egal,
                     Nbr_Comparaison};

   //! Structure d'informations sur une colonne du model.
    struct colonne_info {
        int type = No_Type;     //!< Type de la colonne.
        QString nom;            //!< Nom de la colonne.
    };

protected:
    std::vector<colonne_info> m_colonnes;   //!< Informations sur les colonnes.
    abstract_colonnes_model *m_model;       //!< Model filtré.
    using item_node_model::create_index;
public:
    //! Constructeur.
    find_model(abstract_colonnes_model *model = nullptr, QObject *parent = nullptr);

    //! Accesseur du model filtré.
    abstract_colonnes_model *model() const
        {return m_model;}

    //! Donne la liste des noms des colonnes du model associé.
    QMap<QString,QVariant> nom_colonnes() const;

    //! Teste si l'arbre est réduit à sa racine.
    bool root_leaf() const override
        {return m_data.cbegin().leaf();}

    //! Mutateur du model filtré.
    void set_model(abstract_colonnes_model *model);

    //! Teste si la ligne d'indice id vérifie la condition de la racine.
    bool test_root(szt id) const override;

    //! Teste si la ligne d'indice id vérifie l'arbre des conditions.
    bool test_tree(szt id) const override;

public slots:
    //! Applique la recherche au model à filtré.
    void find();

    //! Réinitialise le model de recherche.
    void reset();

protected:
    //! Insert un nouveau noeud.
    //! Si le noeuds parent est un operation_node un noeud choice_node est ajouté avant pos.
    //! Si le noeuds parent n'est pas un operationnode_ptr, un nouveau operationnode_ptr prend sa place
    //! et le noeud parent devient l'ainé de ce nouveau et un noeud Choicenode_ptr est ajouté en cadet.
    //! Insert count noeuds de nature type avant la position pos de parent.
    std::list<node_iter> insert(const node_index &parent, numt pos, numt count, int type = Default_Type) override;

    //! Fabrique des noeuds.
    node_ptr node_factory(const node_index &parent, post pos, int type) override;

    //! Fabrique des noeuds de condition pour une colonne.
    node_ptr node_condition_factory(szt colonne);

    //! Supprime le noeud et ses descendants.
    bool remove(const node_index &index, numt count = 1) override;

    //! Mutateur des données du model.
    bool set(const node_index &index, const QVariant &value, int role) override;
};

//////////////////////////////////// Noeud ///////////////////

namespace find_node_model {
/*! \ingroup groupe_model
 *\brief Classe mère des neuds de recherche.
 */
class find_node : public item_node {
protected:
    bool m_negation = false;        //!< Négation.
    find_model *m_model;    //!< Pointeur sur le model.
    numt m_pos;              //!< position de la colonne dans le model filtré.
public:
    enum {Vide = -1,
          No_Colonne = 0};

    //! Constructeur.
    find_node(find_model *model, numt pos, int type = No_Type)
        : item_node(type), m_model(model), m_pos(pos) {}

    NODE_COPIE(find_node)

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

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
    void set_pos(numt pos)
        {m_pos = pos;}

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test(szt id, abstract_colonnes_model *model) const
        {return test_value(model->colonne(m_pos).data_test(id));}

    //! Teste une valeur.
    virtual bool test_value(const QVariant &/*value*/) const {return true;}
};

/*! \ingroup groupe_model
 *\brief Classe mère des noeuds de recherche définissant une condition de comparaison.
 */
class comparaison_node : public find_node {
protected:
    numt m_comp;    //!< Indice de la comparaison.
public:
    static const std::array<QString, find_model::Nbr_Comparaison> Strings;        //!< Labels des comparaisons.
    //! Constructeur.
    comparaison_node(find_model *model, numt pos, numt comp = find_model::Egal,int type = No_Type)
        : find_node(model,pos,type), m_comp(comp) {}

    NODE_COPIE(comparaison_node)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;
};

/*! \ingroup groupe_model
 *\brief Classe des noeuds de filtrage sur des booléen.
 */
class bool_node : public find_node {
protected:
    bool m_false;           //!< Filtre contenant les faux.
    bool m_true;            //!< Filtre contenant les vraies.
    QString m_false_label;  //!< Label du filtre faux.
    QString m_true_label;   //!< Label du filtre faux.
public:
    //! Constructeur.
    bool_node(find_model *model, numt pos, const QString &false_label = QString(), const QString true_label = QString(),
             bool true_checked = true, bool false_checked = true)
        : find_node(model,pos,find_model::Bool_Node_Type),
          m_false(false_checked), m_true(true_checked),
          m_false_label(false_label), m_true_label(true_label)  {}

    //! Destructeur.
    ~bool_node() override = default;

    NODE_COPIE(bool_node)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_true &&m_false;}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test_value(const QVariant &value) const override
        {return value.toBool() ? m_true : m_false;}
};

/*! \ingroup groupe_model
 *\brief Classe des noeuds de filtrage sur les dates.
 */
class date_node : public comparaison_node {
protected:
    QDate m_date;       //!< Date de filtrage.
public:
    //! Constructeur.
    date_node(find_model *model, numt pos,const QDate &date = QDate(), numt comp = find_model::Egal)
        : comparaison_node(model,pos,comp,find_model::Date_Node_Type), m_date(date) {}

    //! Destructeur.
    ~date_node() override = default;

    NODE_COPIE(date_node)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return !m_date.isValid();}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test_value(const QVariant &value) const override;
};

/*! \ingroup groupe_model
 *\brief Classe des noeuds de filtrage sur les textes.
 */
class texte_node : public find_node {
protected:
    QString m_texte;                //!< Texte de filtrage.
    QRegularExpression m_regular;   //!< Texte de l'expression regulière.
    bool m_case;                    //!< Recherche sensible à la case.
    bool m_regex;                   //!< La recherche est une expression régulière.
public:
    //! Constructeur.
    texte_node(find_model *model, numt pos,const QString &texte = QString(), bool c = false,bool regex = false);

    //! Destructeur.
    ~texte_node() override = default;

    NODE_COPIE(texte_node)

    //! Accesseur de la donnée associé à column.
    QVariant data(int type, int role, numt num = 0) const override;

    //! Test si le noeud n'intervient pas dans la recherche.
    bool empty() const override
        {return m_texte.isEmpty();}

    //! Mutateur de la donnée associé à column.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Teste si la ligne d'indice id vérifie la condition du noeud.
    bool test_value(const QVariant &value) const override;
};
}// end namespace find_node_model
}// end namespace model_base
}// end namespace mps
#endif // FIND_MODEL_H
