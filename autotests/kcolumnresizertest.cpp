/* This file is part of the KDE frameworks
 *
 * Copyright (c) 2014 Aurélien Gâteau <agateau@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include <kcolumnresizertest.h>

#include <KColumnResizer>

#include <QFile>
#include <QFormLayout>
#include <QLabel>
#include <QScopedPointer>
#include <QUiLoader>
#include <QtTest>

QTEST_MAIN(KColumnResizerTest)

static QWidget *loadUi(const QString &name)
{
    QUiLoader loader;
    QFile file(name);
    file.open(QFile::ReadOnly);
    return loader.load(&file);
}

void KColumnResizerTest::test_data()
{
    QTest::addColumn<QString>("uiFilePath");
    QTest::newRow("forms") << QFINDTESTDATA("kcolumnresizertest-forms.ui");
    QTest::newRow("grids") << QFINDTESTDATA("kcolumnresizertest-grids.ui");
    QTest::newRow("grid-and-form") << QFINDTESTDATA("kcolumnresizertest-grid-and-form.ui");
}

void KColumnResizerTest::test()
{
    // This test checks the x coordinate of the widgets from the column
    // immediately after the resized column, rather than checking the width of
    // the resized column itself because checking the width of the column cannot
    // be done the same way for all layout types.
    QFETCH(QString, uiFilePath);
    QScopedPointer<QWidget> parent(loadUi(uiFilePath));
    QVERIFY(parent.data());

    auto layout1 = parent->findChild<QLayout *>(QStringLiteral("layout1"));
    auto layout2 = parent->findChild<QLayout *>(QStringLiteral("layout2"));
    auto widget1 = parent->findChild<QWidget *>(QStringLiteral("widget1"));
    auto widget2 = parent->findChild<QWidget *>(QStringLiteral("widget2"));
    QVERIFY(layout1);
    QVERIFY(layout2);
    QVERIFY(widget1);
    QVERIFY(widget2);

    // Show the widget so that geometries are updated
    parent->show();

    int widget1x = widget1->x();
    int widget2x = widget2->x();
    QVERIFY(widget1x < widget2x);

    auto resizer = new KColumnResizer(parent.data());
    resizer->addWidgetsFromLayout(layout1);
    resizer->addWidgetsFromLayout(layout2);

    // Wait for resizer to do the work
    QCoreApplication::processEvents();
    // Now wait for the layout change to propagate
    QCoreApplication::processEvents();

    QCOMPARE(widget1->x(), widget2x);
    QCOMPARE(widget2->x(), widget2x);
}
