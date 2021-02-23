/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef ABSTRACTNOYAU_H
#define ABSTRACTNOYAU_H

#include "bdd.h"
/*#include "../Dialog/NewAnneeDialog.h"
#include "../Dialog/NewClasseDialog.h"
#include "../Dialog/NewEtablissementDialog.h"
#include "../Dialog/SelectAnneeDialog.h"*/
#include "AbstractTab.h"
#include "FenFoundFile.h"

/*! \defgroup groupeNoyau Noyau
 *\brief Ensemble de classes regroupant les différentes fenêtres de l'application.
 */

namespace fenMPS {
namespace b2d = b2d;
/*! \ingroup groupeNoyau
 *\brief Noyau de l'application.
 *
 *Noyau de l'application:
 *
 */
class AbstractNoyau : public QObject {
    Q_OBJECT
protected:
     std::unique_ptr<b2d::Bdd> m_bdd;                 //!< Gestionnaire de la Base de donnée.
     std::unique_ptr<fimps::Config> m_config;           //!< Gestionnaire de la configuration.

public:
    //! Constructeur.
    AbstractNoyau() = default;

    //! Destructeur.
    ~AbstractNoyau() = default;

    //! Renvoie un pointeur sur l'onglet actif.
    //TabAbstractModule *activeTab() const;

    //! Renvoie un pointeur sur le gestionnaire de Base de donnée.
    virtual b2d::Bdd &bdd()
        {return *m_bdd;}

    //! Renvoie un pointeur sur le gestionnaire de configuration.
    virtual fimps::Config &config()
        {return *m_config;}

    //! Importe des entités dans la base de données à partir du fichier xml de chemin path.
    void import_xml(const QString &path);

    //! Exporte des entités dans un fichier xml.
    void export_xml(const QString &path, vector_ptr<entities::entity> &&vec, flag option);

    //! Écrit le schema xml des fichiers d'importation de données, dans le fichier de chemin path.
    void schemaXmlForimport(const QString &path);

    //!Ouvre le gestionnaire de Base de données.
    void setBdd(std::unique_ptr<b2d::Bdd> &&bdd, const QString &pathXML, QWidget *modalParent = nullptr);

    //!Ouvre le gestionnaire de configuration.
    void setConfig(std::unique_ptr<fimps::Config> &&config, QWidget *modalParent = nullptr);

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    virtual void setConfigByPath(const QString &configPath, QWidget *modalParent = nullptr) = 0;

protected:
    //! Hydrate l'entité à partire d'un itérateur sur un docXml.
    bool hydrateXml(entities::entity &ent, fichier::doc_xml::const_brother_iterator iter);
};
}
#endif // ABSTRACTNOYAU_H
