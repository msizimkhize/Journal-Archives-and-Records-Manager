#include "mainwindow.h"
#include "journaldelegate.h"

//Qt widgets used
#include <QHBoxLayout> //Arranges widgets horizontally
#include <QVBoxLayout> //Arranges widgets vertically
#include <QFormLayout> //Neatly arranges label-widget pairs
#include <QHeaderView> //Enables configuration of column headers of table
#include <QMessageBox> //For popup message
#include <QDate> //For obtaining year
#include <QFrame> //Simple container widget to be used as panel
#include <QIntValidator> //For ensuring that pages entry is numerals
#include <QWidget> //Base class for Qt widgets
#include <QRegularExpressionValidator> //Checks text matches pattern defined by regex
#include <QRegularExpression> //The validator uses this pattern language

//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Journals"); //Window title
    resize(1200, 480); //Window default size set
    setMinimumWidth(1200); //Window minimum width set

    //Model created
    //Initially it has 0 columns and 7 (COLUMN_COUNT) columns
    memb_model = new QStandardItemModel(0, COLUMN_COUNT, this);

    //A header label is assigned to each column
    memb_model->setHorizontalHeaderLabels({
        "Author", "Year", "Title", "Journal", "Volume", "Issue", "Pages"
    });

    //Proxy model created, which serves the purpose of:
    //Hiding words (filter) that don't match a search term
    //Sorting rows when user clicks column header
    memb_proxyModel = new JournalSortProxyModel(this); //Custom proxy model, handles sorting Pages as well as all other columns
    memb_proxyModel->setSourceModel(memb_model); //Proxy wraps real model
    memb_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive); //Search ignores case

    //UI panels setup
    setupLeftPanel(); //Creates input widgets
    setupRightPanel(); //Creates table view

    //Left and right panel placed in central widget beside each other
    QWidget *central = new QWidget(this);
    QHBoxLayout *horLayout = new QHBoxLayout(central); //horLayout placed in central widget

    //The setup functions have created the member-variable widgets; they are henceforth arranged

    //The left panel container
    QWidget *leftWidget = new QWidget(this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Author", memb_authorEdit);
    formLayout->addRow("Year", memb_yearSpin);
    formLayout->addRow("Title", memb_titleEdit);
    formLayout->addRow("Journal", memb_journalEdit);
    formLayout->addRow("Volume", memb_volumeSpin);
    formLayout->addRow("Issue", memb_issueSpin);
    formLayout->addRow("Pages", memb_pagesEdit);

    //Creating topLayout container layout
    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addLayout(formLayout);
    topLayout->addWidget(memb_addButton);
    topLayout->addWidget(memb_removeButton);

    //Stretching form section vertically
    topLayout->setStretch(0, 1);

    //Add and Remove buttons below the form
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->addLayout(topLayout);
    leftLayout->addSpacing(75);


    //Search/filter area at bottom left
    leftLayout->addWidget(memb_filterEdit);
    leftLayout->addWidget(memb_filterCombo);

    QHBoxLayout *findRow = new QHBoxLayout();
    findRow->addWidget(memb_findButton);
    findRow->addWidget(memb_resetButton);
    leftLayout->addLayout(findRow);

    //Setting a fixed width for the left panel
    leftWidget->setFixedWidth(310);

    leftLayout->addStretch(); //Pushing bottom section to bottom

    //Adding leftWidget and memb_tableView panels to main horizontal layout
    horLayout->addWidget(leftWidget);
    horLayout->addWidget(memb_tableView);

    setCentralWidget(central);

    //Connecting signals to slots
    connectSignals();

}

//Destructor
MainWindow::~MainWindow()
{}

//Returning current year from system clock
int MainWindow::currentYear() const {
    return QDate::currentDate().year();
}

//Creating input widgets
void MainWindow::setupLeftPanel() {

    //Validators

    //Author validator
    memb_authorValidator = new QRegularExpressionValidator(
        QRegularExpression("^[A-Za-z]+([ \\-][A-Za-z]+)*$"),
        //One+ letter(s) for first word/intial, followed by none or more groups of: space/- then letter(s)
        this //Parent widget
        );

    //Journal validator
    QRegularExpression journalRegex("^([A-Za-z]+|[0-9]+)([ ]([A-Za-z]+|[0-9]+))*$"
        //One word (either letters/numbers), followed by none or more groups of: spaces(s) then another word
        );
    memb_journalValidator = new QRegularExpressionValidator(journalRegex,
                                                            this //Parent widget
                                                            );

    //Title validator
    QRegularExpression titleRegex("^([A-Za-z]+|[0-9]+)([ ]([A-Za-z]+|[0-9]+))*$"
        //One word (either letters/numbers), followed by none or more groups of: spaces(s) then another word
        );
    memb_titleValidator = new QRegularExpressionValidator(titleRegex,
                                                          this //Parent widget
                                                          );

    //Pages validator
    memb_pagesValidator = new QRegularExpressionValidator(
        QRegularExpression("^\\d+([ ]-[ ]|-)\\d+$"),
        //One+ number(s), followed by optional spaces, then a hyphen, then an optional spaces, followed by one+ number(s)
        this
        );

    //Spin boxes and line edits
    memb_authorEdit = new QLineEdit(this);
    memb_authorEdit->setValidator(memb_authorValidator); //Attaching memb_authorValidator

    memb_yearSpin = new QSpinBox(this);
    memb_yearSpin->setMinimum(1600);
    memb_yearSpin->setMaximum(currentYear()); //Latest year is current year
    memb_yearSpin->setValue(1900);

    memb_titleEdit = new QLineEdit(this);
    memb_titleEdit->setValidator(memb_titleValidator); //Attaching memb_titleValidator

    memb_journalEdit = new QLineEdit(this);
    memb_journalEdit->setValidator(memb_journalValidator); //Attaching memb_journalValidator

    memb_volumeSpin = new QSpinBox(this);
    memb_volumeSpin->setMinimum(0);
    memb_volumeSpin->setMaximum(9999); //Maximum volume set to 9999

    memb_issueSpin = new QSpinBox(this);
    memb_issueSpin->setMinimum(0);
    memb_issueSpin->setMaximum(9999); //Maximum issue set to 9999

    memb_pagesEdit = new QLineEdit(this);
    memb_pagesEdit->setValidator(memb_pagesValidator); //Attaching memb_pagesValidator
    memb_pagesEdit->setPlaceholderText("e.g. 100-201"); //Instruction for user input

    //Buttons
    memb_addButton = new QPushButton("Add", this);
    memb_removeButton = new QPushButton("Remove", this);

    //Search/filter area
    memb_filterEdit = new QLineEdit(this);
    memb_filterEdit->setPlaceholderText("Enter search term"); //Instruction for user input

    //Search/filter menu for selecting which column to search in
    memb_filterCombo = new QComboBox(this);
    memb_filterCombo->addItem("Author", COLUMN_AUTHOR);
    memb_filterCombo->addItem("Year", COLUMN_YEAR);
    memb_filterCombo->addItem("Title", COLUMN_TITLE);
    memb_filterCombo->addItem("Journal", COLUMN_JOURNAL);
    memb_filterCombo->addItem("Volume", COLUMN_VOLUME);
    memb_filterCombo->addItem("Issue", COLUMN_ISSUE);

    //Buttons
    memb_findButton = new QPushButton("Find", this);
    memb_resetButton = new QPushButton("Reset", this);
}

//Creating and configuring QTableView
void MainWindow::setupRightPanel() {
    memb_tableView = new QTableView(this);

    //Connects table view to proxy model, so that view displays sorted/filtered data
    memb_tableView->setModel(memb_proxyModel);

    //Allowing user to sort column by clicking column header
    memb_tableView->setSortingEnabled(true);
    memb_tableView->horizontalHeader()->setSortIndicatorShown(true);

    //Columns stretch to fit width
    memb_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Making that only whole rows are selected at a time, not individual cells
    memb_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Making that only one row can be selected at a time
    memb_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    //Hiding vertical header - that is the row numbers on the left
    memb_tableView->verticalHeader()->setVisible(false);

    //Attaching the custom delegate so that rows get coloured and year edits validated
    memb_tableView->setItemDelegate(new JournalDelegate(memb_tableView));
}

//Handling signals and slots
void MainWindow::connectSignals() {
    //Running onAddClicked() when memb_addButton is clicked
    connect(memb_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);

    //Running onRemoveClicked() when memb_removeButton is clicked
    connect(memb_removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);

    //Running onResetClicked() when memb_resetButton ic clicked
    connect(memb_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    //Running onFindClicked() when memb_findButton is clicked
    connect(memb_findButton, &QPushButton::clicked, this, &MainWindow::onFindClicked);
}

//onAddClicked() reads all input fields, ensures year is valid, and appends new row to to the bottom of the model
void MainWindow::onAddClicked() {
    //Seeing to it that the year is not later than the current year
    int year = memb_yearSpin->value();

    //Trimming the text fields before running hasAcceptableIput()
    memb_authorEdit->setText(memb_authorEdit->text().trimmed());
    memb_titleEdit->setText(memb_titleEdit->text().trimmed());
    memb_journalEdit->setText(memb_journalEdit->text().trimmed());
    memb_pagesEdit->setText(memb_pagesEdit->text().trimmed());

    //Using hasAcceptableInput() to ensure string fields are not empty and have valid input before allowind Add.
    if(!memb_authorEdit->hasAcceptableInput()) {
        QMessageBox::warning(this, //Parent widget
                            "Author Invalid", //Title of message box
                             "Enter a valid author name.\nOnly letters, hyphens and spaces permissible. (e.g. SZ Mbanjwa-Mhlana)"
                             );
        memb_authorEdit->setFocus(); //Placing cursor in the memb_authorEdit textbox
        return; //Row not added
    }

    if(!memb_titleEdit->hasAcceptableInput()) {
        QMessageBox::warning(this, //Parent widget
                             "Title Invalid", //Title of message box
                             "Enter a valid title.\nOnly letters, numbers and spaces permissible.\nLetters may not be combined with numbers. (e.g. 3D is impermissible)"
                             );
        memb_titleEdit->setFocus(); //Placing cursor in the memb_titleEdit textbox
        return; //Row not added
    }

    if(!memb_journalEdit->hasAcceptableInput()) {
        QMessageBox::warning(this, //Parent widget
                             "Journal Name Invalid", //Title of message box
                             "Enter a valid journal name.\nOnly letters, numbers and spaces permissible.\nLetters may not be combined with numbers. (e.g. 3D is impermissible)"
                             );
        memb_journalEdit->setFocus(); //Placing cursor in the memb_journalEdit textbox
        return; //Row not added
    }

    if(!memb_pagesEdit->hasAcceptableInput()){
        QMessageBox::warning(this, //Parent widget
                             "Pages Invalid", //Title of message box
                             "Enter a valid page number.\nIt must be of form '100-200' or '100 - 200'."
                             );
        memb_pagesEdit->setFocus(); //Placing cursor in the memb_pagesEdit textbox
        return; //Row not added
    }

    //Now, format is valid. Checking that second page is greater than or equal to first page.
    //Page numbers split on '-'
    {
        QStringList pageSegments = memb_pagesEdit->text().split('-'); //String list containing pages
        int startPage = pageSegments.first().trimmed().toInt(); //First page obtained
        int lastPage = pageSegments.last().trimmed().toInt(); //Second page obtained
        if (startPage > lastPage) {
            QMessageBox::warning(this, //Parent widget
                                 "Pages Invalid", //Title of message box
                                 "The first page number may not be greater than the second."
                                 );
            memb_pagesEdit->setFocus(); //Placing cursor in the memb_pagesEdit textbox
            return; //Row not added
        }
    }

    //Building list of QStandardItem objects - one per column, where each QStandardItem is like a single cell in the table
    QList<QStandardItem *> rowItems;

    //Helper lambda creates item uneditable by right click - allows editing by double-clicking cell in table
    auto makeItem = [](const QString &text)->QStandardItem * {
        QStandardItem *item = new QStandardItem(text);
        item->setEditable(true); //Sets editable by double-click
        return item;
    };

    rowItems << makeItem(memb_authorEdit->text().trimmed());

    //Number stored and display text set for Year column
    //Using Qt::UserRole (stores raw integer for correct numeric sorting) and Qt::DisplayRole (stores what is shown to user)
    QStandardItem *yearItem = new QStandardItem();
    yearItem->setData(year, Qt::DisplayRole); //Shown as number in the cell
    yearItem->setData(year, Qt::UserRole); //Stored as integer for sorting
    rowItems << yearItem;

    //Storing title and journal
    rowItems << makeItem(memb_titleEdit->text().trimmed());
    rowItems << makeItem(memb_journalEdit->text().trimmed());

    //Number stored and display text set for Volume column
    //Using Qt::UserRole (stores raw integer for correct numeric sorting) and Qt::DisplayRole (stores what is shown to user)
    QStandardItem *volumeItem = new QStandardItem();
    volumeItem->setData(memb_volumeSpin->value(), Qt::DisplayRole);
    volumeItem->setData(memb_volumeSpin->value(), Qt::UserRole);
    rowItems << volumeItem;

    //Number stored and display text set for Issue column
    //Using Qt::UserRole (stores raw integer for correct numeric sorting) and Qt::DisplayRole (stores what is shown to user)
    QStandardItem *issueItem = new QStandardItem();
    issueItem->setData(memb_issueSpin->value(), Qt::DisplayRole);
    issueItem->setData(memb_issueSpin->value(), Qt::UserRole);
    rowItems << issueItem;

    //Start page (number) stored, and display text (pages range) set for Pages column
    //Using Qt::UserRole (stores raw integer for correct numeric sorting) and Qt::DisplayRole (stores what is shown to user)
    QString pagesColText = memb_pagesEdit->text();
    int startPage = pagesColText.split('-').first().trimmed().toInt();
    QStandardItem *pagesItem = new QStandardItem();
    pagesItem->setData(pagesColText, Qt::DisplayRole);
    pagesItem->setData(startPage, Qt::UserRole);
    rowItems << pagesItem;

    //Appending the row to the end of the model
    memb_model->appendRow(rowItems);

    //Clearing text fields to convinience user entry for following article entry
    memb_authorEdit->clear();
    memb_titleEdit->clear();
    memb_journalEdit->clear();
    memb_pagesEdit->clear();
    memb_volumeSpin->setValue(0);
    memb_issueSpin->setValue(0);
    memb_yearSpin->setValue(1900);
}

//onRemoveClicked() removes selected row in table
void MainWindow::onRemoveClicked() {
    //Acquiring row selected from table view
    QModelIndexList rowSelection = memb_tableView->selectionModel()->selectedRows();

    if (rowSelection.isEmpty()) {
        //If there is nothing selected. Consequently, nothing will be removed.
        QMessageBox::information(this, //Parent widget
                                 "No Row Selected", //Title of pop-up box
                                 "Please select a row to be removed."
                                 );
        return; //Row not removed
    }

    //Mapping first selected row from table view, and mapping it to corresponding row in source model
    QModelIndex proxyIndex = rowSelection.first();
    QModelIndex sourceIndex = memb_proxyModel->mapToSource(proxyIndex);

    //Removing row from the source model
    memb_model->removeRow(sourceIndex.row());
}

//onFindClicked() makes for only rows containing search term for chosen column to be shown
void MainWindow::onFindClicked() {
    //Reading the column user intends to search
    //currentData() returns column number, which was passed to adItem() in setupLeftColumn
    int column = memb_filterCombo->currentData().toInt();

    //Setting the proxy column to be filtered on
    memb_proxyModel->setFilterKeyColumn(column);

    //The search term is obtained and treated as a wildcard pattern
    //* : any number of characters; ? : one character exactly
    //Search term is converted to regex using QRegularExpression::wildcardToRegularExpression(), in order that the proxy can use it
    QString pattern = memb_filterEdit->text().trimmed(); //Text from text box (trimmed)

    if (pattern.isEmpty()) {
        memb_proxyModel->setFilterRegularExpression(QRegularExpression()); //All articles shown (setting empty [invalid] regular expression)
        return;
    }

    QRegularExpression regex(
        QRegularExpression::wildcardToRegularExpression(pattern), //Converting the wildcard expression into a regex
        QRegularExpression::CaseInsensitiveOption //Case insensitive
        );

    memb_proxyModel->setFilterRegularExpression(regex); //Applying the regular expression filter ('regex') to the proxy model (in 'column')
}

//onResetClicked() clears filter so all rows are visible, and clears the search text box
void MainWindow::onResetClicked() {
    memb_filterEdit->clear(); //Clearing search text box

    memb_proxyModel->setFilterRegularExpression(QRegularExpression()); //All articles shown (setting empty [invalid] regular expression)
}
