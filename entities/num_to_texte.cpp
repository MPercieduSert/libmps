#include "num_to_texte.h"

using namespace mps::divers;

const std::vector<std::pair<int,QString>> num_to_string::m_Romain{   {1000,"M"},
                                                    {900,"CM"},
                                                    {500,"D"},
                                                    {400,"CD"},
                                                    {100,"C"},
                                                    {90,"XC"},
                                                    {50,"L"},
                                                    {40,"XL"},
                                                    {10,"X"},
                                                    {9,"IX"},
                                                    {5,"V"},
                                                    {4,"IV"},
                                                    {1,"I"}};

void num_to_string::set_style(enumt style){
    m_style = style;
    switch (style) {
    case Arabe_Suivant_Style:
        m_to_num = [](const QString &str)->int{return str.toInt() - 1;};
        m_to_string = [](int num)->QString{return QString::number(num + 1);};
        m_valide.setRegularExpression(QRegularExpression("[1-9][0-9]*"));
        break;
    case Majuscule_Style:
        m_to_num = Majuscule_to_num;
        m_to_string = Majuscule;
        m_valide.setRegularExpression(QRegularExpression("[A-Z]*"));
        break;
    case Romain_Style:
        m_to_num = Romain_to_num;
        m_to_string = Romain;
        m_valide.setRegularExpression(QRegularExpression("M{0,3}"
                                                         "(CM|CD|(D?C{0,3}))"
                                                         "(XC|XL|(L?X{0,3}))"
                                                         "(IX|IV|(V?I{0,3}))"));
        break;
    case Romain_Suivant_Style:
        m_to_num = [](const QString &str)->int{return Romain_to_num(str) - 1;};
        m_to_string = [](int num)->QString{return Romain(num + 1);};
        m_valide.setRegularExpression(QRegularExpression("M{0,3}"
                                                         "(CM|CD|(D?C{0,3}))"
                                                         "(XC|XL|(L?X{0,3}))"
                                                         "(IX|IV|(V?I{0,3}))"));
        break;
    case Minuscule_Style:
        m_to_num = Minuscule_to_num;
        m_to_string = Minuscule;
        m_valide.setRegularExpression(QRegularExpression("[a-z]*"));
        break;
    case Grec_Minuscule_Style:
        m_to_num = Grec_minuscule_to_num;
        m_to_string = Grec_minuscule;
        m_valide.setRegularExpression(QRegularExpression("[\\x{03B1}-\\x{03C9}]*"));
        break;
    case Grec_Majuscule_Style:
        m_to_string = Grec_majuscule;
        m_to_num = Grec_majuscule_to_num;
        m_valide.setRegularExpression(QRegularExpression("[\\x{0391}-\\x{03A9}]*"));
        break;
    default:
        m_to_num = [](const QString &str)->int{return str.toInt();};
        m_to_string = [](int num)->QString{return QString::number(num );};
        m_valide.setRegularExpression(QRegularExpression("[0-9]*"));
    }
}

QString num_to_string::Romain(int num) {
    QString str;
    for (auto it = m_Romain.cbegin(); num; ++it) {
        while (num >= it->first) {
            str.append(it->second);
            num -= it->first;
        }
    }
    return str;
}

int num_to_string::Romain_to_num(const QString &str) {
    int num = 0;
    int pos = 0;
    auto it = m_Romain.cbegin();
    while(it != m_Romain.cend() &&pos != str.size()) {
        if(it->second.size() == 1){
            if(str.at(pos) == it->second) {
                num += it->first;
                ++pos;
            }
            else
                ++it;
        }
        else {
            if( pos + 1 != str.size() &&str.at(pos) == it->second.front() &&str.at(pos + 1) == it->second.back()) {
                num += it->first;
                pos += 2;
            }
            else
                ++it;
        }
    }
    return num;
}
