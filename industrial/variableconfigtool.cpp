// VariableConfigTool.cpp
#include "VariableConfigTool.h"
#include <QApplication>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QInputDialog>
#include <QTimer>
#include <QDateTime>
#include <QColor>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QMenu>
#include <QSplitter>
namespace Industrial {

// ==================== 辅助函数实现 ====================
/*
QString variableTypeToString(VariableType type)
{
    switch (type) {
    case TYPE_AI: return "AI";
    case TYPE_AO: return "AO";
    case TYPE_DI: return "DI";
    case TYPE_DO: return "DO";
    case TYPE_PI: return "PI";
    case TYPE_PO: return "PO";
    case TYPE_CALC: return "CALC";
    case TYPE_STAT: return "STAT";
    case TYPE_DERIVED: return "DERIVED";
    case TYPE_STATUS: return "STATUS";
    case TYPE_ALARM: return "ALARM";
    case TYPE_EVENT: return "EVENT";
    case TYPE_SETPOINT: return "SETPOINT";
    case TYPE_PID: return "PID";
    case TYPE_CONTROL: return "CONTROL";
    case TYPE_SYSTEM: return "SYSTEM";
    case TYPE_CONFIG: return "CONFIG";
    case TYPE_DIAG: return "DIAG";
    default: return "UNKNOWN";
    }
}

QString dataQualityToString(DataQuality quality)
{
    switch (quality) {
    case QUALITY_GOOD: return "Good";
    case QUALITY_BAD: return "Bad";
    case QUALITY_UNCERTAIN: return "Uncertain";
    case QUALITY_OLD: return "Old";
    case QUALITY_COMM_FAIL: return "Comm Failed";
    case QUALITY_OUT_RANGE: return "Out of Range";
    case QUALITY_SENSOR_FAIL: return "Sensor Failed";
    case QUALITY_CALIBRATION: return "Calibrating";
    case QUALITY_MAINTENANCE: return "Maintenance";
    default: return "Unknown";
    }
}

QString alarmLevelToString(AlarmLevel level)
{
    switch (level) {
    case ALARM_NONE: return "Normal";
    case ALARM_INFO: return "Info";
    case ALARM_WARNING: return "Warning";
    case ALARM_MINOR: return "Minor";
    case ALARM_MAJOR: return "Major";
    case ALARM_CRITICAL: return "Critical";
    default: return "Unknown";
    }
}

QString engineeringUnitToString(EngineeringUnit unit)
{
    switch (unit) {
    case UNIT_NONE: return "";
    case UNIT_TEMPERATURE: return "°C";
    case UNIT_PRESSURE: return "Pa";
    case UNIT_FLOW: return "m³/h";
    case UNIT_LEVEL: return "m";
    case UNIT_SPEED: return "m/s";
    case UNIT_VOLTAGE: return "V";
    case UNIT_CURRENT: return "A";
    case UNIT_POWER: return "W";
    case UNIT_ENERGY: return "kWh";
    case UNIT_PERCENT: return "%";
    case UNIT_COUNT: return "Count";
    case UNIT_TIME: return "s";
    case UNIT_FREQUENCY: return "Hz";
    case UNIT_WEIGHT: return "kg";
    case UNIT_LENGTH: return "m";
    default: return "";
    }
}
*/
// ==================== VariableConfigDialog 实现 ====================

VariableConfigDialog::VariableConfigDialog(QWidget *parent)
    : QDialog(parent)
    , m_variable(nullptr)
    , m_editMode(false)
{
    setupUI();
    setWindowTitle(tr("Variable Configuration"));
    resize(600, 700);
}

void VariableConfigDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 基本信息组
    QGroupBox *basicGroup = new QGroupBox(tr("Basic Information"), this);
    QFormLayout *basicLayout = new QFormLayout(basicGroup);

    m_editTagName = new QLineEdit(this);
    m_editTagName->setReadOnly(true);
    basicLayout->addRow(tr("Tag Name:"), m_editTagName);

    m_editDescription = new QLineEdit(this);
    basicLayout->addRow(tr("Description:"), m_editDescription);

    m_comboType = new QComboBox(this);
    m_comboType->addItem("AI - Analog Input", TYPE_AI);
    m_comboType->addItem("AO - Analog Output", TYPE_AO);
    m_comboType->addItem("DI - Digital Input", TYPE_DI);
    m_comboType->addItem("DO - Digital Output", TYPE_DO);
    m_comboType->addItem("PI - Pulse Input", TYPE_PI);
    m_comboType->addItem("PO - Pulse Output", TYPE_PO);
    m_comboType->addItem("CALC - Calculated", TYPE_CALC);
    m_comboType->addItem("STAT - Statistical", TYPE_STAT);
    connect(m_comboType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VariableConfigDialog::onTypeChanged);
    basicLayout->addRow(tr("Type:"), m_comboType);

    m_comboUnit = new QComboBox(this);
    m_comboUnit->addItem("None", UNIT_NONE);
    m_comboUnit->addItem("Temperature", UNIT_TEMPERATURE);
    m_comboUnit->addItem("Pressure", UNIT_PRESSURE);
    m_comboUnit->addItem("Flow", UNIT_FLOW);
    m_comboUnit->addItem("Level", UNIT_LEVEL);
    m_comboUnit->addItem("Speed", UNIT_SPEED);
    m_comboUnit->addItem("Voltage", UNIT_VOLTAGE);
    m_comboUnit->addItem("Current", UNIT_CURRENT);
    basicLayout->addRow(tr("Unit:"), m_comboUnit);

    basicGroup->setLayout(basicLayout);
    mainLayout->addWidget(basicGroup);

    // 工程范围组
    QGroupBox *rangeGroup = new QGroupBox(tr("Engineering Range"), this);
    QFormLayout *rangeLayout = new QFormLayout(rangeGroup);

    m_spinMin = new QDoubleSpinBox(this);
    m_spinMin->setRange(-1e6, 1e6);
    m_spinMin->setDecimals(3);
    rangeLayout->addRow(tr("Minimum:"), m_spinMin);

    m_spinMax = new QDoubleSpinBox(this);
    m_spinMax->setRange(-1e6, 1e6);
    m_spinMax->setDecimals(3);
    rangeLayout->addRow(tr("Maximum:"), m_spinMax);

    m_spinDeadband = new QDoubleSpinBox(this);
    m_spinDeadband->setRange(0, 1000);
    m_spinDeadband->setDecimals(3);
    rangeLayout->addRow(tr("Deadband:"), m_spinDeadband);

    rangeGroup->setLayout(rangeLayout);
    mainLayout->addWidget(rangeGroup);

    // 更新参数组
    QGroupBox *updateGroup = new QGroupBox(tr("Update Parameters"), this);
    QFormLayout *updateLayout = new QFormLayout(updateGroup);

    m_spinUpdateRate = new QSpinBox(this);
    m_spinUpdateRate->setRange(10, 60000);
    m_spinUpdateRate->setSingleStep(100);
    m_spinUpdateRate->setSuffix(" ms");
    updateLayout->addRow(tr("Update Rate:"), m_spinUpdateRate);

    m_spinPriority = new QSpinBox(this);
    m_spinPriority->setRange(0, 100);
    updateLayout->addRow(tr("Priority:"), m_spinPriority);

    updateGroup->setLayout(updateLayout);
    mainLayout->addWidget(updateGroup);

    // 报警设置组
    QGroupBox *alarmGroup = new QGroupBox(tr("Alarm Settings"), this);
    QGridLayout *alarmLayout = new QGridLayout(alarmGroup);

    alarmLayout->addWidget(new QLabel(tr("Low:"), this), 0, 0);
    m_spinAlarmLo = new QDoubleSpinBox(this);
    m_spinAlarmLo->setRange(-1e6, 1e6);
    m_spinAlarmLo->setDecimals(3);
    alarmLayout->addWidget(m_spinAlarmLo, 0, 1);

    alarmLayout->addWidget(new QLabel(tr("Low-Low:"), this), 0, 2);
    m_spinAlarmLoLo = new QDoubleSpinBox(this);
    m_spinAlarmLoLo->setRange(-1e6, 1e6);
    m_spinAlarmLoLo->setDecimals(3);
    alarmLayout->addWidget(m_spinAlarmLoLo, 0, 3);

    alarmLayout->addWidget(new QLabel(tr("High:"), this), 1, 0);
    m_spinAlarmHi = new QDoubleSpinBox(this);
    m_spinAlarmHi->setRange(-1e6, 1e6);
    m_spinAlarmHi->setDecimals(3);
    alarmLayout->addWidget(m_spinAlarmHi, 1, 1);

    alarmLayout->addWidget(new QLabel(tr("High-High:"), this), 1, 2);
    m_spinAlarmHiHi = new QDoubleSpinBox(this);
    m_spinAlarmHiHi->setRange(-1e6, 1e6);
    m_spinAlarmHiHi->setDecimals(3);
    alarmLayout->addWidget(m_spinAlarmHiHi, 1, 3);

    alarmLayout->addWidget(new QLabel(tr("Level:"), this), 2, 0);
    m_comboAlarmLevel = new QComboBox(this);
    m_comboAlarmLevel->addItem("None", ALARM_NONE);
    m_comboAlarmLevel->addItem("Info", ALARM_INFO);
    m_comboAlarmLevel->addItem("Warning", ALARM_WARNING);
    m_comboAlarmLevel->addItem("Minor", ALARM_MINOR);
    m_comboAlarmLevel->addItem("Major", ALARM_MAJOR);
    m_comboAlarmLevel->addItem("Critical", ALARM_CRITICAL);
    alarmLayout->addWidget(m_comboAlarmLevel, 2, 1, 1, 3);

    alarmGroup->setLayout(alarmLayout);
    mainLayout->addWidget(alarmGroup);

    // 地址映射组
    QGroupBox *addressGroup = new QGroupBox(tr("Address Mapping"), this);
    QFormLayout *addressLayout = new QFormLayout(addressGroup);

    m_editAddress = new QLineEdit(this);
    connect(m_editAddress, &QLineEdit::textChanged,
            this, &VariableConfigDialog::onAddressChanged);
    addressLayout->addRow(tr("Address:"), m_editAddress);

    m_editDataType = new QLineEdit(this);
    addressLayout->addRow(tr("Data Type:"), m_editDataType);

    m_comboAccessGroup = new QComboBox(this);
    m_comboAccessGroup->addItems({"Operator", "Engineer", "Administrator"});
    addressLayout->addRow(tr("Access Group:"), m_comboAccessGroup);

    addressGroup->setLayout(addressLayout);
    mainLayout->addWidget(addressGroup);

    // 按钮组
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_btnSave = new QPushButton(tr("Save"), this);
    connect(m_btnSave, &QPushButton::clicked, this, &VariableConfigDialog::onSaveClicked);
    buttonLayout->addWidget(m_btnSave);

    m_btnDelete = new QPushButton(tr("Delete"), this);
    m_btnDelete->setEnabled(false);
    connect(m_btnDelete, &QPushButton::clicked, this, &VariableConfigDialog::onDeleteClicked);
    buttonLayout->addWidget(m_btnDelete);

    m_btnTest = new QPushButton(tr("Test Connection"), this);
    connect(m_btnTest, &QPushButton::clicked, this, &VariableConfigDialog::onTestClicked);
    buttonLayout->addWidget(m_btnTest);

    QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
    connect(btnCancel, &QPushButton::clicked, this, &VariableConfigDialog::reject);
    buttonLayout->addWidget(btnCancel);

    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void VariableConfigDialog::setVariable(VariableDefinition *var)
{
    m_variable = var;
    loadVariableData();

    m_editMode = (var != nullptr);
    m_btnDelete->setEnabled(m_editMode);

    if (m_editMode) {
        m_editTagName->setText(var->tagName());
        m_comboType->setEnabled(false);
        setWindowTitle(tr("Edit Variable: %1").arg(var->tagName()));
    } else {
        setWindowTitle(tr("New Variable"));
    }
}

VariableDefinition* VariableConfigDialog::getVariable() const
{
    return m_variable;
}

void VariableConfigDialog::setEditMode(bool editMode)
{
    m_editMode = editMode;
    m_btnDelete->setEnabled(editMode);
}

void VariableConfigDialog::loadVariableData()
{
    if (!m_variable) return;

    m_editDescription->setText(m_variable->description());

    int typeIndex = m_comboType->findData(m_variable->type());
    if (typeIndex >= 0) {
        m_comboType->setCurrentIndex(typeIndex);
    }

    int unitIndex = m_comboUnit->findData(m_variable->unit());
    if (unitIndex >= 0) {
        m_comboUnit->setCurrentIndex(unitIndex);
    }

    m_spinMin->setValue(m_variable->minValue());
    m_spinMax->setValue(m_variable->maxValue());
    m_spinDeadband->setValue(m_variable->deadband());
    m_spinUpdateRate->setValue(m_variable->updateRate());
    m_spinPriority->setValue(m_variable->priority());

    m_spinAlarmLo->setValue(m_variable->alarmLo());
    m_spinAlarmHi->setValue(m_variable->alarmHi());
    m_spinAlarmLoLo->setValue(m_variable->alarmLoLo());
    m_spinAlarmHiHi->setValue(m_variable->alarmHiHi());

    int alarmIndex = m_comboAlarmLevel->findData(m_variable->alarmLevel());
    if (alarmIndex >= 0) {
        m_comboAlarmLevel->setCurrentIndex(alarmIndex);
    }

    m_editAddress->setText(m_variable->address());
    m_editDataType->setText(m_variable->dataType());

    int accessIndex = m_comboAccessGroup->findText(m_variable->accessGroup());
    if (accessIndex >= 0) {
        m_comboAccessGroup->setCurrentIndex(accessIndex);
    }
}

void VariableConfigDialog::saveVariableData()
{
    if (!m_variable) {
        QString tagName = m_editTagName->text();
        if (tagName.isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("Tag name cannot be empty"));
            return;
        }

        int type = m_comboType->currentData().toInt();
        m_variable = new VariableDefinition(tagName, static_cast<VariableType>(type));
    }

    m_variable->setDescription(m_editDescription->text());
    m_variable->setUnit(static_cast<EngineeringUnit>(m_comboUnit->currentData().toInt()));
    m_variable->setRange(m_spinMin->value(), m_spinMax->value());
    m_variable->setDeadband(m_spinDeadband->value());
    m_variable->setUpdateRate(m_spinUpdateRate->value());
    m_variable->setPriority(m_spinPriority->value());
    m_variable->setAlarmLimits(m_spinAlarmLo->value(), m_spinAlarmHi->value(),
                               m_spinAlarmLoLo->value(), m_spinAlarmHiHi->value());
    m_variable->setAlarmLevel(static_cast<AlarmLevel>(m_comboAlarmLevel->currentData().toInt()));
    m_variable->setAddress(m_editAddress->text());
    m_variable->setDataType(m_editDataType->text());
    m_variable->setAccessGroup(m_comboAccessGroup->currentText());
}

void VariableConfigDialog::validateInput()
{
    if (m_spinMin->value() >= m_spinMax->value()) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Minimum value must be less than maximum value"));
        return;
    }

    if (m_spinAlarmLoLo->value() > m_spinAlarmLo->value() ||
        m_spinAlarmLo->value() > m_spinAlarmHi->value() ||
        m_spinAlarmHi->value() > m_spinAlarmHiHi->value()) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Alarm limits must be in order: LoLo <= Lo <= Hi <= HiHi"));
        return;
    }
}

void VariableConfigDialog::onSaveClicked()
{
    validateInput();
    saveVariableData();
    emit variableSaved(m_variable);
    accept();
}

void VariableConfigDialog::onDeleteClicked()
{
    if (!m_variable) return;

    QString tagName = m_variable->tagName();
    int result = QMessageBox::question(this, tr("Confirm Delete"),
                                       tr("Are you sure you want to delete variable '%1'?")
                                           .arg(tagName),
                                       QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        emit variableDeleted(tagName);
        accept();
    }
}

void VariableConfigDialog::onTestClicked()
{
    QString address = m_editAddress->text();
    if (address.isEmpty()) {
        QMessageBox::information(this, tr("Test"), tr("Please enter an address first"));
        return;
    }

    QMessageBox::information(this, tr("Test"),
                             tr("Testing connection to: %1").arg(address));
}

void VariableConfigDialog::onAddressChanged()
{
    QString address = m_editAddress->text();
    if (address.contains("Float", Qt::CaseInsensitive)) {
        m_editDataType->setText("Float32");
    } else if (address.contains("Int", Qt::CaseInsensitive)) {
        m_editDataType->setText("Int32");
    } else if (address.contains("Bool", Qt::CaseInsensitive)) {
        m_editDataType->setText("Boolean");
    }
}

void VariableConfigDialog::onTypeChanged(int index)
{
    Q_UNUSED(index);

    VariableType type = static_cast<VariableType>(m_comboType->currentData().toInt());
    bool isAnalog = (type == TYPE_AI || type == TYPE_AO);
    bool isDigital = (type == TYPE_DI || type == TYPE_DO);

    m_comboUnit->setEnabled(isAnalog);
    m_spinMin->setEnabled(!isDigital);
    m_spinMax->setEnabled(!isDigital);
    m_spinDeadband->setEnabled(isAnalog);

    if (isDigital) {
        m_spinMin->setValue(0);
        m_spinMax->setValue(1);
    }
}

// ==================== VariableBrowser 实现 ====================

VariableBrowser::VariableBrowser(QWidget *parent)
    : QWidget(parent)
    , m_database(nullptr)
    , m_rtManager(nullptr)
    , m_treeWidget(new QTreeWidget(this))
    , m_tableWidget(new QTableWidget(this))
    , m_editSearch(new QLineEdit(this))
    , m_comboFilterType(new QComboBox(this))
    , m_comboFilterArea(new QComboBox(this))
    , m_contextMenu(new QMenu(this))
    , m_updateTimer(new QTimer(this))
    , m_filterType(static_cast<VariableType>(-1))
{
    setupUI();
    setupContextMenu();

    connect(m_updateTimer, &QTimer::timeout, this, &VariableBrowser::updateVariableStatus);
    m_updateTimer->start(1000);
}

void VariableBrowser::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    m_editSearch->setPlaceholderText(tr("Search variables..."));
    connect(m_editSearch, &QLineEdit::textChanged,
            this, &VariableBrowser::onSearchTextChanged);
    toolbarLayout->addWidget(new QLabel(tr("Search:"), this));
    toolbarLayout->addWidget(m_editSearch);

    m_comboFilterType->addItem(tr("All Types"), -1);
    m_comboFilterType->addItem("AI", TYPE_AI);
    m_comboFilterType->addItem("AO", TYPE_AO);
    m_comboFilterType->addItem("DI", TYPE_DI);
    m_comboFilterType->addItem("DO", TYPE_DO);
    connect(m_comboFilterType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VariableBrowser::onFilterChanged);
    toolbarLayout->addWidget(new QLabel(tr("Type:"), this));
    toolbarLayout->addWidget(m_comboFilterType);

    m_comboFilterArea->addItem(tr("All Areas"));
    connect(m_comboFilterArea, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VariableBrowser::onFilterChanged);
    toolbarLayout->addWidget(new QLabel(tr("Area:"), this));
    toolbarLayout->addWidget(m_comboFilterArea);

    QPushButton *btnRefresh = new QPushButton(tr("Refresh"), this);
    connect(btnRefresh, &QPushButton::clicked, this, &VariableBrowser::refresh);
    toolbarLayout->addWidget(btnRefresh);

    toolbarLayout->addStretch();
    mainLayout->addLayout(toolbarLayout);

    // 分割窗口
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    m_treeWidget->setHeaderLabel(tr("Plant Structure"));
    m_treeWidget->setMinimumWidth(200);
    connect(m_treeWidget, &QTreeWidget::itemClicked,
            this, &VariableBrowser::onTreeItemClicked);
    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked,
            this, &VariableBrowser::onTreeItemDoubleClicked);
    splitter->addWidget(m_treeWidget);

    m_tableWidget->setColumnCount(8);
    QStringList headers = {
        tr("Tag Name"), tr("Description"), tr("Type"), tr("Value"),
        tr("Quality"), tr("Alarm"), tr("Timestamp"), tr("Unit")
    };
    m_tableWidget->setHorizontalHeaderLabels(headers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableWidget, &QTableWidget::cellDoubleClicked,
            this, &VariableBrowser::onTableItemDoubleClicked);
    connect(m_tableWidget, &QTableWidget::customContextMenuRequested,
            this, &VariableBrowser::onContextMenuRequested);
    splitter->addWidget(m_tableWidget);

    mainLayout->addWidget(splitter);

    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void VariableBrowser::setupContextMenu()
{
    m_actionConfigure = new QAction(tr("Configure..."), this);
    m_actionMonitor = new QAction(tr("Monitor..."), this);
    m_actionExport = new QAction(tr("Export..."), this);
    m_actionImport = new QAction(tr("Import..."), this);

    connect(m_actionConfigure, &QAction::triggered, this, &VariableBrowser::onConfigureAction);
    connect(m_actionMonitor, &QAction::triggered, this, &VariableBrowser::onMonitorAction);
    connect(m_actionExport, &QAction::triggered, this, &VariableBrowser::onExportAction);
    connect(m_actionImport, &QAction::triggered, this, &VariableBrowser::onImportAction);

    m_contextMenu->addAction(m_actionConfigure);
    m_contextMenu->addAction(m_actionMonitor);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_actionExport);
    m_contextMenu->addAction(m_actionImport);
}

void VariableBrowser::setDatabase(VariableDatabase *db)
{
    m_database = db;
    refresh();
}

void VariableBrowser::setRealTimeManager(RealTimeVariableManager *manager)
{
    m_rtManager = manager;
}

void VariableBrowser::refresh()
{
    if (!m_database) return;

    populateTree();
    populateTable();
}

void VariableBrowser::filterByArea(const QString &areaCode)
{
    int index = m_comboFilterArea->findText(areaCode);
    if (index >= 0) {
        m_comboFilterArea->setCurrentIndex(index);
        m_filterArea = areaCode;
        populateTable();
    }
}

void VariableBrowser::filterByType(VariableType type)
{
    int index = m_comboFilterType->findData(type);
    if (index >= 0) {
        m_comboFilterType->setCurrentIndex(index);
        m_filterType = type;
        populateTable();
    }
}

void VariableBrowser::search(const QString &keyword)
{
    m_editSearch->setText(keyword);
    m_currentFilter = keyword;
    populateTable();
}

void VariableBrowser::populateTree()
{
    m_treeWidget->clear();

    if (!m_database) return;

    QTreeWidgetItem *root = new QTreeWidgetItem(m_treeWidget);
    root->setText(0, tr("Plant"));
    root->setData(0, Qt::UserRole, "ROOT");

    // 从数据库加载区域（简化示例）
    QStringList areas = {"Area1", "Area2", "Area3"};
    m_comboFilterArea->clear();
    m_comboFilterArea->addItem(tr("All Areas"));

    for (const QString &area : areas) {
        m_comboFilterArea->addItem(area);

        QTreeWidgetItem *areaItem = new QTreeWidgetItem(root);
        areaItem->setText(0, area);
        areaItem->setData(0, Qt::UserRole, "AREA:" + area);
        areaItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));

        // 示例设备
        QStringList devices = {"Pump101", "Valve201", "Tank301"};
        for (const QString &device : devices) {
            QTreeWidgetItem *deviceItem = new QTreeWidgetItem(areaItem);
            deviceItem->setText(0, device);
            deviceItem->setData(0, Qt::UserRole, "DEVICE:" + device);
            deviceItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
        }
    }

    m_treeWidget->expandAll();
}

void VariableBrowser::populateTable()
{
    m_tableWidget->setRowCount(0);

    if (!m_database) return;

    QList<VariableDefinition*> allVars = m_database->loadAllVariables();
    QList<VariableDefinition*> filteredVars;

    // 应用过滤器
    for (VariableDefinition *var : allVars) {
        bool include = true;

        // 搜索过滤
        if (!m_currentFilter.isEmpty()) {
            if (!var->tagName().contains(m_currentFilter, Qt::CaseInsensitive) &&
                !var->description().contains(m_currentFilter, Qt::CaseInsensitive)) {
                include = false;
            }
        }

        // 类型过滤
        if (m_filterType != static_cast<VariableType>(-1) && var->type() != m_filterType) {
            include = false;
        }

        // 区域过滤（根据标签名前缀）
        if (!m_filterArea.isEmpty() && m_filterArea != tr("All Areas")) {
            if (!var->tagName().startsWith(m_filterArea + ".")) {
                include = false;
            }
        }

        if (include) {
            filteredVars.append(var);
        }
    }

    // 填充表格
    m_tableWidget->setRowCount(filteredVars.size());

    for (int i = 0; i < filteredVars.size(); i++) {
        VariableDefinition *var = filteredVars[i];

        // 获取实时值
        QVariant currentValue;
        QString qualityStr = tr("Unknown");
        QString alarmStr = tr("Normal");
        QString timestampStr;

        if (m_rtManager) {
            RealTimeVariable *rtVar = m_rtManager->getVariable(var->tagName());
            if (rtVar) {
                currentValue = rtVar->value();
                qualityStr = dataQualityToString(rtVar->quality());
                alarmStr = alarmLevelToString(rtVar->alarmLevel());
                timestampStr = rtVar->timestamp().toString("hh:mm:ss");
            }
        }

        // Tag Name
        QTableWidgetItem *tagItem = new QTableWidgetItem(var->tagName());
        tagItem->setData(Qt::UserRole, var->tagName());
        m_tableWidget->setItem(i, 0, tagItem);

        // Description
        m_tableWidget->setItem(i, 1, new QTableWidgetItem(var->description()));

        // Type
        QString typeStr = variableTypeToString(var->type());
        m_tableWidget->setItem(i, 2, new QTableWidgetItem(typeStr));

        // Value
        QString valueStr = currentValue.isValid() ? currentValue.toString() : tr("N/A");
        QTableWidgetItem *valueItem = new QTableWidgetItem(valueStr);

        // 报警颜色
        if (m_rtManager) {
            RealTimeVariable *rtVar = m_rtManager->getVariable(var->tagName());
            if (rtVar && rtVar->isInAlarm()) {
                QColor color;
                switch (rtVar->alarmLevel()) {
                case ALARM_CRITICAL:
                    color = Qt::red;
                    break;
                case ALARM_MAJOR:
                    color = QColor(255, 165, 0);
                    break;
                case ALARM_MINOR:
                    color = Qt::yellow;
                    break;
                default:
                    break;
                }
                valueItem->setBackground(color);
                valueItem->setForeground(Qt::white);
            }
        }

        m_tableWidget->setItem(i, 3, valueItem);

        // Quality
        QTableWidgetItem *qualityItem = new QTableWidgetItem(qualityStr);
        if (qualityStr == tr("Bad") || qualityStr == tr("Comm Failed")) {
            qualityItem->setForeground(Qt::red);
        } else if (qualityStr == tr("Uncertain")) {
            qualityItem->setForeground(Qt::yellow);
        }
        m_tableWidget->setItem(i, 4, qualityItem);

        // Alarm
        m_tableWidget->setItem(i, 5, new QTableWidgetItem(alarmStr));

        // Timestamp
        m_tableWidget->setItem(i, 6, new QTableWidgetItem(timestampStr));

        // Unit
        QString unitStr = engineeringUnitToString(var->unit());
        m_tableWidget->setItem(i, 7, new QTableWidgetItem(unitStr));
    }

    m_tableWidget->resizeColumnsToContents();
}

void VariableBrowser::applyFilters()
{
    m_currentFilter = m_editSearch->text();
    m_filterType = static_cast<VariableType>(m_comboFilterType->currentData().toInt());
    m_filterArea = m_comboFilterArea->currentText();

    populateTable();
}

void VariableBrowser::updateVariableStatus()
{
    if (!m_rtManager || m_tableWidget->rowCount() == 0) return;

    for (int row = 0; row < m_tableWidget->rowCount(); row++) {
        QTableWidgetItem *tagItem = m_tableWidget->item(row, 0);
        if (!tagItem) continue;

        QString tagName = tagItem->data(Qt::UserRole).toString();
        RealTimeVariable *rtVar = m_rtManager->getVariable(tagName);
        if (!rtVar) continue;

        // 更新值
        QTableWidgetItem *valueItem = m_tableWidget->item(row, 3);
        if (valueItem) {
            valueItem->setText(rtVar->value().toString());

            // 更新报警颜色
            if (rtVar->isInAlarm()) {
                QColor color;
                switch (rtVar->alarmLevel()) {
                case ALARM_CRITICAL:
                    color = Qt::red;
                    break;
                case ALARM_MAJOR:
                    color = QColor(255, 165, 0);
                    break;
                case ALARM_MINOR:
                    color = Qt::yellow;
                    break;
                default:
                    break;
                }
                valueItem->setBackground(color);
                valueItem->setForeground(Qt::white);
            } else {
                valueItem->setBackground(Qt::white);
                valueItem->setForeground(Qt::black);
            }
        }

        // 更新质量
        QTableWidgetItem *qualityItem = m_tableWidget->item(row, 4);
        if (qualityItem) {
            qualityItem->setText(dataQualityToString(rtVar->quality()));
        }

        // 更新报警状态
        QTableWidgetItem *alarmItem = m_tableWidget->item(row, 5);
        if (alarmItem) {
            alarmItem->setText(alarmLevelToString(rtVar->alarmLevel()));
        }

        // 更新时间戳
        QTableWidgetItem *timeItem = m_tableWidget->item(row, 6);
        if (timeItem) {
            timeItem->setText(rtVar->timestamp().toString("hh:mm:ss"));
        }
    }
}

void VariableBrowser::onTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    QString data = item->data(0, Qt::UserRole).toString();
    if (data.startsWith("AREA:")) {
        QString areaCode = data.mid(5);
        filterByArea(areaCode);
    }
}

void VariableBrowser::onTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    QString data = item->data(0, Qt::UserRole).toString();
    if (data.startsWith("DEVICE:")) {
        QString deviceName = data.mid(7);
        m_editSearch->setText(deviceName);
        onSearchTextChanged(deviceName);
    }
}

void VariableBrowser::onTableItemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *item = m_tableWidget->item(row, 0);
    if (item) {
        QString tagName = item->data(Qt::UserRole).toString();
        emit variableDoubleClicked(tagName);
    }
}

void VariableBrowser::onContextMenuRequested(const QPoint &pos)
{
    if (!m_tableWidget->selectionModel()->hasSelection()) {
        return;
    }

    m_contextMenu->popup(m_tableWidget->viewport()->mapToGlobal(pos));
}

void VariableBrowser::onConfigureAction()
{
    QList<QTableWidgetItem*> selected = m_tableWidget->selectedItems();
    if (selected.isEmpty()) return;

    QString tagName = selected.first()->data(Qt::UserRole).toString();
    emit configureVariable(tagName);
}

void VariableBrowser::onMonitorAction()
{
    QList<QTableWidgetItem*> selected = m_tableWidget->selectedItems();
    if (selected.isEmpty()) return;

    QString tagName = selected.first()->data(Qt::UserRole).toString();
    emit monitorVariable(tagName);
}

void VariableBrowser::onExportAction()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Variables"), "",
                                                    tr("CSV Files (*.csv);;JSON Files (*.json);;All Files (*)"));

    if (fileName.isEmpty() || !m_database) return;

    if (fileName.endsWith(".json", Qt::CaseInsensitive)) {
        m_database->exportToJson(fileName);
    } else {
        m_database->exportToCsv(fileName);
    }
}

void VariableBrowser::onImportAction()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Variables"), "",
                                                    tr("CSV Files (*.csv);;JSON Files (*.json);;All Files (*)"));

    if (fileName.isEmpty() || !m_database) return;

    bool success = false;
    if (fileName.endsWith(".json", Qt::CaseInsensitive)) {
        success = m_database->importFromJson(fileName);
    } else {
        success = m_database->importFromCsv(fileName);
    }

    if (success) {
        refresh();
        QMessageBox::information(this, tr("Import"), tr("Variables imported successfully"));
    }
}

void VariableBrowser::onFilterChanged()
{
    applyFilters();
}

void VariableBrowser::onSearchTextChanged(const QString &text)
{
    m_currentFilter = text;
    applyFilters();
}

// ==================== BatchConfigTool 实现 ====================

BatchConfigTool::BatchConfigTool(QWidget *parent)
    : QWidget(parent)
    , m_database(nullptr)
    , m_tableConfig(new QTableWidget(this))
    , m_comboTemplate(new QComboBox(this))
    , m_btnImport(new QPushButton(tr("Import..."), this))
    , m_btnExport(new QPushButton(tr("Export..."), this))
    , m_btnValidate(new QPushButton(tr("Validate"), this))
    , m_btnApply(new QPushButton(tr("Apply"), this))
    , m_btnRollback(new QPushButton(tr("Rollback"), this))
    , m_textLog(new QTextEdit(this))
{
    setupUI();
}

void BatchConfigTool::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 模板选择
    QHBoxLayout *templateLayout = new QHBoxLayout();
    templateLayout->addWidget(new QLabel(tr("Template:"), this));

    m_comboTemplate->addItems({tr("Default Template"), tr("Analog Template"), tr("Digital Template")});
    templateLayout->addWidget(m_comboTemplate);

    QPushButton *btnNewTemplate = new QPushButton(tr("New Template"), this);
    connect(btnNewTemplate, &QPushButton::clicked, [this]() {
        QString name = QInputDialog::getText(this, tr("New Template"), tr("Template name:"));
        if (!name.isEmpty()) {
            m_comboTemplate->addItem(name);
            m_comboTemplate->setCurrentText(name);
        }
    });
    templateLayout->addWidget(btnNewTemplate);

    templateLayout->addStretch();
    mainLayout->addLayout(templateLayout);

    // 配置表格
    m_tableConfig->setColumnCount(10);
    QStringList headers = {
        tr("Tag Name"), tr("Description"), tr("Type"), tr("Unit"),
        tr("Min"), tr("Max"), tr("Address"), tr("Update Rate"),
        tr("Alarm Lo"), tr("Alarm Hi")
    };
    m_tableConfig->setHorizontalHeaderLabels(headers);
    m_tableConfig->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(m_tableConfig);

    // 按钮组
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    connect(m_btnImport, &QPushButton::clicked, this, &BatchConfigTool::onImportClicked);
    buttonLayout->addWidget(m_btnImport);

    connect(m_btnExport, &QPushButton::clicked, this, &BatchConfigTool::onExportClicked);
    buttonLayout->addWidget(m_btnExport);

    buttonLayout->addStretch();

    connect(m_btnValidate, &QPushButton::clicked, this, &BatchConfigTool::onValidateClicked);
    buttonLayout->addWidget(m_btnValidate);

    connect(m_btnApply, &QPushButton::clicked, this, &BatchConfigTool::onApplyClicked);
    buttonLayout->addWidget(m_btnApply);

    connect(m_btnRollback, &QPushButton::clicked, this, &BatchConfigTool::onRollbackClicked);
    buttonLayout->addWidget(m_btnRollback);

    mainLayout->addLayout(buttonLayout);

    // 日志
    m_textLog->setReadOnly(true);
    m_textLog->setMaximumHeight(100);
    mainLayout->addWidget(new QLabel(tr("Log:"), this));
    mainLayout->addWidget(m_textLog);
}

void BatchConfigTool::setDatabase(VariableDatabase *db)
{
    m_database = db;
}

void BatchConfigTool::onImportClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Configuration"), "",
                                                    tr("CSV Files (*.csv);;All Files (*)"));

    if (fileName.isEmpty()) return;

    m_textLog->append(tr("Importing from: %1").arg(fileName));
    QMessageBox::information(this, tr("Import"),
                             tr("Import functionality would be implemented here"));
}

void BatchConfigTool::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Configuration"), "",
                                                    tr("CSV Files (*.csv);;All Files (*)"));

    if (fileName.isEmpty()) return;

    m_textLog->append(tr("Exporting to: %1").arg(fileName));
    QMessageBox::information(this, tr("Export"),
                             tr("Export functionality would be implemented here"));
}

void BatchConfigTool::onValidateClicked()
{
    m_textLog->append(tr("Validating configurations..."));
    QMessageBox::information(this, tr("Validate"),
                             tr("Validation would check for errors in the configuration"));
}

void BatchConfigTool::onApplyClicked()
{
    m_textLog->append(tr("Applying changes..."));
    QMessageBox::information(this, tr("Apply"),
                             tr("Changes would be applied to the database"));
}

void BatchConfigTool::onRollbackClicked()
{
    m_textLog->append(tr("Rolling back changes..."));
    QMessageBox::information(this, tr("Rollback"),
                             tr("Changes would be rolled back"));
}

void BatchConfigTool::importFromExcel(const QString &filename)
{
    Q_UNUSED(filename);
    m_textLog->append(tr("Excel import not implemented yet"));
}

void BatchConfigTool::exportToExcel(const QString &filename)
{
    Q_UNUSED(filename);
    m_textLog->append(tr("Excel export not implemented yet"));
}

void BatchConfigTool::generateFromTemplate(const QString &templateName)
{
    Q_UNUSED(templateName);
    m_textLog->append(tr("Template generation not implemented yet"));
}

void BatchConfigTool::validateConfigurations()
{
    m_textLog->append(tr("Validation not implemented yet"));
}

void BatchConfigTool::applyChanges()
{
    m_textLog->append(tr("Apply changes not implemented yet"));
}

void BatchConfigTool::rollbackChanges()
{
    m_textLog->append(tr("Rollback changes not implemented yet"));
}

void BatchConfigTool::loadTemplate(const QString &name)
{
    Q_UNUSED(name);
    // 模板加载逻辑
}

void BatchConfigTool::saveTemplate(const QString &name)
{
    Q_UNUSED(name);
    // 模板保存逻辑
}

void BatchConfigTool::showValidationResults(const QMap<QString, QString> &errors)
{
    if (errors.isEmpty()) {
        m_textLog->append(tr("Validation passed"));
    } else {
        m_textLog->append(tr("Validation failed with %1 errors").arg(errors.size()));
        for (auto it = errors.begin(); it != errors.end(); ++it) {
            m_textLog->append(tr("  %1: %2").arg(it.key()).arg(it.value()));
        }
    }
}

} // namespace Industrial
