#include "journaldelegate.h"
#include <QPainter> //Drawing cell and filling background
#include <QSpinBox> //Manual numeral entry
#include <QMessageBox> //For popup message
#include <QDate> //For obtaining year
#include <QLineEdit> //Text box
#include <QRegularExpressionValidator> //Checks text matches pattern defined by regex
#include <QRegularExpression> //The validator uses this pattern language

#include <QAbstractItemModel> //Base class for Qt models

//Column index for year
static const int COLUMN_YEAR = 1;

//Column index for author
static const int COLUMN_AUTHOR = 0;

//Column index for title
static const int COLUMN_TITLE = 2;

//Column index for journal
static const int COLUMN_JOURNAL = 3;

//Column index for pages
static const int COLUMN_PAGES = 6;

//Constructor (calls parent constructor)
JournalDelegate::JournalDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

//Returns the current year
int JournalDelegate::currentYear() const {
    return QDate::currentDate().year();
}

//Drawing individual cell and painting background
void JournalDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {

    //Getting the year the article of the row was published

    //Getting the row of the column as an integer
    int year = index.sibling(index.row(), COLUMN_YEAR).data().toInt();

    int present = currentYear();
    int articleAge = present - year; //Gives how many years ago it was published

    //The background colour is determined by the article age
    QColor backgroundColour; //Presently, no colour set

    if (articleAge > 10) {
        backgroundColour = QColor(250, 157, 157); //background colour (highlight) set to red
    }
    else if (articleAge>=0 && articleAge <= 5) {
        backgroundColour = QColor(142, 250, 127); //background colour (highlight) set to green
    }

    //If articleAge is between 6-10, backgroundColour remains invalid (unset)

    //If backgroundColour is set, the background must be filled
    if (backgroundColour.isValid()) {
        //Current painter settings saved, coloured rectangle drawn, and then previous settings restored
        //(such that other cells aren't affected)
        painter->save();
        painter->fillRect(option.rect, backgroundColour);
        painter->restore();
    }

    //Selection highlight drawn on top of background colour (so row selection is visible)
    QStyleOptionViewItem opt = option;
    if (backgroundColour.isValid()) {
        //Idea is ensuring Qt doesn't overwrite colour, whilst keeping the selection highlight
        opt.backgroundBrush = QBrush(backgroundColour);
    }

    //The parent class draws the text and selection highlight.
    //Only the background was changed.
    QStyledItemDelegate::paint(painter, opt, index);
}

//Type of widget used for editing cell upon double-click handled.
//QSpinbox is returned for Year column, and QLineEdit for Pages column. Parent class handles the rest.
QWidget *JournalDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
    if (index.column() == COLUMN_YEAR) {
        QSpinBox *spin = new QSpinBox(parent);//A number spinner is created for Year
        spin->setMinimum(1600); //Oldest articles are from the 17th Century
        spin->setMaximum(currentYear()); //Cannot be later than the current year
        spin->setGroupSeparatorShown(false); //Removes comma after millenium
        return spin;
    }

    //For author column. Validator accepts only letters, hyphens and spaces
    if (index.column() == COLUMN_AUTHOR) {
        QLineEdit *editor = new QLineEdit(parent); //A textbox is created for Author
        editor->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-z]+([ \\-][A-Za-z]+)*$"),
                                                             editor //Parent object
                                                             ));
        return editor;
    }

    //For title column. Validator accepts only number or letter words, not a mixture (e.g. 3D invalid)
    if (index.column() == COLUMN_TITLE) {
        QLineEdit *editor = new QLineEdit(parent); //A textbox is created for Title
        editor->setValidator(new QRegularExpressionValidator(QRegularExpression("^([A-Za-z]+|[0-9]+)([ ]([A-Za-z]+|[0-9]+))*$"),
                                                             editor //Parent object
                                                             ));
        return editor;
    }

    //For journal column. Validator accepts only number or letter words, not a mixture (e.g. 3D invalid)
    if (index.column() == COLUMN_JOURNAL) {
        QLineEdit *editor = new QLineEdit(parent); //A textbox is created for Journal
        editor->setValidator(new QRegularExpressionValidator(QRegularExpression("^([A-Za-z]+|[0-9]+)([ ]([A-Za-z]+|[0-9]+))*$"),
                                                             editor //Parent object
                                                             ));
        return editor;
    }

    //For pages column. Validator accepts only form '100-200' or '100 - 200'
    if (index.column() == COLUMN_PAGES) {
        QLineEdit *editor = new QLineEdit(parent); //A textbox is created for Pages
        editor->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d+([ ]-[ ]|-)\\d+$"),
                                                             editor //Parent object
                                                             ));
        return editor;
    }

    //QLineEdit (default editor) used for all other columns
    return QStyledItemDelegate::createEditor(parent, option, index);
}

//setEditorData called immediately after editor is created.
//Editor filled with the current value of the cell, so current value visible to user before editing.
void JournalDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (index.column() == COLUMN_YEAR) {
        QSpinBox *spin = qobject_cast<QSpinBox *>(editor);
        if (spin) {
            //Putting year currently in cell into spinner
            spin->setValue(index.data().toInt());
        }

        return;
    }

    //For all other columns, the parent handles setting the editor widget's value using the model's data
    QStyledItemDelegate::setEditorData(editor, index);
}

//setModelData()
//New value saved to model after user finishes editing.
//User cannot enter a year later than current year.
//Input of validated string columns only saved if it is fully acceptable.
void JournalDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (index.column() == COLUMN_YEAR) {
        QSpinBox *spin = qobject_cast<QSpinBox *>(editor);
        if (spin) {
            //New year is valid. It is saved to the model (cell) as an integer
            model->setData(index, spin->value(), Qt::EditRole);
        }

        return;
    }

    if (index.column() == COLUMN_AUTHOR ||
        index.column() == COLUMN_TITLE ||
        index.column() == COLUMN_PAGES ||
        index.column() == COLUMN_JOURNAL) {

        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if (lineEdit) {
            //Trimming string
            QString trimmed = lineEdit->text().trimmed();

            //Trimmed string validated directly against validator's regex
            const QRegularExpressionValidator *revl = qobject_cast<const QRegularExpressionValidator *>(lineEdit->validator());
            bool valid = false;
            if (revl) {
                int pos = 0;
                QString copy = trimmed;
                valid = (revl->validate(copy, pos) == QValidator::Acceptable);
            }

            if (!valid) {
                QMessageBox::warning(
                    nullptr, //Parent widget
                    "Input Invalid", //Title of message box
                    "The input entered in this field is not valid.\nChanges have not been saved."
                    );
                return; //model->setData() not called. Previous value remains.
            }

            if (index.column() == COLUMN_PAGES) {
                QStringList pageSet = trimmed.split('-'); //Containing segments with pages
                int startPage = pageSet.first().trimmed().toInt(); //First page
                int lastPage = pageSet.last().trimmed().toInt(); //Last page

                if (lastPage < startPage) { //Under condition of page range being invalid
                    QMessageBox::warning(
                        nullptr, //Parent widget
                        "Pages Invalid", //Title of message box
                        "The first page number may not be greater than the second."
                        );
                    return; //Page range change not saved
                }
                //When both format and page order are valid
                model->setData(index, trimmed, Qt::DisplayRole); //Stoirng visible page range
                model->setData(index, startPage, Qt::UserRole); //Storing the start page as integer
            }
            else {
                model->setData(index, trimmed, Qt::EditRole);
            }
        }

        return;
    }

    //For the rest of the columns, parent handles saving to the model
    QStyledItemDelegate::setModelData(editor, model, index);
}

