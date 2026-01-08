// VariableDatabase.h
#ifndef VARIABLEDATABASE_H
#define VARIABLEDATABASE_H

#include"variablesystem.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
namespace Industrial {

class VariableDatabase : public QObject {
    Q_OBJECT
public:
    explicit VariableDatabase(QObject *parent = nullptr);
    ~VariableDatabase();

    bool initialize(const QString &connectionString = ":memory:");
    bool isInitialized() const { return m_initialized; }
     QString databaseName() const;
   // QString databaseName() const { return m_databaseName; }


    // ==================== 变量定义管理 ====================
    bool saveVariableDefinition(VariableDefinition *var);
    bool updateVariableDefinition(VariableDefinition *var);
    bool deleteVariableDefinition(const QString &tagName);
    VariableDefinition* loadVariableDefinition(const QString &tagName);
    QList<VariableDefinition*> loadAllVariables();

    // ==================== 变量组管理 ====================
    bool saveVariableGroup(VariableGroup *group);
    VariableGroup* loadVariableGroup(const QString &groupName);

    // ==================== 区域管理 ====================
    bool savePlantArea(PlantArea *area);
    PlantArea* loadPlantArea(const QString &areaCode);

    // ==================== 查询功能 ====================
    // 按类型查询
    QList<VariableDefinition*> findVariablesByType(VariableType type);

    // 按区域查询
    QList<VariableDefinition*> findVariablesByArea(const QString &areaCode);

    // 按设备查询
    QList<VariableDefinition*> findVariablesByDevice(const QString &deviceName);

    // 报警相关查询
    QList<VariableDefinition*> findAlarmVariables();
    QList<VariableDefinition*> findVariablesWithAlarm(double value);

    // 搜索功能
    QList<VariableDefinition*> searchVariables(const QString &keyword);

    // ==================== 导入导出 ====================
    bool exportToJson(const QString &filename);
    bool importFromJson(const QString &filename);

    bool exportToCsv(const QString &filename);
    bool importFromCsv(const QString &filename);

    // ==================== 统计功能 ====================
    struct Statistics {
        int totalVariables;
        int analogVariables;
        int digitalVariables;
        int alarmVariables;
        int calculatedVariables;
        int areasCount;
        int devicesCount;
    };

    Statistics getStatistics() const;

    // ==================== 备份恢复 ====================
    bool backup(const QString &backupFile);
    bool restore(const QString &backupFile);

    // ==================== 批量操作 ====================
    bool batchSave(const QList<VariableDefinition*> &variables);
    bool batchDelete(const QStringList &tagNames);

    // ==================== 版本管理 ====================
    bool createVersion(const QString &versionName,
                       const QString &description = "");
    bool restoreVersion(const QString &versionName);
    QStringList getVersions() const;

    QStringList parseCsvLine(const QString &line);

signals:
    void databaseChanged();
    void variableSaved(const QString &tagName);
    void variableDeleted(const QString &tagName);
    void importCompleted(int successCount, int failCount);

private:
    bool createTables();
    bool createIndexes();
    bool createTriggers();

    QSqlDatabase m_database;
    bool m_initialized;

    // 缓存
    QMap<QString, VariableDefinition*> m_variableCache;
    mutable QMutex m_cacheMutex;
public:
    // 清理缓存
    void clearCache();
    void updateCache(VariableDefinition *var);
    void removeFromCache(const QString &tagName);
private:
    // ... 原有私有成员 ...
    QString m_databaseName;  // 添加这个成员变量
};

// ==================== 变量数据库管理器 ====================
class VariableDBManager : public QObject {
    Q_OBJECT
public:
    static VariableDBManager* instance();

    VariableDatabase* mainDatabase() { return &m_mainDB; }
    VariableDatabase* historyDatabase() { return &m_historyDB; }
    VariableDatabase* configDatabase() { return &m_configDB; }

    // 数据库同步
    bool syncDatabases();
    bool autoBackup(bool enable);

private:
    VariableDBManager(QObject *parent = nullptr);
    ~VariableDBManager();

    VariableDatabase m_mainDB;      // 主变量库
    VariableDatabase m_historyDB;   // 历史数据
    VariableDatabase m_configDB;    // 配置备份

    static VariableDBManager* m_instance;
};

} // namespace Industrial

#endif // VARIABLEDATABASE_H
