/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef ABSTRACTNOYAU_H
#define ABSTRACTNOYAU_H

#include "Bdd.h"
/*#include "../Dialog/NewAnneeDialog.h"
#include "../Dialog/NewClasseDialog.h"
#include "../Dialog/NewEtablissementDialog.h"
#include "../Dialog/SelectAnneeDialog.h"*/
#include "AbstractTab.h"
#include "FenFoundFile.h"


/*! \defgroup groupeNoyau Noyau
 * \brief Ensemble de classes regroupant les différentes fenêtres de l'application.
 */

/*! \ingroup groupeNoyau
 * \brief Chemin XML du dossier de l'application par default.
 */
#define DEFAULT_DIRECTORY "conf/directories/default"

/*! \ingroup groupeNoyau
 * \brief Chemin XML de la base de donnée.
 */
#define DATA_BASE "conf/files/databases/database"

/*! \ingroup groupeNoyau
 * \brief Chemin XML de l'année par default.
 */
#define DEFAULT_ANNEE "conf/parametre/initialisation/anneeDefaut"

namespace fenMPS {
namespace bmps = bddMPS;
/*! \ingroup groupeNoyau
 * \brief Noyau de l'application.
 *
 * Noyau de l'application:
 *
 */
class AbstractNoyau : public QObject {
    Q_OBJECT
protected:
     std::unique_ptr<bmps::Bdd> m_bdd;                 //!< Gestionnaire de la Base de donnée.
     std::unique_ptr<fimps::Config> m_config;           //!< Gestionnaire de la configuration.

public:
    //! Constructeur.
    AbstractNoyau() = default;

    //! Destructeur.
    ~AbstractNoyau() = default;

    //! Renvoie un pointeur sur l'onglet actif.
    //TabAbstractModule * activeTab() const;

    //! Renvoie un pointeur sur le gestionnaire de Base de donnée.
    bmps::Bdd & bdd()
        {return *m_bdd;}

    //! Renvoie un pointeur sur le gestionnaire de configuration.
    fimps::Config & config()
        {return *m_config;}

    //!Ouvre le gestionnaire de Base de données.
    void setBdd(std::unique_ptr<bmps::Bdd> && bdd, const QString & pathXML, QWidget * modalParent = nullptr);

    //!Ouvre le gestionnaire de configuration.
    void setConfig(std::unique_ptr<fimps::Config> && config, QWidget * modalParent = nullptr);

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    virtual void setConfigByPath(const QString & configPath, QWidget * modalParent = nullptr) = 0;

signals:
    //! Signal d'activation/désactivation de l'action effacer.
    void effacerPermis(bool);

    //! Signal d'activation/désactivation de l'action coller.
    void collerPermis(bool);

    //! Signal d'activation/désactivation de l'action copier.
    void copierPermis(bool);

    //! Signal d'activation/désactivation de l'action couper.
    void couperPermis(bool);

    //! Signal d'activation/désactivation de l'action saugarder.
    void savePermis(bool);
};
}
#endif // ABSTRACTNOYAU_H
