#include "XmlMps.h"

using namespace fichierMPS;

//////////////////////////// XmlDoc/////////////
conteneurMPS::tree<XmlElement>::prevsuiv_iterator XmlDoc::seek(const QString & chemin) {
    auto iter = beginPrevsuiv();
    auto controle = true;
    auto list = chemin.split("/");
    auto i = list.cbegin();
    if(iter->name() == *i) {
        ++i;
        while(controle && i != list.cend()) {
            auto j = iter.cbeginChild();
            while(j && j->name() != *i)
                ++j;
            if(j)
                iter = j;
            else {
                controle = false;
                while(i != list.cend()) {
                    iter = push_back(iter,XmlElement(*i));
                    ++i;
                }
            }
            ++i;
        }
    }
    else
        iter.toParent();
    return iter;
}

QString XmlDoc::toString() const
{
    QString string;
    QString tab(">");
    auto iter = cbeginPrevsuiv();
    while(iter) {
        if(iter.sens()) {
            auto elt = *iter;
            string.append(tab).append(elt.name()).append("\n");
            tab.prepend("-");
        }
        else
            tab.remove(0,1);
        ++iter;
    }
    return string;
}

/////////////////////////////////////////XmlReader/////////////////////////////////////
bool XmlReader::exists(QString nameVar, const std::map<QString,QString> & atts) {
    auto n = nameVar.indexOf('/');
    if(n >= 0){
        while (n >= 0) {
            auto dirVar = nameVar.left(n);
            nameVar = nameVar.right(nameVar.size()-n-1);
            while(readNextStartElement() && name() != dirVar)
                skipCurrentElement();
            if(name() == dirVar)
                n = nameVar.indexOf('/');
            else
                n = -2;
        }
    }
    if (n <= -2)
        return false;
    else {
        std::map<QString,QString>::const_iterator i;
        bool controle=false;
        while(readNextStartElement() && !controle) {
           if(name() == nameVar) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.cbegin();
                    while(i != atts.cend() && attribs.hasAttribute(i->first) && attribs.value(i->first) == i->second) 
                        ++i;
                    controle = (i == atts.cend());
               }
               else
                   controle = true;
           }
            skipCurrentElement();
        }
        return controle;
    }
}

std::list<QString> XmlReader::getVars(QString nameVar, const std::map<QString,QString> & atts) {
    std::list<QString> strList;
    auto n = nameVar.indexOf('/');
    if(n >= 0){
        while (n >= 0) {
            auto dirVar = nameVar.left(n);
            nameVar = nameVar.right(nameVar.size()-n-1);
            while(readNextStartElement() && name() != dirVar)
                skipCurrentElement();
            if(name() == dirVar)
                n = nameVar.indexOf('/');
            else
                n = -2;
        }
    }
    if(n <= -2)
        return strList;
    else {
        std::map<QString,QString>::const_iterator i;
        while(readNextStartElement()) {
            if(name() == nameVar) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.cbegin();
                    while(i != atts.cend() && attribs.hasAttribute(i->first) && attribs.value(i->first) == i->second)
                        ++i;
                    if(i == atts.cend())
                       strList.push_back(readElementText());
                    else
                        skipCurrentElement();
               }
               else
                   strList.push_back(readElementText());
            }
            else
                skipCurrentElement();
        }
        return strList;
    }
}

std::vector<varAtts> XmlReader::getVarsAtts(QString nameVar, const std::map<QString,QString> & atts) {
    std::vector<varAtts> varsAtts;
    auto n = nameVar.indexOf('/');
    if(n >= 0){
        while (n >= 0) {
            auto dirVar = nameVar.left(n);
            nameVar = nameVar.right(nameVar.size()-n-1);
            while(readNextStartElement() && name() != dirVar)
                skipCurrentElement();
            if(name() == dirVar)
                n = nameVar.indexOf('/');
            else
                n = -2;
        }
    }
    if(n <= -2)
        return varsAtts;
    else {
        std::map<QString,QString>::const_iterator i;
        bool addVarAtts;
        while(readNextStartElement()) {
            if(name() == nameVar) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    i = atts.cbegin();
                    while(i != atts.cend() && attribs.hasAttribute(i->first) && attribs.value(i->first) == i->second)
                        ++i;
                    addVarAtts = i == atts.cend();
               }
               else
                   addVarAtts = true;
            }
            else
                addVarAtts = false;
            if(addVarAtts) {
                varAtts var;
                QXmlStreamAttributes atts = attributes();
                for(QXmlStreamAttributes::const_iterator j = atts.constBegin(); j != atts.constEnd(); ++j)
                    var.second.insert({j->name().toString(),j->value().toString()});
                var.first = readElementText();
                varsAtts.push_back(var);
            }
            else
                skipCurrentElement();
        }
        return varsAtts;
    }
}

void XmlReader::readIn(XmlDoc & doc) {
    doc.clear();
    auto iter = doc.begin();
    while(!atEnd() && readNext() != QXmlStreamReader::StartElement) ;
    if(!atEnd())
        *iter = readElement();

    while(!atEnd()) {
        switch (readNext()) {
        case QXmlStreamReader::StartElement:
            iter = doc.push_back(iter, readElement());
            break;
        case QXmlStreamReader::EndElement:
            iter.toParent();
            break;
        case QXmlStreamReader::Characters:
            iter->setText(text().toString());
            break;
        default:
            break;
        }
    }
}

XmlDoc XmlReader::read() {
    XmlDoc doc;
    readIn(doc);
    return doc;
}

////////////////////////////////////////////////////XmlWriter//////////////////////////////////////
void XmlWriter::write(const XmlDoc & doc) {
    setAutoFormatting(true);
    auto iter = doc.cbeginPrevsuiv();
    writeStartDocument();
    while(iter) {
        if(iter.sens()) {
            auto elt = *iter;
            writeStartElement(elt.name());
            if(elt.hasAttributes())
                for(auto i = elt.attributes().cbegin(); i != elt.attributes().cend(); ++i)
                    writeAttribute(i->first,i->second);
            if(elt.hasText())
                writeCharacters(elt.text());
        }
        else
            writeEndElement();
        ++iter;
    }
    writeEndDocument();
}
