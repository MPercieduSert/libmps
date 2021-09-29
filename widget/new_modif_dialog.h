/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/02/2020
 */
#ifndef NEW_MODIF_DIALOG_H
#define NEW_MODIF_DIALOG_H

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
#include "id_combo_box.h"
#include "tree_widget.h"

/*! \defgroup groupe_dialogue Fenêtre de Dialogue
 *\brief Ensemble de classes représentant des fenêtres de dialogue.
 */
namespace mps {
/*! \ingroup groupe_dialogue
 *\brief Espace de noms des fenêtres de dialogue.
 */
namespace dialogue {
/*! \ingroup groupe_dialogue
 *\brief Classe abstraite mère de formulaire pour les dialogues
 */
class abstract_new_modif_form : public QWidget {
    Q_OBJECT
protected:
    const bool m_new;               //! true->création et false->modification ou suppression.
    b2d::bdd &m_bdd;                //! Base de données.

public:
    //! Constructeur.
    abstract_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr)
        : QWidget(parent),
          m_new(new_ent),
          m_bdd(bdd) {}

    //! Destructeur par défaut.
    virtual ~abstract_new_modif_form();

    //! Connecte les signals et slots du formulaire.
    virtual void connexion() = 0;

    //! Paramètre l'absence du bouton supprimé.
    virtual bool del_disable() const noexcept
        {return m_new;}

    //! Accesseur de m_new.
    bool new_ent() const noexcept
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
    virtual void update_data() {}

signals:
    //! Signal d'activation/désactivation de l'action effacer.
    void effacer_permis(bool);

    //! Signal d'activation/désactivation de l'action d'enrgistrer.
    void save_permis(bool);
};

/*! \ingroup groupe_dialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom.
 */
class abstract_nom_new_modif_form : public abstract_new_modif_form {
    Q_OBJECT
protected:
    QLabel *m_name_label;               //!< Label du nom.
    QLineEdit *m_name_line;             //!< Ligne d'édition du nom.
    widget::id_combo_box *m_name_cb;   //!< Liste à choix du nom.
    QHBoxLayout *m_name_layout;         //!< Calque du nom.
    QVBoxLayout *m_main_layout;         //!< Calque principal du formulaire.
public:
    //! Constructeur.
    abstract_nom_new_modif_form(b2d::bdd &bdd, const QString &label, bool new_ent, QWidget *parent = nullptr);

    //! Constructeur.
    abstract_nom_new_modif_form(b2d::bdd &bdd, const QString &label, const std::vector<std::pair<idt,QString>> &noms,
                            bool new_ent, QWidget *parent = nullptr)
        : abstract_nom_new_modif_form (bdd,label,new_ent,parent)
        {set_noms(noms);}

    //! Destructeur par défaut.
    ~abstract_nom_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie l'identifiant du nom sélectionné.
    idt id() const
        {return m_new ? 0 : m_name_cb->id();}

    //! Renvoie le nom selectionné.
    QString nom() const
        {return m_new ? m_name_line->text() : m_name_cb->currentText();}

    //! Mutateur de la liste des noms.
    void set_noms(const std::vector<std::pair<idt,QString>> &noms) {
        if(!m_new){
            m_name_cb->clear();
            m_name_cb->add_text(noms);
        }
    }

    //! Mutateur de la liste des noms.
    template<class Ent> void set_noms(const vector_ptr<Ent> &vec) {
        if(!m_new){
            m_name_cb->clear();
            m_name_cb->add_text(vec,[](const Ent &ent)->const QString &{return ent.nom();});
        }
    }

    //! Mutateur de la liste des noms.
    template<class Ent> void set_noms(vector_ptr<Ent> &&vec) {
        if(!m_new){
            m_name_cb->clear();
            m_name_cb->add_text(std::move(vec),[](const Ent &ent)->const QString &{return ent.nom();});
        }
    }

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return m_new ? !m_name_line->text().isEmpty() : !m_name_cb->currentText().isEmpty();}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupe_dialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom et le nom cours.
 */
class abstract_nc_nom_new_modif_form : public abstract_nom_new_modif_form {
    Q_OBJECT
protected:
    using abstract_nom_new_modif_form::m_main_layout;
    QLabel *m_nc_label;               //!< Label du nom court.
    QLineEdit *m_nc_line;             //!< Ligne d'édition du nom court.
    QHBoxLayout *m_nc_layout;         //!< Calque du nom court.
public:
    //! Constructeur.
    abstract_nc_nom_new_modif_form(b2d::bdd &bdd, const QString &lableNc, const QString &labelNom, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_nc_nom_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie le nom court selectionné.
    QString nc() const
        {return m_nc_line->text();}

    //! Mutateur du nom cours.$
    void set_nc(const QString &nc)
        {m_nc_line->setText(nc);}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return abstract_nom_new_modif_form::valide() &&!m_nc_line->text().isEmpty();}
protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupe_dialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom, le nom court et le type.
 */
class abstract_type_nc_nom_new_modif_form : public abstract_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    using type = entities::ensemble_type::type;
    using Permission = entities::ensemble_type::type_permission;
    using abstract_nc_nom_new_modif_form::m_main_layout;
    int m_cible;                        //!< Numero de cible de l'entité
    bool m_valide;                      //!< Controle de la validité du type
    QLabel *m_type_label;               //!< Label du nom
    widget::tree_widget *m_type_tree; //!< Séléction du type
public:
    enum {nom_type,ncType,nbrColumn};
    //! Constructeur.
    abstract_type_nc_nom_new_modif_form(b2d::bdd_predef &bdd, const QString &ref_root, idt id_entity, const QString &labelType,
                                  const QString &labelNc, const QString &labelNom, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_type_nc_nom_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt id_type() const
        {return m_type_tree->id();}

    //! Mutateur du type.
    void set_type(idt id_type)
        {m_type_tree->set_id(id_type);}

    //! Teste si le formulaire est valide.
    bool valide() const override
        {return abstract_nc_nom_new_modif_form::valide() &&m_valide;}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupe_dialogue
 *\brief Classe des formulaires pour les dialogues commençant par le choix du nom, le nom court et du parent.
 */
class abstract_parent_nc_nom_new_modif_form : public abstract_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    using abstract_nc_nom_new_modif_form::m_main_layout;
    QLabel *m_parent_label;             //!< Label du parent
    widget::tree_widget *m_parent_tree;         //!< Ligne d'édition du nom.
public:
    enum {nomParent,ncParent,nbrColumn};
    //! Constructeur.
    abstract_parent_nc_nom_new_modif_form(b2d::bdd &bdd, const QStringList &header, const QString &parentType,
                                  const QString &labelNc, const QString &labelNom, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_parent_nc_nom_new_modif_form() override = default;

    //! Renvoie l'identifiant du parent sélectionné.
    idt id_parent() const
        {return m_parent_tree->id();}

    //! Mutateur du type.
    void set_parent(idt id_parent)
        {m_parent_tree->set_id(id_parent);}

protected:
    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_temp(Ent &ent);
};

/*! \ingroup groupe_dialogue
 *\brief Fenêtre de dialogue de création, modification ou suppression.
 */
class new_modif_dialog : public QDialog {
    Q_OBJECT
protected:
    QPushButton *m_annul_bouton;                //!< Bouton annuler.
    QPushButton *m_ok_bouton;                   //!< Bouton Ok.
    QPushButton *m_suppr_bouton;                //!< Bouton Suppr
    QHBoxLayout *m_bouton_layout;               //!< Calque des boutons.
    QVBoxLayout *m_main_layout;               //!< Calque principal.
    abstract_new_modif_form *m_form;                      //!< Formulaire.
public:
    //! Constructeur.
    new_modif_dialog(abstract_new_modif_form *form, QWidget *parent = nullptr);

    //! Destructeur.
    ~new_modif_dialog() override;

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

template<class Ent> void abstract_nom_new_modif_form::update_temp(Ent &ent) {
    ent.set_id(id());
    m_bdd.get(ent);
    m_name_cb->setEditable(m_bdd.test_autorisation(ent,b2d::Modif));
    emit effacer_permis(m_bdd.test_autorisation(ent,b2d::Suppr));
}

template<class Ent> void abstract_nc_nom_new_modif_form::update_temp(Ent &ent) {
    abstract_nom_new_modif_form::update_temp<Ent>(ent);
    m_nc_line->setReadOnly(!m_bdd.test_autorisation(ent,b2d::Modif));
    set_nc(ent.nc());
}

template<class Ent> void abstract_type_nc_nom_new_modif_form::update_temp(Ent &ent) {
    abstract_nc_nom_new_modif_form::update_temp<Ent>(ent);
    set_type(ent.type());
}

template<class Ent> void abstract_parent_nc_nom_new_modif_form::update_temp(Ent &ent) {
    abstract_nc_nom_new_modif_form::update_temp<Ent>(ent);
    set_parent(ent.parent());
}
}}
#endif // NEW_MODIF_DIALOG_H
