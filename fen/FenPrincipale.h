/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QAction>
#include <QComboBox>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QToolBar>
#include <QVBoxLayout>
#include <stdexcept>
#include "AbstractNoyau.h"
#include "AbstractZoneCentrale.h"
#include "FenFlags.h"
#include "NewModifDialog.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Fenêtre principale de l'application.
 *
 * Fenêtre principale:
 *  + Crée les différents menus.
 *
 */
class FenPrincipale : public QMainWindow {
    Q_OBJECT
protected:
    // Action
    QAction *m_chercherAction;  //!< Action chercher.
    QAction *m_collerAction;    //!< Action coller.
    QAction *m_copierAction;    //!< Action copier.
    QAction *m_couperAction;    //!< Action couper.
    QAction *m_effacerAction;   //!< Action effacer.
    QAction *m_sauverAction;    //!< Action sauvegarder.
    QAction *m_supprimerAction; //!< Action supprimer.

    // Menu
    QMenu *m_editMenu;              //!< Menu édition.
    QMenu *m_fichierMenu;           //!< Menu Fichier.
    QMenu *m_newModifMenu;          //!< Création modification.

    // Tools Bar
    QToolBar *m_toolBar;            //!< Barre d'outils.

    AbstractZoneCentrale *m_zoneCentrale;    //!< Zone centrale.

    // Noyau
    AbstractNoyau * m_noyau;                //!< Noyau de l'application.

public:
    //! Constructeur.
    explicit FenPrincipale(AbstractNoyau * noyau, std::unique_ptr<bddMPS::Bdd> &&bdd, AbstractZoneCentrale * centralZone,
                           const QString & bddPathXML,
                           const QString & configPath = QDir::currentPath().append("/Config.xml"),
                           QWidget *parent = nullptr);

    //! Destructeur.
    ~FenPrincipale() override = default;

    //! Accesseur du noyau.
    virtual AbstractNoyau * noyau()
        {return m_noyau;}

public slots:
    //! Mutateur des actions.
    void setAction(Action action);

protected:
    //! Crée le menu Bdd.
    void addMenuBdd();

    //! Connecte une QAction à l'ouverture d'un dialogue de création et modification.
    template<class From> void connectActionToNewModifDialog(QAction * action, bool newEnt);

    //! Crée les différente action des menus et toolbars de la fenêtre principale.
    void createAction();

    //! Crée les menus de la fenêtre principale.
    void createMenu();

    //! Crée les toolbars de la fenêtre principale.
    void createToolBar();
};

template<class Form> void FenPrincipale::connectActionToNewModifDialog(QAction * action, bool newEnt) {
    connect(action,&QAction::triggered,this, [this,newEnt](){
                            auto * form = new Form(noyau()->bdd(), newEnt);
                            dialogMPS::NewModifDialog diag(form,this);
                            diag.exec();
                        });
}
}
#endif // FENPRINCIPALE_H
