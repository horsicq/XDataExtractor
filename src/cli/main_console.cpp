/* Copyright (c) 2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include "../global.h"
#include "xextractor.h"
#include "xoptions.h"

// qint32 handleFile(const QString &sFileName, XExtractor::OPTIONS *pExtractorOptions, qint32 nTotal)
// {
//     return 1;
// }

XOptions::CR ScanFiles(QList<QString> *pListArgs, XExtractor::OPTIONS *pExtractorOptions)
{
    XOptions::CR result = XOptions::CR_SUCCESS;

    // TODO check all parameters

    qint32 nNumberOfFiles = 0;

    XOptions::printConsole("Get number of files", Qt::red);

    for (qint32 i = 0; i < pListArgs->count(); i++) {
        QString sFileName = pListArgs->at(i);

        if (QFileInfo::exists(sFileName)) {
            nNumberOfFiles += XBinary::getNumberOfFiles(sFileName, true, 0);
        } else {
            printf("Cannot find: %s\n", sFileName.toUtf8().data());

            result = XOptions::CR_CANNOTFINDFILE;
            break;
        }
    }

    if (result != XOptions::CR_SUCCESS) {
        return result;
    }


    bool bShowFileName = nNumberOfFiles > 1;

    for (qint32 i = 0; i < pListArgs->count(); i++) {
        QString sFileName = pListArgs->at(i);
    }

    // for (qint32 i = 0; i < nNumberOfFiles; i++) {
    //     QString sFileName = listFileNames.at(i);

    //     if (bShowFileName) {
    //         printf("%s:\n", sFileName.toUtf8().data());
    //     }

    //     XScanEngine::SCAN_RESULT scanResult = SpecAbstract().scanFile(sFileName, pScanOptions);

    //     ScanItemModel model(pScanOptions, &(scanResult.listRecords), 1);

    //     XBinary::FORMATTYPE formatType = XBinary::FORMATTYPE_TEXT;

    //     if (pScanOptions->bResultAsCSV) formatType = XBinary::FORMATTYPE_CSV;
    //     else if (pScanOptions->bResultAsJSON) formatType = XBinary::FORMATTYPE_JSON;
    //     else if (pScanOptions->bResultAsTSV) formatType = XBinary::FORMATTYPE_TSV;
    //     else if (pScanOptions->bResultAsXML) formatType = XBinary::FORMATTYPE_XML;
    //     else if (pScanOptions->bResultAsPlainText) formatType = XBinary::FORMATTYPE_PLAINTEXT;

    //     if (formatType != XBinary::FORMATTYPE_TEXT) {
    //         printf("%s\n", model.toString(formatType).toUtf8().data());
    //     } else {
    //         // Colored text
    //         model.coloredOutput();
    //     }
    // }

    return result;
}

int main(int argc, char *argv[])
{
    qint32 nResult = XOptions::CR_SUCCESS;

    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2025 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("target", "The file or directory to open.");

    QCommandLineOption clExtractorMode(QStringList() << "m"
                                                     << "mode",
                                       "Extractor mode <modename>", "modename");

    QCommandLineOption clOutputDirectory(QStringList() << "o"
                                                     << "output",
                                       "Output <directory>", "directory");
    // QCommandLineOption clDeepScan(QStringList() << "d"
    //                                             << "deepscan",
    //                               "Deep scan.");
    // QCommandLineOption clHeuristicScan(QStringList() << "u"
    //                                                  << "heuristicscan",
    //                                    "Heuristic scan.");
    // QCommandLineOption clAggresiveScan(QStringList() << "g"
    //                                                  << "aggressivecscan",
    //                                    "Aggressive scan.");
    // QCommandLineOption clVerbose(QStringList() << "b"
    //                                            << "verbose",
    //                              "Verbose.");
    // QCommandLineOption clAllTypesScan(QStringList() << "a"
    //                                                 << "alltypes",
    //                                   "Scan all types.");
    // QCommandLineOption clFormatResult(QStringList() << "f"
    //                                                 << "format",
    //                                   "Format result.");
    // QCommandLineOption clResultAsXml(QStringList() << "x"
    //                                                << "xml",
    //                                  "Result as XML.");
    // QCommandLineOption clResultAsJson(QStringList() << "j"
    //                                                 << "json",
    //                                   "Result as JSON.");
    // QCommandLineOption clResultAsCSV(QStringList() << "c"
    //                                                << "csv",
    //                                  "Result as CSV.");
    // QCommandLineOption clResultAsTSV(QStringList() << "t"
    //                                                << "tsv",
    //                                  "Result as TSV.");
    // QCommandLineOption clResultAsPlainText(QStringList() << "p"
    //                                                      << "plaintext",
    //                                        "Result as Plain Text.");

    parser.addOption(clExtractorMode);
    parser.addOption(clOutputDirectory);
    // parser.addOption(clHeuristicScan);
    // parser.addOption(clAggresiveScan);
    // parser.addOption(clVerbose);
    // parser.addOption(clAllTypesScan);
    // parser.addOption(clFormatResult);
    // parser.addOption(clResultAsXml);
    // parser.addOption(clResultAsJson);
    // parser.addOption(clResultAsCSV);
    // parser.addOption(clResultAsTSV);
    // parser.addOption(clResultAsPlainText);

    parser.process(app);

    QList<QString> listArgs = parser.positionalArguments();

    XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();

    if (parser.isSet(clExtractorMode)) {
        QString sExtractorMode = parser.value(clExtractorMode);
        extractorOptions.emode = XBinary::ftStringToExtractorMode(sExtractorMode);
    }

    if (parser.isSet(clOutputDirectory)) {
        QString sOutputDirectory = parser.value(clOutputDirectory);
        extractorOptions.sOutputDirectory = sOutputDirectory;
    }

    // scanOptions.bShowType = true;
    // scanOptions.bShowInfo = true;
    // scanOptions.bShowVersion = true;
    // scanOptions.bIsRecursiveScan = parser.isSet(clRecursiveScan);
    // scanOptions.bIsDeepScan = parser.isSet(clDeepScan);
    // scanOptions.bIsHeuristicScan = parser.isSet(clHeuristicScan);
    // scanOptions.bIsVerbose = parser.isSet(clVerbose);
    // scanOptions.bIsAggressiveScan = parser.isSet(clAggresiveScan);
    // scanOptions.bIsAllTypesScan = parser.isSet(clAllTypesScan);
    // scanOptions.bFormatResult = parser.isSet(clFormatResult);
    // scanOptions.bResultAsXML = parser.isSet(clResultAsXml);
    // scanOptions.bResultAsJSON = parser.isSet(clResultAsJson);
    // scanOptions.bResultAsCSV = parser.isSet(clResultAsCSV);
    // scanOptions.bResultAsTSV = parser.isSet(clResultAsTSV);
    // scanOptions.bResultAsPlainText = parser.isSet(clResultAsPlainText);
    // scanOptions.nBufferSize = 2 * 1024 * 1024;  // TODO Check
    // scanOptions.bIsHighlight = true;

    if (listArgs.count()) {
        nResult = ScanFiles(&listArgs, &extractorOptions);
    } else {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return nResult;
}
