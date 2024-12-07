// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEBPACKAGE_H
#define DEBPACKAGE_H

#include <QSharedPointer>

#include <QApt/DebFile>

#include "manager/PackageDependsStatus.h"

namespace Compatible {
class CompPkgInfo;
};  // namespace Compatible

namespace Deb {

// TODO: Use DebPackage::Ptr to replace the scattered package data in the DebListModel / PackageAnalyzer
class DebPackage
{
public:
    using Ptr = QSharedPointer<DebPackage>;

    explicit DebPackage(const QString &debFilePath);
    ~DebPackage() = default;

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool fileExists() const;
    void markNotExists();

    [[nodiscard]] QString filePath() const;
    [[nodiscard]] QByteArray md5();
    [[nodiscard]] const QSharedPointer<QApt::DebFile> &debInfo() const;

    // direct access
    [[nodiscard]] PackageDependsStatus &dependsStatus();

    // depends info
    bool containRemovePackages() const;
    void setMarkedPackages(const QStringList &installDepends);
    QStringList removePackages() const;

    // error
    void setError(int code, const QString &string);
    [[nodiscard]] int errorCode() const;
    [[nodiscard]] QString errorString() const;

    // for compatible mode
    const QSharedPointer<Compatible::CompPkgInfo> &compatible();

private:
    bool m_exists{true};  // file exists

    QByteArray m_md5;  // package's md5 sum
    QSharedPointer<QApt::DebFile> m_debFilePtr;

    PackageDependsStatus m_dependsStatus;

    QStringList m_removePackages;

    int m_errorCode{0};  // sa Pkg::ErrorCode and QApt::ErrorCode
    QString m_errorString;

    QSharedPointer<Compatible::CompPkgInfo> m_compInfoPtr;

    enum TemplatesState {
        UnknownTemplates,
        ContainTemplates,
        NoTemplates,
    };
    TemplatesState m_templatesState{UnknownTemplates};

    Q_DISABLE_COPY(DebPackage)
};

};  // namespace Deb

#endif  // DEBPACKAGE_H