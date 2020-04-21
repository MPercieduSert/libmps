/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/12/2019
 */
#ifndef PREDEFFENPRINCIPALE_H
#define PREDEFFENPRINCIPALE_H

#include <QFileDialog>
#include "PredefZoneCentrale.h"
#include "FenPrincipale.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Fenêtre principale de l'application avec de fonctionnalité prédéfinie.
 *
 * Fenêtre principale:
 *  + Crée le menu de gestion de la base de donées.
 *
 */
class PredefFenPrincipale : public FenPrincipale {
    Q_OBJECT
protected:
    // Menu
    QMenu *m_bddMenu;       //!< Menu de gestion de la base de données.
    QMenu *m_bddTableMenu;  //!< Menu des tables.
    QMenu *m_bddXmlMenu;    //!< Menu d'importation d'entité de la base de donnée au format xml.

public:
    //! Constructeur.
    explicit PredefFenPrincipale(AbstractNoyau * noyau, std::unique_ptr<bddMPS::Bdd> &&bdd, PredefZoneCentrale * centralZone,
                           const QString & bddPathXML,
                           const QString & configPath = QDir::currentPath().append("/Config.xml"),
                           QWidget *parent = nullptr);

    //! Destructeur.
    ~PredefFenPrincipale() override = default;

    //! Accesseur du pointeur de la zone centrale.
    PredefZoneCentrale * centraleZone()
        {return static_cast<PredefZoneCentrale *>(m_zoneCentrale);}

protected:
    //! Crée le menu Bdd.
    void addMenuBdd();

    //! Connecte une QAction à l'ouverture d'un onglet.
    void connectActionToOpenTab(QAction * action, const std::pair<int,int>& pairIndex,
                                const std::vector<QVariant> & vec = std::vector<QVariant>());
public slots:
    //! Importe des entités dans la base de à partir d'un fichier xml fourni par l'utilisateur.
    void importXml();

    //! Écrit le schema xml de validation des fichier d'importation de données.
    void schemaXml();
};
}
#endif // PREDEFFENPRINCIPALE_H
