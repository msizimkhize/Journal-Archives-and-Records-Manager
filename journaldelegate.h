#ifndef JOURNALDELEGATE_H
#define JOURNALDELEGATE_H

#include <QStyledItemDelegate> //For custom delegates

class JournalDelegate : public QStyledItemDelegate
{
    Q_OBJECT //Class uses Qt features

public:
    explicit JournalDelegate(QObject *parent = nullptr);

    //paint() called whenever drawing a cells
    //It is overridden so the background colour of a row can be changed
    void paint (QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    //createEditor() called whenever cell is double-clicked for editing
    //Overridden so 'Year' column uses QSpinBox instead of text box
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    //setEditor() fills editor with the current value of the cell
    void setEditorData (QWidget *editor,
                       const QModelIndex &index) const override;

    //setModel() saves the changes made to the model
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:
    //This helper returns the current year from the system clock
    int currentYear() const;
};

#endif // JOURNALDELEGATE_H
