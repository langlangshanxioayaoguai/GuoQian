// VariableDatabase.cpp - 修正版本
#include"variabledatabase.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QMutexLocker>
#include <QDebug>

namespace Industrial {

// 辅助函数：获取查询错误信息
QString getQueryError(const QSqlQuery& query) {
    return query.lastError().text();
}

// VariableDatabase 实现
VariableDatabase::VariableDatabase(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
}

VariableDatabase::~VariableDatabase()
{
    clearCache();
    if (m_database.isOpen()) {
        m_database.close();
    }
    // 问题：没有移除数据库连接！
    // 应添加：
    QString connectionName = m_database.connectionName();
    m_database = QSqlDatabase();  // 重置
    QSqlDatabase::removeDatabase(connectionName);
}

bool VariableDatabase::initialize(const QString &connectionString)
{
    // 创建唯一的连接名称
    static int connectionCounter = 0;
    QString connectionName = QString("VariableDatabase_%1").arg(++connectionCounter);

    m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    m_database.setDatabaseName(connectionString);
    m_databaseName = connectionString; // 保存路径
    if (!m_database.open()) {
        QString error = m_database.lastError().text();
        qCritical() << "Failed to open database:" << error;
        return false;
    }

    // 启用外键约束
    QSqlQuery query(m_database);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        QString error = query.lastError().text();
        qCritical() << "Failed to enable foreign keys:" << error;
        return false;
    }

    if (!createTables()) {
        qCritical() << "Failed to create tables";
        return false;
    }

    if (!createIndexes()) {
        qCritical() << "Failed to create indexes";
        return false;
    }

    if (!createTriggers()) {
        qWarning() << "Failed to create triggers, continuing anyway";
    }

    m_initialized = true;
    return true;
}

QString VariableDatabase::databaseName() const
{
    return m_databaseName;
}

bool VariableDatabase::createTables()
{
    QSqlQuery query(m_database);

    // 变量定义表
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS variable_definitions (
            tag_name TEXT PRIMARY KEY,
            description TEXT,
            type INTEGER NOT NULL,
            unit INTEGER DEFAULT 0,
            min_value REAL DEFAULT 0.0,
            max_value REAL DEFAULT 100.0,
            deadband REAL DEFAULT 0.1,
            initial_value REAL DEFAULT 0.0,
            update_rate INTEGER DEFAULT 1000,
            priority INTEGER DEFAULT 50,
            alarm_lo REAL,
            alarm_hi REAL,
            alarm_lolo REAL,
            alarm_hihi REAL,
            alarm_level INTEGER DEFAULT 0,
            history_enabled INTEGER DEFAULT 0,
            history_interval INTEGER DEFAULT 60,
            writable INTEGER DEFAULT 1,
            access_group TEXT,
            address TEXT,
            data_type TEXT,
            format_string TEXT,
            created_time DATETIME DEFAULT CURRENT_TIMESTAMP,
            modified_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create variable_definitions table:" << error;
        return false;
    }

    // 变量组表
    sql = R"(
        CREATE TABLE IF NOT EXISTS variable_groups (
            group_name TEXT PRIMARY KEY,
            description TEXT,
            parent_group TEXT,
            created_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create variable_groups table:" << error;
        return false;
    }

    // 变量组映射表
    sql = R"(
        CREATE TABLE IF NOT EXISTS variable_group_mapping (
            tag_name TEXT,
            group_name TEXT,
            PRIMARY KEY (tag_name, group_name),
            FOREIGN KEY (tag_name) REFERENCES variable_definitions(tag_name) ON DELETE CASCADE,
            FOREIGN KEY (group_name) REFERENCES variable_groups(group_name) ON DELETE CASCADE
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create variable_group_mapping table:" << error;
        return false;
    }

    // 区域表
    sql = R"(
        CREATE TABLE IF NOT EXISTS plant_areas (
            area_code TEXT PRIMARY KEY,
            area_name TEXT NOT NULL,
            description TEXT,
            created_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create plant_areas table:" << error;
        return false;
    }

    // 设备表
    sql = R"(
        CREATE TABLE IF NOT EXISTS plant_devices (
            device_name TEXT PRIMARY KEY,
            area_code TEXT NOT NULL,
            device_type TEXT NOT NULL,
            description TEXT,
            created_time DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (area_code) REFERENCES plant_areas(area_code) ON DELETE CASCADE
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create plant_devices table:" << error;
        return false;
    }

    // 关联变量表
    sql = R"(
        CREATE TABLE IF NOT EXISTS variable_relations (
            tag_name TEXT,
            related_tag TEXT,
            relation_type INTEGER DEFAULT 0,
            PRIMARY KEY (tag_name, related_tag),
            FOREIGN KEY (tag_name) REFERENCES variable_definitions(tag_name) ON DELETE CASCADE
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create variable_relations table:" << error;
        return false;
    }

    // 历史版本表
    sql = R"(
        CREATE TABLE IF NOT EXISTS variable_versions (
            version_id INTEGER PRIMARY KEY AUTOINCREMENT,
            version_name TEXT UNIQUE NOT NULL,
            description TEXT,
            created_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create variable_versions table:" << error;
        return false;
    }

    // 版本详情表
    sql = R"(
        CREATE TABLE IF NOT EXISTS version_details (
            version_id INTEGER,
            tag_name TEXT,
            variable_data TEXT NOT NULL,
            PRIMARY KEY (version_id, tag_name),
            FOREIGN KEY (version_id) REFERENCES variable_versions(version_id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(sql)) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create version_details table:" << error;
        return false;
    }

    return true;
}

bool VariableDatabase::createIndexes()
{
    QSqlQuery query(m_database);
    bool success = true;

    QStringList indexes = {
        "CREATE INDEX IF NOT EXISTS idx_variables_type ON variable_definitions(type)",
        "CREATE INDEX IF NOT EXISTS idx_variables_area ON plant_devices(area_code)",
        "CREATE INDEX IF NOT EXISTS idx_variables_device ON variable_definitions(tag_name)",
        "CREATE INDEX IF NOT EXISTS idx_group_mapping ON variable_group_mapping(group_name)",
        "CREATE INDEX IF NOT EXISTS idx_relations ON variable_relations(related_tag)",
        "CREATE INDEX IF NOT EXISTS idx_variables_alarm ON variable_definitions(alarm_level)"
    };

    for (const QString &sql : indexes) {
        if (!query.exec(sql)) {
            QString error = query.lastError().text();
            qWarning() << "Failed to create index:" << error;
            success = false;
        }
    }

    return success;
}

bool VariableDatabase::createTriggers()
{
    QSqlQuery query(m_database);

    // 更新修改时间触发器
    QString trigger = R"(
        CREATE TRIGGER IF NOT EXISTS update_variable_timestamp
        AFTER UPDATE ON variable_definitions
        BEGIN
            UPDATE variable_definitions
            SET modified_time = CURRENT_TIMESTAMP
            WHERE tag_name = NEW.tag_name;
        END
    )";

    if (!query.exec(trigger)) {
        QString error = query.lastError().text();
        qWarning() << "Failed to create trigger:" << error;
        return false;
    }

    return true;
}

bool VariableDatabase::saveVariableDefinition(VariableDefinition *var)
{
    if (!var || !m_initialized) return false;

    QMutexLocker locker(&m_cacheMutex);

    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT OR REPLACE INTO variable_definitions
        (tag_name, description, type, unit, min_value, max_value, deadband,
         initial_value, update_rate, priority, alarm_lo, alarm_hi, alarm_lolo,
         alarm_hihi, alarm_level, history_enabled, history_interval, writable,
         access_group, address, data_type, format_string)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(var->tagName());
    query.addBindValue(var->description());
    query.addBindValue(static_cast<int>(var->type()));
    query.addBindValue(static_cast<int>(var->unit()));
    query.addBindValue(var->minValue());
    query.addBindValue(var->maxValue());
    query.addBindValue(var->deadband());
    query.addBindValue(var->initialValue());
    query.addBindValue(var->updateRate());
    query.addBindValue(var->priority());
    query.addBindValue(var->alarmLo());
    query.addBindValue(var->alarmHi());
    query.addBindValue(var->alarmLoLo());
    query.addBindValue(var->alarmHiHi());
    query.addBindValue(static_cast<int>(var->alarmLevel()));
    query.addBindValue(var->historyEnabled() ? 1 : 0);
    query.addBindValue(var->historyInterval());
    query.addBindValue(var->writable() ? 1 : 0);
    query.addBindValue(var->accessGroup());
    query.addBindValue(var->address());
    query.addBindValue(var->dataType());
    query.addBindValue(var->formatString());

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to save variable:" << error;
        return false;
    }

    // 保存关联变量
    QStringList relatedVars = var->relatedVariables();
    if (!relatedVars.isEmpty()) {
        // 先删除旧的关联
        query.prepare("DELETE FROM variable_relations WHERE tag_name = ?");
        query.addBindValue(var->tagName());
        if (!query.exec()) {
            QString error = query.lastError().text();
            qWarning() << "Failed to delete old relations:" << error;
        }

        // 插入新的关联
        query.prepare("INSERT INTO variable_relations (tag_name, related_tag) VALUES (?, ?)");
        for (const QString &relatedTag : relatedVars) {
            query.addBindValue(var->tagName());
            query.addBindValue(relatedTag);
            if (!query.exec()) {
                QString error = query.lastError().text();
                qWarning() << "Failed to save relation:" << error;
            }
        }
    }

    updateCache(var);

    emit variableSaved(var->tagName());
    emit databaseChanged();

    return true;
}

bool VariableDatabase::updateVariableDefinition(VariableDefinition *var)
{
    return saveVariableDefinition(var); // 使用同样的逻辑
}

bool VariableDatabase::deleteVariableDefinition(const QString &tagName)
{
    if (!m_initialized) return false;

    QMutexLocker locker(&m_cacheMutex);

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM variable_definitions WHERE tag_name = ?");
    query.addBindValue(tagName);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to delete variable:" << error;
        return false;
    }

    removeFromCache(tagName);
    emit variableDeleted(tagName);
    emit databaseChanged();

    return true;
}

VariableDefinition* VariableDatabase::loadVariableDefinition(const QString &tagName)
{
    QMutexLocker locker(&m_cacheMutex);

    // 检查缓存
    if (m_variableCache.contains(tagName)) {
        return m_variableCache[tagName];
    }

    if (!m_initialized) return nullptr;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT description, type, unit, min_value, max_value, deadband,
               initial_value, update_rate, priority, alarm_lo, alarm_hi,
               alarm_lolo, alarm_hihi, alarm_level, history_enabled,
               history_interval, writable, access_group, address, data_type,
               format_string
        FROM variable_definitions
        WHERE tag_name = ?
    )");

    query.addBindValue(tagName);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to execute query for variable" << tagName << ":" << error;
        return nullptr;
    }

    if (!query.next()) {
        qWarning() << "Variable not found:" << tagName;
        return nullptr;
    }

    VariableDefinition *var = new VariableDefinition(tagName,
                                                     static_cast<VariableType>(query.value(1).toInt()));

    var->setDescription(query.value(0).toString());
    var->setUnit(static_cast<EngineeringUnit>(query.value(2).toInt()));
    var->setRange(query.value(3).toDouble(), query.value(4).toDouble());
    var->setDeadband(query.value(5).toDouble());
    var->setInitialValue(query.value(6).toDouble());
    var->setUpdateRate(query.value(7).toInt());
    var->setPriority(query.value(8).toInt());
    var->setAlarmLimits(query.value(9).toDouble(),
                        query.value(10).toDouble(),
                        query.value(11).toDouble(),
                        query.value(12).toDouble());
    var->setAlarmLevel(static_cast<AlarmLevel>(query.value(13).toInt()));
    var->setHistoryEnabled(query.value(14).toInt() != 0);
    var->setHistoryInterval(query.value(15).toInt());
    var->setWritable(query.value(16).toInt() != 0);
    var->setAccessGroup(query.value(17).toString());
    var->setAddress(query.value(18).toString());
    var->setDataType(query.value(19).toString());
    var->setFormatString(query.value(20).toString());

    // 加载关联变量
    query.prepare("SELECT related_tag FROM variable_relations WHERE tag_name = ?");
    query.addBindValue(tagName);
    if (query.exec()) {
        while (query.next()) {
            var->addRelatedVariable(query.value(0).toString());
        }
    } else {
        QString error = query.lastError().text();
        qWarning() << "Failed to load variable relations:" << error;
    }

    m_variableCache.insert(tagName, var);

    return var;
}

QList<VariableDefinition*> VariableDatabase::loadAllVariables()
{
    QList<VariableDefinition*> variables;

    if (!m_initialized) return variables;

    QSqlQuery query(m_database);
    query.prepare("SELECT tag_name FROM variable_definitions ORDER BY tag_name");

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to load variables:" << error;
        return variables;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            variables.append(var);
        }
    }

    return variables;
}

bool VariableDatabase::saveVariableGroup(VariableGroup *group)
{
    if (!group || !m_initialized) return false;

    QSqlQuery query(m_database);

    // 保存组信息
    query.prepare(R"(
        INSERT OR REPLACE INTO variable_groups (group_name, description)
        VALUES (?, ?)
    )");
    query.addBindValue(group->groupName());
    query.addBindValue(group->description());

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to save group:" << error;
        return false;
    }

    // 保存组内变量映射
    for (VariableDefinition *var : group->variables()) {
        query.prepare(R"(
            INSERT OR REPLACE INTO variable_group_mapping (tag_name, group_name)
            VALUES (?, ?)
        )");
        query.addBindValue(var->tagName());
        query.addBindValue(group->groupName());
        if (!query.exec()) {
            QString error = query.lastError().text();
            qWarning() << "Failed to save group mapping:" << error;
        }
    }

    return true;
}

VariableGroup* VariableDatabase::loadVariableGroup(const QString &groupName)
{
    if (!m_initialized) return nullptr;

    QSqlQuery query(m_database);
    query.prepare("SELECT description FROM variable_groups WHERE group_name = ?");
    query.addBindValue(groupName);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to load group:" << error;
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    VariableGroup *group = new VariableGroup(groupName);
    group->setDescription(query.value(0).toString());

    // 加载组内变量
    query.prepare(R"(
        SELECT vd.tag_name
        FROM variable_definitions vd
        JOIN variable_group_mapping vgm ON vd.tag_name = vgm.tag_name
        WHERE vgm.group_name = ?
        ORDER BY vd.tag_name
    )");
    query.addBindValue(groupName);

    if (query.exec()) {
        while (query.next()) {
            QString tagName = query.value(0).toString();
            VariableDefinition *var = loadVariableDefinition(tagName);
            if (var) {
                group->addVariable(var);
            }
        }
    } else {
        QString error = query.lastError().text();
        qWarning() << "Failed to load group variables:" << error;
    }

    return group;
}

bool VariableDatabase::savePlantArea(PlantArea *area)
{
    if (!area || !m_initialized) return false;

    QSqlQuery query(m_database);

    // 保存区域信息
    query.prepare(R"(
        INSERT OR REPLACE INTO plant_areas (area_code, area_name)
        VALUES (?, ?)
    )");
    query.addBindValue(area->areaCode());
    query.addBindValue(area->areaName());

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to save plant area:" << error;
        return false;
    }

    // 保存设备信息
    QList<QString> deviceNames = area->getDeviceNames();
    // 注意：这里需要修改PlantArea类以提供获取设备信息的方法
    // 暂时简化处理

    return true;
}

PlantArea* VariableDatabase::loadPlantArea(const QString &areaCode)
{
    if (!m_initialized) return nullptr;

    QSqlQuery query(m_database);
    query.prepare("SELECT area_name FROM plant_areas WHERE area_code = ?");
    query.addBindValue(areaCode);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to load plant area:" << error;
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    PlantArea *area = new PlantArea(query.value(0).toString(), areaCode);

    // 加载设备
    query.prepare("SELECT device_name, device_type FROM plant_devices WHERE area_code = ?");
    query.addBindValue(areaCode);

    if (query.exec()) {
        while (query.next()) {
            area->addDevice(query.value(0).toString(),
                            query.value(1).toString());
        }
    } else {
        QString error = query.lastError().text();
        qWarning() << "Failed to load plant devices:" << error;
    }

    return area;
}

QList<VariableDefinition*> VariableDatabase::findVariablesByType(VariableType type)
{
    //-----------------后添加的---------------------------
    // 可以添加缓存机制
    static QMap<VariableType, QList<VariableDefinition*>> typeCache;
    static QDateTime lastCacheTime;

    if (typeCache.contains(type) &&
        lastCacheTime.secsTo(QDateTime::currentDateTime()) < 60) {
        return typeCache[type];
    }
   //-----------------后添加的---------------------------
    QList<VariableDefinition*> result;

    if (!m_initialized) return result;

     QSqlQuery query(m_database);
        query.prepare("SELECT tag_name FROM variable_definitions WHERE type = ? ORDER BY tag_name");
        query.addBindValue(static_cast<int>(type));
    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to find variables by type:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }
    //-----------------后添加的---------------------------
    typeCache[type] = result;
    lastCacheTime = QDateTime::currentDateTime();
    //-----------------后添加的---------------------------
    return result;
}

QList<VariableDefinition*> VariableDatabase::findVariablesByArea(const QString &areaCode)
{
    QList<VariableDefinition*> result;

    if (!m_initialized) return result;

    // 这个查询需要结合多个表
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT DISTINCT vd.tag_name
        FROM variable_definitions vd
        WHERE vd.tag_name LIKE ? || '.%'
        ORDER BY vd.tag_name
    )");
    query.addBindValue(areaCode);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to find variables by area:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }

    return result;
}

QList<VariableDefinition*> VariableDatabase::findVariablesByDevice(const QString &deviceName)
{
    QList<VariableDefinition*> result;

    if (!m_initialized) return result;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT DISTINCT vd.tag_name
        FROM variable_definitions vd
        WHERE vd.tag_name LIKE '%.' || ? || '.%'
        ORDER BY vd.tag_name
    )");
    query.addBindValue(deviceName);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to find variables by device:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }

    return result;
}

QList<VariableDefinition*> VariableDatabase::findAlarmVariables()
{
    QList<VariableDefinition*> result;

    if (!m_initialized) return result;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT tag_name
        FROM variable_definitions
        WHERE alarm_level > 0
        ORDER BY alarm_level DESC, tag_name
    )");

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to find alarm variables:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }

    return result;
}

QList<VariableDefinition*> VariableDatabase::findVariablesWithAlarm(double value)
{
    QList<VariableDefinition*> result;

    if (!m_initialized) return result;

    // 查找当前值可能触发报警的变量
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT tag_name
        FROM variable_definitions
        WHERE alarm_level > 0
        AND (? <= alarm_lolo OR ? >= alarm_hihi OR ? <= alarm_lo OR ? >= alarm_hi)
        ORDER BY alarm_level DESC
    )");

    for (int i = 0; i < 4; i++) {
        query.addBindValue(value);
    }

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to find variables with alarm:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }

    return result;
}

QList<VariableDefinition*> VariableDatabase::searchVariables(const QString &keyword)
{
    QList<VariableDefinition*> result;

    if (!m_initialized || keyword.isEmpty()) return result;

    QString searchPattern = "%" + keyword + "%";

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT tag_name
        FROM variable_definitions
        WHERE tag_name LIKE ?
           OR description LIKE ?
        ORDER BY tag_name
    )");

    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qWarning() << "Failed to search variables:" << error;
        return result;
    }

    while (query.next()) {
        QString tagName = query.value(0).toString();
        VariableDefinition *var = loadVariableDefinition(tagName);
        if (var) {
            result.append(var);
        }
    }

    return result;
}

bool VariableDatabase::exportToJson(const QString &filename)
{
    if (!m_initialized) return false;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Failed to open file for writing:" << filename;
        return false;
    }

    QJsonObject root;
    QJsonArray variables;

    // 导出所有变量
    QList<VariableDefinition*> allVars = loadAllVariables();
    for (VariableDefinition *var : allVars) {
        QJsonObject varObj;
        varObj["tagName"] = var->tagName();
        varObj["description"] = var->description();
        varObj["type"] = static_cast<int>(var->type());
        varObj["unit"] = static_cast<int>(var->unit());
        varObj["minValue"] = var->minValue();
        varObj["maxValue"] = var->maxValue();
        varObj["deadband"] = var->deadband();
        varObj["updateRate"] = var->updateRate();
        varObj["alarmLo"] = var->alarmLo();
        varObj["alarmHi"] = var->alarmHi();
        varObj["address"] = var->address();
        varObj["dataType"] = var->dataType();

        variables.append(varObj);
    }

    root["variables"] = variables;
    root["exportTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["totalCount"] = allVars.count();

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool VariableDatabase::importFromJson(const QString &filename)
{
    if (!m_initialized) return false;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open file for reading:" << filename;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qCritical() << "Invalid JSON format";
        return false;
    }

    QJsonObject root = doc.object();
    if (!root.contains("variables") || !root["variables"].isArray()) {
        qCritical() << "Missing variables array in JSON";
        return false;
    }

    QJsonArray variables = root["variables"].toArray();
    int successCount = 0;
    int failCount = 0;

    // 开始事务
    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    for (const QJsonValue &value : variables) {
        QJsonObject varObj = value.toObject();

        if (!varObj.contains("tagName") || !varObj.contains("type")) {
            failCount++;
            continue;
        }

        QString tagName = varObj["tagName"].toString();
        VariableType type = static_cast<VariableType>(varObj["type"].toInt());

        VariableDefinition *var = new VariableDefinition(tagName, type);

        if (varObj.contains("description")) {
            var->setDescription(varObj["description"].toString());
        }
        if (varObj.contains("unit")) {
            var->setUnit(static_cast<EngineeringUnit>(varObj["unit"].toInt()));
        }
        if (varObj.contains("minValue") && varObj.contains("maxValue")) {
            var->setRange(varObj["minValue"].toDouble(),
                          varObj["maxValue"].toDouble());
        }
        if (varObj.contains("deadband")) {
            var->setDeadband(varObj["deadband"].toDouble());
        }
        if (varObj.contains("updateRate")) {
            var->setUpdateRate(varObj["updateRate"].toInt());
        }
        if (varObj.contains("alarmLo") && varObj.contains("alarmHi")) {
            var->setAlarmLimits(varObj["alarmLo"].toDouble(),
                                varObj["alarmHi"].toDouble());
        }
        if (varObj.contains("address")) {
            var->setAddress(varObj["address"].toString());
        }
        if (varObj.contains("dataType")) {
            var->setDataType(varObj["dataType"].toString());
        }

        if (saveVariableDefinition(var)) {
            successCount++;
        } else {
            failCount++;
        }

        delete var;
    }

    // 提交事务
    if (!m_database.commit()) {
        m_database.rollback();
        qCritical() << "Failed to commit transaction";
        return false;
    }

    emit importCompleted(successCount, failCount);
    emit databaseChanged();

    return true;
}

bool VariableDatabase::exportToCsv(const QString &filename)
{
    if (!m_initialized) return false;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file for writing:" << filename;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 写入表头
    out << "TagName,Description,Type,Unit,MinValue,MaxValue,"
        << "Deadband,UpdateRate,AlarmLo,AlarmHi,Address,DataType\n";

    // 写入数据
    QList<VariableDefinition*> allVars = loadAllVariables();
    for (VariableDefinition *var : allVars) {
        out << "\"" << var->tagName() << "\","
            << "\"" << var->description() << "\","
            << static_cast<int>(var->type()) << ","
            << static_cast<int>(var->unit()) << ","
            << var->minValue() << ","
            << var->maxValue() << ","
            << var->deadband() << ","
            << var->updateRate() << ","
            << var->alarmLo() << ","
            << var->alarmHi() << ","
            << "\"" << var->address() << "\","
            << "\"" << var->dataType() << "\"\n";
    }

    file.close();
    return true;
}

bool VariableDatabase::importFromCsv(const QString &filename)
{
    if (!m_initialized) return false;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file for reading:" << filename;
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // 读取表头
    QString header = in.readLine();
    QStringList headers = parseCsvLine(header);

    if (headers.size() < 2) {
        qCritical() << "Invalid CSV format";
        return false;
    }

    int successCount = 0;
    int failCount = 0;

    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) continue;

        QStringList fields = parseCsvLine(line);
        if (fields.size() < headers.size()) {
            failCount++;
            continue;
        }

        QString tagName = fields[0];
        if (tagName.isEmpty()) {
            failCount++;
            continue;
        }

        // 创建变量
        VariableDefinition *var = nullptr;

        // 尝试加载已存在的变量
        var = loadVariableDefinition(tagName);
        if (!var) {
            // 创建新变量（需要类型信息）
            int type = fields.value(2, "0").toInt();
            var = new VariableDefinition(tagName, static_cast<VariableType>(type));
        }

        // 设置其他字段
        if (fields.size() > 1) var->setDescription(fields[1]);
        if (fields.size() > 3) var->setUnit(static_cast<EngineeringUnit>(fields[3].toInt()));
        if (fields.size() > 4 && fields.size() > 5) {
            var->setRange(fields[4].toDouble(), fields[5].toDouble());
        }
        if (fields.size() > 6) var->setDeadband(fields[6].toDouble());
        if (fields.size() > 7) var->setUpdateRate(fields[7].toInt());
        if (fields.size() > 8 && fields.size() > 9) {
            var->setAlarmLimits(fields[8].toDouble(), fields[9].toDouble());
        }
        if (fields.size() > 10) var->setAddress(fields[10]);
        if (fields.size() > 11) var->setDataType(fields[11]);

        if (saveVariableDefinition(var)) {
            successCount++;
        } else {
            failCount++;
        }

        delete var;
    }

    file.close();

    if (!m_database.commit()) {
        m_database.rollback();
        qCritical() << "Failed to commit transaction";
        return false;
    }

    emit importCompleted(successCount, failCount);
    emit databaseChanged();

    return true;
}

QStringList VariableDatabase::parseCsvLine(const QString &line)
{
    QStringList fields;
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < line.length(); i++) {
        QChar ch = line[i];

        if (ch == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                i++; // 跳过下一个引号
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.append(field);
            field.clear();
        } else {
            field += ch;
        }
    }

    fields.append(field); // 添加最后一个字段
    return fields;
}

VariableDatabase::Statistics VariableDatabase::getStatistics() const
{
    Statistics stats = {0};

    if (!m_initialized) return stats;

    QSqlQuery query(m_database);

    // 总变量数
    query.prepare("SELECT COUNT(*) FROM variable_definitions");
    if (query.exec() && query.next()) {
        stats.totalVariables = query.value(0).toInt();
    } else {
        QString error = query.lastError().text();
        qWarning() << "Failed to get total variables:" << error;
    }

    // 模拟量变量
    query.prepare(R"(
        SELECT COUNT(*) FROM variable_definitions
        WHERE type IN (0, 1)  -- AI, AO
    )");
    if (query.exec() && query.next()) {
        stats.analogVariables = query.value(0).toInt();
    }

    // 数字量变量
    query.prepare(R"(
        SELECT COUNT(*) FROM variable_definitions
        WHERE type IN (2, 3)  -- DI, DO
    )");
    if (query.exec() && query.next()) {
        stats.digitalVariables = query.value(0).toInt();
    }

    // 报警变量
    query.prepare("SELECT COUNT(*) FROM variable_definitions WHERE alarm_level > 0");
    if (query.exec() && query.next()) {
        stats.alarmVariables = query.value(0).toInt();
    }

    // 计算变量
    query.prepare(R"(
        SELECT COUNT(*) FROM variable_definitions
        WHERE type IN (10, 11, 12)  -- CALC, STAT, DERIVED
    )");
    if (query.exec() && query.next()) {
        stats.calculatedVariables = query.value(0).toInt();
    }

    // 区域数
    query.prepare("SELECT COUNT(*) FROM plant_areas");
    if (query.exec() && query.next()) {
        stats.areasCount = query.value(0).toInt();
    }

    // 设备数
    query.prepare("SELECT COUNT(*) FROM plant_devices");
    if (query.exec() && query.next()) {
        stats.devicesCount = query.value(0).toInt();
    }

    return stats;
}

bool VariableDatabase::backup(const QString &backupFile)
{
    if (!m_initialized) return false;

    QString sourceFile = m_database.databaseName();
    if (sourceFile.isEmpty()) {
        qCritical() << "Database file name is empty";
        return false;
    }

    if (sourceFile == ":memory:") {
        qCritical() << "Cannot backup in-memory database";
        return false;
    }

    // 关闭当前连接
    m_database.close();

    if (!QFile::copy(sourceFile, backupFile)) {
        qCritical() << "Failed to backup database file";
        // 重新打开数据库
        m_database.open();
        return false;
    }

    // 重新打开数据库
    if (!m_database.open()) {
        qCritical() << "Failed to reopen database after backup";
        return false;
    }

    return true;
}

bool VariableDatabase::restore(const QString &backupFile)
{
    if (!QFile::exists(backupFile)) {
        qCritical() << "Backup file does not exist:" << backupFile;
        return false;
    }

    QString dbFile = m_database.databaseName();

    // 关闭数据库
    m_database.close();

    // 删除现有数据库文件
    if (dbFile != ":memory:" && QFile::exists(dbFile)) {
        if (!QFile::remove(dbFile)) {
            qCritical() << "Failed to remove existing database file";
            return false;
        }
    }

    // 恢复备份
    if (!QFile::copy(backupFile, dbFile)) {
        qCritical() << "Failed to restore backup file";
        return false;
    }

    // 重新打开数据库
    if (!m_database.open()) {
        qCritical() << "Failed to open restored database";
        return false;
    }

    // 清空缓存
    clearCache();

    emit databaseChanged();

    return true;
}

bool VariableDatabase::batchSave(const QList<VariableDefinition*> &variables)
{
    if (!m_initialized) return false;

    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    int successCount = 0;
    for (VariableDefinition *var : variables) {
        if (saveVariableDefinition(var)) {
            successCount++;
        }
    }

    if (!m_database.commit()) {
        m_database.rollback();
        return false;
    }

    return successCount == variables.size();
}

bool VariableDatabase::batchDelete(const QStringList &tagNames)
{
    if (!m_initialized) return false;

    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    int successCount = 0;
    for (const QString &tagName : tagNames) {
        if (deleteVariableDefinition(tagName)) {
            successCount++;
        }
    }

    if (!m_database.commit()) {
        m_database.rollback();
        return false;
    }

    return successCount == tagNames.size();
}

bool VariableDatabase::createVersion(const QString &versionName,
                                     const QString &description)
{
    if (!m_initialized) return false;

    QSqlQuery query(m_database);

    // 创建版本记录
    query.prepare(R"(
        INSERT INTO variable_versions (version_name, description)
        VALUES (?, ?)
    )");
    query.addBindValue(versionName);
    query.addBindValue(description);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to create version:" << error;
        return false;
    }

    int versionId = query.lastInsertId().toInt();

    // 保存当前所有变量的状态
    QList<VariableDefinition*> allVars = loadAllVariables();

    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    for (VariableDefinition *var : allVars) {
        QJsonObject varObj;
        varObj["tagName"] = var->tagName();
        varObj["description"] = var->description();
        varObj["type"] = static_cast<int>(var->type());
        varObj["unit"] = static_cast<int>(var->unit());
        varObj["minValue"] = var->minValue();
        varObj["maxValue"] = var->maxValue();
        varObj["deadband"] = var->deadband();
        varObj["updateRate"] = var->updateRate();
        varObj["alarmLo"] = var->alarmLo();
        varObj["alarmHi"] = var->alarmHi();
        varObj["alarmLevel"] = static_cast<int>(var->alarmLevel());
        varObj["address"] = var->address();
        varObj["dataType"] = var->dataType();
        varObj["formatString"] = var->formatString();

        QJsonDocument doc(varObj);
        QString jsonData = doc.toJson(QJsonDocument::Compact);

        query.prepare(R"(
            INSERT INTO version_details (version_id, tag_name, variable_data)
            VALUES (?, ?, ?)
        )");
        query.addBindValue(versionId);
        query.addBindValue(var->tagName());
        query.addBindValue(jsonData);

        if (!query.exec()) {
            m_database.rollback();
            QString error = query.lastError().text();
            qCritical() << "Failed to save variable to version:" << error;
            return false;
        }
    }

    if (!m_database.commit()) {
        m_database.rollback();
        return false;
    }

    return true;
}

bool VariableDatabase::restoreVersion(const QString &versionName)
{
    if (!m_initialized) return false;

    QSqlQuery query(m_database);

    // 获取版本ID
    query.prepare("SELECT version_id FROM variable_versions WHERE version_name = ?");
    query.addBindValue(versionName);

    if (!query.exec() || !query.next()) {
        QString error = query.lastError().text();
        qCritical() << "Version not found:" << versionName << "Error:" << error;
        return false;
    }

    int versionId = query.value(0).toInt();

    // 加载版本数据
    query.prepare(R"(
        SELECT tag_name, variable_data
        FROM version_details
        WHERE version_id = ?
    )");
    query.addBindValue(versionId);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qCritical() << "Failed to load version data:" << error;
        return false;
    }

    if (!m_database.transaction()) {
        qCritical() << "Failed to start transaction";
        return false;
    }

    // 恢复每个变量
    while (query.next()) {
        QString tagName = query.value(0).toString();
        QString jsonData = query.value(1).toString();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        if (doc.isNull()) {
            qWarning() << "Invalid JSON data for variable:" << tagName;
            continue;
        }

        QJsonObject varObj = doc.object();
        VariableType type = static_cast<VariableType>(varObj["type"].toInt());

        VariableDefinition *var = new VariableDefinition(tagName, type);

        // 恢复属性
        if (varObj.contains("description")) {
            var->setDescription(varObj["description"].toString());
        }
        if (varObj.contains("unit")) {
            var->setUnit(static_cast<EngineeringUnit>(varObj["unit"].toInt()));
        }
        if (varObj.contains("minValue") && varObj.contains("maxValue")) {
            var->setRange(varObj["minValue"].toDouble(),
                          varObj["maxValue"].toDouble());
        }
        if (varObj.contains("deadband")) {
            var->setDeadband(varObj["deadband"].toDouble());
        }
        if (varObj.contains("updateRate")) {
            var->setUpdateRate(varObj["updateRate"].toInt());
        }
        if (varObj.contains("alarmLo") && varObj.contains("alarmHi")) {
            var->setAlarmLimits(varObj["alarmLo"].toDouble(),
                                varObj["alarmHi"].toDouble());
        }
        if (varObj.contains("alarmLevel")) {
            var->setAlarmLevel(static_cast<AlarmLevel>(varObj["alarmLevel"].toInt()));
        }
        if (varObj.contains("address")) {
            var->setAddress(varObj["address"].toString());
        }
        if (varObj.contains("dataType")) {
            var->setDataType(varObj["dataType"].toString());
        }
        if (varObj.contains("formatString")) {
            var->setFormatString(varObj["formatString"].toString());
        }

        if (!saveVariableDefinition(var)) {
            m_database.rollback();
            delete var;
            return false;
        }

        delete var;
    }

    if (!m_database.commit()) {
        m_database.rollback();
        return false;
    }

    // 清空缓存
    clearCache();

    emit databaseChanged();

    return true;
}

QStringList VariableDatabase::getVersions() const
{
    QStringList versions;

    if (!m_initialized) return versions;

    QSqlQuery query(m_database);
    query.prepare("SELECT version_name FROM variable_versions ORDER BY created_time DESC");

    if (query.exec()) {
        while (query.next()) {
            versions.append(query.value(0).toString());
        }
    } else {
        QString error = query.lastError().text();
        qWarning() << "Failed to get versions:" << error;
    }

    return versions;
}

void VariableDatabase::clearCache()
{
    QMutexLocker locker(&m_cacheMutex);
    qDeleteAll(m_variableCache);
    m_variableCache.clear();
}

void VariableDatabase::updateCache(VariableDefinition *var)
{
 /*
 * 更新缓存 - 注意：此函数会接管传入的VariableDefinition对象
 * 调用者不应再使用传入的指针
 */
    QMutexLocker locker(&m_cacheMutex);
    if (var) {
        if (m_variableCache.contains(var->tagName())) {
            delete m_variableCache[var->tagName()];
            m_variableCache.remove(var->tagName());
        }
        // 创建新的变量对象（需要深拷贝）
        VariableDefinition *newVar = new VariableDefinition(var->tagName(), var->type());
        // 这里需要复制所有属性
        newVar->setDescription(var->description());
        newVar->setUnit(var->unit());
        newVar->setRange(var->minValue(), var->maxValue());
        newVar->setDeadband(var->deadband());
        newVar->setInitialValue(var->initialValue());
        newVar->setUpdateRate(var->updateRate());
        newVar->setPriority(var->priority());
        newVar->setAlarmLimits(var->alarmLo(), var->alarmHi(),
                               var->alarmLoLo(), var->alarmHiHi());
        newVar->setAlarmLevel(var->alarmLevel());
        newVar->setHistoryEnabled(var->historyEnabled());
        newVar->setHistoryInterval(var->historyInterval());
        newVar->setWritable(var->writable());
        newVar->setAccessGroup(var->accessGroup());
        newVar->setAddress(var->address());
        newVar->setDataType(var->dataType());
        newVar->setFormatString(var->formatString());

        m_variableCache.insert(var->tagName(), newVar);
    }
}

void VariableDatabase::removeFromCache(const QString &tagName)
{
    QMutexLocker locker(&m_cacheMutex);
    if (m_variableCache.contains(tagName)) {
        delete m_variableCache.take(tagName);
    }
}

// VariableDBManager 实现
VariableDBManager* VariableDBManager::m_instance = nullptr;

VariableDBManager::VariableDBManager(QObject *parent)
    : QObject(parent)
{
}

VariableDBManager::~VariableDBManager()
{
    m_instance = nullptr;
}

VariableDBManager* VariableDBManager::instance()
{
    if (!m_instance) {
        m_instance = new VariableDBManager();
    }
    return m_instance;
}

bool VariableDBManager::syncDatabases()
{
    // 同步主数据库到配置数据库
    QList<VariableDefinition*> mainVars = m_mainDB.loadAllVariables();

    m_configDB.clearCache();

    bool success = true;
    for (VariableDefinition *var : mainVars) {
        if (!m_configDB.saveVariableDefinition(var)) {
            success = false;
            break;
        }
    }

    return success;
}

bool VariableDBManager::autoBackup(bool enable)
{
    Q_UNUSED(enable);
    // 实现自动备份逻辑
    // 例如：每天备份一次，或变量数量变化超过阈值时备份
    return true;
}

} // namespace Industrial
