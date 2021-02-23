/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef STANDARDNODEWIDGET_H
#define STANDARDNODEWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include "CodeWidget.h"
#include "node_ptrView.h"

namespace widgetMPS {

/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'une case à cocher.
 */
class CheckSubnode_ptrWidget : public Subnode_ptrWidget {
    Q_OBJECT
protected:
    QCheckBox *m_checkBox;       //! CheckBox du sous-noeud.
public:
    //! Constructeur.
    CheckSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override {
        m_checkBox->setEnabled(m_flags.test(model_base::EnableFlagnode_ptr));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};

/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'un label.
 */
class LabelSubnode_ptrWidget : public Subnode_ptrWidget {
    Q_OBJECT
protected:
    QLabel *m_label;       //! Label du sous-noeud.
public:
    //! Constructeur.
    LabelSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override
        {setVisible(m_flags.test(model_base::VisibleFlagnode_ptr));}

protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};
/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'un label et d'une liste de choix.
 */
class CodeSubnode_ptrWidget : public LabelSubnode_ptrWidget {
    Q_OBJECT
protected:
    widgetMPS::CodeWidget *m_codeWidget;       //! Choix des drapeaux.
public:
    //! Constructeur.
    CodeSubnode_ptrWidget(const CodeWidget::Cases &cases, const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override {
        LabelSubnode_ptrWidget::updateFlags();
        m_codeWidget->setReadOnly(!m_flags.test(model_base::EnableFlagnode_ptr));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};


/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'un label et d'une liste de choix.
 */
class ComboBoxSubnode_ptrWidget : public LabelSubnode_ptrWidget {
    Q_OBJECT
protected:
    QComboBox *m_comboBox;       //! Liste de choix.
public:
    //! Constructeur.
    ComboBoxSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override {
        LabelSubnode_ptrWidget::updateFlags();
        m_comboBox->setEnabled(m_flags.test(model_base::EnableFlagnode_ptr));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};

/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de date.
 */
class DateSubnode_ptrWidget : public LabelSubnode_ptrWidget {
    Q_OBJECT
protected:
    QDateEdit *m_dateEdit;       //! Ligne d'édition de la date du sous-noeud.
public:
    //! Constructeur.
    DateSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override {
        LabelSubnode_ptrWidget::updateFlags();
        m_dateEdit->setReadOnly(!m_flags.test(model_base::EnableFlagnode_ptr));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};

/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de texte.
 */
class LineEditSubnode_ptrWidget : public LabelSubnode_ptrWidget {
    Q_OBJECT
protected:
    QLineEdit *m_lineEdit;       //! Ligne d'édition du texte du sous-noeud.
public:
    //! Constructeur.
    LineEditSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override {
        LabelSubnode_ptrWidget::updateFlags();
        m_lineEdit->setReadOnly(!m_flags.test(model_base::EnableFlagnode_ptr));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};

/*! \ingroup groupeWidget
 *\brief Classe des sous-noeuds d'un texte à éditer.
 */
class TexteEditnode_ptrWidget : public Subnode_ptrWidget {
    Q_OBJECT
protected:
    QTextEdit *m_texteEdit;       //! Éditeur de texte du sous-noeud.
public:
    //! Constructeur.
    TexteEditnode_ptrWidget(const node_ptrIndex &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void updateFlags() override
        {m_texteEdit->setReadOnly(!m_flags.test(model_base::EnableFlagnode_ptr));}
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubnode_ptr(flag role) override;
};
/*! \ingroup groupeWidget
 *\brief Classe dessinant les nodes widgets à angles arrondis.
 */
class RoundedArcPainter : public node_ptrView::ArcPainter {
public:
    enum parametre {
              LeftExpandMargin = Leftnode_ptrMargin + 10,
              HeightToolsZone = 20,
              HSpacing = 3,
              WidthLine = 2,
              HMaxLine = 100,
              WidthTool = HeightToolsZone,
              RayonToolZone = 3
    };

    //! Dessine l'arc liant les descendants.
    void drawArc(Arcnode_ptrViewWidget *arc) const override;

    //! Dessine la zone permettant l'expansion du noeud.
    void drawToolZone(Arcnode_ptrViewWidget *arc) const override;

    //! Renvoie taille verticale de la zone de demande d'expansion.
    int heightToolZone() const override {return HeightToolsZone;}

    //! Renvoie la marge à gauche pour tracer l'arc.
    int leftExpandedMargin() const override {return LeftExpandMargin;}

    //! Renvoie taille horizontale de la zone de demande d'expansion.
    int widthToolZone(node_ptrView::toolsnode_ptr num) const override {return (num +1) *WidthTool;}
};

/*! \ingroup groupeWidget
 *\brief Classe dessinant les nodes widgets à angles arrondis.
 */
class Roundednode_ptrPainter : public node_ptrWidget::node_ptrPainter {
public:
    enum {Rayon = 10,
         NoSelectedWidth = 2,
         SelectedWidth = NoSelectedWidth,
         CurrentWidth = 2 *SelectedWidth};

protected:
    int m_widthLine = NoSelectedWidth;              //!< Épaisseur du trait.
    QColor m_colorLine;                             //!< Couleur de la ligne.
public:
    //! Constructeur.
    Roundednode_ptrPainter()
        : m_colorLine(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)) {}

    //! Dessine widget.
    void paint(QWidget *widget) override;

    //! Mutateur de l'état de sélection.
    void set_etatSelection(node_ptrWidget::EtatSelection etat) override;
};
}
namespace delegateMPS {
/*! \ingroup groupeDelegate
 *\brief Delegate standard à un node_ptrView.
 */
class Standardnode_ptrDelegate : public Abstractnode_ptrDelegate {
    Q_OBJECT
public:
    using node_ptrIndex = model_base::node_ptrIndex;
    using node_ptrWidget = widgetMPS::node_ptrWidget;
    using Subnode_ptrWidget = widgetMPS::Subnode_ptrWidget;
    //! Constructeur.
    using Abstractnode_ptrDelegate::Abstractnode_ptrDelegate;

    //! Crée un noeud.
    node_ptrWidget *createnode_ptr(const node_ptrIndex &index, QWidget *parent = nullptr) const override;

    //! Crée un sous-noeud.
    virtual Subnode_ptrWidget *createSubnode_ptr(const node_ptrIndex &index, QWidget *parent = nullptr) const;
};

/*! \ingroup groupeDelegate
 *\brief Delegate standard à un node_ptrView avec des noeud de type code.
 */
class CodeStandardnode_ptrDelegate : public Standardnode_ptrDelegate {
    Q_OBJECT
protected:
    widgetMPS::CodeWidget::Cases  m_cases;
public:
    //! Constructeur.
    CodeStandardnode_ptrDelegate(const widgetMPS::CodeWidget::Cases cases, QObject *parent = nullptr)
        : Standardnode_ptrDelegate(parent), m_cases(cases) {}

    //! Crée un sous-noeud.
    Subnode_ptrWidget *createSubnode_ptr(const node_ptrIndex &index, QWidget *parent = nullptr) const override;

    //! Muateur de cases.
    void setCases(const widgetMPS::CodeWidget::Cases cases)
        {m_cases = cases;}
};
}
#endif // STANDARDNODEWIDGET_H
