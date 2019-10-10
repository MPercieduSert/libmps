/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef XMLMPS_H
#define XMLMPS_H

/*! \defgroup groupeXML XML
 *
 * \brief Ensemble de classes utiliser pour la manipulation de fichiers XML.
 */

#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "Tree.h"

/*! \var typedef QPair<QString,QMap<QString,QString> > varAtts;
 * \brief Couple (Valeur,Attribut sous forme liste<clé,valeur>) d'un noeud
 */
typedef QPair<QString,QMap<QString,QString> > varAtts;

/*! \ingroup groupeXML
 * \brief Cette classe est un noeud d'un document XML.
 *
 * Cette classe est un noeud d'arbre permettant de manipuler un document XML avec la classe XmlDoc.
 * \sa XmlDoc
 */
class XmlElement
{
protected:
    QString m_name; //!< Nom du noeuds XML.
    QString m_text; //!< Texte contenue entre les deux balises du noeuds XML.
    QMap<QString,QString> m_attributes; //!< Attributs XML sous la forme <clé,valeur> attachés au noeud.

public:
    //! Constructeur.
    XmlElement()
        {}

    //! Constructeur.
    XmlElement(const QString & name)
        {m_name = name;}

    //! Renvoie le nom de du noeud.
    const QString & name() const
        {return m_name;}

    //! Renvoie le texte du noeud.
    const QString & text() const
        {return m_text;}

    //! Renvoie les attributs du noeud sous la forme d'un QMap<clé,valeur>.
    const QMap<QString,QString> & attributes() const
        {return m_attributes;}

    //! Test si le noeud posséde des attributs.
    bool hasAttributes() const
        {return !m_attributes.isEmpty();}

    //! Test si le noeud contient du texte.
    bool hasText() const
        {return !m_text.isEmpty();}

    //! Modifie le nom du noeud.
    void setName(const QString & name)
        {m_name = name;}

    //! Modifie le texte du noeud courant.
    void setText(const QString & text)
        {m_text = text;}

    //! Remplace les attributs du noeud par ceux donnés en argument sous la forme d'un QMap<clé,valeur>.
    void setAttributes(const QMap<QString,QString> & map)
        {m_attributes = map;}
};

/*! \ingroup groupeXML
 * \brief  Cette classe est un arbre de XmlElement permettant de manipuler un document XML.
 *
 * Le document XML est répresenté par la classe XmlDoc sous la forme un arbre de XmlElement.
 * La navigation dans l'arbre s'effectue à l'aide d'un noeud courant tel un curseur.
 * Voir la classe Tree pour connaitre la manière de parcourir l'arbre, de modifier ou de créer des noeuds dans l'arbre.
 *  - Un XmlDoc peut-être obtenu à partir d'un fichier XML grâce à XmlReader.
 *  - Un XmlDoc peut-être écrit dans un fichier XML grâce à XmlWriter.
 * \sa Tree, XmlElement, XmlReader, XmlWriter
 */
class XmlDoc : public Tree<XmlElement>
{
protected :

public:
    //! Constructeur.
    XmlDoc()
        {}

    //! Renvoie les attributs du noeud courant sous la forme d'un QMap<clé,valeur>.
    const QMap<QString, QString> & attributes() const
        {return (*m_i)->data().attributes();}

    //! Renvoie le nom du noeud courant.
    const QString & name() const
        {return (*m_i)->data().name();}

    //! Renvoie le texte du noeud courant.
    const QString & text() const
        {return (*m_i)->data().text();}

    //! Déplace le noeud courant de navigation au noeud indiqué.
    /*! Le noeud courant de la navigation devient l'élément correspond au chemin indiqué dont les différents noeuds sont
     * séparés par un /. Par défaut, si les noeuds du chemin n'existent pas il sont créés, prendre newElt=false pour modifier ce
     * comportement. Par défaut, le chemin commence à la racine de l'arbre, prendre root=false pour modifier ce
     * comportement.
     */
    XmlDoc & seek(const QString & chemin, bool newElt = true, bool root = true);

    //! Remplace les attributs du noeud courant par ceux donnés en argument sous la forme d'un QMap<clé,valeur>.
    void setAttributes(const QMap<QString, QString> & atts)
        {(*m_i)->modifData().setAttributes(atts);}

    //! Modifie le nom du noeud courant.
    void setName(const QString & name)
        {(*m_i)->modifData().setName(name);}

    //! Modifie le texte du noeud courant.
    void setText(const QString & text)
        {(*m_i)->modifData().setText(text);}
};

/*! \ingroup groupeXML
 * \brief Cette classe permet de lire un document XML.
*
* Cette classe donne accés en lecture aux noeuds d'un document XML ou permet d'obtenir l'arbre des noeuds sous la forme d'un XmlDoc.
* Tous les chemins sont relatifs.
* \sa XmlElement, XmlDoc, XmlReader
*/
class XmlReader : public QXmlStreamReader
{
public:
    //! Constructeur
    XmlReader():QXmlStreamReader()
        {}

    //! Constructeur
    XmlReader(QIODevice *device):QXmlStreamReader(device)
        {}

    //! Test si le noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue existe.
    bool exists(const QString & name, const QString & attName, const QString & attValue)
    {
        QMap<QString,QString> atts;
        atts.insert(attName,attValue);
        return exists(name, atts);
    }

    //! Test si le noeuds dont le chemin est donné par name et possédant les attributs atts donné sous forme d'un QMap<clé,valeur> existe.
    bool exists(const QString & name, const QMap<QString,QString> & atts = QMap<QString,QString>());

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue.
    QStringList getVars(const QString & name, const QString & attName, const QString & attValue)
    {
        QMap<QString,QString> atts;
        atts.insert(attName,attValue);
        return getVars(name, atts);
    }

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name et possédant les attributs atts donné sous forme d'un QMap<clé,valeur>.
    QStringList getVars(const QString & name, const QMap<QString,QString> & atts = QMap<QString,QString>())
    {
        QStringList strList;
        getVarsRec(name, atts, strList);
        return strList;
    }

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue.
    QVector<varAtts> getVarsAtts(const QString & name, const QString & attName, const QString & attValue)
    {
        QMap<QString,QString> atts;
        atts.insert(attName,attValue);
        return getVarsAtts(name, atts);
    }

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name et possédant les attributs atts donné sous forme d'un QMap<clé,valeur>.
    QVector<varAtts> getVarsAtts(const QString & name, const QMap<QString,QString> & atts = QMap<QString,QString>())
    {
        QVector<varAtts> varsAtts;
        getVarsAttsRec(name, atts, varsAtts);
        return varsAtts;
    }

    //! Lit entièrement le fichier Xml et stoke dans l'arbre XmlDoc transmis en argument.
    void readIn(XmlDoc & doc);

    //! Lit entièrement le fichier Xml puis renvoie un arbre XmlDoc.
    XmlDoc read();

    //! Lit le noeud courant du flux QXmlStreamReader puis retourne un XmlElement contenant les informations du noeud.
    XmlElement readElement()
    {XmlElement element;
        element.setName(name().toString());
        QMap<QString,QString> map;
        QXmlStreamAttributes atts = attributes();
        for(QXmlStreamAttributes::const_iterator j = atts.constBegin(); j != atts.constEnd(); ++j)
            map.insert(j->name().toString(),j->value().toString());
        element.setAttributes(map);
        return element;
    }

protected:
    //! fonction récurrsive à la base de la fonction getVars
    void getVarsRec(const QString & name, const QMap<QString,QString> & atts, QStringList & strList);

    //! fonction récurrsive à la base de la fonction getVarsAtts
    void getVarsAttsRec(const QString & name, const QMap<QString,QString> & atts, QVector<varAtts> & varsAtts);

};

/*! \ingroup groupeXML
 * \brief Cette classe permet d'écrire un document XML à partir d'un arbre XmlDoc.
 */
class XmlWriter : public QXmlStreamWriter
{
public:
    //! Constructeur.
    XmlWriter():QXmlStreamWriter()
        {}

    //!Constructeur.
    XmlWriter(QIODevice *device):QXmlStreamWriter(device)
        {}

    //! Fonction écrit l'arbre XmlDoc transmis en argument dans le fichier attaché au flux QXmlStreamWriter.
    void write(const XmlDoc & doc);
};

#endif // XMLMPS_H
