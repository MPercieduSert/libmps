/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef XMLMPS_H
#define XMLMPS_H

/*! \defgroup groupeXML XML
 *
 * \brief Ensemble de classes utiliser pour la manipulation de fichiers XML.
 */

//#include <QList>
//#include <std::map>
#include <QString>
#include <map>
#include <vector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "Tree.h"

namespace fichierMPS {

/*! \var typedef std::pair<QString,std::map<QString,QString> > varAtts;
 * \brief Couple (Valeur,Attribut sous forme liste<clé,valeur>) d'un noeud
 */
using varAtts = std::pair<QString,std::map<QString,QString>>;

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
    std::map<QString,QString> m_attributes; //!< Attributs XML sous la forme <clé,valeur> attachés au noeud.

public:
    //! Constructeur.
    XmlElement()
        {}

    //! Constructeur.
    XmlElement(const QString & name)
        {m_name = name;}

    //! Constructeur.
    XmlElement(const char * name)
        {m_name = name;}

    //! Renvoie les attributs du noeud sous la forme d'un map<clé,valeur>.
    const std::map<QString,QString> & attributes() const
        {return m_attributes;}

    //! Test si le noeud posséde des attributs.
    bool hasAttributes() const
        {return !m_attributes.empty();}

    //! Test si le noeud contient du texte.
    bool hasText() const
        {return !m_text.isEmpty();}

    //! Renvoie le nom de du noeud.
    const QString & name() const
        {return m_name;}

    //! Ajoute ou modifie l'attribut de nom name avec la valeur val.
    void setAttribut(const QString & name, const QString & val)
        {m_attributes[name] = val;}

    //! Remplace les attributs du noeud par ceux donnés en argument sous la forme d'un std::map<clé,valeur>.
    void setAttributes(const std::map<QString,QString> & map)
        {m_attributes = map;}

    //! Modifie le nom du noeud.
    void setName(const QString & name)
        {m_name = name;}

    //! Modifie le texte du noeud courant.
    void setText(const QString & text)
        {m_text = text;}

    //! Renvoie le texte du noeud.
    const QString & text() const
        {return m_text;}
};

/*! \ingroup groupeXML
 * \brief  Cette classe est un arbre de XmlElement permettant de manipuler un document XML.
 *
 * Le document XML est répresenté par la classe XmlDoc sous la forme un arbre de XmlElement.
 * La navigation dans l'arbre s'effectue à l'aide d'un noeud courant tel un curseur.
 * Voir la classe Tree pour connaitre la manière de parcourir l'arbre, de modifier ou de créer des noeuds dans l'arbre.
 *  - Un XmlDoc peut-être obtenu à partir d'un fichier XML grâce à XmlReader.
 *  - Un XmlDoc peut-être écrit dans un fichier XML grâce à XmlWriter.
 * \sa tree, XmlElement, XmlReader, XmlWriter
 */
class XmlDoc : public conteneurMPS::tree<XmlElement>
{
protected :

public:
    //! Constructeur.
    XmlDoc() = default;

//    //! Renvoie les attributs du noeud courant sous la forme d'un std::map<clé,valeur>.
//    const std::map<QString, QString> & attributes() const
//        {return (*m_i)->data().attributes();}

//    //! Renvoie le nom du noeud courant.
//    const QString & name() const
//        {return (*m_i)->data().name();}

//    //! Renvoie le texte du noeud courant.
//    const QString & text() const
//        {return (*m_i)->data().text();}
    
    //! Renvoie un itérateur sur le noeud de chemin indiqué.
    /*! L'itérateur pointe sur l'élément correspond au chemin indiqué dont les différents noeuds sont
     * séparés par un /. Si le premier terme du chemin ne coorespond pas à la racine, l'itérteur renvoyé est nul
     * dans le cas contraire, si les noeuds du chemin n'existent pas il sont créés.
     */
    prevsuiv_iterator seek(const QString & chemin);

    //! Revoie l'arbre dévelopé sous forme d'un QString.
    QString toString() const;

//    //! Remplace les attributs du noeud courant par ceux donnés en argument sous la forme d'un std::map<clé,valeur>.
//    void setAttributes(const std::map<QString, QString> & atts)
//        {(*m_i)->modifData().setAttributes(atts);}

//    //! Modifie le nom du noeud courant.
//    void setName(const QString & name)
//        {(*m_i)->modifData().setName(name);}

//    //! Modifie le texte du noeud courant.
//    void setText(const QString & text)
//        {(*m_i)->modifData().setText(text);}
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
    using QXmlStreamReader::QXmlStreamReader;

    //! Test si le noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue existe.
    bool exists(const QString & name, const QString & attName, const QString & attValue) {
        std::map<QString,QString> atts;
        atts.insert({attName,attValue});
        return exists(name, atts);
    }

    //! Test si le noeuds dont le chemin est donné par name et possédant les attributs atts donné sous forme d'un std::map<clé,valeur> existe.
    bool exists(QString name, const std::map<QString,QString> & atts = std::map<QString,QString>());

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue.
    std::list<QString> getVars(const QString & name, const QString & attName, const QString & attValue) {
        std::map<QString,QString> atts;
        atts.insert({attName,attValue});
        return getVars(name, atts);
    }

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name 
    //! et possédant les attributs atts donné sous forme d'un std::map<clé,valeur>.
    std::list<QString> getVars(QString name, const std::map<QString,QString> & atts = std::map<QString,QString>());

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name 
    //! et possédant l'attribut attName de valeur attValue.
    std::vector<varAtts> getVarsAtts(const QString & name, const QString & attName, const QString & attValue) {
        std::map<QString,QString> atts;
        atts.insert({attName,attValue});
        return getVarsAtts(name, atts);
    }

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name 
    //! et possédant les attributs atts donné sous forme d'un std::map<clé,valeur>.
    std::vector<varAtts> getVarsAtts(QString name, const std::map<QString,QString> & atts = std::map<QString,QString>());

    //! Lit entièrement le fichier Xml et stoke dans l'arbre XmlDoc transmis en argument.
    void readIn(XmlDoc & doc);

    //! Lit entièrement le fichier Xml puis renvoie un arbre XmlDoc.
    XmlDoc read();

    //! Lit le noeud courant du flux QXmlStreamReader puis retourne un XmlElement contenant les informations du noeud.
    XmlElement readElement() {
        XmlElement element;
        element.setName(name().toString());
        std::map<QString,QString> map;
        QXmlStreamAttributes atts = attributes();
        for(auto j = atts.constBegin(); j != atts.constEnd(); ++j)
            map.insert({j->name().toString(),j->value().toString()});
        element.setAttributes(map);
        return element;
    }
protected:
    //! fonction récurrsive à la base de la fonction getVars
    //void getVarsRec(const QString & name, const std::map<QString,QString> & atts, std::list<QString> & strList);

    //! fonction récurrsive à la base de la fonction getVarsAtts
    //void getVarsAttsRec(const QString & name, const std::map<QString,QString> & atts, std::vector<varAtts> & varsAtts);

};

/*! \ingroup groupeXML
 * \brief Cette classe permet d'écrire un document XML à partir d'un arbre XmlDoc.
 */
class XmlWriter : public QXmlStreamWriter
{
public:
    using QXmlStreamWriter::QXmlStreamWriter;

    //! Fonction écrit l'arbre XmlDoc transmis en argument dans le fichier attaché au flux QXmlStreamWriter.
    void write(const XmlDoc & doc);
};
}
#endif // XMLMPS_H
