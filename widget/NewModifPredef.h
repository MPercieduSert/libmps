/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/06/2020
 */
#ifndef NEWMODIFPREDEF_H
#define NEWMODIFPREDEF_H

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTimeEdit>
#include "BddPredef.h"
#include "NewModifDialog.h"


namespace dialogMPS {
/*! \ingroup groupeDialogue
 * \brief Formulaires de création d'un style d'événements.
 */
class EvenementNewModifForm : public AbstractTypeNcNomNewModifForm {
    Q_OBJECT
protected:
    using Evenement = entityMPS::Evenement;
    using EvenementStyle = entityMPS::EvenementStyle;
    // Widget
    QGroupBox * m_debutGroupe;                      //!< Groupe du choix début.
    QGroupBox * m_finGroupe;                        //!< Groupe du choix de fin.
    QGroupBox * m_optionGroupe;                     //!< Groupe du choix des options.
    QLabel * m_styleLabel;                          //!< Label du choix du style.
    QLabel * m_titreLabel;                          //!< Label du choix du titre.
    QCalendarWidget * m_debutDate;                  //!< Choix de la date de début.
    QCalendarWidget * m_finDate;                    //!< Choix de la date de fin.
    QCheckBox * m_jourEntierOption;                 //!< Option d'un événement sur des jours entiers
    QCheckBox * m_unJourOption;                     //!< Option d'un événement sur un seul jour.
    QTimeEdit * m_debutHeure;                       //!< Choix de l'heure de début.
    QTimeEdit * m_finHeure;                         //!< Choix de l'heure de fin.
    QLineEdit * m_titreEdit;                        //!< Choix du titre.
    widgetMPS::IdComboBox * m_styleICB;             //!< Choix du style.
    // Calque
    QGridLayout * m_optionLayout;                   //!< Calque des options.
    QHBoxLayout * m_debutFinLayout;                 //!< Calque du choix du début et fin de l'événement.
    QVBoxLayout * m_debutLayout;                    //!< Calque du choix du début de l'événement.
    QVBoxLayout * m_finLayout;                      //!< Calque du choix de la fin de l'événement.

public:
    //! Constructeur.
    EvenementNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //! Destructeur.
    ~EvenementNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;
    
    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouvel événements") :
                        tr("Modifiaction d'un événements");}

    //! Teste si le formulaire est valide.
    bool valide() const override {
        if(!AbstractTypeNcNomNewModifForm::valide() || m_titreEdit->text().isEmpty())
            return false;
        if(m_debutDate->selectedDate() < m_finDate->selectedDate())
            return true;
        return m_debutDate->selectedDate() == m_finDate->selectedDate()
                && m_debutHeure->time() <= m_finHeure->time()
                && m_unJourOption->checkState() == Qt::Checked;
    }

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Evenement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
};

/*! \ingroup groupeDialogue
 * \brief Formulaires de création d'un style d'événements.
 */
class EvenementStyleNewModifForm : public AbstractNomNewModifForm {
    Q_OBJECT
protected:
    //! Objet .
    enum objet {Bordure,
                Fond,
                Texte,
                Titre};

    using EvenementStyle = entityMPS::EvenementStyle;
    // Donnée
    QColor m_bordureCouleur;                //!< Couleur de la bordure.
    QColor m_fondCouleur;                   //!< Couleur du fond.
    QColor m_texteCouleur;                  //!< Couleur du texte.
    QColor m_titreCouleur;                  //!< Couleur du titre.

    // Widget
    QGroupBox * m_fondGroupe;               //!< Paramétrage du fond.
    QGroupBox * m_formGroupe;               //!< Paramétrage de la forme.
    QGroupBox * m_texteGroupe;              //!< Paramétrage du texte.
    QGroupBox * m_titreGroupe;              //!< Paramétrage du titre.
    QLabel * m_bordureLabel;                //!< Label du choix de la couleur de la bordure.
    QLabel * m_fondLabel;                   //!< Label du choix de la couleur et de la texture du fond.
    QLabel * m_formLabel;                   //!< Label du choix de la forme.
    QLabel * m_texteLabel;                  //!< Label du choix de la couleur et de la police du texte.
    QLabel * m_titreLabel;                  //!< Label du choix de la couleur et de la police du texte.
    QComboBox * m_bordureCB;                //!< Choix de la bordure.
    QComboBox * m_formCB;                   //!< Choix de la forme.
    QComboBox * m_textureCB;                //!< Choix de la texture du fond.
    QPushButton * m_couleurBordureBouton;   //!< Choix de la couleur de la bordure.
    QPushButton * m_couleurFondBouton;      //!< Choix de la couleur du fond.
    QPushButton * m_couleurTexteBouton;     //!< Choix de la couleur du texte.
    QPushButton * m_couleurTitreBouton;     //!< Choix de la couleur du titre.
    QPushButton * m_policeTexteBouton;      //!< Choix de la police du texte.
    QPushButton * m_policeTitreBouton;      //!< Choix de la police du titre.

    // Calque
    QHBoxLayout * m_fondLayout;             //!< Calque du groupe fond.
    QHBoxLayout * m_formLayout;             //!< Calque du groupe forme.
    QHBoxLayout * m_texteLayout;            //!< Calque du groupe texte.
    QHBoxLayout * m_titreLayout;            //!< Calque du groupe titre
public:
    //! Constructeur.
    EvenementStyleNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //! Destructeur.
    ~EvenementStyleNewModifForm() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau style d'événements") :
                        tr("Modifiaction d'un style d'événements");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(EvenementStyle(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
protected:
    //! Mutateur de la couleur.
    void setColor(objet ob, const QColor & color);
};

/*! \ingroup groupeDialog
 * \brief Formulaire de création et modification des sources.
 */
class SourceNewModifForm : public dialogMPS::AbstractTypeNcNomNewModifForm {
    Q_OBJECT
protected:
    using Source = entityMPS::Source;
public:
    //! Constructeur.
    SourceNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent = nullptr)
        : AbstractTypeNcNomNewModifForm(static_cast<bddMPS::BddPredef &>(bdd),"source_root_tp",Source::ID,
                                        "Type de la source :","Nom abrégé de la source :","Nom de la source :",
                                                                     newEnt,parent) {}

    //!Destructeur.
    ~SourceNewModifForm() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'une nouvelle source") :
                        tr("Modification d'une source existante");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Source(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
};
}

#endif // NEWMODIFPREDEF_H
