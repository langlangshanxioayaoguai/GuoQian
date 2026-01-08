// VariableSystem.h
#ifndef VARIABLESYSTEM_H
#define VARIABLESYSTEM_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QDateTime>
#include <QVariant>
#include <QStringList>
#include <QReadWriteLock>
#include <QMutex>
#include <QScopedPointer>
#include <functional>

namespace Industrial {

// ==================== 变量基本类型 ====================
enum VariableType {
    TYPE_AI = 0,
    TYPE_AO = 1,
    TYPE_DI = 2,
    TYPE_DO = 3,
    TYPE_PI = 4,
    TYPE_PO = 5,
    TYPE_CALC = 10,
    TYPE_STAT = 11,
    TYPE_DERIVED = 12,
    TYPE_STATUS = 20,
    TYPE_ALARM = 21,
    TYPE_EVENT = 22,
    TYPE_SETPOINT = 30,
    TYPE_PID = 31,
    TYPE_CONTROL = 32,
    TYPE_SYSTEM = 40,
    TYPE_CONFIG = 41,
    TYPE_DIAG = 42
};

// ==================== 工程单位 ====================
enum EngineeringUnit {
    UNIT_NONE = 0,
    UNIT_TEMPERATURE,
    UNIT_PRESSURE,
    UNIT_FLOW,
    UNIT_LEVEL,
    UNIT_SPEED,
    UNIT_VOLTAGE,
    UNIT_CURRENT,
    UNIT_POWER,
    UNIT_ENERGY,
    UNIT_PERCENT,
    UNIT_COUNT,
    UNIT_TIME,
    UNIT_FREQUENCY,
    UNIT_WEIGHT,
    UNIT_LENGTH
};

// ==================== 变量质量 ====================
enum DataQuality {
    QUALITY_GOOD = 0,
    QUALITY_BAD = 1,
    QUALITY_UNCERTAIN = 2,
    QUALITY_OLD = 3,
    QUALITY_COMM_FAIL = 4,
    QUALITY_OUT_RANGE = 5,
    QUALITY_SENSOR_FAIL = 6,
    QUALITY_CALIBRATION = 7,
    QUALITY_MAINTENANCE = 8
};

// ==================== 报警级别 ====================
enum AlarmLevel {
    ALARM_NONE = 0,
    ALARM_INFO = 1,
    ALARM_WARNING = 2,
    ALARM_MINOR = 3,
    ALARM_MAJOR = 4,
    ALARM_CRITICAL = 5
};

// ==================== 辅助函数声明 ====================
QString engineeringUnitToString(EngineeringUnit unit);
EngineeringUnit stringToEngineeringUnit(const QString& unitStr);
QString dataQualityToString(DataQuality quality);
QString alarmLevelToString(AlarmLevel level);
QString variableTypeToString(VariableType type);

bool isTemperatureUnit(EngineeringUnit unit);
bool isPressureUnit(EngineeringUnit unit);
bool isFlowUnit(EngineeringUnit unit);

// 单位换算函数
double celsiusToFahrenheit(double celsius);
double fahrenheitToCelsius(double fahrenheit);
double celsiusToKelvin(double celsius);
double kelvinToCelsius(double kelvin);
double mpaToBar(double mpa);
double barToMpa(double bar);
double m3hToLmin(double m3h);
double lminToM3h(double lmin);

// ==================== 变量命名规范 ====================
class VariableNaming {
public:
    static QString generateName(const QString &area, const QString &device,
                                const QString &variable, const QString &suffix = "PV") {
        return QString("%1.%2.%3.%4").arg(area).arg(device).arg(variable).arg(suffix);
    }

    static const QString SUFFIX_PV;
    static const QString SUFFIX_SP;
    static const QString SUFFIX_OUT;
    static const QString SUFFIX_ALM;
    static const QString SUFFIX_ACK;
    static const QString SUFFIX_HI;
    static const QString SUFFIX_LO;
    static const QString SUFFIX_HIHI;
    static const QString SUFFIX_LOLO;
    static const QString SUFFIX_AVG;
    static const QString SUFFIX_SUM;
    static const QString SUFFIX_RATE;
};

// ==================== 转换函数基类 ====================
class ConversionFunction {
public:
    virtual ~ConversionFunction() = default;

    // 原生类型转换（高性能）
    virtual double rawToEngineering(double rawValue) const = 0;
    virtual double engineeringToRaw(double engValue) const = 0;

    // QVariant转换（兼容性）
    virtual QVariant convertToEngineering(QVariant rawValue) const {
        if (rawValue.canConvert<double>()) {
            return QVariant(rawToEngineering(rawValue.toDouble()));
        }
        return rawValue;
    }

    virtual QVariant convertToRaw(QVariant engValue) const {
        if (engValue.canConvert<double>()) {
            return QVariant(engineeringToRaw(engValue.toDouble()));
        }
        return engValue;
    }

    virtual ConversionFunction* clone() const = 0;
};

// ==================== 变量定义类 ====================
class VariableDefinition : public QObject {
    Q_OBJECT

    // 原有属性保持不变
    Q_PROPERTY(QString tagName READ tagName CONSTANT)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(VariableType type READ type CONSTANT)//在对象的生命周期类不会改变CONSTANT
    Q_PROPERTY(EngineeringUnit unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(double minValue READ minValue WRITE setMinValue NOTIFY rangeChanged)
    Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue NOTIFY rangeChanged)
    Q_PROPERTY(double rawMinValue READ rawMinValue WRITE setRawMinValue NOTIFY rawRangeChanged)
    Q_PROPERTY(double rawMaxValue READ rawMaxValue WRITE setRawMaxValue NOTIFY rawRangeChanged)
    Q_PROPERTY(double deadband READ deadband WRITE setDeadband NOTIFY deadbandChanged)
    Q_PROPERTY(int updateRate READ updateRate WRITE setUpdateRate NOTIFY updateRateChanged)

    // 值相关属性
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(DataQuality quality READ quality NOTIFY qualityChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)

    // ✅ 新增：类型特定的属性（用于高效访问）
    Q_PROPERTY(double doubleValue READ doubleValue NOTIFY valueChanged)
    Q_PROPERTY(bool boolValue READ boolValue NOTIFY valueChanged)
    Q_PROPERTY(int intValue READ intValue NOTIFY valueChanged)
    Q_PROPERTY(QString stringValue READ stringValue NOTIFY valueChanged)

public:
    explicit VariableDefinition(const QString &tagName,
                                VariableType type,
                                QObject *parent = nullptr);
    ~VariableDefinition();

    // 复制构造函数
    VariableDefinition(const VariableDefinition& other);
    VariableDefinition& operator=(const VariableDefinition& other);

    // ==================== 基本信息 ====================
    QString tagName() const { return m_tagName; }
    QString description() const { return m_description; }
    void setDescription(const QString &desc);

    VariableType type() const { return m_type; }
    EngineeringUnit unit() const { return m_unit; }
    void setUnit(EngineeringUnit unit);

    // ==================== 工程参数 ====================
    double minValue() const { return m_minValue; }
    void setMinValue(double minValue);
    double maxValue() const { return m_maxValue; }
    void setMaxValue(double maxValue);
    void setRange(double min, double max);

    double rawMinValue() const { return m_rawMinValue; }
    void setRawMinValue(double rawMin);
    double rawMaxValue() const { return m_rawMaxValue; }
    void setRawMaxValue(double rawMax);
    void setRawRange(double rawMin, double rawMax);

    double scaleFactor() const { return m_scaleFactor; }
    double offset() const { return m_offset; }
    void setScaling(double scaleFactor, double offset);

    double deadband() const { return m_deadband; }
    void setDeadband(double deadband);

    double initialValue() const { return m_initialValue; }
    void setInitialValue(double value);

    // ==================== 更新参数 ====================
    int updateRate() const { return m_updateRate; }
    void setUpdateRate(int rate);

    int priority() const { return m_priority; }
    void setPriority(int priority);

    // ==================== 值访问接口 ====================
    // ✅ 核心值访问（返回QVariant，保持兼容）
    QVariant value() const;

    // ✅ 新增：高效类型特定访问
    double doubleValue() const;
    bool boolValue() const;
    int intValue() const;
    QString stringValue() const;

    // ✅ 新增：直接原生值访问（最高性能）
    double directDoubleValue() const { return m_nativeValue.asDouble; }
    bool directBoolValue() const { return m_nativeValue.asBool; }
    int directIntValue() const { return m_nativeValue.asInt; }

    DataQuality quality() const;
    QDateTime timestamp() const;

    // ==================== 值设置接口 ====================
    // ✅ 原有接口（保持兼容）
    Q_INVOKABLE void setValue(QVariant newValue,
                              const QDateTime& timestamp = QDateTime::currentDateTime(),
                              DataQuality quality = QUALITY_GOOD);

    // ✅ 新增：高效类型特定设置
    void setDoubleValue(double value,
                        const QDateTime& timestamp = QDateTime::currentDateTime(),
                        DataQuality quality = QUALITY_GOOD);

    void setBoolValue(bool value,
                      const QDateTime& timestamp = QDateTime::currentDateTime(),
                      DataQuality quality = QUALITY_GOOD);

    void setIntValue(int value,
                     const QDateTime& timestamp = QDateTime::currentDateTime(),
                     DataQuality quality = QUALITY_GOOD);

    void setStringValue(const QString& value,
                        const QDateTime& timestamp = QDateTime::currentDateTime(),
                        DataQuality quality = QUALITY_GOOD);

    // ==================== 报警参数 ====================
    void setAlarmLimits(double lo, double hi, double lolo = 0, double hihi = 0);
    double alarmLo() const { return m_alarmLo; }
    double alarmHi() const { return m_alarmHi; }
    double alarmLoLo() const { return m_alarmLoLo; }
    double alarmHiHi() const { return m_alarmHiHi; }

    AlarmLevel alarmLevel() const { return m_alarmLevel; }
    void setAlarmLevel(AlarmLevel level);

    // ==================== 历史记录 ====================
    bool historyEnabled() const { return m_historyEnabled; }
    void setHistoryEnabled(bool enabled);

    int historyInterval() const { return m_historyInterval; }
    void setHistoryInterval(int interval);

    // ==================== 安全性 ====================
    bool writable() const { return m_writable; }
    void setWritable(bool writable);

    QString accessGroup() const { return m_accessGroup; }
    void setAccessGroup(const QString &group);

    // ==================== 地址映射 ====================
    QString address() const { return m_address; }
    void setAddress(const QString &address);

    QString dataType() const { return m_dataType; }
    void setDataType(const QString &type);

    // ==================== 显示格式 ====================
    QString formatString() const { return m_format; }
    void setFormatString(const QString &format);

    QString unitSuffix() const { return m_unitSuffix; }
    void setUnitSuffix(const QString &suffix);

    // ==================== 关联变量 ====================
    void addRelatedVariable(const QString &tagName);
    QStringList relatedVariables() const { return m_relatedVariables; }

    // ==================== 转换功能 ====================
    // QVariant版本（兼容性）
    QVariant rawToEngineering(QVariant rawValue) const;
    QVariant engineeringToRaw(QVariant engValue) const;

    // double版本（高性能）
    double rawToEngineering(double rawValue) const;
    double engineeringToRaw(double engValue) const;

    void setConversionFunction(ConversionFunction* func);
    ConversionFunction* conversionFunction() const { return m_conversionFunc.data(); }

    double convertToUnit(double value, const QString& targetUnit) const;
    QStringList supportedUnits() const;

    // ==================== 验证功能 ====================
    bool validate() const;
    QStringList getValidationErrors() const;
    bool validateRange() const;
    bool validateAlarmLimits() const;

    // ==================== 克隆功能 ====================
    VariableDefinition* clone(const QString& newTagName = "") const;

    // ==================== 缓存管理 ====================
    void invalidateCache();
    bool isCacheValid() const;

    // ==================== 报警检查 ====================
    AlarmLevel checkAlarmFast(double value) const;
    bool isInAlarmFast(double value) const;

    AlarmLevel checkAlarm() const;
    bool isInAlarm() const;

    // ✅ 新增：根据当前值的报警检查
    AlarmLevel checkCurrentAlarm() const;

private:
    // ==================== 存储类型定义 ====================
    enum StorageType {
        ST_Invalid = 0,
        ST_Double,
        ST_Bool,
        ST_Int,
        ST_Long,
        ST_String
    };

    // ==================== 原生值存储 ====================
    union NativeValue {
        double asDouble;
        bool asBool;
        int asInt;
        qint64 asLong;

        NativeValue() : asDouble(0.0) {}
    };

    // ==================== 缓存结构 ====================
    struct ConversionCache {
        double scaleFactor = 1.0;
        double offset = 0.0;
        bool valid = false;

        void clear() {
            scaleFactor = 1.0;
            offset = 0.0;
            valid = false;
        }
    };

    struct AlarmCache {
        bool hasCriticalAlarm : 1;
        bool hasMajorAlarm : 1;
        bool hasMinorAlarm : 1;
        double criticalLoLo;
        double criticalHiHi;
        double majorLo;
        double majorHi;
        bool valid : 1;

        AlarmCache() :
            hasCriticalAlarm(false),
            hasMajorAlarm(false),
            hasMinorAlarm(false),
            criticalLoLo(0.0),
            criticalHiHi(0.0),
            majorLo(0.0),
            majorHi(0.0),
            valid(false) {}

        void clear() {
            hasCriticalAlarm = false;
            hasMajorAlarm = false;
            hasMinorAlarm = false;
            valid = false;
        }
    };

    // ==================== 私有方法 ====================
    void updateScalingParameters();
    void updateConversionCache() const;
    void updateAlarmCache() const;
    double calculateScaleFactor() const;
    double calculateOffset() const;

    // ✅ 新增：内部值设置帮助方法
    void setValueInternal(StorageType type, const NativeValue& nativeValue,
                          const QString& stringValue,
                          const QDateTime& timestamp,
                          DataQuality quality);

    // ✅ 新增：QVariant缓存管理
    QVariant getCachedVariant() const;
    void invalidateVariantCache();

    // ✅ 新增：死区检查
    bool checkDeadband(double oldValue, double newValue) const;
    bool checkDeadband(bool oldValue, bool newValue) const;
    bool checkDeadband(int oldValue, int newValue) const;

signals:
    void descriptionChanged(const QString &description);
    void unitChanged(EngineeringUnit unit);
    void rangeChanged(double min, double max);
    void rawRangeChanged(double rawMin, double rawMax);
    void deadbandChanged(double deadband);
    void updateRateChanged(int rate);
    void alarmLimitsChanged();
    void scalingChanged(double scaleFactor, double offset);
    void unitSuffixChanged(const QString& suffix);

    void valueChanged(QVariant newValue);
    void valueChangedWithInfo(QVariant newValue,
                              const QDateTime& timestamp,
                              DataQuality quality);
    void qualityChanged(DataQuality newQuality);
    void timestampChanged(const QDateTime& newTimestamp);

private:
    // 基本信息
    QString m_tagName;
    QString m_description;
    VariableType m_type;
    EngineeringUnit m_unit;
    QString m_unitSuffix;

    // 工程范围
    double m_minValue;
    double m_maxValue;

    // 原始值范围
    double m_rawMinValue;
    double m_rawMaxValue;

    // 转换参数
    double m_scaleFactor;
    double m_offset;

    // 转换函数
    QScopedPointer<ConversionFunction> m_conversionFunc;

    double m_deadband;
    double m_initialValue;

    // 更新参数
    int m_updateRate;
    int m_priority;

    // ==================== 值存储（混合方案） ====================
    // 1. 原生值存储（高性能）
    NativeValue m_nativeValue;
    QString m_stringValue;          // 字符串单独存储
    StorageType m_storageType;      // 当前存储类型

    // 2. QVariant缓存（按需生成）
    mutable QVariant m_cachedVariant;
    mutable bool m_variantCacheValid;

    // 3. 值元数据
    mutable QMutex m_valueMutex;
    QDateTime m_timestamp;
    DataQuality m_quality;
    bool m_valueValid;

    // 报警参数
    double m_alarmLo;
    double m_alarmHi;
    double m_alarmLoLo;
    double m_alarmHiHi;
    AlarmLevel m_alarmLevel;

    // 历史记录
    bool m_historyEnabled;
    int m_historyInterval;

    // 安全性
    bool m_writable;
    QString m_accessGroup;

    // 地址映射
    QString m_address;
    QString m_dataType;

    // 显示
    QString m_format;

    // 关联
    QStringList m_relatedVariables;

    // 缓存
    mutable ConversionCache m_conversionCache;
    mutable AlarmCache m_alarmCache;
    mutable bool m_cacheValid;
    mutable QMutex m_cacheMutex;
    //mutable QReadWriteLock m_cacheLock;
};
// ==================== 线性转换函数 ====================
class LinearConversion : public ConversionFunction {
public:
    LinearConversion(double rawMin, double rawMax, double engMin, double engMax);
    LinearConversion(double scaleFactor = 1.0, double offset = 0.0);

    double rawToEngineering(double rawValue) const override;
    double engineeringToRaw(double engValue) const override;
    ConversionFunction* clone() const override;

    double scaleFactor() const { return m_scaleFactor; }
    double offset() const { return m_offset; }

private:
    double m_scaleFactor;
    double m_offset;
};

// ==================== 单位转换管理器 ====================
class UnitConversionManager : public QObject {
    Q_OBJECT
public:
    static UnitConversionManager* instance();

    void registerConversion(EngineeringUnit fromUnit, const QString& toUnit,
                            std::function<double(double)> converter);

    double convert(double value, EngineeringUnit fromUnit,
                   const QString& toUnit) const;

    QStringList getSupportedUnits(EngineeringUnit unit) const;
    QString getUnitString(EngineeringUnit unit) const;

    void addCustomUnit(const QString& unitName, const QString& displayName,
                       EngineeringUnit baseUnit, double conversionFactor);

private:
    UnitConversionManager(QObject* parent = nullptr);
    ~UnitConversionManager();

    static UnitConversionManager* m_instance;
    QMap<QPair<EngineeringUnit, QString>, std::function<double(double)>> m_conversions;
    QMap<QString, QString> m_unitDisplayNames;
    QMap<QString, QPair<EngineeringUnit, double>> m_customUnits;
};

// ==================== 变量组 ====================
class VariableGroup : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString groupName READ groupName CONSTANT)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit VariableGroup(const QString &groupName, QObject *parent = nullptr);

    QString groupName() const { return m_groupName; }
    QString description() const { return m_description; }
    void setDescription(const QString &desc);

    void addVariable(VariableDefinition *var);
    void removeVariable(const QString &tagName);
    VariableDefinition* getVariable(const QString &tagName) const;
    QList<VariableDefinition*> variables() const;

    void addSubGroup(VariableGroup *group);
    QList<VariableGroup*> subGroups() const;

    int variableCount() const;
    int alarmCount() const;

signals:
    void descriptionChanged(const QString &description);
    void variableAdded(VariableDefinition *var);
    void variableRemoved(const QString &tagName);
    void alarmStatusChanged();

private:
    QString m_groupName;
    QString m_description;
    QMap<QString, VariableDefinition*> m_variables;
    QList<VariableGroup*> m_subGroups;
};

// ==================== 区域管理 ====================
class PlantArea : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString areaName READ areaName CONSTANT)
    Q_PROPERTY(QString areaCode READ areaCode CONSTANT)

public:
    explicit PlantArea(const QString &areaName,
                       const QString &areaCode,
                       QObject *parent = nullptr);

    QString areaName() const { return m_areaName; }
    QString areaCode() const { return m_areaCode; }

    void addDevice(const QString &deviceName,
                   const QString &deviceType,
                   const QString &description = "");

    void removeDevice(const QString &deviceName);

    void addVariableGroup(VariableGroup *group);
    VariableGroup* getGroup(const QString &groupName) const;

    QList<VariableDefinition*> getAllVariables() const;
    QList<QString> getDeviceNames() const;

private:
    QString m_areaName;
    QString m_areaCode;
    QMap<QString, VariableGroup*> m_groups;
    QMap<QString, QString> m_devices;
};


} // namespace Industrial

#endif // VARIABLESYSTEM_H
