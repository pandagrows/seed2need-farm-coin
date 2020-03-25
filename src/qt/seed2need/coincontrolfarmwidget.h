// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINCONTROLFARMWIDGET_H
#define COINCONTROLFARMWIDGET_H

#include <QDialog>

namespace Ui {
class CoinControlFarmWidget;
}

class CoinControlFarmWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CoinControlFarmWidget(QWidget *parent = nullptr);
    ~CoinControlFarmWidget();

private:
    Ui::CoinControlFarmWidget *ui;
};

#endif // COINCONTROLFARMWIDGET_H
