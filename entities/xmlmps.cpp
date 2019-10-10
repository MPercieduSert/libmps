#include "xmlmps.h"

//////////////////////////// XmlDoc/////////////
XmlDoc & XmlDoc::seek(const QString & chemin, bool newElt, bool root)
{
    if(root) begin();
    bool controle = true;
    QList<QString> list(chemin.split("/"));
    for(QList<QString>::const_iterator i = list.cbegin(); controle && i != list.cend(); ++i)
    {
        QList< TreeItem<XmlElement> *>::const_iterator j = childs().cbegin();
        while(j != childs().cend() && (*j)->data().name() != *i) ++j;
        if(j == childs().cend())
        {
            controle = false;
            if(newElt)
            {
                XmlElement elt;
                for(; i != list.cend(); ++i)
                {
                    elt.setName(*i);
                    addChild(elt);
                }
            }
            else
            {
                m_i = m_root;
            }
        }
        else
        {
            m_i = *j;
        }
    }
    return *this;
}

/////////////////////////////////////////XmlReader/////////////////////////////////////
bool XmlReader::exists(const QString & nameVar, const QMap<QString,QString> & atts)
{
    int n = nameVar.indexOf('/');

    if (n<0)
    {
        QMap<QString,QString>::const_iterator i;
        bool controle=false;
        while(readNextStartElement() && !controle)
        {
           if(name() == nameVar)
           {
               if(!atts.isEmpty())
               {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.constBegin();
                    while(i != atts.constEnd() && attribs.hasAttribute(i.key()) && attribs.value(i.key()) == i.value()) ++i;
                    controle = (i == atts.constEnd());
               }
               else
               {
                   controle = true;
               }
           }
            skipCurrentElement();
        }
        return controle;
    }
    else
    {
        QString dirVar = nameVar.left(n);
        QString pathVar = nameVar.right(nameVar.size()-n-1);

        while(readNextStartElement() && name() != dirVar) skipCurrentElement();
        if(name() == dirVar)
        {
            return exists(pathVar, atts);
        }
        else
        {
            return false;
        }
    }
}

void XmlReader::getVarsRec(const QString & nameVar, const QMap<QString,QString> & atts, QStringList & strList)
{
    int n = nameVar.indexOf('/');

    if (n<0)
    {
        QMap<QString,QString>::const_iterator i;
        while(readNextStartElement())
        {
            if(name() == nameVar)
            {
               if(!atts.isEmpty())
               {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.constBegin();
                    while(i != atts.constEnd() && attribs.hasAttribute(i.key()) && attribs.value(i.key()) == i.value()) ++i;
                    if(i == atts.constEnd())
                    {
                       strList.append(readElementText());
                    }
                    else
                    {
                        skipCurrentElement();
                    }
               }
               else
               {
                   strList.append(readElementText());
               }
            }
            else
            {
                skipCurrentElement();
            }
        }
    }
    else
    {
        QString dirVar = nameVar.left(n);
        QString pathVar = nameVar.right(nameVar.size()-n-1);

        while(readNextStartElement() && name() != dirVar) skipCurrentElement();
        if(name() == dirVar)
        {
           getVarsRec(pathVar, atts, strList);
        }
    }
}

void XmlReader::getVarsAttsRec(const QString & nameVar, const QMap<QString,QString> & atts, QVector<varAtts> & varsAtts)
{
    int n = nameVar.indexOf('/');

    if (n<0)
    {
        QMap<QString,QString>::const_iterator i;
        bool addVarAtts;
        while(readNextStartElement())
        {
            if(name() == nameVar)
            {
               if(!atts.isEmpty())
               {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.constBegin();
                    while(i != atts.constEnd() && attribs.hasAttribute(i.key()) && attribs.value(i.key()) == i.value()) ++i;
                    addVarAtts = i == atts.constEnd();
               }
               else
               {
                   addVarAtts = true;
               }

            }
            else
            {
                addVarAtts = false;

            }
            if(addVarAtts)
            {
                varAtts var;
                QXmlStreamAttributes atts = attributes();
                for(QXmlStreamAttributes::const_iterator j = atts.constBegin(); j != atts.constEnd(); ++j)
                    var.second.insert(j->name().toString(),j->value().toString());
                var.first = readElementText();
                varsAtts.append(var);

            }
            else
            {
                skipCurrentElement();
            }

        }
    }
    else
    {
        QString dirVar = nameVar.left(n);
        QString pathVar = nameVar.right(nameVar.size()-n-1);

        while(readNextStartElement() && name() != dirVar) skipCurrentElement();
        if(name() == dirVar)
        {
           getVarsAttsRec(pathVar, atts, varsAtts);
        }
    }
}

void XmlReader::readIn(XmlDoc & doc)
{
    doc.clear();
    while(!atEnd())
    {
        switch (readNext())
        {
        case QXmlStreamReader::StartElement:
            doc.addChild(readElement());
            break;
        case QXmlStreamReader::EndElement:
            doc.toParent();
            break;
        case QXmlStreamReader::Characters:
            doc.setText(text().toString());
            break;
        default:
            break;
        }
    }
}

XmlDoc XmlReader::read()
{
    XmlDoc doc;
    while(!atEnd())
    {
        switch (readNext())
        {
        case QXmlStreamReader::StartElement:
            doc.addChild(readElement());
            break;
        case QXmlStreamReader::EndElement:
            doc.toParent();
            break;
        case QXmlStreamReader::Characters:
            doc.setText(text().toString());
            break;
        default:
            break;
        }
    }
    return doc;
}

////////////////////////////////////////////////////XmlWriter//////////////////////////////////////
void XmlWriter::write(const XmlDoc & doc)
{
    setAutoFormatting(true);
    doc.begin();
    writeStartDocument();
    while(doc.suivant())
    {
        if(doc.sens())
        {
            XmlElement elt = doc.data();
            writeStartElement(elt.name());
            if(elt.hasAttributes())
            {
                for(QMap<QString,QString>::const_iterator i = elt.attributes().cbegin(); i != elt.attributes().cend(); ++i)
                {
                    writeAttribute(i.key(),i.value());
                }
            }
            if(elt.hasText())
            {
                writeCharacters(elt.text());
            }
        }
        else
        {
            writeEndElement();
        }
    }
    writeEndDocument();
}
