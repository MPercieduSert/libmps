/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */

#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

#include <QFile>
#include <QString>

/*! \defgroup groupeFile Gestionnaire de fichier
 * \brief Ensemble des gestionnaires de fichier.
 */

/*! \ingroup groupeFile
 * \brief Interface pour la gestion des fichiers.
 *
 * Interface pour la gestion des fichiers, cette interface est utilisée par la classe FenFoundFile
 * pour la vérifiaction des chemins des fichiers et leur création s'il sont introuvables.
 */
class FileInterface
{
protected:
    QString m_fileName;         //!< Chemin du fichier.
    QString m_fileExtension;    //!< Entension du fichier.

public:
    //! Constructeur. Demande en argument le chemin du fichier et son extension.
    explicit FileInterface(const QString & fileName, const QString & fileExtension)
        : m_fileName(fileName),
          m_fileExtension(fileExtension)
        {}

    //! Destructeur.
    virtual ~FileInterface();

    /*! \brief Méthode abstraite créant une copie du fichier de chemin name à l'emplacement m_fileName.
     * Retourne true si la copie s'est correctement déroulée et false sinon.
     */
    virtual bool copy(const QString & name) = 0;

    /*! \brief Méthode abstraite créant un fichier à l'emplacement m_fileName.
     * Retourne true si la création du fichier s'est correctement déroulée et false sinon.
     */
    virtual bool creer() = 0;

    //! Méthode abstraite testant si le fichier existe.
    virtual bool exists() const = 0;

    //! Renvoie l'extension du fichier.
    const QString fileExtension() const
        {return m_fileExtension;}

    //! Renvoie le chemin du fichier.
    const QString fileName() const
        {return m_fileName;}

    //! Teste si le fichier est valide.
    virtual bool isValid() = 0;

    //! Ouvre le fichier et retourne true si l'ouverture du fichier s'est correctement déroulée et false sinon.
    virtual bool open() = 0;

    //! Modifie le chemin du fichier.
    virtual void setFileName(const QString & fileName)
        {m_fileName = fileName;}
};

#endif // FILEINTERFACE_H
