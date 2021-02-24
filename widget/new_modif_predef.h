/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/06/2020
 */
#ifndef NEW_MODIF_PREDEF_H
#define NEW_MODIF_PREDEF_H

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTimeEdit>
#include "bdd_predef.h"
#include "new_modif_dialog.h"

namespace mps {
namespace dialogue {
/*! \ingroup groupe_dialogue
 *\brief Formulaires de création d'un style d'événements.
 */
class evenement_new_modif_form : public abstract_type_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    using evenement = entities::evenement;
    using evenement_style = entities::evenement_style;
    // Widget
    QGroupBox *m_debut_groupe;                      //!< Groupe du choix début.
    QGroupBox *m_fin_groupe;                        //!< Groupe du choix de fin.
    QGroupBox *m_option_groupe;                     //!< Groupe du choix des options.
    QLabel *m_style_label;                          //!< Label du choix du style.
    QLabel *m_titre_label;                          //!< Label du choix du titre.
    QCalendarWidget *m_debut_date;                  //!< Choix de la date de début.
    QCalendarWidget *m_fin_date;                    //!< Choix de la date de fin.
    QCheckBox *m_jourEntier_option;                 //!< Option d'un événement sur des jours entiers
    QCheckBox *m_unJour_option;                     //!< Option d'un événement sur un seul jour.
    QTimeEdit *m_debut_heure;                       //!< Choix de l'heure de début.
    QTimeEdit *m_fin_heure;                         //!< Choix de l'heure de fin.
    QLineEdit *m_titre_edit;                        //!< Choix du titre.
    widget::id_combo_box *m_style_icb;             //!< Choix du style.
    // Calque
    QGridLayout *m_option_layout;                   //!< Calque des options.
    QHBoxLayout *m_debut_fin_layout;                 //!< Calque du choix du début et fin de l'événement.
    QVBoxLayout *m_debut_layout;                    //!< Calque du choix du début de l'événement.
    QVBoxLayout *m_fin_layout;                      //!< Calque du choix de la fin de l'événement.

public:
    //! Constructeur.
    evenement_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~evenement_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;
    
    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouvel événements") :
                        tr("Modifiaction d'un événements");}

    //! Teste si le formulaire est valide.
    bool valide() const override {
        if(!abstract_type_nc_nom_new_modif_form::valide() || m_titre_edit->text().isEmpty())
            return false;
        if(m_debut_date->selectedDate() < m_fin_date->selectedDate())
            return true;
        return m_debut_date->selectedDate() == m_fin_date->selectedDate()
                &&m_debut_heure->time() <= m_fin_heure->time()
                &&m_unJour_option->checkState() == Qt::Checked;
    }

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new &&m_bdd.del(evenement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};

/*! \ingroup groupe_dialogue
 *\brief Formulaires de création d'un style d'événements.
 */
class evenement_style_new_modif_form : public abstract_nom_new_modif_form {
    Q_OBJECT
protected:
    //! Objet .
    enum objet {Bordure,
                Fond,
                Texte,
                Titre};

    using evenement_style = entities::evenement_style;
    // Donnée
    QColor m_bordure_couleur;                //!< Couleur de la bordure.
    QColor m_fond_couleur;                   //!< Couleur du fond.
    QColor m_texte_couleur;                  //!< Couleur du texte.
    QColor m_titre_couleur;                  //!< Couleur du titre.

    // Widget
    QGroupBox *m_fond_groupe;               //!< Paramétrage du fond.
    QGroupBox *m_form_groupe;               //!< Paramétrage de la forme.
    QGroupBox *m_texte_groupe;              //!< Paramétrage du texte.
    QGroupBox *m_titre_groupe;              //!< Paramétrage du titre.
    QLabel *m_bordure_label;                //!< Label du choix de la couleur de la bordure.
    QLabel *m_fond_label;                   //!< Label du choix de la couleur et de la texture du fond.
    QLabel *m_form_label;                   //!< Label du choix de la forme.
    QLabel *m_texte_label;                  //!< Label du choix de la couleur et de la police du texte.
    QLabel *m_titre_label;                  //!< Label du choix de la couleur et de la police du texte.
    QComboBox *m_bordure_cb;                //!< Choix de la bordure.
    QComboBox *m_form_cb;                   //!< Choix de la forme.
    QComboBox *m_texture_cb;                //!< Choix de la texture du fond.
    QPushButton *m_couleur_bordure_bouton;  //!< Choix de la couleur de la bordure.
    QPushButton *m_couleur_fond_bouton;     //!< Choix de la couleur du fond.
    QPushButton *m_couleur_texte_bouton;     //!< Choix de la couleur du texte.
    QPushButton *m_couleur_titre_bouton;     //!< Choix de la couleur du titre.
    QPushButton *m_police_texte_bouton;      //!< Choix de la police du texte.
    QPushButton *m_police_titre_bouton;      //!< Choix de la police du titre.

    // Calque
    QHBoxLayout *m_fond_layout;             //!< Calque du groupe fond.
    QHBoxLayout *m_form_layout;             //!< Calque du groupe forme.
    QHBoxLayout *m_texte_layout;            //!< Calque du groupe texte.
    QHBoxLayout *m_titre_layout;            //!< Calque du groupe titre
public:
    //! Constructeur.
    evenement_style_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~evenement_style_new_modif_form() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau style d'événements") :
                        tr("Modifiaction d'un style d'événements");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new &&m_bdd.del(evenement_style(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
protected:
    //! Mutateur de la couleur.
    void set_color(objet ob, const QColor &color);
};

/*! \ingroup groupeDialog
 *\brief Formulaire de création et modification des sources.
 */
class source_new_modif_form : public dialogue::abstract_type_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    using source = entities::source;
public:
    //! Constructeur.
    source_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr)
        : abstract_type_nc_nom_new_modif_form(static_cast<b2d::bdd_predef &>(bdd),"source_root",source::ID,
                                        "Type de la source :","Nom abrégé de la source :","Nom de la source :",
                                                                     new_ent,parent) {}

    //!Destructeur.
    ~source_new_modif_form() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'une nouvelle source") :
                        tr("Modification d'une source existante");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new &&m_bdd.del(source(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};
}}
#endif // NEW_MODIF_PREDEF_H
