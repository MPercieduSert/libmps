/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QDateEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "AbstractColonnesModel.h"
#include "ColonnesForModel.h"
#include "FindModel.h"
#include "NodeView.h"

//////////////////////////////////////// FindDelegate /////////////////////////
namespace delegateMPS {
/*! \ingroup groupeDelegate
 * \brief Delegate de la recherche dans un model de type colonnes.
 */
class FindDelegate : public AbstractNodeDelegate {
    Q_OBJECT
public:
    //! Constructeur.
    FindDelegate(QObject * parent = nullptr)
        : AbstractNodeDelegate (parent) {}

    //! Fabrique d'éditeur.
    NodeWidget * createWidget(const NodeIndex &index, QWidget * parent) const override;
};
}

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type AbstractColonnesModel.
 */
class FindWidget : public QWidget {
    Q_OBJECT
protected:
    // Model & delegate
    using Delegate = delegateMPS::FindDelegate;
    using FindModel = modelMPS::FindModel;
    FindModel * m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton * m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton * m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton * m_findButton;     //!< Bouton de recherche.
    QPushButton * m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    NodeView * m_view;              //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout * m_mainLayout;     //!< Calque principal.
    QHBoxLayout * m_buttonsLayout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindWidget(FindModel * model = new FindModel(),QWidget * parent = nullptr);

    //! Constructeur.
    FindWidget(modelMPS::AbstractColonnesModel * model,QWidget * parent = nullptr)
        : FindWidget(new FindModel(model),parent) {}

    //! Accesseur du Model.
    FindModel * findModel() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void setFindModel(FindModel * model);

    //! Mutateur du model filtré.
    void setModel(modelMPS::AbstractColonnesModel * model) {
        if(m_model)
            m_model->setModel(model);
    }
};
/////////////////////////////////////////////////// Noeud de Recherche /////////////////////////////////
//! Espace de noms des noeud de recheches.
namespace findNodeWidget {
using namespace modelMPS;
using namespace findNodeModel;
/*! \ingroup groupeModel
 * \brief Classe mère des widgets associés aux neuds de recherche.
 */
class FindNodeWidget : public NodeWidget {
    Q_OBJECT
public:
    //! Constructeur.
    using NodeWidget::NodeWidget;
};

/*! \ingroup groupeModel
 * \brief Classe mère des widget de noeuds de recherche avec négation.
 */
class NegationNodeWidget : public FindNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_nonCheckBox;      //!< CheckBox de négation du prédicat de recherche.
    QHBoxLayout * m_mainLayout;     //!< Calque principale du noeud.
public:
    //! Constructeur.
    NegationNodeWidget(const NodeIndex & index, QWidget * parent, int tp = NoType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        connect(m_nonCheckBox,&QCheckBox::stateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::NegCible,m_nonCheckBox->isChecked());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override
        {disconnect(m_nonCheckBox,&QCheckBox::stateChanged,this,nullptr);}

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override
        {m_nonCheckBox->setChecked(m_index.data(FindModel::NegCible,DataRole).toBool());}
};

/*! \ingroup groupeModel
 * \brief Classe mère des widget de noeuds de recherche définissant une condition.
 */
class ConditionNodeWidget : public NegationNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_colonneLabel;            //!< Label du choix de la colonne.
    QComboBox * m_colonneCB;            //!< Choix de la colonne.
    QVBoxLayout * m_colonneLayout;      //!< Calque du choix de colonne.
public:
    //! Constructeur.
    ConditionNodeWidget(const NodeIndex & index, QWidget * parent, int tp = NoType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        NegationNodeWidget::connexion();
        connect(m_colonneCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
            {m_index.model()->setData(m_index,FindModel::ColonneCible,m_colonneCB->currentData());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        NegationNodeWidget::deconnexion();
        disconnect(m_colonneCB,qOverload<int>(&QComboBox::currentIndexChanged),this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override {
        NegationNodeWidget::updateData();
        m_colonneCB->setCurrentIndex(m_colonneCB->findData(m_index.data(FindModel::ColonneCible,DataRole)));
    }
};

/*! \ingroup groupeModel
 * \brief Classe mère des widget de noeuds de recherche définissant une condition de comparaison.
 */
class ComparaisonNodeWidget : public ConditionNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_compLabel;            //!< Label du choix de la comparaison.
    QComboBox * m_compCB;            //!< Choix de la comparaison.
    QVBoxLayout * m_compLayout;      //!< Calque du choix de la comparaison.
public:
    //! Constructeur.
    ComparaisonNodeWidget(const NodeIndex & index, QWidget * parent, int tp = NoType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        ConditionNodeWidget::connexion();
        connect(m_compCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
            {m_index.model()->setData(m_index,FindModel::ColonneCible,m_compCB->currentData());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        ConditionNodeWidget::deconnexion();
        disconnect(m_compCB,qOverload<int>(&QComboBox::currentIndexChanged),this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override {
        ConditionNodeWidget::updateData();
        m_compCB->setCurrentIndex(m_compCB->findData(m_index.data(FindModel::ComparaisonCible,DataRole)));
    }
};

/*! \ingroup groupeModel
 * \brief Classe des widget de noeuds filtrage sur des booléen.
 */
class BoolNodeWidget : public ConditionNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_falseCheck;           //!< CheckBox de faux.
    QCheckBox * m_trueCheck;            //!< CheckBox de vraie
    QHBoxLayout * m_boolLayout;         //!< Calque filtrage sur des booléen.
public:
    //! Constructeur.
    BoolNodeWidget(const NodeIndex & index, QWidget * parent, int tp = FindModel::BoolNodeType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override{
        ConditionNodeWidget::connexion();
        connect(m_falseCheck,&QCheckBox::stateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::FalseCible,m_falseCheck->isChecked());});
        connect(m_trueCheck,&QCheckBox::stateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::TrueCible,m_trueCheck->isChecked());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        ConditionNodeWidget::deconnexion();
        disconnect(m_falseCheck,&QCheckBox::stateChanged,this,nullptr);
        disconnect(m_trueCheck,&QCheckBox::stateChanged,this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override {
        ConditionNodeWidget::updateData();
        m_falseCheck->setChecked(m_index.data(FindModel::FalseCible).toBool());
        m_trueCheck->setChecked(m_index.data(FindModel::TrueCible).toBool());
    }
};

/*! \ingroup groupeModel
 * \brief Classe des widgets de noeud choix.
 */
class ChoiceNodeWidget : public FindNodeWidget {
    Q_OBJECT
protected:
    //! Placement des widgets.
    enum {LabelRow = 0,
         ComboBoxRow = 1,
         OpCol = 0,
         ColonneCol = 1};

    QLabel * m_colonneLabel;        //!< Label du choix de colonne.
    QLabel * m_opLabel;             //!< Label du choix de l'opération.
    QComboBox * m_colonneCB;        //!< Choix de colonne.
    QComboBox * m_opCB;             //!< Choix de l'opération.
    QGridLayout * m_mainLayout;    //!< Calque du choix de l'opération.
public:
    //! Constructeur.
    ChoiceNodeWidget(const NodeIndex & index, QWidget * parent, int tp = FindModel::ChoiceNodeType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        connect(m_opCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
            {m_index.model()->setData(m_index,FindModel::OpCible,m_opCB->currentData());});
        connect(m_colonneCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
            {m_index.model()->setData(m_index,FindModel::ColonneCible,m_colonneCB->currentData());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        disconnect(m_opCB,qOverload<int>(&QComboBox::currentIndexChanged),this,nullptr);
        disconnect(m_colonneCB,qOverload<int>(&QComboBox::currentIndexChanged),this,nullptr);
    }

};

/*! \ingroup groupeModel
 * \brief Classe des widget de noeuds filtrage sur des dates.
 */
class DateNodeWidget : public ComparaisonNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_dateLabel;       //!< Label de date.
    QDateEdit * m_dateEdit;     //!< Choix de la date.
    QVBoxLayout * m_dateLayout; //!< Calque du choix de la date.
public:
    //! Constructeur.
    DateNodeWidget(const NodeIndex & index, QWidget * parent, int tp = FindModel::DateNodeType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        ComparaisonNodeWidget::connexion();
        connect(m_dateEdit,&QDateEdit::dateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::DateCible,m_dateEdit->date());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        ComparaisonNodeWidget::deconnexion();
        disconnect(m_dateEdit,&QDateEdit::dateChanged,this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override {
        ComparaisonNodeWidget::updateData();
        m_dateEdit->setDate(m_index.data(FindModel::DateCible).toDate());
    }
};

/*! \ingroup groupeModel
 * \brief Classe des widget de noeud d'opération.
 */
class OperationNodeWidget : public NegationNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_opLabel;            //!< Label du choix de l'opération.
    QComboBox * m_opCB;            //!< Choix de l'opération.
    QVBoxLayout * m_opLayout;      //!< Calque du choix de l'opération.
public:
    //! Constructeur.
    OperationNodeWidget(const NodeIndex & index, QWidget * parent, int tp = FindModel::OperationNodeType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        NegationNodeWidget::connexion();
        connect(m_opCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
            {m_index.model()->setData(m_index,FindModel::OpCible,m_opCB->currentData());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        NegationNodeWidget::deconnexion();
        disconnect(m_opCB,qOverload<int>(&QComboBox::currentIndexChanged),this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override{
        NegationNodeWidget::updateData();
        m_opCB->setCurrentIndex(m_opCB->findData(m_index.data(FindModel::OpCible,DataRole)));
    }
};

/*! \ingroup groupeModel
 * \brief Classe des widget de noeuds filtrage sur des textes.
 */
class TexteNodeWidget : public ConditionNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_texteLabel;          //!< Label.
    QLineEdit * m_lineEdit;         //!< Texte chercher.
    QCheckBox * m_caseCheck;        //!< CheckBox de la case.
    QCheckBox * m_regexCheck;       //!< CheckBox de l'expression régulière.
    QHBoxLayout * m_texteLayout;    //!< Calque du texte chercher.
public:
    //! Constructeur.
    TexteNodeWidget(const NodeIndex & index, QWidget * parent, int tp = FindModel::TexteNodeType);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override {
        ConditionNodeWidget::connexion();
        connect(m_lineEdit,&QLineEdit::textChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::TexteCible,m_lineEdit->text());});
        connect(m_caseCheck,&QCheckBox::stateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::TrueCible,m_caseCheck->isChecked());});
        connect(m_regexCheck,&QCheckBox::stateChanged,this,[this]()
            {m_index.model()->setData(m_index,FindModel::TrueCible,m_regexCheck->isChecked());});
    }

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override {
        ConditionNodeWidget::deconnexion();
        disconnect(m_lineEdit,&QLineEdit::textChanged,this,nullptr);
        disconnect(m_caseCheck,&QCheckBox::stateChanged,this,nullptr);
        disconnect(m_regexCheck,&QCheckBox::stateChanged,this,nullptr);
    }

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override;
};
}
}
#endif // FINDWIDGET_H
