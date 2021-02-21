#include "xml.h"

using namespace mps::fichier;

//////////////////////////// doc_xml/////////////
doc_xml::prevsuiv_iterator doc_xml::seek(const QString &chemin) {
    auto it_doc = begin_prevsuiv();
    auto controle = true;
    auto list = chemin.split("/");
    auto it_list = list.cbegin();
    if(it_doc->name() == *it_list) {
        ++it_list;
        while(controle &&it_list != list.cend()) {
            auto it_find = it_doc.cbegin_child();
            while(it_find &&it_find->name() != *it_list)
                ++it_find;
            if(it_find)
                it_doc = it_find;
            else {
                controle = false;
                while(it_list != list.cend()) {
                    it_doc = emplace_back(it_doc,std::move(*it_list));
                    ++it_list;
                }
            }
            ++it_list;
        }
    }
    else
        it_doc.to_parent();
    return it_doc;
}

QString doc_xml::to_string() const {
    QString string;
    QString tab(">");
    auto it = cbegin_prevsuiv();
    while(it) {
        if(it.sens()) {
            auto elt = *it;
            string.append(tab).append(elt.name()).append("\n");
            tab.prepend("-");
        }
        else
            tab.remove(0,1);
        ++it;
    }
    return string;
}

/////////////////////////////////////////reader_xml/////////////////////////////////////
bool reader_xml::exists(QString name_var, const std::map<QString,QString> &atts) {
    bool controle = false;
    if (to_path(name_var)) {
        std::map<QString,QString>::const_iterator it;

        while(readNextStartElement() &&!controle) {
           if(name() == name_var) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    it = atts.cbegin();
                    while(it != atts.cend() &&attribs.hasAttribute(it->first) &&attribs.value(it->first) == it->second)
                        ++it;
                    controle = (it == atts.cend());
               }
               else
                   controle = true;
           }
            skipCurrentElement();
        }
    }
    return controle;
}

std::list<QString> reader_xml::get_vars(QString name_var, const std::map<QString,QString> &atts) {
    std::list<QString> str_list;
    if(to_path(name_var)) {
        std::map<QString,QString>::const_iterator it;
        while(readNextStartElement()) {
            if(name() == name_var) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    it = atts.cbegin();
                    while(it != atts.cend() &&attribs.hasAttribute(it->first) &&attribs.value(it->first) == it->second)
                        ++it;
                    if(it == atts.cend())
                       str_list.push_back(readElementText());
                    else
                        skipCurrentElement();
               }
               else
                   str_list.push_back(readElementText());
            }
            else
                skipCurrentElement();
        }
    }
    return str_list;
}

std::vector<val_atts_t> reader_xml::get_vars_atts(QString name_var, const std::map<QString,QString> &atts) {
    std::vector<val_atts_t> vars_atts;
    if(to_path(name_var)) {
        std::map<QString,QString>::const_iterator it;
        bool add_vars_atts;
        while(readNextStartElement()) {
            if(name() == name_var) {
               if(!atts.empty()) {
                    QXmlStreamAttributes attribs = attributes();
                    it = atts.cbegin();
                    while(it != atts.cend() &&attribs.hasAttribute(it->first) &&attribs.value(it->first) == it->second)
                        ++it;
                    add_vars_atts = (it == atts.cend());
               }
               else
                   add_vars_atts = true;
            }
            else
                add_vars_atts = false;
            if(add_vars_atts) {
                val_atts_t var;
                QXmlStreamAttributes atts = attributes();
                for(QXmlStreamAttributes::const_iterator it_att = atts.constBegin(); it_att != atts.constEnd(); ++it_att)
                    var.second.insert({it_att->name().toString(),it_att->value().toString()});
                var.first = readElementText();
                vars_atts.push_back(std::move(var));
            }
            else
                skipCurrentElement();
        }
    }
    return vars_atts;
}

void reader_xml::read_in(doc_xml &doc) {
    doc.clear();
    auto it = doc.begin();
    while(!atEnd() &&readNext() != QXmlStreamReader::StartElement) ;
    if(!atEnd())
        *it = read_element();

    while(!atEnd()) {
        switch (readNext()) {
        case QXmlStreamReader::StartElement:
            it = doc.push_back(it, read_element());
            break;
        case QXmlStreamReader::EndElement:
            it.to_parent();
            break;
        case QXmlStreamReader::Characters:
            it->set_text(text().toString());
            break;
        default:
            break;
        }
    }
}

doc_xml reader_xml::read() {
    doc_xml doc;
    read_in(doc);
    return doc;
}

element_xml reader_xml::read_element() const {
    element_xml element;
    element.set_name(name().toString());
    std::map<QString,QString> map;
    QXmlStreamAttributes atts = attributes();
    for(auto it = atts.constBegin(); it != atts.constEnd(); ++it)
        map.insert({it->name().toString(),it->value().toString()});
    element.set_attributes(map);
    return element;
}

bool reader_xml::to_path(QString &path) {
    auto n = path.indexOf('/');
    if(n != -1){
        while (n >= 0) {
            auto dirVar = path.left(n);
            path = path.right(path.size()-n-1);
            while(readNextStartElement() &&name() != dirVar)
                skipCurrentElement();
            if(name() == dirVar)
                n = path.indexOf('/');
            else
                n = -2;
        }
    }
    return n != -2;
}
////////////////////////////////////////////////////writer_xml//////////////////////////////////////
void writer_xml::write(const doc_xml &doc) {
    setAutoFormatting(true);
    auto it = doc.cbegin_prevsuiv();
    writeStartDocument();
    while(it) {
        if(it.sens()) {
            auto elt = *it;
            writeStartElement(elt.name());
            if(elt.has_attributes())
                for(auto i = elt.attributes().cbegin(); i != elt.attributes().cend(); ++i)
                    writeAttribute(i->first,i->second);
            if(elt.has_text())
                writeCharacters(elt.text());
        }
        else
            writeEndElement();
        ++it;
    }
    writeEndDocument();
}
