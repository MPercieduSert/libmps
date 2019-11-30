#include "Bdd.h"

using namespace bddMPS;

bool Bdd::copy(const QString & name)
{
    Bdd bdd(m_bdd.driverName(), name,0);
    if(bdd.exists() && bdd.isValid())
    {
        QFile file(name);
        return file.copy(m_fileName);
    }
    return false;
}

bool Bdd::creer()
{
    if(openBdd())
    {
        m_manager->creerVersion();
        miseAJourBdd();
        return true;
    }
    return false;
}

void Bdd::miseAJourBdd()
{
    if(m_version != m_manager->numVersion())
    {
        if(m_version > m_manager->numVersion())
        {
            while(m_version != m_manager->numVersion())
                listeMiseAJourBdd(m_manager->numVersion());
        }
        else
        {
            QString str("La base de données est d'une version plus récente que le programme: \n");
            str.append("Version de la base de données requis par le programmen :").append(QString::number(m_version)).append(".\n");
            str.append("Version de la base de données :").append(QString::number(m_manager->numVersion())).append(".\n");
            throw std::runtime_error(str.toStdString());
        }
    }
}

bool Bdd::open()
{
    if(openBdd())
    {
        miseAJourBdd();
        return true;
    }
    return false;
}


bool Bdd::openBdd()
{
    if(m_bdd.open())
    {
        setBdd();
        return true;
    }
    return false;
}

void Bdd::setBdd()
{
    m_bdd.exec("PRAGMA foreign_keys = ON;");
    QSqlQuery req = QSqlQuery(m_bdd);
    req.setForwardOnly(true);
    m_manager->setRequete(req);
}

void Bdd::setFileName(const QString & fileName)
{
    FileInterface::setFileName(fileName);
    if(m_bdd.isOpen())
        m_bdd.close();
    m_bdd.setDatabaseName(fileName);
}
