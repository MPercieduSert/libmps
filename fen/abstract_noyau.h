/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef ABSTRACT_NOYAU_H
#define ABSTRACT_NOYAU_H

#include "abstract_tab.h"
#include "bdd.h"
#include "fen_found_file.h"

/*! \defgroup groupe_noyau Noyau
 *\brief Ensemble de classes regroupant les différentes fenêtres de l'application.
 */

namespace mps {
namespace fenetre {
/*! \ingroup groupe_noyau
 * \brief Noyau de l'application.
 */
class abstract_noyau : public QObject {
    Q_OBJECT
protected:
     std::unique_ptr<b2d::bdd> m_bdd;                 //!< Gestionnaire de la Base de donnée.
     std::unique_ptr<fichier::config> m_config;           //!< Gestionnaire de la configuration.

public:
    //! Constructeur.
    abstract_noyau() = default;

    //! Destructeur.
    ~abstract_noyau() = default;

    //! Renvoie un pointeur sur le gestionnaire de Base de donnée.
    virtual b2d::bdd &bdd()
        {return *m_bdd;}

    //! Renvoie un pointeur sur le gestionnaire de configuration.
    virtual fichier::config &config()
        {return *m_config;}

    //! Importe des entités dans la base de données à partir du fichier xml de chemin path.
    void import_xml(const QString &path);

    //! Exporte des entités dans un fichier xml.
    void export_xml(const QString &path, vector_ptr<entities::entity> &&vec, flag option);

    //! Écrit le schema xml des fichiers d'importation de données, dans le fichier de chemin path.
    void schema_xml_for_import(const QString &path);

    //!Ouvre le gestionnaire de Base de données.
    void set_bdd(std::unique_ptr<b2d::bdd> &&bdd, const QString &path_xml, QWidget *modal_parent = nullptr);

    //!Ouvre le gestionnaire de configuration.
    void set_config(std::unique_ptr<fichier::config> &&config, QWidget *modal_parent = nullptr);

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    virtual void set_config_by_path(const QString &config_path, QWidget *modal_parent = nullptr) = 0;

protected:
    //! Hydrate l'entité à partire d'un itérateur sur un docXml.
    bool hydrate_xml(entities::entity &ent, fichier::doc_xml::const_brother_iterator iter);
};
}}
#endif // ABSTRACT_NOYAU_H
