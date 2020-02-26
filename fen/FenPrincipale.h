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
    QAction *m_actionEditColler;    //!< Action coller.
    QAction *m_actionEditCopier;    //!< Action copier.
    QAction *m_actionEditCouper;    //!< Action couper.
    QAction *m_actionEditEffacer;   //!< Action effacer.
    QAction *m_actionSave;          //!< Action sauvegarder.

    // Menu
    QMenu *m_menuEdit;              //!< Menu édition.
    QMenu *m_menuFichier;           //!< Menu Fichier.

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

protected:
    //! Crée le menu Bdd.
    void addMenuBdd();

    //! Crée les différente action des menus et toolbars de la fenêtre principale.
    void createAction();

    //! Crée les menus de la fenêtre principale.
    void createMenu();

    //! Crée les toolbars de la fenêtre principale.
    void createToolBar();
};
}
#endif // FENPRINCIPALE_H
