// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZFARMCONTROLDIALOG_H
#define ZFARMCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zfarm/zerocoin.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZFarmControlDialog;
}

class CZFarmControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZFarmControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZFarmControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZFarmControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZFarmControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZFarmControlDialog(QWidget *parent);
    ~ZFarmControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZFarmControlDialog *ui;
    WalletModel* model;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZFarmControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZFARMCONTROLDIALOG_H
