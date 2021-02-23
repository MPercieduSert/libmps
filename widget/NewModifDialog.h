/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/02/2020
 */
#ifndef NEWMODIFDIALOG_H
#define NEWMODIFDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QtGlobal>
#include <QVBoxLayout>
#include "bdd_predef.h"
#include "entityPredef.h"
#include "IdComboBox.h"
#include "TreeWidget.h"

/*! \defgroup groupeDialogue Fenêtre de Dialogue
 *\brief Ensemble de classes représentant des fenêtres de dialogue.
 */

/*! \ingroup groupeDialogue
 *\brief Espace de noms des fenêtres de dialogue.
 */
namespace dialogMPS {
using namespace type_mps;

/*! \ingroup groupeDialogue
 *\brief Classe abstraite mère de formulaire pour les dialogues
 */
class AbstractNewModifForm : public QWidget {
    Q_OBJECT
protected:
    const bool m_new;                   //! true->création et false->modification ou suppression.
    b2d::Bdd &m_bdd;                //! Base de données.

public:
    //! Constructeur.
    AbstractNewModifForm(b2d::Bdd &bdd, bool newEnt, QWidget *parent = nullptr)
        : QWidget(parent),
          m_new(newEnt),
          m_bdd(bdd) {}

    //! Destructeur par défaut.
    virtual ~AbstractNewModifForm();

    //! Connecte les signals et slots du formulaire.
    virtual void connexion() = 0;

    //! Paramètre l'absence du bouton supprimé.
    virtual bool delDisable() const noexcept
        {return m_new;}

    //! Accesseur de m_new.
    bool newEnt() const noexcept
        {return m_new;}

    //! Titre de la fenêtre de dialogue.
    virtual QString title() const
        {return QString();}

    //! Teste si le formulaire est valide pour l'enregistrement.
    virtual bool valide() const
        {return true;}

public slots:
    //! Sauve l'entité et les réponces du formulairs dans la bases de donnée.
    virtual void save() = 0;

    //! Supprime l'entité dans la bases de donnée.
    virtual bool del() = 0;

    //! Met à jour le formulaire.
    virtual void updateData() {}

signals:
    //! Signal d'activation/désactivation de l'action effacer.
    void effacerPermis(bool);

    //! Signal d'activation/désactivation de l'action d'enrgistrer.
    void savePermis(bool);
};

/*! \ingroup groupeDialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom.
 */
class AbstractNomNewModifForm : public AbstractNewModifForm {
    Q_OBJECT
protected:
    QLabel *m_nameLabel;               //!< Label du nom.
    QLineEdit *m_nameLine;             //!< Ligne d'édition du nom.
    widgetMPS::IdComboBox *m_nameCB;   //!< Liste à choix du nom.
    QHBoxLayout *m_nameLayout;         //!< Calque du nom.
    QVBoxLayout *m_mainLayout;         //!< Calque principal du formulaire.
public:
    //! Constructeur.
    AbstractNomNewModifForm(b2d::Bdd &bdd, const QString &label, bool newEnt, QWidget *parent = nullptr);

    //! Constructeur.
    AbstractNomNewModifForm(b2d::Bdd &bdd, const QString &label, const std::vector<std::pair<idt,QString>> &noms,
                            bool newEnt, QWidget *parent = nullptr)
        : AbstractNomNewModifForm (bdd,label,newEnt,parent)
        {set_noms(noms);}

    //! Destructeur par défaut.
    ~AbstractNomNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie l'identifiant du nom sélectionné.
    idt id() const
        {return m_new ? 0 : m_nameCB->id();}

    //! Renvoie le nom selectionné.
    QString nom() const
        {return m_new ? m_nameLine->text() : m_nameCB->currentText();}

    //! Mutateur de la liste des noms.
    void set_noms(const std::vector<std::pair<idt,QString>> &noms) {
        if(!m_new){
            m_nameCB->clear();
            m_nameCB->addText(noms);
        }
    }

    //! Mutateur de la liste des noms.
    template<class Ent> void set_noms(const vector_ptr<Ent> &vec) {
        if(!m_new){
            m_nameCB->clear();
            m_nameCB->addText(vec,[](const Ent &ent)->const QString &{return ent.nom();});
        }
    }

    //! Mutateur de la liste des noms.
    template<class Ent> void set_noms(vector_ptr<Ent> &&vec) {
        if(!m_new){
            m_nameCB->clear();
            m_nameCB->addText(std::move(vec),[](const Ent &ent)->const QString &{return ent.nom();});
        }
    }

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return m_new ? !m_nameLine->text().isEmpty() : !m_nameCB->currentText().isEmpty();}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupeDialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom et le nom cours.
 */
class Abstractnc_nomNewModifForm : public AbstractNomNewModifForm {
    Q_OBJECT
protected:
    using AbstractNomNewModifForm::m_mainLayout;
    QLabel *m_ncLabel;               //!< Label du nom court.
    QLineEdit *m_ncLine;             //!< Ligne d'édition du nom court.
    QHBoxLayout *m_ncLayout;         //!< Calque du nom court.
public:
    //! Constructeur.
    Abstractnc_nomNewModifForm(b2d::Bdd &bdd, const QString &lableNc, const QString &labelNom, bool newEnt, QWidget *parent = nullptr);

    //! Destructeur.
    ~Abstractnc_nomNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie le nom court selectionné.
    QString nc() const
        {return m_ncLine->text();}

    //! Mutateur du nom cours.$
    void set_nc(const QString &nc)
        {m_ncLine->set_text(nc);}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return AbstractNomNewModifForm::valide() &&!m_ncLine->text().isEmpty();}
protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupeDialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom, le nom court et le type.
 */
class AbstractTypenc_nomNewModifForm : public Abstractnc_nomNewModifForm {
    Q_OBJECT
protected:
    using Type = entities::ensemble_type::Type;
    using Permission = entities::ensemble_type::type_permission;
    using Abstractnc_nomNewModifForm::m_mainLayout;
    int m_cible;                        //!< Numero de cible de l'entité
    bool m_valide;                      //!< Controle de la validité du type
    QLabel *m_typeLabel;               //!< Label du nom
    widgetMPS::TreeWidget *m_typeTree; //!< Séléction du type
public:
    enum {nom_type,ncType,nbrColumn};
    //! Constructeur.
    AbstractTypenc_nomNewModifForm(b2d::bdd_predef &bdd, const QString &refRoot, idt id_entity, const QString &labelType,
                                  const QString &labelNc, const QString &labelNom, bool newEnt, QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractTypenc_nomNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt id_type() const
        {return m_typeTree->id();}

    //! Mutateur du type.
    void set_type(idt id_type)
        {m_typeTree->set_id(id_type);}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return Abstractnc_nomNewModifForm::valide() &&m_valide;}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupeDialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom, le nom court et du parent.
 */
class AbstractParentnc_nomNewModifForm : public Abstractnc_nomNewModifForm {
    Q_OBJECT
protected:
    using Abstractnc_nomNewModifForm::m_mainLayout;
    QLabel *m_parentLabel;             //!< Label du parent
    widgetMPS::TreeWidget *m_parentTree;         //!< Ligne d'édition du nom.
public:
    enum {nomParent,ncParent,nbrColumn};
    //! Constructeur.
    AbstractParentnc_nomNewModifForm(b2d::Bdd &bdd, const QStringList &header, const QString &parentType,
                                  const QString &labelNc, const QString &labelNom, bool newEnt, QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractParentnc_nomNewModifForm() override = default;

    //! Renvoie l'identifiant du parent sélectionné.
    idt id_parent() const
        {return m_parentTree->id();}

    //! Mutateur du type.
    void set_parent(idt id_parent)
        {m_parentTree->set_id(id_parent);}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupeDialogue
 *\brief Fenêtre de dialogue de création, modification ou suppression.
 */
class NewModifDialog : public QDialog {
    Q_OBJECT
protected:
    QPushButton *m_annulButton;                //!< Bouton annuler.
    QPushButton *m_okButton;                   //!< Bouton Ok.
    QPushButton *m_supprButton;                //!< Bouton Suppr
    QHBoxLayout *m_buttonLayout;               //!< Calque des boutons.
    QVBoxLayout *m_mainLayout;               //!< Calque principal.
    AbstractNewModifForm *m_form;                      //!< Formulaire.
public:
    //! Constructeur.
    NewModifDialog(AbstractNewModifForm *form, QWidget *parent = nullptr);

    //! Destructeur.
    ~NewModifDialog() override;

public slots:
    //! Enregistre l'entitée.
    void accept() override {
        try {
            m_form->save();
            QDialog::accept();
        }
        catch(manager::unique_entity_exception &e){
            QMessageBox box(QMessageBox::Critical,tr("Erreur d'enregistrement"),tr("Impossible d'enregistrer dans la base de données.\n"
                                                                                   "La nouvelle entrée entre en conflit avec une donnée"
                                                                                   " existante."),QMessageBox::Close,this);
            box.setDetailedText(e.what());
            box.exec();
        }
        catch(entities::invalide_entity_exception &e){
            QMessageBox box(QMessageBox::Critical,tr("Erreur d'enregistrement"),tr("Impossible d'enregistrer dans la base de données.\n"
                                                                                   "L'entrée proposée est invalide."),QMessageBox::Close,this);
            box.setDetailedText(e.what());
            box.exec();
        }
    }
};

template<class Ent> void AbstractNomNewModifForm::update_temp(Ent &ent) {
    ent.set_id(id());
    m_bdd.get(ent);
    m_nameCB->setEditable(m_bdd.test_autorisation(entity,b2d::Modif));
    emit effacerPermis(m_bdd.test_autorisation(entity,b2d::Suppr));
}

template<class Ent> void Abstractnc_nomNewModifForm::update_temp(Ent &ent) {
    AbstractNomNewModifForm::update_temp<Ent>(ent);
    m_ncLine->setReadOnly(!m_bdd.test_autorisation(entity,b2d::Modif));
    set_nc(ent.nc());
}

template<class Ent> void AbstractTypenc_nomNewModifForm::update_temp(Ent &ent) {
    Abstractnc_nomNewModifForm::update_temp<Ent>(ent);
    set_type(ent.type());
}

template<class Ent> void AbstractParentnc_nomNewModifForm::update_temp(Ent &ent) {
    Abstractnc_nomNewModifForm::update_temp<Ent>(ent);
    set_parent(ent.parent());
}
}
#endif // NEWMODIFDIALOG_H
