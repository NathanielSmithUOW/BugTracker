#include "database.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QCryptographicHash>

// Connect To Database
// Purpose: Connects to the database
// Input: Void
// Output: bool True for success
bool connectToDatabase(QString &error)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("103.29.85.127");
    db.setPort(3306);
    db.setUserName("bugsy");
    db.setPassword("bug1234");
    db.setDatabaseName("BugTracker");
    db.open();
    if(!db.isOpen())
    {
        qCritical() <<  db.lastError().text();
        error = db.lastError().text();
    }
    return db.isOpen();

}
// Close Database
// Purpose: Closes connection to the database
// Input: Void
// Output: void
void closeDatabase()
{
   QSqlDatabase::database().close();
   QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
// Check Login
// Purpose: Checks user credentials against the Database, can use either email or username
// Input: String identifier = either email or username, String password
// Output: bool True for success
bool checkLogin(QVariant identifier, QVariant password)
{
    if(identifier.toString().isEmpty() || password.toString().isEmpty())
        return false;

    QCryptographicHash* hash = new QCryptographicHash(QCryptographicHash::Sha256);
    QByteArray result = hash->hash(password.toString().toLatin1(), QCryptographicHash::Sha256).toHex();

    QSqlQuery query;
    if(identifier.toString().indexOf('@') > 0)
    {
        query.prepare("SELECT COUNT(*) FROM USER WHERE USER.Email_Address = ? && USER.password = ?");
    }
    else
    {
        query.prepare("SELECT COUNT(*) FROM USER WHERE USER.Username = ? && USER.password = ?");
    }
    query.addBindValue(identifier);
    query.addBindValue(result);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       return query.value(0).toBool();
    }
    hash->reset();
    return false;
}
// Check User Exists
// Purpose: Checks user against the Database, can use either email or username
// Input: String identifier = either email or username
// Output: bool True for Exists already
bool checkUserExists(QVariant identifier)
{
    QSqlQuery query;
    if(identifier.toString().indexOf('@') > 0)
    {
        query.prepare("SELECT COUNT(*) FROM USER WHERE USER.Email_Address = ?");
    }
    else
    {
        query.prepare("SELECT COUNT(*) FROM USER WHERE USER.Username = ?");
    }
    query.addBindValue(identifier);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       return query.value(0).toBool();
    }
    return false;
}
// Get user from ID
// Purpose: Gets user information from the Database
// Input: User ID int
// Output: User object POINTER, NULL If not FOUND
User* getUserFromID(int id)
{
    QSqlQuery query;
    User *u;
    query.prepare("SELECT * FROM USER WHERE USER.ID = ?");
    query.addBindValue(id);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       u = new User( query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString(),
                     query.value(4).toString(),
                     query.value(5).toString(),
                     query.value(6).toString(),
                     query.value(7).toInt(),
                     query.value(8).toString(),
                     query.value(11).toString()
                     );

                 u->setID(query.value(0).toInt());
                 u->setMemberSince(query.value(9).toDate());
                 u->setLastLoggedIn(query.value(10).toDate());
                 u->setStatus(query.value(12).toBool());
           return u;
    }
    return NULL;
}
// Get user from Username
// Purpose: Gets user information from the Database
// Input: Username String
// Output: User object POINTER, NULL If not FOUND
User* getUserFromUsername(QVariant Username)
{
    QSqlQuery query;
    User *u;
    query.prepare("SELECT * FROM USER WHERE USER.Username = ?");
    query.addBindValue(Username);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
        u = new User( query.value(1).toString(),
                      query.value(2).toString(),
                      query.value(3).toString(),
                      query.value(4).toString(),
                      query.value(5).toString(),
                      query.value(6).toString(),
                      query.value(7).toInt(),
                      query.value(8).toString(),
                      query.value(11).toString()
                      );
                  u->setID(query.value(0).toInt());
                  u->setMemberSince(query.value(9).toDate());
                  u->setLastLoggedIn(query.value(10).toDate());
                  u->setStatus(query.value(12).toBool());
           return u;
    }
    return NULL;
}
// Get user from Email
// Purpose: Gets user information from the Database
// Input: User Email String
// Output: User object POINTER, NULL If not FOUND
User* getUserFromEmail(QVariant Email)
{
    QSqlQuery query;
    User *u;
    query.prepare("SELECT * FROM USER WHERE USER.Email_Address = ?");
    query.addBindValue(Email);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
        u = new User( query.value(1).toString(),
                      query.value(2).toString(),
                      query.value(3).toString(),
                      query.value(4).toString(),
                      query.value(5).toString(),
                      query.value(6).toString(),
                      query.value(7).toInt(),
                      query.value(8).toString(),
                      query.value(11).toString()
                      );
                  u->setID(query.value(0).toInt());
                  u->setMemberSince(query.value(9).toDate());
                  u->setLastLoggedIn(query.value(10).toDate());
                  u->setStatus(query.value(12).toBool());
           return u;
    }
    return NULL;
}
// Add new User
// Purpose: Adds user information to the database
// Input: User Object
// Output: bool True if successful
// bool addNewUser(User &u)
// {
//     QSqlQuery query;
//     query.prepare("INSERT INTO USER (First_Name, Family_Name, Username, Email_Address, Password, Gender, Reputation, Role, ProfilePicture)"
//                   "VALUES (?,?,?,?,?,?,?,?,?)");
//     query.addBindValue(u.getFirstName());
//     query.addBindValue(u.getLastName());
//     query.addBindValue(u.getUserName());
//     query.addBindValue(u.getEmail());
//     query.addBindValue(u.getPassword());
//     query.addBindValue(u.getGender());
//     query.addBindValue(u.getReputation());
//     query.addBindValue(u.getRole());
//     query.addBindValue(u.getProfilePicture());
//     bool ok =  query.exec();
//     if(!ok)
//     {
//         qCritical() << query.lastError().text();
//     }
//     return ok;
// }
bool addNewUser(User &u)
{
    QSqlQuery query;
    query.prepare("INSERT INTO USER (First_Name, Family_Name, Username, Email_Address, Password, Gender, Reputation, Role, MemberSince,ProfilePicture,Status)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(u.getFirstName());
    query.addBindValue(u.getLastName());
    query.addBindValue(u.getUserName());
    query.addBindValue(u.getEmail());
    query.addBindValue(u.getPassword());
    query.addBindValue(u.getGender());
    query.addBindValue(u.getReputation());
    query.addBindValue(u.getRole());
    query.addBindValue(u.getMemberSince());
    query.addBindValue(u.getProfilePicture());
    query.addBindValue(u.getStatus());
    bool ok =  query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
// Update User
// Purpose: Updates user information to the database
// Input: User Object
// Output: bool True if successful
bool updateUser(User &u)
{
    QSqlQuery query;
    query.prepare("UPDATE USER SET First_Name = ?, Family_Name = ?, Email_Address = ?, Gender = ?, Password = ?, Reputation = ?, Role = ?, ProfilePicture = ? WHERE ID = ?");
    query.addBindValue(u.getFirstName());
    query.addBindValue(u.getLastName());
    query.addBindValue(u.getEmail());
    query.addBindValue(u.getGender());
    query.addBindValue(u.getPassword());
    query.addBindValue(u.getReputation());
    query.addBindValue(u.getRole());
    query.addBindValue(u.getProfilePicture());
    query.addBindValue(u.getID());
    bool ok =  query.exec();
    if(!ok)
    {
        qDebug() << query.lastError().text();
    }
    return ok;
}
// Get Bug From ID
// Purpose: gets Bug Information from database
// Input: int ID
// Output: Bug Object POINTER, NULL if not found
Bug* getBugFromID(int id)
{
    QSqlQuery query;
    Bug *b;
    query.prepare("SELECT * FROM BUG WHERE BUG.ID = ?");
    query.addBindValue(id);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       b = new Bug( query.value(1).toString(),
                    query.value(2).toString(),
                    query.value(3).toString(),
                    query.value(4).toDouble(),
                    query.value(5).toString(),
                    query.value(6).toString(),
                    query.value(7).toString(),
                    query.value(8).toString(),
                    query.value(9).toString(),
                    query.value(10).toString(),
                    query.value(12).toString()
                     );
                 b->setID(query.value(0).toInt());
                 b->setAssignedTo(query.value(11).toString());
                 b->setSubmitted(QDateTime::fromString(query.value(13).toString(), "yyyy-MM-dd hh:mm:ss"));
           return b;
    }
    return NULL;
}
// Get Bug From ID
// Purpose: gets Bug Information from database
// Input: int ID
// Output: Bug Object POINTER, NULL if not found
Bug* getBugFromTitle(QString title)
{
    QSqlQuery query;
    Bug *b;
    query.prepare("SELECT * FROM BUG WHERE BUG.TITLE = ?");
    query.addBindValue(title);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
        b = new Bug( query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString(),
                     query.value(4).toDouble(),
                     query.value(5).toString(),
                     query.value(6).toString(),
                     query.value(7).toString(),
                     query.value(8).toString(),
                     query.value(9).toString(),
                     query.value(10).toString(),
                     query.value(12).toString()
                      );
                  b->setID(query.value(0).toInt());
                  b->setAssignedTo(query.value(11).toString());
                 b->setSubmitted(QDateTime::fromString(query.value(13).toString(), "yyyy-MM-dd hh:mm:ss"));
            return b;
    }
    return NULL;
}
// Add New Bug
// Purpose: Adds Bug Information to database
// Input: Bug Object
// Output: bool, True if successful
bool addNewBug(Bug& b)
{
    QSqlQuery query;
    query.prepare("INSERT INTO BUG (Title, Status, Application, Version, Description, Platform, Component, Priority, Severity, Walkthrough, AssignedTo, IdentifiedBy, DateSubmitted)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(b.getTitle());
    query.addBindValue(b.getStatus());
    query.addBindValue(b.getApplication());
    query.addBindValue(b.getVersion());
    query.addBindValue(b.getDescription());
    query.addBindValue(b.getPlatform());
    query.addBindValue(b.getComponent());
    query.addBindValue(b.getPriority());
    query.addBindValue(b.getSeverity());
    query.addBindValue(b.getWalkthrough());
    query.addBindValue(b.getAssignedTo());
    query.addBindValue(b.getIdentifiedBy());
    query.addBindValue(b.getSubmitted());
    bool ok =  query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
// Update Existing Bug
// Purpose: Updates Bug Information to database
// Input: Bug Object
// Output: bool, True if successful
bool updateBug(Bug& b)
{
    QSqlQuery query;
    query.prepare("UPDATE BUG SET Title = ?, Status = ?, Application = ?, Version = ?, Description = ?,  Platform = ?, Component = ?, Priority = ?, Severity = ?, Walkthrough = ?, AssignedTo = ? WHERE ID = ?");
    query.addBindValue(b.getTitle());
    query.addBindValue(b.getStatus());
    query.addBindValue(b.getApplication());
    query.addBindValue(b.getVersion());
    query.addBindValue(b.getDescription());
    query.addBindValue(b.getPlatform());
    query.addBindValue(b.getComponent());
    query.addBindValue(b.getPriority());
    query.addBindValue(b.getSeverity());
    query.addBindValue(b.getWalkthrough());
    query.addBindValue(b.getAssignedTo());
    query.addBindValue(b.getID());
    bool ok =  query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<QString> getNotifications(User &u, bool all)
{
    QList<QString> list;

    QSqlQuery query;
    if(all)
    {
         query.prepare("SELECT Notification FROM NOTIFICATION WHERE NOTIFICATION.UserID = ?");
         query.addBindValue(u.getID());
    }
    else
    {
         query.prepare("SELECT Notification FROM NOTIFICATION WHERE NOTIFICATION.UserID = ? AND NOTIFICATION.Date > ?");
         query.addBindValue(u.getID());
         query.addBindValue(u.getLastLoggedIn());

    }
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}

QList<QString> getSubscriptions(User &u)
{
    QList<QString> list;

    QSqlQuery query;
         query.prepare("SELECT BugID FROM SUBSCRIPTION WHERE SUBSCRIPTION.UserID = ?");
         query.addBindValue(u.getUserName());

     bool ok = query.exec();
     if(!ok)
     {
         qCritical() << query.lastError().text();
     }
    while(query.next())
    {
       Bug *bug;
       bug=getBugFromID(query.value(0).toInt());
       list.append(bug->getTitle());
    }
    return list;
}

bool deleteSubscriptions(Bug &b, User &u)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SUBSCRIPTION WHERE SUBSCRIPTION.BugID = ? AND SUBSCRIPTION.UserID = ?");
    query.addBindValue(b.getID());
    query.addBindValue(u.getUserName());
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}

QList<QString> getSubscribers(Bug &b)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT UserID FROM SUBSCRIPTION WHERE SUBSCRIPTION.BugID = ?");
     query.addBindValue(b.getID());

     bool ok = query.exec();
     if(!ok)
     {
         qCritical() << query.lastError().text();
     }
    while(query.next())
    {
       list.append("ID: " + query.value(0).toString());
    }
    return list;
}
bool addSubscriber(Bug &b, User &u)
{

    QSqlQuery query;
    query.prepare("INSERT INTO SUBSCRIPTION (BugID, UserID)"
                  "VALUES (?,?)");
    query.addBindValue(b.getID());
    query.addBindValue(u.getUserName());

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
bool addSubscriber(QString bugID, QString userID)
{

    QSqlQuery query;
    query.prepare("INSERT INTO SUBSCRIPTION (BugID, UserID)"
                  "VALUES (?,?)");
    query.addBindValue(getBugFromTitle(bugID)->getID());
    query.addBindValue(userID);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<QString> getBugHistory(Bug &b)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT History FROM HISTORY WHERE HISTORY.BugID = ?");
     query.addBindValue(b.getID());

     bool ok = query.exec();
     if(!ok)
     {
         qCritical() << query.lastError().text();
     }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}
bool addBugHistory(Bug &b, QString history)
{

    QSqlQuery query;
    query.prepare("INSERT INTO HISTORY (BugID, History)"
                  "VALUES (?,?)");
    query.addBindValue(b.getID());
    query.addBindValue(history);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<QString> getBugComments(Bug &b)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT UserID, Submitted, Comment FROM COMMENT WHERE COMMENT.BugID = ?");
     query.addBindValue(b.getID());

     bool ok = query.exec();
     if(!ok)
     {
         qCritical() << query.lastError().text();
     }
    while(query.next())
    {
       // QString str = "ID: " + query.value(0).toString();
       User *u = getUserFromID(query.value(0).toInt());
       if(u != NULL)
       {
            list.append("User: " + u->getUserName() + "\t" + query.value(1).toString() + "\n" + query.value(2).toString());
       }
    }
    return list;
}
bool addBugComment(Bug &b, User &u, QString comment)
{

    QSqlQuery query;
    query.prepare("INSERT INTO COMMENT (BugID, UserID, Comment)"
                  "VALUES (?,?,?)");
    query.addBindValue(b.getID());
    query.addBindValue(u.getID());
    query.addBindValue(comment);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
bool addBugComment(Bug &b, User &u, QString comment, QDateTime date)
{

    QSqlQuery query;
    query.prepare("INSERT INTO COMMENT (BugID, UserID, Comment, Submitted)"
                  "VALUES (?,?,?,?)");
    query.addBindValue(b.getID());
    query.addBindValue(u.getID());
    query.addBindValue(comment);
    query.addBindValue(date);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
bool deleteAllSubscribers(Bug &b)
{

    QSqlQuery query;
    query.prepare("DELETE FROM SUBSCRIPTION WHERE BugID = ?");
    query.addBindValue(b.getID());

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}

QList<QString> getAttachments(Bug &b)
{
    QList<QString> list;

    QSqlQuery query;
         query.prepare("SELECT Path, Submitted, Attacher FROM ATTACHMENT WHERE ATTACHMENT.BugID = ?");
         query.addBindValue(b.getID());

     bool ok = query.exec();
     if(!ok)
     {
         qCritical() << query.lastError().text();
     }
    while(query.next())
    {
       list.append(query.value(0).toString() + "|" + query.value(1).toString() + "|" + query.value(2).toString());
    }
    return list;
}
bool addAttachment(Bug &b, QString attacher, QString path, QDateTime submitted)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ATTACHMENT (BugID, Attacher, Path, Submitted)"
                  "VALUES (?,?,?,?)");
    query.addBindValue(b.getID());
    query.addBindValue(attacher);
    query.addBindValue(path);
    query.addBindValue(submitted);

    bool ok =  query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<Bug*> searchBug(QGroupBox* box, bool firstLoad)
{
    QList<Bug*> list;
    QSqlQuery query;
    Bug *b;


    if(firstLoad)
    {
        QDate currentDate = QDate::currentDate();
        query.prepare("SELECT * FROM BUG WHERE BUG.DateSubmitted > :line");
        query.addBindValue(currentDate.addDays(-(LOAD_BUG_DAYS)));
    }
    else
    {
        QString queryString = "SELECT * FROM BUG WHERE BUG.Title REGEXP :line";
        QLineEdit *line = box->findChild<QLineEdit *>("main_searchLineEdit");

        if(box->findChild<QCheckBox *>("main_IDCheckBox")->isChecked())
        {
            queryString += " OR BUG.ID = :line";
        }
        if(box->findChild<QCheckBox *>("main_DescriptionCheckBox")->isChecked())
        {
            queryString += " OR BUG.Description REGEXP :line";
        }
        if(box->findChild<QCheckBox *>("main_ApplicationCheckBox")->isChecked())
        {
            queryString += " OR BUG.Application REGEXP :line";
        }
        if(box->findChild<QCheckBox *>("main_UserCheckBox")->isChecked())
        {
            queryString += " OR BUG.IdentifiedBy REGEXP :line";
        }
        if(box->findChild<QCheckBox *>("main_DeveloperCheckBox")->isChecked())
        {
            queryString += " OR BUG.AssignedTo REGEXP :line";
        }
        query.prepare(queryString);
        query.bindValue(":line", line->text());
    }

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
        b = new Bug( query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString(),
                     query.value(4).toDouble(),
                     query.value(5).toString(),
                     query.value(6).toString(),
                     query.value(7).toString(),
                     query.value(8).toString(),
                     query.value(9).toString(),
                     query.value(10).toString(),
                     query.value(12).toString()
                      );
                  b->setID(query.value(0).toInt());
                  b->setAssignedTo(query.value(11).toString());
                 b->setSubmitted(QDateTime::fromString(query.value(13).toString(), "yyyy-MM-dd hh:mm:ss"));
           list.append(b);
    }
    return list;
}
QList<User*> searchUsers(QGroupBox* box, bool loadAll)
{
    QList<User*> list;
    QSqlQuery query;
    User *u;

    if(loadAll)
    {
        QString queryString = "SELECT * FROM USER";
        query.prepare(queryString);
    }
    else
    {
        QString queryString = "SELECT * FROM USER WHERE USER.Username REGEXP :line";
        QLineEdit *line = box->findChild<QLineEdit *>("searchUser_searchBarLine");

        if(box->findChild<QCheckBox *>("searchUser_IDBox")->isChecked())
        {
            queryString += " OR USER.ID = :line";
        }
        if(box->findChild<QCheckBox *>("searchUser_EmailBox")->isChecked())
        {
            queryString += " OR USER.Email_Address REGEXP :line";
        }
        if(box->findChild<QCheckBox *>("searchUser_NameBox")->isChecked())
        {
            queryString += " OR USER.First_Name REGEXP :line";
            queryString += " OR USER.Family_Name REGEXP :line";
        }
        query.prepare(queryString);
        query.bindValue(":line", line->text());
    }

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
        u = new User( query.value(1).toString(),
                      query.value(2).toString(),
                      query.value(3).toString(),
                      query.value(4).toString(),
                      query.value(5).toString(),
                      query.value(6).toString(),
                      query.value(7).toInt(),
                      query.value(8).toString(),
                      query.value(11).toString()
                      );
                  u->setID(query.value(0).toInt());
                  u->setMemberSince(query.value(9).toDate());
                  u->setLastLoggedIn(query.value(10).toDate());
                  u->setStatus(query.value(12).toBool());
           list.append(u);
    }
    return list;
}
QList<QString> getApplications()
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT DISTINCT Name FROM APPLICATION");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}
bool addApplication(QString application, double version)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("INSERT INTO APPLICATION (Name, Version)"
                  "VALUES (?,?)");
    query.addBindValue(application);
    query.addBindValue(version);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<double> getVersions(QString application)
{
    QList<double> list;

    QSqlQuery query;
    query.prepare("SELECT Version FROM APPLICATION WHERE APPLICATION.Name = ?");
    query.addBindValue(application);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toDouble());
    }
    return list;
}
QList<QString> getComponents(QString application)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT Name FROM COMPONENT WHERE COMPONENT.Application = ?");
    query.addBindValue(application);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}
bool addComponent(QString application, QString component)
{
    QSqlQuery query;
    query.prepare("INSERT INTO COMPONENT (Name) WHERE COMPONENT.Application = ?"
                  "VALUES (?)");
    query.addBindValue(application);
    query.addBindValue(component);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<QString> getPlatforms()
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT Name FROM PLATFORM");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}
bool addApplication(QString platform)
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("INSERT INTO PLATFORM (Name)"
                  "VALUES (?)");
    query.addBindValue(platform);

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
bool addNotificationToUser(User &u, QString notification)
{
    QSqlQuery query;
    query.prepare("INSERT INTO NOTIFICATION (UserID, Notification)"
                  "VALUES (?, ?)");
    query.addBindValue(u.getID());
    query.addBindValue(notification);
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    return ok;
}
QList<QString> getDevelopers()
{
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT Username FROM USER WHERE USER.Role = ?");
    query.addBindValue("Developer");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(query.value(0).toString());
    }
    return list;
}
int getBugsReportedBetween(QDate start, QDate end)
{
    return 2420;
    QSqlQuery query;
    query.prepare("SELECT COALESCE(Count(*),0) FROM REPORTED WHERE Changed BETWEEN ? AND ?");
    query.addBindValue(start.toString("yyyy-MM-dd"));
    query.addBindValue(end.toString("yyyy-MM-dd"));
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
           if(!query.value(0).isNull())
           {
               return query.value(0).toInt();
           }
    }
    return 0;
}

int getBugsResolvedBetween(QDate start, QDate end)
{
    return 2310;
    QSqlQuery query;
    query.prepare("SELECT COALESCE(Count(*),0) FROM RESOLVED WHERE Changed BETWEEN ? AND ?");
    query.addBindValue(start.toString("yyyy-MM-dd"));
    query.addBindValue(end.toString("yyy-MM-dd"));
    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
           if(!query.value(0).isNull())
           {
               return query.value(0).toInt();
           }
    }
    return 0;
}

QList<QPair<QString, int>> getBestReporters()
{
    QList<QPair<QString, int>> list;

    QSqlQuery query;
    query.prepare("SELECT Username, Reputation FROM USER WHERE USER.Role = 'Reporter' ORDER BY Reputation DESC LIMIT 10");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(QPair<QString, int>(query.value(0).toString(), query.value(1).toInt()));
    }
    return list;
}

QList<QPair<QString, int>> getBestDevelopers()
{
    QList<QPair<QString, int>> list;

    QSqlQuery query;
    query.prepare("SELECT Username, Reputation FROM USER WHERE USER.Role = 'Developer' ORDER BY Reputation DESC LIMIT 10");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(QPair<QString, int>(query.value(0).toString(), query.value(1).toInt()));
    }
    return list;
}

QList<QPair<QString, int>> getBestReviewers()
{
    QList<QPair<QString, int>> list;

    QSqlQuery query;
    query.prepare("SELECT Username, Reputation FROM USER WHERE USER.Role = 'Reviewer' ORDER BY Reputation DESC LIMIT 10");

    bool ok = query.exec();
    if(!ok)
    {
        qCritical() << query.lastError().text();
    }
    while(query.next())
    {
       list.append(QPair<QString, int>(query.value(0).toString(), query.value(1).toInt()));
    }
    return list;
}
