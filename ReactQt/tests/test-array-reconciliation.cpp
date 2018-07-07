/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QSignalSpy>
#include <QTest>
#include <QTimer>
#include <QtQuick/QQuickView>

#include "reacttestcase.h"
#include "redbox.h"

class TestArrayReconciliation : public ReactTestCase {
    Q_OBJECT

private slots:
    virtual void initTestCase() override;
    CLEANUP_TEST_CASE_DEFAULT(ReactTestCase)

    void testComponentsArrayFirstElementInsert();
    void testComponentsArrayLastElementDelete();

private:
    QTimer* clickDelayTimer = nullptr;
    QQuickItem* topView = nullptr;
    const int INITIAL_ITEMS_COUNT = 3;
    const int CLICKED_ITEMS_COUNT = 2;

    void validateComponentsCount(const int expectedItemsCount, const QString& errorMsg);
};

void TestArrayReconciliation::initTestCase() {
    ReactTestCase::initTestCase();

    clickDelayTimer = new QTimer(this);
    clickDelayTimer->setSingleShot(true);
    clickDelayTimer->setInterval(1000);

    loadQML(QUrl("qrc:/TestArrayReconciliation.qml"));
    waitAndVerifyJsAppStarted();
    showView();
}

void TestArrayReconciliation::validateComponentsCount(const int expectedItemsCount, const QString& errorMsg) {
    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");
    waitAndVerifyCondition([=]() { return topView->childItems().size() == expectedItemsCount; }, errorMsg);
}

void TestArrayReconciliation::testComponentsArrayFirstElementInsert() {
    topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QCOMPARE(topView->childItems().size(), INITIAL_ITEMS_COUNT);

    QQuickItem* textInput = topView->childItems().at(0);
    QCOMPARE(valueOfControlProperty(textInput, "p_title").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "p_title").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "p_title").toString(), QString("ThirdButton"));

    clickItem(textInput);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 1st click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 2nd click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 3rd click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 4th click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(0), "p_title").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "p_title").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "p_title").toString(), QString("ThirdButton"));
}

void TestArrayReconciliation::testComponentsArrayLastElementDelete() {
    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");

    loadJSBundle("TestArrayReconciliationDeleteLast", "ReactQt/tests/JS/TestArrayReconciliationDeleteLast");

    waitAndVerifyJsAppStarted();
    showView();

    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");

    topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QCOMPARE(topView->childItems().size(), INITIAL_ITEMS_COUNT);

    QQuickItem* textInput = topView->childItems().at(0);
    QCOMPARE(valueOfControlProperty(textInput, "p_title").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "p_title").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "p_title").toString(), QString("ThirdButton"));

    clickItem(textInput);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 1st click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 2nd click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 3rd click");

    textInput = topView->childItems().at(0);
    clickItem(textInput);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 4th click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(0), "p_title").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "p_title").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "p_title").toString(), QString("ThirdButton"));
}

QTEST_MAIN(TestArrayReconciliation)
#include "test-array-reconciliation.moc"