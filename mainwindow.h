#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> //For main application window
#include <QStandardItemModel> //Model that stores data in rows/columns
#include <QSortFilterProxyModel> //Model for sorting and filtering other model
#include <QTableView> //View for displaying data in table
#include <QLineEdit> //Single-line text input box
#include <QSpinBox> //Number input box with increment/decrement buttons
#include <QComboBox> //Drop-down selection box
#include <QPushButton> //Clickable button
#include <QLabel> //Label text
#include <QRegularExpressionValidator> //Checks text matches pattern defined by regex
#include <QModelIndex> //Provides QModelIndex, representing location of cell

//Column names are given
//Column count (number of columns) also given
enum Column {
    COLUMN_AUTHOR = 0,
    COLUMN_YEAR = 1,
    COLUMN_TITLE = 2,
    COLUMN_JOURNAL = 3,
    COLUMN_VOLUME = 4,
    COLUMN_ISSUE = 5,
    COLUMN_PAGES = 6,
    COLUMN_COUNT = 7
};

class JournalSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit JournalSortProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        if (left.column() == COLUMN_PAGES) { //Applying to Pages column
            //Retrieving range strings from Qt::DisplayRole
            QString rangeLeft = sourceModel()->data(left, Qt::DisplayRole).toString();
            QString rangeRight = sourceModel()->data(right, Qt::DisplayRole).toString();

            //Splitting each range on '-' to obtain start and end page numbers
            int leftStart = rangeLeft.split('-').first().trimmed().toInt();
            int leftLast = rangeLeft.split('-').last().trimmed().toInt();
            int rightStart = rangeRight.split('-').first().trimmed().toInt();
            int rightLast = rangeRight.split('-').last().trimmed().toInt();

            //Primary sort compares start pages, and secondary sort compares last pages (if start pages are equal)
            if (leftStart != rightStart) {
                return leftStart < rightStart;
            }
            return leftLast < rightLast;
        }
        return QSortFilterProxyModel::lessThan(left, right); //Remaining columns compared against Qt::DisplayRole string
    }
};

//MainWindow is main application window
class MainWindow : public QMainWindow
{
    Q_OBJECT //Class uses Qt features

public:
    //Constructor
    //default values (arguments) used for when no arguments are supplied
    MainWindow(QWidget *parent = nullptr);

    //Destructor
    //called when window destroyed
    ~MainWindow();

private slots:
    void onAddClicked(); //Called when 'Add' button clicked
    void onRemoveClicked(); //Called when 'Remove' button clicked
    void onFindClicked(); //Called when 'Find' button clicked
    void onResetClicked(); //Called when 'Reset' button clicked

private:
    //Private helper functions
    //Builds left panel
    void setupLeftPanel();

    //Builds right panel
    void setupRightPanel();

    //Connects buttons to their slots
    void connectSignals();

    //Returns present year
    int currentYear() const;

    //Member variables
    //Model for data storage
    QStandardItemModel *memb_model;

    //Sorts and filters rows of model in the view
    JournalSortProxyModel *memb_proxyModel;

    //The view (display)
    QTableView *memb_tableView;

    //Left panel input fields
    QLineEdit *memb_authorEdit; //Author name text box
    QSpinBox *memb_yearSpin; //Year spinner
    QLineEdit *memb_titleEdit; //Article title text box
    QLineEdit *memb_journalEdit; //Journal name text box
    QSpinBox *memb_volumeSpin; //Volume number spinner
    QSpinBox *memb_issueSpin; //Issue number spinner
    QLineEdit *memb_pagesEdit; //Page number text box

    //Buttons
    QPushButton *memb_addButton;
    QPushButton *memb_removeButton;

    //Search/filter area
    QLineEdit *memb_filterEdit; //Search term text box
    QComboBox *memb_filterCombo; //Column search choice drop-down
    QPushButton *memb_findButton;
    QPushButton *memb_resetButton;

    //Validators for each specific text field
    QRegularExpressionValidator *memb_authorValidator; //For validating name of author
    QRegularExpressionValidator *memb_titleValidator; //For validating article title
    QRegularExpressionValidator *memb_journalValidator; //For validating journal name
    QRegularExpressionValidator *memb_pagesValidator; //For validating page range
};
#endif // MAINWINDOW_H
