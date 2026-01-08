// VariableConfigTool.h
#ifndef VARIABLECONFIGTOOL_H
#define VARIABLECONFIGTOOL_H

#include "variablesystem.h"
#include "variabledatabase.h"
#include "realtimevariablemanager.h"
#include <QWidget>
#include <QDialog>
#include <QTreeWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSplitter>

namespace Industrial {

// 辅助函数声明
//QString variableTypeToString(VariableType type);
//QString dataQualityToString(DataQuality quality);
//QString alarmLevelToString(AlarmLevel level);
//QString engineeringUnitToString(EngineeringUnit unit);

// ==================== 变量配置对话框 ====================
class VariableConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit VariableConfigDialog(QWidget *parent = nullptr);

    void setVariable(VariableDefinition *var);
    VariableDefinition* getVariable() const;

    void setEditMode(bool editMode);

signals:
    void variableSaved(VariableDefinition *var);
    void variableDeleted(const QString &tagName);

private slots:
    void onSaveClicked();
    void onDeleteClicked();
    void onTestClicked();
    void onAddressChanged();
    void onTypeChanged(int index);

private:
    void setupUI();
    void loadVariableData();
    void saveVariableData();
    void validateInput();

    VariableDefinition *m_variable;
    bool m_editMode;

    // UI控件
    QLineEdit *m_editTagName;
    QLineEdit *m_editDescription;
    QComboBox *m_comboType;
    QComboBox *m_comboUnit;
    QDoubleSpinBox *m_spinMin;
    QDoubleSpinBox *m_spinMax;
    QDoubleSpinBox *m_spinDeadband;
    QSpinBox *m_spinUpdateRate;
    QSpinBox *m_spinPriority;

    // 报警设置
    QDoubleSpinBox *m_spinAlarmLo;
    QDoubleSpinBox *m_spinAlarmHi;
    QDoubleSpinBox *m_spinAlarmLoLo;
    QDoubleSpinBox *m_spinAlarmHiHi;
    QComboBox *m_comboAlarmLevel;

    // 地址设置
    QLineEdit *m_editAddress;
    QLineEdit *m_editDataType;
    QComboBox *m_comboAccessGroup;

    QPushButton *m_btnSave;
    QPushButton *m_btnDelete;
    QPushButton *m_btnTest;
};

// ==================== 变量浏览器 ====================
class VariableBrowser : public QWidget {
    Q_OBJECT
public:
    explicit VariableBrowser(QWidget *parent = nullptr);

    void setDatabase(VariableDatabase *db);
    void setRealTimeManager(RealTimeVariableManager *manager);

    void refresh();
    void filterByArea(const QString &areaCode);
    void filterByType(VariableType type);
    void search(const QString &keyword);

signals:
    void variableSelected(const QString &tagName);
    void variableDoubleClicked(const QString &tagName);
    void configureVariable(const QString &tagName);
    void monitorVariable(const QString &tagName);

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);
    void onTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onTableItemDoubleClicked(int row, int column);
    void onContextMenuRequested(const QPoint &pos);
    void onConfigureAction();
    void onMonitorAction();
    void onExportAction();
    void onImportAction();
    void onFilterChanged();
    void onSearchTextChanged(const QString &text);
    void updateVariableStatus();

private:
    void setupUI();
    void setupContextMenu();
    void populateTree();
    void populateTable();
    void applyFilters();

    VariableDatabase *m_database;
    RealTimeVariableManager *m_rtManager;

    // UI控件
    QTreeWidget *m_treeWidget;
    QTableWidget *m_tableWidget;
    QLineEdit *m_editSearch;
    QComboBox *m_comboFilterType;
    QComboBox *m_comboFilterArea;

    // 右键菜单
    QMenu *m_contextMenu;
    QAction *m_actionConfigure;
    QAction *m_actionMonitor;
    QAction *m_actionExport;
    QAction *m_actionImport;

    // 定时更新
    QTimer *m_updateTimer;

    // 过滤条件
    QString m_currentFilter;
    VariableType m_filterType;
    QString m_filterArea;
};

// ==================== 批量配置工具 ====================
class BatchConfigTool : public QWidget {
    Q_OBJECT
public:
    explicit BatchConfigTool(QWidget *parent = nullptr);

    void setDatabase(VariableDatabase *db);

public slots:
    void importFromExcel(const QString &filename);
    void exportToExcel(const QString &filename);
    void generateFromTemplate(const QString &templateName);
    void validateConfigurations();
    void applyChanges();
    void rollbackChanges();

private slots:
    void onImportClicked();
    void onExportClicked();
    void onValidateClicked();
    void onApplyClicked();
    void onRollbackClicked();

private:
    void setupUI();
    void loadTemplate(const QString &name);
    void saveTemplate(const QString &name);
    void showValidationResults(const QMap<QString, QString> &errors);

    VariableDatabase *m_database;

    // 模板管理
    struct ConfigTemplate {
        QString name;
        QString description;
        QMap<QString, QString> defaults;
        QList<QString> requiredFields;
    };

    QList<ConfigTemplate> m_templates;

    // UI控件
    QTableWidget *m_tableConfig;
    QComboBox *m_comboTemplate;
    QPushButton *m_btnImport;
    QPushButton *m_btnExport;
    QPushButton *m_btnValidate;
    QPushButton *m_btnApply;
    QPushButton *m_btnRollback;

    QTextEdit *m_textLog;

    // 变更跟踪
    QMap<QString, VariableDefinition*> m_originalValues;
    QMap<QString, VariableDefinition*> m_modifiedValues;
};

} // namespace Industrial

#endif // VARIABLECONFIGTOOL_H
