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
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "../global.h"
#include "xextractor.h"
#include "xmodel_extractor.h"
#include "xoptions.h"

qint32 handleFile(const QString &sFileName, XExtractor::OPTIONS *pExtractorOptions, qint32 nTotal)
{
    qint32 nResult = 0;

    Q_UNUSED(nTotal)

    QFileInfo fi(sFileName);

    if (fi.isFile()) {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            XExtractor::DATA extractorData = {};
            extractorData.options = *pExtractorOptions;
            XBinary::FT fileType = extractorData.options.fileType;

            if (fileType == XBinary::FT_UNKNOWN) {
                QSet<XBinary::FT> stFileTypes = XFormats::getFileTypes(&file, true);
                fileType = XBinary::_getPrefFileType(&stFileTypes);
            }

            extractorData.memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, &file);
            XExtractor extractor;
            extractor.setData(&file, &extractorData, nullptr);
            extractor.process();

            if (extractorData.options.bShowList) {
                XModel_Extractor model(&extractorData);
                model.adjustColumnsToContent(true);
                XOptions::printModel(&model);
            }

            file.close();
        }

        nResult++;
    } else if (fi.isDir()) {
        QDir dir(sFileName);

        QFileInfoList eil = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

        const qint32 nNumberOfFiles = eil.count();

        for (qint32 i = 0; i < nNumberOfFiles; i++) {
            nResult += handleFile(eil.at(i).absoluteFilePath(), pExtractorOptions, nTotal);
        }
    }

    return nResult;
}

XOptions::CR ScanFiles(QList<QString> *pListArgs, XExtractor::OPTIONS *pExtractorOptions)
{
    XOptions::CR result = XOptions::CR_SUCCESS;

    // TODO check all parameters

    qint32 nNumberOfFiles = 0;

    qint32 nCount = pListArgs->count();

    for (qint32 i = 0; i < nCount; i++) {
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

    for (qint32 i = 0; i < nCount; i++) {
        QString sFileName = pListArgs->at(i);

        if (QFileInfo::exists(sFileName)) {
            handleFile(sFileName, pExtractorOptions, nNumberOfFiles);
        }
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

    QCommandLineOption clList(QStringList() << "l"
                                            << "list");
    clList.setDescription("Show result as list.");

    QCommandLineOption clExtract(QStringList() << "x"
                                               << "extract");
    clExtract.setDescription("Extract data.");

    // QCommandLineOption clAll(QStringList() << "A"
    //                                         << "all");
    // clAll.setDescription("Extract all types");

    QCommandLineOption clExtractorMode(QStringList() << "m"
                                                     << "mode");
    clExtractorMode.setDefaultValue("HEURISTIC");
    clExtractorMode.setValueName("RAW, FORMAT, HEURISTIC, UNPACK");
    clExtractorMode.setDescription("Set extractor mode. Default is HEURISTIC.");

    QCommandLineOption clOutputDirectory(QStringList() << "o"
                                                       << "output");
    clOutputDirectory.setDefaultValue(".");
    clOutputDirectory.setValueName("directory");
    clOutputDirectory.setDescription("Set output directory. Default is current directory.");

    parser.addPositionalArgument("file", "The file to extract from.");
    parser.addPositionalArgument("directory", "The directory to extract from.");

    parser.addOption(clList);
    parser.addOption(clExtract);
    // parser.addOption(clAll);
    parser.addOption(clExtractorMode);
    parser.addOption(clOutputDirectory);

    parser.process(app);

    QList<QString> listArgs = parser.positionalArguments();

    XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();

    if (parser.isSet(clList)) {
        extractorOptions.bShowList = true;
    }

    if (parser.isSet(clExtract)) {
        extractorOptions.bExtract = true;
    }

    extractorOptions.bAllTypes = true;
    extractorOptions.bAnalyze = true;

    {
        QString sExtractorMode = parser.value(clExtractorMode);
        extractorOptions.emode = XExtractor::ftStringToExtractorMode(sExtractorMode);
    }

    // Update available file types to match selected mode
    extractorOptions.listFileTypes = XExtractor::getAvailableFileTypes(extractorOptions.emode);

    if (parser.isSet(clOutputDirectory)) {
        QString sOutputDirectory = parser.value(clOutputDirectory);
        extractorOptions.sOutputDirectory = sOutputDirectory;
    }

    // Ensure output directory exists when extracting
    if (extractorOptions.bExtract) {
        if (!extractorOptions.sOutputDirectory.isEmpty()) {
            QDir().mkpath(extractorOptions.sOutputDirectory);
        }
    }

    if (listArgs.count()) {
        nResult = ScanFiles(&listArgs, &extractorOptions);
    } else {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return nResult;
}
