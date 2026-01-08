// variablesystem.cpp
#include "variablesystem.h"
#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>

namespace Industrial {

// ==================== 辅助函数实现 ====================
QString engineeringUnitToString(EngineeringUnit unit) {
    static const QMap<EngineeringUnit, QString> unitMap = {
        {UNIT_NONE, "None"},
        {UNIT_TEMPERATURE, "°C"},
        {UNIT_PRESSURE, "MPa"},
        {UNIT_FLOW, "m³/h"},
        {UNIT_LEVEL, "m"},
        {UNIT_SPEED, "m/s"},
        {UNIT_VOLTAGE, "V"},
        {UNIT_CURRENT, "A"},
        {UNIT_POWER, "kW"},
        {UNIT_ENERGY, "kWh"},
        {UNIT_PERCENT, "%"},
        {UNIT_COUNT, "count"},
        {UNIT_TIME, "s"},
        {UNIT_FREQUENCY, "Hz"},
        {UNIT_WEIGHT, "kg"},
        {UNIT_LENGTH, "m"}
    };
    return unitMap.value(unit, "Unknown");
}

EngineeringUnit stringToEngineeringUnit(const QString& unitStr) {
    static const QMap<QString, EngineeringUnit> strMap = {
        {"None", UNIT_NONE},
        {"°C", UNIT_TEMPERATURE}, {"C", UNIT_TEMPERATURE},
        {"MPa", UNIT_PRESSURE}, {"Bar", UNIT_PRESSURE},
        {"m³/h", UNIT_FLOW}, {"m3/h", UNIT_FLOW},
        {"m", UNIT_LEVEL},
        {"m/s", UNIT_SPEED},
        {"V", UNIT_VOLTAGE},
        {"A", UNIT_CURRENT},
        {"kW", UNIT_POWER},
        {"kWh", UNIT_ENERGY},
        {"%", UNIT_PERCENT},
        {"count", UNIT_COUNT},
        {"s", UNIT_TIME},
        {"Hz", UNIT_FREQUENCY},
        {"kg", UNIT_WEIGHT}
    };
    return strMap.value(unitStr, UNIT_NONE);
}

QString dataQualityToString(DataQuality quality) {
    static const char* qualityNames[] = {
        "Good", "Bad", "Uncertain", "Old",
        "Comm Fail", "Out of Range", "Sensor Fail",
        "Calibration", "Maintenance"
    };
    if (quality >= 0 && quality <= 8) {
        return QString(qualityNames[quality]);
    }
    return "Unknown";
}

QString alarmLevelToString(AlarmLevel level) {
    static const char* levelNames[] = {
        "None", "Info", "Warning", "Minor", "Major", "Critical"
    };
    if (level >= 0 && level <= 5) {
        return QString(levelNames[level]);
    }
    return "Unknown";
}

QString variableTypeToString(VariableType type) {
    static const char* typeNames[] = {
        "AI", "AO", "DI", "DO", "PI", "PO",
        "", "", "", "",
        "CALC", "STAT", "DERIVED",
        "", "", "", "", "", "",
        "STATUS", "ALARM", "EVENT",
        "", "", "", "", "", "",
        "SETPOINT", "PID", "CONTROL",
        "", "", "", "", "", "", "",
        "SYSTEM", "CONFIG", "DIAG"
    };
    if (type >= 0 && type <= 42) {
        return QString(typeNames[type]);
    }
    return "Unknown";
}

bool isTemperatureUnit(EngineeringUnit unit) {
    return unit == UNIT_TEMPERATURE;
}

bool isPressureUnit(EngineeringUnit unit) {
    return unit == UNIT_PRESSURE;
}

bool isFlowUnit(EngineeringUnit unit) {
    return unit == UNIT_FLOW;
}

double celsiusToFahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

double mpaToBar(double mpa) {
    return mpa * 10.0;
}

double barToMpa(double bar) {
    return bar / 10.0;
}

double m3hToLmin(double m3h) {
    return m3h * 1000.0 / 60.0;
}

double lminToM3h(double lmin) {
    return lmin * 60.0 / 1000.0;
}

}

// ==================== VariableDefinition 实现 ====================
namespace Industrial {

VariableDefinition::VariableDefinition(const QString &tagName,
                                       VariableType type,
                                       QObject *parent)
    : QObject(parent)
    , m_tagName(tagName)
    , m_type(type)
    , m_unit(UNIT_NONE)
    , m_minValue(0.0)
    , m_maxValue(100.0)
    , m_rawMinValue(0.0)
    , m_rawMaxValue(100.0)
    , m_scaleFactor(1.0)
    , m_offset(0.0)
    , m_deadband(0.1)
    , m_initialValue(0.0)
    , m_updateRate(1000)
    , m_priority(50)
    , m_storageType(ST_Invalid)
    , m_variantCacheValid(false)
    , m_quality(QUALITY_BAD)
    , m_valueValid(false)
    , m_alarmLo(10.0)
    , m_alarmHi(90.0)
    , m_alarmLoLo(5.0)
    , m_alarmHiHi(95.0)
    , m_alarmLevel(ALARM_WARNING)
    , m_historyEnabled(false)
    , m_historyInterval(60)
    , m_writable(true)
    , m_cacheValid(false)
{
    // 根据类型初始化存储
    switch (m_type) {
    case TYPE_AI:
    case TYPE_AO:
    case TYPE_CALC:
    case TYPE_STAT:
    case TYPE_DERIVED:
    case TYPE_SETPOINT:
    case TYPE_PID:
    case TYPE_CONTROL:
        m_storageType = ST_Double;
        m_nativeValue.asDouble = m_initialValue;
        break;
    case TYPE_DI:
    case TYPE_DO:
        m_storageType = ST_Bool;
        m_nativeValue.asBool = false;
        break;
    case TYPE_STATUS:
    case TYPE_ALARM:
    case TYPE_EVENT:
        m_storageType = ST_Int;
        m_nativeValue.asInt = 0;
        break;
    default:
        m_storageType = ST_Invalid;
        break;
    }

    updateScalingParameters();
}

VariableDefinition::~VariableDefinition() {
}

VariableDefinition::VariableDefinition(const VariableDefinition& other)
    : QObject(other.parent())
    , m_tagName(other.m_tagName)
    , m_description(other.m_description)
    , m_type(other.m_type)
    , m_unit(other.m_unit)
    , m_unitSuffix(other.m_unitSuffix)
    , m_minValue(other.m_minValue)
    , m_maxValue(other.m_maxValue)
    , m_rawMinValue(other.m_rawMinValue)
    , m_rawMaxValue(other.m_rawMaxValue)
    , m_scaleFactor(other.m_scaleFactor)
    , m_offset(other.m_offset)
    , m_conversionFunc(other.m_conversionFunc ? other.m_conversionFunc->clone() : nullptr)
    , m_deadband(other.m_deadband)
    , m_initialValue(other.m_initialValue)
    , m_updateRate(other.m_updateRate)
    , m_priority(other.m_priority)
    , m_storageType(other.m_storageType)
    , m_nativeValue(other.m_nativeValue)
    , m_stringValue(other.m_stringValue)
    , m_variantCacheValid(false)
    , m_quality(other.m_quality)
    , m_valueValid(other.m_valueValid)
    , m_timestamp(other.m_timestamp)
    , m_alarmLo(other.m_alarmLo)
    , m_alarmHi(other.m_alarmHi)
    , m_alarmLoLo(other.m_alarmLoLo)
    , m_alarmHiHi(other.m_alarmHiHi)
    , m_alarmLevel(other.m_alarmLevel)
    , m_historyEnabled(other.m_historyEnabled)
    , m_historyInterval(other.m_historyInterval)
    , m_writable(other.m_writable)
    , m_accessGroup(other.m_accessGroup)
    , m_address(other.m_address)
    , m_dataType(other.m_dataType)
    , m_format(other.m_format)
    , m_relatedVariables(other.m_relatedVariables)
    , m_cacheValid(false)
{
}

VariableDefinition& VariableDefinition::operator=(const VariableDefinition& other) {
    if (this != &other) {
        m_tagName = other.m_tagName;
        m_description = other.m_description;
        m_type = other.m_type;
        m_unit = other.m_unit;
        m_unitSuffix = other.m_unitSuffix;
        m_minValue = other.m_minValue;
        m_maxValue = other.m_maxValue;
        m_rawMinValue = other.m_rawMinValue;
        m_rawMaxValue = other.m_rawMaxValue;
        m_scaleFactor = other.m_scaleFactor;
        m_offset = other.m_offset;
        m_conversionFunc.reset(other.m_conversionFunc ? other.m_conversionFunc->clone() : nullptr);
        m_deadband = other.m_deadband;
        m_initialValue = other.m_initialValue;
        m_updateRate = other.m_updateRate;
        m_priority = other.m_priority;
        m_storageType = other.m_storageType;
        m_nativeValue = other.m_nativeValue;
        m_stringValue = other.m_stringValue;
        m_variantCacheValid = false;
        m_quality = other.m_quality;
        m_valueValid = other.m_valueValid;
        m_timestamp = other.m_timestamp;
        m_alarmLo = other.m_alarmLo;
        m_alarmHi = other.m_alarmHi;
        m_alarmLoLo = other.m_alarmLoLo;
        m_alarmHiHi = other.m_alarmHiHi;
        m_alarmLevel = other.m_alarmLevel;
        m_historyEnabled = other.m_historyEnabled;
        m_historyInterval = other.m_historyInterval;
        m_writable = other.m_writable;
        m_accessGroup = other.m_accessGroup;
        m_address = other.m_address;
        m_dataType = other.m_dataType;
        m_format = other.m_format;
        m_relatedVariables = other.m_relatedVariables;
        m_cacheValid = false;
    }
    return *this;
}

// ==================== 基本信息方法 ====================
void VariableDefinition::setDescription(const QString &desc) {
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged(desc);
    }
}

void VariableDefinition::setUnit(EngineeringUnit unit) {
    if (m_unit != unit) {
        m_unit = unit;
        invalidateVariantCache();
        emit unitChanged(unit);
    }
}

// ==================== 工程参数方法 ====================
void VariableDefinition::setMinValue(double minValue) {
    if (!qFuzzyCompare(m_minValue, minValue)) {
        m_minValue = minValue;
        updateScalingParameters();
        invalidateCache();
        emit rangeChanged(m_minValue, m_maxValue);
    }
}

void VariableDefinition::setMaxValue(double maxValue) {
    if (!qFuzzyCompare(m_maxValue, maxValue)) {
        m_maxValue = maxValue;
        updateScalingParameters();
        invalidateCache();
        emit rangeChanged(m_minValue, m_maxValue);
    }
}

void VariableDefinition::setRange(double min, double max) {
    if (!qFuzzyCompare(m_minValue, min) || !qFuzzyCompare(m_maxValue, max)) {
        m_minValue = min;
        m_maxValue = max;
        updateScalingParameters();
        invalidateCache();
        emit rangeChanged(min, max);
    }
}

void VariableDefinition::setRawMinValue(double rawMin) {
    if (!qFuzzyCompare(m_rawMinValue, rawMin)) {
        m_rawMinValue = rawMin;
        updateScalingParameters();
        invalidateCache();
        emit rawRangeChanged(m_rawMinValue, m_rawMaxValue);
    }
}

void VariableDefinition::setRawMaxValue(double rawMax) {
    if (!qFuzzyCompare(m_rawMaxValue, rawMax)) {
        m_rawMaxValue = rawMax;
        updateScalingParameters();
        invalidateCache();
        emit rawRangeChanged(m_rawMinValue, m_rawMaxValue);
    }
}

void VariableDefinition::setRawRange(double rawMin, double rawMax) {
    if (!qFuzzyCompare(m_rawMinValue, rawMin) || !qFuzzyCompare(m_rawMaxValue, rawMax)) {
        m_rawMinValue = rawMin;
        m_rawMaxValue = rawMax;
        updateScalingParameters();
        invalidateCache();
        emit rawRangeChanged(rawMin, rawMax);
    }
}

void VariableDefinition::setScaling(double scaleFactor, double offset) {
    if (!qFuzzyCompare(m_scaleFactor, scaleFactor) || !qFuzzyCompare(m_offset, offset)) {
        m_scaleFactor = scaleFactor;
        m_offset = offset;
        invalidateCache();
        emit scalingChanged(scaleFactor, offset);
    }
}

void VariableDefinition::setDeadband(double deadband) {
    if (!qFuzzyCompare(m_deadband, deadband)) {
        m_deadband = deadband;
        emit deadbandChanged(deadband);
    }
}

void VariableDefinition::setInitialValue(double value) {
    m_initialValue = value;
}

void VariableDefinition::setUpdateRate(int rate) {
    if (m_updateRate != rate) {
        m_updateRate = rate;
        emit updateRateChanged(rate);
    }
}

void VariableDefinition::setPriority(int priority) {
    m_priority = qBound(0, priority, 100);
}

// ==================== 值访问方法 ====================
QVariant VariableDefinition::value() const {
    return getCachedVariant();
}

double VariableDefinition::doubleValue() const {
    switch (m_storageType) {
    case ST_Double:
        return m_nativeValue.asDouble;
    case ST_Bool:
        return m_nativeValue.asBool ? 1.0 : 0.0;
    case ST_Int:
        return static_cast<double>(m_nativeValue.asInt);
    case ST_Long:
        return static_cast<double>(m_nativeValue.asLong);
    case ST_String:
        return m_stringValue.toDouble();
    default:
        return 0.0;
    }
}

bool VariableDefinition::boolValue() const {
    switch (m_storageType) {
    case ST_Bool:
        return m_nativeValue.asBool;
    case ST_Double:
        return !qFuzzyIsNull(m_nativeValue.asDouble);
    case ST_Int:
        return m_nativeValue.asInt != 0;
    case ST_Long:
        return m_nativeValue.asLong != 0;
    case ST_String: {
        QString lower = m_stringValue.toLower();
        return lower == "true" || lower == "1" || lower == "on" || lower == "yes";
    }
    default:
        return false;
    }
}

int VariableDefinition::intValue() const {
    switch (m_storageType) {
    case ST_Int:
        return m_nativeValue.asInt;
    case ST_Double:
        return static_cast<int>(m_nativeValue.asDouble);
    case ST_Bool:
        return m_nativeValue.asBool ? 1 : 0;
    case ST_Long:
        return static_cast<int>(m_nativeValue.asLong);
    case ST_String:
        return m_stringValue.toInt();
    default:
        return 0;
    }
}

QString VariableDefinition::stringValue() const {
    switch (m_storageType) {
    case ST_String:
        return m_stringValue;
    case ST_Double:
        if (!m_format.isEmpty()) {
            return QString::asprintf(m_format.toUtf8().constData(), m_nativeValue.asDouble);
        }
        return QString::number(m_nativeValue.asDouble, 'f', 6);
    case ST_Bool:
        return m_nativeValue.asBool ? "TRUE" : "FALSE";
    case ST_Int:
        return QString::number(m_nativeValue.asInt);
    case ST_Long:
        return QString::number(m_nativeValue.asLong);
    default:
        return QString();
    }
}

DataQuality VariableDefinition::quality() const {
    QMutexLocker locker(&m_valueMutex);
    return m_quality;
}

QDateTime VariableDefinition::timestamp() const {
    QMutexLocker locker(&m_valueMutex);
    return m_timestamp;
}

// ==================== 值设置方法 ====================
void VariableDefinition::setValue(QVariant newValue,
                                  const QDateTime& timestamp,
                                  DataQuality quality) {
    // 根据QVariant类型调用对应的设置方法
    switch (newValue.type()) {
    case QVariant::Double:
        setDoubleValue(newValue.toDouble(), timestamp, quality);
        break;
    case QVariant::Bool:
        setBoolValue(newValue.toBool(), timestamp, quality);
        break;
    case QVariant::Int:
        setIntValue(newValue.toInt(), timestamp, quality);
        break;
    case QVariant::LongLong:
        setIntValue(static_cast<int>(newValue.toLongLong()), timestamp, quality);
        break;
    case QVariant::String:
        setStringValue(newValue.toString(), timestamp, quality);
        break;
    default:
        // 尝试转换为double
        if (newValue.canConvert<double>()) {
            setDoubleValue(newValue.toDouble(), timestamp, quality);
        } else {
            qWarning() << "Variable" << m_tagName << ": Unsupported value type:" << newValue.typeName();
        }
        break;
    }
}

void VariableDefinition::setDoubleValue(double value,
                                        const QDateTime& timestamp,
                                        DataQuality quality) {
    QMutexLocker locker(&m_valueMutex);

    // 检查数据质量
    if (quality != QUALITY_GOOD) {
        NativeValue newValue;
        newValue.asDouble = value;
        setValueInternal(ST_Double, newValue, QString(), timestamp, quality);
        return;
    }

    // 死区检查（仅对模拟量）
    if ((m_type == TYPE_AI || m_type == TYPE_AO || m_type == TYPE_CALC) &&
        m_deadband > 0 && m_valueValid && m_storageType == ST_Double) {
        if (checkDeadband(m_nativeValue.asDouble, value)) {
            return;  // 变化小于死区，不更新
        }
    }

    // 范围检查（可选）
    if (m_minValue != m_maxValue) {
        if (value < m_minValue || value > m_maxValue) {
            quality = QUALITY_OUT_RANGE;
        }
    }

    NativeValue newValue;
    newValue.asDouble = value;
    setValueInternal(ST_Double, newValue, QString(), timestamp, quality);
}

void VariableDefinition::setBoolValue(bool value,
                                      const QDateTime& timestamp,
                                      DataQuality quality) {
    QMutexLocker locker(&m_valueMutex);

    // 死区检查（对于bool，只有值变化才更新）
    if (m_valueValid && m_storageType == ST_Bool) {
        if (checkDeadband(m_nativeValue.asBool, value)) {
            return;
        }
    }

    NativeValue newValue;
    newValue.asBool = value;
    setValueInternal(ST_Bool, newValue, QString(), timestamp, quality);
}

void VariableDefinition::setIntValue(int value,
                                     const QDateTime& timestamp,
                                     DataQuality quality) {
    QMutexLocker locker(&m_valueMutex);

    // 死区检查
    if (m_valueValid && m_storageType == ST_Int) {
        if (checkDeadband(m_nativeValue.asInt, value)) {
            return;
        }
    }

    NativeValue newValue;
    newValue.asInt = value;
    setValueInternal(ST_Int, newValue, QString(), timestamp, quality);
}

void VariableDefinition::setStringValue(const QString& value,
                                        const QDateTime& timestamp,
                                        DataQuality quality) {
    QMutexLocker locker(&m_valueMutex);

    // 对于字符串，只有值变化才更新（无死区）
    if (m_valueValid && m_storageType == ST_String && m_stringValue == value) {
        return;
    }

    NativeValue dummy;
    setValueInternal(ST_String, dummy, value, timestamp, quality);
}

// ==================== 报警相关方法 ====================
void VariableDefinition::setAlarmLimits(double lo, double hi, double lolo, double hihi) {
    if (!qFuzzyCompare(m_alarmLo, lo) || !qFuzzyCompare(m_alarmHi, hi) ||
        !qFuzzyCompare(m_alarmLoLo, lolo) || !qFuzzyCompare(m_alarmHiHi, hihi)) {
        m_alarmLo = lo;
        m_alarmHi = hi;
        m_alarmLoLo = lolo;
        m_alarmHiHi = hihi;
        invalidateCache();
        emit alarmLimitsChanged();
    }
}

void VariableDefinition::setAlarmLevel(AlarmLevel level) {
    if (m_alarmLevel != level) {
        m_alarmLevel = level;
    }
}

void VariableDefinition::setHistoryEnabled(bool enabled) {
    m_historyEnabled = enabled;
}

void VariableDefinition::setHistoryInterval(int interval) {
    m_historyInterval = interval;
}

void VariableDefinition::setWritable(bool writable) {
    m_writable = writable;
}

void VariableDefinition::setAccessGroup(const QString &group) {
    m_accessGroup = group;
}

void VariableDefinition::setAddress(const QString &address) {
    m_address = address;
}

void VariableDefinition::setDataType(const QString &type) {
    m_dataType = type;
}

void VariableDefinition::setFormatString(const QString &format) {
    if (m_format != format) {
        m_format = format;
        invalidateVariantCache();
    }
}

void VariableDefinition::setUnitSuffix(const QString &suffix) {
    if (m_unitSuffix != suffix) {
        m_unitSuffix = suffix;
        invalidateVariantCache();
        emit unitSuffixChanged(suffix);
    }
}

void VariableDefinition::addRelatedVariable(const QString &tagName) {
    if (!m_relatedVariables.contains(tagName)) {
        m_relatedVariables.append(tagName);
    }
}

// ==================== 转换功能 ====================
QVariant VariableDefinition::rawToEngineering(QVariant rawValue) const {
    if (m_conversionFunc) {
        return m_conversionFunc->convertToEngineering(rawValue);
    }

    if (rawValue.canConvert<double>()) {
        double raw = rawValue.toDouble();
        double eng = raw * scaleFactor() + offset();
        return QVariant(eng);
    }

    return rawValue;
}

QVariant VariableDefinition::engineeringToRaw(QVariant engValue) const {
    if (m_conversionFunc) {
        return m_conversionFunc->convertToRaw(engValue);
    }

    if (engValue.canConvert<double>()) {
        double eng = engValue.toDouble();
        double raw = (eng - offset()) / scaleFactor();
        return QVariant(raw);
    }

    return engValue;
}

double VariableDefinition::rawToEngineering(double rawValue) const {
    if (m_conversionFunc) {
        return m_conversionFunc->rawToEngineering(rawValue);
    }

    QMutexLocker locker(&m_cacheMutex);
    if (!m_conversionCache.valid) {
        locker.unlock();
        updateConversionCache();
        locker.relock();
    }

    return rawValue * m_conversionCache.scaleFactor + m_conversionCache.offset;
}

double VariableDefinition::engineeringToRaw(double engValue) const {
    if (m_conversionFunc) {
        return m_conversionFunc->engineeringToRaw(engValue);
    }

  QMutexLocker locker(&m_cacheMutex);
    if (!m_conversionCache.valid) {
        locker.unlock();
        updateConversionCache();
        locker.relock();
    }

    return (engValue - m_conversionCache.offset) / m_conversionCache.scaleFactor;
}

void VariableDefinition::setConversionFunction(ConversionFunction* func) {
    m_conversionFunc.reset(func);
    invalidateCache();
}

double VariableDefinition::convertToUnit(double value, const QString& targetUnit) const {
    // 简化的单位转换，实际项目中应该使用完整的单位转换管理器
    QString sourceUnit = engineeringUnitToString(m_unit);
    if (sourceUnit == "°C" && targetUnit == "°F") {
        return celsiusToFahrenheit(value);
    } else if (sourceUnit == "°F" && targetUnit == "°C") {
        return fahrenheitToCelsius(value);
    } else if (sourceUnit == "MPa" && targetUnit == "Bar") {
        return mpaToBar(value);
    } else if (sourceUnit == "Bar" && targetUnit == "MPa") {
        return barToMpa(value);
    }
    return value;
}

QStringList VariableDefinition::supportedUnits() const {
    QStringList units;
    units << engineeringUnitToString(m_unit);

    // 添加常用转换单位
    switch (m_unit) {
    case UNIT_TEMPERATURE:
        units << "°F" << "K";
        break;
    case UNIT_PRESSURE:
        units << "Bar" << "kPa" << "psi";
        break;
    case UNIT_FLOW:
        units << "L/min" << "L/s" << "GPM";
        break;
    default:
        break;
    }

    return units;
}

// ==================== 验证功能 ====================
bool VariableDefinition::validate() const {
    return validateRange() && validateAlarmLimits();
}

QStringList VariableDefinition::getValidationErrors() const {
    QStringList errors;

    if (!validateRange()) {
        errors << "Invalid range: min >= max";
    }

    if (!validateAlarmLimits()) {
        errors << "Invalid alarm limits";
    }

    if (m_tagName.isEmpty()) {
        errors << "Tag name is empty";
    }

    return errors;
}

bool VariableDefinition::validateRange() const {
    return m_minValue < m_maxValue && m_rawMinValue < m_rawMaxValue;
}

bool VariableDefinition::validateAlarmLimits() const {
    return m_alarmLoLo <= m_alarmLo && m_alarmLo <= m_alarmHi && m_alarmHi <= m_alarmHiHi;
}

// ==================== 克隆功能 ====================
VariableDefinition* VariableDefinition::clone(const QString& newTagName) const {
    VariableDefinition* clone = new VariableDefinition(newTagName.isEmpty() ? m_tagName : newTagName,
                                                       m_type, parent());
    *clone = *this;
    return clone;
}

// ==================== 缓存管理 ====================
void VariableDefinition::invalidateCache() {
    QMutexLocker locker(&m_cacheMutex);
    m_conversionCache.clear();
    m_alarmCache.clear();
    m_cacheValid = false;
}

bool VariableDefinition::isCacheValid() const {
    QMutexLocker locker(&m_cacheMutex);
    return m_cacheValid;
}

// ==================== 报警检查 ====================
AlarmLevel VariableDefinition::checkAlarmFast(double value) const {
    QMutexLocker locker(&m_cacheMutex);
    if (!m_alarmCache.valid) {
        locker.unlock();
        updateAlarmCache();
        locker.relock();
    }

    if (!m_alarmCache.valid) {
        return ALARM_NONE;
    }

    // 检查报警等级
    if (m_alarmCache.hasCriticalAlarm) {
        if (value <= m_alarmCache.criticalLoLo || value >= m_alarmCache.criticalHiHi) {
            return ALARM_CRITICAL;
        }
    }

    if (m_alarmCache.hasMajorAlarm) {
        if (value <= m_alarmCache.majorLo || value >= m_alarmCache.majorHi) {
            return ALARM_MAJOR;
        }
    }

    if (m_alarmCache.hasMinorAlarm) {
        if (value <= m_alarmLo || value >= m_alarmHi) {
            return ALARM_MINOR;
        }
    }

    return ALARM_NONE;
}

bool VariableDefinition::isInAlarmFast(double value) const {
    return checkAlarmFast(value) != ALARM_NONE;
}

AlarmLevel VariableDefinition::checkAlarm() const {
    if (!m_valueValid || m_quality != QUALITY_GOOD) {
        return ALARM_NONE;
    }

    // 只有数值类型才进行报警检查
    if (m_storageType == ST_Double || m_storageType == ST_Int || m_storageType == ST_Long) {
        return checkAlarmFast(doubleValue());
    }

    return ALARM_NONE;
}

bool VariableDefinition::isInAlarm() const {
    return checkAlarm() != ALARM_NONE;
}

AlarmLevel VariableDefinition::checkCurrentAlarm() const {
    return checkAlarm();
}

// ==================== 私有方法 ====================
void VariableDefinition::updateScalingParameters() {
    // 如果用户没有手动设置缩放参数，则自动计算
    if (qFuzzyCompare(m_scaleFactor, 1.0) && qFuzzyIsNull(m_offset)) {
        if (!qFuzzyCompare(m_rawMaxValue - m_rawMinValue, 0.0)) {
            m_scaleFactor = (m_maxValue - m_minValue) / (m_rawMaxValue - m_rawMinValue);
            m_offset = m_minValue - m_scaleFactor * m_rawMinValue;
            invalidateCache();
        }
    }
}

void VariableDefinition::updateConversionCache() const {
    QMutexLocker locker(&m_cacheMutex);

    if (m_conversionFunc) {
        // 如果有转换函数，缓存无效
        m_conversionCache.valid = false;
    } else {
        m_conversionCache.scaleFactor = calculateScaleFactor();
        m_conversionCache.offset = calculateOffset();
        m_conversionCache.valid = true;
    }

    m_cacheValid = true;
}

void VariableDefinition::updateAlarmCache() const {
    QMutexLocker locker(&m_cacheMutex);

    m_alarmCache.clear();

    // 设置报警缓存
    if (m_alarmLoLo < m_alarmLo && m_alarmLo < m_alarmHi && m_alarmHi < m_alarmHiHi) {
        m_alarmCache.hasCriticalAlarm = (m_alarmLoLo > m_minValue || m_alarmHiHi < m_maxValue);
        m_alarmCache.hasMajorAlarm = (m_alarmLo > m_minValue || m_alarmHi < m_maxValue);
        m_alarmCache.hasMinorAlarm = true;

        if (m_alarmCache.hasCriticalAlarm) {
            m_alarmCache.criticalLoLo = m_alarmLoLo;
            m_alarmCache.criticalHiHi = m_alarmHiHi;
        }

        if (m_alarmCache.hasMajorAlarm) {
            m_alarmCache.majorLo = m_alarmLo;
            m_alarmCache.majorHi = m_alarmHi;
        }

        m_alarmCache.valid = true;
    }

    m_cacheValid = true;
}

double VariableDefinition::calculateScaleFactor() const {
    if (qFuzzyCompare(m_rawMaxValue - m_rawMinValue, 0.0)) {
        return 1.0;
    }
    return (m_maxValue - m_minValue) / (m_rawMaxValue - m_rawMinValue);
}

double VariableDefinition::calculateOffset() const {
    return m_minValue - calculateScaleFactor() * m_rawMinValue;
}

void VariableDefinition::setValueInternal(StorageType type,
                                          const NativeValue& nativeValue,
                                          const QString& stringValue,
                                          const QDateTime& timestamp,
                                          DataQuality quality) {
    // 保存旧值用于比较
    QVariant oldValue = getCachedVariant();

    // 更新存储
    m_storageType = type;
    m_nativeValue = nativeValue;
    if (type == ST_String) {
        m_stringValue = stringValue;
    }

    // 更新元数据
    m_timestamp = timestamp;
    m_quality = quality;
    m_valueValid = true;

    // 使QVariant缓存失效
    invalidateVariantCache();

    // 获取新值
    QVariant newValue = getCachedVariant();

    // 检查是否真的发生了变化
    bool valueChanged = (oldValue != newValue);

    // 发出信号
    if (valueChanged) {
        emit this->valueChanged(newValue);
        emit this->valueChangedWithInfo(newValue, timestamp, quality);

        // 检查报警状态变化
        if (m_storageType == ST_Double) {
            AlarmLevel oldAlarm = checkAlarmFast(oldValue.toDouble());
            AlarmLevel newAlarm = checkAlarmFast(newValue.toDouble());
            if (oldAlarm != newAlarm) {
                emit alarmLimitsChanged();
            }
        }
    }

    emit qualityChanged(quality);
    emit timestampChanged(timestamp);
}

QVariant VariableDefinition::getCachedVariant() const {
    if (!m_variantCacheValid) {
        switch (m_storageType) {
        case ST_Double:
            m_cachedVariant = QVariant(m_nativeValue.asDouble);
            break;
        case ST_Bool:
            m_cachedVariant = QVariant(m_nativeValue.asBool);
            break;
        case ST_Int:
            m_cachedVariant = QVariant(m_nativeValue.asInt);
            break;
        case ST_Long:
            m_cachedVariant = QVariant(m_nativeValue.asLong);
            break;
        case ST_String:
            m_cachedVariant = QVariant(m_stringValue);
            break;
        default:
            m_cachedVariant = QVariant();
            break;
        }
        m_variantCacheValid = true;
    }
    return m_cachedVariant;
}

void VariableDefinition::invalidateVariantCache() {
    m_variantCacheValid = false;
}

bool VariableDefinition::checkDeadband(double oldValue, double newValue) const {
    return qAbs(newValue - oldValue) <= m_deadband;
}

bool VariableDefinition::checkDeadband(bool oldValue, bool newValue) const {
    return oldValue == newValue;  // bool类型只有相等才不更新
}

bool VariableDefinition::checkDeadband(int oldValue, int newValue) const {
    return qAbs(newValue - oldValue) <= static_cast<int>(m_deadband);
}

// ==================== LinearConversion 实现 ====================
LinearConversion::LinearConversion(double rawMin, double rawMax, double engMin, double engMax)
    : m_scaleFactor((engMax - engMin) / (rawMax - rawMin))
    , m_offset(engMin - m_scaleFactor * rawMin) {
}

LinearConversion::LinearConversion(double scaleFactor, double offset)
    : m_scaleFactor(scaleFactor)
    , m_offset(offset) {
}

double LinearConversion::rawToEngineering(double rawValue) const {
    return rawValue * m_scaleFactor + m_offset;
}

double LinearConversion::engineeringToRaw(double engValue) const {
    return (engValue - m_offset) / m_scaleFactor;
}

ConversionFunction* LinearConversion::clone() const {
    return new LinearConversion(m_scaleFactor, m_offset);
}
}
// ==================== UnitConversionManager 实现 ====================
namespace Industrial {
UnitConversionManager* UnitConversionManager::m_instance = nullptr;

UnitConversionManager::UnitConversionManager(QObject* parent)
    : QObject(parent) {
    // 注册默认的单位转换
    registerConversion(UNIT_TEMPERATURE, "°F", celsiusToFahrenheit);
    registerConversion(UNIT_TEMPERATURE, "K", celsiusToKelvin);
    registerConversion(UNIT_PRESSURE, "Bar", mpaToBar);
    registerConversion(UNIT_FLOW, "L/min", m3hToLmin);
}

UnitConversionManager::~UnitConversionManager() {
    m_instance = nullptr;
}

UnitConversionManager* UnitConversionManager::instance() {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (!m_instance) {
        m_instance = new UnitConversionManager();
    }
    return m_instance;
}

void UnitConversionManager::registerConversion(EngineeringUnit fromUnit,
                                               const QString& toUnit,
                                               std::function<double(double)> converter) {
    m_conversions.insert(qMakePair(fromUnit, toUnit), converter);
}

double UnitConversionManager::convert(double value, EngineeringUnit fromUnit,
                                      const QString& toUnit) const {
    auto key = qMakePair(fromUnit, toUnit);
    if (m_conversions.contains(key)) {
        return m_conversions.value(key)(value);
    }
    return value;
}

QStringList UnitConversionManager::getSupportedUnits(EngineeringUnit unit) const {
    QStringList units;
    for (auto it = m_conversions.constBegin(); it != m_conversions.constEnd(); ++it) {
        if (it.key().first == unit) {
            units << it.key().second;
        }
    }
    return units;
}

QString UnitConversionManager::getUnitString(EngineeringUnit unit) const {
    return engineeringUnitToString(unit);
}

void UnitConversionManager::addCustomUnit(const QString& unitName,
                                          const QString& displayName,
                                          EngineeringUnit baseUnit,
                                          double conversionFactor) {
    m_unitDisplayNames.insert(unitName, displayName);
    m_customUnits.insert(unitName, qMakePair(baseUnit, conversionFactor));
}
}

// ==================== VariableGroup 实现 ====================
namespace Industrial {
VariableGroup::VariableGroup(const QString &groupName, QObject *parent)
    : QObject(parent)
    , m_groupName(groupName) {
}

void VariableGroup::setDescription(const QString &desc) {
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged(desc);
    }
}

void VariableGroup::addVariable(VariableDefinition *var) {
    if (!m_variables.contains(var->tagName())) {
        m_variables.insert(var->tagName(), var);
        emit variableAdded(var);
    }
}

void VariableGroup::removeVariable(const QString &tagName) {
    if (m_variables.contains(tagName)) {
        m_variables.remove(tagName);
        emit variableRemoved(tagName);
    }
}

VariableDefinition* VariableGroup::getVariable(const QString &tagName) const {
    return m_variables.value(tagName, nullptr);
}

QList<VariableDefinition*> VariableGroup::variables() const {
    return m_variables.values();
}

void VariableGroup::addSubGroup(VariableGroup *group) {
    if (!m_subGroups.contains(group)) {
        m_subGroups.append(group);
    }
}

QList<VariableGroup*> VariableGroup::subGroups() const {
    return m_subGroups;
}

int VariableGroup::variableCount() const {
    return m_variables.size();
}

int VariableGroup::alarmCount() const {
    int count = 0;
    for (auto var : m_variables) {
        if (var->isInAlarm()) {
            count++;
        }
    }
    return count;
}
}

// ==================== PlantArea 实现 ====================
namespace Industrial {

PlantArea::PlantArea(const QString &areaName,
                     const QString &areaCode,
                     QObject *parent)
    : QObject(parent)
    , m_areaName(areaName)
    , m_areaCode(areaCode) {
}

void PlantArea::addDevice(const QString &deviceName,
                          const QString &deviceType,
                          const QString &description) {
    m_devices.insert(deviceName, deviceType);
    // 可以为设备添加描述信息
    Q_UNUSED(description);
}

void PlantArea::removeDevice(const QString &deviceName) {
    m_devices.remove(deviceName);
}

void PlantArea::addVariableGroup(VariableGroup *group) {
    if (!m_groups.contains(group->groupName())) {
        m_groups.insert(group->groupName(), group);
    }
}

VariableGroup* PlantArea::getGroup(const QString &groupName) const {
    return m_groups.value(groupName, nullptr);
}

QList<VariableDefinition*> PlantArea::getAllVariables() const {
    QList<VariableDefinition*> variables;
    for (auto group : m_groups) {
        variables.append(group->variables());
    }
    return variables;
}

QList<QString> PlantArea::getDeviceNames() const {
    return m_devices.keys();
}




} // namespace Industrial
