/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef STANDARD_NODE_WIDGET_H
#define STANDARD_NODE_WIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include "code_widget.h"
#include "node_view.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'une case à cocher.
 */
class check_sub_node_widget : public sub_node_widget {
    Q_OBJECT
protected:
    QCheckBox *m_check_box;       //! CheckBox du sous-noeud.
public:
    //! Constructeur.
    check_sub_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override {
        m_check_box->setEnabled(m_flags.test(model_base::Enable_Flag_Node));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'un label.
 */
class label_sub_node_widget : public sub_node_widget {
    Q_OBJECT
protected:
    QLabel *m_label;       //! Label du sous-noeud.
public:
    //! Constructeur.
    label_sub_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override
        {setVisible(m_flags.test(model_base::Visible_Flag_Node));}

protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'un label et d'une liste de choix.
 */
class code_sub_node_widget : public label_sub_node_widget {
    Q_OBJECT
protected:
    widget::code_widget *m_code_widget;       //! Choix des drapeaux.
public:
    //! Constructeur.
    code_sub_node_widget(const code_widget::vec_option_case &cases, const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override {
        label_sub_node_widget::update_flags();
        m_code_widget->set_read_only(!m_flags.test(model_base::Enable_Flag_Node));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'un label et d'une liste de choix.
 */
class combo_box_sub_node_widget : public label_sub_node_widget {
    Q_OBJECT
protected:
    QComboBox *m_combo_box;       //! Liste de choix.
public:
    //! Constructeur.
    combo_box_sub_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override {
        label_sub_node_widget::update_flags();
        m_combo_box->setEnabled(m_flags.test(model_base::Enable_Flag_Node));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de date.
 */
class date_sub_node_widget : public label_sub_node_widget {
    Q_OBJECT
protected:
    QDateEdit *m_dateEdit;       //! Ligne d'édition de la date du sous-noeud.
public:
    //! Constructeur.
    date_sub_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override {
        label_sub_node_widget::update_flags();
        m_dateEdit->setReadOnly(!m_flags.test(model_base::Enable_Flag_Node));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de texte.
 */
class line_edit_sub_node_widget : public label_sub_node_widget {
    Q_OBJECT
protected:
    QLineEdit *m_lineEdit;       //! Ligne d'édition du texte du sous-noeud.
public:
    //! Constructeur.
    line_edit_sub_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override {
        label_sub_node_widget::update_flags();
        m_lineEdit->setReadOnly(!m_flags.test(model_base::Enable_Flag_Node));
    }
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe des sous-noeuds d'un texte à éditer.
 */
class texte_edit_node_widget : public sub_node_widget {
    Q_OBJECT
protected:
    QTextEdit *m_texteEdit;       //! Éditeur de texte du sous-noeud.
public:
    //! Constructeur.
    texte_edit_node_widget(const node_index &index, QWidget *parent);

    //! Met à jour le widget après un changement de drapeaux.
    void update_flags() override
        {m_texteEdit->setReadOnly(!m_flags.test(model_base::Enable_Flag_Node));}
protected:
    //! Met à jour les données du label à partir des données du model.
    void update_data_sub_node(flag role) override;
};

/*! \ingroup groupe_Widget
 *\brief Classe dessinant les nodes widgets à angles arrondis.
 */
class rounded_arc_painter : public node_view::arc_painter {
public:
    enum parametre {
              Left_Expand_Margin = Left_Node_Margin + 10,
              Height_Tools_Zone = 20,
              HSpacing = 3,
              Width_Line = 2,
              HMax_Line = 100,
              Width_Tool = Height_Tools_Zone,
              Rayon_Tool_Zone = 3
    };

    //! Dessine l'arc liant les descendants.
    void draw_arc(arc_node_view_widget *arc) const override;

    //! Dessine la zone permettant l'expansion du noeud.
    void draw_tool_zone(arc_node_view_widget *arc) const override;

    //! Renvoie taille verticale de la zone de demande d'expansion.
    int height_tool_zone() const override {return Height_Tools_Zone;}

    //! Renvoie la marge à gauche pour tracer l'arc.
    int left_expanded_margin() const override {return Left_Expand_Margin;}

    //! Renvoie taille horizontale de la zone de demande d'expansion.
    int width_tool_zone(node_view::tools_node num) const override {return (num +1) *Width_Tool;}
};

/*! \ingroup groupe_Widget
 *\brief Classe dessinant les nodes widgets à angles arrondis.
 */
class rounded_node_painter : public node_widget::node_painter {
public:
    enum {Rayon = 10,
         No_Selected_Width = 2,
         Selected_Width = No_Selected_Width,
         Current_Width = 2 *Selected_Width};

protected:
    int m_width_line = No_Selected_Width;   //!< Épaisseur du trait.
    QColor m_color_line;                    //!< Couleur de la ligne.
public:
    //! Constructeur.
    rounded_node_painter()
        : m_color_line(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)) {}

    //! Dessine widget.
    void paint(QWidget *widget) override;

    //! Mutateur de l'état de sélection.
    void set_etat_selection(node_widget::etat_selection etat) override;
};
}

namespace delegate {
/*! \ingroup groupe_delegate
 *\brief Delegate standard à un node_view.
 */
class standard_node_delegate : public abstract_node_delegate {
    Q_OBJECT
public:
    using node_index = model_base::node_index;
    using node_widget = widget::node_widget;
    using sub_node_widget = widget::sub_node_widget;
    //! Constructeur.
    using abstract_node_delegate::abstract_node_delegate;

    //! Crée un noeud.
    node_widget *create_node(const node_index &index, QWidget *parent = nullptr) const override;

    //! Crée un sous-noeud.
    virtual sub_node_widget *create_sub_node(const node_index &index, QWidget *parent = nullptr) const;
};

/*! \ingroup groupe_delegate
 *\brief Delegate standard à un node_view avec des noeud de type code.
 */
class code_standard_node_delegate : public standard_node_delegate {
    Q_OBJECT
protected:
    widget::code_widget::vec_option_case m_cases;
public:
    //! Constructeur.
    code_standard_node_delegate(const widget::code_widget::vec_option_case cases, QObject *parent = nullptr)
        : standard_node_delegate(parent), m_cases(cases) {}

    //! Crée un sous-noeud.
    sub_node_widget *create_sub_node(const node_index &index, QWidget *parent = nullptr) const override;

    //! Muateur de cases.
    void setCases(const widget::code_widget::vec_option_case cases)
        {m_cases = cases;}
};
}}
#endif // STANDARD_NODE_WIDGET_H
