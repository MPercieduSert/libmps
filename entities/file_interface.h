/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */

#ifndef FILE_INTERFACE_H
#define FILE_INTERFACE_H

#include <QFile>
#include <QString>

/*! \defgroup groupeFile Gestionnaire de fichier
 *\brief Ensemble des gestionnaires de fichier.
 */

namespace mps {
/*! \ingroup groupeFile
 *\brief Espace de noms de gestion des fichiers.
 */
namespace fichier {

/*! \ingroup groupeFile
 *\brief Interface pour la gestion des fichiers.
 *
 *Interface pour la gestion des fichiers, cette interface est utilisée par la classe FenFoundFile
 *pour la vérifiaction des chemins des fichiers et leur création s'il sont introuvables.
 */
class file_interface {
protected:
    QString m_name;         //!< Chemin du fichier.
    QString m_extension;    //!< Entension du fichier.

public:
    //! Constructeur. Demande en argument le chemin du fichier et son extension.
    explicit file_interface(const QString &name, const QString &extension)
        : m_name(name),
          m_extension(extension) {}

    //! Destructeur.
    virtual ~file_interface();

    /*! \brief Méthode abstraite créant une copie du fichier de chemin name à l'emplacement m_name.
     *Retourne true si la copie s'est correctement déroulée et false sinon.
     */
    virtual bool copy(const QString &name) = 0;

    /*! \brief Méthode abstraite créant un fichier à l'emplacement m_name.
     *Retourne true si la création du fichier s'est correctement déroulée et false sinon.
     */
    virtual bool creer() = 0;

    //! Méthode abstraite testant si le fichier existe.
    virtual bool exists() const = 0;

    //! Renvoie l'extension du fichier.
    const QString extension() const
        {return m_extension;}

    //! Renvoie le chemin du fichier.
    const QString name() const
        {return m_name;}

    //! Teste si le fichier est valide.
    virtual bool is_valid() = 0;

    //! Ouvre le fichier et retourne true si l'ouverture du fichier s'est correctement déroulée et false sinon.
    virtual bool open() = 0;

    //! Modifie le chemin du fichier.
    virtual void set_name(const QString &name)
        {m_name = name;}
};
}}
#endif // FILEINTERFACE_H
