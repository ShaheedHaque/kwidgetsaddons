/* This file is part of the KDE libraries

   Copyright (C) 2005 Joseph Wenninger <jowenn@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KCHARSELECT_P_H
#define KCHARSELECT_P_H

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QTextDocument>
#include <QFont>
#include <QApplication>
#include <QMimeData>
#include <QTableView>

#include "kcharselectdata_p.h"

class KCharSelectTablePrivate;

/**
 * @short Character selection table
 *
 * A table widget which displays the characters of a font. Internally
 * used by KCharSelect. See the KCharSelect documentation for further
 * details.
 *
 * @author Reginald Stadlbauer <reggie@kde.org>
 * @author Daniel Laidig <d.laidig@gmx.de>
 */

class KCharSelectTable : public QTableView
{
    Q_OBJECT

public:
    /**
     * Constructor. Using @p _font, draw a table of chars.
     * @sa setContents
     */
    KCharSelectTable(QWidget *parent, const QFont &_font);

    ~KCharSelectTable();

    /**
     * Reimplemented.
     */
    virtual void resizeEvent(QResizeEvent *);

    /** Set the font to be displayed to @p _font . */
    void setFont(const QFont &_font);

    /** Set the highlighted character to @p c . */
    void setChar(QChar c);
    /** Set the contents of the table to @p chars . */
    void setContents(const QVector<QChar>& chars);

    /** @return Currently highlighted character. */
    QChar chr();

    /**
     * Returns the currently displayed font.
     */
    QFont font() const;

    /**
     * Returns a list of currently displayed characters.
     */
    QVector<QChar> displayedChars() const;

    /**
     * Reimplemented.
     */
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) Q_DECL_OVERRIDE;

protected:
    /**
     * Reimplemented.
     */
    virtual void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

Q_SIGNALS:
    /** Emitted to indicate that character @p c is activated (such as by double-clicking it). */
    void activated(QChar c);
    void focusItemChanged(QChar c);
    void showCharRequested(QChar c);

private:
    Q_PRIVATE_SLOT(d, void _k_slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected))
    Q_PRIVATE_SLOT(d, void _k_resizeCells())
    Q_PRIVATE_SLOT(d, void _k_doubleClicked(const QModelIndex &index))

private:
    friend class KCharSelectTablePrivate;
    KCharSelectTablePrivate *const d;

    Q_DISABLE_COPY(KCharSelectTable)
};

// NO D-Pointer needed, private internal class, no public API

class KCharSelectItemModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    KCharSelectItemModel(QVector<QChar> chars, const QFont &font, QObject *parent): QAbstractTableModel(parent), m_chars(chars), m_font(font)
    {
        if (chars.count()) {
            m_columns = chars.count();
        } else {
            m_columns = 1;
        }
    }

    enum internalRoles {CharacterRole = Qt::UserRole};
    int rowCount(const QModelIndex & = QModelIndex()) const
    {
        if (m_chars.count() % m_columns == 0) {
            return m_chars.count() / m_columns;
        } else {
            return m_chars.count() / m_columns + 1;
        }
    }
    int columnCount(const QModelIndex & = QModelIndex()) const
    {
        return m_columns;
    }

    void setFont(const QFont &font)
    {
        beginResetModel();
        m_font = font;
        endResetModel();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        int pos = m_columns * (index.row()) + index.column();
        if (pos >= m_chars.size() || index.row() < 0 || index.column() < 0) {
            return Qt::ItemIsDropEnabled;
        }
        return (Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QMimeData *mimeData(const QModelIndexList &indexes) const
    {
        if (indexes.size() != 1) {
            return 0;
        }
        QMimeData *mimeData = new QMimeData();
        mimeData->setText(data(indexes[0], CharacterRole).toString());
        return mimeData;
    }
    Qt::DropActions supportedDropActions() const
    {
        return Qt::CopyAction;
    }
    QStringList mimeTypes() const
    {
        QStringList types;
        types << QStringLiteral("text/plain");
        return types;
    }
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    void setColumnCount(int columns);

    QVector<QChar> chars() const { return m_chars; }
private:
    QVector<QChar> m_chars;
    QFont m_font;
    int m_columns;

Q_SIGNALS:
    void showCharRequested(QChar c);

};
#endif // KCHARSELECT_P_H
