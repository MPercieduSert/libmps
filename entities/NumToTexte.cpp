#include "NumToTexte.h"

using namespace diversMPS;

const std::vector<std::pair<int,QString>> NumToTexte::m_romain{   {1000,"M"},
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

void NumToTexte::setStyle(enumt style){
    m_style = style;
    switch (style) {
    case ArabeSuivant:
        m_toNum = [](const QString & str)->int{return str.toInt() - 1;};
        m_toString = [](int num)->QString{return QString::number(num + 1);};
        m_valide.setRegularExpression(QRegularExpression("[1-9][0-9]*"));
        break;
    case Majuscule:
        m_toNum = majusculeToNum;
        m_toString = majuscule;
        m_valide.setRegularExpression(QRegularExpression("[A-Z]*"));
        break;
    case Romain:
        m_toNum = romainToNum;
        m_toString = romain;
        m_valide.setRegularExpression(QRegularExpression("M{0,3}"
                                                         "(CM|CD|(D?C{0,3}))"
                                                         "(XC|XL|(L?X{0,3}))"
                                                         "(IX|IV|(V?I{0,3}))"));
        break;
    case RomainSuivant:
        m_toNum = [](const QString & str)->int{return romainToNum(str) - 1;};
        m_toString = [](int num)->QString{return romain(num + 1);};
        m_valide.setRegularExpression(QRegularExpression("M{0,3}"
                                                         "(CM|CD|(D?C{0,3}))"
                                                         "(XC|XL|(L?X{0,3}))"
                                                         "(IX|IV|(V?I{0,3}))"));
        break;
    case Minuscule:
        m_toNum = minusculeToNum;
        m_toString = minuscule;
        m_valide.setRegularExpression(QRegularExpression("[a-z]*"));
        break;
    case GrecMinuscule:
        m_toNum = grecMinusculeToNum;
        m_toString = grecMinuscule;
        m_valide.setRegularExpression(QRegularExpression("[\\x{03B1}-\\x{03C9}]*"));
        break;
    case GrecMajuscule:
        m_toString = grecMajuscule;
        m_toNum = grecMajusculeToNum;
        m_valide.setRegularExpression(QRegularExpression("[\\x{0391}-\\x{03A9}]*"));
        break;
    default:
        m_toNum = [](const QString & str)->int{return str.toInt();};
        m_toString = [](int num)->QString{return QString::number(num );};
        m_valide.setRegularExpression(QRegularExpression("[0-9]*"));
    }
}

QString NumToTexte::romain(int num) {
    QString str;
    for (auto iter = m_romain.cbegin(); num; ++iter) {
        while (num >= iter->first) {
            str.append(iter->second);
            num -= iter->first;
        }
    }
    return str;
}

int NumToTexte::romainToNum(const QString & str) {
    int num = 0;
    int pos = 0;
    auto iter = m_romain.cbegin();
    while(iter != m_romain.cend() && pos != str.count()) {
        if(iter->second.count() == 1){
            if(str.at(pos) == iter->second) {
                num += iter->first;
                ++pos;
            }
            else
                ++iter;
        }
        else {
            if( pos + 1 != str.count() && str.at(pos) == iter->second.front() && str.at(pos + 1) == iter->second.back()) {
                num += iter->first;
                pos += 2;
            }
            else
                ++iter;
        }
    }
    return num;
}
