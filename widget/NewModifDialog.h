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
#include "BddPredef.h"
#include "EntityPredef.h"
#include "IdComboBox.h"

/*! \defgroup groupeDialogue Fenêtre de Dialogue
 * \brief Ensemble de classes représentant des fenêtres de dialogue.
 */

/*! \ingroup groupeDialogue
 * \brief Espace de noms des fenêtres de dialogue.
 */
namespace dialogMPS {
using namespace typeMPS;

/*! \ingroup groupeDialogue
 * \brief Classe abstraite mère de formulaire pour les dialogues
 */
class AbstractNewModifForm : public QWidget {
    Q_OBJECT
protected:
    const bool m_new;                   //! true->création et false->modification ou suppression.
    bddMPS::Bdd & m_bdd;                //! Base de données.

public:
    //! Constructeur.
    AbstractNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent = nullptr)
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
 * \brief Classe des formulaires pour les dialogues commençant par le choix du nom.
 */
class AbstractNomNewModifForm : public AbstractNewModifForm {
    Q_OBJECT
protected:
    QLabel * m_nameLabel;               //!< Label du nom.
    QLineEdit * m_nameLine;             //!< Ligne d'édition du nom.
    widgetMPS::IdComboBox * m_nameCB;   //!< Liste à choix du nom.
    QVBoxLayout * m_mainLayout;         //!< Calque principal du formulaire.

public:
    //! Constructeur.
    AbstractNomNewModifForm(bddMPS::Bdd & bdd, const QString &label, bool newEnt, QWidget * parent = nullptr);

    //! Constructeur.
    AbstractNomNewModifForm(bddMPS::Bdd & bdd, const QString &label, const std::vector<std::pair<idt,QString>> & noms,
                            bool newEnt, QWidget * parent = nullptr)
        : AbstractNomNewModifForm (bdd,label,newEnt,parent)
        {setNoms(noms);}

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
    void setNoms(const std::vector<std::pair<idt,QString>> & noms)
        {m_nameCB->setText(noms);}

    //! Mutateur de la liste des noms.
    template<class Ent> void setNoms(const conteneurMPS::VectorPtr<Ent> & vec)
        {m_nameCB->setText(vec,[](const Ent & entity)->const QString &{return entity.nom();});}

    //! Mutateur de la liste des noms.
    template<class Ent> void setNoms(conteneurMPS::VectorPtr<Ent> && vec)
        {m_nameCB->setText(std::move(vec),[](const Ent & entity)->const QString &{return entity.nom();});}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return m_new ? !m_nameLine->text().isEmpty() : !m_nameCB->currentText().isEmpty();}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void updateTemp(Ent & entity);
};

/*! \ingroup groupeDialogue
 * \brief Classe des formulaires pour les dialogues commençant par le choix du nom et le nom cours.
 */
class AbstractNcNomNewModifForm : public AbstractNomNewModifForm {
    Q_OBJECT
protected:
    using AbstractNomNewModifForm::m_mainLayout;
    QLabel * m_ncLabel;               //!< Label du nom court.
    QLineEdit * m_ncLine;             //!< Ligne d'édition du nom court.
public:
    //! Constructeur.
    AbstractNcNomNewModifForm(bddMPS::Bdd & bdd, const QString &lableNc, const QString &labelNom, bool newEnt, QWidget * parent = nullptr);

    //! Destructeur.
    ~AbstractNcNomNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie le nom court selectionné.
    QString nc() const
        {return m_ncLine->text();}

    //! Mutateur du nom cours.$
    void setNc(const QString & nc)
        {m_ncLine->setText(nc);}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return AbstractNomNewModifForm::valide() && !m_ncLine->text().isEmpty();}
protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void updateTemp(Ent & entity);
};

/*! \ingroup groupeDialogue
 * \brief Classe des formulaires pour les dialogues commençant par le choix du nom, le nom court et le type.
 */
class AbstractTypeNcNomNewModifForm : public AbstractNcNomNewModifForm {
    Q_OBJECT
protected:
    using Type = entityMPS::ensembleType::Type;
    using Permission = entityMPS::ensembleType::TypePermission;
    using AbstractNcNomNewModifForm::m_mainLayout;
    int m_cible;                        //!< Numero de cible de l'entité
    bool m_valide;              //!< Controle de la validité du type
    QLabel * m_typeLabel;               //!< Label du nom
    QTreeWidget * m_typeTree;             //!< Ligne d'édition du nom.
public:
    enum {nomType,ncType,nbrColumn};
    //! Constructeur.
    AbstractTypeNcNomNewModifForm(bddMPS::BddPredef &bdd, idt idProgRoot, idt idEntity, const QString & labelType, const QString &labelNc,
                                  const QString &labelNom, bool newEnt, QWidget * parent = nullptr);

    //! Destructeur.
    ~AbstractTypeNcNomNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt idType() const
        {return m_typeTree->selectionModel()->hasSelection() ? m_typeTree->currentItem()->data(nomType,Qt::UserRole).toUInt() : 0;}

    //! Mutateur du type.
    void setType(idt idType);

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return AbstractNcNomNewModifForm::valide() && m_valide;}

protected:
    //! Ajouter les fils de iter à item.
    void addChilds(conteneurMPS::tree<Type>::brother_iterator iter, QTreeWidgetItem * item);

    //! Modifie les drapeaux de selection des items.
    void setFlags(QTreeWidgetItem * item);

    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void updateTemp(Ent & entity);
};

/*! \ingroup groupeDialogue
 * \brief Fenêtre de dialogue de création, modification ou suppression.
 */
class NewModifDialog : public QDialog {
    Q_OBJECT
protected:
    QPushButton * m_annulButton;                //!< Bouton annuler.
    QPushButton * m_okButton;                   //!< Bouton Ok.
    QPushButton * m_supprButton;                //!< Bouton Suppr
    QHBoxLayout * m_buttonLayout;               //!< Calque des boutons.
    QVBoxLayout * m_mainLayout;               //!< Calque principal.
    AbstractNewModifForm * m_form;                      //!< Formulaire.
public:
    //! Constructeur.
    NewModifDialog(AbstractNewModifForm * form, QWidget * parent = nullptr);

    //! Destructeur.
    ~NewModifDialog() override;

public slots:
    //! Enregistre l'entitée.
    void accept() override {
        try {
            m_form->save();
            QDialog::accept();
        }
        catch(managerMPS::UniqueEntityException & e){
            QMessageBox box(QMessageBox::Critical,tr("Erreur d'enregistrement"),tr("Impossible d'enregistrer dans la base de données.\n"
                                                                                   "La nouvelle entrée entre en conflit avec une donnée"
                                                                                   " existante."),QMessageBox::Close,this);
            box.setDetailedText(e.what());
            box.exec();
        }
        catch(entityMPS::InvalideEntityException & e){
            QMessageBox box(QMessageBox::Critical,tr("Erreur d'enregistrement"),tr("Impossible d'enregistrer dans la base de données.\n"
                                                                                   "L'entrée proposée est invalide."),QMessageBox::Close,this);
            box.setDetailedText(e.what());
            box.exec();
        }
    }
};

template<class Ent> void AbstractNomNewModifForm::updateTemp(Ent & entity) {
    entity.setId(id());
    m_bdd.get(entity);
    m_nameCB->setEditable(m_bdd.getAutorisation(entity,bddMPS::Modif));
    emit effacerPermis(m_bdd.getAutorisation(entity,bddMPS::Suppr));
}

template<class Ent> void AbstractNcNomNewModifForm::updateTemp(Ent & entity) {
    AbstractNomNewModifForm::updateTemp<Ent>(entity);
    m_ncLine->setReadOnly(!m_bdd.getAutorisation(entity,bddMPS::Modif));
    setNc(entity.nc());
}

template<class Ent> void AbstractTypeNcNomNewModifForm::updateTemp(Ent & entity) {
    AbstractNcNomNewModifForm::updateTemp<Ent>(entity);
    setType(entity.type());
}
}
#endif // NEWMODIFDIALOG_H
